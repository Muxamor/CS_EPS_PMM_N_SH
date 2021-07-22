#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_tim.h"
#include "median_filter.h"
#include "tim_pwm.h"
#include "PBM/pbm_config.h"
#include "PBM/pbm_control.h"
#include "PBM/pbm_init.h"
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

void CAN_Var4_cmd_parser(uint64_t *cmd_status, _EPS_Param eps_p ){

	uint8_t number_cmd_reg = 0;
	uint64_t cmd_bit_flag = 0;

	NVIC_DisableIRQ(CAN1_RX0_IRQn);
	NVIC_DisableIRQ(CAN2_RX0_IRQn);

	for( number_cmd_reg = 0; number_cmd_reg < 64; number_cmd_reg++ ){

		cmd_bit_flag = (*cmd_status) >> number_cmd_reg;

		if( cmd_bit_flag & 0x0000000000000001 ){

			switch (number_cmd_reg) {

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

                case CAN_AB1_Charge_key_1_offset: //Enable/Disable Charge AB1 branch 1 // PBM_data

                    if( CAN_IVar4_RegCmd.CAN_AB1_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB1 branch 1\n", CAN_AB1_Charge_key_1_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_1, PBM_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB1 branch 1\n", CAN_AB1_Charge_key_1_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_1, PBM_OFF_CHARGE);
                    }
                    break;

                case CAN_AB1_Discharge_key_1_offset: //Enable/Disable Discharge AB1 branch 1 // PBM_data

                    if( CAN_IVar4_RegCmd.CAN_AB1_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB1 branch 1\n", CAN_AB1_Discharge_key_1_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_1, PBM_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB1 branch 1\n", CAN_AB1_Discharge_key_1_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_1, PBM_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB1_Charge_key_2_offset: //Enable/Disable Charge AB1 branch 2 // PBM_data

                    if (CAN_IVar4_RegCmd.CAN_AB1_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB1 branch 2\n", CAN_AB1_Charge_key_2_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_2, PBM_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB1 branch 2\n", CAN_AB1_Charge_key_2_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_2, PBM_OFF_CHARGE);
                    }
                    break;

                case CAN_AB1_Discharge_key_2_offset: //Enable/Disable Discharge AB1 branch 2 // PBM_data

                    if (CAN_IVar4_RegCmd.CAN_AB1_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB1 branch 2\n", CAN_AB1_Discharge_key_2_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_2, PBM_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB1 branch 2\n", CAN_AB1_Discharge_key_2_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_2, PBM_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB2_Charge_key_1_offset: //Enable/Disable Charge AB2 branch 1 // PBM_data

                    if( CAN_IVar4_RegCmd.CAN_AB2_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB2 branch 1\n", CAN_AB2_Charge_key_1_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_1, PBM_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB2 branch 1\n", CAN_AB2_Charge_key_1_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_1, PBM_OFF_CHARGE);
                    }
                    break;

                case CAN_AB2_Discharge_key_1_offset: //Enable/Disable Discharge AB2 branch 1 // PBM_data

                    if( CAN_IVar4_RegCmd.CAN_AB2_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB2 branch 1\n", CAN_AB2_Discharge_key_1_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_1, PBM_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB2 branch 1\n", CAN_AB2_Discharge_key_1_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_1, PBM_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB2_Charge_key_2_offset: //Enable/Disable Charge AB2 branch 2 // PBM_data

                    if (CAN_IVar4_RegCmd.CAN_AB2_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB2 branch 2\n", CAN_AB2_Charge_key_2_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_2, PBM_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB2 branch 2\n", CAN_AB2_Charge_key_2_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_2, PBM_OFF_CHARGE);
                    }
                    break;

                case CAN_AB2_Discharge_key_2_offset: //Enable/Disable Discharge AB2 branch 2 // PBM_data

                    if (CAN_IVar4_RegCmd.CAN_AB2_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB2 branch 2\n", CAN_AB2_Discharge_key_2_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_2, PBM_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB2 branch 2\n", CAN_AB2_Discharge_key_2_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_2, PBM_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB3_Charge_key_1_offset: //Enable/Disable Charge AB3 branch 1 // PBM_data

                    if( CAN_IVar4_RegCmd.CAN_AB3_Charge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB3 branch 1\n", CAN_AB3_Charge_key_1_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_1, PBM_ON_CHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB3 branch 1\n", CAN_AB3_Charge_key_1_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_1, PBM_OFF_CHARGE);
                    }
                    break;

                case CAN_AB3_Discharge_key_1_offset: //Enable/Disable Discharge AB3 branch 1 // PBM_data

                    if( CAN_IVar4_RegCmd.CAN_AB3_Discharge_key_branch_1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB3 branch 1\n", CAN_AB3_Discharge_key_1_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_1, PBM_ON_DISCHARGE);
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB3 branch 1\n", CAN_AB3_Discharge_key_1_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_1, PBM_OFF_DISCHARGE);
                    }
                    break;

                case CAN_AB3_Charge_key_2_offset: //Enable/Disable Charge AB3 branch 2 // PBM_data

                    if (CAN_IVar4_RegCmd.CAN_AB3_Charge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE charge AB3 branch 2\n", CAN_AB3_Charge_key_2_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_2, PBM_ON_CHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE charge AB3 branch 2\n", CAN_AB3_Charge_key_2_offset);
                        #endif
                        PBM_SetStateChargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_2, PBM_OFF_CHARGE);
                    }
                    break;

                case CAN_AB3_Discharge_key_2_offset: //Enable/Disable Discharge AB3 branch 2 // PBM_data

                    if (CAN_IVar4_RegCmd.CAN_AB3_Discharge_key_branch_2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE discharge AB3 branch 2\n", CAN_AB3_Discharge_key_2_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_2, PBM_ON_DISCHARGE);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE discharge AB3 branch 2\n", CAN_AB3_Discharge_key_2_offset);
                        #endif
                        PBM_SetStateDischargeBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_2, PBM_OFF_DISCHARGE);
                    }
                    break;

                case CAN_PWR_CH1_offset: //Enable/Disable power channel 1 (SOP1)

                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 1\n", CAN_PWR_CH1_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_1, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 1\n", CAN_PWR_CH1_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_1, DISABLE );
                    }
                    break;

                case CAN_PWR_CH2_offset: //Enable/Disable power channel 2 (SOP2)

                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 2\n", CAN_PWR_CH2_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_2, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 2\n", CAN_PWR_CH2_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_2, DISABLE );
                    }
                    break;

                case CAN_PWR_CH3_offset: //Enable/Disable power channel 3 (BRK1)

                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 3\n", CAN_PWR_CH3_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 3\n", CAN_PWR_CH3_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_3, DISABLE );
                    }
                    break;

                case CAN_PWR_CH4_offset: //Enable/Disable power channel 4 (BRK2)  uint8_t CAN_PDM_PWR_CH4;

                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 4\n", CAN_PWR_CH4_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 4\n", CAN_PWR_CH4_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_4, DISABLE );
                    }
                    break;

                case CAN_PWR_CH5_offset: //Enable/Disable power channel 5

                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 5\n", CAN_PWR_CH5_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_5, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 5\n", CAN_PWR_CH5_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_5, DISABLE );
                    }
                    break;

                case CAN_PWR_CH6_offset: //Enable/Disable power channel 6

                    if( CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power channel 6\n", CAN_PWR_CH6_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_6, ENABLE );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power channel 6\n", CAN_PWR_CH6_offset);
                        #endif
                        PDM_Set_state_PWR_CH( eps_p.eps_pdm_ptr, PDM_PWR_Channel_6, DISABLE );
                    }
                    break;

                 case CAN_PWR_VBAT1_offset: //Enable/Disable power channel VBAT1

                    if( CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power VBAT1\n", CAN_PWR_VBAT1_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF1, ENABLE );
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF2, ENABLE ); 
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power VBAT1\n", CAN_PWR_VBAT1_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF1, DISABLE );
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT1_eF2, DISABLE );
                    }
                    break;

                case CAN_PWR_VBAT2_offset: //Enable/Disable power channel VBAT2

                    if( CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE power VBAT2\n", CAN_PWR_VBAT2_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF1, ENABLE );
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF2, ENABLE ); 
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE power VBAT2\n", CAN_PWR_VBAT2_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF1, DISABLE );
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr,  PMM_PWR_Ch_VBAT2_eF2, DISABLE );
                    }
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

					if( CAN_IVar4_RegCmd.CAN_Set_active_CPU == 0x00 ){
						#ifdef DEBUGprintf
							printf("Get comm. reg. %d -> Set active CPUmain\n", CAN_Switch_active_CPU_offset);
						#endif
                        if( eps_p.eps_pmm_ptr->Active_CPU != CAN_IVar4_RegCmd.CAN_Set_active_CPU ){
                            eps_p.eps_serv_ptr->Req_SW_Active_CPU = 1;
                            eps_p.eps_serv_ptr->Set_Active_CPU = CPUmain_Active;
                        }
					}else if( CAN_IVar4_RegCmd.CAN_Set_active_CPU == 0x01 ){
						#ifdef DEBUGprintf
							printf("Get comm. reg. %d -> Set active CPUbackup\n", CAN_Switch_active_CPU_offset);
						#endif
                        if( eps_p.eps_pmm_ptr->Active_CPU != CAN_IVar4_RegCmd.CAN_Set_active_CPU ){
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

                case CAN_AB1_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB1 branch 1 // PBM_data
                    if (CAN_IVar4_RegCmd.CAN_AB1_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB1 branch 1\n", CAN_AB1_Heat_Branch1_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_1, PBM_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB1 branch 1\n", CAN_AB1_Heat_Branch1_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_1, PBM_OFF_HEAT);
                    }
                    break;

                case CAN_AB1_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB1 branch 2 // PBM_data
                    if (CAN_IVar4_RegCmd.CAN_AB1_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB1 branch 2\n", CAN_AB1_Heat_Branch2_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_2, PBM_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB1 branch 2\n", CAN_AB1_Heat_Branch2_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 0, PBM_BRANCH_2, PBM_OFF_HEAT);
                    }
                    break;

                case CAN_AB2_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB2 branch 1 // PBM_data
                    if (CAN_IVar4_RegCmd.CAN_AB2_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB2 branch 1\n", CAN_AB2_Heat_Branch1_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_1, PBM_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB2 branch 1\n", CAN_AB2_Heat_Branch1_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_1, PBM_OFF_HEAT);
                    }
                    break;

                case CAN_AB2_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB2 branch 2 // PBM_data
                    if (CAN_IVar4_RegCmd.CAN_AB2_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB2 branch 2\n", CAN_AB2_Heat_Branch2_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_2, PBM_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB2 branch 2\n", CAN_AB2_Heat_Branch2_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 1, PBM_BRANCH_2, PBM_OFF_HEAT);
                    }
                    break;

                case CAN_AB3_Heat_Branch1_offset: //Enable/Disable Auto heat (thermostat) AB3 branch 1 // PBM_data
                    if (CAN_IVar4_RegCmd.CAN_AB3_Heat_Branch1 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB3 branch 1\n", CAN_AB3_Heat_Branch1_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_1, PBM_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB3 branch 1\n", CAN_AB3_Heat_Branch1_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_1, PBM_OFF_HEAT);
                    }
                    break;

                case CAN_AB3_Heat_Branch2_offset: //Enable/Disable Auto heat (thermostat) AB3 branch 2 // PBM_data
                    if (CAN_IVar4_RegCmd.CAN_AB3_Heat_Branch2 == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE auto heat AB3 branch 2\n", CAN_AB3_Heat_Branch2_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_2, PBM_ON_HEAT);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE auto heat AB3 branch 2\n", CAN_AB3_Heat_Branch2_offset);
                        #endif
                        PBM_SetStateHeatBranch(PBM_I2C_PORT, eps_p.eps_pbm_ptr, 2, PBM_BRANCH_2, PBM_OFF_HEAT);
                    }
                    break;

                case CAN_PAM_PWR_DC_DC_offset: //Enable/Disable PAM Power DC_DC source // PAM_data
                    if ( CAN_IVar4_RegCmd.CAN_PAM_PWR_DC_DC == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE PAM Power DC_DC source \n", CAN_PAM_PWR_DC_DC_offset);
                        #endif
                       // PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_DC_DC, ENABLE);
                        eps_p.eps_pam_ptr->State_DC_DC = ENABLE;
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
                        //PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_LDO, ENABLE);
                        eps_p.eps_pam_ptr->State_LDO = ENABLE;
                        PAM_init(eps_p.eps_pam_ptr);
                    } else {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE PAM Power LDO source \n", CAN_PAM_PWR_LDO_offset);
                        #endif
                        PAM_Set_state_PWR_Supply(eps_p.eps_pam_ptr, PAM_PWR_LDO, DISABLE);
                    }
                    break;

			    case CAN_PMM_PWR_PBM_Logic_offset://Enable/Disable PMM Power PBM Logic
                    if ( CAN_IVar4_RegCmd.CAN_PMM_PWR_PBM_Logic == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> ENABLE PMM  power PBM Logic\n", CAN_PMM_PWR_PBM_Logic_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_PBMs_Logic, ENABLE );
                        LL_mDelay(3);
                        PBM_Init( eps_p.eps_pbm_ptr );
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> DISABLE PMM  power PBM Logic\n", CAN_PMM_PWR_PBM_Logic_offset);
                        #endif
                        PMM_Set_state_PWR_CH( eps_p.eps_pmm_ptr, PMM_PWR_Ch_PBMs_Logic, DISABLE );
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

			    case CAN_PMM_PWR_OFF_Passive_CPU_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PMM_PWR_OFF_Passive_CPU == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Enable power OFF Passive CPU\n", CAN_PMM_PWR_OFF_Passive_CPU_offset);
                        #endif
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = ENABLE;
                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d -> Disable power OFF Passive CPU\n", CAN_PMM_PWR_OFF_Passive_CPU_offset);
                        #endif
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = DISABLE;
                    }
                    eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
			        break;

                case CAN_PMM_Reboot_Passive_CPU_offset:
                    if ( CAN_IVar4_RegCmd.CAN_PMM_Reboot_Passive_CPU == 0x01) {
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. %d ->Reboot Passive CPU\n", CAN_PMM_Reboot_Passive_CPU_offset);
                        #endif
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        LL_mDelay(50);
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
                        PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
                        CAN_IVar4_RegCmd.CAN_PMM_Reboot_Passive_CPU = 0x00;
                    }
                    break;

                default:
					break;
			}

			if( ( cmd_bit_flag & 0xFFFFFFFFFFFFFFFE ) == 0 ){
				break;
			}
		}
	}

	*cmd_status = 0;
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	NVIC_EnableIRQ(CAN2_RX0_IRQn);
}


