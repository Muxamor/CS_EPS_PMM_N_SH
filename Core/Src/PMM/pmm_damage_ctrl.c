#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_tim.h"
#include "SetupPeriph.h"
#include "tim_pwm.h"
#include "PMM/eps_struct.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_init.h"
#include "PDM/pdm_init.h"
#include "PDM/pdm_ctrl.h"
#include "PAM/pam_init.h"
#include "PAM/pam_ctrl.h"
#include "PBM/pbm_config.h"
#include "PBM/pbm_init.h"
#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include "uart_eps_comm.h"


uint32_t PMM_Start_Time_Check_CAN;
uint32_t PMM_Start_Time_Check_UART_PassiveCPU;
/** @brief  Check Damage CAN port ( Time interval check = PMM_CAN_Exch_Data_Check_Time_Gap )
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
void PMM_Damage_Check_CAN_m_b( _EPS_Param eps_p ){

    //Check time interval
    if( ((uint32_t)(SysTick_Counter - PMM_Start_Time_Check_CAN) ) > ((uint32_t)PMM_CAN_Exch_Data_Check_Time_Gap) ){

        if( (CAN1_exchange_data_flag == 0) && (eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain == ENABLE) ){
            eps_p.eps_pmm_ptr->Error_CAN_port_M = ERROR;
        }else{
            eps_p.eps_pmm_ptr->Error_CAN_port_M = SUCCESS;
        }

        if( ( (CAN2_exchange_data_flag == 0) && (eps_p.eps_pmm_ptr->Error_CAN_port_M == ERROR) && (eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup == ENABLE) ) ||
                ( (CAN2_exchange_data_flag == 0) && (eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain == DISABLE) ) ){
            eps_p.eps_pmm_ptr->Error_CAN_port_B = ERROR;
        }else{
            eps_p.eps_pmm_ptr->Error_CAN_port_B = SUCCESS;
        }

        //Reboot power CAN (try to repair) and switch Active CPU
        if( ( (eps_p.eps_pmm_ptr->Error_CAN_port_M == ERROR) && (eps_p.eps_pmm_ptr->Error_CAN_port_B == ERROR) ) ||
                    ( (eps_p.eps_pmm_ptr->Error_CAN_port_M == ERROR) && (eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup == DISABLE) ) ||
                        ( (eps_p.eps_pmm_ptr->Error_CAN_port_B == ERROR) && (eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain == DISABLE) ) ){

            if( eps_p.eps_serv_ptr->Was_Reboot_PWR_CAN == 0 ){
                CAN_DeInit_eps(CAN1);
                CAN_DeInit_eps(CAN2);
                PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, DISABLE);
                PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANbackup, DISABLE);
                PDM_Set_state_PWR_CH(eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, DISABLE);
                PDM_Set_state_PWR_CH(eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, DISABLE);
                LL_mDelay(80);
                PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE);
                PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE);
                PDM_Set_state_PWR_CH(eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, ENABLE);
                PDM_Set_state_PWR_CH(eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, ENABLE);
                CAN_init_eps(CAN1);
                CAN_init_eps(CAN2);
                eps_p.eps_serv_ptr->Was_Reboot_PWR_CAN = 1;

            }else{
                //Enable passive CPU if disabled
                if (eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU == ENABLE){
                    PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
                    PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
                    eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = DISABLE;
                }

                //Switch active CPU because CANm and CANb is ERROR
                if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
                    eps_p.eps_serv_ptr->Set_Active_CPU = CPUbackup_Active;

                }else if(eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUbackup){
                    eps_p.eps_serv_ptr->Set_Active_CPU = CPUmain_Active;
                }
                eps_p.eps_serv_ptr->Req_SW_Active_CPU = 1;

                eps_p.eps_serv_ptr->Was_Reboot_PWR_CAN = 0;
            }
        }else{
            eps_p.eps_serv_ptr->Was_Reboot_PWR_CAN = 0;
        }

        CAN1_exchange_data_flag = 0;
        CAN2_exchange_data_flag = 0;
        PMM_Start_Time_Check_CAN = SysTick_Counter;
    }

}



/** @brief  Uart ports EPS damage control and get reboot counter passive CPU for Active CPU
	@param  *UART_Main_eps_comm - pointer to Main UART port struct with get data.
    @param  *UART_Backup_eps_comm - pointer to Backup UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
ErrorStatus PMM_Damage_Check_UART_m_b_ActiveCPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

    int8_t error_status = SUCCESS;
    int8_t error_UART = ERROR_N; //0-OK -1-ERROR_N
    uint32_t i = 0;

    uint32_t old_val_reboot_counter_CPUm = 0;
    uint32_t old_val_reboot_counter_CPUb = 0;

    old_val_reboot_counter_CPUb = eps_p.eps_pmm_ptr->reboot_counter_CPUb;
    old_val_reboot_counter_CPUm = eps_p.eps_pmm_ptr->reboot_counter_CPUm;

    if( eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU == DISABLE ){
        //Main port UART
        error_UART = ERROR_N;
        i = 0;
        while((error_UART != SUCCESS) && (i < pmm_uart_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

            error_UART = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_Reboot_count, 1, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p);

            if( error_UART != SUCCESS ){
                i++;
                LL_mDelay(pmm_uart_delay_att_conn);
            }
        }

        error_status += error_UART;

        //Backup port UART
        error_UART = ERROR_N;
        i = 0;
        while((error_UART != SUCCESS) && (i < pmm_uart_attempt_conn)){//Enable/Disable INPUT Efuse power channel.

            error_UART = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_Reboot_count, 2, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p);

            if( error_UART != SUCCESS ){
                i++;
                LL_mDelay(pmm_uart_delay_att_conn);
            }
        }

        error_status += error_UART;

        if( old_val_reboot_counter_CPUb != eps_p.eps_pmm_ptr->reboot_counter_CPUb || old_val_reboot_counter_CPUm != eps_p.eps_pmm_ptr->reboot_counter_CPUm ){
            eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
        }

        //Set Error Flag to PMM struct
        if( UART_Main_eps_comm->error_port_counter == 0 ){
            eps_p.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
        }else if( UART_Main_eps_comm->error_port_counter >= UART_EPS_ERROR_Threshold ){
            UART_Main_eps_comm->error_port_counter = UART_EPS_ERROR_Threshold;
            eps_p.eps_pmm_ptr->Error_UART_port_M = ERROR;
        }

        if( UART_Backup_eps_comm->error_port_counter == 0 ){
            eps_p.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
        }else if( UART_Backup_eps_comm->error_port_counter >= UART_EPS_ERROR_Threshold ){
            UART_Backup_eps_comm->error_port_counter = UART_EPS_ERROR_Threshold;
            eps_p.eps_pmm_ptr->Error_UART_port_B = ERROR;
        }

    }else{
        eps_p.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
        eps_p.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
        error_status = SUCCESS;
    }

    if(error_status != SUCCESS ){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief  Uart ports EPS damage control for Passive CPU
	@param  *UART_Main_eps_comm - pointer to Main UART port struct with get data.
    @param  *UART_Backup_eps_comm - pointer to Backup UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
void PMM_Damage_Check_UART_m_b_PassiveCPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

    if( ( (uint32_t)(SysTick_Counter - PMM_Start_Time_Check_UART_PassiveCPU) ) > (uint32_t)PMM_UART_Exch_Data_Gap ){

        if( UART_Main_eps_comm->data_exchange_flag == 0 ){
            eps_p.eps_pmm_ptr->Error_UART_port_M = ERROR;
        }else{
            eps_p.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
        }

        if( UART_Backup_eps_comm->data_exchange_flag == 0 ){
            eps_p.eps_pmm_ptr->Error_UART_port_B = ERROR;
        }else{
            eps_p.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
        }

        UART_Main_eps_comm->data_exchange_flag = 0;
        UART_Backup_eps_comm->data_exchange_flag = 0;

        PMM_Start_Time_Check_UART_PassiveCPU = SysTick_Counter;
    }
}


/** @brief CANmain and CANbackup ports power off protection.
           Enable power CANmain and CANbackup if all is power off.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
void PMM_Portecion_PWR_OFF_CAN_m_b( _EPS_Param eps_p ){

    if( eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain == DISABLE && eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup == DISABLE ){
        PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE);
        PMM_Set_state_PWR_CH(eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE);
        CAN_Var4_fill(eps_p);
    }
}


/** @brief BRC main  and backup ports power off protection.
           Enable power BRCmain and BRCbackup if all is power off.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
void PMM_Portecion_PWR_OFF_BRC_m_b( _EPS_Param eps_p ){
    //TODO Добавить проверку флага что батареи не заряжены в будущем (после первого полета).
    if( ( eps_p.eps_pdm_ptr->PWR_Channel[PDM_PWR_Channel_3].State_eF_in == DISABLE || eps_p.eps_pdm_ptr->PWR_Channel[PDM_PWR_Channel_3].State_eF_out == DISABLE ) &&
            ( eps_p.eps_pdm_ptr->PWR_Channel[PDM_PWR_Channel_4].State_eF_in == DISABLE || eps_p.eps_pdm_ptr->PWR_Channel[PDM_PWR_Channel_4].State_eF_out == DISABLE)){
        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr,  PDM_PWR_Channel_3, ENABLE );
        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr,  PDM_PWR_Channel_4, ENABLE );
        CAN_Var4_fill(eps_p);
    }

}


/** @brief ReInit EPS (PMM, PAM, PDM, PBM)
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
void PMM_ReInit_EPS( _EPS_Param eps_p ){

    static uint32_t start_time = 0;

    if( ( (uint32_t)( SysTick_Counter - start_time) ) > PMM_ReInit_Time_Gap ){

        PMM_init(eps_p.eps_pmm_ptr);

        //Branch for Active CPU
        if( (eps_p.eps_pmm_ptr->Active_CPU == CPUmain_Active && eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain) ||
                (eps_p.eps_pmm_ptr->Active_CPU == CPUbackup_Active && eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){

            PDM_init(eps_p.eps_pdm_ptr);
            PAM_init(eps_p.eps_pam_ptr);
            PBM_Re_Init(eps_p.eps_pbm_ptr);

        //Branch for Passive CPU
        }else{

            CAN_DeInit_eps(CAN1);
            CAN_DeInit_eps(CAN2);

            I2C4_DeInit();
            PWM_DeInit_Ch3_Ch4();
        }

        start_time = SysTick_Counter;
    }
}


/** @brief  Disable power subsystem if we reached ZERO Energy level.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None.
*/
void PMM_ZERO_Energy_PWR_OFF_SubSystem( _EPS_Param eps_p ){

    uint8_t num_pwr_ch;

    if( (eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < PBM_ZERO_ENERGY_EDGE )  &&  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val < PBM_ZERO_ENERGY_EDGE ){

        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_VBAT1_eF1, DISABLE );
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_VBAT1_eF2, DISABLE );
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_VBAT2_eF1, DISABLE );
        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, DISABLE );

        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_1, DISABLE );
        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_2, DISABLE );

        //Disable TM SP power channels.
        for( num_pwr_ch = 0; num_pwr_ch < PAM_PWR_TM_SP_Ch_quantity; num_pwr_ch++ ){
            PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, num_pwr_ch, DISABLE);
        }

        CAN_Var4_fill(eps_p);
    }

}
