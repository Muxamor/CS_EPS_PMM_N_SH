#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_iwdg.h"
#include "SetupPeriph.h"
#include "PCA9534.h"
#include "ADS1015.h"
#include "tim_pwm.h"
#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init_IC.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_init.h"
#include "PDM/pdm_ctrl.h"
#include "PDM/pdm_init.h"
#include "PAM/pam_init.h"
#include "PAM/pam.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PMM/pmm_deploy.h"

/** @brief  Deploy CubeSat Norbi.
 *  @param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None
*/
ErrorStatus PMM_Deploy( _EPS_Param eps_p ){

    //int8_t error_status = SUCCESS;
    uint16_t  i = 0;
    int8_t error_status = SUCCESS;
    uint8_t deploy_stage;
	uint8_t PBM_Number = 0, Branch_Number = 0, Heat_Number = 0;
    static uint32_t  Deploy_start_time_delay = 0;
    static uint32_t  Exit_LSW_poll_time_delay  = 0;
    static uint16_t  Counter_deploy_exit_LSW_1  = 0;
    static uint16_t  Counter_deploy_exit_LSW_2  = 0;

    deploy_stage = eps_p.eps_pmm_ptr->Deploy_stage;

    //Enable power Deploy Logic
    if( eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic == DISABLE ){
        error_status += PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
        LL_mDelay( 5 );
    }else{
        error_status += PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
    }

    //Deploy stage 0 - In delivery container
    if( deploy_stage == 0 ){
        uint8_t value_deploy_exit_LSW_1 = 1;
        uint8_t value_deploy_exit_LSW_2 = 1;

        if( ((uint32_t)(SysTick_Counter - Exit_LSW_poll_time_delay)) > ((uint32_t) 1000) ){
            error_status = ERROR_N;
            error_status = PMM_Deploy_Get_Exit_LSW( eps_p, &value_deploy_exit_LSW_1, &value_deploy_exit_LSW_2 );

            if(error_status == SUCCESS ){
                if( value_deploy_exit_LSW_1 == 0){
                    Counter_deploy_exit_LSW_1++;
                }else{
                    Counter_deploy_exit_LSW_1 = 0;
                }

                if( value_deploy_exit_LSW_2 == 0){
                    Counter_deploy_exit_LSW_2++;
                }else{
                    Counter_deploy_exit_LSW_2 = 0;
                }
            }

            Exit_LSW_poll_time_delay = SysTick_Counter;// Renew counter for new loop
        }

        if( Counter_deploy_exit_LSW_1 >= 10){
            eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 = 1;
        }

        if( Counter_deploy_exit_LSW_2 >= 10){
            eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 = 1;
        }

        //Change stage deploy
        if( (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 == 0) && (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 == 0) ){
            eps_p.eps_pmm_ptr->Deploy_stage = 0; // Next deploy stage 0 - In delivery container

        }else if( ( eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 == 1 ) && ( eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 == 1) ){
            //Set next deploy stage
            eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge battery if enegy lavel is low
            Deploy_start_time_delay = SysTick_Counter;
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

        }else if( (( eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 == 1 ) && ( Counter_deploy_exit_LSW_2 == 0)) ||
        		        (( eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 == 1 ) && ( Counter_deploy_exit_LSW_1 == 0)) ){
            //Set next deploy stage
            eps_p.eps_pmm_ptr->Deploy_stage = 1; // Next deploy stage 1 - Only one Limit switch = 1, waiting good generation level
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

        if( (eps_p.eps_pmm_ptr->Deploy_stage == 1) || (eps_p.eps_pmm_ptr->Deploy_stage == 2) ){
            //Enable main CAN
            PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE);
            LL_mDelay( 50 );
            CAN_init_eps(CAN1);
            CAN_RegisterAllVars();

            //Enable PBM logic power and thermostat.
            PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_PBMs_Logic, ENABLE );

            //PBM
            for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++  ){
            	for(Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
                    eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].DchgEnableCmd = ENABLE;
                    eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].ChgEnableCmd = ENABLE;
                    eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Cmd = ENABLE;
                    eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Cmd = DISABLE;
            	}
            	for(Heat_Number = 0; Heat_Number < PBM_T1_HEAT_QUANTITY; Heat_Number++){
            		eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_Number].PCA9534_Heat_CMD = DISABLE;
            	}
            }

            LPUART1_Init();
            USART3_Init();
            Setup_UART_Interrupt();

            //Enable passive CPU
            PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
            PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
            PWM_DeInit_Ch3_Ch4( );
            eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = DISABLE;

            PMM_init( eps_p.eps_pmm_ptr );
            PDM_init( eps_p.eps_pdm_ptr );
            PAM_init( eps_p.eps_pam_ptr );
            PBM_T1_Init( eps_p.eps_pbm_ptr );

            //Fill Var4
            CAN_Var4_fill(eps_p);

            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

    // Deploy stage 1 - Only one Limit switch = 1, waiting good generation level
    }else if( deploy_stage == 1 ){
        uint8_t total_error_pwr_mon_pam = 0;
        uint16_t total_power_gen_pam = 0;

        //Check Enable state power supply PAM module and get telemetry PAM if PWR supply disable
        if( (eps_p.eps_pam_ptr->State_DC_DC == DISABLE) && (eps_p.eps_pam_ptr->State_LDO == DISABLE) ){
            eps_p.eps_pam_ptr->State_DC_DC = ENABLE;
            error_status += PAM_init( eps_p.eps_pam_ptr );
            error_status += PAM_Get_Telemetry( eps_p.eps_pam_ptr );
            CAN_Var4_fill(eps_p);
        }

        //Checking quantity error input power monitors on PAM
        for( total_error_pwr_mon_pam = 0, i = 0; i < PAM_PWR_IN_Ch_quantity; i++){
            total_error_pwr_mon_pam = total_error_pwr_mon_pam + eps_p.eps_pam_ptr->PWR_IN_Channel[i].Error_PWR_Mon;
        }

        //if all is errors it is mean PAM is broken and we go to next stage deploy
        if( total_error_pwr_mon_pam == PAM_PWR_IN_Ch_quantity ){
            error_status = ERROR_N;
            eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
            Deploy_start_time_delay = SysTick_Counter;
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

        }else{
            for ( total_power_gen_pam = 0, i = 0; i < PAM_PWR_IN_Ch_quantity; i++ ){
                total_power_gen_pam = total_power_gen_pam + ( (eps_p.eps_pam_ptr->PWR_IN_Channel[i].Current_val * eps_p.eps_pam_ptr->PWR_IN_Channel[i].Voltage_val ) / 1000 );
            }

            if( total_power_gen_pam > PMM_Deploy_Power_Gen_EDGE ){
                eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
                Deploy_start_time_delay = SysTick_Counter;
                eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

            }else{
                eps_p.eps_pmm_ptr->Deploy_stage = 1;
            }
        }

    // Deploy stage 2 - waiting timeout before deploy
    }else if( deploy_stage == 2 ){
    	if( ((uint32_t)(SysTick_Counter - Deploy_start_time_delay)) > ((uint32_t) PMM_Deploy_Time_Delay) ){
            eps_p.eps_pmm_ptr->Deploy_stage = 3; // Next deploy stage 3 - low level energy, check and waiting for charge if battery low.
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
    	}

    // Deploy stage 3 -  low level energy, check battery level and waiting for charge if battery low.
    }else if(deploy_stage == 3){
        if( (eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val > PBM_T1_NORMAL_ENERGY_EDGE) || (eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Voltage_val > PBM_T1_NORMAL_ENERGY_EDGE) ||
                (eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val > PBM_T1_NORMAL_ENERGY_EDGE) || (eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Voltage_val > PBM_T1_NORMAL_ENERGY_EDGE) ){
            eps_p.eps_pmm_ptr->Deploy_stage = 4; // Next deploy stage 4 - deploy at channel 1
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }else{
            eps_p.eps_pmm_ptr->Deploy_stage = 3; // waiting for the batteries to charge
        }

    // Deploy stage 4 -  burn channel 1. ( antenna Z side)
    }else if( deploy_stage == 4 ){
        error_status += PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE );
        error_status += PMM_Deploy_Burn_Procedure( eps_p, PMM_PWR_Deploy_Ch1);
        eps_p.eps_pmm_ptr->Deploy_stage = 5; // Next deploy stage 5 - deploy at channel 2
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

    // Deploy stage 5 -  burn channel 2. ( antenna Z side)
    }else if( deploy_stage == 5 ){
        error_status += PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE);
        error_status += PMM_Deploy_Burn_Procedure(eps_p, PMM_PWR_Deploy_Ch2);
        eps_p.eps_pmm_ptr->Deploy_stage = 6; // Next deploy stage 6 - Enable BRC
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

    // Deploy stage 6 - Enable BRK1, BRK2, CANm, CANb, PAM DC-DC.
    }else if( deploy_stage == 6 ){
        //Enable CAN
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE );
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE );
        LL_mDelay( 50 );
        CAN_init_eps(CAN1);
        CAN_init_eps(CAN2);
        CAN_RegisterAllVars();

        //Enable PAM DC-DC
        eps_p.eps_pam_ptr->State_DC_DC = ENABLE;
        PAM_init( eps_p.eps_pam_ptr );
        //Enable BRC
        error_status += PDM_Set_state_PWR_CH(eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, ENABLE);
        error_status += PDM_Set_state_PWR_CH(eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, ENABLE);

        //Fill Var4
        CAN_Var4_fill(eps_p);

        eps_p.eps_pmm_ptr->Deploy_stage = 7; // Next deploy stage 7 - deploy at channel 3
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

    // Deploy stage 7 -  burn channel 3. (SP Y)
    }else if( deploy_stage == 7 ){
        error_status += PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE);
        error_status += PMM_Deploy_Burn_Procedure(eps_p, PMM_PWR_Deploy_Ch3);
        eps_p.eps_pmm_ptr->Deploy_stage = 8; // Next deploy stage 8 - deploy at channel 4
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

    // Deploy stage 8 -  burn channel 4. (SP Y)
    }else if( deploy_stage == 8 ){
        error_status += PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE);
        error_status += PMM_Deploy_Burn_Procedure(eps_p, PMM_PWR_Deploy_Ch4);

        error_status += PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE);

        //Disable Power deploy logic
        error_status += PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );

        eps_p.eps_pmm_ptr->Deploy_stage = 9; //  Next deploy stage 9 - Finish deploy
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

        //Fill Var4
        CAN_Var4_fill(eps_p);
    }

    if( error_status != SUCCESS ){
        return ERROR_N;
    }
    return SUCCESS;
}

