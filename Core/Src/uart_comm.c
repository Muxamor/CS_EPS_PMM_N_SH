#include "stm32l4xx.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_usart.h"
#include "uart_comm.h"


/** @brief  Checking flag status any LL function with signature uint32_t LL_SOME_FUNC(I2C_TypeDef *USARTx)
	@param  (*I2C_FRAM_USART_check_flag)(I2C_TypeDef *) - pointer to LL function.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  flag_state - (SET or RESET) value to be compared result of (*USART_check_flag)(I2C_TypeDef *).
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus USART_check_flag(uint32_t (*USART_check_flag)(USART_TypeDef*), USART_TypeDef* USARTx, uint8_t flag_state){
	uint32_t counter =0;

	for( counter = 0; USART_check_flag(USARTx) == flag_state; counter++){
		if(counter == USART_TIMEOUT){

			return ERROR_N;
		}
	}

	return SUCCESS;
}


/** @brief  This function send a byte to the USART .
	* @param  USARTx - pointer to the UART port number.
	* @param  Data - byte to send.
	* @retval  0 -SUCCESS, -1 -ERROR 
	*/
ErrorStatus USART_send_byte(USART_TypeDef* USARTx, uint8_t Data){

	if(USART_check_flag(LL_USART_IsActiveFlag_TC, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_USART_TransmitData8( USARTx, Data );   

	return SUCCESS;
}


/** @brief  This function send a string to the USART.
	* @param  USARTx - pointer to the UART port number.
	* @param  Data - pointer to the send string.
	* @retval  0 -SUCCESS, -1 -ERROR
	*/
ErrorStatus USART_send_string(USART_TypeDef* USARTx, uint8_t *Data){

	if(USART_check_flag(LL_USART_IsActiveFlag_TC, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	/*if(USART_check_flag(LL_USART_IsActiveFlag_TXE, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_USART_TransmitData8( USARTx, '\r' );
	if(USART_check_flag(LL_USART_IsActiveFlag_TXE, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_USART_TransmitData8( USARTx, '\n' );*/
	 
	for(/*Empty*/; *Data != '\0' ; Data++ ){ 

		if(USART_check_flag(LL_USART_IsActiveFlag_TXE, USARTx, RESET) != SUCCESS){
			return ERROR_N;
		}

		LL_USART_TransmitData8( USARTx, *Data );
	}

	if(USART_check_flag(LL_USART_IsActiveFlag_TC, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief  This function send a string to the USART.
	* @param  LPUARTx - pointer to the UART port number.
	* @param  Data - send_array to the send .
	* @retval  0 -SUCCESS, -1 -ERROR
	*/
ErrorStatus USART_send_array(USART_TypeDef* USARTx, uint8_t send_array[], uint32_t size_array){

	uint32_t i = 0;

	if(USART_check_flag(LL_USART_IsActiveFlag_TC, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}
	 
	for( i = 0; i != size_array; i++ ){ 

		if(USART_check_flag(LL_USART_IsActiveFlag_TXE, USARTx, RESET) != SUCCESS){
			return ERROR_N;
		}

		LL_USART_TransmitData8( USARTx, send_array[i] );
	}

	if(USART_check_flag(LL_USART_IsActiveFlag_TC, USARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}



/** @brief  This function send a byte to the LPUSART (low power).
	* @param  LPUARTx - pointer to the UART port number.
	* @param  Data - byte to send.
	* @retval  0 -SUCCESS, -1 -ERROR 
	*/
ErrorStatus LPUART_send_byte(USART_TypeDef* LPUARTx, uint8_t Data){

	if(USART_check_flag(LL_LPUART_IsActiveFlag_TC, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_LPUART_TransmitData8( LPUARTx, Data );   

	return SUCCESS;
}



/** @brief  This function send a string to the LPUART (low power).
	* @param  LPUARTx - pointer to the UART port number.
	* @param  Data - pointer to the send string.
	* @retval  0 -SUCCESS, -1 -ERROR
	*/
ErrorStatus LPUART_send_string(USART_TypeDef* LPUARTx, uint8_t *Data){

	if(USART_check_flag(LL_LPUART_IsActiveFlag_TC, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	/*if(USART_check_flag(LL_LPUART_IsActiveFlag_TXE, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_LPUART_TransmitData8( LPUARTx, '\r' );
	if(USART_check_flag(LL_LPUART_IsActiveFlag_TXE, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_LPUART_TransmitData8( LPUARTx, '\n' );*/
	 
	for(/*Empty*/; *Data != '\0' ; Data++ ){ 

		if(USART_check_flag(LL_LPUART_IsActiveFlag_TXE, LPUARTx, RESET) != SUCCESS){
			return ERROR_N;
		}

		LL_LPUART_TransmitData8( LPUARTx, *Data );
	}

	if(USART_check_flag(LL_LPUART_IsActiveFlag_TC, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}



/** @brief  This function send a string to the LPUART (low power).
	* @param  LPUARTx - pointer to the UART port number.
	* @param  Data - send_array to the send .
	* @retval  0 -SUCCESS, -1 -ERROR
	*/
ErrorStatus LPUART_send_array(USART_TypeDef* LPUARTx, uint8_t send_array[], uint32_t size_array){

	uint32_t i = 0;

	if(USART_check_flag(LL_LPUART_IsActiveFlag_TC, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}
	 
	for( i = 0; i != size_array; i++ ){ 

		if(USART_check_flag(LL_LPUART_IsActiveFlag_TXE, LPUARTx, RESET) != SUCCESS){
			return ERROR_N;
		}

		LL_LPUART_TransmitData8( LPUARTx, send_array[i] );
	}

	if(USART_check_flag(LL_LPUART_IsActiveFlag_TC, LPUARTx, RESET) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}




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

	//if(0 < vsprintf(string,fmt,argp)) // build string
	//{
	//  HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
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
