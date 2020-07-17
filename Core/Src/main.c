#include "main.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "i2c_comm.h"
#include "tim_pwm.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "FRAM.h"

#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include "CAND/canv.h"

#include "PBM_config.h"
#include "PBM_struct.h"
#include "PBM_init_IC.h"
#include "PBM_init.h"
#include "PBM_control.h"
#include "PBM.h"

#include "pam_struct.h"

#include "pdm_config.h"
#include "pdm_struct.h"
#include "pdm_init.h"
#include "pdm_ctrl.h"
#include "pdm.h"

#include "pmm_config.h"
#include "pmm_struct.h"
#include "pmm_init_IC.h"
#include "pmm_init.h"
#include "pmm_ctrl.h"
#include "pmm_sw_cpu.h"
#include "pmm.h"

#include "eps_struct.h"

#include "uart_comm.h"
#include "uart_eps_comm.h"

#include  <stdio.h>
#include "fram.h"

#include "DS2777.h"

/****************************TODO*************************
 1. Need to think about delay 30 minuts.
 2. Need change constatn mode EN/Dis after teste with Doroshkin in CAN_cmd.c (delete debug)
 3. In PDM module  необходимо добавить переинициализацую входов каждый раз при обращении.
 4. Подумать как включать VBAT eF1 и eF2. Возможно написать автомат переключения ?
 5. Убрать подтяжку CAN в боевой прошивке. Отключать CAN не на основном CPU
 6. В CAN_IVar5_telemetry.CAN_Subsystem_power_line_status и CAN_Spacecraft_total_power добавить два последних бита VBAT1 и VBAT2 когда будет готово.
 7. Подумать над тем если CAN при инициализации выдает ошибку стоит ли переходить на резервный МК.
 8. Сделать ресет I2C. Если выдается ошибка общения с подистемами по I2C.

 **********************************************************/

extern uint64_t CAN_cmd_mask_status;
extern uint8_t CAN1_exchange_timeout_flag;
extern uint8_t CAN2_exchange_timeout_flag;

_UART_EPS_COMM uart_m_eps_communication = { 0 }, *UART_M_eps_comm = &uart_m_eps_communication;  // Main EPS UART is LPUART1
_UART_EPS_COMM uart_b_eps_communication = { 0 }, *UART_B_eps_comm = &uart_b_eps_communication;  // Backup EPS UART is USART3

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

