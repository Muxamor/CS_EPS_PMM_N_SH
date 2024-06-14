#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "INA238.h"
#include "PDM/pdm_config.h"
#include "PDM/pdm_struct.h"
#include "PDM/pdm_init_IC.h"


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
		if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = ERROR;
		}else if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = ERROR;
		}
	}else{
		if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = SUCCESS;
		}else if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = SUCCESS;
		}
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


/** @brief Set power down mode for TMP1075 temperature sensor at PDM module
    @param 	*pdm_ptr - pointer to struct which contain all information about PDM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch - Number channel MUX
	@retval 0 - SUCCESS, -1 - Error temp. sensor , -2  Error I2C MUX
*/
ErrorStatus PDM_Power_Down_TMP1075( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t Error_I2C_MUX = ERROR_N;

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

            error_I2C = TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_SHUTDOWN_MODE);

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
		if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = ERROR;
		}else if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = ERROR;
		}
	}else{
		if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = SUCCESS;
		}else if(i2c_mux_addr == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = SUCCESS;
		}
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




/** @brief  Init INA231 Power Monitor on PDM module.
    @param 	*pdm_ptr - pointer to struct which contain all information about PDM.
	@param  num_pwr_ch - number of channel :
								PDM_PWR_Channel_1 
								PDM_PWR_Channel_2
								PDM_PWR_Channel_3
								PDM_PWR_Channel_4
								PDM_PWR_Channel_5
								PDM_PWR_Channel_6 
								PDM_PWR_Channel_7
								PDM_PWR_Channel_8
								PDM_PWR_Channel_9
								PDM_PWR_Channel_10
								PDM_PWR_Channel_11
								PDM_PWR_Channel_12
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_init_PWR_Mon_INA238( _PDM *pdm_ptr, uint8_t num_pwr_ch){


	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;
	_PDM_table pdm_table;

	if( num_pwr_ch > PDM_PWR_Ch_quantity ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PDM

	//Fill pdm_table depends in number power channel.
	pdm_table = PDM__Table(num_pwr_ch); 

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( pdm_table.I2Cx_I2C_MUX, pdm_table.I2C_addr_I2C_MUX, pdm_table.I2C_MUX_Ch );

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

			if( INA238_Hard_Reset( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon) == SUCCESS ){
				if ( INA238_Setup_Calibration_int16( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Max_Current_int16, pdm_table.PWR_Mon_Rshunt_int16) == SUCCESS ){
					if ( INA238_Setup_ADCRANGE( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_ADC_Range) == SUCCESS ){
						if ( INA238_Setup_VBUSCT( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Convr_Time) == SUCCESS ){
							if ( INA238_Setup_VSHCT( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Convr_Time) == SUCCESS ){
								if ( INA238_Setup_VTCT( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Convr_Time) == SUCCESS ){
									if ( INA238_Setup_AVG( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Aver_Count) == SUCCESS ){
										error_I2C = INA238_Setup_MODE( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Mode);
									}
								}
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
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( pdm_table.I2Cx_I2C_MUX, pdm_table.I2C_addr_I2C_MUX, pdm_table.I2C_MUX_Ch );

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = ERROR;
		}else if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = ERROR;
		}
	}else{
		if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = SUCCESS;
		}else if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = SUCCESS;
		}
	}

	if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;
	}else{
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
	}

	return error_I2C;
}


/** @brief  Power down INA231 Power Monitor on PDM module.
    @param 	*pdm_ptr - pointer to struct which contain all information about PDM.
	@param  num_pwr_ch - number of channel :
								PDM_PWR_Channel_1
								PDM_PWR_Channel_2
								PDM_PWR_Channel_3
								PDM_PWR_Channel_4
								PDM_PWR_Channel_5
								PDM_PWR_Channel_6
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PDM_PWR_Down_PWR_Mon_INA238( _PDM *pdm_ptr, uint8_t num_pwr_ch){


    uint8_t i = 0;
    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t Error_I2C_MUX = ERROR_N;
    _PDM_table pdm_table;

    if( num_pwr_ch > PDM_PWR_Ch_quantity ){
		#ifdef DEBUGprintf
        Error_Handler();
		#endif
        return ERROR_N;
    }

    SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PDM

    //Fill pdm_table depends in number power channel.
    pdm_table = PDM__Table(num_pwr_ch);

    //Enable I2C MUX channel
    i=0;
    error_I2C = ERROR_N;
    while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

        error_I2C = TCA9548_Enable_I2C_ch( pdm_table.I2Cx_I2C_MUX, pdm_table.I2C_addr_I2C_MUX, pdm_table.I2C_MUX_Ch );

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

            error_I2C = INA238_Setup_MODE( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, INA238_SHUTDOWN );

            if( error_I2C != SUCCESS ){
                i++;
                LL_mDelay( pdm_i2c_delay_att_conn );
            }
        }
    }

    //Disable I2C MUX channel.
    //Note: Do not check the error since it doesn’t matter anymore.
    TCA9548_Disable_I2C_ch( pdm_table.I2Cx_I2C_MUX, pdm_table.I2C_addr_I2C_MUX, pdm_table.I2C_MUX_Ch );

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = ERROR;
		}else if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = ERROR;
		}
	}else{
		if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX1 ){
			pdm_ptr->Error_I2C_MUX_1 = SUCCESS;
		}else if(pdm_table.I2C_addr_I2C_MUX == PDM_I2CADDR_I2C_MUX2){
			pdm_ptr->Error_I2C_MUX_2 = SUCCESS;
		}
	}

    if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
		#ifdef DEBUGprintf
        Error_Handler();
		#endif
        pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;
    }else{
        pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
    }

    return error_I2C;
}