void CAN_Var5_fill_telemetry( _EPS_Param eps_p ){

	uint8_t num_pwr_ch = 0;

	// -------------------  ТМИ 4  ------------------ //
    CAN_IVar5_telemetry.CAN_SP_current_pX                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[3].Current_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SP_current_nX                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[1].Current_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SP_current_pY                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[0].Current_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SP_current_nY                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[2].Current_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SPF_current_1    							= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[4].Current_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SPF_current_2								= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[5].Current_val; // PAM_data


    CAN_IVar5_telemetry.CAN_SP_voltage_pX                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[3].Voltage_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SP_voltage_nX                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[1].Voltage_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SP_voltage_pY                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[0].Voltage_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SP_voltage_nY                           	= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[2].Voltage_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SPF_voltage_1    							= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[4].Voltage_val; // PAM_data
    CAN_IVar5_telemetry.CAN_SPF_voltage_2								= (uint16_t) eps_p.eps_pam_ptr->PWR_IN_Channel[5].Voltage_val; // PAM_data


    CAN_IVar5_telemetry.CAN_Panel_median_temperature_pX                 = (uint8_t) GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value, PAM_SP4_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_Panel_median_temperature_nX                 = (uint8_t) GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value, PAM_SP2_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_Panel_median_temperature_pY                 = (uint8_t) GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value, PAM_SP5_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_Panel_median_temperature_nY                 = (uint8_t) GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value, PAM_SP6_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_Hinged_panel_median_temperature_pY          = (uint8_t) GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value, PAM_SP1_temp_sens_quantity );
    CAN_IVar5_telemetry.CAN_Hinged_panel_median_temperature_nY          = (uint8_t) GetMedian( eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value, PAM_SP3_temp_sens_quantity );

    CAN_IVar5_telemetry.CAN_Solar_panel_status[0]                       = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[0] & 0x01) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[1] & 0x01) << 1) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[2] & 0x01) << 2) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[3] & 0x01) << 3) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[4] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].State_ID[5] & 0x01) << 5) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_I2C_GPIO_Ext1) << 6) | (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch2].State_ID_In << 7);

    CAN_IVar5_telemetry.CAN_Solar_panel_status[1]                       = (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch2].Error_PWR_Mon) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[PAM_PWR_TM_SP_Ch2].PG_eF_out << 1 ) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[0] & 0x01) << 2) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[1] & 0x01) << 3) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[2] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[3] & 0x01) << 5) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[4] & 0x01) << 6) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].State_ID[5] & 0x01) << 7);


    CAN_IVar5_telemetry.CAN_Solar_panel_status[2]                       = (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_I2C_GPIO_Ext1) | (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch4].State_ID_In << 1) |
                                                                          (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch4].Error_PWR_Mon << 2 ) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[PAM_PWR_TM_SP_Ch4].PG_eF_out << 3 ) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].State_ID[0] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].State_ID[1] & 0x01) << 5) |
                                                                          (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_I2C_GPIO_Ext1 << 6 ) | (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch1].State_ID_In << 7);

    CAN_IVar5_telemetry.CAN_Solar_panel_status[3]                       = (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch1].Error_PWR_Mon) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[PAM_PWR_TM_SP_Ch5].PG_eF_out << 1 ) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].State_ID[0] & 0x01) << 2) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].State_ID[1] & 0x01) << 3) |
                                                                          ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].State_ID[2] & 0x01) << 4) | ((eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_I2C_GPIO_Ext1) << 5) |
                                                                          (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch3].State_ID_In << 6) | (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch3].Error_PWR_Mon << 7);

    CAN_IVar5_telemetry.CAN_Solar_panel_status[4]                       = (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[PAM_PWR_TM_SP_Ch6].PG_eF_out) | (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch5].State_ID_In << 1) |
                                                                          (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch5].Error_PWR_Mon << 2) | (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch6].State_ID_In << 3) |
                                                                          (eps_p.eps_pam_ptr->PWR_IN_Channel[PAM_PWR_IN_Ch6].Error_PWR_Mon << 4) | (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[PAM_PWR_TM_SP_Ch1].PG_eF_out << 5 ) |
                                                                          (eps_p.eps_pam_ptr->PWR_Channel_TM_SP[PAM_PWR_TM_SP_Ch3].PG_eF_out << 6 );

    CAN_IVar5_telemetry.CAN_Full_ABs_Energy_level_percent 				= 0x0000;
    for( num_pwr_ch = 0; num_pwr_ch < PBM_QUANTITY; num_pwr_ch++ ){
    	CAN_IVar5_telemetry.CAN_Full_ABs_Energy_level_percent           = CAN_IVar5_telemetry.CAN_Full_ABs_Energy_level_percent + (uint8_t)(eps_p.eps_pbm_ptr[num_pwr_ch].TotalRelativeCapacity / 3);
    }  // PBM_data

    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB1_line1             = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_AbcoluteCapacity; // PBM_data
	CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB1_line2             = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_AbcoluteCapacity; // PBM_data
	CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB2_line1             = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_AbcoluteCapacity; // PBM_data
	CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB2_line2             = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_AbcoluteCapacity; // PBM_data
	CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line1             = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_AbcoluteCapacity; // PBM_data
	CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line2             = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_AbcoluteCapacity; // PBM_data

	CAN_IVar5_telemetry.CAN_Full_capacity_charge_level	 				= 0x0000;
    for( num_pwr_ch = 0; num_pwr_ch < PBM_QUANTITY; num_pwr_ch++ ){
    	CAN_IVar5_telemetry.CAN_Full_capacity_charge_level		        = CAN_IVar5_telemetry.CAN_Full_capacity_charge_level + (uint16_t)(eps_p.eps_pbm_ptr[num_pwr_ch].TotalAbcoluteCapacity);
    }  // PBM_data


	//***
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
	//***

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

	//***
	CAN_IVar5_telemetry.CAN_Charge_discharge_AB_key_status              = (uint16_t)((eps_p.eps_pbm_ptr[0].Error_Charge_1) | (eps_p.eps_pbm_ptr[0].Error_Discharge_1 << 1) |
	                                                                        (eps_p.eps_pbm_ptr[0].Error_Charge_2 << 2) | (eps_p.eps_pbm_ptr[0].Error_Discharge_2 << 3) |
	                                                                        (eps_p.eps_pbm_ptr[1].Error_Charge_1 << 4) | (eps_p.eps_pbm_ptr[1].Error_Discharge_1 << 5) |
	                                                                        (eps_p.eps_pbm_ptr[1].Error_Charge_2 << 6) | (eps_p.eps_pbm_ptr[1].Error_Discharge_2 << 7) |
	                                                                        (eps_p.eps_pbm_ptr[2].Error_Charge_1 << 8) | (eps_p.eps_pbm_ptr[2].Error_Discharge_1 << 9) |
	                                                                        (eps_p.eps_pbm_ptr[2].Error_Charge_2 << 10) | (eps_p.eps_pbm_ptr[2].Error_Discharge_2 << 11)); 	// PBM_data
    //---

	CAN_IVar5_telemetry.CAN_Channel1_current                            =  (uint16_t)( eps_p.eps_pdm_ptr->PWR_Channel[0].Current_val );
	CAN_IVar5_telemetry.CAN_Channel2_current                            =  (uint16_t)( eps_p.eps_pdm_ptr->PWR_Channel[1].Current_val );
	CAN_IVar5_telemetry.CAN_Channel3_current                            =  (uint16_t)( eps_p.eps_pdm_ptr->PWR_Channel[2].Current_val );
	CAN_IVar5_telemetry.CAN_Channel4_current                            =  (uint16_t)( eps_p.eps_pdm_ptr->PWR_Channel[3].Current_val );
	CAN_IVar5_telemetry.CAN_Channel5_current                            =  0x0000;
	CAN_IVar5_telemetry.CAN_Channel6_current                            =  0x0000;

	//***
    if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < 4000 ){  
        CAN_IVar5_telemetry.CAN_VBAT1_voltage                           =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Voltage_val;  
    }else{
        CAN_IVar5_telemetry.CAN_VBAT1_voltage                           =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val;  
    }
    //---

    //***
    if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val < 4000 ){  
        CAN_IVar5_telemetry.CAN_VBAT2_voltage                           =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Voltage_val;  
    }else{
        CAN_IVar5_telemetry.CAN_VBAT2_voltage                           =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val;  
    }
    //---

    CAN_IVar5_telemetry.CAN_VBAT1_current                               = (uint16_t)( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Current_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Current_val );
    CAN_IVar5_telemetry.CAN_VBAT2_current                               = (uint16_t)( eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Current_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Current_val );

    //***
	CAN_IVar5_telemetry.CAN_Subsystem_power_line_status                 =  0x00;
	for( num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){

		if( (eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_out != SUCCESS) || (eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_in != SUCCESS) || \
				(eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_out != SUCCESS) || (eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_in != SUCCESS) ){

			CAN_IVar5_telemetry.CAN_Subsystem_power_line_status = ( CAN_IVar5_telemetry.CAN_Subsystem_power_line_status | (1 << num_pwr_ch) );
		}
	}

    if(  (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 != SUCCESS) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 != SUCCESS) || (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat1_eF1 != SUCCESS) \
        ||  (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat1_eF2 != SUCCESS) ){

        CAN_IVar5_telemetry.CAN_Subsystem_power_line_status = ( CAN_IVar5_telemetry.CAN_Subsystem_power_line_status | (1 << 6) );
    }

    if(  (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 != SUCCESS) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 != SUCCESS) || (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat2_eF1 != SUCCESS) \
         ||  (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat2_eF2 != SUCCESS) ){

        CAN_IVar5_telemetry.CAN_Subsystem_power_line_status = ( CAN_IVar5_telemetry.CAN_Subsystem_power_line_status | (1 << 7) );
    }
    //---

    CAN_IVar5_telemetry.CAN_SES_current_consumption                     = (uint16_t)(eps_p.eps_pmm_ptr->PWR_Supply_Backup_eF_out_Current_val + eps_p.eps_pmm_ptr->PWR_Supply_Main_eF_out_Current_val);
    CAN_IVar5_telemetry.CAN_SES_Voltage_power_supply                    = 3300; // TODO Позже придумать как мерить.

    //***
    CAN_IVar5_telemetry.CAN_Full_charge_discharge_power 				= 0x0000;
    for( num_pwr_ch = 0; num_pwr_ch < PBM_QUANTITY; num_pwr_ch++ ){
    	CAN_IVar5_telemetry.CAN_Full_charge_discharge_power             = CAN_IVar5_telemetry.CAN_Full_charge_discharge_power +
    																		(uint16_t)( ( ( (eps_p.eps_pbm_ptr[num_pwr_ch].Branch_1_VoltageHi + eps_p.eps_pbm_ptr[num_pwr_ch].Branch_1_VoltageLo) *
    																		eps_p.eps_pbm_ptr[num_pwr_ch].Branch_1_Current) / 1000 ) +
                															( ((eps_p.eps_pbm_ptr[num_pwr_ch].Branch_2_VoltageHi + eps_p.eps_pbm_ptr[num_pwr_ch].Branch_2_VoltageLo) *
                															eps_p.eps_pbm_ptr[num_pwr_ch].Branch_2_Current) / 1000 ) );
    }  // PBM_data
    //---

    //***
    CAN_IVar5_telemetry.CAN_Total_Generate_Power_SP 					= 0x0000;
    for( num_pwr_ch = 0; num_pwr_ch < PAM_PWR_IN_Ch_quantity; num_pwr_ch++ ){
    	CAN_IVar5_telemetry.CAN_Total_Generate_Power_SP             	= CAN_IVar5_telemetry.CAN_Total_Generate_Power_SP + eps_p.eps_pam_ptr->PWR_IN_Channel[num_pwr_ch].Power_val;
    }  // PAM_data
    //---

    //***
	CAN_IVar5_telemetry.CAN_Spacecraft_total_power                      =  0x0000;
	for( num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++ ){
		CAN_IVar5_telemetry.CAN_Spacecraft_total_power                  = CAN_IVar5_telemetry.CAN_Spacecraft_total_power + eps_p.eps_pdm_ptr->PWR_Channel[num_pwr_ch].Power_val;
	}

    CAN_IVar5_telemetry.CAN_Spacecraft_total_power                      = CAN_IVar5_telemetry.CAN_Spacecraft_total_power + eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Power_val +
                                                                            eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF2_Power_val + eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Power_val +
                                                                            eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val;
    //---


