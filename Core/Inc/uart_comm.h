

#ifndef INC_UART_COMM_H_
#define INC_UART_COMM_H_


#define USART_TIMEOUT 40000

ErrorStatus USART_check_flag(uint32_t (*USART_check_flag)(USART_TypeDef*), USART_TypeDef* USARTx, uint8_t flag_state);

/** Function for usual USART  */
ErrorStatus USART_send_byte(USART_TypeDef* USARTx, uint8_t Data);
ErrorStatus USART_send_string(USART_TypeDef* USARTx, uint8_t *Data);
ErrorStatus USART_send_arrey(USART_TypeDef* USARTx, uint8_t send_array[], uint32_t size_array);

/** Function for usual LPUART  */
ErrorStatus LPUART_send_byte(USART_TypeDef* LPUARTx, uint8_t Data);
ErrorStatus LPUART_send_string(USART_TypeDef* LPUARTx, uint8_t *Data);
ErrorStatus LPUART_send_arrey(USART_TypeDef* LPUARTx, uint8_t send_array[], uint32_t size_array);


#endif /* INC_UART_COMM_H_ */
