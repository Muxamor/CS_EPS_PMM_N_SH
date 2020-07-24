
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "CAND/CAN.h"
#include "PMM/eps_struct.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_ctrl.h"
#include "uart_eps_comm.h"
#include "PMM/pmm_sw_cpu.h"

/** @brief Checking active CPU flag  (pmm_ptr->Active_CPU) between main and backup CPU.
 * 			In case when (pmm_ptr->Active_CPU) the same value in the Main and Backup CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Check_Active_CPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm,  _EPS_Param eps_p ){

	_PMM backup_CPU_pmm = {0};
	_EPS_Param tmp_eps_param = {.eps_pmm_ptr = &backup_CPU_pmm };


	int8_t error_status = ERROR_N;
	uint32_t i = 0;
	uint32_t timeout_counter = 0;

	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == 0 ){ //Only for Main CPU

		while( ( error_status != SUCCESS ) && ( i < pmm_uart_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			error_status = UART_EPS_Send_CMD( UART_EPS_ID_CMD_Get_PMM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, tmp_eps_param );

			if( error_status != SUCCESS ){
				i++;
				LL_mDelay( pmm_uart_delay_att_conn );
			}
		}

		if(error_status == SUCCESS ){

			 if( eps_p.eps_pmm_ptr->Active_CPU != tmp_eps_param.eps_pmm_ptr->Active_CPU ){

				 if(  tmp_eps_param.eps_pmm_ptr->Active_CPU == 1){
					 eps_p.eps_pmm_ptr->Active_CPU = 1;
				 }else{
					 eps_p.eps_pmm_ptr->Active_CPU = 0;
				 }
			 }
		}

	}else{//Only for Backup CPU Wait request PMM struct. from Main CPU

		timeout_counter = 0;

		while( timeout_counter != 10000 ){ //waiting_answer_flag - The flag should be reset in the function UART_EPS_Pars_Get_Package when a response is received.

			timeout_counter++;

			if( UART_Main_eps_comm->stop_recv_pack_flag == 1){ //Response processing
				UART_EPS_Pars_Get_Package(UART_Main_eps_comm, eps_p );
			}
			if( UART_Backup_eps_comm->stop_recv_pack_flag == 1){ //Response processing
				UART_EPS_Pars_Get_Package( UART_Backup_eps_comm, eps_p );
			}
		}
	}
}


/** @brief Checking active CPU flag  (pmm_ptr->Active_CPU) between main and backup CPU.
 * 			In case when (pmm_ptr->Active_CPU) the same value in the Main and Backup CPU
 			This function use Active CPU  to transfer control to passive CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Switch_Active_CPU(uint8_t set_active_CPU,  _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p ){

	int8_t error_status = ERROR_N;
	uint32_t i = 0;

	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain && eps_p.eps_pmm_ptr->Active_CPU == CPUmain_Active && set_active_CPU == CPUmain_Active ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUmain_Active;
		error_status = SUCCESS;

	}else if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUbackup && eps_p.eps_pmm_ptr->Active_CPU == CPUbackup_Active && set_active_CPU == CPUbackup_Active ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUbackup_Active;
		error_status = SUCCESS;

	}else{

		i = 0;
		error_status = ERROR_N;
		while( ( error_status != SUCCESS ) && ( i < pmm_uart_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			if( UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PMM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p ) == SUCCESS ){
				if( UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PDM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p ) == SUCCESS ){
					if( UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PAM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p ) == SUCCESS ){
						error_status = UART_EPS_Send_CMD( UART_EPS_ID_CMD_SAVE_PBM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );

					}
				}
			}

			if( error_status != SUCCESS ){
				i++;
				LL_mDelay( pmm_uart_delay_att_conn );
			}
		}
		
		if( error_status == SUCCESS ){
			UART_EPS_Send_NFC( UART_EPS_ID_NFS_Prep_Take_CTRL, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );
			//LL_mDelay(5);
			eps_p.eps_pmm_ptr->Active_CPU = set_active_CPU;

			PMM_Set_MUX_CAN_CPUm_CPUb( eps_p.eps_pmm_ptr );

			i = 0;
			error_status = ERROR_N;
			while( ( error_status != SUCCESS ) && ( i < pmm_uart_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

				error_status = UART_EPS_Send_CMD( UART_EPS_ID_CMD_Take_CTRL, 0, UART_Main_eps_comm, UART_Backup_eps_comm, eps_p );
			
				if( error_status != SUCCESS ){
					i++;
					LL_mDelay( pmm_uart_delay_att_conn );
				}
			}

			if( error_status == SUCCESS ){
				//eps_p.eps_pmm_ptr->Active_CPU = set_active_CPU;
				PMM_Set_mode_Passive_CPU(eps_p);
				//eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
			}

		}

		if( error_status == ERROR_N ){
			eps_p.eps_pmm_ptr->Active_CPU = !set_active_CPU;
			PMM_Set_MUX_CAN_CPUm_CPUb( eps_p.eps_pmm_ptr );
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}

	}

	return error_status;
}

/** @brief  Set Active CPU mode if we get or sent request from/to another CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Set_mode_Active_CPU( _EPS_Param eps_p ){
		
	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUmain_Active;
	}else{
		eps_p.eps_pmm_ptr->Active_CPU = CPUbackup_Active;
	}

	PMM_init( eps_p.eps_pmm_ptr );

	CAN_init_eps(CAN1);
	CAN_init_eps(CAN2);
	CAN_RegisterAllVars();

	eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1;
}

/** @brief  Set Passive CPU mode if we get or sent request from/to another CPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Set_mode_Passive_CPU( _EPS_Param eps_p ){

	if( eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ){
		eps_p.eps_pmm_ptr->Active_CPU = CPUbackup_Active ;
	}else{
		eps_p.eps_pmm_ptr->Active_CPU = CPUmain_Active;
	}

	CAN_DeInit_eps(CAN1);
	CAN_DeInit_eps(CAN2);

    PBM_Low_Energy_Reset_pin();
    PMM_RT_FL_EPS1_Reset_pin();
    PMM_RT_FL_EPS2_Reset_pin();

	PMM_init( eps_p.eps_pmm_ptr );

	eps_p.eps_pmm_ptr->PMM_save_conf_flag = 1; 
}




