#include "stm32l4xx.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_iwdg.h"
//#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_bus.h"
//#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_usart.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "PMM/pmm_config.h"
#include "canv.h"

//#include  <stdio.h>
//#include "stm32l4xx_ll_dma.h"


uint32_t SysTick_Counter;

/** @brief Initialization of basic functionality
 * @retval None
 */
void LL_Init(void) {

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* System interrupt init*/
	/* MemoryManagement_IRQn interrupt configuration */
	NVIC_SetPriority(MemoryManagement_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* BusFault_IRQn interrupt configuration */
	NVIC_SetPriority(BusFault_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* UsageFault_IRQn interrupt configuration */
	NVIC_SetPriority(UsageFault_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* SVCall_IRQn interrupt configuration */
	NVIC_SetPriority(SVCall_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* DebugMonitor_IRQn interrupt configuration */
	NVIC_SetPriority(DebugMonitor_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* PendSV_IRQn interrupt configuration */
	NVIC_SetPriority(PendSV_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	/* SysTick_IRQn interrupt configuration */
	NVIC_SetPriority(SysTick_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));

	/* PVD Configuration */
	LL_PWR_SetPVDLevel(LL_PWR_PVDLEVEL_1);
	/* Enable the PVD */
	LL_PWR_EnablePVD();
}

/** @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(uint32_t CPU_Speed) {

    uint8_t HSE_Fault = 0;
    uint32_t down_counter = 500000;

    if( CPU_Speed == CPU_Clock_16MHz  ){

    	if( LL_RCC_PLL_GetMainSource() == LL_RCC_PLLSOURCE_HSE ){
    		LL_RCC_HSE_EnableCSS();
    		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
    		/* Wait till System clock is ready */
    		down_counter = 500000;
    		while((LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE) && (down_counter != 0)) {
    			down_counter--;
    		}

    	}else if( LL_RCC_PLL_GetMainSource() == LL_RCC_PLLSOURCE_HSI ){
    		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    		/* Wait till System clock is ready */
    		down_counter = 500000;
    		while((LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) && (down_counter != 0)) {
    			down_counter--;
    		}

    		LL_RCC_HSE_DisableBypass();
    		LL_RCC_HSE_Disable();
    	}

    	LL_RCC_PLL_DisableDomain_SYS();
        LL_RCC_PLL_Disable();

        LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
        if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0) {
            #ifdef DEBUGprintf
                Error_Handler();
		    #endif
        }

        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
        LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

        LL_Init1msTick(16000000);
        LL_SetSystemCoreClock(16000000);

    }else{

        LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);

        if (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4) {
            #ifdef DEBUGprintf
                Error_Handler();
		    #endif
        }

        LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

        /* Enable HSE */
        LL_RCC_HSE_EnableBypass();
        LL_RCC_HSE_Enable();
        /* Wait till HSE is ready */
        down_counter = 500000;
        while((LL_RCC_HSE_IsReady() != 1) && (down_counter != 0)) {
            down_counter--;
        }

        if (down_counter == 0) {
            HSE_Fault = 1;

            /* Enable HSI - backup clock*/
            LL_RCC_HSI_Enable();
            /* Wait till HSI is ready */
            down_counter = 500000;
            while((LL_RCC_HSI_IsReady() != 1) && (down_counter != 0)) {
            	down_counter--;
            }

            //LL_RCC_HSI_SetCalibTrimming(64); // Not need.
			#ifdef DEBUGprintf
            	Error_Handler();
	    	#endif
        }

        /* Enable LSI for enable WDG */
        LL_RCC_LSI_Enable();
        /* Wait till LSI is ready */
        while (LL_RCC_LSI_IsReady() != 1) {
        }

        if (HSE_Fault == 0) {
            /* Enable CSS */
            LL_RCC_HSE_EnableCSS();
            LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2);
        }else{
            LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLLM_DIV_1, 10, LL_RCC_PLLR_DIV_2);
        }

        LL_RCC_PLL_EnableDomain_SYS();
        LL_RCC_PLL_Enable();

        /* Wait till PLL is ready */
        down_counter = 500000;
        while((LL_RCC_PLL_IsReady() != 1) && (down_counter != 0)) {
        	down_counter--;
        }

        LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

        /* Wait till System clock is ready */
        down_counter = 500000;
        while((LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) && (down_counter != 0)) {
            down_counter--;
        }

        LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
        LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
        LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

        LL_Init1msTick(80000000);
        LL_SetSystemCoreClock(80000000);
    }

    /* Setup RTC clock */
    //LL_PWR_EnableBkUpAccess();
    //LL_RCC_ForceBackupDomainReset();
    //LL_RCC_ReleaseBackupDomainReset();
    //LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    //LL_RCC_EnableRTC();
    LL_RCC_SetLPUARTClockSource(LL_RCC_LPUART1_CLKSOURCE_PCLK1); // main LPUART1 for comm. CPUm <-> CPUb
    LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1); // backup USART3 for comm. CPUm <-> CPUb
    //LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1); // Comm. with module
    //LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2); // Comm. with module
    //LL_RCC_SetUARTClockSource(LL_RCC_UART5_CLKSOURCE_PCLK1); // Terminal or debug UART5
    //LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1); 	 // System I2C1 for comm. with another subsystem CubeSat
    LL_RCC_SetI2CClockSource(LL_RCC_I2C3_CLKSOURCE_PCLK1); // I2C3 comm. inside main and backup block
    //LL_RCC_SetI2CClockSource(LL_RCC_I2C2_CLKSOURCE_PCLK1);     // Backup I2C1 Comm. with module PAM and PDM and another part of PMM module
    LL_RCC_SetI2CClockSource(LL_RCC_I2C4_CLKSOURCE_PCLK1); // Main I2C4 Comm. with module PAM and PDM and another part of PMM module

}



