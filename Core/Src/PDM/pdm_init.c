#include "stm32l4xx.h"
#include "Error_Handler.h"
#include "pdm_config.h"
#include "pdm_struct.h"
#include "pdm_init_IC.h"
#include "TCA9548.h"
#include "pdm_ctrl.h"
#include "pdm_init.h"



/** @brief  Initialisation PDM board
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init(_PDM *pdm_ptr){

	int8_t error_status = SUCCESS;
	uint8_t num_pwr_ch;
	

	//Restoring power channels.
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){

		if( (pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_in == ENABLE) && (pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_out == ENABLE) ){
			error_status += PDM_Set_state_PWR_CH( pdm_ptr, num_pwr_ch, ENABLE );
		}else{
			error_status += PDM_Set_state_PWR_CH( pdm_ptr, num_pwr_ch, DISABLE );
		}
	}


	//Disable all channels TCA9548 I2C MUX on PDM module.
	//Note: We don’t check return errors because in any case, trying to configure the chips behind the I2C MUX
	TCA9548_Disable_I2C_ch( PDM_I2Cx_I2C_MUX, PDM_I2CADDR_I2C_MUX, TCA9548_ALL_CHANNELS );


	//Initialization temperature sensors.
	error_status += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );
	error_status += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );
	error_status += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );
	error_status += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4, PDM_I2CADDR_I2C_MUX, TCA9548_CH6 );


	//Initialization Power monitor INA231 power channel
	for(num_pwr_ch = 0; num_pwr_ch < PDM_PWR_Ch_quantity; num_pwr_ch++){
		error_status += PDM_init_PWR_Mon_INA231( pdm_ptr, num_pwr_ch);
	}


	if(error_status != SUCCESS){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	return SUCCESS;
}


