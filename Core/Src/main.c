
#include "main.h"
#include  "Error_Handler.h"
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
#include "pdm.h"


#include "pmm_config.h"
#include "pmm_struct.h"
#include "pmm_init_IC.h"
#include "pmm_init.h"
#include "pmm_ctrl.h"

#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"





#include  <stdio.h>

/****************************TODO*************************
1. Need to think about delay 30 minuts. 
2. Need change constatn mode EN/Dis after teste with Doroshkin in CAN_cmd.c (delete debug)
3. In PDM module  необходимо добавить переинициализацую входов каждый раз при обращении.


**********************************************************/

extern uint32_t CAN_cmd_mask_status;

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

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
  /* USER CODE END 2 */


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

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	PMM_default_init_I2C_GPIOExt1(PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1); //Temp function

	PMM_Set_state_PWR_CAN( pmm_ptr, CANmain, ENABLE );
	PMM_Set_state_PWR_CAN( pmm_ptr, CANbackup, ENABLE );

	CAN_Init(CAN1);
	CAN_Init(CAN2);
	CAN_RegisterAllVars();
//---------------------------------------------------

	SetupInterrupt();
	//IWDG_Init();

//******************************************************************
	ENABLE_TMUX1209_I2C();   
	PDM_init( pdm_ptr );
	LL_mDelay(100); //Delay for startup power supply
	
/*
	PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_1, ENABLE );
	PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_2, ENABLE );
	PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, ENABLE );
	PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, ENABLE );
	PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_5, ENABLE );
	PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_6, ENABLE );
*/
	//PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_1, DISABLE );
	//PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_2, DISABLE );
	//PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, DISABLE );
	//PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, DISABLE );
	//PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_5, DISABLE );
	//PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_6, DISABLE );
//******************************************************************


//Need test!!!!!!!!!!!!
	//uint8_t pwr_reboot= 6;
//	PMM_Detect_PowerRebootCPU(&pwr_reboot);
//!!!!!!!


#ifdef DEBUGprintf
uint32_t last_cmd_mask_status = 0;
#endif

	while (1){


		#ifdef DEBUGprintf
			if(last_cmd_mask_status != CAN_cmd_mask_status){
				last_cmd_mask_status = CAN_cmd_mask_status;
				printf("cmd_reg status: " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN"\n",
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 24), BYTE_TO_BINARY(CAN_cmd_mask_status >> 16),
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 8), BYTE_TO_BINARY(CAN_cmd_mask_status));
			}
		#endif

		if(CAN_cmd_mask_status != 0){
			CAN_Var4_cmd_parser(&CAN_cmd_mask_status, pdm_ptr);
		}

	}

}


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

