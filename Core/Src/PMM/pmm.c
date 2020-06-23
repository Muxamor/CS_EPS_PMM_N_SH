#include "stm32l4xx.h"
#include "pmm_config.h"
#include "pmm_struct.h"
#include "pmm_ctrl.h"
#include "pmm.h"


/** @brief  Get all telemetry data from PMM
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_Telemetry( _PMM *pmm_ptr ){

	int8_t error_stutus = SUCCESS;

	//Get value Current, Voltage, Power  of power channel VBAT1 and VBAT2.
	error_stutus += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1 );
	error_stutus += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2 );
	error_stutus += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1 );
	error_stutus += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2 );

	//Get temperature value.
	error_stutus += PMM_Get_Temperature( pmm_ptr, PMM_I2Cx_TMP1075, PMM_I2CADDR_TMP1075 );


	if(error_stutus != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

