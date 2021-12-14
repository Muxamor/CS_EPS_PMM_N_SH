#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "INA238.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_struct.h"
#include "PBM_T1/pbm_T1_init_IC.h"
#include "TCA9548.h"
#include "Error_Handler.h"

/** @brief	Initialize TMP1075 sensor for selected heat.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
   	@param 	Heat_number - select number Heat (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  sensor_number - I2C sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval Error status
 */
ErrorStatus PBM_T1_Init_Heat_TMP1075( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number, uint8_t sensor_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);
		if( Error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_I2C;

	if (Error_I2C == SUCCESS ){

		//Setup TMP1075
		i=0;
		Error_I2C = ERROR_N;

		while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

			if ( TMP1075_set_mode(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], pbm_table.TMP1075_Mode) == SUCCESS) {
				if ( TMP1075_ALERT_active_level(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], pbm_table.TMP1075_Alert_Level) == SUCCESS) {
					if ( TMP1075_set_mode_ALERT_pin(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], pbm_table.TMP1075_Mode_Alert) == SUCCESS) {
						if ( TMP1075_set_time_conversion(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], pbm_table.TMP1075_Convr_Time) == SUCCESS) {
							if ( TMP1075_ALERT_sensitivity(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], pbm_table.TMP1075_Alert_Sens) == SUCCESS) {
								if ( TMP1075_set_low_limit(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], PBM_T1_TMP1075_TEMP_LO) == SUCCESS) {
									Error_I2C = TMP1075_set_high_limit(I2Cx, pbm_table.TempSens_Heat_Addr[sensor_number], PBM_T1_TMP1075_TEMP_HI);
								}
							}
						}
					}
				}
			}

			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Heat[Heat_number].Heat_TMP1075[sensor_number] = 0x7F;
		pbm[PBM_number].Heat[Heat_number].Error_Heat_TMP1075[sensor_number]= ERROR;
	}else{
		pbm[PBM_number].Heat[Heat_number].Error_Heat_TMP1075[sensor_number] = SUCCESS; //No error
	}

	if( Error_I2C != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Initialize TMP1075 sensor for selected PBM.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
   	@param 	Heat_Branch - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  temp_number - I2C sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval Error status
 */
/*ErrorStatus PBM_T1_Init_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t temp_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, 0);

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_TempSens);
		if( Error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_I2C;

	if (Error_I2C == SUCCESS ){

		//Setup TMP1075
		i=0;
		Error_I2C = ERROR_N;

		while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

			if (TMP1075_set_mode(I2Cx, pbm_table.TempSens_Addr[temp_number], pbm_table.TMP1075_Mode) == SUCCESS ){
				if ( TMP1075_set_time_conversion(I2Cx, pbm_table.TempSens_Addr[temp_number], pbm_table.TMP1075_Convr_Time) == SUCCESS ){

					Error_I2C = TMP1075_disable_ALERT_pin( I2Cx, pbm_table.TempSens_Addr[temp_number]);
				}
			}

			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_TempSens);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].TMP1075_Temp[temp_number] = 0x7F;
		pbm[PBM_number].Error_TMP1075[temp_number]= ERROR;
	}else{
		pbm[PBM_number].Error_TMP1075[temp_number] = SUCCESS; //No error
	}

	if( Error_I2C != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}*/

/** @brief	Initialize power monitor INA238 for selected heat.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
   	@param 	Heat_number - select number heat (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval Error status
 */
ErrorStatus PBM_T1_Init_Heat_INA238(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_PwrMon);
		if( Error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_I2C;

	if (Error_I2C == SUCCESS ){

		//Setup TMP1075
		i=0;
		Error_I2C = ERROR_N;

		while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

			if( INA238_Hard_Reset( I2Cx, pbm_table.PwrMon_Addr) == SUCCESS ){
				if ( INA238_Setup_Calibration_int16( I2Cx, pbm_table.PwrMon_Addr, PBM_T1_INA238_MAX_CURRENT, PBM_T1_INA238_RSHUNT) == SUCCESS ){
					if ( INA238_Setup_ADCRANGE( I2Cx, pbm_table.PwrMon_Addr, pbm_table.INA238_ADC_Range) == SUCCESS ){
						if ( INA238_Setup_VBUSCT( I2Cx, pbm_table.PwrMon_Addr, pbm_table.INA238_Convr_Time) == SUCCESS ){
							if ( INA238_Setup_VSHCT( I2Cx, pbm_table.PwrMon_Addr, pbm_table.INA238_Convr_Time) == SUCCESS ){
								if ( INA238_Setup_VTCT( I2Cx, pbm_table.PwrMon_Addr, pbm_table.INA238_Convr_Time) == SUCCESS ){
									if ( INA238_Setup_AVG( I2Cx, pbm_table.PwrMon_Addr, pbm_table.INA238_Aver_Count) == SUCCESS ){
										Error_I2C = INA238_Setup_MODE( I2Cx, pbm_table.PwrMon_Addr, pbm_table.INA238_Mode);
									}
								}
							}
						}
					}
				}
			}

			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_PwrMon);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Heat[Heat_number].Error_INA238 = ERROR;
	}else{
		pbm[PBM_number].Heat[Heat_number].Error_INA238 = SUCCESS; //No error
	}

	if( Error_I2C != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Initialize TCA9548 for selected PBM.
   	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
    @retval Error status
 */
ErrorStatus PBM_T1_Init_I2CMux(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	pbm_table = PBM_T1_Table(PBM_number, 0, 0);

	SW_TMUX1209_I2C_main_PBM();

	while( ( error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

		error_I2C = TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, TCA9548_ALL_CHANNELS);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	if( error_I2C == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		return ERROR_N;
	}

    return SUCCESS;
}
