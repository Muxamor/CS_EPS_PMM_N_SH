#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "PCA9534.h"
#include "ADS1015.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init_IC.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/eps_struct.h"
#include "PMM/pmm_deploy.h"


void PMM_Deploy( _EPS_Param eps_p ){

    //int8_t error_status = SUCCESS;
    uint8_t deploy_stage;
    static uint32_t  Deploy_start_time_delay = 0;
    static uint16_t  Counter_deploy_exit_LSW_1  = 0;
    static uint16_t  Counter_deploy_exit_LSW_2  = 0;
    static uint32_t  Exit_LSW_poll_time_delay  = 0;
    deploy_stage = eps_p.eps_pmm_ptr->Deploy_stage;

    //Enable power Deploy Logic
    if( eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic == DISABLE ){
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
        LL_mDelay( 20 );
    }else{
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
    }

    //Deploy stage 0 - In delivery container
    if( deploy_stage == 0 ){
        uint8_t value_deploy_exit_LSW_1 = 1;
        uint8_t value_deploy_exit_LSW_2 = 1;

        if( (SysTick_Counter - Exit_LSW_poll_time_delay) > 1000 ){
            PCA9534_read_input_pin(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P05, &value_deploy_exit_LSW_1);
            PCA9534_read_input_pin(PMM_I2Cx_DeployGPIOExt, PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P06, &value_deploy_exit_LSW_2);

            if( value_deploy_exit_LSW_1 == 0){
                Counter_deploy_exit_LSW_1  ++;
            }else{
                Counter_deploy_exit_LSW_1 = 0;
            }

            if( value_deploy_exit_LSW_2 == 0){
                Counter_deploy_exit_LSW_2 ++;
            }else{
                Counter_deploy_exit_LSW_2 = 0;
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
            eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
            Deploy_start_time_delay = SysTick_Counter;
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

        }else{
            eps_p.eps_pmm_ptr->Deploy_stage = 1; // Next deploy stage 1 - Only one Limit switch = 1, waiting good generation level
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

    // Deploy stage 1 - Only one Limit switch = 1, waiting good generation level
    }else if( deploy_stage == 1 ){
        //TODO дописать проверку генерации как будет дописан PAM.
        //TODO сделать проверку что нет ошибок PWR Mon и I2C EXT GPIO
        eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
        Deploy_start_time_delay = SysTick_Counter;
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

    // Deploy stage 2 - waiting timeout before deploy
    }else if( deploy_stage == 2 ){
        if( (SysTick_Counter - Deploy_start_time_delay) > PMM_Deploy_Time_Delay ){
            eps_p.eps_pmm_ptr->Deploy_stage = 3; // Next deploy stage 3 - low level energy, check and waiting for charge if battery low.
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

    // Deploy stage 3 -  low level energy, check battery level and waiting for charge if battery low.
    }else if(deploy_stage == 3){
        if( eps_p.eps_pbm_ptr->Low_Energy_Flag == 0 ){
            eps_p.eps_pmm_ptr->Deploy_stage = 4; // Next deploy stage 4 - deploy at channel 1
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }else{
            eps_p.eps_pmm_ptr->Deploy_stage = 3; // Next deploy stage 3 - low level energy, check and waiting for charge if battery low.
        }

    }else if( deploy_stage == 4 ){

        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE );

        PMM_Deploy_Burn_Procedure( eps_p, PMM_PWR_Deploy_Ch1);

        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE );

    }




}

ErrorStatus PMM_Deploy_Burn_Procedure( _EPS_Param eps_p, uint8_t burn_pwr_ch_num ){

    int8_t error_status = SUCCESS;
    uint8_t get_state_limit_switch_1 = 0;
    uint8_t get_state_limit_switch_2 = 0;

    error_status += PMM_Deploy_Burn_PWR_Ch( eps_p, PMM_Deploy_Burn_Attempt_1, burn_pwr_ch_num, &get_state_limit_switch_1, &get_state_limit_switch_2 );

    if( (get_state_limit_switch_1 == 0) || (get_state_limit_switch_2 == 0) ){
        LL_mDelay(10);
        error_status += PMM_Deploy_Burn_PWR_Ch( eps_p, PMM_Deploy_Burn_Attempt_2, burn_pwr_ch_num, &get_state_limit_switch_1, &get_state_limit_switch_2 );
    }

    if( (get_state_limit_switch_1 == 0) || (get_state_limit_switch_2 == 0) ){
        LL_mDelay(10);
        error_status += PMM_Deploy_Burn_PWR_Ch( eps_p, PMM_Deploy_Burn_Attempt_3, burn_pwr_ch_num, &get_state_limit_switch_1, &get_state_limit_switch_2 );
    }

    return error_status;
}


ErrorStatus PMM_Deploy_Burn_PWR_Ch( _EPS_Param eps_p, uint8_t attepmt_burn ,uint8_t burn_pwr_ch_num, uint8_t *ret_state_limit_switch_1,  uint8_t *ret_state_limit_switch_2){

    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
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

    if( attepmt_burn == PMM_Deploy_Burn_Attempt_1 ){
        //Whait Burn time 1
        while((SysTick_Counter - start_burn_time) < PMM_Deploy_Burn_time_1 ){

        }

    }else{
        if( attepmt_burn == PMM_Deploy_Burn_Attempt_2 ){
            deploy_burn_timeout = PMM_Deploy_Burn_time_2;

        }else if( attepmt_burn == PMM_Deploy_Burn_Attempt_3 ){
            deploy_burn_timeout = PMM_Deploy_Burn_time_3;
        }

        while((SysTick_Counter - start_burn_time) < deploy_burn_timeout ){
            PMM_Deploy_check_Lim_SW( eps_p,  burn_pwr_ch_num, ret_state_limit_switch_1, ret_state_limit_switch_2 );

            if( (*ret_state_limit_switch_1 == 1) && ( *ret_state_limit_switch_2 == 1) ){
                break;
            }
        }
    }

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

    if(attepmt_burn == PMM_Deploy_Burn_Attempt_1){
        PMM_Deploy_check_Lim_SW( eps_p, burn_pwr_ch_num,  ret_state_limit_switch_1, ret_state_limit_switch_2 );
    }

//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );
//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE );

    if( error_I2C == SUCCESS ){
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_GPIO_Ext = SUCCESS;
    }else{
        eps_p.eps_pmm_ptr-> Error_I2C_Deploy_GPIO_Ext = ERROR;
    }

    return error_I2C;
}

ErrorStatus PMM_Deploy_check_Lim_SW( _EPS_Param eps_p, uint8_t burn_pwr_ch_num, uint8_t *ret_state_limit_switch_1,  uint8_t *ret_state_limit_switch_2){

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    float ADC_ch_meas = (float)0.0;
    uint8_t ADC_num_ch = 0;

//    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );

    SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

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

    while((error_I2C != SUCCESS) && (i < pmm_i2c_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

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
        if( ADC_ch_meas < 0.4 ){
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

        }else if((ADC_ch_meas > 0.4) && (ADC_ch_meas < 0.7)){
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




