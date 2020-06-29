
#include "stm32l4xx.h"
#include "Fn_CRC16.h"
#include "uart_comm.h"
#include "pmm_config.h"


//typedef struct{
//
//	uint8_t eps_in_buffer[256];
//	uint16_tsize_in_data;


//	uint8_t get_parcel_flag :1;
//	uint8_t waiting_answer_flag :1;
//	uint8_t sending_flag :1
//
//
//	uint8_t Error_UART_main :1
//	uint8_t Error_UART_backup :1
//
//
//}_UART_EPS_COMM;




/**********EPS UART Protocol***********/
// 0. (1 byte) 0xAA preamble.
// 1. (1 byte) 0xYY destination address 254- broadcast package.
// 2. (1 byte) 0xYY source address.
// 3. (1 byte) 0xYY Package Tag 0x00-Command, 0x01-Answer, 0x02-Notification.
// 4. (2 byte) 0xYYXX - Data size in package 0..65529.
// 5. (2 byte) 0xYYXX - CRC-16-CCITT all package.
/****************************************/

ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t sourse_addr,uint8_t package_tag, uint8_t send_data[], uint16_t size_data ){

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
