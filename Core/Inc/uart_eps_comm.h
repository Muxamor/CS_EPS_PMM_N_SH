/*
 * uart_eps_comm.h
 *
 *  Created on: 29 июн. 2020 г.
 *      Author: Ivan
 */

#ifndef INC_UART_EPS_COMM_H_
#define INC_UART_EPS_COMM_H_

ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t sourse_addr,uint8_t package_tag, uint8_t send_data[], uint16_t size_data );

#endif /* INC_UART_EPS_COMM_H_ */
