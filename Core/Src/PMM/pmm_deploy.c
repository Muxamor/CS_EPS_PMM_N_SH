#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "PCA9534.h"
#include "SetupPeriph.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init_IC.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/eps_struct.h"
#include "PMM/pmm_deploy.h"


void PMM_Deploy( _EPS_Param eps_p){

    //int8_t error_status = SUCCESS;
    uint8_t deploy_stage;
    static uint32_t  Deploy_start_time_delay = 0;

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

        //Read position exit Limit switch numer 1 and 2
        PCA9534_read_input_pin(PMM_I2Cx_DeployGPIOExt,  PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P05, &value_deploy_exit_LSW_1);
        PCA9534_read_input_pin(PMM_I2Cx_DeployGPIOExt,  PMM_I2CADDR_DeployGPIOExt, PCA9534_IO_P06, &value_deploy_exit_LSW_2);

        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 = !value_deploy_exit_LSW_1;
        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 = !value_deploy_exit_LSW_2;

        //Change stage deploy
        if( (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 == 0) && (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 == 0) ){
            eps_p.eps_pmm_ptr->Deploy_stage = 0; // Next deploy stage 0 - In delivery container

        }else if( ( eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 == 1 ) && ( eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 == 1) ){
            eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

        }else{
            eps_p.eps_pmm_ptr->Deploy_stage = 1; // Next deploy stage 1 - Only one Limit switch = 1, waiting good generation level
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

        return;

    // Deploy stage 1 - Only one Limit switch = 1, waiting good generation level
    }else if( deploy_stage == 1 ){
        //TODO дописать проверку генерации как будет дописан PAM.
        eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;

        return;

    // Deploy stage 2 - low level energy, check battery level and waiting for charge if battery low.
    }else if( deploy_stage == 2 ){

       if( eps_p.eps_pbm_ptr->Low_Energy_Flag == 0 ){
           eps_p.eps_pmm_ptr->Deploy_stage = 3; // Next deploy stage 3 - low level energy, check and waiting for charge if battery low.
           Deploy_start_time_delay = SysTick_Counter;
           eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
       }else{
           eps_p.eps_pmm_ptr->Deploy_stage = 2; // Next deploy stage 2 - low level energy, check and waiting for charge if battery low.
       }

       return;

    // Deploy stage 3 -  waiting timeout before deploy
    }else if(deploy_stage == 3){

        if( (SysTick_Counter - Deploy_start_time_delay) > PMM_Deploy_Time_Delay ){
            eps_p.eps_pmm_ptr->Deploy_stage = 4; // Next deploy stage 4 - low level energy, check and waiting for charge if battery low.
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

    }else if( deploy_stage == 4 ){

    }

    //TODO внутри ADS1015_init поправить выставление ошибки а мождет и совсем убрать отуда
    //ADS1015_init(  eps_p.eps_pmm_ptr, PMM_I2Cx_DeployADC, PMM_I2CADDR_DeployADC );

}
