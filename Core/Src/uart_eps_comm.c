
#include "stm32l4xx.h"
#include "Fn_CRC16.h"
#include "pmm_struct.h"
#include "pdm_struct.h"
#include "uart_comm.h"
#include "pmm_config.h"
#include "pmm_init.h"
#include "uart_eps_comm.h"
#include <string.h>

/**********EPS UART Protocol***********/
// 0. (1 byte) 0xAA preamble.
// 1. (1 byte) 0xYY destination address 254- broadcast package.
// 2. (1 byte) 0xYY source address.
// 3. (1 byte) 0xYY Package Tag 0x00-Command, 0x01-Answer, 0x02-Notification.
// 4. (2 byte) 0xYYXX - Data size in package 0..65529.
// 5. (2 byte) 0xYYXX - CRC-16-CCITT all package.
/****************************************/



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

 	//4. Size send data in package
 	send_pack_buf[4] = (uint8_t)( size_data >> 8 );
 	send_pack_buf[5] = (uint8_t)( size_data );

 	//5. Put send data.
	memcpy( &(send_pack_buf[6]), &(send_data[0]), size_data );

	size_package = 6 + size_data;

	//6. Calculate CRC-16-CCITT.
	crc = Crc16_1021( send_pack_buf, size_package );

	send_pack_buf[size_package] = (uint8_t)(crc>>8);
	size_package = size_package + 1;

	send_pack_buf[size_package] = (uint8_t)(crc);
	size_package = size_package + 1;

	if( USARTx == LPUART1){
		return LPUART_send_array( USARTx, send_pack_buf, size_package);
	}else{
		return USART_send_array( USARTx, send_pack_buf, size_package);
	}

}


/** @brief  Check  Crc uart eps  pockage
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Chaeck_CRC_Packag( _UART_EPS_COMM *UART_eps_comm ){
	
	uint16_t crc_calc = 0;
	uint16_t get_crc = 0;

	int8_t error_status = ERROR_N;

	crc_calc = Crc16_1021( UART_eps_comm->recv_pack_buf, UART_eps_comm->size_recv_pack - 2 );

	get_crc = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[UART_eps_comm->size_recv_pack - 2]) ) << 8 ) | ( (uint16_t)(UART_eps_comm->recv_pack_buf[UART_eps_comm->size_recv_pack - 1]) );

	if(get_crc == crc_calc ){
		error_status = SUCCESS;
	}else{
		error_status = ERROR_N;
	}

	return error_status;
}


/** @brief Set error EPS UART ports to pmm_ptr structure.
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.	
	@retval None.
*/
void UART_EPS_Set_Error_ports( _UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr ){

	if( UART_eps_comm->error_port_counter == 0 ){ // Errors in UARTs

		if( UART_eps_comm->USARTx == LPUART1 ){
			pmm_ptr->Error_UART_port_M = SUCCESS;
			
		}else if( UART_eps_comm->USARTx == USART3 ){
			pmm_ptr->Error_UART_port_B = SUCCESS;
		}
		 
	}else{
		
		if(UART_eps_comm->error_port_counter >= UART_EPS_ERROR_Threshold ){ // If there are five errors in a row, then set the UART port error

			if( UART_eps_comm->USARTx == LPUART1 ){
				pmm_ptr->Error_UART_port_M = ERROR;

			}else if( UART_eps_comm->USARTx == USART3 ){
				pmm_ptr->Error_UART_port_B = ERROR;
			}
		}
	}
	
}


