#include  <stdio.h>
#include  <string.h>
#include "SetupPeriph.h"
#include "stm32l4xx.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_iwdg.h"
//#include "stm32l4xx_ll_rcc.h"
#include "PMM/eps_struct.h"
#include "CAND/CAN_cmd.h"
#include "CAND/CAN.h"
#include "PDM/pdm_init.h"
#include "PDM/pdm.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_sw_cpu.h"
#include "PMM/pmm.h"
#include "PMM/pmm_deploy.h"
#include "PMM/pmm_savedata.h"
#include "PMM/pmm_damage_ctrl.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PBM_T1/pbm_T1.h"
#include "PAM/pam_init.h"
#include "PAM/pam_ctrl.h"
#include "PAM/pam_sp_ctrl.h"
#include "PAM/pam.h"
#include "uart_eps_comm.h"
/**********************************************************/

//extern uint32_t SysTick_Counter;
//extern uint64_t CAN_cmd_mask_status;
extern uint8_t CAN1_exchange_data_flag;
extern uint8_t CAN2_exchange_data_flag;

_UART_EPS_COMM uart_m_eps_communication = {0}, *UART_M_eps_comm = &uart_m_eps_communication;  // Main EPS UART is LPUART1
_UART_EPS_COMM uart_b_eps_communication = {0}, *UART_B_eps_comm = &uart_b_eps_communication;  // Backup EPS UART is USART3

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

