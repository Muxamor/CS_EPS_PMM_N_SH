
#include "main.h"
#include "SetupPeriph.h"
#include "tim_pwm.h"
#include "TMP1075.h"
#include "FRAM.h"
#include  <stdio.h>

/****************************TODO*************************
1.  Need to think about delay 30 minuts. 


**********************************************************/

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

/*

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void vprint(const char *fmt, va_list argp)
{
    char string[200];
    int i=0;
    uint32_t len=0;

    len = strlen(string);

   // if(0 < vsprintf(string,fmt,argp)) // build string
    //{
    //	HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    //}
    if(0 < vsprintf(string,fmt,argp)){
    	for(i=0;i<len;i++){
    		ITM_SendChar(string[i]);
    	}
	}
}

void my_printf(const char *fmt, ...) // custom printf() function
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}


	uint8_t str[80];
	sprintf (str, "%s %d %c", "one", 2, '3');
*/




int main(void){

	/** Initialization Periph STM32L496*/
	LL_Init();
	SystemClock_Config();
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	LPUART1_Init();
	UART5_Init();
	GPIO_Init();
	PWM_init(100000, 50, 0); //F=100kHz, Duty = 50%, tim devider=0

	//SetupInterrupt();
	//IWDG_Init();




	//printf("%f,%d \r\n  ", temp_binary, Uint_temp_binary);




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

