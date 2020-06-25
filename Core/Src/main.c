#include "main.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "i2c_comm.h"
#include "tim_pwm.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "FRAM.h"

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
#include "pmm.h"

#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include "CAND/canv.h"

#include "uart_comm.h"

#include  <stdio.h>
#include "fram.h"
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

uint8_t UART_CHANGE_ACTIVE_CPU_FLAG=0;

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

int main(void){



	_PDM pdm = {0}, *pdm_ptr = &pdm;
	_PMM pmm = {0}, *pmm_ptr = &pmm;

	CAN_cmd_mask_status = 0;

	/** Initialization Periph STM32L496*/
	LL_Init();
	SystemClock_Config();
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	I2C3_Init();
	I2C4_Init();
	LPUART1_Init();
	USART3_Init();
	UART5_Init();
	GPIO_Init();

	PWM_init(100000, 50, 0); //F=100kHz, Duty = 50%, tim devider=0
	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_mDelay(40);

	SetupInterrupt();
	//IWDG_Init();

	CAN_init_eps(CAN1);
	CAN_init_eps(CAN2);
	CAN_RegisterAllVars();


	//Need test!!!!!!!!!!!!
	//uint8_t pwr_reboot= 6;
	//PMM_Detect_PowerRebootCPU(&pwr_reboot);
	//!!!!!!!


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!Need erase FRAM at flight unit befor 08.06.2020
	//FRAM_erase(PMM_I2Cx_FRAM1, PMM_I2CADDR_FRAM1, FRAM_SIZE_64KB);
	//FRAM_erase(PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM2, FRAM_SIZE_64KB);

	pmm_ptr->Main_Backup_mode_CPU =  PMM_Detect_MasterBackupCPU();

	if( pmm_ptr->Main_Backup_mode_CPU == 0){

		//pmm_ptr->Detect_Active_CPU = 0;

		ENABLE_TMUX1209_I2C();

		PDM_init( pdm_ptr );
		PMM_init( pmm_ptr );

		LL_mDelay(50); //Delay for startup power supply

		while(1){
			PMM_Get_PWR_Supply_m_b_I( pmm_ptr, PMM_I2Cx_PowerADC, PMM_I2CADDR_PowerADC);
		}
		PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE );
		PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE );

		PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, ENABLE );
		PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, ENABLE );

		while (1){

			PDM_Get_Telemetry( pdm_ptr );
			PMM_Get_Telemetry( pmm_ptr );
			CAN_Var5_fill_telemetry( pdm_ptr, pmm_ptr );

			if(CAN_cmd_mask_status != 0){

				CAN_Var4_cmd_parser(&CAN_cmd_mask_status, pdm_ptr, pmm_ptr);
			}
		}




	}else{ // Backup Mode CPU Main_Backup_mode_CPU = 0;

		uint8_t mas_string[] = "Set active CPUbackup\r\n";

		LL_mDelay(40);

		while(1){

			if( UART_CHANGE_ACTIVE_CPU_FLAG == 1 ){
				PMM_Set_MUX_CAN_CPUm_CPUb( CPUbackup );

				USART_send_string( UART5, mas_string);

				ENABLE_TMUX1209_I2C();

				UART_CHANGE_ACTIVE_CPU_FLAG = 0;
			}


			if(CAN_cmd_mask_status != 0){
				CAN_Var4_cmd_parser(&CAN_cmd_mask_status, pdm_ptr, pmm_ptr);
			}


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



/*

//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);

//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);

		printf("test  \n");
#ifdef DEBUGprintf

		//	Error_Handler();

#endif
*/

/*
 *
 *
//#define TCA9539_I2C_ADDR					0b01110100
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')


 *
 *
 *
 *
 * 		#ifdef DEBUGprintf
		uint32_t last_cmd_mask_status = 0;
		#endif
 *
 *

			#ifdef DEBUGprintf
				if(last_cmd_mask_status != CAN_cmd_mask_status){
				last_cmd_mask_status = CAN_cmd_mask_status;
				printf("cmd_reg status: " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN"\n",
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 24), BYTE_TO_BINARY(CAN_cmd_mask_status >> 16),
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 8), BYTE_TO_BINARY(CAN_cmd_mask_status));
			}
		#endif*/

