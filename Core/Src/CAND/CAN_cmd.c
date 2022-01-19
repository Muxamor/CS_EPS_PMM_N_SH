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

                case CAN_PBM1_Emerg_Charge_Key_Branch1_offset: //Enable/Disable Emergency charge PBM1 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Emerg_Charge_Key_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM1 branch 1\n", CAN_PBM1_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM1 branch 1\n", CAN_PBM1_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;


                case CAN_PBM1_Emerg_Charge_Key_Branch2_offset: //Enable/Disable Emergency charge PBM1 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Emerg_Charge_Key_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM1 branch 2\n", CAN_PBM1_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM1 branch 2\n", CAN_PBM1_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;

                case CAN_PBM2_Emerg_Charge_Key_Branch1_offset: //Enable/Disable Emergency charge PBM2 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Emerg_Charge_Key_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM2 branch 1\n", CAN_PBM2_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM2 branch 1\n", CAN_PBM2_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;


                case CAN_PBM2_Emerg_Charge_Key_Branch2_offset: //Enable/Disable Emergency charge PBM2 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Emerg_Charge_Key_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM2 branch 2\n", CAN_PBM2_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM2 branch 2\n", CAN_PBM2_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;

                case CAN_PBM3_Emerg_Charge_Key_Branch1_offset: //Enable/Disable Emergency charge PBM3 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Emerg_Charge_Key_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM3 branch 1\n", CAN_PBM3_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM3 branch 1\n", CAN_PBM3_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;


                case CAN_PBM3_Emerg_Charge_Key_Branch2_offset: //Enable/Disable Emergency charge PBM3 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Emerg_Charge_Key_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM3 branch 2\n", CAN_PBM3_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM3 branch 2\n", CAN_PBM3_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;

                case CAN_PBM4_Emerg_Charge_Key_Branch1_offset: //Enable/Disable Emergency charge PBM4 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Emerg_Charge_Key_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM4 branch 1\n", CAN_PBM4_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM4 branch 1\n", CAN_PBM4_Emerg_Charge_Key_Branch1_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;

                case CAN_PBM4_Emerg_Charge_Key_Branch2_offset: //Enable/Disable Emergency charge PBM4 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Emerg_Charge_Key_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE emergency charge PBM4 branch 2\n", CAN_PBM4_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_ON_EMERG_CHRG);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE emergency charge PBM4 branch 2\n", CAN_PBM4_Emerg_Charge_Key_Branch2_offset);
                        #endif
                        PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_OFF_EMERG_CHRG);
                    }
                    break;

                case CAN_PBM1_Auto_Corr_Cap_Level_Branch1_offset: //Enable/Disable auto correction capacity level PBM1 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Auto_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM1 branch 1\n", CAN_PBM1_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM1 branch 1\n", CAN_PBM1_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM1_Auto_Corr_Cap_Level_Branch2_offset: //Enable/Disable auto correction capacity level PBM1 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Auto_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM1 branch 2\n", CAN_PBM1_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM1 branch 2\n", CAN_PBM1_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM2_Auto_Corr_Cap_Level_Branch1_offset: //Enable/Disable auto correction capacity level PBM2 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Auto_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM2 branch 1\n", CAN_PBM2_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM2 branch 1\n", CAN_PBM2_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM2_Auto_Corr_Cap_Level_Branch2_offset: //Enable/Disable auto correction capacity level PBM2 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Auto_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM2 branch 2\n", CAN_PBM2_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM2 branch 2\n", CAN_PBM2_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM3_Auto_Corr_Cap_Level_Branch1_offset: //Enable/Disable auto correction capacity level PBM3 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Auto_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM3 branch 1\n", CAN_PBM3_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM3 branch 1\n", CAN_PBM3_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM3_Auto_Corr_Cap_Level_Branch2_offset: //Enable/Disable auto correction capacity level PBM3 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Auto_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM3 branch 2\n", CAN_PBM3_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM3 branch 2\n", CAN_PBM3_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM4_Auto_Corr_Cap_Level_Branch1_offset: //Enable/Disable auto correction capacity level PBM4 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Auto_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM4 branch 1\n", CAN_PBM4_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM4 branch 1\n", CAN_PBM4_Auto_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM4_Auto_Corr_Cap_Level_Branch2_offset: //Enable/Disable auto correction capacity level PBM4 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Auto_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto correction capacity level PBM4 branch 2\n", CAN_PBM4_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_ON_AUTO_CAP_CORRECT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto correction capacity level PBM4 branch 2\n", CAN_PBM4_Auto_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_SetStateAutoCorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_OFF_AUTO_CAP_CORRECT);
                    }
                    break;

                case CAN_PBM1_Corr_Cap_Level_Branch1_offset: //Perform once time correction capacity level PBM1 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM1 branch 1\n", CAN_PBM1_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM1_Corr_Cap_Level_Branch2_offset: //Perform once time correction capacity level PBM1 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM1 branch 2\n", CAN_PBM1_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM2_Corr_Cap_Level_Branch1_offset: //Perform once time correction capacity level PBM2 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM2 branch 1\n", CAN_PBM2_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM2_Corr_Cap_Level_Branch2_offset: //Perform once time correction capacity level PBM2 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM2 branch 2\n", CAN_PBM2_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM3_Corr_Cap_Level_Branch1_offset: //Perform once time correction capacity level PBM3 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM3 branch 1\n", CAN_PBM3_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM3_Corr_Cap_Level_Branch2_offset: //Perform once time correction capacity level PBM3 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM3 branch 2\n", CAN_PBM3_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM4_Corr_Cap_Level_Branch1_offset: //Perform once time correction capacity level PBM4 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Corr_Cap_Level_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM4 branch 1\n", CAN_PBM4_Corr_Cap_Level_Branch1_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM4_Corr_Cap_Level_Branch2_offset: //Perform once time correction capacity level PBM4 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Corr_Cap_Level_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Perform once time correction capacity level PBM4 branch 2\n", CAN_PBM4_Corr_Cap_Level_Branch2_offset);
                        #endif
                        PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2, PBM_T1_MAX_BATT_CAP);
                    }
                    break;

                case CAN_PBM1_Reset_Branch1_offset: //Reset MAX17320 PBM1 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Reset_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM1 branch 1\n", CAN_PBM1_Reset_Branch1_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_1);
                    }
                    break;

                case CAN_PBM1_Reset_Branch2_offset: //Reset MAX17320 PBM1 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM1_Reset_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM1 branch 2\n", CAN_PBM1_Reset_Branch2_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_1, PBM_T1_BRANCH_2);
                    }
                    break;

                case CAN_PBM2_Reset_Branch1_offset: //Reset MAX17320 PBM2 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Reset_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM2 branch 1\n", CAN_PBM2_Reset_Branch1_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_1);
                    }
                    break;

                case CAN_PBM2_Reset_Branch2_offset: //Reset MAX17320 PBM2 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM2_Reset_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM2 branch 2\n", CAN_PBM2_Reset_Branch2_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_2, PBM_T1_BRANCH_2);
                    }
                    break;

                case CAN_PBM3_Reset_Branch1_offset: //Reset MAX17320 PBM3 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Reset_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM3 branch 1\n", CAN_PBM3_Reset_Branch1_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_1);
                    }
                    break;

                case CAN_PBM3_Reset_Branch2_offset: //Reset MAX17320 PBM3 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM3_Reset_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM3 branch 2\n", CAN_PBM3_Reset_Branch2_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_3, PBM_T1_BRANCH_2);
                    }
                    break;

                case CAN_PBM4_Reset_Branch1_offset: //Reset MAX17320 PBM4 branch 1  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Reset_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM4 branch 1\n", CAN_PBM4_Reset_Branch1_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_1);
                    }
                    break;

                case CAN_PBM4_Reset_Branch2_offset: //Reset MAX17320 PBM4 branch 2  // PBM_data
                    if ( CAN_IVar4_RegCmd.CAN_PBM4_Reset_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Reset PBM4 branch 2\n", CAN_PBM4_Reset_Branch2_offset);
                        #endif
                        PBM_T1_ResetBranch(PBM_T1_I2C_PORT, eps_p.eps_pbm_ptr, PBM_T1_4, PBM_T1_BRANCH_2);
                    }
                    break;

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
	uint8_t PBM_Number = 0, Branch_Number = 0, Heat_number = 0;

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
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage_Aver_10s             =  (uint16_t)( Filtr2Step(CHF_U7, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage ) );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage_Aver_10s             =  (uint16_t)( Filtr2Step(CHF_U8, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage ) );

	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current                      =  (int16_t)( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Current_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Current_val );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current                      =  (int16_t)( eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Current_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Current_val );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current_Aver_10s             =  (int16_t)( Filtr2Step(CHF_I7, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current ) );
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current_Aver_10s             =  (int16_t)( Filtr2Step(CHF_I8, (int16_t)CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current ) );

	CAN_IVar5_telemetry.CAN_TMIx_PMM_Own_Current_Consumption            = eps_p.eps_pmm_ptr->PWR_Supply_Main_eF_out_Current_val;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Voltage                            = 3300;

	//***
	if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < 4000 || eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF1 == ERROR ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val;
	}else{
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val;
	}

	//***
	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power            =  0x0000;
	for( num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power        =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power + eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Power_val;
	}

	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power            =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Power_val +
	                                                                        eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val;
	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {
	    for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){
	        CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power    =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power + eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_number].HeatPower;
	    }
	}
	//---

	//***
	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power            =  0x0000;
	for( num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){
	    CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power       =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power + eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Power_val;
	}

	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power           =  CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Power_val +
	                                                                            eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val;
	//---


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


    // -------------------  ТМИ 1  ------------------ //
    //PAM Module
    CAN_IVar5_telemetry.CAN_TMI1_version                                 = 0x0002;

    //***
    CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power 		 =  0x0000;
    for( num_pwr_ch = 0; num_pwr_ch < PAM_PWR_IN_Ch_quantity; num_pwr_ch++ ){
        CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power      =  CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power + eps_p.eps_pam_ptr->PWR_IN_Channel[num_pwr_ch].Power_val;
    }
    //---


    //***
    CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask                = (eps_p.eps_pam_ptr->State_DC_DC) | (eps_p.eps_pam_ptr->State_LDO << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_PWR_TM_SP_Ch_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask  = CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask  | ( eps_p.eps_pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out << move_bit_pos );
    }
    //---

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

    //***
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }
    //---

    //***
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }
    //---

    //***
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }
    //---

    //***
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }
    //---

    //***
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }
    //---

    //***
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask                = 0x0000;
    CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask                = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_I2C_GPIO_Ext2 << 1);
    for( num_pwr_ch = 0, move_bit_pos = 2; num_pwr_ch < PAM_SP_ID_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].State_ID[num_pwr_ch] << move_bit_pos );
    }
    for( num_pwr_ch = 0; num_pwr_ch < PAM_SP_temp_sens_max_quantity; num_pwr_ch++, move_bit_pos ++ ){
        CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask            = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask | ( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_temp_sensor[num_pwr_ch] << move_bit_pos );
    }
    //---

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
    // PBM part1 //
    CAN_IVar5_telemetry.CAN_TMI2_version                                = 0x0002;

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask     = 0x0000;
    for (PBM_Number = 0, move_bit_pos = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++){
        for( Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
           CAN_IVar5_telemetry.CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask   =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask | (eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].ChgEnableBit << move_bit_pos);
           move_bit_pos++;
           CAN_IVar5_telemetry.CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask   =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask | (eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].DchgControlFlag << move_bit_pos);
           move_bit_pos++;
       }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Thermostat_ComReg_BitMask          = 0x0000;
    for (PBM_Number = 0,  move_bit_pos = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {
        for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){
            CAN_IVar5_telemetry.CAN_TMIx_PBM_Thermostat_ComReg_BitMask  =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Thermostat_ComReg_BitMask | (eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_number].PCA9534_ON_Heat_CMD << move_bit_pos);
            move_bit_pos++;
        }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Heating_Element_State_BitMask          = 0x0000;
    for (PBM_Number = 0,  move_bit_pos = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {
        for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){
            CAN_IVar5_telemetry.CAN_TMIx_PBM_Heating_Element_State_BitMask  =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Heating_Element_State_BitMask | (eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_number].PCA9534_Heat_State << move_bit_pos);
            move_bit_pos++;
        }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Wreck_Charge_Key_State_BitMask    = 0x0000;
    for (PBM_Number = 0, move_bit_pos = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++){
        for( Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
            CAN_IVar5_telemetry.CAN_TMIx_PBM_Wreck_Charge_Key_State_BitMask   =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Wreck_Charge_Key_State_BitMask | (eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Key << move_bit_pos);
            move_bit_pos++;
        }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Auto_Correct_Cap_State_BitMask    = 0x0000;
    for (PBM_Number = 0, move_bit_pos = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++){
        for( Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
            CAN_IVar5_telemetry.CAN_TMIx_PBM_Auto_Correct_Cap_State_BitMask   =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Auto_Correct_Cap_State_BitMask | (eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Key << move_bit_pos);
            move_bit_pos++;
        }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_PBM_Char_Dischar_Power 		 =  0x0000;
    for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++ ){
        for(Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
            CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_PBM_Char_Dischar_Power    =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_PBM_Char_Dischar_Power + (int16_t) (eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Power);
        }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Heating_Elements_Power 		 =  0x0000;
    for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {
        for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){
            CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Heating_Elements_Power    =  CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Heating_Elements_Power + eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_number].HeatPower;
        }
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_mAh	 		 = 0x0000;
    for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_mAh		 = CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_mAh + (uint16_t)(eps_p.eps_pbm_ptr[PBM_Number].TotalAbcoluteCapacity_mAh);
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_percent 		 = 0x0000;
    for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++ ){
        CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_percent     = CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_percent + (uint8_t)(eps_p.eps_pbm_ptr[PBM_Number].TotalRelativeCapacity_Perc / PBM_T1_QUANTITY);
    }

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Error_BitMask 		 			 = eps_p.eps_pbm_ptr[PBM_T1_1].Error_PCA9534 | (eps_p.eps_pbm_ptr[PBM_T1_1].Error_I2C_MUX << 1) | (eps_p.eps_pbm_ptr[PBM_T1_1].Low_Energy_Flag << 2)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_1].Zero_Energy_Flag << 3) | (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].Error_INA238 << 4)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].Error_Heat << 5) | (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 6)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 7) | (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].Error_INA238 << 8)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].Error_Heat << 9) | (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 10)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 11);

    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Error_BitMask 		 			 = eps_p.eps_pbm_ptr[PBM_T1_2].Error_PCA9534 | (eps_p.eps_pbm_ptr[PBM_T1_2].Error_I2C_MUX << 1) | (eps_p.eps_pbm_ptr[PBM_T1_2].Low_Energy_Flag << 2)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_2].Zero_Energy_Flag << 3) | (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].Error_INA238 << 4)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].Error_Heat << 5) | (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 6)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 7) | (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].Error_INA238 << 8)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].Error_Heat << 9) | (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 10)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 11);

    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Error_BitMask 		 			 = eps_p.eps_pbm_ptr[PBM_T1_3].Error_PCA9534 | (eps_p.eps_pbm_ptr[PBM_T1_3].Error_I2C_MUX << 1) | (eps_p.eps_pbm_ptr[PBM_T1_3].Low_Energy_Flag << 2)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_3].Zero_Energy_Flag << 3) | (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].Error_INA238 << 4)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].Error_Heat << 5) | (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 6)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 7) | (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].Error_INA238 << 8)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].Error_Heat << 9) | (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 10)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 11);

    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Error_BitMask 		 			 = eps_p.eps_pbm_ptr[PBM_T1_4].Error_PCA9534 | (eps_p.eps_pbm_ptr[PBM_T1_4].Error_I2C_MUX << 1) | (eps_p.eps_pbm_ptr[PBM_T1_4].Low_Energy_Flag << 2)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_4].Zero_Energy_Flag << 3) | (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].Error_INA238 << 4)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].Error_Heat << 5) | (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 6)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 7) | (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].Error_INA238 << 8)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].Error_Heat << 9) | (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1] << 10)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].Error_Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2] << 11);

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Controller1_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].OVP | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Controller2_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].OVP | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Controller1_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].OVP | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Controller2_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].OVP | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Controller1_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].OVP | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Controller2_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].OVP | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Controller1_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].OVP | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Controller2_Error_BitMask 		 = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].OVP | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].UVP << 1)
    																				| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].OCCP << 2) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].ODCP << 3)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].PreqF << 4) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].LDet << 5)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].OC << 6) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].OD << 7)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].SC << 8) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].PreqChrg << 9)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].DchgControlFlag << 10) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].ChgControlFlag << 11)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_Discharge << 12) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_Charge << 13)
																					| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_Emerg_Chrg << 14) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 15);

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].RelativeCapacity_Perc;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].RelativeCapacity_Perc;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].AbcoluteCapacity_mAh;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Charge_Level_Percent 	 = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].AbcoluteCapacity_mAh;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Current;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Char_Discha_Current    = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Current;

    CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller1                    = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Temperature;
    CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller2                    = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Temperature;
    CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor1                        = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor2                        = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor3                        = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor4                        = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller1                    = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Temperature;
    CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller2                    = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Temperature;
    CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor1                        = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor2                        = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor3                        = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor4                        = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller1                    = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Temperature;
    CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller2                    = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Temperature;
    CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor1                        = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor2                        = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor3                        = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor4                        = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM4_Temp_Controller1                    = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Temperature;
    CAN_IVar5_telemetry.CAN_PBM4_Temp_Controller2                    = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Temperature;
    CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor1                        = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor2                        = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];
    CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor3                        = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_1];
    CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor4                        = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].Heat_TMP1075[PBM_T1_HEAT_TEMPSENS_2];


    // -------------------  ТМИ 3 ------------------ //
    // PBM part2 //
    CAN_IVar5_telemetry.CAN_TMI3_version                             = 0x0002;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Voltage[1];
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Battery1_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Voltage[0];
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Battery2_Voltage       = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Voltage[1];

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].MinCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Max_Current            = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].MaxCurr;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Min_Current            = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].MinCurr;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].MinVCELL;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Min_Voltage            = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].MinVCELL;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].HeatCurrent;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Heat_Current           = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].HeatCurrent;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Age                    = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Age                    = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Age                    = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Age                    = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Age                    = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Age                    = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Age                    = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].AgeScalar;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Age                    = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].AgeScalar;

    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].CycleCounter;
    CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Char_Discha_Cycle      = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].CycleCounter;


    // -------------------  Beacon  ------------------ //
    CAN_IVar5_telemetry.CAN_Beacon_PMM_Switch_Active_CPU               = CAN_IVar5_telemetry.CAN_TMIx_PMM_Switch_Active_CPU;
    CAN_IVar5_telemetry.CAN_Beacon_PMM_Temp_Sensor                     = CAN_IVar5_telemetry.CAN_TMIx_PMM_Temp_Sensor;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Main_Bus_Voltage                = CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_CubeSat_Total_Cons_Power        = CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_CubeSat_SubSystem_Cons_Power    = CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power;
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Total_PAM_Generation_Power      = CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power;
    CAN_IVar5_telemetry.CAN_Beacon_PAM_In_PwrCh_ID_State_BitsMask      = CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_ID_State_BitsMask;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch1_Median_Temp               = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch2_Median_Temp               = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch3_Median_Temp               = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch4_Median_Temp               = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch5_Median_Temp               = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_SP_TM_Ch6_Median_Temp               = CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Median_Temp;
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Total_PBM_Char_Dischar_Power    = CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_PBM_Char_Dischar_Power;
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Total_Heating_Elements_Power    = CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Heating_Elements_Power;
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Total_Battery_Level_mAh         = CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_mAh;
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Heating_Element_State_BitMask   = CAN_IVar5_telemetry.CAN_TMIx_PBM_Heating_Element_State_BitMask;
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Error_Status_BitsMask_BEACON[0] = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_Discharge | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_Charge << 1)
																				| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 2) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_Discharge << 3)
																				| (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_Charge << 4) | (eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 5)
    																			| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_Discharge << 6) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_Charge << 7);
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Error_Status_BitsMask_BEACON[1] = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Error_MAX17320	| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_Discharge << 1)
    																			| (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_Charge << 2) | (eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 3)
    	    	    															| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_Discharge << 4) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_Charge << 5)
																				| (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 6) | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_Discharge << 7);
    CAN_IVar5_telemetry.CAN_Beacon_PBM_Error_Status_BitsMask_BEACON[2] = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_Charge | (eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 1)
																				| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_Discharge << 2) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_Charge << 3)
																				| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Error_MAX17320 << 4)	| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_Discharge << 5)
																				| (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_Charge << 6) | (eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Error_MAX17320 << 7);
	CAN_IVar5_telemetry.CAN_Beacon_EPS_Error_Status_BitsMask_BEACON[0] = 0x00; //TODO заполнить так как этот параметр только для маяка
    CAN_IVar5_telemetry.CAN_Beacon_EPS_Error_Status_BitsMask_BEACON[1] = 0x00; //TODO заполнить так как этот параметр только для маяка

}

