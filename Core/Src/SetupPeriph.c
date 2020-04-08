
#include "stm32l4xx.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_iwdg.h"
#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_usart.h"
#include "SetupPeriph.h"

#include  <stdio.h>

//#include "stm32l4xx_ll_dma.h"

/** @brief Initialization of basic functionality
  * @retval None
*/
void LL_Init(void){

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/
	/* MemoryManagement_IRQn interrupt configuration */
	NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* BusFault_IRQn interrupt configuration */
	NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* UsageFault_IRQn interrupt configuration */
	NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* SVCall_IRQn interrupt configuration */
	NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* DebugMonitor_IRQn interrupt configuration */
	NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* PendSV_IRQn interrupt configuration */
	NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	/* SysTick_IRQn interrupt configuration */
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),15, 0));

	/* PVD Configuration */
	LL_PWR_SetPVDLevel(LL_PWR_PVDLEVEL_1);
	/* Enable the PVD */
	LL_PWR_EnablePVD();
}


/** @brief System Clock Configuration
  * @retval None
 */
void SystemClock_Config(void){

	uint8_t HSE_Fault=0;
	uint32_t down_counter = 500000;


	LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

	if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4){
		Error_Handler();
	}

	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

	/* Enabele HSE */
	LL_RCC_HSE_EnableBypass();
	LL_RCC_HSE_Enable();
	/* Wait till HSE is ready */
	while( (LL_RCC_HSE_IsReady() != 1) && (down_counter != 0)){
		down_counter--;
	}


	if(down_counter==0){

		HSE_Fault=1;
		Error_Handler();

		/* Enabele HSI - backup clock*/
		LL_RCC_HSI_Enable();
		/* Wait till HSI is ready */
		while(LL_RCC_HSI_IsReady() != 1){}
		//LL_RCC_HSI_SetCalibTrimming(64); // Not need.
	}

	/* Enable LSI */ //????????
	LL_RCC_LSI_Enable();
	/* Wait till LSI is ready */
	while(LL_RCC_LSI_IsReady() != 1){} ///?????????????

	if(HSE_Fault==0){
		/* Enable CSS */
		LL_RCC_HSE_EnableCSS();
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2);
	}else{
		LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2);
	}

	LL_RCC_PLL_EnableDomain_SYS();
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1){}

	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL){}


	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_Init1msTick(80000000);

	LL_SetSystemCoreClock(80000000);

	/* Setup RTC clock */
	//LL_PWR_EnableBkUpAccess();
	//LL_RCC_ForceBackupDomainReset();
	//LL_RCC_ReleaseBackupDomainReset();
	//LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
	//LL_RCC_EnableRTC();

	LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1); // main LPUART1 for comm. CPUm <-> CPUb
	LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);   // backup USART3 for comm. CPUm <-> CPUb
	//LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1); // Comm. with module
	//LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2); // Comm. with module
	LL_RCC_SetUARTClockSource(LL_RCC_UART5_CLKSOURCE_PCLK1); 	 // Terminal or debug UART5 
	//LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1); 	 // System I2C1 for comm. with another subsystem CubeSat
	LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_PCLK1);       // I2C3 comm. inside main and backup block
	//LL_RCC_SetI2CClockSource(LL_RCC_I2C2_CLKSOURCE_PCLK1);     // Backup I2C1 Comm. with module PAM and PDM and another part of PMM module
	LL_RCC_SetI2CClockSource(LL_RCC_I2C4_CLKSOURCE_PCLK1);       // Main I2C4 Comm. with module PAM and PDM and another part of PMM module

}

/** @brief LPUART1 Initialization Function
  * @param None
  * @retval None
 */
void LPUART1_Init(void){

	LL_LPUART_InitTypeDef LPUART_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
	LL_PWR_EnableVddIO2();
	/**LPUART1 GPIO Configuration
  	  PC0   ------> LPUART1_RX
  	  PG7   ------> LPUART1_TX */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	LPUART_InitStruct.BaudRate = 115200;
	LPUART_InitStruct.DataWidth = LL_LPUART_DATAWIDTH_8B;
	LPUART_InitStruct.StopBits = LL_LPUART_STOPBITS_1;
	LPUART_InitStruct.Parity = LL_LPUART_PARITY_NONE;
	LPUART_InitStruct.TransferDirection = LL_LPUART_DIRECTION_TX_RX;
	LPUART_InitStruct.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;
	LL_LPUART_Init(LPUART1, &LPUART_InitStruct);
	LL_LPUART_Enable(LPUART1);
}


/** @brief USART3 Initialization Function
  * @param None
  * @retval None
 */
void USART3_Init(void){

  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  /**USART3 GPIO Configuration
  	  PB10   ------> USART3_TX
  	  PB11   ------> USART3_RX */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART3, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART3);
  LL_USART_Enable(USART3);

}

/**
  * @brief UART5 Initialization Function. UART5 use for Terminal or Debug 
  * @param None
  * @retval None
 */
void UART5_Init(void){

  	LL_USART_InitTypeDef UART_InitStruct = {0};
 	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  	/* Peripheral clock enable */
  	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART5);
  
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);

  	/* UART5 GPIO Configuration  
  	PC12   ------> UART5_TX
  	PD2   ------> UART5_RX */
  	GPIO_InitStruct.Pin = LL_GPIO_PIN_12;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
  	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  	UART_InitStruct.BaudRate = 115200;
  	UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  	UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  	UART_InitStruct.Parity = LL_USART_PARITY_NONE;
  	UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  	UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  	UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  	LL_USART_Init(UART5, &UART_InitStruct);
  	LL_USART_ConfigAsyncMode(UART5);
  	LL_USART_Enable(UART5);
}

