#include "stm32l4xx.h"
#include "pdm_config.h"
#include "pdm_struct.h"
#include "pdm_init_IC.h"
#include "pdm_init.h"

//TODO : Need add initialization I2C multiplexor


/** @brief  Inicialisation PDM board
	@param  *pdm_str_ptr - pointer to struct which save all information about PDM.
	@retval None
*/
void PDM_init(_PDM *pdm_str_ptr){

	if( PDM_init_TMP1075(PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1) != SUCCESS ){
		pdm_str_ptr->Error_temp_sensor_1 = 1;
	}else{
		pdm_str_ptr->Error_temp_sensor_1 = 0;
	}

	if( PDM_init_TMP1075(PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2) != SUCCESS ){
		pdm_str_ptr->Error_temp_sensor_2 = 1;
	}else{
		pdm_str_ptr->Error_temp_sensor_2 = 0;
	}

	if( PDM_init_TMP1075(PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3) != SUCCESS ){
		pdm_str_ptr->Error_temp_sensor_3 = 1;
	}else{
		pdm_str_ptr->Error_temp_sensor_3 = 0;
	}

	if( PDM_init_TMP1075(PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4) != SUCCESS ){
		pdm_str_ptr->Error_temp_sensor_4 = 1;
	}else{
		pdm_str_ptr->Error_temp_sensor_4 = 0;
	}

	if( PDM_init_I2C_GPIOExt(PDM_I2Cx_GPIOExt1, PDM_I2CADDR_GPIOExt1) != SUCCESS ){
		pdm_str_ptr->Error_I2C_GPIO_Ext1 = 1;
	}else{
		pdm_str_ptr->Error_I2C_GPIO_Ext1 = 0;	
	}

	if( PDM_init_I2C_GPIOExt(PDM_I2Cx_GPIOExt2, PDM_I2CADDR_GPIOExt2) != SUCCESS ){
		pdm_str_ptr->Error_I2C_GPIO_Ext2 = 1;
	}else{
		pdm_str_ptr->Error_I2C_GPIO_Ext2 = 0;	
	}

	//Need add initialization I2C multiplexor
}


