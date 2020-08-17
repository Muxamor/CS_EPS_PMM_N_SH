
#ifndef INC_UART_EPS_COMM_H_
#define INC_UART_EPS_COMM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/eps_struct.h"

#define UART_EPS_CMD 0x01
#define UART_EPS_ACK 0x02
#define UART_EPS_NFC 0x03

#define UART_EPS_PACK_SIZE_BUFF  512
#define UART_EPS_ERROR_Threshold 3

#define UART_EPS_ACK_TIMEOUT 250 //ms

//Protocol UART EPS
#define UART_EPS_ID_CMD_SAVE_PMM_struct 	0x01
#define UART_EPS_ID_CMD_SAVE_PDM_struct 	0x02
#define UART_EPS_ID_CMD_SAVE_PAM_struct 	0x03
#define UART_EPS_ID_CMD_SAVE_PBM_struct 	0x04
#define UART_EPS_ID_CMD_Get_PMM_struct	 	0x05
#define UART_EPS_ID_CMD_Get_PDM_struct	 	0x06
#define UART_EPS_ID_CMD_Get_PAM_struct 		0x07
#define UART_EPS_ID_CMD_Get_PBM_struct 		0x08
#define UART_EPS_ID_CMD_Get_Reboot_count 	0x09
#define UART_EPS_ID_CMD_Reboot 				0x0A
#define UART_EPS_ID_CMD_Take_CTRL			0x0B
#define UART_EPS_ID_CMD_Ping				0x0C

#define UART_EPS_ID_ACK_SAVE_PMM_struct 	0x01
#define UART_EPS_ID_ACK_SAVE_PDM_struct 	0x02
#define UART_EPS_ID_ACK_SAVE_PAM_struct 	0x03
#define UART_EPS_ID_ACK_SAVE_PBM_struct 	0x04
#define UART_EPS_ID_ACK_Get_PMM_struct	 	0x05
#define UART_EPS_ID_ACK_Get_PDM_struct	 	0x06
#define UART_EPS_ID_ACK_Get_PAM_struct 		0x07
#define UART_EPS_ID_ACK_Get_PBM_struct 		0x08
#define UART_EPS_ID_ACK_Get_Reboot_count	0x09
#define UART_EPS_ID_ACK_Reboot 				0x0A
#define UART_EPS_ID_ACK_Take_CTRL 			0x0B
#define UART_EPS_ID_ACK_Ping				0x0C

#define UART_EPS_ID_NFS_Prep_Take_CTRL 		0x01

typedef struct{

	USART_TypeDef* USARTx;
	uint16_t expected_size_recv_pack;
	uint16_t size_recv_pack;

	uint8_t recv_pack_buf[UART_EPS_PACK_SIZE_BUFF];

	uint8_t uart_unit_addr; 

	uint8_t permit_recv_pack_flag     :1;
	uint8_t stop_recv_pack_flag 	  :1;
	uint8_t recv_pack_timeout_flag 	  :1;
	uint8_t waiting_answer_flag 	  :1; 
	uint8_t :5;

	uint8_t error_port_counter;
	uint8_t Alignment_structure1; //Alignment

}_UART_EPS_COMM;


ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t source_addr, uint8_t package_tag, uint8_t send_data[], uint16_t size_data );
ErrorStatus UART_EPS_Check_CRC_Package( _UART_EPS_COMM *UART_eps_comm );

ErrorStatus UART_EPS_Pars_Get_CMD( _UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p );
ErrorStatus UART_EPS_Pars_Get_ACK( _UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p );
ErrorStatus UART_EPS_Pars_Get_NFC(_UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p );
ErrorStatus UART_EPS_Pars_Get_Package( _UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p );

ErrorStatus UART_EPS_Send_CMD( uint8_t cmd_id, uint8_t choice_uart_port, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p );
ErrorStatus UART_EPS_Send_NFC( uint8_t nfc_id, uint8_t choice_uart_port, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p );
ErrorStatus UART_EPS_Send_ACK ( _UART_EPS_COMM *UART_eps_comm , uint8_t send_data[], uint16_t size_data );

#ifdef __cplusplus
}
#endif

#endif /* INC_UART_EPS_COMM_H_ */
