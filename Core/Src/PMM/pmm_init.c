#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TCA9539.h"
#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include "CAND/canv.h"
#include "pdm_init.h"
#include "pmm_struct.h"
#include "eps_struct.h"
#include "pmm_config.h"
#include "pmm_init_IC.h"
#include "pmm_ctrl.h"
#include "pmm_init_IC.h"
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

/** @rief  Initialization Active CPU 
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval None
 */	
void PMM_Init_ActiveCPUblock( _EPS_Param eps_p ){

	PMM_Set_MUX_CAN_CPUm_CPUb( eps_p.eps_pmm_ptr );

	ENABLE_TMUX1209_I2C();

	if( eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain == DISABLE && eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup == DISABLE ){
		eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain = ENABLE;
	    eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup = ENABLE;
	}

	CAN_init_eps(CAN1);
	CAN_init_eps(CAN2);
	CAN_RegisterAllVars();

	PMM_init( eps_p.eps_pmm_ptr );
}


/** @rief  Initialization Active CPU 
	@param  none
	@retval None
 */
void PMM_Init_PassiveCPUblock( _EPS_Param eps_p ){
	
	PMM_HARD_Reset_I2C_GPIOExt( PMM_I2CADDR_GPIOExt1);
	DISABLE_TMUX1209_I2C();
	CAN_DeInit(CAN1);
	CAN_DeInit(CAN2);
	PMM_Power_Down_TMP1075( eps_p.eps_pmm_ptr, PMM_I2Cx_TMP1075, PMM_I2CADDR_TMP1075);
}
