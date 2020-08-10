#include "stm32l4xx.h"
#include "TCA9548.h"
#include "PAM/pam_config.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_ctrl.h"
#include "PAM/pam.h"


/** @brief  Get all telemetry data from PAM
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_Telemetry( _PAM *pam_ptr ){

	int8_t error_status = SUCCESS;
	uint8_t num_pwr_ch = 0;

	// Get state power supply
	error_status += PAM_Get_PG_PWR_Supply(pam_ptr, PAM_PWR_DC_DC);
	error_status += PAM_Get_PG_PWR_Supply(pam_ptr, PAM_PWR_LDO);

    // Check state power supply
	error_status += PAM_Check_state_PWR_Supply(pam_ptr, PAM_PWR_DC_DC);
	error_status += PAM_Check_state_PWR_Supply(pam_ptr, PAM_PWR_LDO);

   	//Get value Current, Voltage, Power, State ID
	for(num_pwr_ch = 0; num_pwr_ch < PAM_PWR_IN_Ch_quantity; num_pwr_ch++){

		error_status += PAM_Get_State_ID_PWR_Ch_In(pam_ptr, num_pwr_ch);
		error_status += PAM_Get_PWR_CH_IN_I_V_P(pam_ptr, num_pwr_ch);
	}

	//Get temperature value.
	error_status += PAM_Get_Temperature( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_1, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6 );
	error_status += PAM_Get_Temperature( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_2, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6 );
	error_status += PAM_Get_Temperature( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_3, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6 );
	error_status += PAM_Get_Temperature( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_4, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6 );

	if(error_status != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}