//
    CAN_IVar5_telemetry.CAN_Median_PMM_temp                             =  (uint8_t) eps_p.eps_pmm_ptr->Temp_sensor;
    CAN_IVar5_telemetry.CAN_Median_PAM_temp                             =  (uint8_t) GetMedian( eps_p.eps_pam_ptr->Temp_sensor, 4 ); // PAM_data
	CAN_IVar5_telemetry.CAN_Median_PDM_temp                             =  (uint8_t) GetMedian( eps_p.eps_pdm_ptr->Temp_sensor, 4 );

    //*** EPS status
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[0]        =  (eps_p.eps_pmm_ptr->Main_Backup_mode_CPU) | (eps_p.eps_pmm_ptr->Active_CPU<<1) |
                                                                           (eps_p.eps_pmm_ptr->Error_UART_port_M<<2) | (eps_p.eps_pmm_ptr->Error_UART_port_B<<3) |
                                                                           (eps_p.eps_pmm_ptr->Error_CAN_port_M<<4) | (eps_p.eps_pmm_ptr->Error_CAN_port_M<<5) |
                                                                           (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANmain<<6) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANbackup<<7);

    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] = 0;

    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1]        =  eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1]        =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (eps_p.eps_pmm_ptr->PWR_Supply_Main_PG<<1);
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1]        =  CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (eps_p.eps_pmm_ptr->PWR_Supply_Backup_PG<<2);

    //PMM Some Error
    if( (eps_p.eps_pmm_ptr->Error_TMP1075_sensor == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANmain == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANbackup == 1) ||
        (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 == 1) ||
        (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_PBMs_Logic == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Deploy_Logic == 1) ||
        (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Deploy_Power == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_5V_Bus == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_3_3V_Bus == 1) ||
        (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_I2C_Bus == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF1 == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF2 == 1) ||
        (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF1 == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF2 == 1) || (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext1 == 1) ||
        (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext2 == 1) || (eps_p.eps_pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon == 1) ||(eps_p.eps_pmm_ptr->Error_I2C_Deploy_GPIO_Ext == 1) ||
        (eps_p.eps_pmm_ptr->Error_I2C_Deploy_ADC == 1) || (eps_p.eps_pmm_ptr->Error_FRAM1 == 1) || (eps_p.eps_pmm_ptr->Error_FRAM2 == 1) || (eps_p.eps_pmm_ptr->Error_UART_port_M == 1) ||
        (eps_p.eps_pmm_ptr->Error_UART_port_B == 1) || (eps_p.eps_pmm_ptr->Error_CAN_port_M == 1) || (eps_p.eps_pmm_ptr->Error_CAN_port_B == 1) ){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (1<<3);
    }

    //PDM Some Error
    if( (eps_p.eps_pdm_ptr->Error_I2C_GPIO_Ext1 == 1) || (eps_p.eps_pdm_ptr->Error_I2C_GPIO_Ext2 == 1) || (eps_p.eps_pdm_ptr->Error_I2C_MUX == 1) || (eps_p.eps_pdm_ptr->Error_temp_sensor_1 == 1) ||
        (eps_p.eps_pdm_ptr->Error_temp_sensor_2 == 1) || (eps_p.eps_pdm_ptr->Error_temp_sensor_3 == 1) || (eps_p.eps_pdm_ptr->Error_temp_sensor_4 == 1) ){
        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (1<<4);
    }

    if( (eps_p.eps_pbm_ptr[0].Error_DS2777_1 == 1) || (eps_p.eps_pbm_ptr[0].Error_DS2777_2 == 1) || (eps_p.eps_pbm_ptr[0].Error_TMP1075_1 == 1) || (eps_p.eps_pbm_ptr[0].Error_TMP1075_2 == 1) ||
        (eps_p.eps_pbm_ptr[0].Error_TMP1075_3 == 1) || (eps_p.eps_pbm_ptr[0].Error_TMP1075_4 == 1) || (eps_p.eps_pbm_ptr[0].Error_PCA9534 == 1) || (eps_p.eps_pbm_ptr[0].Error_Heat_1 == 1) ||
        (eps_p.eps_pbm_ptr[0].Error_Heat_2 == 1) || (eps_p.eps_pbm_ptr[0].Error_Charge_1 == 1) || (eps_p.eps_pbm_ptr[0].Error_Charge_2 == 1) || (eps_p.eps_pbm_ptr[0].Error_Discharge_1 == 1) ||
        (eps_p.eps_pbm_ptr[0].Error_Discharge_2 == 1) || (eps_p.eps_pbm_ptr[0].Low_Energy_Flag == 1) || (eps_p.eps_pbm_ptr[0].Zero_Energy_Flag == 1) ){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (1<<5);
    }

    if( (eps_p.eps_pbm_ptr[1].Error_DS2777_1 == 1) || (eps_p.eps_pbm_ptr[1].Error_DS2777_2 == 1) || (eps_p.eps_pbm_ptr[1].Error_TMP1075_1 == 1) || (eps_p.eps_pbm_ptr[1].Error_TMP1075_2 == 1) ||
        (eps_p.eps_pbm_ptr[1].Error_TMP1075_3 == 1) || (eps_p.eps_pbm_ptr[1].Error_TMP1075_4 == 1) || (eps_p.eps_pbm_ptr[1].Error_PCA9534 == 1) || (eps_p.eps_pbm_ptr[1].Error_Heat_1 == 1) ||
        (eps_p.eps_pbm_ptr[1].Error_Heat_2 == 1) || (eps_p.eps_pbm_ptr[1].Error_Charge_1 == 1) || (eps_p.eps_pbm_ptr[1].Error_Charge_2 == 1) || (eps_p.eps_pbm_ptr[1].Error_Discharge_1 == 1) ||
        (eps_p.eps_pbm_ptr[1].Error_Discharge_2 == 1) || (eps_p.eps_pbm_ptr[1].Low_Energy_Flag == 1) || (eps_p.eps_pbm_ptr[1].Zero_Energy_Flag == 1) ){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (1<<6);
    }

    if( (eps_p.eps_pbm_ptr[2].Error_DS2777_1 == 1) || (eps_p.eps_pbm_ptr[2].Error_DS2777_2 == 1) || (eps_p.eps_pbm_ptr[2].Error_TMP1075_1 == 1) || (eps_p.eps_pbm_ptr[2].Error_TMP1075_2 == 1) ||
        (eps_p.eps_pbm_ptr[2].Error_TMP1075_3 == 1) || (eps_p.eps_pbm_ptr[2].Error_TMP1075_4 == 1) || (eps_p.eps_pbm_ptr[2].Error_PCA9534 == 1) || (eps_p.eps_pbm_ptr[2].Error_Heat_1 == 1) ||
        (eps_p.eps_pbm_ptr[2].Error_Heat_2 == 1) || (eps_p.eps_pbm_ptr[2].Error_Charge_1 == 1) || (eps_p.eps_pbm_ptr[2].Error_Charge_2 == 1) || (eps_p.eps_pbm_ptr[2].Error_Discharge_1 == 1) ||
        (eps_p.eps_pbm_ptr[2].Error_Discharge_2 == 1) || (eps_p.eps_pbm_ptr[2].Low_Energy_Flag == 1) || (eps_p.eps_pbm_ptr[2].Zero_Energy_Flag == 1) ){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1] | (1<<7);
    }

    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]        =  0x00;

    if( (eps_p.eps_pam_ptr->PG_DC_DC == ERROR) || (eps_p.eps_pam_ptr->PG_LDO == ERROR) || (eps_p.eps_pam_ptr->Error_State_DC_DC == ERROR) || (eps_p.eps_pam_ptr->Error_State_LDO == ERROR) ||
        (eps_p.eps_pam_ptr->Error_I2C_GPIO_Ext == ERROR) || (eps_p.eps_pam_ptr->Error_I2C_MUX_1 == ERROR) || (eps_p.eps_pam_ptr->Error_I2C_MUX_2 == ERROR) || (eps_p.eps_pam_ptr->Error_temp_sensor_1 == ERROR) ||
        (eps_p.eps_pam_ptr->Error_temp_sensor_2 == ERROR) || (eps_p.eps_pam_ptr->Error_temp_sensor_3 == ERROR) || (eps_p.eps_pam_ptr->Error_temp_sensor_4 == ERROR) ){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    =  1;
    }

    if( (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_temp_sensor[0] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_temp_sensor[1] == ERROR) ||
        (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_temp_sensor[2] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_temp_sensor[3] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Error_I2C_GPIO_Ext1 == ERROR)){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2] | (1<<1);
    }

    if( (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_temp_sensor[0] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_temp_sensor[1] == ERROR) ||
        (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_temp_sensor[2] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_temp_sensor[3] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Error_I2C_GPIO_Ext1 == ERROR)){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2] | (1<<2);
    }

    if( (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_temp_sensor[0] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_temp_sensor[1] == ERROR) ||
       (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Error_I2C_GPIO_Ext1 == ERROR)){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2] | (1<<3);
    }

    if( (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_temp_sensor[0] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_temp_sensor[1] == ERROR) ||
        (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Error_I2C_GPIO_Ext1 == ERROR)){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2] | (1<<4);
    }

    if( (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_temp_sensor[0] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_temp_sensor[1] == ERROR) ||
        (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_temp_sensor[2] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_temp_sensor[3] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Error_I2C_GPIO_Ext1 == ERROR)){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2] | (1<<5);
    }

    if( (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_temp_sensor[0] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_temp_sensor[1] == ERROR) ||
        (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_temp_sensor[2] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_temp_sensor[3] == ERROR) || (eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Error_I2C_GPIO_Ext1 == ERROR)){

        CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]    = CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2] | (1<<6);
    }
    //---


    uint16_t  tmp_voltage;
    if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < 4000 ){
        tmp_voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val;
    }else{
        tmp_voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val;
    }
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[3]        =  (uint8_t)tmp_voltage;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[4]        =  (uint8_t)(tmp_voltage>>8);
    //---

    CAN_IVar5_telemetry.CAN_Number_of_restarts_of_the_SES_module        =  eps_p.eps_pmm_ptr->reboot_counter_CPUm;
    CAN_IVar5_telemetry.CAN_Number_of_restarts_of_the_reserve_SES_module=  eps_p.eps_pmm_ptr->reboot_counter_CPUb;
    CAN_IVar5_telemetry.CAN_State_PWR_CANmain                           =  eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain;
    CAN_IVar5_telemetry.CAN_State_PWR_CANbackup                         =  eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup;
    CAN_IVar5_telemetry.CAN_OFF_PWR_Passive_CPU                         =  eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU;
    CAN_IVar5_telemetry.CAN_EPS_Const_Mode                              =  eps_p.eps_pmm_ptr->CAN_constatnt_mode;

    CAN_IVar5_telemetry.CAN_Primary_standby_switch                      =  eps_p.eps_pmm_ptr->Active_CPU;