/** @brief  Parsing received CMD from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_CMD( _UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr ){

	uint8_t cmd_id = 0;
	//uint8_t data_size = 0;
	uint16_t size_pack_ACK = 0;
	uint8_t pack_ACK_buf[UART_EPS_PACK_SIZE_BUFF];
	uint8_t ACK_Attribute = 0; // 0 - Send Ask ERROR, 1 - Send Ask OK, 2 - Send data; 

	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) - 1);
	cmd_id = UART_eps_comm->recv_pack_buf[6];

	if( cmd_id == UART_EPS_ID_CMD_SAVE_PMM_struct ){
		memcpy(pmm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *pmm_ptr) );
		pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_MasterBackupCPU();
		pmm_ptr->PMM_save_conf_flag = 1; //Save received settings in FRAM 
	
		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PDM_struct ){
		memcpy(pdm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *pdm_ptr) );
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

	}else if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_count ){
		uint32_t reboot_counter = 0;

		if( pmm_ptr->Main_Backup_mode_CPU == 0 ){
			reboot_counter = pmm_ptr->reboot_counter_CPUm; 
		}else{
			reboot_counter = pmm_ptr->reboot_counter_CPUb; 
		}

		pack_ACK_buf[0] = UART_EPS_ID_CMD_Get_Reboot_count;
		pack_ACK_buf[1] = (uint8_t)(reboot_counter>>24);
		pack_ACK_buf[2] = (uint8_t)(reboot_counter>>16);
		pack_ACK_buf[3] = (uint8_t)(reboot_counter>>8);
		pack_ACK_buf[4] = (uint8_t)(reboot_counter);

		size_pack_ACK = 5;
		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Reboot ){
		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Take_CTRL ){
		ACK_Attribute = 1;

	}else{
		ACK_Attribute = 0;
	}


	if( ACK_Attribute == 0 ){ // Sends an ERROR in response
		pack_ACK_buf[0] = cmd_id;
		pack_ACK_buf[1] = 0x00; 
		size_pack_ACK = 2; 

	}else if( ACK_Attribute == 1 ){ //Sends an OK in response
		pack_ACK_buf[0] = cmd_id;
		pack_ACK_buf[1] = 0x01; 
		size_pack_ACK = 2;

	}else if( ACK_Attribute == 2 ){ //Sends an DATA in response
		//Empty
	}

	UART_eps_comm->permit_recv_pack_flag = 0;
	UART_eps_comm->stop_recv_pack_flag = 0;

	return UART_EPS_Send_ACK ( UART_eps_comm,  pack_ACK_buf, size_pack_ACK );
}


/** @brief  Parsing received ACK from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_ACK(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr ){

	uint8_t cmd_id = 0;
	//uint8_t data_size = 0;
	int8_t error_status = SUCCESS;

	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) - 1);
	cmd_id = UART_eps_comm->recv_pack_buf[6];

	// UART_EPS_ID_CMD_SAVE_PMM_struct
	// UART_EPS_ID_CMD_SAVE_PDM_struct
	// UART_EPS_ID_CMD_SAVE_PBM1_struct
	// UART_EPS_ID_CMD_SAVE_PBM2_struct
	// UART_EPS_ID_CMD_SAVE_PBM3_struct
	// UART_EPS_ID_CMD_Reboot
	// UART_EPS_ID_CMD_Take_CTRL
	// UART_EPS_ID_CMD_Ping

	if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_count ){

		uint32_t reboot_counter = 0;
		reboot_counter = *(uint32_t * )(&UART_eps_comm->recv_pack_buf[7]);
		// (((uint32_t)UART_eps_comm->recv_pack_buf[7]) << 24) | (((uint32_t)UART_eps_comm->recv_pack_buf[8]) << 16) | (((uint32_t)UART_eps_comm->recv_pack_buf[9]) << 8) | ((uint32_t)UART_eps_comm->recv_pack_buf[10]);
		
		if( pmm_ptr->Main_Backup_mode_CPU == 0 ){
			pmm_ptr->reboot_counter_CPUm = reboot_counter; 
		}else{
			pmm_ptr->reboot_counter_CPUb = reboot_counter; 
		}

	}else{ 
		
		if (UART_eps_comm->recv_pack_buf[7] == 0x00 ) { // In answer Error
			error_status = ERROR_N;
		}
	}

	UART_eps_comm->waiting_answer_flag = 0; 
	UART_eps_comm->permit_recv_pack_flag = 0;
	UART_eps_comm->stop_recv_pack_flag = 0;

	return error_status;
}



/** @brief  Parsing received notification NFC from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_NFC(_UART_EPS_COMM *UART_eps_comm ){

	uint8_t nfc_id = 0;
	//uint8_t data_size = 0;
	int8_t error_status = SUCCESS;

	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) - 1);
	nfc_id = UART_eps_comm->recv_pack_buf[6];

	if( nfc_id ==  UART_EPS_ID_NFS_Prep_Take_CTRL ){
		//TO DO write  processing
	}else{ 
		
	}

	UART_eps_comm->permit_recv_pack_flag = 0;
	UART_eps_comm->stop_recv_pack_flag = 0;

	return error_status;
}


/** @brief  Parsing received CMD, ACK and NFC package from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_Package(_UART_EPS_COMM *UART_eps_comm, _PMM *pmm_ptr, _PDM *pdm_ptr){

	uint8_t package_tag = 0;
	
	int8_t error_status = ERROR_N;

	//Not get data, nothing to parsing. Protection
	if( UART_eps_comm->stop_recv_pack_flag != 1){
		return SUCCESS;
	}

	//Check CRC get pakage
	error_status = UART_EPS_Chaeck_CRC_Packag( UART_eps_comm ); 

	// Start parsing get packeage 
	//Get Package ID tag (CMD, ACK, NFC)
	if( error_status == SUCCESS ){

		package_tag = UART_eps_comm->recv_pack_buf[3];

		// Parsing CMD, ACK or NFC. 
		if( package_tag == UART_EPS_CMD ){
			error_status = UART_EPS_Pars_Get_CMD( UART_eps_comm, pmm_ptr, pdm_ptr );

		}else if( package_tag == UART_EPS_ACK /* && UART_eps_comm->waiting_answer_flag == 1 */){
			error_status = UART_EPS_Pars_Get_ACK( UART_eps_comm, pmm_ptr );

		}else if( package_tag == UART_EPS_NFC ){
			error_status = UART_EPS_Pars_Get_NFC( UART_eps_comm );

		}else{
			//Not recognize package TAG.
			UART_eps_comm->permit_recv_pack_flag = 0;
			UART_eps_comm->stop_recv_pack_flag = 0;
		}
	}

	//Set error flag to PMM struct
	if(error_status == SUCCESS ){
		UART_eps_comm->error_port_counter = 0;
	}else{
		UART_eps_comm->error_port_counter++;
	}

	//UART_EPS_Set_Error_ports( UART_eps_comm, pmm_ptr ); 

	return error_status;
}


