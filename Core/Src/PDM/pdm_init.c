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
	int8_t ret_val_error = 0;

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


	//Initialization temp. sensor 1.
	ret_val_error = PDM_init_TMP1075( PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1, PDM_I2CADDR_MUX, TCA9548_CH6);

	if(ret_val_error == -2){
		pdm_ptr->Error_I2C_MUX = 1;
		pdm_ptr->Error_temp_sensor_1 = 1;
		error_stutus += ERROR_N;

	}else if(ret_val_error == -1){
		pdm_ptr->Error_temp_sensor_1 = 1;
		error_stutus += ERROR_N;

	}else{
		pdm_ptr->Error_I2C_MUX = 0;
		pdm_ptr->Error_temp_sensor_1 = 0;
	}

	//Initialization temp. sensor 2.
	ret_val_error = PDM_init_TMP1075( PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2, PDM_I2CADDR_MUX, TCA9548_CH6);

	if(ret_val_error == -2){
		pdm_ptr->Error_I2C_MUX = 1;
		pdm_ptr->Error_temp_sensor_2 = 1;
		error_stutus += ERROR_N;

	}else if(ret_val_error == -1){
		pdm_ptr->Error_temp_sensor_2 = 1;
		error_stutus += ERROR_N;

	}else{
		pdm_ptr->Error_I2C_MUX = 0;
		pdm_ptr->Error_temp_sensor_2 = 0;
	}

	//Initialization temp. sensor 3.
	ret_val_error = PDM_init_TMP1075( PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3, PDM_I2CADDR_MUX, TCA9548_CH6);

	if(ret_val_error == -2){
		pdm_ptr->Error_I2C_MUX = 1;
		pdm_ptr->Error_temp_sensor_3 = 1;
		error_stutus += ERROR_N;

	}else if(ret_val_error == -1){
		pdm_ptr->Error_temp_sensor_3 = 1;
		error_stutus += ERROR_N;

	}else{
		pdm_ptr->Error_I2C_MUX = 0;
		pdm_ptr->Error_temp_sensor_3 = 0;
	}


	//Initialization temp. sensor 4.
	ret_val_error = PDM_init_TMP1075( PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4, PDM_I2CADDR_MUX, TCA9548_CH6);

	if(ret_val_error == -2){
		pdm_ptr->Error_I2C_MUX = 1;
		pdm_ptr->Error_temp_sensor_4 = 1;
		error_stutus += ERROR_N;

	}else if(ret_val_error == -1){
		pdm_ptr->Error_temp_sensor_4 = 1;
		error_stutus += ERROR_N;

	}else{
		pdm_ptr->Error_I2C_MUX = 0;
		pdm_ptr->Error_temp_sensor_4 = 0;
	}


//TO DO add initialization INA231.

	if(error_stutus != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