void CAN_Var5_fill_telemetry_const(void){

	CAN_IVar5_telemetry.CAN_TMI0_version                                = 0x0201;

	// PMM //
	CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS_Const_Mode                     = 0x03;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_EPS__Mode                          = 0x04;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Switch_Active_CPU                  = 0x05;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_OFF_PWR_Passive_CPU                = 0x06;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Temp_Sensor                        = 0x07;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_PWR_Ch_State_BitMask               = 0x0908;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_PWR_PG_BitMask                     = 0x0B0A;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Error_BitMask                      = 0x0E0E0D0C;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Reboot_Counter_Main_CPU            = 0x1211100F;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Reboot_Counter_Backup_CPU          = 0x16151413;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage                      = 0x1817;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage                      = 0x1A19;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Voltage_Aver_10s             = 0x1C1B;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Voltage_Aver_10s             = 0x1E1D;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current                      = 0x201F;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current                      = 0x2221;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT1_Current_Aver_10s             = 0x2423;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_VBAT2_Current_Aver_10s             = 0x2625;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Own_Current_Consumption            = 0x2827;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Voltage                            = 0x2A29;
	CAN_IVar5_telemetry.CAN_TMIx_EPS_Main_Bus_Voltage                   = 0x2C2B;
	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_Total_Cons_Power           = 0x2E2D;
	CAN_IVar5_telemetry.CAN_TMIx_EPS_CubeSat_SubSystem_Cons_Power       = 0x302F;
	CAN_IVar5_telemetry.CAN_TMIx_EPS_Deploy_Status                      = 0x3231;
	CAN_IVar5_telemetry.CAN_TMIx_PMM_Version_FW                         = 0x3433;

	// PDM //
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch_State_BitsMask              = 0x3635;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_PG_BitsMask                    = 0x3837;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_Error_BitMask                      = 0x3C3B3A39;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor1                       = 0x3D;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor2                       = 0x3E;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor3                       = 0x3F;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_Temp_Sensor4                       = 0x40;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_Median_Temp                        = 0x41;

	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage                    = 0x4342;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage                    = 0x4544;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage                    = 0x4746;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage                    = 0x4948;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Voltage                    = 0x4B4A;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Voltage                    = 0x4D4C;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Voltage_Aver_10s           = 0x4F4E;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Voltage_Aver_10s           = 0x5150;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Voltage_Aver_10s           = 0x5352;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Voltage_Aver_10s           = 0x5554;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Voltage_Aver_10s           = 0x5756;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Voltage_Aver_10s           = 0x5958;

	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current                    = 0x5B5A;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current                    = 0x5D5C;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current                    = 0x5F5E;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current                    = 0x6160;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Current                    = 0x6362;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Current                    = 0x6564;

	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch1_Current_Aver_10s           = 0x6766;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch2_Current_Aver_10s           = 0x6968;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch3_Current_Aver_10s           = 0x6B6A;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch4_Current_Aver_10s           = 0x6D6C;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch5_Current_Aver_10s           = 0x6F6E;
	CAN_IVar5_telemetry.CAN_TMIx_PDM_PWR_Ch6_Current_Aver_10s           = 0x7170;
	//Total 61 byte
	CAN_IVar5_telemetry.CAN_TMI0_Reserved[0]                            = 0x72;
	CAN_IVar5_telemetry.CAN_TMI0_Reserved[1]                            = 0x73;

	// -------------------  ТМИ 1  ------------------ //

	CAN_IVar5_telemetry.CAN_TMI1_version                                = 0x7574;
	// PAM //
	CAN_IVar5_telemetry.CAN_TMIx_EPS_Total_PAM_Generation_Power         = 0x7776;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Ch_State_BitMask               = 0x7978;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_PG_BitMask                     = 0x7B7A;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_PWR_Error_BitMask                  = 0x7F7E7D7C;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor1                       = 0x01;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor2                       = 0x02;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor3                       = 0x03;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_Temp_Sensor4                       = 0x04;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_Median_Temp                        = 0x05;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_ID_State_BitsMask         = 0x85;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh_Error_BitsMask            = 0x86;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh1_Voltage                  = 0x8887;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh2_Voltage                  = 0x8A89;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh3_Voltage                  = 0x8C8B;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh4_Voltage                  = 0x8E8D;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT1_Voltage                  = 0x908F;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT2_Voltage                  = 0x9291;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh1_Current                  = 0x0201;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh2_Current                  = 0x0403;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh3_Current                  = 0x0605;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrCh4_Current                  = 0x0807;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT1_Current                  = 0x0A09;
	CAN_IVar5_telemetry.CAN_TMIx_PAM_In_PwrLT2_Current                  = 0x0C0B;

	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Status_BitMask               = 0xA09F;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Status_BitMask               = 0xA2A1;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Status_BitMask               = 0xA4A3;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Status_BitMask               = 0xA6A5;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Status_BitMask               = 0xA8A7;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Status_BitMask               = 0xAAA9;

	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor1                 = 0x01;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor2                 = 0x02;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor3                 = 0x03;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Temp_Sensor4                 = 0x04;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor1                 = 0x05;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor2                 = 0x06;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor3                 = 0x07;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Temp_Sensor4                 = 0x08;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor1                 = 0x09;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor2                 = 0x0A;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor3                 = 0x0B;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Temp_Sensor4                 = 0x0C;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor1                 = 0x0D;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor2                 = 0x0E;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor3                 = 0x0F;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Temp_Sensor4                 = 0x10;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor1                 = 0x11;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor2                 = 0x12;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor3                 = 0x13;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Temp_Sensor4                 = 0x14;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor1                 = 0x15;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor2                 = 0x16;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor3                 = 0x17;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Temp_Sensor4                 = 0x18;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch1_Median_Temp                  = 0x19;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch2_Median_Temp                  = 0x1A;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch3_Median_Temp                  = 0x1B;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch4_Median_Temp                  = 0x1C;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch5_Median_Temp                  = 0x1D;
	CAN_IVar5_telemetry.CAN_TMIx_SP_TM_Ch6_Median_Temp                  = 0x1E;

	CAN_IVar5_telemetry.CAN_TMI1_Reserved[0]                            = 0xC9;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[1]                            = 0xCA;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[2]                            = 0xCB;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[3]                            = 0xCD;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[4]                            = 0xCE;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[5]                            = 0xCF;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[6]                            = 0xD0;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[7]                            = 0xD1;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[8]                            = 0xD2;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[9]                            = 0xD3;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[10]                           = 0xD4;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[11]                           = 0xD5;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[12]                           = 0xD6;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[13]                           = 0xD7;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[14]                           = 0xD8;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[15]                           = 0xD9;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[16]                           = 0xDA;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[17]                           = 0xDB;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[18]                           = 0xDC;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[19]                           = 0xDD;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[20]                           = 0xDE;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[21]                           = 0xDF;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[22]                           = 0xE0;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[23]                           = 0xE1;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[24]                           = 0xE2;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[25]                           = 0xE3;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[26]                           = 0xE4;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[27]                           = 0xE5;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[28]                           = 0xE6;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[29]                           = 0xE7;
	CAN_IVar5_telemetry.CAN_TMI1_Reserved[30]                           = 0xE8;


	CAN_IVar5_telemetry.CAN_TMI2_version                                = 0x0201;
	// PBM part1 //
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_PBM_Char_Dischar_Power       = 0x0403;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Heating_Elements_Power       = 0x0605;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_mAh            = 0x0807;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Total_Battery_Level_percent        = 0x09;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask     = 0x0B0A;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Thermostat_ComReg_BitMask          = 0x0C;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Heating_Element_State_BitMask      = 0x0D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Wreck_Charge_Key_State_BitMask     = 0x0E;
	CAN_IVar5_telemetry.CAN_TMIx_PBM_Auto_Correct_Cap_State_BitMask     = 0x0F;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Error_BitMask                     = 0x1110;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Error_BitMask                     = 0x1312;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Error_BitMask                     = 0x1514;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Error_BitMask                     = 0x1716;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Controller1_Error_BitMask         = 0x1918;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Controller2_Error_BitMask         = 0x1B1A;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Controller1_Error_BitMask         = 0x1D1C;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Controller2_Error_BitMask         = 0x1F1E;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Controller1_Error_BitMask         = 0x2120;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Controller2_Error_BitMask         = 0x2322;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Controller1_Error_BitMask         = 0x2524;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Controller2_Error_BitMask         = 0x2726;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_Percent      = 0x28;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_Percent      = 0x29;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_Percent      = 0x2A;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_Percent      = 0x2B;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Charge_Level_Percent      = 0x2C;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Charge_Level_Percent      = 0x2D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Charge_Level_Percent      = 0x2E;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Charge_Level_Percent      = 0x2F;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Charge_Level_mAh          = 0x3130;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Charge_Level_mAh          = 0x3332;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Charge_Level_mAh          = 0x3534;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Charge_Level_mAh          = 0x3736;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Charge_Level_mAh          = 0x3938;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Charge_Level_mAh          = 0x3B3A;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Charge_Level_mAh          = 0x3D3C;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Charge_Level_mAh          = 0x3F3E;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Char_Discha_Current       = 0x4140;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Char_Discha_Current       = 0x4342;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Char_Discha_Current       = 0x4544;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Char_Discha_Current       = 0x4746;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Char_Discha_Current       = 0x4948;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Char_Discha_Current       = 0x4B4A;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Char_Discha_Current       = 0x4D4C;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Char_Discha_Current       = 0x4F4E;

	CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller1                       = 0x50;
	CAN_IVar5_telemetry.CAN_PBM1_Temp_Controller2                       = 0x51;
	CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor1                           = 0x52;
	CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor2                           = 0x53;
	CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor3                           = 0x54;
	CAN_IVar5_telemetry.CAN_PBM1_Temp_Sensor4                           = 0x55;
	CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller1                       = 0x56;
	CAN_IVar5_telemetry.CAN_PBM2_Temp_Controller2                       = 0x57;
	CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor1                           = 0x58;
	CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor2                           = 0x59;
	CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor3                           = 0x5A;
	CAN_IVar5_telemetry.CAN_PBM2_Temp_Sensor4                           = 0x5B;
	CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller1                       = 0x5C;
	CAN_IVar5_telemetry.CAN_PBM3_Temp_Controller2                       = 0x5D;
	CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor1                           = 0x5E;
	CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor2                           = 0x5F;
	CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor3                           = 0x60;
	CAN_IVar5_telemetry.CAN_PBM3_Temp_Sensor4                           = 0x61;
	CAN_IVar5_telemetry.CAN_PBM4_Temp_Controller1                       = 0x62;
	CAN_IVar5_telemetry.CAN_PBM4_Temp_Controller2                       = 0x63;
	CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor1                           = 0x64;
	CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor2                           = 0x65;
	CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor3                           = 0x66;
	CAN_IVar5_telemetry.CAN_PBM4_Temp_Sensor4                           = 0x67;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[0]                            = 0x68;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[1]                            = 0x69;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[2]                            = 0x6A;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[3]                            = 0x6B;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[4]                            = 0x6C;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[5]                            = 0x6D;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[6]                            = 0x6E;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[7]                            = 0x6F;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[8]                            = 0x70;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[9]                            = 0x71;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[10]                           = 0x72;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[11]                           = 0x73;
	CAN_IVar5_telemetry.CAN_TMI2_Reserved[12]                           = 0x74;


	CAN_IVar5_telemetry.CAN_TMI3_version                                = 0x7675;
	// PBM part2 //
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery1_Voltage          = 0x7877;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Battery2_Voltage          = 0x7A79;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery1_Voltage          = 0x7C7B;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Battery2_Voltage          = 0x7E7D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery1_Voltage          = 0x807F;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Battery2_Voltage          = 0x8281;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery1_Voltage          = 0x8483;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Battery2_Voltage          = 0x8685;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery1_Voltage          = 0x8887;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Battery2_Voltage          = 0x8A89;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery1_Voltage          = 0x8C8B;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Battery2_Voltage          = 0x8E8D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Battery1_Voltage          = 0x908F;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Battery2_Voltage          = 0x9291;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Battery1_Voltage          = 0x9493;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Battery2_Voltage          = 0x9695;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Max_Current               = 0x0201;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Min_Current               = 0x0403;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Max_Current               = 0x0605;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Min_Current               = 0x0807;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Max_Current               = 0x0A09;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Min_Current               = 0x0C0B;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Max_Current               = 0x0E0D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Min_Current               = 0x100F;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Max_Current               = 0x1211;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Min_Current               = 0x1413;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Max_Current               = 0x1615;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Min_Current               = 0x1817;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Max_Current               = 0x1A19;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Min_Current               = 0x1C1B;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Max_Current               = 0x1E1D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Min_Current               = 0x201F;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Min_Voltage               = 0xB8B7;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Min_Voltage               = 0xBAB9;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Min_Voltage               = 0xBCBB;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Min_Voltage               = 0xBEBD;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Min_Voltage               = 0xC0BF;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Min_Voltage               = 0xC2C1;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Min_Voltage               = 0xC4C3;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Min_Voltage               = 0xC6C5;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Heat_Current              = 0x0201;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Heat_Current              = 0x0403;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Heat_Current              = 0x0605;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Heat_Current              = 0x0807;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Heat_Current              = 0x0A09;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Heat_Current              = 0x0C0B;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Heat_Current              = 0x0E0D;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Heat_Current              = 0x100F;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Age                       = 0xD8;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Age                       = 0xD9;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Age                       = 0xDA;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Age                       = 0xDB;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Age                       = 0xDC;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Age                       = 0xDD;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Age                       = 0xDE;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Age                       = 0xDF;

	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch1_Char_Discha_Cycle         = 0xE0;
	CAN_IVar5_telemetry.CAN_TMIx_PBM1_Branch2_Char_Discha_Cycle         = 0xE1;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch1_Char_Discha_Cycle         = 0xE2;
	CAN_IVar5_telemetry.CAN_TMIx_PBM2_Branch2_Char_Discha_Cycle         = 0xE3;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch1_Char_Discha_Cycle         = 0xE4;
	CAN_IVar5_telemetry.CAN_TMIx_PBM3_Branch2_Char_Discha_Cycle         = 0xE5;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch1_Char_Discha_Cycle         = 0xE6;
	CAN_IVar5_telemetry.CAN_TMIx_PBM4_Branch2_Char_Discha_Cycle         = 0xE7;
	CAN_IVar5_telemetry.CAN_TMI3_Reserved[0]                            = 0xE8;
	CAN_IVar5_telemetry.CAN_TMI3_Reserved[1]                            = 0xE9;
}

