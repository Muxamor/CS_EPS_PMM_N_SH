#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_tim.h"
#include "median_filter.h"
#include "SetupPeriph.h"
#include "tim_pwm.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PDM/pdm_config.h"
#include "PDM/pdm_ctrl.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_deploy.h"
#include "PAM/pam_ctrl.h"
#include "PAM/pam_init.h"
#include "PAM/pam_sp_init.h"
#include "uart_eps_comm.h"
#include "CAND/filter2D.h"
#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include  <stdio.h>

//extern struct CAN_IVar5 CAN_IVar5_telemetry;
//extern struct CAN_IVar4  CAN_IVar4_RegCmd;

extern _UART_EPS_COMM *UART_M_eps_comm;
extern _UART_EPS_COMM *UART_B_eps_comm;

void CAN_Var4_cmd_parser( _EPS_Param eps_p ){

	uint16_t number_cmd_reg = 0;

	NVIC_DisableIRQ(CAN1_RX0_IRQn);
	NVIC_DisableIRQ(CAN2_RX0_IRQn);

	for( number_cmd_reg = 0; number_cmd_reg < CAN_cmd_Buff.length; number_cmd_reg++ ){

	    switch (CAN_cmd_Buff.CAN_IN_BUFF[number_cmd_reg]){

                case CAN_Constant_mode_offset://Constant mode command
                    if( CAN_IVar4_RegCmd.CAN_Constant_mode == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Enable constant mode\n", CAN_Constant_mode_offset);
                        #endif
                        eps_p.eps_pmm_ptr->CAN_constatnt_mode = 1;
                        CAN_Var5_fill_telemetry_const();

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Disable constant mode\n", CAN_Constant_mode_offset);
                        #endif
                        eps_p.eps_pmm_ptr->CAN_constatnt_mode = 0;
                        CAN_Var5_fill_telemetry( eps_p );
                    }
                    eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
                    break;

                case CAN_EPS_Mode_offset:
                    if(CAN_IVar4_RegCmd.CAN_EPS_Mode == 0xFF ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> EPS in SERVICE mode\n", CAN_EPS_Mode_offset);
                        #endif
                        eps_p.eps_pmm_ptr->EPS_Mode = EPS_SERVICE_MODE;
                        eps_p.eps_pmm_ptr->Deploy_stage = 0;

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> EPS in COMBAT Mode\n", CAN_EPS_Mode_offset);
                        #endif
                            eps_p.eps_pmm_ptr->EPS_Mode = EPS_COMBAT_MODE;
                    }
                    eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
                    break;

                case CAN_Switch_active_CPU_offset: // Switch active CPU (CPUmain active or CPUbackup
                    if( CAN_IVar4_RegCmd.CAN_Switch_active_CPU == 0x00 ){
						#ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Set active CPUmain\n", CAN_Switch_active_CPU_offset);
						#endif
                        if( eps_p.eps_pmm_ptr->Active_CPU != CAN_IVar4_RegCmd.CAN_Switch_active_CPU ){
                            eps_p.eps_serv_ptr->Req_SW_Active_CPU = 1;
                            eps_p.eps_serv_ptr->Set_Active_CPU = CPUmain_Active;
                        }
                    }else if( CAN_IVar4_RegCmd.CAN_Switch_active_CPU == 0x01 ){
						#ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Set active CPUbackup\n", CAN_Switch_active_CPU_offset);
						#endif
                        if( eps_p.eps_pmm_ptr->Active_CPU != CAN_IVar4_RegCmd.CAN_Switch_active_CPU ){
                            eps_p.eps_serv_ptr->Req_SW_Active_CPU = 1;
                            eps_p.eps_serv_ptr->Set_Active_CPU = CPUbackup_Active;
                        }
                    }
                    break;

                case  CAN_Reset_to_default_offset: // Switch active CPU (CPUmain active or CPUbackup
                    if( CAN_IVar4_RegCmd.CAN_Reset_to_default == 0x01 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Reset to default settings\n",  CAN_Reset_to_default_offset);
                        #endif
                        eps_p.eps_pmm_ptr->Deploy_stage = 0;
                        eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF1 = DISABLE;
                        eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2 = DISABLE;
                        eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF1 = DISABLE;
                        eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2 = DISABLE;
                        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 = 0;
                        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn = 0;
                        eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn = 0;

                        eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
                        CAN_IVar4_RegCmd.CAN_Reset_to_default = 0x00;
                    }
                    break;

                case CAN_Perform_Deploy_offset: //Perform Deploy CubeSat
                    if( CAN_IVar4_RegCmd.CAN_Perform_Deploy != 0 ){
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE );
                        LL_mDelay( 5 ); //Delay for startup power
                    }

                    if( CAN_IVar4_RegCmd.CAN_Perform_Deploy == 0x01 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Perform Deploy Channel 1\n", CAN_Perform_Deploy_offset );
                        #endif
                        PMM_Deploy_Burn_Procedure( eps_p, PMM_PWR_Deploy_Ch1);

                    }else if( CAN_IVar4_RegCmd.CAN_Perform_Deploy == 0x02 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Perform Deploy Channel 2\n", CAN_Perform_Deploy_offset );
                        #endif
                        PMM_Deploy_Burn_Procedure( eps_p, PMM_PWR_Deploy_Ch2);

                    }else if( CAN_IVar4_RegCmd.CAN_Perform_Deploy == 0x03 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Perform Deploy Channel 3\n", CAN_Perform_Deploy_offset );
                        #endif
                        PMM_Deploy_Burn_Procedure( eps_p, PMM_PWR_Deploy_Ch3);

                    }else if( CAN_IVar4_RegCmd.CAN_Perform_Deploy == 0x04 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Perform Deploy Channel 4\n", CAN_Perform_Deploy_offset );
                        #endif
                        PMM_Deploy_Burn_Procedure( eps_p, PMM_PWR_Deploy_Ch4);

                    }else if( CAN_IVar4_RegCmd.CAN_Perform_Deploy == 0xFE ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> Reset Deploy stage = 0\n", CAN_Perform_Deploy_offset );
                        #endif
                        eps_p.eps_pmm_ptr->Deploy_stage = 0;
                    }

                    CAN_IVar4_RegCmd.CAN_Perform_Deploy = 0x00;
                    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );
                    PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE );
                    break;

                case CAN_PMM_PWR_OFF_Passive_CPU_offset: // Power off Passive MCU
                    if ( CAN_IVar4_RegCmd.CAN_PMM_PWR_OFF_Passive_CPU == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Enable power OFF Passive CPU\n", CAN_PMM_PWR_OFF_Passive_CPU_offset);
                        #endif
                        PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = ENABLE;
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Disable power OFF Passive CPU\n", CAN_PMM_PWR_OFF_Passive_CPU_offset);
                        #endif
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        PWM_DeInit_Ch3_Ch4( );
                        eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = DISABLE;
                    }
                    eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
                    break;

                case CAN_PMM_Reboot_Passive_CPU_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PMM_Reboot_Passive_CPU == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d ->Reboot Passive CPU\n", CAN_PMM_Reboot_Passive_CPU_offset);
                        #endif
                        PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        LL_mDelay(50);
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        PWM_DeInit_Ch3_Ch4( );
                        CAN_IVar4_RegCmd.CAN_PMM_Reboot_Passive_CPU = 0x00;
                    }
                    break;

                case CAN_PMM_PWR_CAN_main_offset://Enable/Disable Power main CAN
                    if ( CAN_IVar4_RegCmd.CAN_PMM_PWR_CAN_main == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power main CAN \n", CAN_PMM_PWR_CAN_main_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE  power main CAN\n", CAN_PMM_PWR_CAN_main_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANmain, DISABLE);
                    }
                    break;

                case CAN_PMM_PWR_CAN_backup_offset://Enable/Disable Power main CAN
                    if ( CAN_IVar4_RegCmd.CAN_PMM_PWR_CAN_backup == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power main CAN \n", CAN_PMM_PWR_CAN_backup_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE  power main CAN\n", CAN_PMM_PWR_CAN_backup_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,PMM_PWR_Ch_CANbackup, DISABLE);
                    }
                    break;

                case CAN_PWR_VBAT1_key1_offset: //Enable/Disable power channel VBAT1 eF1
                    if( CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1_key1  == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power VBAT1 key1\n", CAN_PWR_VBAT1_key1_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF1, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power VBAT1 key1\n", CAN_PWR_VBAT1_key1_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF1, DISABLE );
                    }
                    break;

                case CAN_PWR_VBAT1_key2_offset: //Enable/Disable power channel VBAT1 eF2
                    if( CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1_key2  == 0x01 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> ENABLE power VBAT1 key2\n", CAN_PWR_VBAT1_key2_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF2, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE power VBAT1 key2\n", CAN_PWR_VBAT1_key2_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF2, DISABLE );
                    }
                    break;

                case CAN_PWR_VBAT2_key1_offset: //Enable/Disable power channel VBAT2 eF1
                    if( CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2_key1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power VBAT2\n", CAN_PWR_VBAT2_key1_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF1, ENABLE );
                       // PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF2, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power VBAT2\n", CAN_PWR_VBAT2_key1_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF1, DISABLE );
                        //PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF2, DISABLE );
                    }
                    break;

                case CAN_PWR_VBAT2_key2_offset: //Enable/Disable power channel VBAT2 eF2
                    if( CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2_key2 == 0x01 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> ENABLE power VBAT2\n", CAN_PWR_VBAT2_key2_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF2, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE power VBAT2\n", CAN_PWR_VBAT2_key2_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF2, DISABLE );
                    }
                    break;

                case CAN_PMM_PWR_PBM_Logic_offset://Enable/Disable PMM Power PBM Logic
                    if ( CAN_IVar4_RegCmd.CAN_PMM_PWR_PBM_Logic == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE PMM  power PBM Logic\n", CAN_PMM_PWR_PBM_Logic_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_PBMs_Logic, ENABLE );
                        LL_mDelay(3);
                        PBM_T1_Init( eps_p.eps_pbm_ptr );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE PMM  power PBM Logic\n", CAN_PMM_PWR_PBM_Logic_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_PBMs_Logic, DISABLE );
                    }
                    break;

                case CAN_PWR_CH1_offset: //Enable/Disable power channel 1 (SOP1)
                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1 == 0x01 && PDM_PCB_Assembled_PWR_CH1 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 1\n", CAN_PWR_CH1_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_1, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 1\n", CAN_PWR_CH1_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_1, DISABLE );
                        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1 = 0x00;
                    }
                    break;

                case CAN_PWR_CH2_offset: //Enable/Disable power channel 2 (SOP2)
                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2 == 0x01 && PDM_PCB_Assembled_PWR_CH2){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 2\n", CAN_PWR_CH2_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_2, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 2\n", CAN_PWR_CH2_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_2, DISABLE );
                        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2 = 0x00;
                    }
                    break;

                case CAN_PWR_CH3_offset: //Enable/Disable power channel 3 (BRK1)
                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3 == 0x01 && PDM_PCB_Assembled_PWR_CH3 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 3\n", CAN_PWR_CH3_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 3\n", CAN_PWR_CH3_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, DISABLE );
                        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3 = 0x00;
                    }
                    break;

                case CAN_PWR_CH4_offset: //Enable/Disable power channel 4 (BRK2)  uint8_t CAN_PDM_PWR_CH4;
                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4 == 0x01 && PDM_PCB_Assembled_PWR_CH4 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 4\n", CAN_PWR_CH4_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 4\n", CAN_PWR_CH4_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, DISABLE );
                        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4 = 0x00;
                    }
                    break;

                case CAN_PWR_CH5_offset: //Enable/Disable power channel 5
                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5 == 0x01 && PDM_PCB_Assembled_PWR_CH5 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 5\n", CAN_PWR_CH5_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_5, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 5\n", CAN_PWR_CH5_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_5, DISABLE );
                        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5 = 0x00;
                    }
                    break;

                case CAN_PWR_CH6_offset: //Enable/Disable power channel 6
                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6 == 0x01 && PDM_PCB_Assembled_PWR_CH6 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 6\n", CAN_PWR_CH6_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_6, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 6\n", CAN_PWR_CH6_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_6, DISABLE );
                        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6 = 0x00;
                    }
                    break;

                case CAN_PAM_PWR_DC_DC_offset: //Enable/Disable PAM Power DC_DC source // PAM_data
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_DC_DC == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE PAM Power DC_DC source \n", CAN_PAM_PWR_DC_DC_offset);
                        #endif
                        PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_DC_DC, ENABLE);
                        PAM_init(eps_p.eps_pam_ptr);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE PAM Power DC_DC source \n", CAN_PAM_PWR_DC_DC_offset);
                        #endif
                        PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_DC_DC, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_LDO_offset: //Enable/Disable PAM Power LDO source // PAM_data
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_LDO == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE PAM Power LDO source \n", CAN_PAM_PWR_LDO_offset);
                        #endif
                        PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_LDO, ENABLE);
                        PAM_init(eps_p.eps_pam_ptr);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE PAM Power LDO source \n", CAN_PAM_PWR_LDO_offset);
                        #endif
                        PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_LDO, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_TM_SP_CH1_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power TM SP Channel 1 \n", CAN_PAM_PWR_TM_SP_CH1_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch1, ENABLE);
                        PAM_SP_init( eps_p.eps_pam_ptr, PAM_SP1);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power TM SP Channel 1\n", CAN_PAM_PWR_TM_SP_CH1_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch1, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_TM_SP_CH2_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power TM SP Channel 2 \n", CAN_PAM_PWR_TM_SP_CH2_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch2, ENABLE);
                        PAM_SP_init( eps_p.eps_pam_ptr, PAM_SP2);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power TM SP Channel 2\n", CAN_PAM_PWR_TM_SP_CH2_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch2, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_TM_SP_CH3_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH3 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power TM SP Channel 3 \n", CAN_PAM_PWR_TM_SP_CH3_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch3, ENABLE);
                        PAM_SP_init( eps_p.eps_pam_ptr, PAM_SP3);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power TM SP Channel 3\n", CAN_PAM_PWR_TM_SP_CH3_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch3, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_TM_SP_CH4_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH4 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power TM SP Channel 4\n", CAN_PAM_PWR_TM_SP_CH4_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch4, ENABLE);
                        PAM_SP_init( eps_p.eps_pam_ptr, PAM_SP4);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power TM SP Channel 4\n", CAN_PAM_PWR_TM_SP_CH4_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch4, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_TM_SP_CH5_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH5 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power TM SP Channel 5\n", CAN_PAM_PWR_TM_SP_CH5_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch5, ENABLE);
                        PAM_SP_init( eps_p.eps_pam_ptr, PAM_SP5);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power TM SP Channel 5\n", CAN_PAM_PWR_TM_SP_CH5_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch5, DISABLE);
                    }
                    break;

                case CAN_PAM_PWR_TM_SP_CH6_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH6 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power TM SP Channel 6\n", CAN_PAM_PWR_TM_SP_CH6_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch6, ENABLE);
                        PAM_SP_init( eps_p.eps_pam_ptr, PAM_SP6);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power TM SP Channel 6\n", CAN_PAM_PWR_TM_SP_CH6_offset);
                        #endif
                        PAM_Set_state_PWR_TM_SP_CH( eps_p.eps_pam_ptr, PAM_PWR_TM_SP_Ch6, DISABLE);
                    }
                    break;

                case CAN_AB1_Charge_key_1_offset: //Enable/Disable Charge AB1 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB1 branch 1\n", CAN_AB1_Charge_key_1_offset);
                        #endif
                        // change!!!!!! Morsin A.A.
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB1 branch 1\n", CAN_AB1_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB1_Discharge_key_1_offset: //Enable/Disable Discharge AB1 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB1 branch 1\n", CAN_AB1_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB1 branch 1\n", CAN_AB1_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB1_Charge_key_2_offset: //Enable/Disable Charge AB1 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB1 branch 2\n", CAN_AB1_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB1 branch 2\n", CAN_AB1_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB1_Discharge_key_2_offset: //Enable/Disable Discharge AB1 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB1 branch 2\n", CAN_AB1_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB1 branch 2\n", CAN_AB1_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB2_Charge_key_1_offset: //Enable/Disable Charge AB2 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB2 branch 1\n", CAN_AB2_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB2 branch 1\n", CAN_AB2_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB2_Discharge_key_1_offset: //Enable/Disable Discharge AB2 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB2 branch 1\n", CAN_AB2_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB2 branch 1\n", CAN_AB2_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB2_Charge_key_2_offset: //Enable/Disable Charge AB2 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB2 branch 2\n", CAN_AB2_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB2 branch 2\n", CAN_AB2_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB2_Discharge_key_2_offset: //Enable/Disable Discharge AB2 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB2 branch 2\n", CAN_AB2_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB2 branch 2\n", CAN_AB2_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB3_Charge_key_1_offset: //Enable/Disable Charge AB3 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB3 branch 1\n", CAN_AB3_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB3 branch 1\n", CAN_AB3_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB3_Discharge_key_1_offset: //Enable/Disable Discharge AB3 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB3 branch 1\n", CAN_AB3_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB3 branch 1\n", CAN_AB3_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB3_Charge_key_2_offset: //Enable/Disable Charge AB3 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB3 branch 2\n", CAN_AB3_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB3 branch 2\n", CAN_AB3_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB3_Discharge_key_2_offset: //Enable/Disable Discharge AB3 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB3 branch 2\n", CAN_AB3_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB3 branch 2\n", CAN_AB3_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB4_Charge_key_1_offset: //Enable/Disable Charge AB4 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM4_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> ENABLE charge AB4 branch 1\n", CAN_AB4_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE charge AB4 branch 1\n", CAN_AB4_Charge_key_1_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB4_Discharge_key_1_offset: //Enable/Disable Discharge AB4 branch 1 // PBM_data
                    if( CAN_IVar4_RegCmd.CAN_PBM4_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> ENABLE discharge AB4 branch 1\n", CAN_AB4_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE discharge AB4 branch 1\n", CAN_AB4_Discharge_key_1_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB4_Charge_key_2_offset: //Enable/Disable Charge AB4 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> ENABLE charge AB4 branch 2\n", CAN_AB4_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE charge AB4 branch 2\n", CAN_AB4_Charge_key_2_offset);
                        #endif
                        PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_OFF_CHARGE);
                    }
                    break;

                case CAN_AB4_Discharge_key_2_offset: //Enable/Disable Discharge AB4 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> ENABLE discharge AB4 branch 2\n", CAN_AB4_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                        printf("Get comm. reg. %d -> DISABLE discharge AB4 branch 2\n", CAN_AB4_Discharge_key_2_offset);
                        #endif
                        PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB1_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB1 branch 1 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB1 branch 1\n", CAN_AB1_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_HEAT_1, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB1 branch 1\n", CAN_AB1_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_HEAT_1, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB1_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB1 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB1 branch 2\n", CAN_AB1_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_HEAT_2, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB1 branch 2\n", CAN_AB1_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_HEAT_2, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB2_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB2 branch 1 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB2 branch 1\n", CAN_AB2_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_HEAT_1, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB2 branch 1\n", CAN_AB2_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_HEAT_1, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB2_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB2 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB2 branch 2\n", CAN_AB2_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_HEAT_2, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB2 branch 2\n", CAN_AB2_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_HEAT_2, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB3_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB3 branch 1 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB3 branch 1\n", CAN_AB3_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_HEAT_1, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB3 branch 1\n", CAN_AB3_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_HEAT_1, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB3_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB3 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB3 branch 2\n", CAN_AB3_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_HEAT_2, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB3 branch 2\n", CAN_AB3_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_HEAT_2, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB4_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB4 branch 1 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB4 branch 1\n", CAN_AB4_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_HEAT_1, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB4 branch 1\n", CAN_AB4_Heat_Branch1_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_HEAT_1, PBM_T1_OFF_HEAT);
                    }
                    break;

                case CAN_AB4_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB4 branch 2 // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB4 branch 2\n", CAN_AB4_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_HEAT_2, PBM_T1_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB4 branch 2\n", CAN_AB4_Heat_Branch2_offset);
                        #endif
                        PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_HEAT_2, PBM_T1_OFF_HEAT);
                    }
                    break;

                  /*
                     CAN_PBM1_Emerg_Charge_Key_Branch1_offset
                     CAN_PBM1_Emerg_Charge_Key_Branch2_offset
                     CAN_PBM2_Emerg_Charge_Key_Branch1_offset
                     CAN_PBM2_Emerg_Charge_Key_Branch2_offset
                     CAN_PBM3_Emerg_Charge_Key_Branch1_offset
                     CAN_PBM3_Emerg_Charge_Key_Branch2_offset
                     CAN_PBM4_Emerg_Charge_Key_Branch1_offset
                     CAN_PBM4_Emerg_Charge_Key_Branch2_offset

                     CAN_PBM1_Corr_Charge_Level_Branch1_offset
                     CAN_PBM1_Corr_Charge_Level_Branch2_offset
                     CAN_PBM2_Corr_Charge_Level_Branch1_offset
                     CAN_PBM2_Corr_Charge_Level_Branch2_offset
                     CAN_PBM3_Corr_Charge_Level_Branch1_offset
                     CAN_PBM3_Corr_Charge_Level_Branch2_offset
                     CAN_PBM4_Corr_Charge_Level_Branch1_offset
                     CAN_PBM4_Corr_Charge_Level_Branch2_offset

                     CAN_PBM1_Corr_MAX_Charge_Level_Branch1_offset
                     CAN_PBM1_Corr_MAX_Charge_Level_Branch2_offset
                     CAN_PBM2_Corr_MAX_Charge_Level_Branch1_offset
                     CAN_PBM2_Corr_MAX_Charge_Level_Branch2_offset
                     CAN_PBM3_Corr_MAX_Charge_Level_Branch1_offset
                     CAN_PBM3_Corr_MAX_Charge_Level_Branch2_offset
                     CAN_PBM4_Corr_MAX_Charge_Level_Branch1_offset
                     CAN_PBM4_Corr_MAX_Charge_Level_Branch2_offset

                     CAN_PBM1_Reset_Branch1_offset
                     CAN_PBM1_Reset_Branch2_offset
                     CAN_PBM2_Reset_Branch1_offset
                     CAN_PBM2_Reset_Branch2_offset
                     CAN_PBM3_Reset_Branch1_offset
                     CAN_PBM3_Reset_Branch2_offset
                     CAN_PBM4_Reset_Branch1_offset
                     CAN_PBM4_Reset_Branch2_offset
                  */
                default:
					break;
			}

		}

	CAN_cmd_Buff.length = 0;
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	NVIC_EnableIRQ(CAN2_RX0_IRQn);
}


