#include  <stdio.h>
#include "stm32l4xx_ll_utils.h"
#include "SetupPeriph.h"
#include "stm32l4xx_ll_iwdg.h"
#include "PMM/eps_struct.h"
#include "CAND/CAN_cmd.h"
#include "CAND/CAN.h"
#include "PDM/pdm_init.h"
#include "PDM/pdm.h"
#include "PDM/pdm_ctrl.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_sw_cpu.h"
#include "PMM/pmm.h"
#include "PMM/pmm_deploy.h"
#include "PMM/pmm_savedata.h"
#include "PMM/pmm_damage_ctrl.h"
#include "PBM/pbm_control.h"
#include "PBM/pbm_init.h"
#include "PBM/pbm.h"
#include "PAM/pam_init.h"
#include "PAM/pam.h"
#include "uart_eps_comm.h"


/*//TODO
4. Подумать как включать VBAT eF1 и eF2. Возможно написать автомат переключения ?

**********************************************************/

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
    CAN_cmd_mask_status = 0;
    CAN1_exchange_data_flag = 0;
    CAN2_exchange_data_flag = 0;

	UART_M_eps_comm->USARTx = LPUART1;
	UART_B_eps_comm->USARTx = USART3;

	_PDM pdm = {0}, *pdm_ptr = &pdm;
	_PMM pmm = {0}, *pmm_ptr = &pmm;
	_PAM pam = {0}, *pam_ptr = &pam;
	_PBM pbm_mas[PBM_QUANTITY] = {0};
    _EPS_Service eps_service = {0}, *eps_service_ptr = &eps_service;

	_EPS_Param eps_param = {.eps_pmm_ptr = pmm_ptr, 
							.eps_pdm_ptr = pdm_ptr,
							.eps_pam_ptr = pam_ptr,
							.eps_pbm_ptr = pbm_mas,
							.eps_serv_ptr = eps_service_ptr
						    };

    pmm_ptr->Version_FW =  ( ((uint16_t)VERSION_FW_MAJOR) << 8 ) |( (uint16_t)VERSION_FW_MINOR ); //Firmware version

	/** Initialization Periph. STM32L496*/
	LL_Init();
	SystemClock_Config();
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	GPIO_Init();
	I2C3_Init();
	//UART5_Init();

   	LPUART1_Init();
	USART3_Init();
	I2C4_Init();

    PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0

	SetupInterrupt();

	//IWDG_Init(4000);
    LL_IWDG_ReloadCounter(IWDG);

    pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_MasterBackupCPU();

    if( pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
        UART_M_eps_comm->uart_unit_addr = UART_EPS_CPUm_Addr;
        UART_B_eps_comm->uart_unit_addr = UART_EPS_CPUm_Addr;
    }else{ // CPUbackup
        UART_M_eps_comm->uart_unit_addr = UART_EPS_CPUb_Addr;
        UART_B_eps_comm->uart_unit_addr = UART_EPS_CPUb_Addr;
    }

    //Restore settings EPS from FRAM
    PMM_FRAM_Restore_Settings(eps_param);

    if( pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
        pmm_ptr->reboot_counter_CPUm++;
    }else{ // CPUbackup
        pmm_ptr->reboot_counter_CPUb++;
    }

    pmm_ptr->PMM_save_conf_flag = 1; // Need to save reboot counter value after reboot.

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
    if(pmm_ptr->CAN_constatnt_mode == ENABLE ){
        CAN_Var5_fill_telemetry_const();
    }

	//Initialization EPS and CAN for active CPU
	if( (pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){ 
		PDM_init( pdm_ptr );
		PBM_Init( pbm_mas );
        PAM_init( pam_ptr );

        if( pmm_ptr->CAN_constatnt_mode == ENABLE){
            CAN_Var5_fill_telemetry_const();
        }


        CAN_init_eps(CAN1);
		CAN_init_eps(CAN2);
		CAN_RegisterAllVars();
        PMM_Start_Time_Check_CAN = SysTick_Counter;


    //Initialization CAN for passive CPU
	}else{
        PMM_Set_mode_Passive_CPU( eps_param );
        I2C4_DeInit();
        PWM_DeInit_Ch3_Ch4();
		CAN_DeInit_eps(CAN1);
		CAN_DeInit_eps(CAN2);
	}



	//Infinity Loop
	while(1){

        LL_IWDG_ReloadCounter(IWDG);
        //Save setting to FRAM for Active and Passive CPU and sync. settings Active->Passive CPU
        if((pmm_ptr->PMM_save_conf_flag == SET) || (pdm_ptr->PDM_save_conf_flag == SET) || (pam_ptr->PAM_save_conf_flag == SET) || (PBM_CheckSaveSetupFlag(pbm_mas) == SET)){
            PMM_Sync_and_Save_Settings_A_P_CPU(eps_param);
        }

        //ReInit EPS
        PMM_ReInit_EPS( eps_param );

        LL_IWDG_ReloadCounter(IWDG);

        //ActiveCPU branch
		if( (pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){ //Initialization Active CPU

            PMM_Get_Telemetry(pmm_ptr);
            PDM_Get_Telemetry(pdm_ptr);
            PAM_Get_Telemetry(pam_ptr);
            PBM_Get_Telemetry(pbm_mas);

            //EPS_COMBAT_MODE
            if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE ){

                LL_IWDG_ReloadCounter(IWDG);
                if( pmm_ptr->Deploy_stage != 9 ){
                    PMM_Deploy( eps_param );
                }

                //CAN ports power off protection. ( In combat mode start protection only out from the container )
                if( pmm_ptr->Deploy_stage >=1 ){
                	PMM_Portecion_PWR_OFF_CAN_m_b(eps_param);
                }


                if( pmm_ptr->Deploy_stage > 6){
                    //Disable PWR SubSystem if reach Zero energy level
                    PMM_ZERO_Energy_PWR_OFF_SubSystem( eps_param );
                    //Protection for off all BRC
                    PMM_Portecion_PWR_OFF_BRC_m_b( eps_param );
                }

            // EPS_SERVICE_MODE
            }else{
                //All CAN ports power off protection.
                PMM_Portecion_PWR_OFF_CAN_m_b(eps_param);
            }

            //In case when Backup CPU is Active and Main CPU reboot and findout active CPU
            if((pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){
                UART_EPS_Pars_Get_Package(UART_M_eps_comm, eps_param );
                UART_EPS_Pars_Get_Package(UART_B_eps_comm, eps_param );
            }

            //Check CAN ports
            PMM_Damage_Check_CAN_m_b(eps_param);

            //Check Errors UART ports and get reboot counter passive CPU.
            PMM_Damage_Check_UART_m_b(UART_M_eps_comm, UART_B_eps_comm, eps_param);

            //Parsing command from CAN
            if(CAN_cmd_mask_status != 0){
                CAN_Var4_cmd_parser(&CAN_cmd_mask_status, eps_param );
            }

            //Fill CAN Var5
            if( pmm_ptr->CAN_constatnt_mode == 0 ){ //Constant mode OFF
                CAN_Var5_fill_telemetry(eps_param);
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
		    while(( (uint32_t)(SysTick_Counter - Passive_CPU_start_time_wait_data ) ) < ( (uint32_t)250) ){ //wait data from active CPU 250ms
                UART_EPS_Pars_Get_Package(UART_M_eps_comm, eps_param);
                UART_EPS_Pars_Get_Package(UART_B_eps_comm, eps_param);
            }

            //EPS_COMBAT_MODE
            if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE ){

            }else{

            }
		}
	}
}

//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight
//	FRAM_erase(PMM_I2Cx_FRAM1, PMM_I2CADDR_FRAM1, FRAM_SIZE_64KB);
//	FRAM_erase(PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM2, FRAM_SIZE_64KB);
//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight

//printf("Date: %s  Time: %s \r\n",  __DATE__, __TIME__);

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("Date: %s  Time: %s \r\n",  __DATE__, __TIME__);

//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PBM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_param );
//		UART_EPS_Send_NFC( UART_EPS_ID_NFS_Prep_Take_CTRL, 0, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr );
//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PDM_struct, 1, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );
//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PMM_struct, 1, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );
//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_Get_Reboot_count, 0, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );

//==========================================//


