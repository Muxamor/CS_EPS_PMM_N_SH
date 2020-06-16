#include "stm32l4xx.h"
#include "pdm_config.h"
#include "pdm_struct.h"
#include "pdm_ctrl.h"
#include "TCA9548.h"
#include "pdm.h"


/** @brief  Get all telemetry data from PDM
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_Telemetry( _PDM *pdm_ptr ){

	int8_t error_stutus = SUCCESS;
	uint8_t num_pwr_ch = 0;

	//Check GPIO eFuse state.
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		error_stutus += PDM_Check_state_PWR_CH( pdm_ptr, num_pwr_ch);
	}


	//Check GPIO eFuse state.
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		error_stutus += PDM_Get_PG_PWR_CH( pdm_ptr, num_pwr_ch);
	}

	//Get value Current, Voltage, Power
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		error_stutus += PDM_Get_PWR_CH_I_V_P( pdm_ptr, num_pwr_ch);
	}

	//Initialization temperature sensors.
	error_stutus += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );
	error_stutus += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );
	error_stutus += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );
	error_stutus += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );

	if(error_stutus != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


