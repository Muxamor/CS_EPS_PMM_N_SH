
#include "main.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_it.h"
#include "CAND/CAN.h"
#include "uart_comm.h"

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**  @brief This function handles Non maskable interrupt. */
void NMI_Handler(void){

}

/** @brief This function handles Hard fault interrupt. */
void HardFault_Handler(void){

	while (1){

	}
}

/** @brief This function handles Memory management fault. */
void MemManage_Handler(void){

	while (1){

	}
}

/** @brief This function handles Prefetch fault, memory access fault. */
void BusFault_Handler(void){

	while (1){

	}
}

/** @brief This function handles Undefined instruction or illegal state. */
void UsageFault_Handler(void){

	while (1){

	}
}

/** @brief This function handles System service call via SWI instruction.*/
void SVC_Handler(void){

}

/** @brief This function handles Debug monitor. */
void DebugMon_Handler(void){

}

/** @brief This function handles Pendable request for system service. */
void PendSV_Handler(void){

}

/** @brief This function handles System tick timer. */
void SysTick_Handler(void){

}

/******************************************************************************/
/* STM32L4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l4xx.s).                    */
/******************************************************************************/


extern uint8_t UART_CHANGE_ACTIVE_CPU_FLAG;

/** @brief This function handles LPUARTq global interrupt */
void LPUART1_IRQHandler(void){



	uint8_t input_byte = 0;
	if(LL_LPUART_IsActiveFlag_RXNE(LPUART1)){

		input_byte = LL_LPUART_ReceiveData8(LPUART1);

		if(input_byte == 0xAA || input_byte == 0xBB ){

			UART_CHANGE_ACTIVE_CPU_FLAG = 1;
		}
	}



}

/** @brief This function handles UART5 global interrupt */
void UART5_IRQHandler(void){

	uint8_t input_byte = 0;
	if(LL_USART_IsActiveFlag_RXNE(UART5)){

		//input_byte = LL_USART_ReceiveData8(UART5);
		input_byte = LL_USART_ReceiveData8(UART5);

		if(input_byte == 0xAA || input_byte == 0xBB ){
			UART_CHANGE_ACTIVE_CPU_FLAG = 1;
		}
	}
}



/** @brief This function handles USART3 global interrupt */
void USART3_IRQHandler(void){

	//uint8_t input_byte = 0;
	if(LL_USART_IsActiveFlag_RXNE(USART3)){
		//input_byte = LL_USART_ReceiveData8(USART3);
	}
}



/** @brief This function handles CAN1 global interrupt */
//interrupt handler is in ../Core/Src/CAND/canv.c


/** @brief This function handles CAN2 global interrupt */
//interrupt handler is in ../Core/Src/CAND/canv.c




