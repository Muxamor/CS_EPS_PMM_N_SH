#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_iwdg.h"
#include "stm32l4xx_ll_tim.h"
#include "TCA9539.h"
#include "tim_pwm.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
//#include "PBM/pbm_config.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_damage_ctrl.h"
#include "uart_eps_comm.h"
#include "PMM/pmm_sw_cpu.h"
#include  <stdio.h>

/** @brief CPU main checking active CPU flag  (pmm_ptr->Active_CPU) between main and backup CPU.
 * 			In case when (pmm_ptr->Active_CPU) the same value in the Main and Backup CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_CPUm_Check_Active_CPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t error_status = ERROR_N;
    uint8_t read_val_CAN_MUX_pin14 = 2;
    uint8_t read_val_CAN_MUX_pin16 = 2;
    uint8_t save_value_Active_CPU;
    uint8_t get_package_tag = 0;
    uint32_t UART_answer_add_timeout = 0;
	uint32_t i = 0;


    LL_IWDG_ReloadCounter(IWDG);
    //if( (eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain) && (eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU == DISABLE) ) { //Only for Main CPU //
    if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ) { //Only for Main CPU //
        save_value_Active_CPU = eps_p.eps_pmm_ptr->Active_CPU;
        //Get Active CPU from Main UART port
        UART_EPS_Pars_Get_Package(UART_Backup_eps_comm, eps_p);
        UART_EPS_Pars_Get_Package(UART_Main_eps_comm, eps_p);
        error_status = ERROR_N;
        error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_Active_status, 1, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );

        if( error_status != SUCCESS ){

            error_status = SUCCESS;
            get_package_tag = 0;
            UART_answer_add_timeout = SysTick_Counter;

            while(  UART_EPS_ACK != get_package_tag ){

                if ( ( (uint32_t)(SysTick_Counter - UART_answer_add_timeout) ) > 700 ){
                    LL_IWDG_ReloadCounter(IWDG);
                    error_status = ERROR_N;
                    #ifdef DEBUGprintf
                        Error_Handler();
                    #endif
                    break;
                }

                UART_EPS_Pars_Get_Package(UART_Backup_eps_comm, eps_p);
                UART_EPS_Pars_Get_Package(UART_Main_eps_comm, eps_p);
                get_package_tag = UART_Main_eps_comm->recv_pack_buf[3];
            }
        }

        
        if(  error_status != SUCCESS ){

            //Get Active CPU from Backup UART port
            UART_EPS_Pars_Get_Package(UART_Backup_eps_comm, eps_p);
            UART_EPS_Pars_Get_Package(UART_Main_eps_comm, eps_p);
            error_status = ERROR_N;
            error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_Active_status, 2, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );

            if( error_status != SUCCESS ){

                error_status = SUCCESS;
                get_package_tag = 0;
                UART_answer_add_timeout = SysTick_Counter;

                while(  UART_EPS_ACK != get_package_tag ){

                    if ( ( (uint32_t)(SysTick_Counter - UART_answer_add_timeout) ) > 700 ){
                        LL_IWDG_ReloadCounter(IWDG);
                        error_status = ERROR_N;
                        #ifdef DEBUGprintf
                            Error_Handler();
                        #endif
                        break;
                    }

                    UART_EPS_Pars_Get_Package(UART_Backup_eps_comm, eps_p);
                    UART_EPS_Pars_Get_Package(UART_Main_eps_comm, eps_p);
                    get_package_tag = UART_Backup_eps_comm->recv_pack_buf[3];
                }
            }

        }

        if( error_status == SUCCESS ){

            if ( save_value_Active_CPU != eps_p.eps_pmm_ptr->Active_CPU ) {
                if( eps_p.eps_pmm_ptr->Active_CPU == 1 ){
                    eps_p.eps_pmm_ptr->Active_CPU = 1;
                } else {
                    eps_p.eps_pmm_ptr->Active_CPU = 0;
                    //eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = DISABLE;
                }
            }

        }else{ //If UART communication is broken ///

        	i = 0;
            error_I2C = ERROR_N;
        	while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

        	    if( TCA9539_conf_IO_dir_input(PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14) == SUCCESS ){
        	        error_I2C =TCA9539_conf_IO_dir_input(PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P16);
        	    }

        	    if( error_I2C != SUCCESS ){
        	        i++;
        	        LL_mDelay(pmm_i2c_delay_att_conn);
        	    }
        	}

            i = 0;
            error_I2C = ERROR_N;
            while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

                if( TCA9539_read_input_pin(PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14, &read_val_CAN_MUX_pin14) == SUCCESS ){
                    error_I2C = TCA9539_read_input_pin(PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P16, &read_val_CAN_MUX_pin16);
                }

                if( error_I2C != SUCCESS ){
                    i++;
                    LL_mDelay(pmm_i2c_delay_att_conn);
                }
            }

            if( error_I2C == SUCCESS ){
                if((read_val_CAN_MUX_pin14 == 0) && (read_val_CAN_MUX_pin16 == 0)){
                    eps_p.eps_pmm_ptr->Active_CPU = 0;
                    PMM_Reboot_EPS_PassiveCPU( );
                }else{
                    eps_p.eps_pmm_ptr->Active_CPU = 1;
                }
            }

        }

	}

}


/** @brief Checking active CPU flag  (pmm_ptr->Active_CPU) between main and backup CPU.
 * 			In case when (pmm_ptr->Active_CPU) the same value in the Main and Backup CPU
 			This function use Active CPU  to transfer control to passive CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Switch_Active_CPU(uint8_t set_active_CPU,  _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

	int8_t error_status = ERROR_N;
	uint32_t i = 0;

	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain && eps_p.eps_pmm_ptr->Active_CPU == CPUmain_Active && set_active_CPU == CPUmain_Active ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUmain_Active;
		error_status = SUCCESS;

	}else if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUbackup && eps_p.eps_pmm_ptr->Active_CPU == CPUbackup_Active && set_active_CPU == CPUbackup_Active ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUbackup_Active;
		error_status = SUCCESS;

	}else{

		i = 0;
		error_status = ERROR_N;
		while( ( error_status != SUCCESS ) && ( i < pmm_uart_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.
		    LL_IWDG_ReloadCounter(IWDG);
			if( UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PMM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p ) == SUCCESS ){
				if( UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PDM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p ) == SUCCESS ){
					if( UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PAM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p ) == SUCCESS ){
						error_status = UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PBM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );

					}
				}
			}

			if( error_status != SUCCESS ){
				i++;
				LL_mDelay( pmm_uart_delay_att_conn );
				#ifdef DEBUGprintf
				    Error_Handler();
			    #endif
			}
		}
		
		if( error_status == SUCCESS ){
			UART_EPS_Send_NFC( UART_EPS_ID_NFS_Prep_Take_CTRL, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );
			//LL_mDelay(5);
			eps_p.eps_pmm_ptr->Active_CPU = set_active_CPU;

			PMM_Set_MUX_CAN_CPUm_CPUb( eps_p.eps_pmm_ptr );

			i = 0;
			error_status = ERROR_N;
			while( ( error_status != SUCCESS ) && ( i < pmm_uart_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

				error_status = UART_EPS_Send_CMD( UART_EPS_ID_CMD_Take_CTRL, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );
			
				if( error_status != SUCCESS ){
					i++;
					LL_mDelay( pmm_uart_delay_att_conn );
					#ifdef DEBUGprintf
					    Error_Handler();
			        #endif
				}
			}

			if( error_status == SUCCESS ){
				//eps_p.eps_pmm_ptr->Active_CPU = set_active_CPU;
				PMM_Set_mode_Passive_CPU(eps_p);
				//eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
			}

		}

		if( error_status == ERROR_N ){
			eps_p.eps_pmm_ptr->Active_CPU = !set_active_CPU;
			PMM_Set_MUX_CAN_CPUm_CPUb( eps_p.eps_pmm_ptr );
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}

	}

	return error_status;
}


/** @brief  Reboot passive CPU.
	@param  none
	@retval none
*/
void PMM_Reboot_EPS_PassiveCPU( void ){

    //Disable power fo real active CPU
    PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0
    PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
    PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);

    //Delay fo disable power at Active CPU
    LL_mDelay(120);

    //Enable power of passive CPU
    PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
    PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
    PWM_DeInit_Ch3_Ch4( );
}