//	    // -------------------  ТМИ 7  ------------------ //
	CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB1_line1          = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_Current;  // PBM_data
	CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB1_line2          = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_Current;  // PBM_data
	CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB2_line1          = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_Current;  // PBM_data
	CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB2_line2          = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_Current;  // PBM_data
	CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB3_line1          = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_Current;  // PBM_data
	CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB3_line2          = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_Current;  // PBM_data

	CAN_IVar5_telemetry.CAN_Voltage_AB1_line1_battery_1                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_VoltageHi; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB1_line1_battery_2                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_1_VoltageLo; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB1_line2_battery_1                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_VoltageHi; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB1_line2_battery_2                 = (uint16_t) eps_p.eps_pbm_ptr[0].Branch_2_VoltageLo; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB2_line1_battery_1                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_VoltageHi; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB2_line1_battery_2                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_1_VoltageLo; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB2_line2_battery_1                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_VoltageHi; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB2_line2_battery_2                 = (uint16_t) eps_p.eps_pbm_ptr[1].Branch_2_VoltageLo; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB3_line1_battery_1                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_VoltageHi; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB3_line1_battery_2                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_1_VoltageLo; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB3_line2_battery_1                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_VoltageHi; 		// PBM_data
	CAN_IVar5_telemetry.CAN_Voltage_AB3_line2_battery_2                 = (uint16_t) eps_p.eps_pbm_ptr[2].Branch_2_VoltageLo; 		// PBM_data

	CAN_IVar5_telemetry.CAN_Percent_charge_level_AB1_line1              = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_1_RelativeCapacity;  // PBM_data
	CAN_IVar5_telemetry.CAN_Percent_charge_level_AB1_line2              = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_2_RelativeCapacity;  // PBM_data
	CAN_IVar5_telemetry.CAN_Percent_charge_level_AB2_line1              = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_1_RelativeCapacity;  // PBM_data
	CAN_IVar5_telemetry.CAN_Percent_charge_level_AB2_line2              = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_2_RelativeCapacity;  // PBM_data
	CAN_IVar5_telemetry.CAN_Percent_charge_level_AB3_line1              = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_1_RelativeCapacity;  // PBM_data
	CAN_IVar5_telemetry.CAN_Percent_charge_level_AB3_line2              = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_2_RelativeCapacity;  // PBM_data

	CAN_IVar5_telemetry.CAN_Channel1_voltage                            = eps_p.eps_pdm_ptr->PWR_Channel[0].Voltage_val;
	CAN_IVar5_telemetry.CAN_Channel2_voltage                            = eps_p.eps_pdm_ptr->PWR_Channel[1].Voltage_val;
	CAN_IVar5_telemetry.CAN_Channel3_voltage                            = eps_p.eps_pdm_ptr->PWR_Channel[2].Voltage_val;
	CAN_IVar5_telemetry.CAN_Channel4_voltage                            = eps_p.eps_pdm_ptr->PWR_Channel[3].Voltage_val;
	CAN_IVar5_telemetry.CAN_Channel5_voltage                            = 0x0000;
	CAN_IVar5_telemetry.CAN_Channel6_voltage                            = 0x0000;

	CAN_IVar5_telemetry.CAN_Opening_elements_state                      = (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1) | (eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 << 1) |
	                                                                      (eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1_Yp << 2) | (eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2_Yp << 3) |
                                                                          (eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1_Yn << 4) | (eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2_Yn << 5) |
                                                                          (eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1_Zp << 6) | (eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2_Zp << 7) |
                                                                          (eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1_Zn << 8) | (eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2_Zn << 9) |
	                                                                      (eps_p.eps_pmm_ptr->Deploy_stage << 12);


	//PMM
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[0]                   = (eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain) | (eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup << 1) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF1 << 2) |
                                                                          (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2 << 3) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF1 << 4) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2 << 5) |
                                                                          (eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic << 6) | (eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic << 7);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[1]                   = (eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Power) | (eps_p.eps_pmm_ptr->PWR_Ch_State_5V_Bus << 1) | (eps_p.eps_pmm_ptr->PWR_Ch_State_3_3V_Bus << 2) |
                                                                          (eps_p.eps_pmm_ptr->PWR_Ch_State_I2C_Bus << 3) | (eps_p.eps_pmm_ptr->PWR_Supply_Main_PG << 4) | (eps_p.eps_pmm_ptr->PWR_Supply_Backup_PG << 5) |
                                                                          (eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU << 6) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_CANmain << 7);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[2]                   = (eps_p.eps_pmm_ptr->PWR_Ch_PG_CANbackup) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat1_eF1 << 1) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat1_eF2 << 2) |
                                                                          (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat2_eF1 << 3) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_Vbat2_eF2 << 4) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_PBMs_Logic << 5) |
                                                                          (eps_p.eps_pmm_ptr->PWR_Ch_PG_Deploy_LP << 6) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_5V_Bus << 7);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[3]                   = (eps_p.eps_pmm_ptr->PWR_Ch_PG_3_3V_Bus) | (eps_p.eps_pmm_ptr->PWR_Ch_PG_I2C_Bus << 1) | (eps_p.eps_pmm_ptr->Main_Backup_mode_CPU << 2) |
                                                                          (eps_p.eps_pmm_ptr->Active_CPU << 3) | (eps_p.eps_pmm_ptr-> Deploy_stage  << 4);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[4]                   = eps_p.eps_pmm_ptr->EPS_Mode;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[5]                   = (eps_p.eps_pmm_ptr->Error_TMP1075_sensor) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANmain << 1) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_CANbackup << 2) |
                                                                          (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 << 3) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 << 4) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 << 5) |
                                                                          (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 << 6) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_PBMs_Logic << 7);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[6]                   = (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Deploy_Logic) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_Deploy_Power << 1) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_5V_Bus << 2) |
                                                                          (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_3_3V_Bus << 3) | (eps_p.eps_pmm_ptr->Error_PWR_Ch_State_I2C_Bus << 4) | (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF1 << 5) |
                                                                          (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat1_eF2 << 6) | (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF1 << 7);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[7]                   = (eps_p.eps_pmm_ptr->Error_PWR_Mon_Vbat2_eF2) | (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext1 << 1) | (eps_p.eps_pmm_ptr->Error_I2C_GPIO_Ext2 << 2) |
                                                                          (eps_p.eps_pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon << 3) | (eps_p.eps_pmm_ptr->Error_I2C_Deploy_GPIO_Ext << 4) | (eps_p.eps_pmm_ptr->Error_I2C_Deploy_ADC << 5) |
                                                                          (eps_p.eps_pmm_ptr->Error_FRAM1 << 6) | (eps_p.eps_pmm_ptr->Error_FRAM2 << 7);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[8]                   = (eps_p.eps_pmm_ptr->Error_UART_port_M) | (eps_p.eps_pmm_ptr->Error_UART_port_B << 1) | (eps_p.eps_pmm_ptr->Error_CAN_port_M << 2) |
                                                                          (eps_p.eps_pmm_ptr->Error_CAN_port_B << 3) | (eps_p.eps_pmm_ptr->CAN_constatnt_mode << 4);
	CAN_IVar5_telemetry.CAN_SES_module_data_array2[9]                   = 0x00;

	//PDM
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[10]                  = (eps_p.eps_pdm_ptr->Error_I2C_GPIO_Ext1) | (eps_p.eps_pdm_ptr->Error_I2C_GPIO_Ext2 << 1) | (eps_p.eps_pdm_ptr->Error_I2C_MUX << 2) |
                                                                          (eps_p.eps_pdm_ptr->Error_temp_sensor_1 << 3) | (eps_p.eps_pdm_ptr->Error_temp_sensor_2 << 4) | (eps_p.eps_pdm_ptr->Error_temp_sensor_3 << 5) |
                                                                          (eps_p.eps_pdm_ptr->Error_temp_sensor_4 << 6);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[11]                  = (eps_p.eps_pdm_ptr->PWR_Channel[0].State_eF_in) | (eps_p.eps_pdm_ptr->PWR_Channel[0].State_eF_out << 1) | (eps_p.eps_pdm_ptr->PWR_Channel[0].PG_eF_in << 2) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[0].PG_eF_out << 3) | (eps_p.eps_pdm_ptr->PWR_Channel[0].Error_State_eF_in << 4) | (eps_p.eps_pdm_ptr->PWR_Channel[0].Error_State_eF_out << 5) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[0].Error_PWR_Mon << 6);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[12]                  = (eps_p.eps_pdm_ptr->PWR_Channel[1].State_eF_in) | (eps_p.eps_pdm_ptr->PWR_Channel[1].State_eF_out << 1) | (eps_p.eps_pdm_ptr->PWR_Channel[1].PG_eF_in << 2) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[1].PG_eF_out << 3) | (eps_p.eps_pdm_ptr->PWR_Channel[1].Error_State_eF_in << 4) | (eps_p.eps_pdm_ptr->PWR_Channel[1].Error_State_eF_out << 5) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[1].Error_PWR_Mon << 6);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[13]                  = (eps_p.eps_pdm_ptr->PWR_Channel[2].State_eF_in) | (eps_p.eps_pdm_ptr->PWR_Channel[2].State_eF_out << 1) | (eps_p.eps_pdm_ptr->PWR_Channel[2].PG_eF_in << 2) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[2].PG_eF_out << 3) | (eps_p.eps_pdm_ptr->PWR_Channel[2].Error_State_eF_in << 4) | (eps_p.eps_pdm_ptr->PWR_Channel[2].Error_State_eF_out << 5) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[2].Error_PWR_Mon << 6);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[14]                  = (eps_p.eps_pdm_ptr->PWR_Channel[3].State_eF_in) | (eps_p.eps_pdm_ptr->PWR_Channel[3].State_eF_out << 1) | (eps_p.eps_pdm_ptr->PWR_Channel[3].PG_eF_in << 2) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[3].PG_eF_out << 3) | (eps_p.eps_pdm_ptr->PWR_Channel[3].Error_State_eF_in << 4) | (eps_p.eps_pdm_ptr->PWR_Channel[3].Error_State_eF_out << 5) |
                                                                          (eps_p.eps_pdm_ptr->PWR_Channel[3].Error_PWR_Mon << 6);
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[15]                  =  0x00;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[16]                  =  0x00;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[17]                  =  0x00;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[18]                  =  eps_p.eps_pdm_ptr->PDM_ID_module;
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
	CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_1                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[0];
	CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_2                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[1];
	CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_3                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[2];
	CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_4                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP4].Temp_value[3];
	CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_1                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[0];
	CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_2                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[1];
	CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_3                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[2];
	CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_4                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP2].Temp_value[3];
	CAN_IVar5_telemetry.CAN_Panal_temp_pY_sensor_1                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value[0];
	CAN_IVar5_telemetry.CAN_Panal_temp_pY_sensor_2                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP5].Temp_value[1];
	CAN_IVar5_telemetry.CAN_Panal_temp_nY_sensor_1                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value[0];
	CAN_IVar5_telemetry.CAN_Panal_temp_nY_sensor_2                      =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP6].Temp_value[1];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_1               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[0];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_2               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[1];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_3               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[2];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_4               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP1].Temp_value[3];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_1               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[0];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_2               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[1];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_3               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[2];
	CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_4               =  eps_p.eps_pam_ptr->Solar_Panel[PAM_SP3].Temp_value[3];

	CAN_IVar5_telemetry.CAN_AB1_controller_1_sensor                     = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_1_Temperature; 			// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_controller_2_sensor                     = (uint8_t) eps_p.eps_pbm_ptr[0].Branch_2_Temperature; 			// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_sensor_on_board1                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_1; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_sensor_on_board2                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_2; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_sensor_on_board3                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_3; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_sensor_on_board4                        = (uint8_t) eps_p.eps_pbm_ptr[0].TMP1075_temp_4; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_controller_1_sensor                     = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_1_Temperature; 			// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_controller_2_sensor                     = (uint8_t) eps_p.eps_pbm_ptr[1].Branch_2_Temperature; 			// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_sensor_on_board1                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_1; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_sensor_on_board2                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_2; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_sensor_on_board3                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_3; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_sensor_on_board4                        = (uint8_t) eps_p.eps_pbm_ptr[1].TMP1075_temp_4; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_controller_1_sensor                     = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_1_Temperature; 			// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_controller_2_sensor                     = (uint8_t) eps_p.eps_pbm_ptr[2].Branch_2_Temperature; 			// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_sensor_on_board1                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_1; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_sensor_on_board2                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_2; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_sensor_on_board3                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_3; 	   			// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_sensor_on_board4                        = (uint8_t) eps_p.eps_pbm_ptr[2].TMP1075_temp_4; 	   			// PBM_data


	CAN_IVar5_telemetry.CAN_AB1_Charge_key_branch_1                     = (uint8_t) CAN_IVar4_RegCmd.CAN_AB1_Charge_key_branch_1;  		// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_Discharge_key_branch_1                  = (uint8_t) CAN_IVar4_RegCmd.CAN_AB1_Discharge_key_branch_1; 	// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_Charge_key_branch_2                     = (uint8_t) CAN_IVar4_RegCmd.CAN_AB1_Charge_key_branch_2;  		// PBM_data
	CAN_IVar5_telemetry.CAN_AB1_Discharge_key_branch_2                  = (uint8_t) CAN_IVar4_RegCmd.CAN_AB1_Discharge_key_branch_2; 	// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_Charge_key_branch_1                     = (uint8_t) CAN_IVar4_RegCmd.CAN_AB2_Charge_key_branch_1;  		// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_Discharge_key_branch_1                  = (uint8_t) CAN_IVar4_RegCmd.CAN_AB2_Discharge_key_branch_1; 	// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_Charge_key_branch_2                     = (uint8_t) CAN_IVar4_RegCmd.CAN_AB2_Charge_key_branch_2;  		// PBM_data
	CAN_IVar5_telemetry.CAN_AB2_Discharge_key_branch_2                  = (uint8_t) CAN_IVar4_RegCmd.CAN_AB2_Discharge_key_branch_2; 	// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_Charge_key_branch_1                     = (uint8_t) CAN_IVar4_RegCmd.CAN_AB3_Charge_key_branch_1;  		// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_Discharge_key_branch_1                  = (uint8_t) CAN_IVar4_RegCmd.CAN_AB3_Discharge_key_branch_1; 	// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_Charge_key_branch_2                     = (uint8_t) CAN_IVar4_RegCmd.CAN_AB3_Charge_key_branch_2;  		// PBM_data
	CAN_IVar5_telemetry.CAN_AB3_Discharge_key_branch_2                  = (uint8_t) CAN_IVar4_RegCmd.CAN_AB3_Discharge_key_branch_2; 	// PBM_data


    CAN_IVar5_telemetry.CAN_Line1                                       =	CAN_IVar4_RegCmd.CAN_PDM_PWR_CH1;
    CAN_IVar5_telemetry.CAN_Line2                                       =	CAN_IVar4_RegCmd.CAN_PDM_PWR_CH2;
    CAN_IVar5_telemetry.CAN_Line3                                       =	CAN_IVar4_RegCmd.CAN_PDM_PWR_CH3;
    CAN_IVar5_telemetry.CAN_Line4                                       =   CAN_IVar4_RegCmd.CAN_PDM_PWR_CH4;
	CAN_IVar5_telemetry.CAN_Line5                                       =   CAN_IVar4_RegCmd.CAN_PDM_PWR_CH5;
    CAN_IVar5_telemetry.CAN_Line6                                       =   CAN_IVar4_RegCmd.CAN_PDM_PWR_CH6;
    CAN_IVar5_telemetry.CAN_Line_VBAT1                                  =   CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT1;
    CAN_IVar5_telemetry.CAN_Line_VBAT2                                  =   CAN_IVar4_RegCmd.CAN_PMM_PWR_VBAT2;

    //***
    if( eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val < 4000 ){  
        CAN_IVar5_telemetry.CAN_SES_internal_bus_voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val;
    }else{
        CAN_IVar5_telemetry.CAN_SES_internal_bus_voltage                =  eps_p.eps_pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val;
    }
    //---

    CAN_IVar5_telemetry.CAN_PMM_sensor1                                 = (uint8_t) eps_p.eps_pmm_ptr->Temp_sensor;
    CAN_IVar5_telemetry.CAN_PAM_sensor1                                 = (uint8_t) (eps_p.eps_pam_ptr->Temp_sensor[0]); // PAM_data
    CAN_IVar5_telemetry.CAN_PAM_sensor2                                 = (uint8_t) (eps_p.eps_pam_ptr->Temp_sensor[1]); // PAM_data
    CAN_IVar5_telemetry.CAN_PAM_sensor3                                 = (uint8_t) (eps_p.eps_pam_ptr->Temp_sensor[2]); // PAM_data
    CAN_IVar5_telemetry.CAN_PAM_sensor4                                 = (uint8_t) (eps_p.eps_pam_ptr->Temp_sensor[3]); // PAM_data

	CAN_IVar5_telemetry.CAN_PDM_sensor1                                 = (uint8_t)(eps_p.eps_pdm_ptr->Temp_sensor[0]);
	CAN_IVar5_telemetry.CAN_PDM_sensor2                                 = (uint8_t)(eps_p.eps_pdm_ptr->Temp_sensor[1]);
	CAN_IVar5_telemetry.CAN_PDM_sensor3                                 = (uint8_t)(eps_p.eps_pdm_ptr->Temp_sensor[2]);
	CAN_IVar5_telemetry.CAN_PDM_sensor4                                 = (uint8_t)(eps_p.eps_pdm_ptr->Temp_sensor[3]);

	    // -------------------  ТМИ 0  ------------------ //
	CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_pX		    =  CAN_IVar5_telemetry.CAN_Panel_median_temperature_pX;
	CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_nX		    =  CAN_IVar5_telemetry.CAN_Panel_median_temperature_nX;
    CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[0]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[0];
	CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[1]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[1];
	CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[2]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[2];
	CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[3]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[3];
	CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[4]				=  CAN_IVar5_telemetry.CAN_Solar_panel_status[4];
	CAN_IVar5_telemetry.CAN_Beacon_full_capacity_charge_level			=  CAN_IVar5_telemetry.CAN_Full_capacity_charge_level;
	CAN_IVar5_telemetry.CAN_Beacon_AB_status[0]							=  CAN_IVar5_telemetry.CAN_AB_status[0];
	CAN_IVar5_telemetry.CAN_Beacon_AB_status[1]						 	=  CAN_IVar5_telemetry.CAN_AB_status[1];
	CAN_IVar5_telemetry.CAN_Beacon_AB_status[2]						 	=  CAN_IVar5_telemetry.CAN_AB_status[2];
	CAN_IVar5_telemetry.CAN_Beacon_charge_discharge_AB_key_status       =  CAN_IVar5_telemetry.CAN_Charge_discharge_AB_key_status;
	CAN_IVar5_telemetry.CAN_Beacon_subsystem_power_line_status		    =  CAN_IVar5_telemetry.CAN_Subsystem_power_line_status;
	CAN_IVar5_telemetry.CAN_Beacon_full_charge_discharge_power		    =  CAN_IVar5_telemetry.CAN_Full_charge_discharge_power;
	CAN_IVar5_telemetry.CAN_Beacon_Total_IN_Power_SP                    =  CAN_IVar5_telemetry.CAN_Total_Generate_Power_SP;
	CAN_IVar5_telemetry.CAN_Beacon_spacecraft_total_power               =  CAN_IVar5_telemetry.CAN_Spacecraft_total_power;
	CAN_IVar5_telemetry.CAN_Beacon_median_PMM_temp					    =  CAN_IVar5_telemetry.CAN_Median_PMM_temp;
	CAN_IVar5_telemetry.CAN_Beacon_median_PAM_temp					    =  CAN_IVar5_telemetry.CAN_Median_PAM_temp;
	CAN_IVar5_telemetry.CAN_Beacon_median_PDM_temp					    =  CAN_IVar5_telemetry.CAN_Median_PDM_temp;
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

    CAN_IVar5_telemetry.CAN_EPS__Mode                                   = eps_p.eps_pmm_ptr->EPS_Mode;

    CAN_IVar5_telemetry.CAN_Version_FW                                  = eps_p.eps_pmm_ptr->Version_FW;

    CAN_IVar5_telemetry.CAN_Channel1_current_average_10s                =  (uint16_t)( Filtr2Step( CHF_I1, (int16_t)CAN_IVar5_telemetry.CAN_Channel1_current ) );
    CAN_IVar5_telemetry.CAN_Channel2_current_average_10s                =  (uint16_t)( Filtr2Step( CHF_I2, (int16_t)CAN_IVar5_telemetry.CAN_Channel2_current ) );
    CAN_IVar5_telemetry.CAN_Channel3_current_average_10s                =  (uint16_t)( Filtr2Step( CHF_I3, (int16_t)CAN_IVar5_telemetry.CAN_Channel3_current ) );
    CAN_IVar5_telemetry.CAN_Channel4_current_average_10s                =  (uint16_t)( Filtr2Step( CHF_I4, (int16_t)CAN_IVar5_telemetry.CAN_Channel4_current ) );
    CAN_IVar5_telemetry.CAN_VBAT1_current_average_10s                   =  (uint16_t)( Filtr2Step( CHF_I5, (int16_t)CAN_IVar5_telemetry.CAN_VBAT1_current ) );
    CAN_IVar5_telemetry.CAN_VBAT2_current_average_10s                   =  (uint16_t)( Filtr2Step( CHF_I6, (int16_t)CAN_IVar5_telemetry.CAN_VBAT2_current ) );

    CAN_IVar5_telemetry.CAN_Channel1_voltage_average_10s                =  (uint16_t)( Filtr2Step( CHF_U1, (int16_t)CAN_IVar5_telemetry.CAN_Channel1_voltage ) );
    CAN_IVar5_telemetry.CAN_Channel2_voltage_average_10s                =  (uint16_t)( Filtr2Step( CHF_U2, (int16_t)CAN_IVar5_telemetry.CAN_Channel2_voltage ) );
    CAN_IVar5_telemetry.CAN_Channel3_voltage_average_10s                =  (uint16_t)( Filtr2Step( CHF_U3, (int16_t)CAN_IVar5_telemetry.CAN_Channel3_voltage ) );
    CAN_IVar5_telemetry.CAN_Channel4_voltage_average_10s                =  (uint16_t)( Filtr2Step( CHF_U4, (int16_t)CAN_IVar5_telemetry.CAN_Channel4_voltage ) );
    CAN_IVar5_telemetry.CAN_VBAT1_voltage_average_10s                   =  (uint16_t)( Filtr2Step( CHF_U5, (int16_t)CAN_IVar5_telemetry.CAN_VBAT1_voltage ) );
    CAN_IVar5_telemetry.CAN_VBAT2_voltage_average_10s                   =  (uint16_t)( Filtr2Step( CHF_U6, (int16_t)CAN_IVar5_telemetry.CAN_VBAT2_voltage ) );

}