int main(void){

   uint32_t Passive_CPU_start_time_wait_data = 0 ;

    SysTick_Counter = 0;
    CAN_cmd_Buff.length = 0;
    CAN1_exchange_data_flag = 0;
    CAN2_exchange_data_flag = 0;

	UART_M_eps_comm->USARTx = LPUART1;
	UART_B_eps_comm->USARTx = USART3;

	_PDM pdm = {0}, *pdm_ptr = &pdm;
	_PMM pmm = {0}, *pmm_ptr = &pmm;
	_PAM pam = {0}, *pam_ptr = &pam;
	_PBM_T1 pbm_mas[PBM_T1_QUANTITY] = {0};
    _EPS_Service eps_service = {0}, *eps_service_ptr = &eps_service;

	_EPS_Param eps_param = {.eps_pmm_ptr = pmm_ptr, 
							.eps_pdm_ptr = pdm_ptr,
							.eps_pam_ptr = pam_ptr,
							.eps_pbm_ptr = pbm_mas,
							.eps_serv_ptr = eps_service_ptr
						    };

    pmm_ptr->Version_FW = ( ((uint16_t)VERSION_FW_MAJOR) << 8 ) |( (uint16_t)VERSION_FW_MINOR ); //Firmware version

	/** Initialization Periph. STM32L496*/
	LL_Init();
	SystemClock_Config(CPU_Clock_80MHz);
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	GPIO_Init();
	I2C3_Init();
	I2C4_Init();

    //PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0 -  moved to PMM_init

    //UART5_Init();
    LPUART1_Init();
    USART3_Init();
    Setup_UART_Interrupt();
    LL_SYSTICK_EnableIT();

   // IWDG_Init(4000);
    LL_IWDG_ReloadCounter(IWDG);

    //Restore settings of EPS
    pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_Main_Backup_CPU();

    if( pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
        UART_M_eps_comm->uart_unit_addr = UART_EPS_CPUm_Addr;
        UART_B_eps_comm->uart_unit_addr = UART_EPS_CPUm_Addr;
    }else{ // CPUbackup
        UART_M_eps_comm->uart_unit_addr = UART_EPS_CPUb_Addr;
        UART_B_eps_comm->uart_unit_addr = UART_EPS_CPUb_Addr;
    }

    //Restore settings EPS from FRAM
    PMM_FRAM_Restore_Settings(eps_param);

    //Get settings from the neighbor CPU if detect errors FRAM1 and FRAM2.
    if( eps_param.eps_pmm_ptr->Error_FRAM1 == ERROR && eps_param.eps_pmm_ptr->Error_FRAM2 == ERROR ){
        PMM_Get_Settings_From_NeighborCPU( eps_param );
    }

    if( pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
        pmm_ptr->reboot_counter_CPUm++;
		#ifdef DEBUGprintf
        	printf("Main CPU started\n");
        #endif
    }else{ // CPUbackup
        pmm_ptr->reboot_counter_CPUb++;
		#ifdef DEBUGprintf
        	printf("Backup CPU started\n");
        #endif
    }

    pmm_ptr->PMM_save_conf_flag = 1; // Need to save reboot counter value after reboot.
    pam_ptr->PAM_save_conf_flag = 1; //To properly startup the power supplies
    pdm_ptr->PDM_save_conf_flag = 1; //To properly startup the power supplies

    LL_IWDG_ReloadCounter(IWDG);

    //Check Active flag between active and passive CPU.
    PMM_CPUm_Check_Active_CPU(UART_M_eps_comm, UART_B_eps_comm, eps_param);

    //Turn off to avoid overheating of the resistor on reboot
    pmm_ptr->PWR_Ch_State_Deploy_Logic = DISABLE;
    pmm_ptr->PWR_Ch_State_Deploy_Power = DISABLE;

    //Initialization PMM (active and passive CPU)
    PMM_init( pmm_ptr );


    //Fill VarID4
    CAN_Var4_fill(eps_param);
    if( pmm_ptr->CAN_constatnt_mode == ENABLE){
        CAN_Var5_fill_telemetry_const();
    }

	//Initialization EPS and CAN for active CPU
	if( (pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){ 

	    if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE && pmm_ptr->Deploy_stage == 0 ){
	        PDM_PWR_Down_init( pdm_ptr);
	    }else{
	        PDM_init( pdm_ptr );
	    }
	    PAM_init( pam_ptr );

	    if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){
	        PBM_T1_Init( pbm_mas );
	    }

        if( pmm_ptr->PWR_Ch_State_CANmain == ENABLE ){
        	CAN_init_eps(CAN1);
        }
        if( pmm_ptr->PWR_Ch_State_CANbackup == ENABLE ){
        	CAN_init_eps(CAN2);
        }

        if( pmm_ptr->PWR_Ch_State_CANmain == ENABLE || pmm_ptr->PWR_Ch_State_CANbackup == ENABLE  ){
        	CAN_RegisterAllVars();
        }

        if(pmm_ptr->PWR_OFF_Passive_CPU == ENABLE){
        	LPUART1_DeInit();
        	USART3_DeInit();
        }

        if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE && pmm_ptr->Deploy_stage == 0 && pmm_ptr->PWR_Ch_State_CANmain == DISABLE && pmm_ptr->PWR_OFF_Passive_CPU == ENABLE ){
            PMM_CPU_SPEED_MODE( pmm_ptr, CPU_Clock_16MHz );
        }

        PMM_Start_Time_Check_CAN = SysTick_Counter;

    //Initialization CAN for passive CPU
	}else{
        PMM_Set_mode_Passive_CPU( eps_param );
        I2C4_DeInit();
        PWM_DeInit_Ch3_Ch4();
		CAN_DeInit_eps(CAN1);
		CAN_DeInit_eps(CAN2);
        PMM_Start_Time_Check_UART_PassiveCPU = SysTick_Counter;
	}

	//Infinity Loop
	while(1){

        LL_IWDG_ReloadCounter(IWDG);
        //Save setting to FRAM for Active and Passive CPU and sync. settings Active->Passive CPU
        if((pmm_ptr->PMM_save_conf_flag == SET) || (pdm_ptr->PDM_save_conf_flag == SET) || (pam_ptr->PAM_save_conf_flag == SET) || (PBM_T1_CheckSaveSetupFlag(pbm_mas) == SET)){
            PMM_Sync_and_Save_Settings_A_P_CPU(eps_param);
        }

        //ReInit EPS
        PMM_ReInit_EPS( eps_param );

        LL_IWDG_ReloadCounter(IWDG);

        //ActiveCPU branch
        if( (pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){ //Initialization Active CPU

            if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE && pmm_ptr->Deploy_stage == 0 ){
//                if(pmm_ptr->PWR_Ch_State_PBMs_Logic == DISABLE){
//                    PBM_T1_EraseData(pbm_mas);
//                }
//                if( pam_ptr->State_DC_DC == DISABLE && pam_ptr->State_LDO == DISABLE ){
//                    PAM_EraseData(pam_ptr);
//                    PAM_SP_EraseData(pam_ptr);
//                }

            }else{

                PMM_Get_Telemetry(pmm_ptr);
                PDM_Get_Telemetry(pdm_ptr);
                PAM_Get_Telemetry(pam_ptr);
                LL_IWDG_ReloadCounter(IWDG);
                if(pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE){
                    PBM_T1_Get_Telemetry(pbm_mas);
                }else{
                    PBM_T1_EraseData(pbm_mas);
                }
            }

            //In case when Backup CPU is Active and Main CPU reboot and findout who is an active CPU
            //In a case when no Activ CPU read settings if got errors FRAM1 and FRAM2
            //if((pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){
            if( UART_M_eps_comm->permit_recv_pack_flag == 1 || UART_M_eps_comm->stop_recv_pack_flag == 1 ||
                    UART_B_eps_comm->permit_recv_pack_flag == 1 || UART_B_eps_comm->stop_recv_pack_flag == 1 ){
                UART_EPS_Check_TimeOut_Receive( UART_M_eps_comm );
                UART_EPS_Check_TimeOut_Receive( UART_B_eps_comm );
                UART_EPS_Pars_Get_Package( UART_M_eps_comm, eps_param );
                UART_EPS_Pars_Get_Package( UART_B_eps_comm, eps_param );
            }
            //}

            //EPS_COMBAT_MODE
            if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE ){

                LL_IWDG_ReloadCounter(IWDG);
                if( pmm_ptr->Deploy_stage != 9 ){
                   PMM_Deploy( eps_param );
                }

                //CAN ports power off protection. ( In combat mode start protection only out from the container )
                if( pmm_ptr->Deploy_stage >= 1 ){
                	PMM_Portecion_PWR_OFF_CAN_m_b(eps_param);
                }

                if( pmm_ptr->Deploy_stage > 6){
                    //Check CAN ports
                    PMM_Damage_Check_CAN_m_b(eps_param);

                    //Disable PWR SubSystem if reach Zero energy level
                    PMM_ZERO_Energy_PWR_OFF_SubSystem( eps_param );
                    //Protection for off all BRC
                    PMM_Protecion_PWR_OFF_BRC_m_b( eps_param );
                }

            // EPS_SERVICE_MODE
            }else{
                PMM_Start_Time_Check_CAN = SysTick_Counter;
                //All CAN ports power off protection.
                PMM_Protecion_PWR_OFF_CANmain(eps_param);
            }

            LL_IWDG_ReloadCounter(IWDG);
            //Check Errors UART ports and get reboot counter passive CPU.
            PMM_Damage_Check_UART_m_b_ActiveCPU(UART_M_eps_comm, UART_B_eps_comm, eps_param);

            //Parsing command from CAN
            if(CAN_cmd_Buff.length != 0){
                CAN_Var4_cmd_parser( eps_param );
            }

            //Fill CAN Var5
            LL_IWDG_ReloadCounter(IWDG);
            if( pmm_ptr->CAN_constatnt_mode == DISABLE ){ //Constant mode OFF
                CAN_Var5_fill_telemetry(eps_param);
                memcpy(&CAN_IVar5_ready_telemetry, &CAN_IVar5_telemetry, sizeof(CAN_IVar5_telemetry)); //Execution time at 80MHz is 61.5us.
            }

			//Switch active CPU 
			if( eps_service_ptr->Req_SW_Active_CPU == 1 ){

                if( pmm_ptr->PWR_OFF_Passive_CPU == DISABLE ){
                    PMM_Switch_Active_CPU(eps_service_ptr->Set_Active_CPU, UART_M_eps_comm, UART_B_eps_comm, eps_param); // Need rewrite this function
                }
                CAN_Var4_fill(eps_param);
				eps_service_ptr->Req_SW_Active_CPU = 0;
			}

		// Passive CPU branch
		}else{

            Passive_CPU_start_time_wait_data = SysTick_Counter;
		    while( ( (uint32_t)(SysTick_Counter - Passive_CPU_start_time_wait_data ) ) < ( (uint32_t)250) ){ //wait data from active CPU 250ms
                UART_EPS_Check_TimeOut_Receive( UART_M_eps_comm );
                UART_EPS_Check_TimeOut_Receive( UART_B_eps_comm );
                UART_EPS_Pars_Get_Package(UART_M_eps_comm, eps_param);
                UART_EPS_Pars_Get_Package(UART_B_eps_comm, eps_param);
            }

            //EPS_COMBAT_MODE
            if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE ){

                PMM_Damage_Check_UART_m_b_PassiveCPU( UART_M_eps_comm, UART_B_eps_comm, eps_param );

                // Take control Only for BackupCPU.
                if( (pmm_ptr->Error_UART_port_M == ERROR) && (pmm_ptr->Error_UART_port_B == ERROR) && ( pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){
                    LL_IWDG_ReloadCounter(IWDG);
                    PMM_Take_Control_EPS_PassiveCPU( eps_param );
                }

            }else{
                PMM_Start_Time_Check_UART_PassiveCPU = SysTick_Counter;

            }

		}
	}
}

//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight
//	FRAM_erase(PMM_I2Cx_FRAM1, PMM_I2CADDR_FRAM1, FRAM_SIZE_64KB);
//	FRAM_erase(PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM2, FRAM_SIZE_64KB);
//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight

//!!!!!!!!!!!!!!!!!!For first start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  pmm_ptr->PWR_Ch_State_CANmain = ENABLE;
 // pmm_ptr->PWR_Ch_State_CANbackup = ENABLE;
//   pmm_ptr->EPS_Mode = 0xFF;
//   pmm_ptr->Active_CPU = 0x00;
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


/*
pdm_ptr->PWR_Channel[PDM_PWR_Channel_1].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_2].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_3].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_4].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_5].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_6].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_7].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_8].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_9].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_10].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_11].State_eF  = DISABLE;//ENABLE;
pdm_ptr->PWR_Channel[PDM_PWR_Channel_12].State_eF  = DISABLE;//ENABLE;
*/
// pam_ptr->State_DC_DC= ENABLE;
//printf("Date: %s  Time: %s \r\n",  __DATE__, __TIME__);


