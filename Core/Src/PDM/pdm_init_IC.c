
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "pdm_config.h"

#include "pdm_init_IC.h"

/** @brief  Init TMP1075 sensor on PDM module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address 
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

	uint8_t i = 0;

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	for( i = 0; i < pdm_i2c_attempt_conn; i++){
		
		if( TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) == SUCCESS ){
			break;
		}

		LL_mDelay(pdm_i2c_delay_att_conn);

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}

	for( i = 0; i < pdm_i2c_attempt_conn; i++){
		
		if( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){
			break;
		}

		LL_mDelay(pdm_i2c_delay_att_conn);

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}

	for( i = 0; i < pdm_i2c_attempt_conn; i++){
		
		if( TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) == SUCCESS ){
			break;
		}

		LL_mDelay(pdm_i2c_delay_att_conn);

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}

	return SUCCESS;
}

/** @brief  Init I2C GPIO extenders (Ext1-U13 and Ext2-U20) on PDM module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tca9539_addr - I2C address GPIO Exneder
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_init_I2C_GPIOExt(I2C_TypeDef *I2Cx, uint8_t tca9539_addr){

	uint8_t i = 0;

	SW_TMUX1209_I2C_main_PDM();// Switch MUX to PDM I2C bus on PMM

	for( i = 0; i < pdm_i2c_attempt_conn; i++){
		
		if( TCA9539_Reset_output_pin(I2Cx, tca9539_addr, TCA9539_IO_P00|TCA9539_IO_P02|TCA9539_IO_P04|TCA9539_IO_P05|TCA9539_IO_P07\
													|TCA9539_IO_P11|TCA9539_IO_P12|TCA9539_IO_P14|TCA9539_IO_P16|TCA9539_IO_P17 ) == SUCCESS ){
			break;
		}

		LL_mDelay(pdm_i2c_delay_att_conn);

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}


	for( i = 0; i < pdm_i2c_attempt_conn; i++){
		
		if( TCA9539_conf_IO_dir_output(I2Cx, tca9539_addr, TCA9539_IO_P00|TCA9539_IO_P02|TCA9539_IO_P04|TCA9539_IO_P05|TCA9539_IO_P07\
														|TCA9539_IO_P11|TCA9539_IO_P12|TCA9539_IO_P14|TCA9539_IO_P16|TCA9539_IO_P17 ) == SUCCESS ){
			break;
		}

		LL_mDelay(pdm_i2c_delay_att_conn);

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}


	for( i = 0; i < pdm_i2c_attempt_conn; i++){
		
		if(TCA9539_conf_IO_dir_input(I2Cx, tca9539_addr, TCA9539_IO_P01|TCA9539_IO_P03|TCA9539_IO_P06|TCA9539_IO_P10|TCA9539_IO_P13|TCA9539_IO_P15) == SUCCESS ){
			break;
		}

		LL_mDelay(pdm_i2c_delay_att_conn);

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			return ERROR_N;
		}
	}
	
	return SUCCESS;
}