/** @brief  Deploy burn procedure. Burning threads for deploy elements of the CubeSat.
 *  @param  eps_p - contain pointer to struct which contain all parameters EPS.
   	@param  burn_pwr_ch_num - number of channel to burn:
   	                                    PMM_PWR_Deploy_Ch1
   	                                    PMM_PWR_Deploy_Ch2
   	                                    PMM_PWR_Deploy_Ch3
   	                                    PMM_PWR_Deploy_Ch4
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Deploy_Burn_Procedure( _EPS_Param eps_p, uint8_t burn_pwr_ch_num ){

    int8_t error_status = SUCCESS;
    uint8_t get_state_limit_switch_1 = 0;
    uint8_t get_state_limit_switch_2 = 0;

    //First attempt to deploy for a specific channel.
    error_status += PMM_Deploy_Burn_PWR_Ch( eps_p, PMM_Deploy_Burn_Attempt_1, burn_pwr_ch_num, &get_state_limit_switch_1, &get_state_limit_switch_2 );

    if( (get_state_limit_switch_1 != 1) || (get_state_limit_switch_2 != 1) ){
        //Second attempt to deploy for a specific channel.
        LL_mDelay(900);
        LL_IWDG_ReloadCounter(IWDG);
        error_status += PMM_Deploy_Burn_PWR_Ch( eps_p, PMM_Deploy_Burn_Attempt_2, burn_pwr_ch_num, &get_state_limit_switch_1, &get_state_limit_switch_2 );
    }

    if( (get_state_limit_switch_1 != 1) || (get_state_limit_switch_2 != 1) ){
        //Third attempt to deploy for a specific channel.
        LL_mDelay(900);
        LL_IWDG_ReloadCounter(IWDG);
        error_status += PMM_Deploy_Burn_PWR_Ch( eps_p, PMM_Deploy_Burn_Attempt_3, burn_pwr_ch_num, &get_state_limit_switch_1, &get_state_limit_switch_2 );
    }

    if( error_status != SUCCESS ){
        error_status = ERROR_N;
    }
    return error_status;
}

/** @brief  Deploy burn threads in burn power channel.
 *  @param  eps_p - contain pointer to struct which contain all parameters EPS.
 *  @param  attempt_burn - attempt to burn threads:
                                        PMM_Deploy_Burn_Attempt_1
                                        PMM_Deploy_Burn_Attempt_2
                                        PMM_Deploy_Burn_Attempt_3
   	@param  burn_pwr_ch_num - number of channel to burn:
   	                                    PMM_PWR_Deploy_Ch1
   	                                    PMM_PWR_Deploy_Ch2
   	                                    PMM_PWR_Deploy_Ch3
   	                                    PMM_PWR_Deploy_Ch4
   	@param  ret_state_limit_switch_1 - pointer for return value state limit switch 1
   	@param  ret_state_limit_switch_2 - pointer for return value state limit switch 2
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Deploy_Burn_PWR_Ch( _EPS_Param eps_p, uint8_t attempt_burn , uint8_t burn_pwr_ch_num, uint8_t *ret_state_limit_switch_1, uint8_t *ret_state_limit_switch_2){

    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t error_status = ERROR_N;
    uint8_t i = 0;
    uint32_t start_burn_time = 0;
    uint32_t deploy_burn_timeout = 0;
    _PMM_table pmm_table;

    SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

    pmm_table = PMM__Table( burn_pwr_ch_num );

//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE );
//    LL_mDelay(20);

    start_burn_time = SysTick_Counter;

    i = 0;
    error_I2C = ERROR_N;

    //Enable burn
    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

        if( PCA9534_Set_output_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext ) == SUCCESS ){
            error_I2C = PCA9534_conf_IO_dir_output( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );
        }

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pmm_i2c_delay_att_conn );
        }
    }

    error_status = error_I2C;

    if( attempt_burn == PMM_Deploy_Burn_Attempt_1 ){
        //Whit Burn time 1
        while(((uint32_t)(SysTick_Counter - start_burn_time)) < ((uint32_t)PMM_Deploy_Burn_time_1) ){
            LL_IWDG_ReloadCounter(IWDG);
        }

    }else{
        if( attempt_burn == PMM_Deploy_Burn_Attempt_2 ){
            deploy_burn_timeout = (uint32_t)PMM_Deploy_Burn_time_2;

        }else if( attempt_burn == PMM_Deploy_Burn_Attempt_3 ){
            deploy_burn_timeout = (uint32_t)PMM_Deploy_Burn_time_3;
        }

        while( ( (uint32_t)(SysTick_Counter - start_burn_time) ) < deploy_burn_timeout ){
            error_status += PMM_Deploy_check_Lim_SW( eps_p,  burn_pwr_ch_num, ret_state_limit_switch_1, ret_state_limit_switch_2 );
            LL_IWDG_ReloadCounter(IWDG);
            if( (*ret_state_limit_switch_1 == 1) && ( *ret_state_limit_switch_2 == 1) ){
                break;
            }
        }
    }

    i = 0;
    error_I2C = ERROR_N;
    //Disable burn
    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

        if( PCA9534_Reset_output_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext ) == SUCCESS ){
            error_I2C = PCA9534_conf_IO_dir_output( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );
        }

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pmm_i2c_delay_att_conn );
        }
    }

    error_status = error_status + error_I2C;

    if( attempt_burn == PMM_Deploy_Burn_Attempt_1){
        error_status += PMM_Deploy_check_Lim_SW( eps_p, burn_pwr_ch_num, ret_state_limit_switch_1, ret_state_limit_switch_2 );
    }

//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );
//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE );

    if( error_I2C == SUCCESS ){
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_GPIO_Ext = SUCCESS;
    }else{
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_GPIO_Ext = ERROR;
    }

    if( error_status != SUCCESS ){
        error_status = ERROR_N;
    }

    return error_status;
}


/** @brief  Deploy burn threads in burn power channel.
 *  @param  eps_p - contain pointer to struct which contain all parameters EPS.
   	@param  burn_pwr_ch_num - number of channel to burn:
   	                                    PMM_PWR_Deploy_Ch1
   	                                    PMM_PWR_Deploy_Ch2
   	                                    PMM_PWR_Deploy_Ch3
   	                                    PMM_PWR_Deploy_Ch4
   	@param  ret_state_limit_switch_1 - pointer for return value state limit switch 1
   	@param  ret_state_limit_switch_2 - pointer for return value state limit switch 2
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Deploy_check_Lim_SW( _EPS_Param eps_p, uint8_t burn_pwr_ch_num, uint8_t *ret_state_limit_switch_1,  uint8_t *ret_state_limit_switch_2){

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    float ADC_ch_meas = 0.0f;
    uint8_t ADC_num_ch = 0;

//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );

    SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

    //Init. deploy ADC
    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){

        error_I2C = ADS1015_init( eps_p.eps_pmm_ptr, PMM_I2Cx_DeployADC, PMM_I2CADDR_DeployADC );

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pmm_i2c_delay_att_conn );
        }
    }

    LL_mDelay(2);

    if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch1 ){
        ADC_num_ch = ADS1015_AINp0_AINnGND;
    }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch2 ){
        ADC_num_ch = ADS1015_AINp1_AINnGND;
    }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch3 ){
        ADC_num_ch = ADS1015_AINp2_AINnGND;
    }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch4 ){
        ADC_num_ch = ADS1015_AINp3_AINnGND;
    }

    i = 0;
    error_I2C = ERROR_N;

    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){

        error_I2C =  PMM_ADS1015_average_meas(PMM_I2Cx_DeployADC, PMM_I2CADDR_DeployADC,  ADC_num_ch, 16, &ADC_ch_meas );

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pmm_i2c_delay_att_conn );
        }
    }

    //PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );

    if( error_I2C == SUCCESS ){

        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_ADC = SUCCESS;
        //Parsing the received data
        if( ADC_ch_meas < 0.35 ){
            //all Limit Switch close
            *ret_state_limit_switch_1 = 0;
            *ret_state_limit_switch_2 = 0;

            if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch1 ){
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp = 0;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch2 ){
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn = 0;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch3 ){
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn = 0;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch4 ){
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp = 0;
            }

        }else if((ADC_ch_meas > 0.35) && (ADC_ch_meas < 0.7)){
            //2k open, 1k close Limit Switch
            *ret_state_limit_switch_1 = 1;
            *ret_state_limit_switch_2 = 0;

            if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch1 ){
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp = 0;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch2 ){
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn = 0;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch3 ){
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn = 0;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch4 ){
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp = 0;
            }

        }else if((ADC_ch_meas > 0.7) && (ADC_ch_meas < 1.1)){
            //2k close, 1k open Limit Switch
            *ret_state_limit_switch_1 = 0;
            *ret_state_limit_switch_2 = 1;

            if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch1 ){
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp = 1;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch2 ){
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn = 1;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch3 ){
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn = 1;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch4 ){
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp = 0;
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp = 1;
            }

        }else if( ADC_ch_meas > 1.1 ){
            //ALL open Limit Switch
            *ret_state_limit_switch_1 = 1;
            *ret_state_limit_switch_2 = 1;

            if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch1 ){
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp = 1;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch2 ){
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn = 1;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch3 ){
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn = 1;

            }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch4 ){
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp = 1;
                eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp = 1;
            }
        }

    }else{
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_ADC = ERROR;

        *ret_state_limit_switch_1 = 0;
        *ret_state_limit_switch_2 = 0;

        if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch1 ){
            eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp = 0;
            eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp = 0;

        }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch2 ){
            eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn = 0;
            eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn = 0;

        }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch3 ){
            eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn = 0;
            eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn = 0;

        }else if( burn_pwr_ch_num == PMM_PWR_Deploy_Ch4 ){
            eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp = 0;
            eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp = 0;
        }
    }

    return  error_I2C;
}


/** @brief  Get value of exit limit switch 1 and 2.
 *  @param  eps_p - contain pointer to struct which contain all parameters EPS.
   	@param  ret_exit_LSW_1 - pointer for return value state exit from container limit switch 1
   	@param  ret_exit_LSW_2 - pointer for return value state exit from container switch 2
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Deploy_Get_Exit_LSW( _EPS_Param eps_p, uint8_t *ret_exit_LSW_1, uint8_t *ret_exit_LSW_2 ){

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N;

    SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

    i = 0;
    error_I2C = ERROR_N;
    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

        if( PCA9534_conf_IO_dir_input(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P05) == SUCCESS){
            if(PCA9534_conf_IO_pol_normal(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P05) == SUCCESS){
                error_I2C = PCA9534_read_input_pin(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P05, ret_exit_LSW_1);
            }
        }

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay(pmm_i2c_delay_att_conn);
        }
    }

    i = 0;
    error_I2C = ERROR_N;
    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

        if( PCA9534_conf_IO_dir_input(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P06) == SUCCESS){
            if(PCA9534_conf_IO_pol_normal(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P06) == SUCCESS){
                error_I2C = PCA9534_read_input_pin(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P06, ret_exit_LSW_2);
            }
        }

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay(pmm_i2c_delay_att_conn);
        }
    }

    if( error_I2C == SUCCESS ){
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_GPIO_Ext = SUCCESS;
    }else{
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_GPIO_Ext = ERROR;
    }

    return error_I2C;
}