/** @rief  This function setup interrupts for all ports and inside event .
  * @param  None
  * @retval None
 */
void SetupInterrupt(void){
	
    //LL_EXTI_InitTypeDef EXTI_InitStruct;

  	 /* LPUART1 interrupt Init */
  	NVIC_SetPriority(LPUART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),2, 0)); //Set priority №2 from 0..15
  	LL_LPUART_EnableIT_RXNE(LPUART1); //Enable Interrupt RX buff no empty 
  	//LL_LPUART_DisableIT_RXNE(LPUART1);
  	NVIC_EnableIRQ(LPUART1_IRQn);
  	//NVIC_DisableIRQ(LPUART1_IRQn);
  	/**********************************************/

  	/* USART5 interrupt Init */
  	NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0)); //Set priority №14 from 0..15
  	LL_USART_EnableIT_RXNE(USART3); //Enable Interrupt RX buff no empty 
  	//LL_USART_DisableIT_RXNE(USART3);
  	NVIC_EnableIRQ(USART3_IRQn);
  	//NVIC_DisableIRQ(UART3_IRQn);
  	/**********************************************/

  	/* UART5 interrupt Init */
  	NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),14, 0)); //Set priority №14 from 0..15
  	LL_USART_EnableIT_RXNE(UART5); //Enable Interrupt RX buff no empty 
  	//LL_USART_DisableIT_RXNE(UART5);
  	NVIC_EnableIRQ(UART5_IRQn);
  	//NVIC_DisableIRQ(UART5_IRQn);
  	/**********************************************/
  	
}

/** @brief GPIO Initialization Function
  * @param None
  * @retval None
 */
void GPIO_Init(void){
  	
  	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  	LL_PWR_EnableVddIO2();

  	/* GPIO Ports Clock Enable */
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
  	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOF);

  	/* Configure GPIO pin to control I2C bus multiplexor TMUX1209  (U5 and U18) */
  	GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
  	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  	LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	/* Setup default state */
  	LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_12|LL_GPIO_PIN_13);
  	LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_0);

  	/*For control multiplexor TMUX1209  need use:	 										*/
	/*ENABLE_TMUX1209_I2C1();      	- Enable multiplexor TMUX1209 							*/
	/*DISABLE_TMUX1209_I2C1();		- Disable multiplexor TMUX1209 							*/
	/*SW_TMUX1209_I2C1_main_PAM();  - Switch I2C1 line for comm. with the PAM module		*/
	/*SW_TMUX1209_I2C1_main_PDM();  - Switch I2C1 line for comm. with the PDM module  		*/
	/*SW_TMUX1209_I2C1_main_BAT();  - Switch I2C1 line for comm. with the Battery module	*/
	/*SW_TMUX1209_I2C1_main_PMM();	- Switch I2C1 line for comm. with the PMM module		*/

}


/** @rief  This function setup interrupts for all ports and inside event .
  * @param  freq - Frequency  PWM in Hz
  * @param  duty_cycle - PWM in %
  * @param  tim_devider - devider Timer
  * @retval None
 */
void PWM_init(uint32_t freq, uint32_t duty_cycle, uint16_t tim_devider){
	
	int pwm_freq = SystemCoreClock / ((tim_devider + 1) * freq);

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

	//Setup GPIO TIM3_CH3 ------> PB0 (pin M5)
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//Setup GPIO TIM3_CH4 ------> PB1 (pin M6)
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* TIM3 clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

	LL_TIM_StructInit(&TIM_InitStruct);
	TIM_InitStruct.Prescaler = tim_devider;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = pwm_freq-1;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM3, &TIM_InitStruct);

	LL_TIM_SetClockSource(TIM3, LL_TIM_CLOCKSOURCE_INTERNAL);
	
	//Setup PWM TIM3_CH3
	LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_OC_EnableFast(TIM3, LL_TIM_CHANNEL_CH3);

	LL_TIM_OC_StructInit(&TIM_OC_InitStruct);
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = (pwm_freq * duty_cycle / 100)-1;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &TIM_OC_InitStruct);

	//Setup PWM TIM3_CH4
	LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_OC_EnableFast(TIM3, LL_TIM_CHANNEL_CH4);

	LL_TIM_OC_StructInit(&TIM_OC_InitStruct);
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = (pwm_freq * duty_cycle / 100)-1;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);

	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

	//LL_TIM_EnableAllOutputs(TIM3); //???
	LL_TIM_EnableCounter(TIM3);
}


/** @brief IWDG Initialization Function
  * @param None
  * @retval Noneb
 */
void IWDG_Init(void){

	LL_IWDG_Enable(IWDG);
	LL_IWDG_EnableWriteAccess(IWDG);
	LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32);
	LL_IWDG_SetReloadCounter(IWDG, 4095);
	while (LL_IWDG_IsReady(IWDG) != 1){}

	LL_IWDG_SetWindow(IWDG, 4095);
	LL_IWDG_ReloadCounter(IWDG);

}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
 */
void _Error_Handler(char *file, int line){

	printf("Wrong parameters value: file %s on line %d\r\n", file, line);

}


