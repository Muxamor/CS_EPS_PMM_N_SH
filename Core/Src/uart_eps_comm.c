
#include "stm32l4xx.h"
#include "Fn_CRC16.h"
#include "uart_comm.h"
#include "pmm_config.h"
#include "uart_eps_comm.h"


/**********EPS UART Protocol***********/
// 0. (1 byte) 0xAA preamble.
// 1. (1 byte) 0xYY destination address 254- broadcast package.
// 2. (1 byte) 0xYY source address.
// 3. (1 byte) 0xYY Package Tag 0x00-Command, 0x01-Answer, 0x02-Notification.
// 4. (2 byte) 0xYYXX - Data size in package 0..65529.
// 5. (2 byte) 0xYYXX - CRC-16-CCITT all package.
/****************************************/



typedef struct{

	uint16_t size_in_parsel;
	uint16_t expected_size_in_parsel;

	uint8_t in_parsel_buf[256];
	uint8_t uart_unit_addr; 

	uint8_t permit_get_parcel_flag 	:1;
	uint8_t get_parcel_flag 		:1;
	uint8_t get_parsel_timeout_flag :1;
	uint8_t waiting_answer_flag 	:1;
	uint8_t :4;

}_UART_EPS_COMM;



#define UART_EPS_CMD_SAVE_PMM_struct 0x01

extern _UART_EPS_COMM UART_M_eps_comm;


ErrorStatus UART_EPS_Pars_Get_Package(_UART_EPS_COMM *UART_eps_comm){

	uint8_t package_tag = 0;
	uint16_t crc_calc = 0;
	uint16_t get_crc = 0;

	//Not buffer empty nothing to parsing
	if( UART_eps_comm->get_parcel_flag != 1){
		return SUCCESS;
	}

	crc_calc = Crc16_1021( UART_eps_comm->in_parsel_buf, UART_eps_comm->size_in_parsel - 2 );

	get_crc = ( ( (uint16_t)(UART_eps_comm->in_parsel_buf[UART_eps_comm->size_in_parsel - 2]) ) << 8 ) | ( (uint16_t)(UART_eps_comm->in_parsel_buf[UART_eps_comm->size_in_parsel - 1]) );

	if( crc_calc != get_crc ){
		return ERROR_N; //????
		//если команада отправить ошибку в ответ. 
		//Или ничего не отправлять так  не известно что пришло и кому. 
	}

	// Start parsing get packeage 
	package_tag = UART_eps_comm->in_parsel_buf[3];


	if( package_tag == UART_CMD ){

		UART_EPS_Pars_Get_CMD( UART_eps_comm );

	}else if( package_tag == UART_ACK ){

	}else if( package_tag == UART_NFC ){

	}else{
		//Need send error Answer?
		return ERROR_N;
	}

}



ErrorStatus UART_EPS_Pars_Get_CMD(_UART_EPS_COMM *UART_eps_comm){

	uint16_t data_size = 0;
	uint8_t cmd_id = UART_eps_comm->in_parsel_buf[6];

	cmd_id = UART_eps_comm->in_parsel_buf[6];
	data_size = ( ( (uint16_t)(UART_eps_comm->in_parsel_buf[5]) ) << 8 ) | ( (uint16_t)(UART_eps_comm->in_parsel_buf[4]) ) - 1;

		if( CMD_ID == UART_EPS_CMD_SAVE_PMM_struct ){



		}else if(){

		}


}



ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t sourse_addr, uint8_t package_tag, uint8_t send_data[], uint16_t size_data ){

	uint8_t package[256];
	uint16_t size_package = 0;
 	uint8_t i = 0;
 	uint16_t crc = 0;


	//0. 0xAA preamble.
 	package[0x00] = 0xAA;

	//1. Destination address, 254 - broadcast package.
 	package[0x01] = destination_addr;

	//2. Source address.
 	package[0x02] = sourse_addr;

	//3. Package Tag 0x00-Command, 0x01-Answer, 0x02-Notification.
 	package[0x03] = package_tag;
ошибка нетуразмера данных
 	//4. Put send data.
	for(i = 0 ; i < size_data; i++ ){
		package[i+0x04] = send_data[i];
	}
	size_package = 0x04 + size_data;

	//5. Calculate CRC-16-CCITT.
	crc = Crc16_1021( package, size_package );

	size_package = size_package + 1;
	package[size_package] = (uint8_t)(crc>>8);

	size_package = size_package + 1;
	package[size_package] = (uint8_t)(crc);

	if( USARTx == LPUART1){
		return LPUART_send_array( USARTx, package, size_package);
	}else{
		return USART_send_array( USARTx, package, size_package);
	}

}
