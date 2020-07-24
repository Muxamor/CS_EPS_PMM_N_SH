
#include <string.h>
#include "stm32l4xx.h"
#include "Error_Handler.h"
#include "Fn_CRC16.h"
#include "uart_comm.h"
#include "PMM/eps_struct.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_sw_cpu.h"
#include "PBM/pbm_config.h"
#include "uart_eps_comm.h"

extern uint32_t SysTick_Counter;
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
	@param  source_addr - source address.
	@param  package_tag - package tag CMD, ACK or NFC.
	@param  send_data[] - send data massive.
	@param 	size_data -  size sending data.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_Package( USART_TypeDef* USARTx, uint8_t destination_addr, uint8_t source_addr, uint8_t package_tag, uint8_t send_data[], uint16_t size_data ){

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
 	send_pack_buf[2] = source_addr;

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


/** @brief  Check CRC UART EPS package
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Check_CRC_Package( _UART_EPS_COMM *UART_eps_comm ){
	
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
	@param  eps_p - contain pointer to struct which contain all parameters EPS.	
	@retval None.
*/
void UART_EPS_Set_Error_ports( _UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p ){

	if( UART_eps_comm->error_port_counter == 0 ){ // Errors in UARTs

		if( UART_eps_comm->USARTx == LPUART1 ){
			eps_p.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
			
		}else if( UART_eps_comm->USARTx == USART3 ){
			eps_p.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
		}
		 
	}else{
		
		if(UART_eps_comm->error_port_counter >= UART_EPS_ERROR_Threshold ){ // If there are five errors in a row, then set the UART port error

			if( UART_eps_comm->USARTx == LPUART1 ){
				eps_p.eps_pmm_ptr->Error_UART_port_M = ERROR;

			}else if( UART_eps_comm->USARTx == USART3 ){
				eps_p.eps_pmm_ptr->Error_UART_port_B = ERROR;
			}
		}
	}
	
}


/** @brief  Parsing received CMD from UART EPS port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.	
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_CMD( _UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p ){

	uint8_t cmd_id = 0;
	uint16_t size_ACK = 0;
	uint8_t ACK_buf[UART_EPS_PACK_SIZE_BUFF];
	uint8_t ACK_Attribute = 0; // 0 - Send Ask ERROR, 1 - Send Ask OK, 2 - Send data; 

	//uint8_t data_size = 0;
	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) - 1);

	cmd_id = UART_eps_comm->recv_pack_buf[6];

	if( cmd_id == UART_EPS_ID_CMD_SAVE_PMM_struct ){
		
		//Save reboot counter value.
		uint32_t reboot_counter = 0;
		if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){
			reboot_counter = eps_p.eps_pmm_ptr->reboot_counter_CPUm;
		}else{
			reboot_counter = eps_p.eps_pmm_ptr->reboot_counter_CPUb;
		}

		memcpy( eps_p.eps_pmm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *(eps_p.eps_pmm_ptr) ) );
		eps_p.eps_pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_MasterBackupCPU();
		eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1; //Save received settings in FRAM 

		//Save restore counter value.
		if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){
			eps_p.eps_pmm_ptr->reboot_counter_CPUm = reboot_counter;
		}else{
			eps_p.eps_pmm_ptr->reboot_counter_CPUb = reboot_counter;
		}

		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PDM_struct ){
		memcpy( eps_p.eps_pdm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *(eps_p.eps_pdm_ptr) ) );
		eps_p.eps_pdm_ptr->PDM_save_conf_flag = 1; //Save received settings in FRAM 
		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PAM_struct ){
		memcpy( eps_p.eps_pam_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *(eps_p.eps_pam_ptr) ) );
		eps_p.eps_pam_ptr->PAM_save_conf_flag = 1; //Save received settings in FRAM 
		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM_struct ){
		memcpy( eps_p.eps_pbm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), ( sizeof( eps_p.eps_pbm_ptr[0] ) * PBM_QUANTITY ) );
		eps_p.eps_pbm_ptr->PBM_save_conf_flag = 1; //Save received settings in FRAM
		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_PMM_struct ){
		uint16_t size_data = 0;
		size_data = sizeof( *(eps_p.eps_pmm_ptr) );
		ACK_buf[0] = UART_EPS_ID_ACK_Get_PMM_struct;
		memcpy( &(ACK_buf[1]), eps_p.eps_pmm_ptr, size_data );
		size_ACK = size_data + 1;
		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_PDM_struct ){
		uint16_t size_data = 0;
		size_data = sizeof( *(eps_p.eps_pdm_ptr) );
		ACK_buf[0] = UART_EPS_ID_ACK_Get_PDM_struct;
		memcpy( &(ACK_buf[1]), eps_p.eps_pdm_ptr, size_data );
		size_ACK = size_data + 1;
		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_PAM_struct ){
		uint16_t size_data = 0;
		size_data = sizeof( *(eps_p.eps_pam_ptr) );
		ACK_buf[0] = UART_EPS_ID_ACK_Get_PAM_struct;
		memcpy( &(ACK_buf[1]), eps_p.eps_pam_ptr, size_data );
		size_ACK = size_data + 1;
		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_PBM_struct ){
		uint16_t size_data = 0;
		size_data = sizeof( eps_p.eps_pbm_ptr[0]) * PBM_QUANTITY;
		ACK_buf[0] = UART_EPS_ID_ACK_Get_PBM_struct;
		memcpy( &(ACK_buf[1]), eps_p.eps_pbm_ptr, size_data );
		size_ACK = size_data + 1;
		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_count ){
		uint32_t reboot_counter = 0;

		if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){
			reboot_counter = eps_p.eps_pmm_ptr->reboot_counter_CPUm; 
		}else{
			reboot_counter = eps_p.eps_pmm_ptr->reboot_counter_CPUb; 
		}

		ACK_buf[0] = UART_EPS_ID_ACK_Get_Reboot_count;
		ACK_buf[1] = (uint8_t)(reboot_counter);
		ACK_buf[2] = (uint8_t)(reboot_counter>>8);
		ACK_buf[3] = (uint8_t)(reboot_counter>>16);
		ACK_buf[4] = (uint8_t)(reboot_counter>>24);

		size_ACK = 5;
		ACK_Attribute = 2;

	}else if( cmd_id == UART_EPS_ID_CMD_Reboot ){
		ACK_Attribute = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Take_CTRL ){
		PMM_Set_mode_Active_CPU( eps_p );
		ACK_Attribute = 1;

	}else{
		ACK_Attribute = 0;
	}


	if( ACK_Attribute == 0 ){ // Sends an ERROR in response
		ACK_buf[0] = cmd_id;
		ACK_buf[1] = 0x00;
		size_ACK = 2;

	}else if( ACK_Attribute == 1 ){ //Sends an OK in response
		ACK_buf[0] = cmd_id;
		ACK_buf[1] = 0x01;
		size_ACK = 2;

	}else if( ACK_Attribute == 2 ){ //Sends an DATA in response
		//Empty
	}

	UART_eps_comm->permit_recv_pack_flag = 0;
	UART_eps_comm->stop_recv_pack_flag = 0;

	return UART_EPS_Send_ACK ( UART_eps_comm,  ACK_buf, size_ACK );
}


/** @brief  Parsing received ACK from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.	
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_ACK(_UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p ){

	uint8_t cmd_id = 0;
	int8_t error_status = SUCCESS;

	//uint8_t data_size = 0;
	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) - 1);

	cmd_id = UART_eps_comm->recv_pack_buf[6];

	if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_count ){

		uint32_t reboot_counter = 0;
		reboot_counter = *(uint32_t*)(&(UART_eps_comm->recv_pack_buf[7]));
			//	(((uint32_t)UART_eps_comm->recv_pack_buf[7]) << 24) | (((uint32_t)UART_eps_comm->recv_pack_buf[8]) << 16)  (((uint32_t)UART_eps_comm->recv_pack_buf[9]) << 8) | ((uint32_t)UART_eps_comm->recv_pack_buf[10]);
		
		if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){
			eps_p.eps_pmm_ptr->reboot_counter_CPUb = reboot_counter;
		}else{
			eps_p.eps_pmm_ptr->reboot_counter_CPUm = reboot_counter;
		}

	}else if( cmd_id == UART_EPS_ID_ACK_Get_PMM_struct	 ){
		memcpy( eps_p.eps_pmm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *(eps_p.eps_pmm_ptr) ) );

	}else if( cmd_id == UART_EPS_ID_ACK_Get_PDM_struct	 ){
		memcpy( eps_p.eps_pdm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *(eps_p.eps_pdm_ptr) ) );

	}else if( cmd_id == UART_EPS_ID_ACK_Get_PAM_struct	 ){
		memcpy( eps_p.eps_pam_ptr, (&(UART_eps_comm->recv_pack_buf[7])), sizeof( *(eps_p.eps_pam_ptr) ) );

	}else if( cmd_id == UART_EPS_ID_ACK_Get_PBM_struct	 ){
		memcpy( eps_p.eps_pbm_ptr, (&(UART_eps_comm->recv_pack_buf[7])), ( sizeof( eps_p.eps_pbm_ptr[0] ) * PBM_QUANTITY ) );
		
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
	@param  eps_p - contain pointer to struct which contain all parameters EPS.	
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_NFC(_UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p ){

	uint8_t nfc_id = 0;
	int8_t error_status = SUCCESS; //TO DO

	//uint8_t data_size = 0;
	//data_size = ( ( (uint16_t)(UART_eps_comm->recv_pack_buf[5]) ) << 8 ) | (( (uint16_t)(UART_eps_comm->recv_pack_buf[4]) ) - 1);
	nfc_id = UART_eps_comm->recv_pack_buf[6];

	if( nfc_id == UART_EPS_ID_NFS_Prep_Take_CTRL ){
		//TO DO write  processing
	}else{ 
		
	}

	UART_eps_comm->permit_recv_pack_flag = 0;
	UART_eps_comm->stop_recv_pack_flag = 0;

	return error_status;
}


/** @brief  Parsing received CMD, ACK and NFC package from UART port. (Internal exchange between main and backup CPU).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Pars_Get_Package(_UART_EPS_COMM *UART_eps_comm, _EPS_Param eps_p ){

	uint8_t package_tag = 0;
	
	int8_t error_status = ERROR_N;

	//Not get data, nothing to parsing. Protection
	if( UART_eps_comm->stop_recv_pack_flag != 1){
		return SUCCESS;
	}

	//Check CRC get package
	error_status = UART_EPS_Check_CRC_Package( UART_eps_comm );

	// Start parsing get package
	//Get Package ID tag (CMD, ACK, NFC)
	if( error_status == SUCCESS ){

		package_tag = UART_eps_comm->recv_pack_buf[3];

		// Parsing CMD, ACK or NFC. 
		if( package_tag == UART_EPS_CMD ){
			error_status = UART_EPS_Pars_Get_CMD( UART_eps_comm, eps_p );

		}else if( package_tag == UART_EPS_ACK /* && UART_eps_comm->waiting_answer_flag == 1 */){
			error_status = UART_EPS_Pars_Get_ACK( UART_eps_comm, eps_p );

		}else if( package_tag == UART_EPS_NFC ){
			error_status = UART_EPS_Pars_Get_NFC( UART_eps_comm, eps_p );

		}else{
			//Not recognize package tag.
			UART_eps_comm->permit_recv_pack_flag = 0;
			UART_eps_comm->stop_recv_pack_flag = 0;
		}
	}

	//Set error flag to PMM struct
	if(error_status == SUCCESS ){
		UART_eps_comm->error_port_counter = 0;
	}else{
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
		UART_eps_comm->error_port_counter++;
	}

	//UART_EPS_Set_Error_ports( UART_eps_comm, pmm_ptr ); 

	return error_status;
}