/** @brief I2C3 Initialization Function. I2C3 use for communiction inside main and backup block
 * @param None
 * @retval None
 */
void I2C3_Init( void ) {

	LL_I2C_Disable(I2C3);
	//LL_I2C_Enable(I2C3);

	LL_I2C_DeInit(I2C3);

	LL_I2C_InitTypeDef I2C_InitStruct = { 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	//LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
	//LL_PWR_EnableVddIO2();


	/**I2C3 in Grifon GPIO Configuration
	 PA7   ------> I2C3_SCL
	 PG8   ------> I2C3_SDA */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C3);

	/** I2C Initialization */
	LL_I2C_EnableAutoEndMode(I2C3);
	LL_I2C_DisableOwnAddress2(I2C3);
	LL_I2C_DisableGeneralCall(I2C3);
	LL_I2C_EnableClockStretching(I2C3);

	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
	if( SystemCoreClock == CPU_Clock_16MHz ){
	    I2C_InitStruct.Timing = 0x00303D5B;
	}else{
	    I2C_InitStruct.Timing = 0x00702991;//0x00702991 - 400kHz, 0x10909CEC - 100kHz.
	}

	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
	I2C_InitStruct.DigitalFilter = 0;
	I2C_InitStruct.OwnAddress1 = 0;
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	LL_I2C_Init(I2C3, &I2C_InitStruct);
	LL_I2C_SetOwnAddress2(I2C3, 0, LL_I2C_OWNADDRESS2_NOMASK);
	LL_I2C_Enable(I2C3);
}

/** @brief I2C3 DeInitialization Function. Main I2C4 communication
 * 		with module PAM and PDM and another part of PMM module.
 * @param None
 * @retval None */
void I2C3_DeInit(void){

    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    LL_I2C_Disable(I2C3);
    LL_I2C_DeInit(I2C3);

    /* Peripheral clock disable */
    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C3);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
   // LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
    //LL_PWR_EnableVddIO2();

    /**I2C3 GPIO Configuration
	 PC0   ------> I2C3_SCL
	 PC1   ------> I2C3_SDA */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/** @brief I2C4 Initialization Function. Main I2C4 communication 
 * 		with module PAM and PDM and another part of PMM module.
 * @param None
 * @retval None */