/** @brief Send CMD.
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  cmd_id - Command ID.
	@param  tuart_port - Set the port through which to send the command. 0 - Auto fault port detection, 1 - Main UART port, 2 - Backup UART port.
	@param  send_data[] - send data massive.
	@param 	size_data -  size sending data.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_CMD( uint8_t cmd_id, uint8_t choice_uart_port, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _PMM *pmm_ptr,  _PDM *pdm_ptr ){

	uint8_t send_buf[UART_EPS_PACK_SIZE_BUFF];
	uint16_t size_send_data = 0;
	uint16_t size_struct = 0;
	uint8_t destination_addr = 0;
	uint8_t sourse_addr = 0;
	uint32_t timeout_counter = 0;
	int8_t error_status = ERROR_N;
	
	_UART_EPS_COMM *UART_X_eps_comm;


	if( cmd_id == UART_EPS_ID_CMD_SAVE_PMM_struct ){
		
		send_buf[0] = UART_EPS_ID_CMD_SAVE_PMM_struct;

		size_struct = sizeof( *pmm_ptr);
		memcpy( (&(send_buf[1])), pmm_ptr, size_struct );
		size_send_data = size_struct + 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PDM_struct ){

		send_buf[0] = UART_EPS_ID_CMD_SAVE_PDM_struct;

		size_struct = sizeof( *pdm_ptr);
		memcpy( (&(send_buf[1])), pdm_ptr, size_struct );
		size_send_data = size_struct + 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PAM_struct ){
	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM1_struct ){
	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM2_struct ){
	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM3_struct ){
	}else if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_count ){

		send_buf[0] = UART_EPS_ID_CMD_Get_Reboot_count;
		size_send_data = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Reboot ){
	}else if( cmd_id == UART_EPS_ID_CMD_Take_CTRL ){

	}else if( cmd_id == UART_EPS_ID_CMD_Ping ){

		send_buf[0] = UART_EPS_ID_CMD_Ping;
		size_send_data = 1;
	}else{
		return ERROR_N;
	}

	//Choice UART port Main, Backup or Auto detect.
	if( choice_uart_port == 1 ){ // Set Main UART port 
		UART_X_eps_comm = UART_Main_eps_comm;

	}else if( choice_uart_port == 2 ){ //Set Backup UART port 
		UART_X_eps_comm = UART_Backup_eps_comm;

	}else{//Set auto detect UART port 
		if( pmm_ptr->Error_UART_port_M == SUCCESS ){
			UART_X_eps_comm = UART_Main_eps_comm;
		}else{ //Error UART_port_M
			UART_X_eps_comm = UART_Backup_eps_comm;
		}
	}		

	//Set source and destination address.
	if( pmm_ptr->Main_Backup_mode_CPU == 0 ){
		sourse_addr = UART_X_eps_comm->uart_unit_addr;
		destination_addr = UART_EPS_CPUb_Addr;
	}else{
		sourse_addr = UART_X_eps_comm->uart_unit_addr; ;
		destination_addr = UART_EPS_CPUm_Addr;
	}

	error_status = ERROR_N;

	//Cleaar input EPS UART Buffer.
	UART_EPS_Pars_Get_Package( UART_X_eps_comm, pmm_ptr, pdm_ptr);
	
	//Send a command and wait for an answer.
	if( UART_EPS_Send_Package( UART_X_eps_comm->USARTx, destination_addr,  sourse_addr, UART_EPS_CMD, send_buf, size_send_data) == SUCCESS ){

		UART_X_eps_comm->waiting_answer_flag = 1;
		timeout_counter = 0;

		while( UART_X_eps_comm->waiting_answer_flag != 0 ){ //waiting_answer_flag - The flag should be reset in the function UART_EPS_Pars_Get_Package when a response is received.

			timeout_counter++;

			if ( timeout_counter == UART_EPS_ACK_TIMEOUT ){
				UART_X_eps_comm->waiting_answer_flag = 0;
				UART_X_eps_comm->error_port_counter++;
				error_status = ERROR_N;
				break;
			}

			if( UART_X_eps_comm->stop_recv_pack_flag == 1){ //Response processing
				error_status = UART_EPS_Pars_Get_Package( UART_X_eps_comm, pmm_ptr, pdm_ptr);
			}
		}
	}

	if( error_status == SUCCESS ){
		UART_X_eps_comm->error_port_counter = 0;
	}

 	return error_status;
}


/** @brief Send NFC.
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  cmd_id - Command ID.
	@param  tuart_port - Set the port through which to send the command. 0 - Auto fault port detection, 1 - Main UART port, 2 - Backup UART port.
	@param  send_data[] - send data massive.
	@param 	size_data -  size sending data.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_NFC( uint8_t nfc_id, uint8_t choice_uart_port, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _PMM *pmm_ptr ){

	uint8_t send_buf[UART_EPS_PACK_SIZE_BUFF];
	uint16_t size_send_data = 0;
	uint8_t destination_addr = 0;
	uint8_t sourse_addr = 0;
	int8_t error_status = ERROR_N;
	_UART_EPS_COMM *UART_X_eps_comm;

	if( nfc_id == UART_EPS_ID_NFS_Prep_Take_CTRL ){

		send_buf[0] = UART_EPS_ID_NFS_Prep_Take_CTRL;
		size_send_data = 1;

	}else{
		return ERROR_N;
	}

	//Choice UART port Main, Backup or Auto detect.
	if( choice_uart_port == 1 ){ // Set Main UART port 
		UART_X_eps_comm = UART_Main_eps_comm;

	}else if( choice_uart_port == 2 ){ //Set Backup UART port 
		UART_X_eps_comm = UART_Backup_eps_comm;

	}else{//Set auto detect UART port 
		if( pmm_ptr->Error_UART_port_M == SUCCESS ){
			UART_X_eps_comm = UART_Main_eps_comm;
		}else{ //Error UART_port_M
			UART_X_eps_comm = UART_Backup_eps_comm;
		}
	}		

	//Set source and destination address.
	if( pmm_ptr->Main_Backup_mode_CPU == 0 ){
		sourse_addr = CPUmain; 
		destination_addr = CPUbackup;
	}else{
		sourse_addr = CPUbackup;
		destination_addr = CPUmain; 
	}

	error_status = ERROR_N;

	error_status = UART_EPS_Send_Package( UART_X_eps_comm->USARTx, destination_addr,  sourse_addr, UART_EPS_NFC, send_buf, size_send_data);

	if( error_status == ERROR_N ){
		UART_X_eps_comm->error_port_counter++;
	}else{
		UART_X_eps_comm->error_port_counter = 0;
	}

 	return error_status;
}


/** @brief Send acknowledgment.
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  send_data[] - send data massive.
	@param 	size_data -  size sending data.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_ACK ( _UART_EPS_COMM *UART_eps_comm , uint8_t send_data[], uint16_t size_data ){
	return UART_EPS_Send_Package( UART_eps_comm->USARTx, UART_eps_comm->recv_pack_buf[2], UART_eps_comm->recv_pack_buf[1], UART_EPS_ACK, send_data, size_data );
}


