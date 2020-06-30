
#ifndef INC_UART_EPS_COMM_H_
#define INC_UART_EPS_COMM_H_

#define UART_CMD 0x01
#define UART_NFC 0x02
#define UART_ACK 0x03

#define UART_EPS_PACK_SIZE_BUFF  512

typedef struct{

	uint16_t expected_size_recv_pack;
	uint16_t size_recv_pack;

	uint8_t recv_pack_buf[UART_EPS_PACK_SIZE_BUFF];

	uint8_t uart_unit_addr; 

	uint8_t permit_recv_pack_flag     :1;
	uint8_t stop_recv_pack_flag 	  :1;
	uint8_t recv_pack_timeout_flag 	  :1;//?????
	uint8_t waiting_answer_flag 	  :1;
	uint8_t :4;

}_UART_EPS_COMM;


ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t sourse_addr,uint8_t package_tag, uint8_t send_data[], uint16_t size_data );

#endif /* INC_UART_EPS_COMM_H_ */
