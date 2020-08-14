#include  <stdio.h>
#include "stm32l4xx_ll_utils.h"
#include "SetupPeriph.h"
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
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_deploy.h"
#include "PMM/pmm_savedata.h"
#include "PBM/pbm_control.h"
#include "PBM/pbm_init.h"
#include "PBM/pbm.h"
#include "PAM/pam_init.h"
#include "PAM/pam.h"
#include "uart_eps_comm.h"


/*//TODO
4. Подумать как включать VBAT eF1 и eF2. Возможно написать автомат переключения ?
7. Подумать над тем если CAN при инициализации выдает ошибку стоит ли переходить на резервный МК.

**********************************************************/

//extern uint32_t SysTick_Counter;
//extern uint64_t CAN_cmd_mask_status;
//extern uint8_t CAN1_exchange_timeout_flag;
//extern uint8_t CAN2_exchange_timeout_flag;

_UART_EPS_COMM uart_m_eps_communication = {0}, *UART_M_eps_comm = &uart_m_eps_communication;  // Main EPS UART is LPUART1
_UART_EPS_COMM uart_b_eps_communication = {0}, *UART_B_eps_comm = &uart_b_eps_communication;  // Backup EPS UART is USART3

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

int main(void){

    SysTick_Counter = 0;
    CAN_cmd_mask_status = 0;

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

	SetupInterrupt();

	//IWDG_Init(3000);
    //LL_IWDG_ReloadCounter(IWDG);

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


    //Check Active flag between active and passive CPU.
    PMM_Check_Active_CPU(UART_M_eps_comm, UART_B_eps_comm, eps_param);

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

        PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0

        CAN_init_eps(CAN1);
		CAN_init_eps(CAN2);
		CAN_RegisterAllVars();
		//LL_mDelay(20); //Delay for startup power supply

    //Initialization CAN for passive CPU
	}else{
        PMM_Set_mode_Passive_CPU( eps_param );
        I2C4_DeInit();
        PWM_DeInit_Ch3_Ch4();
		CAN_DeInit_eps(CAN1);
		CAN_DeInit_eps(CAN2);
	}


	while(1){

        //Save setting to FRAM for Active and Passive CPU and sync. settings Active->Passive CPU
        if((pmm_ptr->PMM_save_conf_flag == SET) || (pdm_ptr->PDM_save_conf_flag == SET) || (pam_ptr->PAM_save_conf_flag == SET) || (PBM_CheckSaveSetupFlag(pbm_mas) == SET)){
            PMM_Sync_and_Save_Settings_A_P_CPU(eps_param);
        }

        //ActiveCPU branch
		if( (pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){ //Initialization Active CPU

            PMM_Get_Telemetry(pmm_ptr);
            PDM_Get_Telemetry(pdm_ptr);
            PAM_Get_Telemetry(pam_ptr);
            PBM_Get_Telemetry(pbm_mas);

            //Protection for OFF all power of CAN//TODO move to functin
            if( pmm_ptr->PWR_Ch_State_CANmain == DISABLE && pmm_ptr->PWR_Ch_State_CANbackup == DISABLE ){
            	PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE );
            	PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE );
            }

            if( pmm_ptr->EPS_Mode == EPS_COMBAT_MODE ){

                if( pmm_ptr->Deploy_stage != 9 ){
                    PMM_Deploy( eps_param );
                }

                //TODO сделать проверку уровня заряда батарей ( и дергать пин BAT_LOW) если свзи с PBMs нету. НАпряжение брать с VBAT1 или VBAT2
                //TODO need oFF and on if we reached edge PBM_NORMAL_ENERGY_EDGE  PBM_LOW_ENERGY_EDGE  PBM_ZERO_ENERGY_EDGE (как сохронять если один из БРК выключен ? )

                //Protection for off all BRK //TODO move to functin // Добавить проверку флага что батареи не разряжены
                if( pmm_ptr->Deploy_stage == 9 && ( pdm_ptr->PWR_Channel[PDM_PWR_Channel_3].State_eF_in == DISABLE || pdm_ptr->PWR_Channel[PDM_PWR_Channel_3].State_eF_out == DISABLE )
                    && ( pdm_ptr->PWR_Channel[PDM_PWR_Channel_4].State_eF_in == DISABLE || pdm_ptr->PWR_Channel[PDM_PWR_Channel_4].State_eF_out == DISABLE)  ){
                    PDM_Set_state_PWR_CH( pdm_ptr,  PDM_PWR_Channel_3, ENABLE );
                    PDM_Set_state_PWR_CH( pdm_ptr,  PDM_PWR_Channel_4, ENABLE );
                }
            }else{// EPS_SERVICE_MODE
                //No start Deploy
            }

            //In case when Backup CPU is Active and Main CPU reboot and findout active CPU
            if((pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){
                UART_EPS_Pars_Get_Package(UART_M_eps_comm, eps_param );
                UART_EPS_Pars_Get_Package(UART_B_eps_comm, eps_param );
            }


            //Check Errors UART ports and get reboot counter passive CPU.
            UART_ports_damage_check( UART_M_eps_comm, UART_B_eps_comm, eps_param );

            //Check and parsing command from CAN
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

			UART_EPS_Pars_Get_Package(UART_M_eps_comm, eps_param );
			UART_EPS_Pars_Get_Package(UART_B_eps_comm, eps_param );
		}

	}


}

//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight
//	FRAM_erase(PMM_I2Cx_FRAM1, PMM_I2CADDR_FRAM1, FRAM_SIZE_64KB);
//	FRAM_erase(PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM2, FRAM_SIZE_64KB);
//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// printf("Date: %s  Time: %s \r\n",  __DATE__, __TIME__);

//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PBM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_param );
//		UART_EPS_Send_NFC( UART_EPS_ID_NFS_Prep_Take_CTRL, 0, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr );
//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PDM_struct, 1, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );
//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PMM_struct, 1, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );
//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_Get_Reboot_count, 0, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );

//==========================================//