/** @brief  Send CMD command.
	@param  cmd_id - Command ID
	@param  choice_uart_por - Set the port through which to send the command. 0 - Auto fault port detection, 1 - Main UART port, 2 - Backup UART port.
	@param  *UART_Main_eps_comm - pointer to main UART port struct with get data.
	@param  *UART_Backup_eps_comm - pointer to backup UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_CMD( uint8_t cmd_id, uint8_t choice_uart_port, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

	uint8_t send_buf[UART_EPS_PACK_SIZE_BUFF];
	uint16_t size_send_data = 0;
	uint16_t size_struct = 0;
	uint8_t destination_addr = 0;
	uint8_t source_addr = 0;
	uint32_t timeout_counter = 0;
	int8_t error_status = ERROR_N;
	
	_UART_EPS_COMM *UART_X_eps_comm;


	if( cmd_id == UART_EPS_ID_CMD_SAVE_PMM_struct ){
		
		send_buf[0] = UART_EPS_ID_CMD_SAVE_PMM_struct;
		size_struct = sizeof( *(eps_p.eps_pmm_ptr) );
		memcpy( (&(send_buf[1])), eps_p.eps_pmm_ptr, size_struct );
		size_send_data = size_struct + 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PDM_struct ){

		send_buf[0] = UART_EPS_ID_CMD_SAVE_PDM_struct;
		size_struct = sizeof( *(eps_p.eps_pdm_ptr) );
		memcpy( (&(send_buf[1])), eps_p.eps_pdm_ptr, size_struct );
		size_send_data = size_struct + 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PAM_struct ){

		send_buf[0] = UART_EPS_ID_CMD_SAVE_PAM_struct;
		size_struct = sizeof( *(eps_p.eps_pam_ptr) );
		memcpy( (&(send_buf[1])), eps_p.eps_pam_ptr, size_struct );
		size_send_data = size_struct + 1;

	}else if( cmd_id == UART_EPS_ID_CMD_SAVE_PBM_struct ){
		send_buf[0] = UART_EPS_ID_CMD_SAVE_PBM_struct;
		size_struct = sizeof( eps_p.eps_pbm_ptr[0] ) * PBM_QUANTITY;
		memcpy( (&(send_buf[1])),eps_p.eps_pbm_ptr, size_struct );
		size_send_data = size_struct + 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_PMM_struct ){
		 send_buf[0] = UART_EPS_ID_CMD_Get_PMM_struct;
		 size_send_data = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Get_Reboot_count ){
		send_buf[0] = UART_EPS_ID_CMD_Get_Reboot_count;
		size_send_data = 1;

	}else if( cmd_id == UART_EPS_ID_CMD_Reboot ){
		return ERROR_N; //Temporarily until the command is implemented
	}else if( cmd_id == UART_EPS_ID_CMD_Take_CTRL ){
		send_buf[0] = UART_EPS_ID_CMD_Take_CTRL;
		size_send_data = 1;

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
		if( eps_p.eps_pmm_ptr->Error_UART_port_M == SUCCESS ){
			UART_X_eps_comm = UART_Main_eps_comm;
		}else{ //Error UART_port_M
			UART_X_eps_comm = UART_Backup_eps_comm;
		}
	}		

	//Set source and destination address.
	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){
		source_addr = UART_X_eps_comm->uart_unit_addr;
		destination_addr = UART_EPS_CPUb_Addr;
	}else{
		source_addr = UART_X_eps_comm->uart_unit_addr;
		destination_addr = UART_EPS_CPUm_Addr;
	}

	//Clear input EPS UART buffer.
	UART_EPS_Pars_Get_Package( UART_X_eps_comm, eps_p );

    error_status = ERROR_N;
	//Send a command and wait for an answer.
	if( UART_EPS_Send_Package( UART_X_eps_comm->USARTx, destination_addr,  source_addr, UART_EPS_CMD, send_buf, size_send_data) == SUCCESS ){

        timeout_counter = SysTick_Counter;
        UART_X_eps_comm->waiting_answer_flag = 1;

		while( UART_X_eps_comm->waiting_answer_flag != 0 ){ //waiting_answer_flag - The flag should be reset in the function UART_EPS_Pars_Get_Package when a response is received.

			if ( (SysTick_Counter - timeout_counter) > UART_EPS_ACK_TIMEOUT ){
				UART_X_eps_comm->waiting_answer_flag = 0;
				UART_X_eps_comm->error_port_counter++;
                #ifdef DEBUGprintf
				    Error_Handler();
                #endif
				break;
			}

			if( UART_X_eps_comm->stop_recv_pack_flag == 1){ //Response processing
				error_status = UART_EPS_Pars_Get_Package( UART_X_eps_comm, eps_p);
			}
		}
	}

	if( error_status == SUCCESS ){
		UART_X_eps_comm->error_port_counter = 0;
	}

 	return error_status;
}


/** @brief Send NFC (notification).
	@param  cmd_id - Command ID
	@param  choice_uart_por - Set the port through which to send the command. 0 - Auto fault port detection, 1 - Main UART port, 2 - Backup UART port.
	@param  *UART_Main_eps_comm - pointer to main UART port struct with get data.
	@param  *UART_Backup_eps_comm - pointer to backup UART port struct with get data.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_NFC( uint8_t nfc_id, uint8_t choice_uart_port, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

	uint8_t send_buf[UART_EPS_PACK_SIZE_BUFF];
	uint16_t size_send_data = 0;
	uint8_t destination_addr = 0;
	uint8_t source_addr = 0;
	int8_t error_status = ERROR_N;
	_UART_EPS_COMM *UART_X_eps_comm;

	if( nfc_id == UART_EPS_ID_NFS_Prep_Take_CTRL ){

		send_buf[0] = UART_EPS_ID_NFS_Prep_Take_CTRL;
		size_send_data = 1;

		//PMM_Set_MUX_CAN_CPUm_CPUb( CPUmain );
	}else{
		return ERROR_N;
	}

	//Choice UART port Main, Backup or Auto detect.
	if( choice_uart_port == 1 ){ // Set Main UART port 
		UART_X_eps_comm = UART_Main_eps_comm;

	}else if( choice_uart_port == 2 ){ //Set Backup UART port 
		UART_X_eps_comm = UART_Backup_eps_comm;

	}else{//Set auto detect UART port 
		if(eps_p.eps_pmm_ptr->Error_UART_port_M == SUCCESS ){
			UART_X_eps_comm = UART_Main_eps_comm;
		}else{ //Error UART_port_M
			UART_X_eps_comm = UART_Backup_eps_comm;
		}
	}		

	//Set source and destination address.
	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){
		source_addr = UART_X_eps_comm->uart_unit_addr;
		destination_addr = UART_EPS_CPUb_Addr;
	}else{
		source_addr = UART_X_eps_comm->uart_unit_addr;
		destination_addr = UART_EPS_CPUm_Addr;
	}		

	error_status = ERROR_N;

	error_status = UART_EPS_Send_Package( UART_X_eps_comm->USARTx, destination_addr,  source_addr, UART_EPS_NFC, send_buf, size_send_data);

	if( error_status == ERROR_N ){
		UART_X_eps_comm->error_port_counter++;
	}else{
		UART_X_eps_comm->error_port_counter = 0;
	}

 	return error_status;
}


/** @brief Send ACK acknowledgment (answer).
	@param  *UART_eps_comm - pointer to UART port struct with get data.
	@param  send_data[] - send data massive.
	@param 	size_data -  size sending data.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus UART_EPS_Send_ACK ( _UART_EPS_COMM *UART_eps_comm , uint8_t send_data[], uint16_t size_data ){
	return UART_EPS_Send_Package( UART_eps_comm->USARTx, UART_eps_comm->recv_pack_buf[2], UART_eps_comm->recv_pack_buf[1], UART_EPS_ACK, send_data, size_data );
}