void I2C4_Init( void ) {

	LL_I2C_Disable(I2C4);
	//LL_I2C_Enable(I2C4);

	LL_I2C_DeInit(I2C4);

	LL_I2C_InitTypeDef I2C_InitStruct = { 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
	/**I2C4 GPIO Configuration
	 PD12   ------> I2C4_SCL
	 PD13   ------> I2C4_SDA */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_I2C4);

	/** I2C Initialization */
	LL_I2C_EnableAutoEndMode(I2C4);
	LL_I2C_DisableOwnAddress2(I2C4);
	LL_I2C_DisableGeneralCall(I2C4);
	LL_I2C_EnableClockStretching(I2C4);

	I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
	if( SystemCoreClock == CPU_Clock_16MHz ){
	    I2C_InitStruct.Timing = 0x20601418; //100kHz and Rise time=1000ns
	}else{
	    I2C_InitStruct.Timing = 0xB0801A1F; //0x00702991 - 400kHz, 0x10909CEC - 100kHz, 0xB0801A1F-100kHz and Rise time=1000ns
	}

	I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
	I2C_InitStruct.DigitalFilter = 0;
	I2C_InitStruct.OwnAddress1 = 0;
	I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
	I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
	LL_I2C_Init(I2C4, &I2C_InitStruct);

	LL_I2C_SetOwnAddress2(I2C4, 0, LL_I2C_OWNADDRESS2_NOMASK);

	LL_I2C_Enable(I2C4);
}

/** @brief I2C4 DeInitialization Function. Main I2C4 communication
 * 		with module PAM and PDM and another part of PMM module.
 * @param None
 * @retval None */
void I2C4_DeInit(void){

    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    LL_I2C_Disable(I2C4);

    LL_I2C_DeInit(I2C4);

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
    /**I2C4 GPIO Configuration
     PF14   ------> I2C4_SCL
     PF15   ------> I2C4_SDA */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Peripheral clock disable */
    LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_I2C4);
}

/** @brief Re-initialization selected I2C
 * @param  *I2Cx - pointer to I2C controller, where x is a number (I2C3, I2C4).
 * @retval None
 */
void I2C_ReInit(I2C_TypeDef *I2Cx) {

	LL_I2C_DeInit(I2Cx);

	if (I2Cx == I2C3) {
		I2C3_Init();
	} else if (I2Cx == I2C4) {
		I2C4_Init();
	}
}

/** @brief I2C bus software reset
 * @param  *I2Cx - pointer to I2C controller, where x is a number (I2C3, I2C4).
 * @param  number_cycle - number of clocks.
 * @retval None
 */
void I2C_Bus_SoftwareReset(I2C_TypeDef *I2Cx, uint8_t number_cycle ) {

	uint16_t period = 0, i = 0, count = 0;

	period = ( uint16_t )(( ((float)SystemCoreClock) * 5.0f) / (1000000.0f * 10.5f));
	//period = 42;
	count = period;

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	if (I2Cx == I2C3) {
	    I2C3_DeInit();
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

		GPIO_InitStruct.Pin = LL_GPIO_PIN_0|LL_GPIO_PIN_1;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1);

		for (i = 0; i <= number_cycle; i++) {
			while (count > 0) {
				count--;
			}
			count = period;
			LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_0);
			while (count > 0) {
				count--;
			}
			count = period;
			LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_0);
		}
		LL_mDelay( 20 );
		I2C3_Init();
		LL_mDelay( 20 );
	}

	if (I2Cx == I2C4) {
	    I2C4_DeInit();
		LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);

		GPIO_InitStruct.Pin = LL_GPIO_PIN_12|LL_GPIO_PIN_13;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12);
		LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13);

		for (i = 0; i <= number_cycle; i++) {
			while (count > 0) {
				count--;
			}
			count = period;
			LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12);
			while (count > 0) {
				count--;
			}
			count = period;
			LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12);
		}

		LL_mDelay( 20 );
		I2C4_Init();
		LL_mDelay( 20 );
	}
}