int main(void) {

	UART_M_eps_comm->USARTx = LPUART1;
	UART_B_eps_comm->USARTx = USART3;

	_PDM pdm = { 0 }, *pdm_ptr = &pdm;
	_PMM pmm = { 0 }, *pmm_ptr = &pmm;
	_PAM pam = { 0 }, *pam_ptr = &pam;
	_PBM pbm_mas[PBM_QUANTITY] = { 0 };

	_EPS_Service eps_service = { 0 }, *eps_service_ptr = &eps_service;

	_EPS_Param eps_param = { .eps_pmm_ptr = pmm_ptr, .eps_pdm_ptr = pdm_ptr, .eps_pam_ptr = pam_ptr, .eps_pbm_ptr = pbm_mas, .eps_serv_ptr = eps_service_ptr };

	CAN_cmd_mask_status = 0;

	/** Initialization Periph STM32L496*/
	LL_Init();
	SystemClock_Config();
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	GPIO_Init();
	I2C3_Init();
	UART5_Init();

//Think abot power off CPU
//	PWM_init(100000, 50, 0); //F=100kHz, Duty = 50%, tim devider=0
//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);

//TODO read settings from FRAM.
	pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_MasterBackupCPU();

	if (pmm_ptr->Main_Backup_mode_CPU == CPUmain) {
		UART_M_eps_comm->uart_unit_addr = UART_EPS_CPUm_Addr;
		UART_B_eps_comm->uart_unit_addr = UART_EPS_CPUm_Addr;
	} else { // CPUbackup
		UART_M_eps_comm->uart_unit_addr = UART_EPS_CPUb_Addr;
		UART_B_eps_comm->uart_unit_addr = UART_EPS_CPUb_Addr;
	}

	if (pmm_ptr->Main_Backup_mode_CPU == CPUmain) {
		pmm_ptr->reboot_counter_CPUm++;
	} else { // CPUbackup
		pmm_ptr->reboot_counter_CPUb++;
	}

	pmm_ptr->PMM_save_conf_flag = 1; // Need to save reboot counter value after reboot.

	LPUART1_Init();
	USART3_Init();
	I2C4_Init();

	SetupInterrupt();

	//IWDG_Init();
	//!!!
	pmm_ptr->PWR_Ch_State_PBMs_Logic = ENABLE; // Удалить после добавления команды управления и записиво флеш.
	// !!!!

	PMM_Check_Active_CPU(UART_M_eps_comm, UART_B_eps_comm, eps_param);

	//Initialization Active CPU
	if ((pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain)
			|| (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup)) {
		PMM_Init_ActiveCPUblock(eps_param);
		PDM_init(pdm_ptr);
		//Add init PAM
		//Add init PBM
		LL_mDelay(10); //Delay for startup power supply

  }else{//Initialization passiveCPU
		PMM_Init_PassiveCPUblock( eps_param );

	}

	ENABLE_TMUX1209_I2C();

	PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm_mas, PBM_1, PBM_BRANCH_ALL, PBM_ON_HEAT);
	PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm_mas, PBM_2, PBM_BRANCH_ALL, PBM_ON_HEAT);
	PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm_mas, PBM_3, PBM_BRANCH_ALL, PBM_ON_HEAT);

	PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm_mas, PBM_1, PBM_BRANCH_ALL, PBM_ON_CHARGE);
	PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm_mas, PBM_2, PBM_BRANCH_ALL, PBM_ON_CHARGE);
	PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm_mas, PBM_3, PBM_BRANCH_ALL, PBM_ON_CHARGE);

	PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm_mas, PBM_1, PBM_BRANCH_ALL, PBM_ON_DISCHARGE);
	PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm_mas, PBM_2, PBM_BRANCH_ALL, PBM_ON_DISCHARGE);
	PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm_mas, PBM_3, PBM_BRANCH_ALL, PBM_ON_DISCHARGE);

	PBM_Init(pbm_mas);

	I2C_Bus_SoftwareReset(PBM_I2C_PORT, 5);

	while (1) {

		LL_mDelay(10);
		PBM_GetTelemetry(pbm_mas);
		PBM_Re_Init(pbm_mas);
	}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight unit befor 08.06.2020
	//FRAM_erase(PMM_I2Cx_FRAM1, PMM_I2CADDR_FRAM1, FRAM_SIZE_64KB);
	//FRAM_erase(PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM2, FRAM_SIZE_64KB);

	while (1) {

		//Save setting to FRAM for Active and Passive  CPU 
		if ((pmm_ptr->PMM_save_conf_flag == 1) || (pdm_ptr->PDM_save_conf_flag == 1) || (pam_ptr->PAM_save_conf_flag == 1) || (PBM_CheckSaveSetupFlag(pbm_mas) == 1)) {

			//Add save settings to FRAM.
		}

		//Active CPU branch
		if ((pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain)
				|| (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup)) { //Initialization Active CPU
			PDM_Get_Telemetry(pdm_ptr);
			PMM_Get_Telemetry(pmm_ptr);

	//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PBM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_param );
	//		UART_EPS_Send_NFC( UART_EPS_ID_NFS_Prep_Take_CTRL, 0, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr );
	//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PDM_struct, 1, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );
	//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PMM_struct, 1, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );
	//		UART_EPS_Send_CMD( UART_EPS_ID_CMD_Get_Reboot_count, 0, UART_M_eps_comm, UART_B_eps_comm, pmm_ptr, pdm_ptr );

			CAN_Var5_fill_telemetry(eps_param);

			if (CAN_cmd_mask_status != 0) {
				CAN_Var4_cmd_parser(&CAN_cmd_mask_status, eps_param);
			}

			//Switch active CPU 
			if (eps_service_ptr->Req_SW_Active_CPU == 1) {
				PMM_Switch_Active_CPU(eps_service_ptr->Set_Active_CPU, UART_M_eps_comm, UART_B_eps_comm, eps_param); // Need rewrite this function
			}

			// Passive CPU branch
		} else {

			UART_EPS_Pars_Get_Package(UART_M_eps_comm, eps_param);
			UART_EPS_Pars_Get_Package(UART_B_eps_comm, eps_param);

		}

	}

}

//------------- FRAM test ---------------//

//#define I2C_FRAM1_addr 0x50
//	uint8_t fram_array[128] = {0};
//	uint8_t fram_write_array[128];
//	int8_t error_status = 0;
//
//	for(uint8_t i = 0; i < 128; i++){
//		fram_write_array[i] = 0xFF;
//	}
//
//	for(uint8_t i = 0; i < 128; i++){
//		error_status += FRAM_majority_read_byte(I2C3, I2C_FRAM1_addr, i, fram_array + i);
//	}
//	error_status += FRAM_set_write_access(FRAM_WRITE_PROTECTION_DISABLE);
//	error_status += FRAM_triple_write_data(I2C3, I2C_FRAM1_addr, fram_write_array, 128);
//
//	for(uint8_t i = 0; i < 128; i++){
//		error_status += FRAM_majority_read_byte(I2C3, I2C_FRAM1_addr, i, fram_array + i);
//	}
////	error_status += FRAM_erase(I2C3, I2C_FRAM1_addr, FRAM_SIZE_64KB);
//	error_status += FRAM_is_empty(I2C3, I2C_FRAM1_addr, I2C_FRAM1_addr, FRAM_SIZE_64KB);
//
//	for(uint8_t i = 0; i < 128; i++){
//		error_status += FRAM_majority_read_byte(I2C3, I2C_FRAM1_addr, i, fram_array + i);
//	}
//	error_status += FRAM_set_write_access(FRAM_WRITE_PROTECTION_ENABLE);

//==========================================//

//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);

//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);

//	printf("test  \n");

