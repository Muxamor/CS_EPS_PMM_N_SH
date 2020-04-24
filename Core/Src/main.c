
#include "main.h"
#include  "Error_Handler.h"
#include "SetupPeriph.h"
#include "i2c_comm.h"
#include "tim_pwm.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "FRAM.h"
#include "CAN.h"
#include  <stdio.h>

/****************************TODO*************************
1.  Need to think about delay 30 minuts. 


**********************************************************/

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

#define TCA9539_I2C_ADDR					0b01110100
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

	//while(1);

	/** Initialization Periph STM32L496*/
	LL_Init();
	SystemClock_Config();
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	I2C3_Init();
	I2C4_Init();
	LPUART1_Init();
	USART3_Init();
//	UART5_Init();
	GPIO_Init();
	PWM_init(100000, 50, 0); //F=100kHz, Duty = 50%, tim devider=0


	int8_t status = 0;
	status += TCA9539_conf_IO_dir_input(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);
	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR,  TCA9539_IO_P17 | TCA9539_IO_P15); // Turn on only CANbus

	status += CAN_Init(CAN1);
	status += CAN_Init(CAN2);
	CAN_RegisterAllVars();

	SetupInterrupt();
	//IWDG_Init();


//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);

//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);

#ifdef DEBUGprintf

		printf("test  \n");
		//	Error_Handler();

#endif
uint32_t last_cmd_mask_status = 0;
CAN_cmd_mask_status = 0;
	while (1){
		#ifdef DEBUGprintf
			if(last_cmd_mask_status != CAN_cmd_mask_status){
				last_cmd_mask_status = CAN_cmd_mask_status;
				printf("cmd_reg status: " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN"\n",
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 24), BYTE_TO_BINARY(CAN_cmd_mask_status >> 16),
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 8), BYTE_TO_BINARY(CAN_cmd_mask_status));
			}
		#endif

		CAN_check_cmd_status(&CAN_cmd_mask_status);

		LL_mDelay(1000);

	}

}