/** @brief LPUART1 Initialization Function
 * @param None
 * @retval None
 */
void LPUART1_Init(void) {

	LL_LPUART_InitTypeDef LPUART_InitStruct = { 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_LPUART1);

	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

	/**LPUART1 GPIO Configuration
	 PA2   ------> LPUART1_RX
	 PA3   ------> LPUART1_TX */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_3;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO; //Set PULL UP but default set PULL NO
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	LPUART_InitStruct.BaudRate = 1536000; //115200;
	LPUART_InitStruct.DataWidth = LL_LPUART_DATAWIDTH_8B;
	LPUART_InitStruct.StopBits = LL_LPUART_STOPBITS_1;
	LPUART_InitStruct.Parity = LL_LPUART_PARITY_NONE;
	LPUART_InitStruct.TransferDirection = LL_LPUART_DIRECTION_TX_RX;
	LPUART_InitStruct.HardwareFlowControl = LL_LPUART_HWCONTROL_NONE;
	LL_LPUART_Init(LPUART1, &LPUART_InitStruct);
	LL_LPUART_Enable(LPUART1);
}


/** @brief LPUART1 DeInitialization Function
 * @param None
 * @retval None
 */
void LPUART1_DeInit(void){

//    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    LL_LPUART_Disable(LPUART1);
    LL_LPUART_DeInit(LPUART1);

//    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
//    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOG);
//    LL_PWR_EnableVddIO2();

//    GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
//    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
//    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
//    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
//    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//    LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    LL_APB1_GRP2_DisableClock(LL_APB1_GRP2_PERIPH_LPUART1);

    NVIC_DisableIRQ(LPUART1_IRQn);
}


/** @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
void USART3_Init(void) {

	LL_USART_InitTypeDef USART_InitStruct = { 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);
	/**USART3 GPIO Configuration
	 PB10   ------> USART3_TX
	 PB11   ------> USART3_RX */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4|LL_GPIO_PIN_5;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;//Set PULL UP but default set PULL NO
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	USART_InitStruct.BaudRate = 1536000; //115200;
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


/** @brief USART3 DeInitialization Function
 * @param None
 * @retval None
 */
void USART3_DeInit(void){

//    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//
//	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    LL_USART_Disable(USART3);
    LL_USART_DeInit(USART3);



//    GPIO_InitStruct.Pin = LL_GPIO_PIN_10|LL_GPIO_PIN_11;
//    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
//    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_USART3);

    NVIC_DisableIRQ(USART3_IRQn);
}


/**
 * @brief UART5 Initialization Function. UART5 use for Terminal or Debug
 * @param None
 * @retval None
 */
void UART5_Init(void) {

	LL_USART_InitTypeDef UART_InitStruct = { 0 };
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

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
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;//Set PULL UP but default set PULL NO
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;//LL_GPIO_PULL_UP; //Set PULL UP but default set PULL NO
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
void Setup_UART_Interrupt( void) {

	//LL_EXTI_InitTypeDef EXTI_InitStruct;

	/*Enable interrupt for SysTick */
    //LL_SYSTICK_EnableIT();
    //LL_SYSTICK_DisableIT();
    /**********************************************/

	/* LPUART1 interrupt Init */
	NVIC_SetPriority(LPUART1_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 2, 0)); //Set priority №2 from 0..15
	LL_LPUART_EnableIT_RXNE(LPUART1); //Enable Interrupt RX buff no empty 
	//LL_LPUART_DisableIT_RXNE(LPUART1);
	//LL_LPUART_DisableIT_ERROR(LPUART1);
	NVIC_EnableIRQ(LPUART1_IRQn);
	//NVIC_DisableIRQ(LPUART1_IRQn);
	/**********************************************/

	/* USART3 interrupt Init */
	NVIC_SetPriority(USART3_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 3, 0)); //Set priority №14 from 0..15
	LL_USART_EnableIT_RXNE(USART3); //Enable Interrupt RX buff no empty 
	//LL_USART_DisableIT_RXNE(USART3);
	//LL_USART_DisableIT_ERROR(USART3);
	NVIC_EnableIRQ(USART3_IRQn);
	//NVIC_DisableIRQ(USART3_IRQn);
	/**********************************************/

	/* UART5 interrupt Init */
	//For Norbi not need yat NVIC_SetPriority(UART5_IRQn,NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 14, 0)); //Set priority №14 from 0..15
    //For Norbi not need yat LL_USART_EnableIT_RXNE(UART5); //Enable Interrupt RX buff no empty
	//LL_USART_DisableIT_RXNE(UART5);
	//LL_USART_DisableIT_ERROR(USART5);
    //For Norbi not need yat NVIC_EnableIRQ(UART5_IRQn);
	//NVIC_DisableIRQ(UART5_IRQn);
	/**********************************************/
}

