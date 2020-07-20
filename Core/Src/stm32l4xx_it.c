#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_it.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_usart.h"
#include "uart_eps_comm.h"

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
/* please refer to the startup file (startup_stm32l4xx->s).                    */
/******************************************************************************/


extern _UART_EPS_COMM *UART_M_eps_comm;

/** @brief This function handles LPUARTq global interrupt */
void LPUART1_IRQHandler(void){

	
	if( LL_LPUART_IsActiveFlag_ORE(LPUART1) || LL_LPUART_IsActiveFlag_NE(LPUART1) || LL_LPUART_IsActiveFlag_FE(LPUART1) || LL_LPUART_IsActiveFlag_PE(LPUART1) ){

		LL_LPUART_ClearFlag_ORE(LPUART1);
		LL_LPUART_ClearFlag_NE(LPUART1);
		LL_LPUART_ClearFlag_FE(LPUART1);
		LL_LPUART_ClearFlag_PE(LPUART1);
		//(void) LPUART1->ISR;
		//(void) LPUART1->RDR;
		UART_M_eps_comm->stop_recv_pack_flag = 0;
		UART_M_eps_comm->permit_recv_pack_flag = 0;
		LL_LPUART_ReceiveData8(LPUART1);

		UART_M_eps_comm->error_port_counter++;

	}else if(LL_LPUART_IsActiveFlag_RXNE(LPUART1)){
		uint8_t input_byte = 0;

		input_byte = LL_LPUART_ReceiveData8(LPUART1);

		// Geting preamble.
		if(input_byte == 0xAA && UART_M_eps_comm->permit_recv_pack_flag == 0 && UART_M_eps_comm->stop_recv_pack_flag == 0 ){ 
			UART_M_eps_comm->size_recv_pack = 0;
			UART_M_eps_comm->recv_pack_buf[UART_M_eps_comm->size_recv_pack] = input_byte;

			UART_M_eps_comm->stop_recv_pack_flag = 0;
			UART_M_eps_comm->permit_recv_pack_flag = 1;
			UART_M_eps_comm->expected_size_recv_pack = 8; // 1(preabmle)+1(destination address)+1(source address)+1(package tag)+2(data size)+2(crc)
			UART_M_eps_comm->size_recv_pack++;

		// Geting and check address.
		}else if( UART_M_eps_comm->permit_recv_pack_flag == 1 && UART_M_eps_comm->size_recv_pack == 1 && UART_M_eps_comm->stop_recv_pack_flag == 0  ){

				UART_M_eps_comm->recv_pack_buf[UART_M_eps_comm->size_recv_pack] = input_byte;
			
			if(	UART_M_eps_comm->recv_pack_buf[UART_M_eps_comm->size_recv_pack] == UART_M_eps_comm->uart_unit_addr || UART_M_eps_comm->recv_pack_buf[UART_M_eps_comm->size_recv_pack] == 254 ){ // Chaeck addres in parsel. 254- broadcast package.
				
				UART_M_eps_comm->size_recv_pack++;

			}else{ // if the address did not match finish get parsel.
				UART_M_eps_comm->permit_recv_pack_flag = 0;
				UART_M_eps_comm->size_recv_pack = 0;
			}
		
		// Geting the rest of the package.
		}else if( UART_M_eps_comm->permit_recv_pack_flag == 1 && UART_M_eps_comm->size_recv_pack > 1 && UART_M_eps_comm->stop_recv_pack_flag == 0 ){ 

			UART_M_eps_comm->recv_pack_buf[UART_M_eps_comm->size_recv_pack] = input_byte;

			//Get the package size we need to get
			if(UART_M_eps_comm->size_recv_pack == 5 ){
				
				UART_M_eps_comm->expected_size_recv_pack = UART_M_eps_comm->expected_size_recv_pack + ( ( ( (uint16_t)(UART_M_eps_comm->recv_pack_buf[4]) ) << 8 ) | ( (uint16_t)(UART_M_eps_comm->recv_pack_buf[5]) ) );
				
				//If the packet size is larger than the receive buffer, we stop receiving the packet
				if( UART_M_eps_comm->expected_size_recv_pack > UART_EPS_PACK_SIZE_BUFF ){ 
					UART_M_eps_comm->permit_recv_pack_flag = 0;
					UART_M_eps_comm->size_recv_pack = 0;
				}
			}

			UART_M_eps_comm->size_recv_pack++;

			//We received the package
			if( UART_M_eps_comm->size_recv_pack >= UART_M_eps_comm->expected_size_recv_pack ){
				//NVIC_DisableIRQ(LPUART1_IRQn);
				UART_M_eps_comm->stop_recv_pack_flag = 1;
			}

		// Some byte 
		}else{
		 /*empty*/
		}

	}else{
		LL_LPUART_ReceiveData8(LPUART1);
	}

}


extern _UART_EPS_COMM *UART_B_eps_comm;

