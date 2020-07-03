
#ifndef INC_UART_EPS_COMM_H_
#define INC_UART_EPS_COMM_H_

#include "pmm_struct.h"
#include "pdm_struct.h"

#define UART_EPS_CMD 0x01
#define UART_EPS_ACK 0x02
#define UART_EPS_NFC 0x03

#define UART_EPS_PACK_SIZE_BUFF  512

//Protocol UART EPS
#define UART_EPS_ID_CMD_SAVE_PMM_struct 	0x01
#define UART_EPS_ID_CMD_SAVE_PDM_struct 	0x02
#define UART_EPS_ID_CMD_SAVE_PAM_struct 	0x03
#define UART_EPS_ID_CMD_SAVE_PBM1_struct 	0x04
#define UART_EPS_ID_CMD_SAVE_PBM2_struct 	0x05
#define UART_EPS_ID_CMD_SAVE_PBM3_struct 	0x06
#define UART_EPS_ID_CMD_Get_Reboot_count 	0x07
#define UART_EPS_ID_CMD_Reboot 				0x08
#define UART_EPS_ID_CMD_Take_CTRL			0x09

#define UART_EPS_ID_ACK_SAVE_PMM_struct 	0x01
#define UART_EPS_ID_ACK_SAVE_PDM_struct 	0x02
#define UART_EPS_ID_ACK_SAVE_PAM_struct 	0x03
#define UART_EPS_ID_ACK_SAVE_PBM1_struct 	0x04
#define UART_EPS_ID_ACK_SAVE_PBM2_struct 	0x05
#define UART_EPS_ID_ACK_SAVE_PBM3_struct 	0x06
#define UART_EPS_ID_ACK_Get_Reboot_count	0x07
#define UART_EPS_ID_ACK_Reboot 				0x08
#define UART_EPS_ID_ACK_Take_CTRL 			0x09

#define UART_EPS_ID_NFS_Prep_Take_CTRL 		0x01

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

	USART_TypeDef* USARTx;

}_UART_EPS_COMM;


ErrorStatus UART_EPS_Pars_Get_Package(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr);
ErrorStatus UART_EPS_Pars_Get_CMD(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr);
ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t sourse_addr,uint8_t package_tag, uint8_t send_data[], uint16_t size_data );

#endif /* INC_UART_EPS_COMM_H_ */