/* CAN1 interrupt Init */
//CAN interrupt initialization is in CAN_init_eps in this file.
/**********************************************/

/* CAN2 interrupt Init */
//CAN interrupt initialization is in CAN_init_eps in this file.
/**********************************************/

/** @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init(void) {

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };


	/* GPIO Ports Clock Enable */
	 LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOE);
	 LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	 LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
	 LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
	 LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

	// LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);


	/*-------------------------------------------------------------------------------------*/
	/* Configure GPIO pin to control I2C bus multiplexor TMUX1209  (U5 and U18) */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* Setup default state */
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6);


	/*For control multiplexor TMUX1209  need use:											*/
	/*ENABLE_TMUX1209_I2C();       	- Enable multiplexor TMUX1209 							*/
	/*DISABLE_TMUX1209_I2C();		- Disable multiplexor TMUX1209 							*/
	/*SW_TMUX1209_I2C_main_PAM();  	- Switch I2C line for comm. with the PAM module			*/
	/*SW_TMUX1209_I2C_main_PDM();  	- Switch I2C line for comm. with the PDM module  		*/
	/*SW_TMUX1209_I2C_main_PBM();  	- Switch I2C line for comm. with the PBM module			*/
	/*SW_TMUX1209_I2C_main_PMM();	- Switch I2C line for comm. with the PMM module			*/
	/****************************************************************************************/

	/*-------------------------------------------------------------------------------------*/
	//Setup TCA9539 GPIOExt1 controls pin RESET. at PMM modules.

	//Set Reset pin like input because if setup as output pin pull down and will twitch at the time of reboot CPU
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//  GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
//	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
//	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
//	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
//	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
//	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/***************************************************************************************/

	/*-------------------------------------------------------------------------------------*/
	//Setup controls pins RESET. at  PAM PDM modules.

	//Set Reset pin like input because if setup as output pin pull down and will twitch at the time of reboot CPU
	GPIO_InitStruct.Pin = LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/***************************************************************************************/


	/*-------------------------------------------------------------------------------------*/
	/* Configure GPIO pin to control write protection on FRAM1 and FRAM2  FM24Cl64 */
	/** FRAM write control pin  
	 PB2 ------> Write control FRAM1 (main)
	 PC9  ------> Write control FRAM1 (backup)
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* Setup default state */
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_2); // Disable write to FRAM1
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);  // Disable write to FRAM2
	/****************************************************************************************/

	/*-------------------------------------------------------------------------------------*/
	/* Configure pins for detect Main/Backup mode for CPU  */
	/** PD9 | PD10 | PD11  = 1 -> Main CPU mode
	 PD9 | PD10 | PD11  = 0 -> Backup CPU mode
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	/****************************************************************************************/

	/*-------------------------------------------------------------------------------------*/
	/* Pins for get Power Good */
	/**
	 PE2 - Power good for PWR_Supply_Main_PG
	 PE3 - Power good for PWR_Supply_Backup_PG
	 PE4 - Power good for eF_PG_LPBM_1	 (  )
	 PE5 - Power good for eF_PG_LPBM_2   ( Cleon1 PWR_Ch_PG_Deploy_LP)
	 PE7 - Power good for eF_PG_Deploy_L ( Cleon1 PWR_Ch_PG_Vbat2_eF2)
	 PE8 - Power good for eF_PG_Deploy_P ( Cleon1 PWR_Ch_PG_Vbat2_eF1)
	 PE9 - Power good for eF_PG_Vbat1    ( Cleon1 PWR_Ch_PG_Vbat1_eF2)
	 PE10 - Power good for eF_PG_Vbat2   ( Cleon1 PWR_Ch_PG_Vbat1_eF1)
	 PE11 - Power good for PG_Power_CPUm ( Cleon1 PWR_Ch_PG_CANmain)
	 PE12 - 				             ( Cleon1 PWR_Ch_PG_CANbackup)
	 PE13 -  							 ( Cleon1 PWR_Ch_PG_I2C_Bus)
	 PE14 - Power good for PG_DC_5V_CANb ( Cleon1 PWR_Ch_PG_3_3V_Bus)
	 PE15 - Power good for PG_DC_5V_CANm ( Cleon1 PWR_Ch_PG_5V_Bus)
	 PD8  - Power good for PG_Power_CPUb ( Cleon1 CPUm_I2C_GPIO_Ext_PMM_INT)
	  */

	GPIO_InitStruct.Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10\
                            | LL_GPIO_PIN_11 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	/****************************************************************************************/

    /*-------------------------------------------------------------------------------------*/
    /* Pins for show  PBM Energy Low*/
    /** PD14 - PBM Energy level Low pin. Reset pin when PBM Energy level is Low.*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    /*For control pin PBM Energy Level LOW  need use:	*/
    /*PBM_Low_Energy_Set_pin();       		    		*/
    /*PBM_Low_Energy_Reset_pin();       		    	*/
    PBM_Low_Energy_Reset_pin(); //Set defaule state
    /****************************************************************************************/


	/*-------------------------------------------------------------------------------------*/
	/* Set analog mode for free pins. */
	/** Free pins*/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_9 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_5 |LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_3 |  LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_13;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_6 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/** @rief  Init PWM TIM 3 Ch3 and Ch4
 * @param  freq - Frequency  PWM in Hz
 * @param  duty_cycle - PWM in %
 * @param  tim_divider - divider Timer
 * @retval None
 */