/** @brief This function handles USART3 global interrupt */
void USART3_IRQHandler(void){

	if( LL_USART_IsActiveFlag_ORE(USART3) || LL_USART_IsActiveFlag_NE(USART3) || LL_USART_IsActiveFlag_FE(USART3) || LL_USART_IsActiveFlag_PE(USART3) ){
		LL_USART_ClearFlag_ORE(USART3);
		LL_USART_ClearFlag_NE(USART3);
		LL_USART_ClearFlag_FE(USART3);
		LL_USART_ClearFlag_PE(USART3);
		//(void) USART3->ISR;
		//(void) USART3->RDR;
		UART_B_eps_comm->stop_recv_pack_flag = 0;
		UART_B_eps_comm->permit_recv_pack_flag = 0;
		LL_USART_ReceiveData8(USART3);
		UART_B_eps_comm->error_port_counter++;

	}else if(LL_USART_IsActiveFlag_RXNE(USART3)){

		uint8_t input_byte = 0;
		input_byte = LL_USART_ReceiveData8(USART3);

		// Geting preamble.
		if(input_byte == 0xAA && UART_B_eps_comm->permit_recv_pack_flag  == 0 && UART_B_eps_comm->stop_recv_pack_flag == 0 ){
			UART_B_eps_comm->size_recv_pack = 0;
			UART_B_eps_comm->recv_pack_buf[UART_B_eps_comm->size_recv_pack] = input_byte;

			UART_B_eps_comm->stop_recv_pack_flag = 0;
			UART_B_eps_comm->permit_recv_pack_flag = 1;
			UART_B_eps_comm->expected_size_recv_pack = 8; // 1(preabmle)+1(destination address)+1(source address)+1(package tag)+2(data size)+2(crc)
			UART_B_eps_comm->size_recv_pack++;

		// Geting and check address.
		}else if( UART_B_eps_comm->permit_recv_pack_flag == 1 && UART_B_eps_comm->size_recv_pack == 1 && UART_B_eps_comm->stop_recv_pack_flag == 0){

			UART_B_eps_comm->recv_pack_buf[UART_B_eps_comm->size_recv_pack] = input_byte;

			if(	UART_B_eps_comm->recv_pack_buf[UART_B_eps_comm->size_recv_pack] == UART_B_eps_comm->uart_unit_addr || UART_B_eps_comm->recv_pack_buf[UART_B_eps_comm->size_recv_pack] == 254 ){ // Chaeck addres in parsel. 254- broadcast package.

				UART_B_eps_comm->size_recv_pack++;

			}else{ // if the address did not match finish get parsel.
				UART_B_eps_comm->permit_recv_pack_flag = 0;
				UART_B_eps_comm->size_recv_pack = 0;
			}

		// Geting the rest of the package.
		}else if( UART_B_eps_comm->permit_recv_pack_flag == 1 && UART_B_eps_comm->size_recv_pack > 1 && UART_B_eps_comm->stop_recv_pack_flag == 0 ){

			UART_B_eps_comm->recv_pack_buf[UART_B_eps_comm->size_recv_pack] = input_byte;

			//Get the package size we need to get
			if(UART_B_eps_comm->size_recv_pack == 5 ){

				UART_B_eps_comm->expected_size_recv_pack = UART_B_eps_comm->expected_size_recv_pack + ( ( ( (uint16_t)(UART_B_eps_comm->recv_pack_buf[4]) ) << 8 ) | ( (uint16_t)(UART_B_eps_comm->recv_pack_buf[5]) ) );

				//If the packet size is larger than the receive buffer, we stop receiving the packet
				if( UART_B_eps_comm->expected_size_recv_pack > UART_EPS_PACK_SIZE_BUFF ){
					UART_B_eps_comm->permit_recv_pack_flag = 0;
					UART_B_eps_comm->size_recv_pack = 0;
				}
			}

			UART_B_eps_comm->size_recv_pack++;

			//We received the package
			if( UART_B_eps_comm->size_recv_pack >= UART_B_eps_comm->expected_size_recv_pack ){
				//NVIC_DisableIRQ( USART3_IRQn );
				UART_B_eps_comm->stop_recv_pack_flag = 1;
			}

		// Some byte
		}else{
			/*empty*/
		}

	}else{
		LL_USART_ReceiveData8(USART3);
	}
}



/** @brief This function handles UART5 global interrupt */
void UART5_IRQHandler(void){

	uint8_t input_byte = 0;
	if(LL_USART_IsActiveFlag_RXNE(UART5)){

		//input_byte = LL_USART_ReceiveData8(UART5);
		input_byte = LL_USART_ReceiveData8(UART5);

		if(input_byte == 0xAA || input_byte == 0xBB ){
			//UART_CHANGE_ACTIVE_CPU_FLAG = 1;
		}
	}
}






/** @brief This function handles CAN1 global interrupt */
//interrupt handler is in ../Core/Src/CAND/canv.c


/** @brief This function handles CAN2 global interrupt */
//interrupt handler is in ../Core/Src/CAND/canv.c