/** @brief  Take control passive CPU.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval none
*/
void PMM_Take_Control_EPS_PassiveCPU( _EPS_Param eps_p ){

    //Disable power fo real active CPU
    PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0
    PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
    PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
    eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = ENABLE;

    //Delay for disable power at Active CPU
    LL_mDelay(500);

    // Take control.
    PMM_Set_mode_Active_CPU( eps_p );
}


/** @brief  Set Active CPU mode if we get or sent request from/to another CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Set_mode_Active_CPU( _EPS_Param eps_p ){
		
	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUmain_Active;
	}else{
		eps_p.eps_pmm_ptr->Active_CPU = CPUbackup_Active;
	}

    eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic = DISABLE;
    eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Power = DISABLE;

    //eps_p.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
    //eps_p.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
    eps_p.eps_pmm_ptr->Error_CAN_port_M = SUCCESS;
    eps_p.eps_pmm_ptr->Error_CAN_port_B  = SUCCESS;

    I2C4_Init();

	PMM_init( eps_p.eps_pmm_ptr );

	CAN_init_eps(CAN1);
	CAN_init_eps(CAN2);
	CAN_RegisterAllVars();

	if(eps_p.eps_pmm_ptr ->CAN_constatnt_mode == ENABLE ){
		CAN_Var5_fill_telemetry_const();
    }

    CAN_Var4_fill(eps_p);

    PMM_Start_Time_Check_CAN = SysTick_Counter;

	eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
}

/** @brief  Set Passive CPU mode if we get or sent request from/to another CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Set_mode_Passive_CPU( _EPS_Param eps_p ){

	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUbackup_Active ;
	}else{
		eps_p.eps_pmm_ptr->Active_CPU = CPUmain_Active;
	}

    eps_p.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
    eps_p.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
    eps_p.eps_pmm_ptr->Error_CAN_port_M = SUCCESS;
    eps_p.eps_pmm_ptr->Error_CAN_port_B  = SUCCESS;


    CAN_DeInit_eps(CAN1);
	CAN_DeInit_eps(CAN2);

    I2C4_DeInit();
    PWM_DeInit_Ch3_Ch4();

    PBM_Low_Energy_Reset_pin();
    PMM_RT_FL_EPS1_Reset_pin();
    PMM_RT_FL_EPS2_Reset_pin();

	PMM_init( eps_p.eps_pmm_ptr );

    PMM_Start_Time_Check_UART_PassiveCPU = SysTick_Counter;

	//eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1; // Will be command (SAVE PMM and ..... ) from active CPU
}