void PWM_Init_Ch3_Ch4( uint32_t freq, uint32_t duty_cycle, uint16_t tim_divider) {

	uint32_t pwm_freq = 0;

	pwm_freq = (uint32_t) (SystemCoreClock / (((uint32_t) (tim_divider + 1)) * freq));

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	LL_TIM_InitTypeDef TIM_InitStruct = { 0 };
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = { 0 };

	//Setup GPIO TIM3_CH3 ------> PB0
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//Setup GPIO TIM3_CH4 ------> PB1
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
	TIM_InitStruct.Prescaler = tim_divider;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = pwm_freq - 1;
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
	TIM_OC_InitStruct.CompareValue = (pwm_freq * duty_cycle / 100) - 1;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &TIM_OC_InitStruct);

	//Setup PWM TIM3_CH4
	LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_OC_EnableFast(TIM3, LL_TIM_CHANNEL_CH4);

	LL_TIM_OC_StructInit(&TIM_OC_InitStruct);
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = (pwm_freq * duty_cycle / 100) - 1;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH4, &TIM_OC_InitStruct);

	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);

	//LL_TIM_EnableAllOutputs(TIM3); //???
	LL_TIM_EnableCounter(TIM3);
}



/** @rief  DeInit PWM TIM 3 Ch3 and Ch4
 * @param  None
 * @retval None
 */
void PWM_DeInit_Ch3_Ch4( void ){

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_DisableChannel(TIM3, LL_TIM_CHANNEL_CH4);
	LL_TIM_DisableCounter(TIM3);
	LL_TIM_DeInit(TIM3);
	LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM3);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}


