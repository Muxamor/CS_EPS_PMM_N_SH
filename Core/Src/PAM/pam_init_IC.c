#include "pam_init_IC.h"

/** @brief  Init TMP1075 sensor on PAM module
    @param 	*pam_ptr - pointer to struct which contain all information about PAM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch - Number channel MUX
	@retval 0 - SUCCESS, -1 - Error temp. sensor , -2  Error I2C MUX
*/
ErrorStatus PAM_init_TMP1075( _PAM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PDM I2C bus on PMM

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

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

			if (TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOUS_CONV) == SUCCESS ){
				if ( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){

					error_I2C = TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr);
				}
			}

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pdm_i2c_delay_att_conn );
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pdm_ptr->Error_I2C_MUX = 1;
	}else{
		pdm_ptr->Error_I2C_MUX = 0;
	}

	switch(tmp1075_addr){
		case  PDM_I2CADDR_TMP1075_1:
			if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pdm_ptr->Temp_sensor[0] = 0x7F;
				pdm_ptr->Error_temp_sensor_1  = ERROR;
			}else{
				pdm_ptr->Temp_sensor[0] = 0x00;
				pdm_ptr->Error_temp_sensor_1  = SUCCESS;
			}
			break;

		case PDM_I2CADDR_TMP1075_2:
			if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pdm_ptr->Temp_sensor[1] = 0x7F;
				pdm_ptr->Error_temp_sensor_2 = ERROR;
			}else{
				pdm_ptr->Temp_sensor[1] = 0x00;
				pdm_ptr->Error_temp_sensor_2 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_TMP1075_3:
			if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pdm_ptr->Temp_sensor[2] = 0x7F;
				pdm_ptr->Error_temp_sensor_3 = ERROR;
			}else{
				pdm_ptr->Temp_sensor[2] = 0x00;
				pdm_ptr->Error_temp_sensor_3 = SUCCESS;
			}
			break;

		case PDM_I2CADDR_TMP1075_4:
			if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pdm_ptr->Temp_sensor[3] = 0x7F;
				pdm_ptr->Error_temp_sensor_4 = ERROR;
			}else{
				pdm_ptr->Temp_sensor[2] = 0x00;
				pdm_ptr->Error_temp_sensor_4 = SUCCESS;
			}
			break;

		default:
			break;
	}

	return error_I2C;
}
