#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "INA238.h"
#include "PAM/pam_config.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_init_IC.h"

/** @brief  Init TMP1075 sensor on PAM module
    @param 	*pam_ptr - pointer to struct which contain all information about PAM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch - Number channel MUX
	@retval 0 - SUCCESS, -1 - Error temp. sensor , -2  Error I2C MUX
*/
ErrorStatus PAM_init_TMP1075( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr ){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

		if (TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOUS_CONV) == SUCCESS ){
			if ( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){
				error_I2C = TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr);
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}


	switch(tmp1075_addr){
		case  PAM_I2CADDR_TMP1075_1:
			if( error_I2C == ERROR_N ){
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[0] = 0x7F;
				pam_ptr->Error_temp_sensor_1  = ERROR;
			}else{
				pam_ptr->Temp_sensor[0] = 0x00;
				pam_ptr->Error_temp_sensor_1  = SUCCESS;
			}
			break;

		case PAM_I2CADDR_TMP1075_2:
			if( error_I2C == ERROR_N ){
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[1] = 0x7F;
				pam_ptr->Error_temp_sensor_2 = ERROR;
			}else{
				pam_ptr->Temp_sensor[1] = 0x00;
				pam_ptr->Error_temp_sensor_2 = SUCCESS;
			}
			break;

		default:
			break;
	}

	return error_I2C;
}

/** @brief  Init INA238 Power Monitor on PAM module.
    @param 	*pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_pwr_ch - number of channel :
								PAM_PWR_Channel_1
								PAM_PWR_Channel_2
								PAM_PWR_Channel_3
								PAM_PWR_Channel_4
								PAM_PWR_Channel_5
								PAM_PWR_Channel_6
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PAM_init_PWR_Mon_INA238(_PAM *pam_ptr, uint8_t num_pwr_ch){


	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;
	_PAM_table pam_table;

	if( num_pwr_ch > PAM_PWR_IN_Ch_quantity ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PAM

	//Fill pam_table depends in number power channel.
	pam_table = PAM__Table( num_pwr_ch, PAM_TYPE_PWR_CH_IN );

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

		error_I2C = TCA9548_Enable_I2C_ch( pam_table.I2Cx_PORT, pam_table.I2C_addr_I2C_MUX_PWR, pam_table.I2C_MUX_Ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	if (error_I2C  == SUCCESS ){

		//Setup INA231
		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

			if( INA238_Hard_Reset( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon) == SUCCESS ){
				if ( INA238_Setup_Calibration_int16( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Max_Current_int16, pam_table.PWR_Mon_Rshunt_int16, pam_table.PWR_Mon_ADC_Range) == SUCCESS ){
					if ( INA238_Setup_ADCRANGE( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_ADC_Range) == SUCCESS ){
						if ( INA238_Setup_VBUSCT( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Convr_Time) == SUCCESS ){
							if ( INA238_Setup_VSHCT( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Convr_Time) == SUCCESS ){
								if ( INA238_Setup_VTCT( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Convr_Time) == SUCCESS ){
									if ( INA238_Setup_AVG( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Aver_Count) == SUCCESS ){
										error_I2C = INA238_Setup_MODE( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Mode);
									}
								}
							}
						}
					}
				}
			}

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pam_i2c_delay_att_conn );
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( pam_table.I2Cx_PORT, pam_table.I2C_addr_I2C_MUX_PWR, pam_table.I2C_MUX_Ch );


	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_MUX_1 = ERROR;
	}else{
		pam_ptr->Error_I2C_MUX_1 = SUCCESS;
	}

	if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;
	}else{
		pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
	}

	return error_I2C;
}
