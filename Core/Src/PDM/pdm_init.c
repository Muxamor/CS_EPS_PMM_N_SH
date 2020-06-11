#include "stm32l4xx.h"
#include "pdm_config.h"
#include "pdm_struct.h"
#include "pdm_init_IC.h"
#include "TCA9548.h"
#include "pdm_ctrl.h"
#include "pdm_init.h"

//TODO : Need add initialization I2C multiplexor


/** @brief  Inicialisation PDM board
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init(_PDM *pdm_ptr){

	int8_t error_stutus = SUCCESS; 

	//Restoring power supply channels.
	if( (pdm_ptr->PWR_CH1_State_eF_in == ENABLE) && (pdm_ptr->PWR_CH1_State_eF_out == ENABLE) ){
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_1, ENABLE );
	}else{
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_1, DISABLE );
	}

	if( (pdm_ptr->PWR_CH2_State_eF_in == ENABLE) && (pdm_ptr->PWR_CH2_State_eF_out == ENABLE) ){
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_2, ENABLE );
	}else{
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_2, DISABLE );
	}

	if( (pdm_ptr->PWR_CH3_State_eF_in == ENABLE) && (pdm_ptr->PWR_CH3_State_eF_out == ENABLE) ){
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, ENABLE );
	}else{
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, DISABLE );
	}

	if( (pdm_ptr->PWR_CH4_State_eF_in == ENABLE) && (pdm_ptr->PWR_CH4_State_eF_out == ENABLE) ){
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, ENABLE );
	}else{
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, DISABLE );
	}

	if( (pdm_ptr->PWR_CH5_State_eF_in == ENABLE) && (pdm_ptr->PWR_CH5_State_eF_out == ENABLE) ){
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_5, ENABLE );
	}else{
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_5, DISABLE );
	}

	if( (pdm_ptr->PWR_CH6_State_eF_in == ENABLE) && (pdm_ptr->PWR_CH6_State_eF_out == ENABLE) ){
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_6, ENABLE );
	}else{
		error_stutus += PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_6, DISABLE );
	}


	//Disable all channels TCA9548 I2C MUX on PDM module.
	//Note: We don’t check return errors because in any case, trying to configure the chips behind the I2C MUX
	TCA9548_Disable_I2C_ch( PDM_I2Cx_MUX, PDM_I2CADDR_MUX, TCA9548_ALL_CHANNELS );

	//Initialization temperature sensors.
	error_stutus += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1, PDM_I2CADDR_MUX, TCA9548_CH6 );
	error_stutus += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2, PDM_I2CADDR_MUX, TCA9548_CH6 );
	error_stutus += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3, PDM_I2CADDR_MUX, TCA9548_CH6 );
	error_stutus += PDM_init_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4, PDM_I2CADDR_MUX, TCA9548_CH6 );


	//Initialization Power monitor INA231 power channel 1
	if(PDM_PCB_Assembled_PWR_CH1 == 0x01 ){
		error_stutus += PDM_init_INA231( pdm_ptr, PDM_I2Cx_PWR_Mon_Ch1, PDM_I2CADDR_PWR_Mon_Ch1, 2.0, 0.015, PDM_I2CADDR_MUX, TCA9548_CH6 );
	}

	//Initialization Power monitor INA231 power channel 2
	if(PDM_PCB_Assembled_PWR_CH2 == 0x01 ){
		error_stutus += PDM_init_INA231( pdm_ptr, PDM_I2Cx_PWR_Mon_Ch2, PDM_I2CADDR_PWR_Mon_Ch2, 2.0, 0.015, PDM_I2CADDR_MUX, TCA9548_CH4 );
	}

	//Initialization Power monitor INA231 power channel 3
	if(PDM_PCB_Assembled_PWR_CH3 == 0x01 ){
		error_stutus += PDM_init_INA231( pdm_ptr, PDM_I2Cx_PWR_Mon_Ch3, PDM_I2CADDR_PWR_Mon_Ch3, 2.0, 0.015, PDM_I2CADDR_MUX, TCA9548_CH3 );
	}

	//Initialization Power monitor INA231 power channel 4
	if(PDM_PCB_Assembled_PWR_CH4 == 0x01 ){
		error_stutus += PDM_init_INA231( pdm_ptr, PDM_I2Cx_PWR_Mon_Ch4, PDM_I2CADDR_PWR_Mon_Ch4, 2.0, 0.015, PDM_I2CADDR_MUX, TCA9548_CH2 );
	}

	//Initialization Power monitor INA231 power channel 5
	if(PDM_PCB_Assembled_PWR_CH5 == 0x01 ){
		error_stutus += PDM_init_INA231( pdm_ptr, PDM_I2Cx_PWR_Mon_Ch5, PDM_I2CADDR_PWR_Mon_Ch5, 2.0, 0.015, PDM_I2CADDR_MUX, TCA9548_CH1 );
	}

	//Initialization Power monitor INA231 power channel 6
	if(PDM_PCB_Assembled_PWR_CH6 == 0x01 ){
		error_stutus += PDM_init_INA231( pdm_ptr, PDM_I2Cx_PWR_Mon_Ch6, PDM_I2CADDR_PWR_Mon_Ch6, 2.0, 0.015, PDM_I2CADDR_MUX, TCA9548_CH0 );
	}


	if(error_stutus != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


