
#include "main.h"
#include  "Error_Handler.h"
#include "SetupPeriph.h"
#include "i2c_comm.h"
#include "tim_pwm.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "FRAM.h"
#include  <stdio.h>

/****************************TODO*************************
1.  Need to think about delay 30 minuts. 


**********************************************************/

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release


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
	UART5_Init();
	GPIO_Init();
	PWM_init(100000, 50, 0); //F=100kHz, Duty = 50%, tim devider=0

	//SetupInterrupt();
	//IWDG_Init();


//
//	if( TCA9539_Reset_output_pin(I2C3, 0x74, TCA9539_IO_ALL) != SUCCESS){
//		while(1);
//	}
//
//	if( TCA9539_conf_IO_dir_output( I2C3, 0x74, TCA9539_IO_P10 ) != SUCCESS){
//		while(1);
//	}
//
//
//	if( TCA9539_Set_output_pin( I2C3, 0x74, TCA9539_IO_P10 ) != SUCCESS){
//		while(1);
//	}
//
//	ENABLE_TMUX1209_I2C();
//	SW_TMUX1209_I2C_main_PMM();
//
//	uint8_t read_data;
//	uint16_t read_data_16;
//
//	I2C_Read_byte_St_ReSt(I2C4, 0x48, I2C_SIZE_REG_ADDR_U8 , uint32_t reg_addr, uint8_t *data)
//
//	while(1){
//
//	I2C_Read_word_u16_St_ReSt(I2C4, 0x27, I2C_SIZE_REG_ADDR_U8, 0x00, &read_data_16);

//	PCA9534_conf_IO_dir_output(I2C4, 0x20, PCA9534_IO_ALL);
//	while(1){
//		PCA9534_read_IO_dir_reg (I2C4, 0x38, &read_data) ;
//	}
//
//	}
//	if(PCA9534_read_IO_dir_reg (I2C4, 0x20, &read_data) != SUCCESS){ //PCA9534_read_input_reg
//		while(1);
//	}
//
//	if(PCA9534_read_IO_dir_reg (I2C4, 0x20, &read_data) != SUCCESS ){ //PCA9534_read_input_reg
//		while(1);
//	}



//	TCA9539_read_IO_dir_reg(I2C3, 0x74, &read_data);
//	TCA9539_read_output_reg(I2C3, 0x74, &read_data);
//
//	TCA9539_read_IO_dir_pin(I2C3, 0x74, TCA9539_IO_ALL, &read_data);
//
//	TCA9539_read_output_pin(I2C3, 0x74, TCA9539_IO_ALL, &read_data);
//
//	printf("%d \r\n  ", read_data);




	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);

	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);

#ifdef DEBUGprintf

		printf("test  ");
		//	Error_Handler();

#endif

  while (1){}

}