/** @brief IWDG Initialization Function
 * @param  period - Set WDG period in ms (now max 4095 ms)
 * @retval None
 */
void IWDG_Init(uint16_t period) {

	uint32_t down_counter = 500000;

	if(period > 4095){
		period = 4095;
	}

	LL_IWDG_Enable(IWDG);
	LL_IWDG_EnableWriteAccess(IWDG);
	LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_32); // (IWDG_PRESCALER/32000)=(time 1 tick) -> 32/32000=1ms
	LL_IWDG_SetReloadCounter(IWDG, period); // 1.0*ReloadCounter = WDG_period ms

	while ((LL_IWDG_IsReady(IWDG) != 1) && (down_counter != 0)) {
		down_counter--;
	}

	//LL_IWDG_SetWindow(IWDG, 4095);
	LL_IWDG_ReloadCounter(IWDG);
}

/** @brief Init CAN
 * @param *can_ref - pointer to CAN number port.
 * @retval None
 */
int8_t CAN_init_eps(CAN_TypeDef *can_ref) {
	int8_t error_status = 0;
	uint32_t CAN_BTR = (0x00 << 24) | (0x01 << 20) | (12 << 16) | (4 << 0); // SJW = 1; TS2 = 1+1; TS1 = 12+1; Prescaler = 40;

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

	if (can_ref == CAN1) {
		GPIO_InitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_9;
		LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	} else if (can_ref == CAN2) {
		GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
		GPIO_InitStruct.Alternate = LL_GPIO_AF_10;
		LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	} else {

		return -1;
	}

	error_status = CAN_Init(can_ref, CAN_BTR);

	if (error_status == ERR_CAN_NORMAL_MODE) {
		CAN->MSR &= ~CAN_MSR_WKUI;
	} else if (error_status == ERR_CAN_INIT_MODE) {
		return ERR_CAN_INIT_MODE;
	}

	if (can_ref == CAN1) {
		/* CAN1 interrupt Init */
		NVIC_DisableIRQ(CAN1_RX0_IRQn);
		NVIC_SetPriority(CAN1_RX0_IRQn,
				NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0)); //Set priority №14 from 0..15
		NVIC_EnableIRQ(CAN1_RX0_IRQn);
	} else if (can_ref == CAN2) {
		/* CAN2 interrupt Init */
		NVIC_DisableIRQ(CAN2_RX0_IRQn);
		NVIC_SetPriority(CAN2_RX0_IRQn,
				NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 1, 0)); //Set priority №14 from 0..15
		NVIC_EnableIRQ(CAN2_RX0_IRQn);
	}

	return SUCCESS;
}

/** @brief DeInit CAN
 * @param *can_ref - pointer to CAN number port.
 * @retval None
 */
int8_t CAN_DeInit_eps(CAN_TypeDef *can_ref) {

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	if (can_ref == CAN1) {
		RCC->APB1RSTR1 |= RCC_APB1RSTR1_CAN1RST;
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_CAN1EN;

	} else if (can_ref == CAN2) {
		RCC->APB1RSTR1 |= RCC_APB1RSTR1_CAN2RST;
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_CAN2EN;

	} else {
		return ERROR_N;
	}

	can_ref->BTR = 0;
	can_ref->FMR = 0xE01;
	can_ref->FS1R = 0;
	can_ref->FFA1R = 0;
	can_ref->MCR = (1 << 16) | (1 << 1);

	if (can_ref == CAN1) {
		NVIC_DisableIRQ(CAN1_RX0_IRQn);
	} else {
		NVIC_DisableIRQ(CAN2_RX0_IRQn);
	}

	if (can_ref == CAN1) {
		GPIO_InitStruct.Pin = LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	} else if (can_ref == CAN2) {
		GPIO_InitStruct.Pin = LL_GPIO_PIN_12 | LL_GPIO_PIN_13;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(GPIOB, &GPIO_InitStruct);


	} else {
		return ERROR_N;
	}

	return SUCCESS;
}
