#include "pdm_init_IC.h"

/** @brief  Init TMP1075 sensor on PDM module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address 
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 
 	
 	if( TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) != SUCCESS ){
 		if( TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) != SUCCESS ){
 			if( TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) != SUCCESS ){
 				return ERROR_N;
 			}
 		}
 	}

 	if( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) != SUCCESS ){
 		if( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) != SUCCESS ){
 			if( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) != SUCCESS ){
 				return ERROR_N;
 			}
 		}
 	}

 	if( TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) != SUCCESS ){
 		if( TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) != SUCCESS ){
 			if( TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) != SUCCESS ){
 				return ERROR_N;
 			}
 		}
 	}

	return SUCCESS;
}

/** @brief  Init I2C GPIO extender TCA9539 on PDM module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address 
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init_TCA9539(I2C_TypeDef *I2Cx, uint8_t tca9539_addr){

	SW_TMUX1209_I2C_main_PDM();// Switch MUX to PDM I2C bus on PMM 

	 	
	return SUCCESS;
}