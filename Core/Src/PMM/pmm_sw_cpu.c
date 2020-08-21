
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
#include "PBM/pbm_config.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_damage_ctrl.h"
#include "uart_eps_comm.h"
#include "PMM/pmm_sw_cpu.h"

/** @brief CPU main checking active CPU flag  (pmm_ptr->Active_CPU) between main and backup CPU.
 * 			In case when (pmm_ptr->Active_CPU) the same value in the Main and Backup CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_CPUm_Check_Active_CPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

	_PMM backup_CPU_pmm = {0};
    _PAM backup_CPU_pam = {0};
    _PDM backup_CPU_pdm = {0};
    _PBM backup_CPU_pbm[PBM_QUANTITY] = {0};
	_EPS_Param tmp_eps_param = {.eps_pmm_ptr = &backup_CPU_pmm,
                                .eps_pam_ptr = &backup_CPU_pam,
                                .eps_pdm_ptr = &backup_CPU_pdm,
                                .eps_pbm_ptr = backup_CPU_pbm
	                            };

    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t error_status = ERROR_N;
    uint8_t read_val_CAN_MUX_pin14 = 2;
    uint8_t read_val_CAN_MUX_pin16 = 2;
	uint32_t UART_answer_add_timeout = 0;
	uint32_t i = 0;


	if( (eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain) && (eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU == DISABLE) ) { //Only for Main CPU

        error_status = ERROR_N;
        i = 0;
        LL_IWDG_ReloadCounter(IWDG);

        while ((error_status != SUCCESS) && (i < 2)){

            error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_PMM_struct, 1, UART_Main_eps_comm, UART_Backup_eps_comm, tmp_eps_param);

            if( error_status != SUCCESS ){

                UART_Main_eps_comm->waiting_answer_flag = 1;
                UART_answer_add_timeout = SysTick_Counter;

                while(  UART_Main_eps_comm->waiting_answer_flag != 0 ){ //waiting_answer_flag - The flag should be reset in the function UART_EPS_Pars_Get_Package when a response is received.

                    if ( ( (uint32_t)(SysTick_Counter - UART_answer_add_timeout) ) > 600 ){
                        UART_Main_eps_comm->waiting_answer_flag = 0;
                        UART_Main_eps_comm->error_port_counter++;
                        error_status = ERROR_N;
                        #ifdef DEBUGprintf
                            Error_Handler();
                        #endif
                        break;
                    }

                    if(  UART_Main_eps_comm->stop_recv_pack_flag == 1){ //Response processing
                        error_status = UART_EPS_Pars_Get_Package(  UART_Main_eps_comm, eps_p);
                    }

                    //For case when Backup CPU is active.
                    if(  UART_Backup_eps_comm->stop_recv_pack_flag == 1){
                        UART_EPS_Pars_Get_Package(UART_Backup_eps_comm, tmp_eps_param);
                    }

                }
            }

            if( error_status != SUCCESS ) {
                i++;
            }
        }

        LL_IWDG_ReloadCounter(IWDG);

        if( error_status != SUCCESS ){
        	error_status = ERROR_N;
        	i = 0;
            while ((error_status != SUCCESS) && (i < 2)){

                error_status =UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_PMM_struct, 2, UART_Main_eps_comm, UART_Backup_eps_comm, tmp_eps_param);

                if( error_status != SUCCESS ){

                    UART_Backup_eps_comm->waiting_answer_flag = 1;
                    UART_answer_add_timeout = SysTick_Counter;

                    while( UART_Backup_eps_comm->waiting_answer_flag != 0 ){ //waiting_answer_flag - The flag should be reset in the function UART_EPS_Pars_Get_Package when a response is received.

                        if ( ( (uint32_t)(SysTick_Counter - UART_answer_add_timeout) ) > 600 ){
                            UART_Backup_eps_comm->waiting_answer_flag = 0;
                            UART_Backup_eps_comm->error_port_counter++;
                            error_status = ERROR_N;
                            #ifdef DEBUGprintf
                                Error_Handler();
                            #endif
                            break;
                        }

                        if( UART_Backup_eps_comm->stop_recv_pack_flag == 1){ //Response processing
                            error_status = UART_EPS_Pars_Get_Package( UART_Backup_eps_comm, eps_p);
                        }

                        //For case when Backup CPU is active.
                        if(  UART_Main_eps_comm->stop_recv_pack_flag == 1){
                            UART_EPS_Pars_Get_Package(UART_Main_eps_comm, tmp_eps_param);
                        }

                    }
                }

                if( error_status != SUCCESS ) {
                    i++;
                }
            }
        }

        if( error_status == SUCCESS ){

            if (eps_p.eps_pmm_ptr->Active_CPU != tmp_eps_param.eps_pmm_ptr->Active_CPU) {
                if (tmp_eps_param.eps_pmm_ptr->Active_CPU == 1) {
                    eps_p.eps_pmm_ptr->Active_CPU = 1;
                } else {
                    eps_p.eps_pmm_ptr->Active_CPU = 0;
                }
            }

        }else{
        //If UART communication is broken
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


/** @brief  Take control passive CPU.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Take_Control_EPS_PassiveCPU( _EPS_Param eps_p ){

    //Disabel power fo real active CPU
    PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
    PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
    eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = ENABLE;

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

    CAN_Var4_fill(eps_p);

    if(eps_p.eps_pmm_ptr ->CAN_constatnt_mode == ENABLE ){
        CAN_Var5_fill_telemetry_const();
    }

    I2C4_Init();
    PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0

	PMM_init( eps_p.eps_pmm_ptr );

	CAN_init_eps(CAN1);
	CAN_init_eps(CAN2);
	CAN_RegisterAllVars();

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

	//eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1; // Wiil be command (SAVE PMM and ..... ) from active CPU   
}




