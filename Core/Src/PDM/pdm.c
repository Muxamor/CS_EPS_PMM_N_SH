#include "stm32l4xx.h"
#include "TCA9548.h"
#include "PDM/pdm_config.h"
#include "PDM/pdm_struct.h"
#include "PDM/pdm_ctrl.h"
#include "PDM/pdm.h"


/** @brief  Get all telemetry data from PDM
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_Telemetry( _PDM *pdm_ptr ){

	int8_t error_status = SUCCESS;
	uint8_t num_pwr_ch = 0;

	//Check GPIO eFuse state.
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		error_status += PDM_Check_state_PWR_CH( pdm_ptr, num_pwr_ch);
	}


	//Check GPIO eFuse state.
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		error_status += PDM_Get_PG_PWR_CH( pdm_ptr, num_pwr_ch);
	}

	//Get value Current, Voltage, Power
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		error_status += PDM_Get_PWR_CH_I_V_P( pdm_ptr, num_pwr_ch);
	}

	//Get temperature value.
	error_status += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_PORT, PDM_I2CADDR_TMP1075_1, PDM_I2CADDR_I2C_MUX1, TCA9548_CH6 );
	error_status += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_PORT, PDM_I2CADDR_TMP1075_2, PDM_I2CADDR_I2C_MUX1, TCA9548_CH6 );
	error_status += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_PORT, PDM_I2CADDR_TMP1075_3, PDM_I2CADDR_I2C_MUX2, TCA9548_CH6 );
	error_status += PDM_Get_Temperature( pdm_ptr, PDM_I2Cx_PORT, PDM_I2CADDR_TMP1075_4, PDM_I2CADDR_I2C_MUX2, TCA9548_CH6 );

	if(error_status != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