void CAN_Var5_fill_telemetry( _EPS_Param eps_p ){

	uint8_t num_pwr_ch = 0;
	uint8_t move_bit_pos = 0;
	uint8_t PBM_Number = 0, Branch_Number = 0;

	// -------------------  ТМИ 0  ------------------ //
	CAN_IVar5_telemetry.CAN_TMI0_version                                = 0x0002;

	//PMM module
	CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS_Const_Mode                     = eps_p.eps_pmm_ptr->CAN_constatnt_mode;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS__Mode                          = eps_p.eps_pmm_ptr->EPS_Mode;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Switch_Active_CPU                  = eps_p.eps_pmm_ptr->Active_CPU;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_OFF_PWR_Passive_CPU                = eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Temp_Sensor                        = eps_p.eps_pmm_ptr->Temp_sensor;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_PWR_Ch_State_BitMask               = (eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain) | (eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup << 1) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF1 << 2) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2 << 3) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF1 << 4) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2 << 5) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic << 6) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic << 7) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Power << 8) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Power << 9) | (eps_p.eps_pmm_ptr->PWR_Ch_State_5V_Bus << 10) | (eps_p.eps_pmm_ptr->PWR_Ch_State_3_3V_Bus << 11) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Ch_State_I2C_Bus << 12);

	CAN_IVar5_telemetry.CAN_TMIx_PMM_PWR_PG_BitMask                     = (eps_p.eps_pmm_ptr->PWR_Ch_PG_CANmain) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_CANbackup << 1) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat1_eF1 << 2) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat1_eF2 << 3) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat2_eF1 << 4) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat2_eF2 << 5) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_PBMs_Logic << 6) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Supply_Main_PG << 7) | (eps_p.eps_pmm_ptr->PWR_Supply_Backup_PG << 8) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Deploy_LP << 9) |
	                                                                      (eps_p.eps_pmm_ptr->PWR_Ch_PG_5V_Bus << 10) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_3_3V_Bus << 11) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_I2C_Bus  << 12);

	CAN_IVar5_telemetry.CAN_TMIx_PMM_Error_BitMask                      = (eps_p.eps_pmm_ptr->Error_TMP1075_sensor) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANmain << 1) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANbackup << 2) |
	                                                                      (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 << 3) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 << 4) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 << 5) |
	                                                                      (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 << 6) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_PBMs_Logic << 7) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Deploy_Logic << 8) |
	                                                                      (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Deploy_Power << 9) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_5V_Bus << 10) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_3_3V_Bus << 11) |
	                                                                      (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_I2C_Bus << 12) | (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF1 << 13) | (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF2 << 14) |
	                                                                      (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF1 << 15) | (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF2 << 16) | (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext1 << 17) |
	                                                                      (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext1 << 18) | (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext2 << 19) | (eps_p.eps_pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon << 20) |
	                                                                      (eps_p.eps_pmm_ptr->Error_I2C_Deploy_GPIO_Ext << 21) | (eps_p.eps_pmm_ptr->Error_I2C_Deploy_ADC << 22) | (eps_p.eps_pmm_ptr->Error_FRAM1 << 23) |
	                                                                      (eps_p.eps_pmm_ptr->Error_FRAM2 << 24) | (eps_p.eps_pmm_ptr->Error_UART_port_M << 25) | (eps_p.eps_pmm_ptr->Error_UART_port_B << 26) | (eps_p.eps_pmm_ptr->Error_CAN_port_M << 27) |
	                                                                      (eps_p.eps_pmm_ptr->Error_CAN_port_B << 28);

	CAN_IVar5_telemetry.CAN_TMIx_PMM_Reboot_Counter_Main_CPU            =  eps_p.eps_pmm_ptr->reboot_counter_CPUm;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Reboot_Counter_Backup_CPU          =  eps_p.eps_pmm_ptr->reboot_counter_CPUb;

	//***
	if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < 4000 || eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF1 == ERROR ){
	    CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage                  =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Voltage_val;
	}else{
	    CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage                  =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val;
	}
	//***
	if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val < 4000 || eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF1 == ERROR ){
	    CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage                  =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Voltage_val;
	}else{
	    CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage                  =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val;
	}
	//---
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage_Aver_10s             =  (uint16_t)( Filtr2Step(CHF_U5, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage ) );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage_Aver_10s             =  (uint16_t)( Filtr2Step(CHF_U6, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage ) );

	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current                      =  (uint16_t)( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Current_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Current_val );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current                      =  (uint16_t)( eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Current_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Current_val );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current_Aver_10s             =  (uint16_t)( Filtr2Step(CHF_I5, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current ) );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current_Aver_10s             =  (uint16_t)( Filtr2Step(CHF_I6, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current ) );

	//uint16_t CAN_TMIx_PMM_Own_Current_Consumption;                  //+68,69  |  const = 0x                   |   Ток потребления модуля PMM
	//uint16_t CAN_TMIx_PMM_Voltage;                                  //+70,71  |  const = 0x                   |   Напряжение питания модуля PMM 3.3V

	//***
	if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < 4000 || eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF1 == ERROR ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val;
	}else{
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val;
	}

	CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PBM_Char_Dischar_Power 		 =  0x0000;
	// change!!!!!! Morsin A.A.
	for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++ ){
		for(Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
			CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PBM_Char_Dischar_Power    =  CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PBM_Char_Dischar_Power +
																					(int16_t) (eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Power);
		}
	}

	CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power 		 =  0x0000;
	for( num_pwr_ch = 0; num_pwr_ch < PAM_PWR_IN_Ch_quantity; num_pwr_ch++ ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power      =  CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power + eps_p.eps_pam_ptr->PWR_IN_Channel[num_pwr_ch].Power_val;
	}


	//TODO добавить потрбление нагревателей
	//***
	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power            =  0x0000;
	for( num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power        =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power + eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Power_val;
	}

	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power            =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Power_val +
	                                                                        eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val;
	//---

	// change!!!!!! Morsin A.A.
	CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_mAh	 		 = 0x0000;
	for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++ ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_mAh		 = CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_mAh + (uint16_t)(eps_p.eps_pbm_ptr[PBM_Number].TotalAbcoluteCapacity_mAh);
	}

	CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_percent 		 = 0x0000;
	for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++ ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_percent     = CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_percent + (uint8_t)(eps_p.eps_pbm_ptr[PBM_Number].TotalRelativeCapacity_Perc / PBM_T1_QUANTITY);
	}


	CAN_IVar5_telemetry.CAN_TMIx_EPS_Deploy_Status                       = (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1) | (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 << 1) |
	                                                                       (eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp << 2) | (eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp << 3) |
	                                                                       (eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn << 4) | (eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn << 5) |
	                                                                       (eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp << 6) | (eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp << 7) |
	                                                                       (eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn << 8) | (eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn << 9) |
	                                                                       (eps_p.eps_pmm_ptr->Deploy_stage << 12);

	CAN_IVar5_telemetry.CAN_TMIx_PMM_Version_FW                          = eps_p.eps_pmm_ptr->Version_FW;

    //PDM Module
    // PDM Power state key bit mask
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask               = 0x0000;
    for( num_pwr_ch = 0, move_bit_pos = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){

        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask           = CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_in << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask           = CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_out << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
    }

    // PDM PG bit mask
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_PG_BitsMask                     = 0x0000;
    for( num_pwr_ch = 0, move_bit_pos = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){

        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_PG_BitsMask                 = CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_PG_BitsMask  | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_in << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_PG_BitsMask                 = CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_PG_BitsMask  | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_out << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
    }

    // PDM Error bit mask
    CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask                       = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask                       = (eps_p.eps_pdm_ptr->Error_I2C_GPIO_Ext1) | (eps_p.eps_pdm_ptr->Error_I2C_GPIO_Ext2 << 1) | (eps_p.eps_pdm_ptr->Error_I2C_MUX << 2) |
                                                                           (eps_p.eps_pdm_ptr->Error_temp_sensor_1 << 3) | (eps_p.eps_pdm_ptr->Error_temp_sensor_2 << 4) | (eps_p.eps_pdm_ptr->Error_temp_sensor_3 << 5) |
                                                                           (eps_p.eps_pdm_ptr->Error_temp_sensor_4 << 6);

    for( num_pwr_ch = 0, move_bit_pos = 7; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){

        CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask                   = CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_in << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask                   = CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_out << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask                   = CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask | ( eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon << move_bit_pos );
        move_bit_pos = move_bit_pos + 1;
    }

    CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor1                        = (eps_p.eps_pdm_ptr->Temp_sensor[0]);
    CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor2                        = (eps_p.eps_pdm_ptr->Temp_sensor[1]);
    CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor3                        = (eps_p.eps_pdm_ptr->Temp_sensor[2]);
    CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor4                        = (eps_p.eps_pdm_ptr->Temp_sensor[3]);
    CAN_IVar5_telemetry.CAN_TMIx_PDM_Median_Temp                         = GetMedian(eps_p.eps_pdm_ptr->Temp_sensor, 4 );

    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage                     = eps_p.eps_pdm_ptr->PWR_Channel[0].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage                     = eps_p.eps_pdm_ptr->PWR_Channel[1].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage                     = eps_p.eps_pdm_ptr->PWR_Channel[2].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage                     = eps_p.eps_pdm_ptr->PWR_Channel[3].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Voltage                     = eps_p.eps_pdm_ptr->PWR_Channel[4].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Voltage                     = 0x0000;

    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage_Aver_10s            = (uint16_t)( Filtr2Step(CHF_U1, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage_Aver_10s            = (uint16_t)( Filtr2Step(CHF_U2, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage_Aver_10s            = (uint16_t)( Filtr2Step(CHF_U3, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage_Aver_10s            = (uint16_t)( Filtr2Step(CHF_U4, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Voltage_Aver_10s            = (uint16_t)( Filtr2Step(CHF_U5, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Voltage ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Voltage_Aver_10s            = 0x0000;

    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current                     = ( eps_p.eps_pdm_ptr->PWR_Channel[0].Current_val );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current                     = ( eps_p.eps_pdm_ptr->PWR_Channel[1].Current_val );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current                     = ( eps_p.eps_pdm_ptr->PWR_Channel[2].Current_val );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current                     = ( eps_p.eps_pdm_ptr->PWR_Channel[3].Current_val );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Current                     = ( eps_p.eps_pdm_ptr->PWR_Channel[4].Current_val );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Current                     =  0x0000;

    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current_Aver_10s            = (int16_t)( Filtr2Step(CHF_I1, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current_Aver_10s            = (int16_t)( Filtr2Step(CHF_I2, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current_Aver_10s            = (int16_t)( Filtr2Step(CHF_I3, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current_Aver_10s            = (int16_t)( Filtr2Step(CHF_I4, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Current_Aver_10s            = (int16_t)( Filtr2Step(CHF_I5, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Current ) );
    CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Current_Aver_10s            = 0x0000;
    CAN_IVar5_telemetry.CAN_TMI0_Reserved[0]                             = 0x00;

    // -------------------  ТМИ 1  ------------------ //
    CAN_IVar5_telemetry.CAN_TMI1_version                                 = 0x0002;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask                = (eps_p.eps_pam_ptr->State_DC_DC) | (eps_p.eps_pam_ptr->State_LDO << 1);

    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_PWR_TM_SP_Ch_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask  = CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask  | ( eps_p.eps_pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_PG_BitMask                      = (eps_p.eps_pam_ptr->PG_DC_DC) | (eps_p.eps_pam_ptr->PG_LDO << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_PWR_TM_SP_Ch_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_PG_BitMask =  CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_PG_BitMask | ( eps_p.eps_pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].PG_eF_out << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Error_BitMask                   = (eps_p.eps_pam_ptr->Error_State_DC_DC) | (eps_p.eps_pam_ptr->Error_State_LDO << 1) | (eps_p.eps_pam_ptr->Error_I2C_GPIO_Ext << 2) |
                                                                           (eps_p.eps_pam_ptr->Error_I2C_MUX_1 << 3) | (eps_p.eps_pam_ptr->Error_I2C_MUX_2 << 4) | (eps_p.eps_pam_ptr->Error_temp_sensor_1 << 5) |
                                                                           (eps_p.eps_pam_ptr->Error_temp_sensor_2 << 6) | (eps_p.eps_pam_ptr->Error_temp_sensor_3 << 7) | (eps_p.eps_pam_ptr->Error_temp_sensor_4 << 8);
    for( num_pwr_ch = 0, move_bit_pos = 9; num_pwr_ch < PAM_PWR_TM_SP_Ch_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Error_BitMask               = CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Error_BitMask | ( eps_p.eps_pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].Error_State_eF_out << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor1                        = eps_p.eps_pam_ptr->Temp_sensor[0];
    CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor2                        = eps_p.eps_pam_ptr->Temp_sensor[1];
    CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor3                        = eps_p.eps_pam_ptr->Temp_sensor[2];
    CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor4                        = eps_p.eps_pam_ptr->Temp_sensor[3];
    CAN_IVar5_telemetry.CAN_TMIx_PAM_Median_Temp                         = GetMedian(eps_p.eps_pam_ptr->Temp_sensor, 4 );

    for( num_pwr_ch = 0, move_bit_pos = 0; num_pwr_ch <PAM_PWR_IN_Ch_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_ID_State_BitsMask      = CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_ID_State_BitsMask | (eps_p.eps_pam_ptr->PWR_IN_Channel[num_pwr_ch].State_ID_In << move_bit_pos);
    }

    for( num_pwr_ch = 0, move_bit_pos = 0; num_pwr_ch <PAM_PWR_IN_Ch_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_Error_BitsMask         = CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_Error_BitsMask | (eps_p.eps_pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon << move_bit_pos);
    }

    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh1_Voltage                   = eps_p.eps_pam_ptr->PWR_IN_Channel[0].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh2_Voltage                   = eps_p.eps_pam_ptr->PWR_IN_Channel[1].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh3_Voltage                   = eps_p.eps_pam_ptr->PWR_IN_Channel[2].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh4_Voltage                   = eps_p.eps_pam_ptr->PWR_IN_Channel[3].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT1_Voltage    				 = eps_p.eps_pam_ptr->PWR_IN_Channel[4].Voltage_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT2_Voltage					 = eps_p.eps_pam_ptr->PWR_IN_Channel[5].Voltage_val;

    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh1_Current                   = eps_p.eps_pam_ptr->PWR_IN_Channel[0].Current_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh2_Current                   = eps_p.eps_pam_ptr->PWR_IN_Channel[1].Current_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh3_Current                   = eps_p.eps_pam_ptr->PWR_IN_Channel[2].Current_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh4_Current                   = eps_p.eps_pam_ptr->PWR_IN_Channel[3].Current_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT1_Current    				 = eps_p.eps_pam_ptr->PWR_IN_Channel[4].Current_val;
    CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT1_Current					 = eps_p.eps_pam_ptr->PWR_IN_Channel[5].Current_val;

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }

    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor1                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[0];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor2                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[1];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor3                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[2];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor4                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[3];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor1                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[0];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor2                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[1];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor3                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[2];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor4                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[3];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor1                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[0];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor2                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[1];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor3                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[2];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor4                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[3];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor1                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[0];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor2                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[1];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor3                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[2];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor4                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[3];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor1                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value[0];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor2                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value[1];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor3                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value[2];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor4                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value[3];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor1                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value[0];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor2                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value[1];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor3                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value[2];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor4                 = eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value[3];
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Median_Temp                  = GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value, PAM_SP1_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Median_Temp                  = GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value, PAM_SP2_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Median_Temp                  = GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value, PAM_SP3_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Median_Temp                  = GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value, PAM_SP4_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Median_Temp                  = GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value, PAM_SP5_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Median_Temp                  = GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value, PAM_SP6_temp_sens_quantity );

    // -------------------  ТМИ 2  ------------------ //


    // PBM //
    /*
    uint16_t CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask;           //+ |  const = 0x                   |
    uint16_t CAN_TMIx_PBM_Chrg_Dichrg_Key_Status_BitMask;           //+ |  const = 0x                   |
    uint8_t CAN_TMIx_PBM_Thermostat_ComReg_BitMask;                 //+ |  const = 0x                   | Команда на включение Термостат
    uint8_t CAN_TMIx_PBM_Heating_Element_State_BitMask;             //+ |  const = 0x                       | Текущее состояние Термостата
    uint8_t CAN_TMIx_PBM_Wreck_Charge_Key_State_BitMask;            //+ |  const = 0x                   | Ключи аварийной зарядки
    uint16_t CAN_TMIx_PBM1_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM2_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM3_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM4_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM1_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM1_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM2_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM2_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM3_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM3_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM4_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM4_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска

    uint8_t  CAN_TMIx_PBM1_Branch1_Charge_Level_Percent;            //+     |  const = 0x                |  Уровень заряда АБ1 ветвь 1
    uint8_t  CAN_TMIx_PBM1_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ1 ветвь 2
    uint8_t  CAN_TMIx_PBM2_Branch1_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ2 ветвь 1
    uint8_t  CAN_TMIx_PBM2_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ2 ветвь 2
    uint8_t  CAN_TMIx_PBM3_Branch1_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ3 ветвь 1
    uint8_t  CAN_TMIx_PBM3_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ3 ветвь 2
    uint8_t  CAN_TMIx_PBM4_Branch1_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ4 ветвь 1
    uint8_t  CAN_TMIx_PBM4_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ4 ветвь 2


    uint16_t CAN_TMIx_PBM1_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ1 ветвь 1
    uint16_t CAN_TMIx_PBM1_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ1 ветвь 1
    uint16_t CAN_TMIx_PBM2_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ2 ветвь 2
    uint16_t CAN_TMIx_PBM2_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ2 ветвь 2
    uint16_t CAN_TMIx_PBM3_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ3 ветвь 2
    uint16_t CAN_TMIx_PBM3_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ3 ветвь 2
    uint16_t CAN_TMIx_PBM4_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ4 ветвь 2
    uint16_t CAN_TMIx_PBM4_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ4 ветвь 2

    int16_t CAN_TMIx_PBM1_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ1 ветвь 1
    int16_t CAN_TMIx_PBM1_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ1 ветвь 2
    int16_t CAN_TMIx_PBM2_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ2 ветвь 1
    int16_t CAN_TMIx_PBM2_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ2 ветвь 2
    int16_t CAN_TMIx_PBM3_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ3 ветвь 1
    int16_t CAN_TMIx_PBM3_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ3 ветвь 2
    int16_t CAN_TMIx_PBM4_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ4 ветвь 1
    int16_t CAN_TMIx_PBM4_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ4 ветвь 2

    uint16_t CAN_TMIx_PBM1_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM1_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM1_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM1_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 2 аккумулятор 2
    uint16_t CAN_TMIx_PBM2_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM2_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM2_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM2_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 2 аккумулятор 2
    uint16_t CAN_TMIx_PBM3_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM3_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM3_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM3_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 2
    uint16_t CAN_TMIx_PBM4_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM4_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM4_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM4_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 2

    int8_t  CAN_PBM1_Temp_Controller1;                              //+     |  const = 0x               |   АБ1 датчик контроллера 1
    int8_t  CAN_PBM1_Temp_Controller2;                              //+     |  const = 0x               |   АБ1 датчик контроллера 2
    int8_t  CAN_PBM1_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ1 датчик на плате 1
    int8_t  CAN_PBM1_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ1 датчик на плате 2
    int8_t  CAN_PBM1_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ1 датчик на плате 3
    int8_t  CAN_PBM1_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ1 датчик на плате 4
    int8_t  CAN_PBM2_Temp_Controller1;                              //+     |  const = 0x               |   АБ2 датчик контроллера 1
    int8_t  CAN_PBM2_Temp_Controller2;                              //+     |  const = 0x               |   АБ2 датчик контроллера 2
    int8_t  CAN_PBM2_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ2 датчик на плате 1
    int8_t  CAN_PBM2_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ2 датчик на плате 2
    int8_t  CAN_PBM2_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ2 датчик на плате 3
    int8_t  CAN_PBM2_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ2 датчик на плате 4
    int8_t  CAN_PBM3_Temp_Controller1;                              //+     |  const = 0x               |   АБ3 датчик контроллера 1
    int8_t  CAN_PBM3_Temp_Controller2;                              //+     |  const = 0x               |   АБ3 датчик контроллера 2
    int8_t  CAN_PBM3_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ3 датчик на плате 1
    int8_t  CAN_PBM3_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ3 датчик на плате 2
    int8_t  CAN_PBM3_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ3 датчик на плате 3
    int8_t  CAN_PBM3_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ3 датчик на плате 4
    int8_t  CAN_PBM4_Temp_Controller1;                              //+     |  const = 0x               |   АБ4 датчик контроллера 1
    int8_t  CAN_PBM4_Temp_Controller2;                              //+     |  const = 0x               |   АБ4 датчик контроллера 2
    int8_t  CAN_PBM4_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ4 датчик на плате 1
    int8_t  CAN_PBM4_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ4 датчик на плате 2
    int8_t  CAN_PBM4_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ4 датчик на плате 3
    int8_t  CAN_PBM4_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ4 датчик на плате 4
    uint8_t  CAN_TMI2_Reserved[9];                                  //+  |  const = 0x */












    /*

        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_mAh             = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_AbcoluteCapacity; // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_mAh             = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_AbcoluteCapacity; // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_mAh             = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_AbcoluteCapacity; // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_mAh             = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_AbcoluteCapacity; // PBM_data
        CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line1             = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_AbcoluteCapacity; // PBM_data
        CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line2             = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_AbcoluteCapacity; // PBM_data





        int8_t temp_mas [2] = {0}; 																					// PBM_data

        temp_mas[0] = eps_p.eps_pbm_ptr[0].Branch_1_Temperature; 													// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[0].Branch_2_Temperature;  													// PBM_data
        CAN_IVar5_telemetry.CAN_Average_temp_controller_AB1                 =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[0].TMP1075_temp_1;  														// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[0].TMP1075_temp_2;  														// PBM_data
        CAN_IVar5_telemetry.CAN_Average_line1_temp_AB1                      =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[0].TMP1075_temp_3;  														// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[0].TMP1075_temp_4;  														// PBM_data
        CAN_IVar5_telemetry.CAN_Average_line2_temp_AB1                      =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[1].Branch_1_Temperature;  													// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[1].Branch_2_Temperature;  													// PBM_data
        CAN_IVar5_telemetry.CAN_Average_temp_controller_AB2                 =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[1].TMP1075_temp_1;  														// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[1].TMP1075_temp_2;  														// PBM_data
        CAN_IVar5_telemetry.CAN_Average_line1_temp_AB2                      =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[1].TMP1075_temp_3;  														// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[1].TMP1075_temp_4;  														// PBM_data
        CAN_IVar5_telemetry.CAN_Average_line2_temp_AB2                      =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[2].Branch_1_Temperature;  													// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[2].Branch_2_Temperature;  													// PBM_data
        CAN_IVar5_telemetry.CAN_Average_temp_controller_AB3                 =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[2].TMP1075_temp_1;  														// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[2].TMP1075_temp_2;  														// PBM_data
        CAN_IVar5_telemetry.CAN_Average_line1_temp_AB3                      =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        temp_mas[0] = eps_p.eps_pbm_ptr[2].TMP1075_temp_3;  														// PBM_data
        temp_mas[1] = eps_p.eps_pbm_ptr[2].TMP1075_temp_4;  														// PBM_data
        CAN_IVar5_telemetry.CAN_Average_line2_temp_AB3                      =  (uint8_t) GetMedian(temp_mas, 2); 	// PBM_data
        /

        CAN_IVar5_telemetry.CAN_AB_status[0]                                = (uint8_t)((eps_p.eps_pbm_ptr[0].Error_DS2777_1) | (eps_p.eps_pbm_ptr[0].Error_DS2777_2 << 1) |
                                                                                (eps_p.eps_pbm_ptr[0].Error_Heat_1 << 2)    | (eps_p.eps_pbm_ptr[0].Error_Heat_2 << 3) |
                                                                                (eps_p.eps_pbm_ptr[0].Error_TMP1075_1 << 4) | (eps_p.eps_pbm_ptr[0].Error_TMP1075_2 << 5) |
                                                                                (eps_p.eps_pbm_ptr[0].Error_TMP1075_3 << 6) | (eps_p.eps_pbm_ptr[0].Error_TMP1075_4 << 7));		// PBM_data

        CAN_IVar5_telemetry.CAN_AB_status[1]                                = (uint8_t)((eps_p.eps_pbm_ptr[1].Error_DS2777_1) | (eps_p.eps_pbm_ptr[1].Error_DS2777_2 << 1) |
                                                                                (eps_p.eps_pbm_ptr[1].Error_Heat_1 << 2)    | (eps_p.eps_pbm_ptr[1].Error_Heat_2 << 3) |
                                                                                (eps_p.eps_pbm_ptr[1].Error_TMP1075_1 << 4) | (eps_p.eps_pbm_ptr[1].Error_TMP1075_2 << 5) |
                                                                                (eps_p.eps_pbm_ptr[1].Error_TMP1075_3 << 6) | (eps_p.eps_pbm_ptr[1].Error_TMP1075_4 << 7));		// PBM_data

        CAN_IVar5_telemetry.CAN_AB_status[2]                                = (uint8_t)((eps_p.eps_pbm_ptr[2].Error_DS2777_1) | (eps_p.eps_pbm_ptr[2].Error_DS2777_2 << 1) |
                                                                                (eps_p.eps_pbm_ptr[2].Error_Heat_1 << 2)    | (eps_p.eps_pbm_ptr[2].Error_Heat_2 << 3) |
                                                                                (eps_p.eps_pbm_ptr[2].Error_TMP1075_1 << 4) | (eps_p.eps_pbm_ptr[2].Error_TMP1075_2 << 5) |
                                                                                (eps_p.eps_pbm_ptr[2].Error_TMP1075_3 << 6) | (eps_p.eps_pbm_ptr[2].Error_TMP1075_4 << 7));		// PBM_data

        /
        CAN_IVar5_telemetry.CAN_Charge_discharge_AB_key_status              = (uint16_t)((eps_p.eps_pbm_ptr[0].Error_Charge_1) | (eps_p.eps_pbm_ptr[0].Error_Discharge_1 << 1) |
                                                                                (eps_p.eps_pbm_ptr[0].Error_Charge_2 << 2) | (eps_p.eps_pbm_ptr[0].Error_Discharge_2 << 3) |
                                                                                (eps_p.eps_pbm_ptr[1].Error_Charge_1 << 4) | (eps_p.eps_pbm_ptr[1].Error_Discharge_1 << 5) |
                                                                                (eps_p.eps_pbm_ptr[1].Error_Charge_2 << 6) | (eps_p.eps_pbm_ptr[1].Error_Discharge_2 << 7) |
                                                                                (eps_p.eps_pbm_ptr[2].Error_Charge_1 << 8) | (eps_p.eps_pbm_ptr[2].Error_Discharge_1 << 9) |
                                                                                (eps_p.eps_pbm_ptr[2].Error_Charge_2 << 10) | (eps_p.eps_pbm_ptr[2].Error_Discharge_2 << 11)); 	// PBM_data
        //---


    //	    // -------------------  ТМИ 7  ------------------ //
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Char_Discha_Current          = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_Current;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Char_Discha_Current          = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_Current;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Char_Discha_Current          = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_Current;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Char_Discha_Current          = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_Current;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Char_Discha_Current          = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_Current;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Char_Discha_Current          = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_Current;  // PBM_data

        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery1_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_VoltageHi; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery2_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_VoltageLo; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery1_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_VoltageHi; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery2_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_VoltageLo; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery1_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_VoltageHi; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery2_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_VoltageLo; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery1_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_VoltageHi; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery2_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_VoltageLo; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery1_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_VoltageHi; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery2_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_VoltageLo; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery1_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_VoltageHi; 		// PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery2_Voltage                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_VoltageLo; 		// PBM_data

        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_Percent              = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_1_RelativeCapacity;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_Percent              = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_2_RelativeCapacity;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_Percent              = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_1_RelativeCapacity;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_Percent              = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_2_RelativeCapacity;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Charge_Level_Percent              = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_1_RelativeCapacity;  // PBM_data
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Charge_Level_Percent              = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_2_RelativeCapacity;  // PBM_data






        //PBM 1
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[19]                  =  (eps_p.eps_pbm_ptr[0].Branch_1_DchgEnableBit) | (eps_p.eps_pbm_ptr[0].Branch_1_ChgEnableBit << 1) | (eps_p.eps_pbm_ptr[0].Branch_1_DchgControlFlag << 2) |
                                                                               (eps_p.eps_pbm_ptr[0].Branch_1_ChgControlFlag << 3) | (eps_p.eps_pbm_ptr[0].Branch_1_LrnFlag << 4) | (eps_p.eps_pbm_ptr[0].Branch_1_StbEmptyFlag << 5) |
                                                                               (eps_p.eps_pbm_ptr[0].Branch_1_ActEmptyFlag  << 6) | (eps_p.eps_pbm_ptr[0].Branch_1_ChgTerminateFlag << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[20]                  =  eps_p.eps_pbm_ptr[0].Branch_1_AgeScalar;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[21]                  =  eps_p.eps_pbm_ptr[0].Branch_1_CycleCounter;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[22]                  =  (eps_p.eps_pbm_ptr[0].Branch_2_DchgEnableBit) | (eps_p.eps_pbm_ptr[0].Branch_2_ChgEnableBit << 1) | (eps_p.eps_pbm_ptr[0].Branch_2_DchgControlFlag << 2) |
                                                                               (eps_p.eps_pbm_ptr[0].Branch_2_ChgControlFlag << 3) | (eps_p.eps_pbm_ptr[0].Branch_2_LrnFlag << 4) | (eps_p.eps_pbm_ptr[0].Branch_2_StbEmptyFlag << 5) |
                                                                               (eps_p.eps_pbm_ptr[0].Branch_2_ActEmptyFlag  << 6) | (eps_p.eps_pbm_ptr[0].Branch_2_ChgTerminateFlag << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[23]                  =  eps_p.eps_pbm_ptr[0].Branch_2_AgeScalar;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[24]                  =  eps_p.eps_pbm_ptr[0].Branch_2_CycleCounter;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[25]                  =  (eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_1) | (eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_2 << 1) | (eps_p.eps_pbm_ptr[0].PCA9534_TempSens_State_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[0].PCA9534_TempSens_State_2 << 3) | (eps_p.eps_pbm_ptr[0].PCA9534_PIO_1 << 4) | (eps_p.eps_pbm_ptr[0].PCA9534_PIO_2 << 5) |
                                                                               (eps_p.eps_pbm_ptr[0].PCA9534_P1 << 6) | (eps_p.eps_pbm_ptr[0].PCA9534_P2 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[26]                  =  (eps_p.eps_pbm_ptr[0].Error_DS2777_1) | (eps_p.eps_pbm_ptr[0].Error_DS2777_2 << 1) | (eps_p.eps_pbm_ptr[0].Error_TMP1075_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[0].Error_TMP1075_2 << 3) | (eps_p.eps_pbm_ptr[0].Error_TMP1075_3 << 4) | (eps_p.eps_pbm_ptr[0].Error_TMP1075_4 << 5) |
                                                                               (eps_p.eps_pbm_ptr[0].Error_PCA9534 << 6) | (eps_p.eps_pbm_ptr[0].Error_Heat_1 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[27]                  =  (eps_p.eps_pbm_ptr[0].Error_Heat_2) | (eps_p.eps_pbm_ptr[0].Error_Charge_1 << 1) | (eps_p.eps_pbm_ptr[0].Error_Charge_2 << 2) |
                                                                               (eps_p.eps_pbm_ptr[0].Error_Discharge_1 << 3) | (eps_p.eps_pbm_ptr[0].Error_Discharge_2 << 4) | (eps_p.eps_pbm_ptr[0].Low_Energy_Flag << 5) |
                                                                               (eps_p.eps_pbm_ptr[0].Zero_Energy_Flag  << 6);


        CAN_IVar5_telemetry.CAN_SES_module_data_array2[28]                  =  (eps_p.eps_pbm_ptr[1].Branch_1_DchgEnableBit) | (eps_p.eps_pbm_ptr[1].Branch_1_ChgEnableBit << 1) | (eps_p.eps_pbm_ptr[1].Branch_1_DchgControlFlag << 2) |
                                                                               (eps_p.eps_pbm_ptr[1].Branch_1_ChgControlFlag << 3) | (eps_p.eps_pbm_ptr[1].Branch_1_LrnFlag << 4) | (eps_p.eps_pbm_ptr[1].Branch_1_StbEmptyFlag << 5) |
                                                                               (eps_p.eps_pbm_ptr[1].Branch_1_ActEmptyFlag  << 6) | (eps_p.eps_pbm_ptr[1].Branch_1_ChgTerminateFlag << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[29]                  =  eps_p.eps_pbm_ptr[1].Branch_1_AgeScalar;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[31]                  =  eps_p.eps_pbm_ptr[1].Branch_1_CycleCounter;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[31]                  =  (eps_p.eps_pbm_ptr[1].Branch_2_DchgEnableBit) | (eps_p.eps_pbm_ptr[1].Branch_2_ChgEnableBit << 1) | (eps_p.eps_pbm_ptr[1].Branch_2_DchgControlFlag << 2) |
                                                                               (eps_p.eps_pbm_ptr[1].Branch_2_ChgControlFlag << 3) | (eps_p.eps_pbm_ptr[1].Branch_2_LrnFlag << 4) | (eps_p.eps_pbm_ptr[1].Branch_2_StbEmptyFlag << 5) |
                                                                               (eps_p.eps_pbm_ptr[1].Branch_2_ActEmptyFlag  << 6) | (eps_p.eps_pbm_ptr[1].Branch_2_ChgTerminateFlag << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[32]                  =  eps_p.eps_pbm_ptr[1].Branch_2_AgeScalar;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[33]                  =  eps_p.eps_pbm_ptr[1].Branch_2_CycleCounter;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[34]                  =  (eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_1) | (eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_2 << 1) | (eps_p.eps_pbm_ptr[1].PCA9534_TempSens_State_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[1].PCA9534_TempSens_State_2 << 3) | (eps_p.eps_pbm_ptr[1].PCA9534_PIO_1 << 4) | (eps_p.eps_pbm_ptr[1].PCA9534_PIO_2 << 5) |
                                                                               (eps_p.eps_pbm_ptr[1].PCA9534_P1 << 6) | (eps_p.eps_pbm_ptr[1].PCA9534_P2 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[35]                  =  (eps_p.eps_pbm_ptr[1].Error_DS2777_1) | (eps_p.eps_pbm_ptr[1].Error_DS2777_2 << 1) | (eps_p.eps_pbm_ptr[1].Error_TMP1075_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[1].Error_TMP1075_2 << 3) | (eps_p.eps_pbm_ptr[1].Error_TMP1075_3 << 4) | (eps_p.eps_pbm_ptr[1].Error_TMP1075_4 << 5) |
                                                                               (eps_p.eps_pbm_ptr[1].Error_PCA9534 << 6) | (eps_p.eps_pbm_ptr[1].Error_Heat_1 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[36]                  =  (eps_p.eps_pbm_ptr[1].Error_Heat_2) | (eps_p.eps_pbm_ptr[1].Error_Charge_1 << 1) | (eps_p.eps_pbm_ptr[1].Error_Charge_2 << 2) |
                                                                               (eps_p.eps_pbm_ptr[1].Error_Discharge_1 << 3) | (eps_p.eps_pbm_ptr[1].Error_Discharge_2 << 4) | (eps_p.eps_pbm_ptr[1].Low_Energy_Flag << 5) |
                                                                               (eps_p.eps_pbm_ptr[1].Zero_Energy_Flag  << 6);


        CAN_IVar5_telemetry.CAN_SES_module_data_array2[37]                  =  (eps_p.eps_pbm_ptr[2].Branch_1_DchgEnableBit) | (eps_p.eps_pbm_ptr[2].Branch_1_ChgEnableBit << 1) | (eps_p.eps_pbm_ptr[2].Branch_1_DchgControlFlag << 2) |
                                                                               (eps_p.eps_pbm_ptr[2].Branch_1_ChgControlFlag << 3) | (eps_p.eps_pbm_ptr[2].Branch_1_LrnFlag << 4) | (eps_p.eps_pbm_ptr[2].Branch_1_StbEmptyFlag << 5) |
                                                                               (eps_p.eps_pbm_ptr[2].Branch_1_ActEmptyFlag  << 6) | (eps_p.eps_pbm_ptr[2].Branch_1_ChgTerminateFlag << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[38]                  =  eps_p.eps_pbm_ptr[2].Branch_1_AgeScalar;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[39]                  =  eps_p.eps_pbm_ptr[2].Branch_1_CycleCounter;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[40]                  =  (eps_p.eps_pbm_ptr[2].Branch_2_DchgEnableBit) | (eps_p.eps_pbm_ptr[2].Branch_2_ChgEnableBit << 1) | (eps_p.eps_pbm_ptr[2].Branch_2_DchgControlFlag << 2) |
                                                                               (eps_p.eps_pbm_ptr[2].Branch_2_ChgControlFlag << 3) | (eps_p.eps_pbm_ptr[2].Branch_2_LrnFlag << 4) | (eps_p.eps_pbm_ptr[2].Branch_2_StbEmptyFlag << 5) |
                                                                               (eps_p.eps_pbm_ptr[2].Branch_2_ActEmptyFlag  << 6) | (eps_p.eps_pbm_ptr[2].Branch_2_ChgTerminateFlag << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[41]                  =  eps_p.eps_pbm_ptr[2].Branch_2_AgeScalar;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[42]                  =  eps_p.eps_pbm_ptr[2].Branch_2_CycleCounter;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[43]                  =  (eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_1) | (eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_2 << 1) | (eps_p.eps_pbm_ptr[2].PCA9534_TempSens_State_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[2].PCA9534_TempSens_State_2 << 3) | (eps_p.eps_pbm_ptr[2].PCA9534_PIO_1 << 4) | (eps_p.eps_pbm_ptr[2].PCA9534_PIO_2 << 5) |
                                                                               (eps_p.eps_pbm_ptr[2].PCA9534_P1 << 6) | (eps_p.eps_pbm_ptr[2].PCA9534_P2 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[44]                  =  (eps_p.eps_pbm_ptr[2].Error_DS2777_1) | (eps_p.eps_pbm_ptr[2].Error_DS2777_2 << 1) | (eps_p.eps_pbm_ptr[2].Error_TMP1075_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[2].Error_TMP1075_2 << 3) | (eps_p.eps_pbm_ptr[2].Error_TMP1075_3 << 4) | (eps_p.eps_pbm_ptr[2].Error_TMP1075_4 << 5) |
                                                                               (eps_p.eps_pbm_ptr[2].Error_PCA9534 << 6) | (eps_p.eps_pbm_ptr[2].Error_Heat_1 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[45]                  =  (eps_p.eps_pbm_ptr[2].Error_Heat_2) | (eps_p.eps_pbm_ptr[2].Error_Charge_1 << 1) | (eps_p.eps_pbm_ptr[2].Error_Charge_2 << 2) |
                                                                               (eps_p.eps_pbm_ptr[2].Error_Discharge_1 << 3) | (eps_p.eps_pbm_ptr[2].Error_Discharge_2 << 4) | (eps_p.eps_pbm_ptr[2].Low_Energy_Flag << 5) |
                                                                               (eps_p.eps_pbm_ptr[2].Zero_Energy_Flag << 6);
        //PAM
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[46]                  =  (eps_p.eps_pam_ptr->State_DC_DC) | (eps_p.eps_pam_ptr->State_LDO << 1) | (eps_p.eps_pam_ptr->PG_DC_DC << 2) |
                                                                               (eps_p.eps_pam_ptr->PG_LDO << 3) | (eps_p.eps_pam_ptr->Error_State_DC_DC << 4) | (eps_p.eps_pam_ptr->Error_State_LDO << 5) |
                                                                               (eps_p.eps_pam_ptr->Error_I2C_GPIO_Ext << 6) | (eps_p.eps_pam_ptr->Error_I2C_MUX_1 << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[47]                  =  (eps_p.eps_pam_ptr->Error_I2C_MUX_2) | (eps_p.eps_pam_ptr->Error_temp_sensor_1 << 1) | (eps_p.eps_pam_ptr->Error_temp_sensor_2 << 2) |
                                                                               (eps_p.eps_pam_ptr->Error_temp_sensor_3 << 3) | (eps_p.eps_pam_ptr->Error_temp_sensor_4 << 4);

        CAN_IVar5_telemetry.CAN_SES_module_data_array2[48]                  =  (eps_p.eps_pam_ptr->PWR_IN_Channel[0].State_ID_In) | (eps_p.eps_pam_ptr->PWR_IN_Channel[0].Error_PWR_Mon << 1) | (eps_p.eps_pam_ptr->PWR_IN_Channel[1].State_ID_In << 2) |
                                                                               (eps_p.eps_pam_ptr->PWR_IN_Channel[1].Error_PWR_Mon << 3) | (eps_p.eps_pam_ptr->PWR_IN_Channel[2].State_ID_In << 4) | (eps_p.eps_pam_ptr->PWR_IN_Channel[2].Error_PWR_Mon << 5) |
                                                                               (eps_p.eps_pam_ptr->PWR_IN_Channel[3].State_ID_In << 6) | (eps_p.eps_pam_ptr->PWR_IN_Channel[3].Error_PWR_Mon << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[49]                  =  (eps_p.eps_pam_ptr->PWR_IN_Channel[4].State_ID_In) | (eps_p.eps_pam_ptr->PWR_IN_Channel[4].Error_PWR_Mon << 1) | (eps_p.eps_pam_ptr->PWR_IN_Channel[5].State_ID_In << 2) |
                                                                               (eps_p.eps_pam_ptr->PWR_IN_Channel[5].Error_PWR_Mon << 3);

        CAN_IVar5_telemetry.CAN_SES_module_data_array2[50]                  =  (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[0].State_eF_out) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[0].PG_eF_out << 1) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[0].Error_State_eF_out << 2) |
                                                                               (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[1].State_eF_out << 3) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[1].PG_eF_out << 4) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[1].Error_State_eF_out << 5)|
                                                                               (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[2].State_eF_out << 6) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[2].PG_eF_out << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[51]                  =  (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[2].Error_State_eF_out) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[3].State_eF_out << 1) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[3].PG_eF_out << 2) |
                                                                               (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[3].Error_State_eF_out << 3) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[4].State_eF_out << 4) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[4].PG_eF_out << 5)|
                                                                               (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[4].Error_State_eF_out << 6) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[5].State_eF_out << 7);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[52]                  =  (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[5].PG_eF_out) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[5].Error_State_eF_out << 1);

        CAN_IVar5_telemetry.CAN_SES_module_data_array2[53]                  =  eps_p.eps_pam_ptr->PAM_ID_module;



        CAN_IVar5_telemetry.CAN_SES_module_data_array2[54]                  =  (eps_p.eps_pam_ptr->Solar_Panel[0].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[0].State_ID[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[0].State_ID[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[0].State_ID[3] & 0x01) << 3) | ((eps_p.eps_pam_ptr->Solar_Panel[0].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[0].State_ID[5] & 0x01) << 5);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[55]                  =  (eps_p.eps_pam_ptr->Solar_Panel[0].Error_temp_sensor[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[0].Error_temp_sensor[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[0].Error_temp_sensor[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[0].Error_temp_sensor[3] & 0x01) << 3) | (eps_p.eps_pam_ptr->Solar_Panel[0].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->Solar_Panel[0].Error_I2C_GPIO_Ext1 << 7 );

        CAN_IVar5_telemetry.CAN_SES_module_data_array2[56]                  =  (eps_p.eps_pam_ptr->Solar_Panel[1].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[1].State_ID[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[1].State_ID[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[1].State_ID[3] & 0x01) << 3) | ((eps_p.eps_pam_ptr->Solar_Panel[1].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[1].State_ID[5] & 0x01) << 5);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[57]                  =  (eps_p.eps_pam_ptr->Solar_Panel[1].Error_temp_sensor[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[1].Error_temp_sensor[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[1].Error_temp_sensor[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[1].Error_temp_sensor[3] & 0x01) << 3) | (eps_p.eps_pam_ptr->Solar_Panel[1].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->Solar_Panel[1].Error_I2C_GPIO_Ext1 << 7 );

        CAN_IVar5_telemetry.CAN_SES_module_data_array2[58]                  =  (eps_p.eps_pam_ptr->Solar_Panel[2].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[2].State_ID[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[2].State_ID[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[2].State_ID[3] & 0x01) << 3) | ((eps_p.eps_pam_ptr->Solar_Panel[2].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[2].State_ID[5] & 0x01) << 5);
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[59]                  =  (eps_p.eps_pam_ptr->Solar_Panel[2].Error_temp_sensor[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[2].Error_temp_sensor[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[2].Error_temp_sensor[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[2].Error_temp_sensor[3] & 0x01) << 3) | (eps_p.eps_pam_ptr->Solar_Panel[2].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->Solar_Panel[2].Error_I2C_GPIO_Ext1 << 7 );


    //	    // ----------------------- ТМИ 8 -------------------


        CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller1                     = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_1_Temperature; 			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller2                     = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_2_Temperature; 			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor1                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_1; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor2                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_2; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor3                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_3; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor4                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_4; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller1                     = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_1_Temperature; 			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller2                     = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_2_Temperature; 			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor1                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_1; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor2                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_2; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor3                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_3; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor4                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_4; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller1                     = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_1_Temperature; 			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller2                     = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_2_Temperature; 			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor1                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_1; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor2                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_2; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor3                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_3; 	   			// PBM_data
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor4                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_4; 	   			// PBM_data


        CAN_IVar5_telemetry.CAN_AB1_Charge_key_branch_1                     = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_1;  		// PBM_data
        CAN_IVar5_telemetry.CAN_AB1_Discharge_key_branch_1                  = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_1; 	// PBM_data
        CAN_IVar5_telemetry.CAN_AB1_Charge_key_branch_2                     = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_2;  		// PBM_data
        CAN_IVar5_telemetry.CAN_AB1_Discharge_key_branch_2                  = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_2; 	// PBM_data
        CAN_IVar5_telemetry.CAN_AB2_Charge_key_branch_1                     = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_1;  		// PBM_data
        CAN_IVar5_telemetry.CAN_AB2_Discharge_key_branch_1                  = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_1; 	// PBM_data
        CAN_IVar5_telemetry.CAN_AB2_Charge_key_branch_2                     = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_2;  		// PBM_data
        CAN_IVar5_telemetry.CAN_AB2_Discharge_key_branch_2                  = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_2; 	// PBM_data
        CAN_IVar5_telemetry.CAN_AB3_Charge_key_branch_1                     = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_1;  		// PBM_data
        CAN_IVar5_telemetry.CAN_AB3_Discharge_key_branch_1                  = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_1; 	// PBM_data
        CAN_IVar5_telemetry.CAN_AB3_Charge_key_branch_2                     = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_2;  		// PBM_data
        CAN_IVar5_telemetry.CAN_AB3_Discharge_key_branch_2                  = (uint8_t) CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_2; 	// PBM_data


        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask                                       =	CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1;
        CAN_IVar5_telemetry.CAN_Line2                                       =	CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2;
        CAN_IVar5_telemetry.CAN_Line3                                       =	CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3;
        CAN_IVar5_telemetry.CAN_Line4                                       =   CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4;
        CAN_IVar5_telemetry.CAN_Line5                                       =   CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5;
        CAN_IVar5_telemetry.CAN_Line6                                       =   CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6;
        CAN_IVar5_telemetry.CAN_Line_VBAT1                                  =   CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1;
        CAN_IVar5_telemetry.CAN_Line_VBAT2                                  =   CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2;

        --




        // -------------------  ТМИ 0  ------------------ //
        CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_pX		    =  CAN_IVar5_telemetry.CAN_Panel_median_temperature_pX;
        CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_nX		    =  CAN_IVar5_telemetry.CAN_Panel_median_temperature_nX;
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[0]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[0];
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[1]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[1];
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[2]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[2];
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[3]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[3];
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[4]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[4];
        CAN_IVar5_telemetry.CAN_Beacon_full_capacity_charge_level			=  CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_mAh;
        CAN_IVar5_telemetry.CAN_Beacon_AB_status[0]							=  CAN_IVar5_telemetry.CAN_AB_status[0];
        CAN_IVar5_telemetry.CAN_Beacon_AB_status[1]						 	=  CAN_IVar5_telemetry.CAN_AB_status[1];
        CAN_IVar5_telemetry.CAN_Beacon_AB_status[2]						 	=  CAN_IVar5_telemetry.CAN_AB_status[2];
        CAN_IVar5_telemetry.CAN_Beacon_charge_discharge_AB_key_status       =  CAN_IVar5_telemetry.CAN_Charge_discharge_AB_key_status;
        CAN_IVar5_telemetry.CAN_Beacon_subsystem_power_line_status		    =  CAN_IVar5_telemetry.CAN_TMIx_PMM_PWR_PG_BitMask;
        CAN_IVar5_telemetry.CAN_Beacon_full_charge_discharge_power		    =  CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PBM_Char_Dischar_Power;
        CAN_IVar5_telemetry.CAN_Beacon_Total_IN_Power_SP                    =  CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power;
        CAN_IVar5_telemetry.CAN_Beacon_spacecraft_total_power               =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power;
        CAN_IVar5_telemetry.CAN_Beacon_median_PMM_temp					    =  CAN_IVar5_telemetry.CAN_Median_PMM_temp;
        CAN_IVar5_telemetry.CAN_Beacon_median_PAM_temp					    =  CAN_IVar5_telemetry.CAN_TMIx_PAM_Median_Temp;
        CAN_IVar5_telemetry.CAN_Beacon_median_PDM_temp					    =  CAN_IVar5_telemetry.CAN_TMIx_PDM_Median_Temp;
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[0] =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[0];
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[1] =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1];
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[2] =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2];
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[3] =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[3];
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[4] =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[4];


        CAN_IVar5_telemetry.CAN_data_array3[0]                              =  (eps_p.eps_pam_ptr->Solar_Panel[3].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[3].State_ID[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[3].State_ID[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[3].State_ID[3] & 0x01) << 3) | ((eps_p.eps_pam_ptr->Solar_Panel[3].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[3].State_ID[5] & 0x01) << 5);
        CAN_IVar5_telemetry.CAN_data_array3[1]                              =  (eps_p.eps_pam_ptr->Solar_Panel[3].Error_temp_sensor[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[3].Error_temp_sensor[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[3].Error_temp_sensor[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[3].Error_temp_sensor[3] & 0x01) << 3) | (eps_p.eps_pam_ptr->Solar_Panel[3].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->Solar_Panel[3].Error_I2C_GPIO_Ext1 << 7 );
        CAN_IVar5_telemetry.CAN_data_array3[2]                              =  (eps_p.eps_pam_ptr->Solar_Panel[4].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[4].State_ID[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[4].State_ID[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[4].State_ID[3] & 0x01) << 3) | ((eps_p.eps_pam_ptr->Solar_Panel[4].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[4].State_ID[5] & 0x01) << 5);
        CAN_IVar5_telemetry.CAN_data_array3[3]                              =  (eps_p.eps_pam_ptr->Solar_Panel[4].Error_temp_sensor[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[4].Error_temp_sensor[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[4].Error_temp_sensor[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[4].Error_temp_sensor[3] & 0x01) << 3) | (eps_p.eps_pam_ptr->Solar_Panel[4].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->Solar_Panel[4].Error_I2C_GPIO_Ext1 << 7 );
        CAN_IVar5_telemetry.CAN_data_array3[4]                              =  (eps_p.eps_pam_ptr->Solar_Panel[5].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[5].State_ID[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[5].State_ID[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[5].State_ID[3] & 0x01) << 3) | ((eps_p.eps_pam_ptr->Solar_Panel[5].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[5].State_ID[5] & 0x01) << 5);
        CAN_IVar5_telemetry.CAN_data_array3[5]                              =  (eps_p.eps_pam_ptr->Solar_Panel[5].Error_temp_sensor[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[5].Error_temp_sensor[1] & 0x01) << 1) | ((eps_p.eps_pam_ptr->Solar_Panel[5].Error_temp_sensor[2] & 0x01) << 2) |
                                                                               ((eps_p.eps_pam_ptr->Solar_Panel[5].Error_temp_sensor[3] & 0x01) << 3) | (eps_p.eps_pam_ptr->Solar_Panel[5].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->Solar_Panel[5].Error_I2C_GPIO_Ext1 << 7 );
        CAN_IVar5_telemetry.CAN_data_array3[6]                              =  0x00;
        CAN_IVar5_telemetry.CAN_data_array3[7]                              =  0x00;
        CAN_IVar5_telemetry.CAN_data_array3[8]                              =  0x00;
        CAN_IVar5_telemetry.CAN_data_array3[9]                              =  0x00;

        CAN_IVar5_telemetry.CAN_PMM_PBM_Termostat_bit_mask                  =  (eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_1) | ( eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_2 << 1) | (eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_1 << 2) |
                                                                               (eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_2 << 3) | ( eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_1 << 4) | ( eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_2 << 5);

        CAN_IVar5_telemetry.CAN_PMM_PWR_PBM_Logic                           =  eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic;

        CAN_IVar5_telemetry.CAN_PAM_PWR_DC_DC                               =  eps_p.eps_pam_ptr->State_DC_DC;
        CAN_IVar5_telemetry.CAN_PAM_PWR_LDO                                 =  eps_p.eps_pam_ptr->State_LDO;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH1                              =  eps_p.eps_pam_ptr->PWR_Channel_TM_SP[0].State_eF_out;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH2                              =  eps_p.eps_pam_ptr->PWR_Channel_TM_SP[1].State_eF_out;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH3                              =  eps_p.eps_pam_ptr->PWR_Channel_TM_SP[2].State_eF_out;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH4                              =  eps_p.eps_pam_ptr->PWR_Channel_TM_SP[3].State_eF_out;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH5                              =  eps_p.eps_pam_ptr->PWR_Channel_TM_SP[4].State_eF_out;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH6                              =  eps_p.eps_pam_ptr->PWR_Channel_TM_SP[5].State_eF_out;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS__Mode                                   = eps_p.eps_pmm_ptr->EPS_Mode;
    */


    // -------------------  Beacon  ------------------ //
    CAN_IVar5_telemetry.CAN_Beacon_PMM_Switch_Active_CPU                    = CAN_IVar5_telemetry.CAN_TMIx_PMM_Switch_Active_CPU;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Main_Bus_Voltage                     = CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Total_PBM_Char_Dischar_Power         = CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PBM_Char_Dischar_Power;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Total_PAM_Generation_Power           = CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_CubeSat_Total_Cons_Power             = CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Total_Battery_Level_mAh              = CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_mAh;
    CAN_IVar5_telemetry.CAN_Beacon_PMM_Temp_Sensor                          = CAN_IVar5_telemetry.CAN_TMIx_PMM_Temp_Sensor;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch1_Median_Temp                    = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch2_Median_Temp                    = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch3_Median_Temp                    = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch4_Median_Temp                    = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch5_Median_Temp                    = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch6_Median_Temp                    = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_PAM_In_PwrCh_ID_State_BitsMask           = CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_ID_State_BitsMask;
   // uint16_t  CAN_Beacon_SP_State_BitsMask_BEACON;                  //+19,20     |  const = 0x                 |  Статус солнечных панелей, битовая маска (в т.ч. ФЭП) (МАЯК)
   // uint16_t CAN_Beacon_PBM_Chrg_Dichrg_Key_Status_BitMask;         //+21,22     |  const = 0x                 | Статус ключей заряда/разряда
   // uint8_t  CAN_Beacon_PBM_Heating_Element_State_BitMask;          //+23        |  const = 0x                 | Текущее состояние Термостата
   // uint8_t  CAN_Beacon_PBM_Error_Status_BitsMask_BEACON[3];        //+24,25,26  |  const = 0x                 | Статусы отказов (ошибок)АБ, битовая маска (МАЯК)
   // uint8_t  CAN_Beacon_EPS_Error_Status_BitsMask_BEACON[2];        //+27,28     |  const = 0x                 | Статус отказов элементов СЭС, битовая маска (МАЯК)
                                                                         //Total 29 byte

}

void CAN_Var5_fill_telemetry_const(void){

	uint8_t i = 0;
	uint8_t j = 0;
	/*
        // -------------------  ТМИ 0  ------------------ //
        CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_pX		    =  0x74;
        CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_nX		    =  0x75;
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[0]				=  0x7A;
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[1]				=  0x7B;
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[2]				=  0xC7;
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[3]				=  0x7D;
        CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[4]				=  0x7E;
        CAN_IVar5_telemetry.CAN_Beacon_full_capacity_charge_level			=  0x8C8D;
        CAN_IVar5_telemetry.CAN_Beacon_AB_status[0]							=  0x97;
        CAN_IVar5_telemetry.CAN_Beacon_AB_status[1]						 	=  0x98;
        CAN_IVar5_telemetry.CAN_Beacon_AB_status[2]						 	=  0x99;
        CAN_IVar5_telemetry.CAN_Beacon_charge_discharge_AB_key_status       =  0x9A9B;
        CAN_IVar5_telemetry.CAN_Beacon_subsystem_power_line_status		    =  0xB0;
        CAN_IVar5_telemetry.CAN_Beacon_full_charge_discharge_power		    =  0xB5B6;
        CAN_IVar5_telemetry.CAN_Beacon_Total_IN_Power_SP                    =  0xB7B8;
        CAN_IVar5_telemetry.CAN_Beacon_spacecraft_total_power               =  0xB9BA;
        CAN_IVar5_telemetry.CAN_Beacon_median_PMM_temp					    =  0xBB;
        CAN_IVar5_telemetry.CAN_Beacon_median_PAM_temp					    =  0xBC;
        CAN_IVar5_telemetry.CAN_Beacon_median_PDM_temp					    =  0xBD;
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[0] =  0xBE;
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[1] =  0xBF;
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[2] =  0xC0;
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[3] =  0xC1;
        CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[4] =  0xC2;
        // -------------------  ТМИ 4  ------------------ //
        CAN_IVar5_telemetry.CAN_SP_current_pX                           	=  0x5C5D;
        CAN_IVar5_telemetry.CAN_SP_current_nX                           	=  0x5E5F;
        CAN_IVar5_telemetry.CAN_SP_current_pY                           	=  0x6061;
        CAN_IVar5_telemetry.CAN_SP_current_nY                           	=  0x6263;
        CAN_IVar5_telemetry.CAN_SPF_current_1    							=  0x6465;
        CAN_IVar5_telemetry.CAN_SPF_current_2    							=  0x6667;


        CAN_IVar5_telemetry.CAN_SP_voltage_pX                           	=  0x6869;
        CAN_IVar5_telemetry.CAN_SP_voltage_nX                           	=  0x6A6B;
        CAN_IVar5_telemetry.CAN_SP_voltage_pY                           	=  0x6C6D;
        CAN_IVar5_telemetry.CAN_SP_voltage_nY                           	=  0x6E6F;
        CAN_IVar5_telemetry.CAN_SPF_voltage_1    							=  0x7071;
        CAN_IVar5_telemetry.CAN_SPF_voltage_2    							=  0x7273;


        CAN_IVar5_telemetry.CAN_Panel_median_temperature_pX                 =  0x74;
        CAN_IVar5_telemetry.CAN_Panel_median_temperature_nX                 =  0x75;
        CAN_IVar5_telemetry.CAN_Panel_median_temperature_pY                 =  0x76;
        CAN_IVar5_telemetry.CAN_Panel_median_temperature_nY                 =  0x77;
        CAN_IVar5_telemetry.CAN_Hinged_panel_median_temperature_pY          =  0x78;
        CAN_IVar5_telemetry.CAN_Hinged_panel_median_temperature_nY          =  0x79;
        CAN_IVar5_telemetry.CAN_Solar_panel_status[0]                       =  0x7A;
        CAN_IVar5_telemetry.CAN_Solar_panel_status[1]                       =  0x7B;
        CAN_IVar5_telemetry.CAN_Solar_panel_status[2]                       =  0x7C;
        CAN_IVar5_telemetry.CAN_Solar_panel_status[3]                       =  0x7D;
        CAN_IVar5_telemetry.CAN_Solar_panel_status[4]                       =  0x7E;


        CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_percent       			=  0x7F;


        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_mAh             =  0x8081;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_mAh             =  0x8283;
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_mAh             =  0x8485;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_mAh             =  0x8687;
        CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line1             =  0x8889;
        CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line2             =  0x8A8B;
        CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_Battery_Level_mAh			        =  0x8C8D;


        CAN_IVar5_telemetry.CAN_Average_temp_controller_AB1                 =  0x8E;
        CAN_IVar5_telemetry.CAN_Average_line1_temp_AB1                      =  0x8F;
        CAN_IVar5_telemetry.CAN_Average_line2_temp_AB1                      =  0x90;
        CAN_IVar5_telemetry.CAN_Average_temp_controller_AB2                 =  0x91;
        CAN_IVar5_telemetry.CAN_Average_line1_temp_AB2                      =  0x92;
        CAN_IVar5_telemetry.CAN_Average_line2_temp_AB2                      =  0x93;
        CAN_IVar5_telemetry.CAN_Average_temp_controller_AB3                 =  0x94;
        CAN_IVar5_telemetry.CAN_Average_line1_temp_AB3                      =  0x95;
        CAN_IVar5_telemetry.CAN_Average_line2_temp_AB3                      =  0x96;
        CAN_IVar5_telemetry.CAN_AB_status[0]                                =  0x97;
        CAN_IVar5_telemetry.CAN_AB_status[1]                                =  0x98;
        CAN_IVar5_telemetry.CAN_AB_status[2]                                =  0x99;

        CAN_IVar5_telemetry.CAN_Charge_discharge_AB_key_status              =  0x9A9B;

        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current                            =  0x9C9D;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current                            =  0x9E9F;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current                            =  0xA0A1;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current                            =  0xA2A3;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Current                            =  0xA4A5;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Current                            =  0xA6A7;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage                               =  0xA8A9;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage                               =  0xAAAB;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current                               =  0xACAD;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current                               =  0xAEAF;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_PWR_PG_BitMask                 =  0xB0;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_Own_Current_Consumption                     =  0xB1B2;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_Voltage                    =  0xB3B4;
        CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PBM_Char_Dischar_Power                 =  0xB5B6;
        CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power                           =  0xB7B8;
        CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power                      =  0xB9BA;

        CAN_IVar5_telemetry.CAN_Median_PMM_temp                             =  0xBB;
        CAN_IVar5_telemetry.CAN_TMIx_PAM_Median_Temp                             =  0xBC;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Median_Temp                             =  0xBD;
        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[0]        =  0xBE;
        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1]        =  0xBF;
        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]        =  0xC0;
        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[3]        =  0xC1;
        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[4]        =  0xC2;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_Reboot_Counter_Main_CPU        =  0xC3C4C5C6;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_Reboot_Counter_Backup_CPU=  0xC7C8C9CA;

        CAN_IVar5_telemetry.CAN_PMM_PWR_State_CANmain                           =  0xCB;
        CAN_IVar5_telemetry.CAN_PMM_PWR_State_CANbackup                         =  0xCC;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_OFF_PWR_Passive_CPU                         =  0xCD;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS_Const_Mode                              =  0xCE;


        CAN_IVar5_telemetry.CAN_TMIx_PMM_Switch_Active_CPU                      =  0xCF;
        // -------------------  ТМИ 7  ------------------ //
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Char_Discha_Current          =  0xB8B9;
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Char_Discha_Current          =  0xBABB;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Char_Discha_Current          =  0xBCBD;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Char_Discha_Current          =  0xBEBF;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Char_Discha_Current          =  0xC0C1;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Char_Discha_Current          =  0xC2C3;

        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery1_Voltage                 =	0xC4C5;
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery2_Voltage                 =	0xC6C7;
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery1_Voltage                 =	0xC8C9;
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery2_Voltage                 =	0xCACB;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery1_Voltage                 =	0xCCCD;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery2_Voltage                 =	0xCECF;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery1_Voltage                 =	0xD0D1;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery2_Voltage                 =	0xD2D3;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery1_Voltage                 =	0xD4D5;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery2_Voltage                 =	0xD6D7;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery1_Voltage                 =	0xD8D9;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery2_Voltage                 =	0xDADB;

        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_Percent              =	0xDC;
        CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_Percent              =	0xDD;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_Percent              =	0xDE;
        CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_Percent              =	0xDF;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Charge_Level_Percent              =	0xE0;
        CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Charge_Level_Percent              =	0xE1;

        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage                            =	0xE2E3;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage                            =	0xE4E5;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage                            =	0xE6E7;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage                            =	0xE8E9;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Voltage                            =	0xEAEB;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Voltage                            =	0xECED;

        CAN_IVar5_telemetry.CAN_TMIx_EPS_Deploy_Status                      =	0x0000;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[0]                   =	0xF0;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[1]                   =  0xF1;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[2]                   =  0xF2;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[3]                   =  0xF3;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[4]                   =  0xF4;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[5]                   =  0xF5;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[6]                   =  0xF6;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[7]                   =  0xF7;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[8]                   =  0xF8;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[9]                   =  0xF9;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[10]                  =  0xFA;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[11]                  =  0xFB;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[12]                  =  0xFC;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[13]                  =  0xFD;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[14]                  =  0xFE;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[15]                  =  0xFF;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[16]                  =  0x00;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[17]                  =  0x01;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[18]                  =  0x02;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[19]                  =  0x03;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[20]                  =  0x04;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[21]                  =  0x05;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[22]                  =  0x06;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[23]                  =  0x07;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[24]                  =  0x08;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[25]                  =  0x09;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[26]                  =  0x0A;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[27]                  =  0x0B;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[28]                  =  0x0C;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[29]                  =  0x0D;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[30]                  =  0x0E;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[31]                  =  0x0F;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[32]                  =  0x10;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[33]                  =  0x11;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[34]                  =  0x12;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[35]                  =  0x13;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[36]                  =  0x14;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[37]                  =  0x15;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[38]                  =  0x16;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[39]                  =  0x17;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[40]                  =  0x18;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[41]                  =  0x19;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[42]                  =  0x1A;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[43]                  =  0x1B;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[44]                  =  0x1C;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[45]                  =  0x1D;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[46]                  =  0x1E;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[47]                  =  0x1F;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[48]                  =  0x20;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[49]                  =  0x21;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[50]                  =  0x22;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[51]                  =  0x23;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[52]                  =  0x24;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[53]                  =  0x25;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[54]                  =  0x26;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[55]                  =  0x27;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[56]                  =  0x28;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[57]                  =  0x29;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[58]                  =  0x2A;
        CAN_IVar5_telemetry.CAN_SES_module_data_array2[59]                  =  0x2B;


        // ----------------------- ТМИ 8 -------------------
        CAN_IVar5_telemetry.CAN_SPanel_temp_pX_sensor_1                      =	0x2C;
        CAN_IVar5_telemetry.CAN_SPanel_temp_pX_sensor_2                      =	0x2D;
        CAN_IVar5_telemetry.CAN_SPanel_temp_pX_sensor_3                      =	0x2E;
        CAN_IVar5_telemetry.CAN_SPanel_temp_pX_sensor_4                      =	0x2F;
        CAN_IVar5_telemetry.CAN_SPanel_temp_nX_sensor_1                      =	0x30;
        CAN_IVar5_telemetry.CAN_SPanel_temp_nX_sensor_2                      =	0x31;
        CAN_IVar5_telemetry.CAN_SPanel_temp_nX_sensor_3                      =	0x32;
        CAN_IVar5_telemetry.CAN_SPanel_temp_nX_sensor_4                      =	0x33;
        CAN_IVar5_telemetry.CAN_SPanel_temp_pY_sensor_1                      =	0x34;
        CAN_IVar5_telemetry.CAN_SPanel_temp_pY_sensor_2                      =	0x35;
        CAN_IVar5_telemetry.CAN_SPanel_temp_nY_sensor_1                      =	0x36;
        CAN_IVar5_telemetry.CAN_SPanel_temp_nY_sensor_2                      =	0x37;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_1               =  0x38;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_2               =  0x39;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_3               =  0x3A;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_4               =  0x3B;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_1               =  0x3C;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_2               =  0x3D;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_3               =  0x3E;
        CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_4               =  0x3F;

        CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller1                     =	0x40;
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller2                     =	0x41;
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor1                        =	0x42;
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor2                        =	0x43;
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor3                        =	0x44;
        CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor4                        =	0x45;
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller1                     =	0x46;
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller2                     =	0x47;
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor1                        =	0x48;
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor2                        =	0x49;
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor3                        =	0x4A;
        CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor4                        =	0x4B;
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller1                     =	0x4C;
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller2                     =	0x4D;
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor1                        =	0x4E;
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor2                        =	0x4F;
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor3                        =	0x50;
        CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor4                        =	0x51;


        CAN_IVar5_telemetry.CAN_AB1_Charge_key_branch_1                     =  0x52;
        CAN_IVar5_telemetry.CAN_AB1_Discharge_key_branch_1                  =  0x53;
        CAN_IVar5_telemetry.CAN_AB1_Charge_key_branch_2                     =  0x54;
        CAN_IVar5_telemetry.CAN_AB1_Discharge_key_branch_2                  =  0x55;
        CAN_IVar5_telemetry.CAN_AB2_Charge_key_branch_1                     =  0x56;
        CAN_IVar5_telemetry.CAN_AB2_Discharge_key_branch_1                  =  0x57;
        CAN_IVar5_telemetry.CAN_AB2_Charge_key_branch_2                     =  0x58;
        CAN_IVar5_telemetry.CAN_AB2_Discharge_key_branch_2                  =  0x59;
        CAN_IVar5_telemetry.CAN_AB3_Charge_key_branch_1                     =  0x5A;
        CAN_IVar5_telemetry.CAN_AB3_Discharge_key_branch_1                  =  0x5B;
        CAN_IVar5_telemetry.CAN_AB3_Charge_key_branch_2                     =  0x5C;
        CAN_IVar5_telemetry.CAN_AB3_Discharge_key_branch_2                  =  0x5D;


        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask                                       =	0x5E;
        CAN_IVar5_telemetry.CAN_Line2                                       =	0x5F;
        CAN_IVar5_telemetry.CAN_Line3                                       =	0x60;
        CAN_IVar5_telemetry.CAN_Line4                                       =	0x61;
        CAN_IVar5_telemetry.CAN_Line5                                       =	0x62;
        CAN_IVar5_telemetry.CAN_Line6                                       =	0x63;
        CAN_IVar5_telemetry.CAN_Line_VBAT1                                  =	0x64;
        CAN_IVar5_telemetry.CAN_Line_VBAT2                                  =	0x65;

        CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage                    =	0x6667;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_Temp_Sensor                                 =	0x68;
        CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor1                                 =	0x69;
        CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor2                                 =	0x6A;
        CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor3                                 =	0x6B;
        CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor4                                 =	0x6C;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor1                                 =	0x6D;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor2                                 =	0x6E;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor3                                 =	0x6F;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor4                                 =	0x70;

        for( i = 0, j = 0x71; i < 10; i++, j++ ){
            CAN_IVar5_telemetry.CAN_data_array3[i] = j;
        }

        CAN_IVar5_telemetry.CAN_PMM_PBM_Termostat_bit_mask                  =   0x7B;
        CAN_IVar5_telemetry.CAN_PMM_PWR_PBM_Logic                           =   0x7C;

        CAN_IVar5_telemetry.CAN_PAM_PWR_DC_DC                               =   0x7D;
        CAN_IVar5_telemetry.CAN_PAM_PWR_LDO                                 =   0x7E;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH1                              =   0x7F;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH2                              =   0x80;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH3                              =   0x81;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH4                              =   0x82;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH5                              =   0x83;
        CAN_IVar5_telemetry.CAN_PAM_SP_PWR_CH6                              =   0x84;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS__Mode                                   =   0x85;

        CAN_IVar5_telemetry.CAN_TMIx_PMM_Version_FW                                  =   0x8687;

        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current_Aver_10s                =   0x8889;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current_Aver_10s                =   0x8A8B;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current_Aver_10s                =   0x8C8D;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current_Aver_10s                =   0x8E8F;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current_Aver_10s                   =   0x9091;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current_Aver_10s                   =   0x9293;

        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage_Aver_10s                =   0x9495;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage_Aver_10s                =   0x9697;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage_Aver_10s                =   0x9899;
        CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage_Aver_10s                =   0x9A9B;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage_Aver_10s                   =   0x9C9D;
        CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage_Aver_10s                   =   0x9E9F;
    */
}

void CAN_Var4_fill( _EPS_Param eps_p ){

    CAN_IVar4_RegCmd.CAN_time_byte0                           			= 0x00;
    /*	CAN_IVar4_RegCmd.CAN_time_byte1                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_time_byte2                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_time_byte3                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Constant_mode                          		= eps_p.eps_pmm_ptr->CAN_constatnt_mode;
        CAN_IVar4_RegCmd.CAN_Common_cmd0                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd1                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd2                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd3                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd4                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd5                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd6                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd7                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd8                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd9                           			= 0x00;
        CAN_IVar4_RegCmd.CAN_Common_cmd10                           		= 0x00;

        CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_1                        = eps_p.eps_pbm_ptr[0].Branch_1_ChgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_1                     = eps_p.eps_pbm_ptr[0].Branch_1_DchgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_2                        = eps_p.eps_pbm_ptr[0].Branch_2_ChgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_2                     = eps_p.eps_pbm_ptr[0].Branch_2_DchgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_1                        = eps_p.eps_pbm_ptr[1].Branch_1_ChgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_1                     = eps_p.eps_pbm_ptr[1].Branch_1_DchgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_2                        = eps_p.eps_pbm_ptr[1].Branch_2_ChgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_2                     = eps_p.eps_pbm_ptr[1].Branch_2_DchgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_1                        = eps_p.eps_pbm_ptr[2].Branch_1_ChgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_1                     = eps_p.eps_pbm_ptr[2].Branch_1_DchgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_2                        = eps_p.eps_pbm_ptr[2].Branch_2_ChgEnableBit;
        CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_2                     = eps_p.eps_pbm_ptr[2].Branch_2_DchgEnableBit;
    //TODO Add fill 4 PBM

        if( eps_p.eps_pdm_ptr->PWR_Channel[0].State_eF_in == ENABLE && eps_p.eps_pdm_ptr->PWR_Channel[0].State_eF_out == ENABLE ){
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1                     				= 0x01;
        }else{
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1                     				= 0x00;
        }

        if( eps_p.eps_pdm_ptr->PWR_Channel[1].State_eF_in == ENABLE && eps_p.eps_pdm_ptr->PWR_Channel[1].State_eF_out == ENABLE ){
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2                     				= 0x01;
        }else{
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2                     				= 0x00;
        }

        if( eps_p.eps_pdm_ptr->PWR_Channel[2].State_eF_in == ENABLE && eps_p.eps_pdm_ptr->PWR_Channel[2].State_eF_out == ENABLE ){
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3                     				= 0x01;
        }else{
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3                     				= 0x00;
        }

        if( eps_p.eps_pdm_ptr->PWR_Channel[3].State_eF_in == ENABLE && eps_p.eps_pdm_ptr->PWR_Channel[3].State_eF_out == ENABLE ){
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4                     				= 0x01;
        }else{
            CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4                     				= 0x00;
        }

        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5                     					= 0x00;
        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6                     					= 0x00;

        if( eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF1 == ENABLE || eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2 == ENABLE ){
            CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1                     				= 0x01;
        }else{
            CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1                     				= 0x00;
        }

        if( eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF1 == ENABLE || eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2 == ENABLE ){
            CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2                     				= 0x01;
        }else{
            CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2                     				= 0x00;
        }

        CAN_IVar4_RegCmd.CAN_EPS_Mode                     					= eps_p.eps_pmm_ptr->EPS_Mode;
        CAN_IVar4_RegCmd.CAN_Switch_active_CPU               				= eps_p.eps_pmm_ptr->Active_CPU;
        CAN_IVar4_RegCmd.CAN_Reset_to_default              					= 0x00;
        CAN_IVar4_RegCmd.CAN_Perform_Deploy                					= 0x00;
        CAN_IVar4_RegCmd.CAN_PBM1_Heat_Branch1              					= eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_1;
        CAN_IVar4_RegCmd.CAN_PBM1_Heat_Branch2              					= eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_2;
        CAN_IVar4_RegCmd.CAN_PBM2_Heat_Branch1              					= eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_1;
        CAN_IVar4_RegCmd.CAN_PBM2_Heat_Branch2              					= eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_2;
        CAN_IVar4_RegCmd.CAN_PBM3_Heat_Branch1              					= eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_1;
        CAN_IVar4_RegCmd.CAN_PBM3_Heat_Branch2              					= eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_2;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_DC_DC              					= eps_p.eps_pam_ptr->State_DC_DC;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_LDO              		    			= eps_p.eps_pam_ptr->State_LDO;
        CAN_IVar4_RegCmd.CAN_PMM_PWR_PBM_Logic              				= eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic;
        CAN_IVar4_RegCmd.CAN_PMM_PWR_CAN_main                				= eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain;
        CAN_IVar4_RegCmd.CAN_PMM_PWR_CAN_backup                				= eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH1                            = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[0].State_eF_out;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH2                            = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[1].State_eF_out;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH3                            = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[2].State_eF_out;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH4                            = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[3].State_eF_out;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH5                            = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[4].State_eF_out;
        CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH6                            = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[5].State_eF_out;

        CAN_IVar4_RegCmd.CAN_PMM_PWR_OFF_Passive_CPU                      = eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU;
        CAN_IVar4_RegCmd.CAN_PMM_Reboot_Passive_CPU                       = 0x00;
    */
}
