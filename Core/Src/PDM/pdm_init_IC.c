
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "TCA9548.h"
#include "INA231.h"
#include "pdm_config.h"
#include "pdm_struct.h"
#include "pdm_init_IC.h"


/** @brief  Init TMP1075 sensor on PDM module
    @param 	*pdm_ptr - pointer to struct which contain all information about PDM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address 
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch - Number channel MUX
	@retval 0 - SUCCESS, -1 - Error temp. sensor , -2  Error I2C MUX
*/
ErrorStatus PDM_init_TMP1075( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t Error_TMP1075 = ERROR_N;

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	if (error_I2C == SUCCESS ){

		//Setup INA231
		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			if (TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) == SUCCESS ){
				if ( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){

					error_I2C = TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr);
				}
			}

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pdm_i2c_delay_att_conn );
			}
		}

		Error_TMP1075 = error_I2C;
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		pdm_ptr->Error_I2C_MUX = 1;
	}else{
		pdm_ptr->Error_I2C_MUX = 0;
	}

	switch(tmp1075_addr){
		case  PDM_I2CADDR_TMP1075_1:
			if( (Error_TMP1075== ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_temp_sensor_1  = ERROR;
			}else{
				pdm_ptr->Error_temp_sensor_1  = SUCCESS;
			}
			break;

		case PDM_I2CADDR_TMP1075_2:
			if( (Error_TMP1075 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_temp_sensor_2 = ERROR;
			}else{
				pdm_ptr->Error_temp_sensor_2 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_TMP1075_3:
			if( (Error_TMP1075== ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_temp_sensor_3 = ERROR;
			}else{
				pdm_ptr->Error_temp_sensor_3 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_TMP1075_4:
			if( (Error_TMP1075 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_temp_sensor_4 = ERROR;
			}else{
				pdm_ptr->Error_temp_sensor_4 = SUCCESS;
			}
			break;

		default:
			break;
	}

	return error_I2C;
}


/** @brief  Init INA231 Power Monitor on PDM module.
    @param 	*pdm_ptr - pointer to struct which contain all information about PDM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  INA231_addr - I2C sensor address
	@param  max_current - maximum current.
	@param  Rshunt - Resistance measuring кesistor
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init_INA231( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t INA231_addr, float max_current, float Rshunt, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t Error_INA231 = ERROR_N;

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	if (error_I2C  == SUCCESS ){

		//Setup INA231
		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			if ( INA231_power_reset( I2Cx, INA231_addr) == SUCCESS ){
				if ( INA231_Set_calibration_float( I2Cx, INA231_addr, max_current, Rshunt) == SUCCESS ){
					if ( INA231_write_aver_mode( I2Cx, INA231_addr, INA231_AVERAGING_NUM_64) == SUCCESS ){
						if ( INA231_write_bus_conv_time( I2Cx, INA231_addr, INA231_CONVERSION_TIME_1100us ) == SUCCESS ){
							if ( INA231_write_shunt_conv_time( I2Cx, INA231_addr, INA231_CONVERSION_TIME_1100us ) == SUCCESS ){

								error_I2C = INA231_write_mode( I2Cx, INA231_addr, INA231_SHUNT_AND_BUS_CONTINUOUS );
							}
						}
					}
				}
			}

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pdm_i2c_delay_att_conn );
			}
		}

		Error_INA231 = error_I2C;
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );


	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		pdm_ptr->Error_I2C_MUX = ERROR;
	}else{
		pdm_ptr->Error_I2C_MUX = SUCCESS;
	}

	switch(INA231_addr){
		case PDM_I2CADDR_PWR_Mon_Ch1:
			if( (Error_INA231 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_PWR_Mon_CH1 = ERROR;
			}else{
				pdm_ptr->Error_PWR_Mon_CH1 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_PWR_Mon_Ch2:
			if( (Error_INA231 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_PWR_Mon_CH2 = ERROR;
			}else{
				pdm_ptr->Error_PWR_Mon_CH2 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_PWR_Mon_Ch3:
			if( (Error_INA231 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_PWR_Mon_CH3 = ERROR;
			}else{
				pdm_ptr->Error_PWR_Mon_CH3 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_PWR_Mon_Ch4:
			if( (Error_INA231 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_PWR_Mon_CH4 = ERROR;
			}else{
				pdm_ptr->Error_PWR_Mon_CH4 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_PWR_Mon_Ch5:
			if( (Error_INA231 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_PWR_Mon_CH5 = ERROR;
			}else{
				pdm_ptr->Error_PWR_Mon_CH5 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_PWR_Mon_Ch6:
			if( (Error_INA231 == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				pdm_ptr->Error_PWR_Mon_CH6 = ERROR;
			}else{
				pdm_ptr->Error_PWR_Mon_CH6 = SUCCESS;
			}
			break;

		default:
			break;
	}

	return error_I2C;
}


/** @brief  Init I2C GPIO extenders (Ext1-U13 and Ext2-U20) on PDM module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tca9539_addr - I2C address GPIO Exneder
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
//ErrorStatus PDM_init_I2C_GPIOExt(I2C_TypeDef *I2Cx, uint8_t tca9539_addr){
//
//	uint8_t i = 0;
//
//	SW_TMUX1209_I2C_main_PDM();// Switch MUX to PDM I2C bus on PMM
//
//	for( i = 0; i < pdm_i2c_attempt_conn; i++){
//
//		if( TCA9539_Reset_output_pin(I2Cx, tca9539_addr, TCA9539_IO_P00|TCA9539_IO_P02|TCA9539_IO_P04|TCA9539_IO_P05|TCA9539_IO_P07\|TCA9539_IO_P11|TCA9539_IO_P12|TCA9539_IO_P14|TCA9539_IO_P16|TCA9539_IO_P17 ) == SUCCESS ){
//			break;
//		}
//
//		LL_mDelay(pdm_i2c_delay_att_conn);
//
//		if( i == (pdm_i2c_attempt_conn - 1) ) {
//			return ERROR_N;
//		}
//	}
//
//
//	for( i = 0; i < pdm_i2c_attempt_conn; i++){
//
//		if( TCA9539_conf_IO_dir_output(I2Cx, tca9539_addr, TCA9539_IO_P00|TCA9539_IO_P02|TCA9539_IO_P04|TCA9539_IO_P05|TCA9539_IO_P07|TCA9539_IO_P11|TCA9539_IO_P12|TCA9539_IO_P14|TCA9539_IO_P16|TCA9539_IO_P17 ) == SUCCESS ){
//			break;
//		}
//
//		LL_mDelay(pdm_i2c_delay_att_conn);
//
//		if( i == (pdm_i2c_attempt_conn - 1) ) {
//			return ERROR_N;
//		}
//	}
//
//
//	for( i = 0; i < pdm_i2c_attempt_conn; i++){
//
//		if(TCA9539_conf_IO_dir_input(I2Cx, tca9539_addr, TCA9539_IO_P01|TCA9539_IO_P03|TCA9539_IO_P06|TCA9539_IO_P10|TCA9539_IO_P13|TCA9539_IO_P15) == SUCCESS ){
//			break;
//		}
//
//		LL_mDelay(pdm_i2c_delay_att_conn);
//
//		if( i == (pdm_i2c_attempt_conn - 1) ) {
//			return ERROR_N;
//		}
//	}
//
//	return SUCCESS;
//}


/** @brief  Init TMP1075 sensor on PDM module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@retval 0 - SUCCESS, -1 - Error temp. sensor , -2  Error I2C MUX
*/
//int8_t PDM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t mux_ch){
//
//	uint8_t i = 0;
//
//	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM
//
//	//Enable I2C MUX channel
//	for( i = 0; i < pdm_i2c_attempt_conn; i++){
//		if( TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, mux_ch ) == SUCCESS ){
//			break;
//		}
//
//		LL_mDelay(pdm_i2c_delay_att_conn);
//
//		if( i == (pdm_i2c_attempt_conn - 1) ) {
//			return -2;
//		}
//	}
//
//	for( i = 0; i < pdm_i2c_attempt_conn; i++){
//
//		if( TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) == SUCCESS ){
//
//			if( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){
//
//				if( TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) == SUCCESS ){
//					break;
//				}
//			}
//		}
//
//		LL_mDelay(pdm_i2c_delay_att_conn);
//
//		if( i == (pdm_i2c_attempt_conn - 1) ) {
//			return -1;
//		}
//	}
//
//	//Disable I2C MUX channel
//	for( i = 0; i < pdm_i2c_attempt_conn; i++){
//
//		if( TCA9548_Disable_I2C_ch( I2Cx, i2c_mux_addr, mux_ch ) == SUCCESS ){
//			break;
//		}
//
//		LL_mDelay(pdm_i2c_delay_att_conn);
//
//		if( i == (pdm_i2c_attempt_conn - 1) ) {
//			return -2;
//		}
//	}
//
//	return 0;
//}