void CAN_Var5_fill_telemetry_const(void){

	uint8_t i = 0;
	uint8_t j = 0;

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


    CAN_IVar5_telemetry.CAN_Full_ABs_Energy_level_percent       			=  0x7F;


    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB1_line1             =  0x8081;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB1_line2             =  0x8283;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB2_line1             =  0x8485;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB2_line2             =  0x8687;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line1             =  0x8889;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line2             =  0x8A8B;
    CAN_IVar5_telemetry.CAN_Full_capacity_charge_level			        =  0x8C8D;


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

    CAN_IVar5_telemetry.CAN_Channel1_current                            =  0x9C9D;
    CAN_IVar5_telemetry.CAN_Channel2_current                            =  0x9E9F;
    CAN_IVar5_telemetry.CAN_Channel3_current                            =  0xA0A1;
    CAN_IVar5_telemetry.CAN_Channel4_current                            =  0xA2A3;
    CAN_IVar5_telemetry.CAN_Channel5_current                            =  0xA4A5;
    CAN_IVar5_telemetry.CAN_Channel6_current                            =  0xA6A7;

    CAN_IVar5_telemetry.CAN_VBAT1_voltage                               =  0xA8A9;
    CAN_IVar5_telemetry.CAN_VBAT2_voltage                               =  0xAAAB;

    CAN_IVar5_telemetry.CAN_VBAT1_current                               =  0xACAD;
    CAN_IVar5_telemetry.CAN_VBAT2_current                               =  0xAEAF;

    CAN_IVar5_telemetry.CAN_Subsystem_power_line_status                 =  0xB0;
    CAN_IVar5_telemetry.CAN_SES_current_consumption                     =  0xB1B2;
    CAN_IVar5_telemetry.CAN_SES_Voltage_power_supply                    =  0xB3B4;
    CAN_IVar5_telemetry.CAN_Full_charge_discharge_power                 =  0xB5B6;
    CAN_IVar5_telemetry.CAN_Total_Generate_Power_SP                           =  0xB7B8;
    CAN_IVar5_telemetry.CAN_Spacecraft_total_power                      =  0xB9BA;

    CAN_IVar5_telemetry.CAN_Median_PMM_temp                             =  0xBB;
    CAN_IVar5_telemetry.CAN_Median_PAM_temp                             =  0xBC;
    CAN_IVar5_telemetry.CAN_Median_PDM_temp                             =  0xBD;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[0]        =  0xBE;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1]        =  0xBF;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]        =  0xC0;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[3]        =  0xC1;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[4]        =  0xC2;
    CAN_IVar5_telemetry.CAN_Number_of_restarts_of_the_SES_module        =  0xC3C4C5C6;
    CAN_IVar5_telemetry.CAN_Number_of_restarts_of_the_reserve_SES_module=  0xC7C8C9CA;

    CAN_IVar5_telemetry.CAN_State_PWR_CANmain                           =  0xCB;
    CAN_IVar5_telemetry.CAN_State_PWR_CANbackup                         =  0xCC;
    CAN_IVar5_telemetry.CAN_OFF_PWR_Passive_CPU                         =  0xCD;
    CAN_IVar5_telemetry.CAN_EPS_Const_Mode                              =  0xCE;


    CAN_IVar5_telemetry.CAN_Primary_standby_switch                      =  0xCF;
    // -------------------  ТМИ 7  ------------------ //
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB1_line1          =  0xB8B9;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB1_line2          =  0xBABB;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB2_line1          =  0xBCBD;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB2_line2          =  0xBEBF;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB3_line1          =  0xC0C1;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB3_line2          =  0xC2C3;

    CAN_IVar5_telemetry.CAN_Voltage_AB1_line1_battery_1                 =	0xC4C5;
    CAN_IVar5_telemetry.CAN_Voltage_AB1_line1_battery_2                 =	0xC6C7;
    CAN_IVar5_telemetry.CAN_Voltage_AB1_line2_battery_1                 =	0xC8C9;
    CAN_IVar5_telemetry.CAN_Voltage_AB1_line2_battery_2                 =	0xCACB;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line1_battery_1                 =	0xCCCD;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line1_battery_2                 =	0xCECF;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line2_battery_1                 =	0xD0D1;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line2_battery_2                 =	0xD2D3;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line1_battery_1                 =	0xD4D5;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line1_battery_2                 =	0xD6D7;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line2_battery_1                 =	0xD8D9;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line2_battery_2                 =	0xDADB;

    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB1_line1              =	0xDC;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB1_line2              =	0xDD;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB2_line1              =	0xDE;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB2_line2              =	0xDF;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB3_line1              =	0xE0;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB3_line2              =	0xE1;

    CAN_IVar5_telemetry.CAN_Channel1_voltage                            =	0xE2E3;
    CAN_IVar5_telemetry.CAN_Channel2_voltage                            =	0xE4E5;
    CAN_IVar5_telemetry.CAN_Channel3_voltage                            =	0xE6E7;
    CAN_IVar5_telemetry.CAN_Channel4_voltage                            =	0xE8E9;
    CAN_IVar5_telemetry.CAN_Channel5_voltage                            =	0xEAEB;
    CAN_IVar5_telemetry.CAN_Channel6_voltage                            =	0xECED;

    CAN_IVar5_telemetry.CAN_Opening_elements_state                      =	0x0000;
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
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_1                      =	0x2C;
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_2                      =	0x2D;
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_3                      =	0x2E;
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_4                      =	0x2F;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_1                      =	0x30;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_2                      =	0x31;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_3                      =	0x32;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_4                      =	0x33;
    CAN_IVar5_telemetry.CAN_Panal_temp_pY_sensor_1                      =	0x34;
    CAN_IVar5_telemetry.CAN_Panal_temp_pY_sensor_2                      =	0x35;
    CAN_IVar5_telemetry.CAN_Panal_temp_nY_sensor_1                      =	0x36;
    CAN_IVar5_telemetry.CAN_Panal_temp_nY_sensor_2                      =	0x37;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_1               =  0x38;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_2               =  0x39;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_3               =  0x3A;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_4               =  0x3B;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_1               =  0x3C;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_2               =  0x3D;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_3               =  0x3E;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_4               =  0x3F;

    CAN_IVar5_telemetry.CAN_AB1_controller_1_sensor                     =	0x40;
    CAN_IVar5_telemetry.CAN_AB1_controller_2_sensor                     =	0x41;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board1                        =	0x42;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board2                        =	0x43;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board3                        =	0x44;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board4                        =	0x45;
    CAN_IVar5_telemetry.CAN_AB2_controller_1_sensor                     =	0x46;
    CAN_IVar5_telemetry.CAN_AB2_controller_2_sensor                     =	0x47;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board1                        =	0x48;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board2                        =	0x49;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board3                        =	0x4A;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board4                        =	0x4B;
    CAN_IVar5_telemetry.CAN_AB3_controller_1_sensor                     =	0x4C;
    CAN_IVar5_telemetry.CAN_AB3_controller_2_sensor                     =	0x4D;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board1                        =	0x4E;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board2                        =	0x4F;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board3                        =	0x50;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board4                        =	0x51;


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


    CAN_IVar5_telemetry.CAN_Line1                                       =	0x5E;
    CAN_IVar5_telemetry.CAN_Line2                                       =	0x5F;
    CAN_IVar5_telemetry.CAN_Line3                                       =	0x60;
    CAN_IVar5_telemetry.CAN_Line4                                       =	0x61;
    CAN_IVar5_telemetry.CAN_Line5                                       =	0x62;
    CAN_IVar5_telemetry.CAN_Line6                                       =	0x63;
    CAN_IVar5_telemetry.CAN_Line_VBAT1                                  =	0x64;
    CAN_IVar5_telemetry.CAN_Line_VBAT2                                  =	0x65;

    CAN_IVar5_telemetry.CAN_SES_internal_bus_voltage                    =	0x6667;

    CAN_IVar5_telemetry.CAN_PMM_sensor1                                 =	0x68;
    CAN_IVar5_telemetry.CAN_PAM_sensor1                                 =	0x69;
    CAN_IVar5_telemetry.CAN_PAM_sensor2                                 =	0x6A;
    CAN_IVar5_telemetry.CAN_PAM_sensor3                                 =	0x6B;
    CAN_IVar5_telemetry.CAN_PAM_sensor4                                 =	0x6C;
    CAN_IVar5_telemetry.CAN_PDM_sensor1                                 =	0x6D;
    CAN_IVar5_telemetry.CAN_PDM_sensor2                                 =	0x6E;
    CAN_IVar5_telemetry.CAN_PDM_sensor3                                 =	0x6F;
    CAN_IVar5_telemetry.CAN_PDM_sensor4                                 =	0x70;

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

    CAN_IVar5_telemetry.CAN_EPS__Mode                                   =   0x85;

    CAN_IVar5_telemetry.CAN_Version_FW                                  =   0x8687;

    CAN_IVar5_telemetry.CAN_Channel1_current_average_10s                =   0x8889;
    CAN_IVar5_telemetry.CAN_Channel2_current_average_10s                =   0x8A8B;
    CAN_IVar5_telemetry.CAN_Channel3_current_average_10s                =   0x8C8D;
    CAN_IVar5_telemetry.CAN_Channel4_current_average_10s                =   0x8E8F;
    CAN_IVar5_telemetry.CAN_VBAT1_current_average_10s                   =   0x9091;
    CAN_IVar5_telemetry.CAN_VBAT2_current_average_10s                   =   0x9293;

    CAN_IVar5_telemetry.CAN_Channel1_voltage_average_10s                =   0x9495;
    CAN_IVar5_telemetry.CAN_Channel2_voltage_average_10s                =   0x9697;
    CAN_IVar5_telemetry.CAN_Channel3_voltage_average_10s                =   0x9899;
    CAN_IVar5_telemetry.CAN_Channel4_voltage_average_10s                =   0x9A9B;
    CAN_IVar5_telemetry.CAN_VBAT1_voltage_average_10s                   =   0x9C9D;
    CAN_IVar5_telemetry.CAN_VBAT2_voltage_average_10s                   =   0x9E9F;

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

	CAN_IVar4_RegCmd.CAN_AB1_Charge_key_branch_1                        = eps_p.eps_pbm_ptr[0].Branch_1_ChgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB1_Discharge_key_branch_1                     = eps_p.eps_pbm_ptr[0].Branch_1_DchgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB1_Charge_key_branch_2                        = eps_p.eps_pbm_ptr[0].Branch_2_ChgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB1_Discharge_key_branch_2                     = eps_p.eps_pbm_ptr[0].Branch_2_DchgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB2_Charge_key_branch_1                        = eps_p.eps_pbm_ptr[1].Branch_1_ChgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB2_Discharge_key_branch_1                     = eps_p.eps_pbm_ptr[1].Branch_1_DchgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB2_Charge_key_branch_2                        = eps_p.eps_pbm_ptr[1].Branch_2_ChgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB2_Discharge_key_branch_2                     = eps_p.eps_pbm_ptr[1].Branch_2_DchgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB3_Charge_key_branch_1                        = eps_p.eps_pbm_ptr[2].Branch_1_ChgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB3_Discharge_key_branch_1                     = eps_p.eps_pbm_ptr[2].Branch_1_DchgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB3_Charge_key_branch_2                        = eps_p.eps_pbm_ptr[2].Branch_2_ChgEnableBit;
	CAN_IVar4_RegCmd.CAN_AB3_Discharge_key_branch_2                     = eps_p.eps_pbm_ptr[2].Branch_2_DchgEnableBit;


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
	CAN_IVar4_RegCmd.CAN_Set_active_CPU               					= eps_p.eps_pmm_ptr->Active_CPU;
	CAN_IVar4_RegCmd.CAN_Reset_to_default              					= 0x00;
	CAN_IVar4_RegCmd.CAN_Perform_Deploy                					= 0x00;
	CAN_IVar4_RegCmd.CAN_AB1_Heat_Branch1              					= eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_1;
	CAN_IVar4_RegCmd.CAN_AB1_Heat_Branch2              					= eps_p.eps_pbm_ptr[0].PCA9534_ON_Heat_2;
	CAN_IVar4_RegCmd.CAN_AB2_Heat_Branch1              					= eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_1;
	CAN_IVar4_RegCmd.CAN_AB2_Heat_Branch2              					= eps_p.eps_pbm_ptr[1].PCA9534_ON_Heat_2;
	CAN_IVar4_RegCmd.CAN_AB3_Heat_Branch1              					= eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_1;
	CAN_IVar4_RegCmd.CAN_AB3_Heat_Branch2              					= eps_p.eps_pbm_ptr[2].PCA9534_ON_Heat_2;
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

}
