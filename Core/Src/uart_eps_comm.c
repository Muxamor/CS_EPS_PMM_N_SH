
#include "stm32l4xx.h"
#include "Fn_CRC16.h"
#include "pmm_struct.h"
#include "pdm_struct.h"
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

/** @brief  Parsing received CMD, ACK and NFC package from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_Package(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr){

	uint8_t package_tag = 0;
	uint16_t crc_calc = 0;
	uint16_t get_crc = 0;
	int8_t error_stutus = SUCCESS;

	//Not get data, nothing to parsing.
	if( UART_eps_comm->stop_recv_pack_flag != 1){
		return SUCCESS;
	}

	//Сalculate CRC received packetю
	crc_calc = Crc16_1021( UART_eps_comm->recv_pack_buf, UART_eps_comm->size_recv_pack - 2 );

	get_crc = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[UART_eps_comm->size_recv_pack - 2]) ) << 8 ) | ( (uint16_t)(UART_eps_comm->recv_pack_buf[UART_eps_comm->size_recv_pack - 1]) );

	if(get_crc != crc_calc ){

		uint8_t error_ask = 0;
		UART_EPS_Send_Package( UART_eps_comm->USARTx, UART_eps_comm->recv_pack_buf[2], UART_eps_comm->recv_pack_buf[1], UART_EPS_ACK, &error_ask, 1 );

		if( UART_eps_comm->USARTx == LPUART1 ){
			pmm_ptr->Error_UART_M = ERROR;
		}else if( UART_eps_comm->USARTx == USART3 ){
			pmm_ptr->Error_UART_B = ERROR;
		}

		error_stutus = ERROR_N;
	}

	// Start parsing get packeage 
	//Get Package ID tag (CMD, ACK, NFC)
	if( error_stutus == SUCCESS ){

		package_tag = UART_eps_comm->recv_pack_buf[3];

		// Parsing CMD, ACK or NFC. 
		if( package_tag == UART_EPS_CMD && UART_eps_comm->waiting_answer_flag == 0){
			error_stutus = UART_EPS_Pars_Get_CMD( UART_eps_comm, pmm_ptr, pdm_ptr );

		}else if( package_tag == UART_EPS_ACK && UART_eps_comm->waiting_answer_flag == 1 ){

		}else if( package_tag == UART_EPS_NFC && UART_eps_comm->waiting_answer_flag == 0 ){

		}else{
			//Need send error Answer?
			error_stutus = ERROR_N;
		}
	}

	return error_stutus;
}


/** @brief  Parsing received CMD from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_CMD(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr){

	uint8_t cmd_id = 0;
	//uint8_t data_size = 0;
	uint16_t size_pack_ACK = 0;
	uint8_t pack_ACK_buf[UART_EPS_PACK_SIZE_BUFF];
	uint8_t ACK_Attribute = 0; // 0 - Send Ask ERROR, 1 - Send Ask OK, 2 - Send data; 

	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) - 1);
	cmd_id = UART_eps_comm->recv_pack_buf[6];

	if( cmd_id == UART_EPS_ID_CMD_SAVE_PMM_struct ){

		*pmm_ptr = *(_PMM *)( &(UART_eps_comm->recv_pack_buf[7]) );
		pmm_ptr->PMM_save_conf_flag = 1; //Save received settings in FRAM 

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PDM_struct ){

		*pdm_ptr = *(_PDM *)( &(UART_eps_comm->recv_pack_buf[7]) );
		pdm_ptr->PDM_save_conf_flag = 1; //Save received settings in FRAM 

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PAM_struct ){

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM1_struct ){

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM2_struct ){

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM3_struct ){

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_coun ){

		uint32_t reboot_counter = 0;
		if( pmm_ptr->Main_Backup_mode_CPU == 0 ){
			reboot_counter = pmm_ptr->reboot_counter_CPUm; 
		}else{
			reboot_counter = pmm_ptr->reboot_counter_CPUb; 
		}

		pack_ACK_buf[0] = (uint8_t)(reboot_counter>>24);
		pack_ACK_buf[1] = (uint8_t)(reboot_counter>>16);
		pack_ACK_buf[2] = (uint8_t)(reboot_counter>>8);
		pack_ACK_buf[3] = (uint8_t)(reboot_counter); 

		size_pack_ACK = 4; 

		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Reboot ){

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Take_CTRL ){

		ACK_Attribute = 1;

	}else{
		ACK_Attribute = 0;
	}


	if( ACK_Attribute == 0 ){ // Sends an ERROR in response
		pack_ACK_buf[0] = 0x00; 
		size_pack_ACK = 1; 

	}else if( ACK_Attribute == 1 ){ //Sends an OK in response
		pack_ACK_buf[0] = 0x01; 
		size_pack_ACK = 1; 

	}else if( ACK_Attribute == 2 ){ //Sends an DATA in response
		//Empty
	}
	
	return UART_EPS_Send_Package( UART_eps_comm->USARTx, UART_eps_comm->recv_pack_buf[2], UART_eps_comm->recv_pack_buf[1], UART_EPS_ACK, pack_ACK_buf, size_pack_ACK );
}


/** @brief  Parsing received ACK from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
// ErrorStatus UART_EPS_Pars_Get_ACK(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr){

// 	uint8_t cmd_id = 0;
// 	//uint8_t data_size = 0;

// 	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) - 1);
// 	cmd_id = UART_eps_comm->recv_pack_buf[6];

// 	if( cmd_id == UART_EPS_ID_CMD_SAVE_PMM_struct ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PDM_struct ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PAM_struct ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM1_struct ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM2_struct ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM3_struct ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_coun ){

// 		uint32_t reboot_counter = 0;
// 		reboot_counter = (((uint32_t)UART_eps_comm->recv_pack_buf[7]) << 24) | (((uint32_t)UART_eps_comm->recv_pack_buf[8]) << 16) | \
// 							(((uint32_t)UART_eps_comm->recv_pack_buf[9]) << 8) | ((uint32_t)UART_eps_comm->recv_pack_buf[10]);
		
// 		if( pmm_ptr->Main_Backup_mode_CPU == 0 ){
// 			pmm_ptr->reboot_counter_CPUm = reboot_counter; 
// 		}else{
// 			pmm_ptr->reboot_counter_CPUb = reboot_counter; 
// 		}

// 	}else if( cmd_id == UART_EPS_ID_CMD_Reboot ){

// 	}else if( cmd_id == UART_EPS_ID_CMD_Take_CTRL ){

// 	}else{
// 		//???????????!!!!!!!!!!!!
// 	}

// 	UART_eps_comm->waiting_answer_flag = 0; 
// 	//TO DO Stop counter whait answer;

// 	return SUCCESS;
// }


/** @brief  Preparing and sending a packet on the UART in accordance with the protocol.
	@param  *USARTx - pointer to UART port.
	@param  destination_addr - destination address.
	@param  sourse_addr - source address.
	@param  package_tag - package tag CMD, ACK or NFC.
	@param  send_data[] - send data massive.
	@param 	size_data -  size sending data.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t sourse_addr, uint8_t package_tag, uint8_t send_data[], uint16_t size_data ){

	uint8_t send_pack_buf[UART_EPS_PACK_SIZE_BUFF];
	uint16_t size_package = 0;
 	uint8_t i = 0;
 	uint16_t crc = 0;

 	if( size_data > UART_EPS_PACK_SIZE_BUFF ){
 		return ERROR_N;
 	}


	//0. 0xAA preamble.
 	send_pack_buf[0] = 0xAA;

	//1. Destination address, 254 - broadcast package.
 	send_pack_buf[1] = destination_addr;

	//2. Source address.
 	send_pack_buf[2] = sourse_addr;

	//3. Package Tag 0x00-Command, 0x01-Answer, 0x02-Notification.
 	send_pack_buf[3] = package_tag;

 	//4. Package Tag 0x00-Command, 0x01-Answer, 0x02-Notification.
 	send_pack_buf[4] = (uint8_t)( size_data >> 8 );
 	send_pack_buf[5] = (uint8_t)( size_data );

 	//5. Put send data.
	for(i = 0 ; i < size_data; i++ ){
		send_pack_buf[i + 6] = send_data[i];
	}

	size_package = 6 + size_data;

	//6. Calculate CRC-16-CCITT.
	crc = Crc16_1021( send_pack_buf, size_package );

	size_package = size_package + 1;
	send_pack_buf[size_package] = (uint8_t)(crc>>8);

	size_package = size_package + 1;
	send_pack_buf[size_package] = (uint8_t)(crc);

	if( USARTx == LPUART1){
		return LPUART_send_array( USARTx, send_pack_buf, size_package);
	}else{
		return USART_send_array( USARTx, send_pack_buf, size_package);
	}

}
