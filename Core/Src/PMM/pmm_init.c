
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "uart_eps_comm.h"
#include "TCA9539.h"
#include "pmm_struct.h"
#include "pdm_struct.h"
#include "pmm_config.h"
#include "pmm_init_IC.h"
#include "pmm_ctrl.h"

#include "pmm_init.h"


//*******TO DO ******//
//1. Поудмать над тем чтобы было две функции инит плата PMM и блок CPU.

/** @brief  Inicialisation PMM board
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_init(_PMM *pmm_ptr){

	//pmm->Main_Backup_mode_CPU = PMM_Detect_MasterBackupCPU();

	int8_t error_stutus = SUCCESS; 

	if( pmm_ptr->PWR_Ch_State_CANmain == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_CANbackup == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_Vbat1_eF1 == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1, DISABLE );
	}
	
	if( pmm_ptr->PWR_Ch_State_Vbat1_eF2 == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_Vbat2_eF1 == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, DISABLE );
	}	

	if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, DISABLE );
	}
	
	if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_PBMs_Logic, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_PBMs_Logic, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_Deploy_Logic == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_Deploy_Power == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_5V_Bus == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_5V_Bus, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_5V_Bus, DISABLE );
	}

	if( pmm_ptr->PWR_Ch_State_3_3V_Bus == ENABLE){
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_3_3V_Bus, ENABLE );
	}else{
		error_stutus += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_3_3V_Bus, DISABLE );
	}


	error_stutus += PMM_init_TMP1075( pmm_ptr, PMM_I2Cx_TMP1075, PMM_I2CADDR_TMP1075 );

	error_stutus += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1);
	error_stutus += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2);
	error_stutus += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1);
	error_stutus += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2);

	error_stutus += ADS1015_init( pmm_ptr, PMM_I2Cx_PowerADC, PMM_I2CADDR_PowerADC);

	if(error_stutus != SUCCESS){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	return SUCCESS;
}


/** @rief  Detect Main or Backup mode CPU block PMM .
	@param  None
	@retval 1 - CPU Main mode, 0 - CPU Backup mode.
 */
uint8_t PMM_Detect_MasterBackupCPU(void){

	uint8_t mode = 0;

	mode = LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_9)\
			+LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_10)\
			+LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_11);
	
	if( mode >= 2 ){
		return 0;  //Main mode CPU
	}else{
		return 1;  //Backup mode CPU
	}

}


/** @brief Checking active CPU flag  (pmm_ptr->Active_CPU) between main and backup CPU.
 * 			In case when (pmm_ptr->Active_CPU) the same value in the Main and Backup CPU
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
void PMM_Check_Active_CPU(_PMM *pmm_ptr,  _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm){

	_PMM backup_CPU_pmm = {0};
	_PDM null_pdm = {0};
	_PBM null_pbm[1] = {0};

	int8_t error_status = ERROR_N;
	uint32_t i = 0;
	uint32_t timeout_counter = 0;

	if( pmm_ptr->Main_Backup_mode_CPU == 0 ){ //Only for Main CPU

		while( ( error_status != SUCCESS ) && ( i < pmm_uart_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			error_status = UART_EPS_Send_CMD( UART_EPS_ID_CMD_Get_PMM_struct, 0, UART_Main_eps_comm, UART_Backup_eps_comm, &backup_CPU_pmm, &null_pdm, null_pbm  );

			if( error_status != SUCCESS ){
				i++;
				LL_mDelay( pmm_uart_delay_att_conn );
			}
		}

		if(error_status == SUCCESS ){

			 if( pmm_ptr->Active_CPU != backup_CPU_pmm.Active_CPU ){

				 if(  backup_CPU_pmm.Active_CPU == 1){
					 pmm_ptr->Active_CPU = 1;
				 }else{
					 pmm_ptr->Active_CPU = 0;
				 }
			 }
		}

	}else{//Only for Backup CPU Wait request PMM struct. from Main CPU

		timeout_counter = 0;

		while( timeout_counter  != 10000   ){ //waiting_answer_flag - The flag should be reset in the function UART_EPS_Pars_Get_Package when a response is received.

			timeout_counter++;

			if( UART_Main_eps_comm->stop_recv_pack_flag == 1){ //Response processing
				UART_EPS_Pars_Get_Package(UART_Main_eps_comm, pmm_ptr, &null_pdm);
			}
			if( UART_Backup_eps_comm->stop_recv_pack_flag == 1){ //Response processing
				UART_EPS_Pars_Get_Package( UART_Backup_eps_comm, pmm_ptr, &null_pdm);
			}
		}
	}
}




