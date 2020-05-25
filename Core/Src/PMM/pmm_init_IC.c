
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "pmm_config.h"

#include "pmm_init_IC.h"

/*********************** TODO *********************/
//1. Change or delate fn. PMM_default_init_I2C_GPIOExt1 - temporary function for debugging period

/**************************************************/

ErrorStatus PMM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

	uint8_t i = 0;

	SW_TMUX1209_I2C_main_PMM();// Switch MUX to PDM I2C bus on PMM

	for( i = 0; i < pmm_i2c_attempt_conn; i++){

		if(  TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) == SUCCESS ){
			break;
		}

		LL_mDelay(pmm_i2c_delay_att_conn);

		if( i == (pmm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}
 	
	for( i = 0; i < pmm_i2c_attempt_conn; i++){

		if(  TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){
			break;
		}

		LL_mDelay(pmm_i2c_delay_att_conn);

		if( i == (pmm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}

	for( i = 0; i < pmm_i2c_attempt_conn; i++){

		if(  TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) == SUCCESS ){
			break;
		}

		LL_mDelay(pmm_i2c_delay_att_conn);

		if( i == (pmm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}

	return SUCCESS;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TEMP   FN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/** @brief  Default Init I2C GPIO extenders ( Ext1 (U3) ) on PMM module.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tca9539_addr - I2C address GPIO Exneder
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_default_init_I2C_GPIOExt1 (I2C_TypeDef *I2Cx, uint8_t tca9539_addr){ 

	uint8_t i = 0;

	SW_TMUX1209_I2C_main_PMM();// Switch MUX to PDM I2C bus on PMM

	for( i = 0; i < pmm_i2c_attempt_conn; i++){
		
		if( TCA9539_Reset_output_pin(I2Cx, tca9539_addr, TCA9539_IO_ALL ) == SUCCESS ){
			break;
		}

		LL_mDelay(pmm_i2c_delay_att_conn);

		if( i == (pmm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}


	for( i = 0; i < pmm_i2c_attempt_conn; i++){
		
		if( TCA9539_conf_IO_dir_output(I2Cx, tca9539_addr, TCA9539_IO_ALL ) == SUCCESS ){
			break;
		}

		LL_mDelay(pmm_i2c_delay_att_conn);

		if( i == (pmm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}

	return SUCCESS;
}