void CAN_Var4_fill( _EPS_Param eps_p ){

    CAN_IVar4_RegCmd.CAN_time_byte0                           			= 0x00;
    CAN_IVar4_RegCmd.CAN_time_byte1                           			= 0x00;
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

    // Command of PMM
    CAN_IVar4_RegCmd.CAN_EPS_Mode                                       = eps_p.eps_pmm_ptr->EPS_Mode;
    CAN_IVar4_RegCmd.CAN_Switch_active_CPU                              = eps_p.eps_pmm_ptr->Active_CPU;
    CAN_IVar4_RegCmd.CAN_Reset_to_default                               = 0x00;
    CAN_IVar4_RegCmd.CAN_Perform_Deploy                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_OFF_Passive_CPU                        = eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU;
    CAN_IVar4_RegCmd.CAN_PMM_Reboot_Passive_CPU                         = 0x00;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_CAN_main                               = eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_CAN_backup                             = eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1_key1                             = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF1;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1_key2                             = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2_key1                             = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF1;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2_key2                             = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2;
    CAN_IVar4_RegCmd.CAN_PMM_PWR_PBM_Logic                              = eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic;

    // Command of PDM
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

    if( eps_p.eps_pdm_ptr->PWR_Channel[4].State_eF_in == ENABLE && eps_p.eps_pdm_ptr->PWR_Channel[4].State_eF_out == ENABLE ){
        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5                     				= 0x01;
    }else{
        CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5                     				= 0x00;
    }

    CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6                     					= 0x00;

    // Command of PAM
    CAN_IVar4_RegCmd.CAN_PAM_PWR_DC_DC              				        = eps_p.eps_pam_ptr->State_DC_DC;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_LDO              		    			    = eps_p.eps_pam_ptr->State_LDO;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH1                                  = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[0].State_eF_out;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH2                                  = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[1].State_eF_out;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH3                                  = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[2].State_eF_out;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH4                                  = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[3].State_eF_out;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH5                                  = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[4].State_eF_out;
    CAN_IVar4_RegCmd.CAN_PAM_PWR_TM_SP_CH6                                  = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[5].State_eF_out;

    // Command of PBM
    CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_1                           = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_1                        = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM1_Charge_key_branch_2                           = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM1_Discharge_key_branch_2                        = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_1                           = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_1                        = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM2_Charge_key_branch_2                           = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM2_Discharge_key_branch_2                        = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_1                           = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_1                        = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM3_Charge_key_branch_2                           = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM3_Discharge_key_branch_2                        = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM4_Charge_key_branch_1                           = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM4_Discharge_key_branch_1                        = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].DchgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM4_Charge_key_branch_2                           = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].ChgEnableBit;
    CAN_IVar4_RegCmd.CAN_PBM4_Discharge_key_branch_2                        = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].DchgEnableBit;

    CAN_IVar4_RegCmd.CAN_PBM1_Heat_Branch1                                  = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_1].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM1_Heat_Branch2                                  = eps_p.eps_pbm_ptr[PBM_T1_1].Heat[PBM_T1_HEAT_2].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM2_Heat_Branch1                                  = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_1].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM2_Heat_Branch2                                  = eps_p.eps_pbm_ptr[PBM_T1_2].Heat[PBM_T1_HEAT_2].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM3_Heat_Branch1                                  = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_1].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM3_Heat_Branch2                                  = eps_p.eps_pbm_ptr[PBM_T1_3].Heat[PBM_T1_HEAT_2].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM4_Heat_Branch1                                  = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_1].PCA9534_ON_Heat_CMD;
    CAN_IVar4_RegCmd.CAN_PBM4_Heat_Branch2                                  = eps_p.eps_pbm_ptr[PBM_T1_4].Heat[PBM_T1_HEAT_2].PCA9534_ON_Heat_CMD;

    CAN_IVar4_RegCmd.CAN_PBM1_Emerg_Charge_Key_Branch1                      = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM1_Emerg_Charge_Key_Branch2                      = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM2_Emerg_Charge_Key_Branch1                      = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM2_Emerg_Charge_Key_Branch2                      = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM3_Emerg_Charge_Key_Branch1                      = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM3_Emerg_Charge_Key_Branch2                      = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM4_Emerg_Charge_Key_Branch1                      = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].PCA9534_Emerg_Chrg_Key;
    CAN_IVar4_RegCmd.CAN_PBM4_Emerg_Charge_Key_Branch2                      = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].PCA9534_Emerg_Chrg_Key;

    CAN_IVar4_RegCmd.CAN_PBM1_Auto_Corr_Cap_Level_Branch1                   = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_1].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM1_Auto_Corr_Cap_Level_Branch2                   = eps_p.eps_pbm_ptr[PBM_T1_1].Branch[PBM_T1_BRANCH_2].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM2_Auto_Corr_Cap_Level_Branch1                   = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_1].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM2_Auto_Corr_Cap_Level_Branch2                   = eps_p.eps_pbm_ptr[PBM_T1_2].Branch[PBM_T1_BRANCH_2].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM3_Auto_Corr_Cap_Level_Branch1                   = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_1].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM3_Auto_Corr_Cap_Level_Branch2                   = eps_p.eps_pbm_ptr[PBM_T1_3].Branch[PBM_T1_BRANCH_2].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM4_Auto_Corr_Cap_Level_Branch1                   = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_1].Auto_Corr_Capacity_Key;
    CAN_IVar4_RegCmd.CAN_PBM4_Auto_Corr_Cap_Level_Branch2                   = eps_p.eps_pbm_ptr[PBM_T1_4].Branch[PBM_T1_BRANCH_2].Auto_Corr_Capacity_Key;

    CAN_IVar4_RegCmd.CAN_PBM1_Corr_Cap_Level_Branch1                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM1_Corr_Cap_Level_Branch2                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM2_Corr_Cap_Level_Branch1                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM2_Corr_Cap_Level_Branch2                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM3_Corr_Cap_Level_Branch1                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM3_Corr_Cap_Level_Branch2                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM4_Corr_Cap_Level_Branch1                        = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM4_Corr_Cap_Level_Branch2                        = 0x00;

    CAN_IVar4_RegCmd.CAN_PBM1_Reset_Branch1                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM1_Reset_Branch2                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM2_Reset_Branch1                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM2_Reset_Branch2                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM3_Reset_Branch1                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM3_Reset_Branch2                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM4_Reset_Branch1                                 = 0x00;
    CAN_IVar4_RegCmd.CAN_PBM4_Reset_Branch2                                 = 0x00;
}
