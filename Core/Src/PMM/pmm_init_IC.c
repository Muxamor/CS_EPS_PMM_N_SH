#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "INA231.h"
#include "ADS1015.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_struct.h"
#include "PMM/pmm_init_IC.h"

/** @brief  Init TMP1075 temperature sensor at PMM
    @param 	*pmm_ptr - pointer to struct which contain all information about PMM.
    @param 	I2Cx - number I2C port
    @param  tmp1075_addr - I2C address
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_init_TMP1075(_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){


	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	//Setup TMP1075
	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){

		if (TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOUS_CONV) == SUCCESS ){
			if ( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){

				error_I2C = TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr);
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if( error_I2C == ERROR ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pmm_ptr->Temp_sensor = 0x7F;
		pmm_ptr->Error_TMP1075_sensor = ERROR;
	}else{
		pmm_ptr->Temp_sensor = 0x00;
		pmm_ptr->Error_TMP1075_sensor = SUCCESS;
	}

	return error_I2C;
}

/** @brief  Set power down mode for TMP1075 temperature sensor at PMM
    @param 	*pmm_ptr - pointer to struct which contain all information about PMM.
    @param 	I2Cx - number I2C port
    @param  tmp1075_addr - I2C address
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_Power_Down_TMP1075(_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){

		error_I2C = TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_SHUTDOWN_MODE);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if( error_I2C == ERROR ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pmm_ptr->Temp_sensor = 0x7F;
		pmm_ptr->Error_TMP1075_sensor = ERROR;
	}else{

		pmm_ptr->Temp_sensor = 0x00;
		pmm_ptr->Error_TMP1075_sensor = SUCCESS;
	}

	return error_I2C;
}



/** @brief  Init INA231 Power Monitor on PMM module.
    @param 	*pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_ch - number of channel:
							PMM_PWR_Ch_VBAT1_eF1
							PMM_PWR_Ch_VBAT1_eF2 
							PMM_PWR_Ch_VBAT2_eF1
							PMM_PWR_Ch_VBAT2_eF2  
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_init_PWR_Mon_INA231( _PMM *pmm_ptr, uint8_t num_pwr_ch){


	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	_PMM_table pmm_table;

	SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

	//Fill pmm_table depends in number power channel.
	pmm_table = PMM__Table(num_pwr_ch);

	//Setup INA231
	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if ( INA231_Power_Reset( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon) == SUCCESS ){
			if ( INA231_Set_Calibration_int16( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_Max_Current_int16, pmm_table.PWR_Mon_Rshunt_int16) == SUCCESS ){
			    if ( INA231_Setup_AVG( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_aver_mode) == SUCCESS ){
			        if ( INA231_Setup_VbusCT( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_bus_conv_time ) == SUCCESS ){
			            if ( INA231_Setup_VshCT( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_shunt_conv_time ) == SUCCESS ){

			                error_I2C = INA231_Setup_Mode( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_work_mode );
						}
					}
				}
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}


	if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF1 ){

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = SUCCESS;

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = ERROR;
		}

	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF2 ){

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = SUCCESS;

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = ERROR;
		}

	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF1 ){

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = SUCCESS;

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = ERROR;
		}

	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF2 ){

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = SUCCESS;

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = ERROR;
		}

	}

	return error_I2C;
}



/** @brief  Power down INA231 Power Monitor on PMM module.
    @param 	*pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_ch - number of channel:
							PMM_PWR_Ch_VBAT1_eF1
							PMM_PWR_Ch_VBAT1_eF2
							PMM_PWR_Ch_VBAT2_eF1
							PMM_PWR_Ch_VBAT2_eF2
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_PWR_Down_PWR_Mon_INA231( _PMM *pmm_ptr, uint8_t num_pwr_ch){


    uint8_t i = 0;
    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

    _PMM_table pmm_table;

    SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

    //Fill pmm_table depends in number power channel.
    pmm_table = PMM__Table(num_pwr_ch);

    //Setup INA231
    while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

        error_I2C = INA231_Setup_Mode( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, INA231_POWER_DOWN);

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pmm_i2c_delay_att_conn );
        }
    }


    if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF1 ){

        if( error_I2C == SUCCESS ){
            pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = SUCCESS;

        }else{
			#ifdef DEBUGprintf
            Error_Handler();
			#endif
            pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = ERROR;
        }

    }else if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF2 ){

        if( error_I2C == SUCCESS ){
            pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = SUCCESS;

        }else{
			#ifdef DEBUGprintf
            Error_Handler();
			#endif
            pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = ERROR;
        }

    }else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF1 ){

        if( error_I2C == SUCCESS ){
            pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = SUCCESS;

        }else{
			#ifdef DEBUGprintf
            Error_Handler();
			#endif
            pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = ERROR;
        }

    }else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF2 ){

        if( error_I2C == SUCCESS ){
            pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = SUCCESS;

        }else{
			#ifdef DEBUGprintf
            Error_Handler();
			#endif
            pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = ERROR;
        }

    }

    return error_I2C;
}



/** @brief  Init ADS1015 on PMM module.
    @param 	*pmm_ptr - pointer to struct which contain all information about PMM.
    @param  *I2Cx - Number I2C bus.
	@param  I2C_ADS1015_addr - I2C address
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus ADS1015_init( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

	//Setup ADS1015
	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if ( ADS1015_setup_gain_FSR(I2Cx, I2C_ADS1015_addr, ADS1015_FSR_4096mV) == SUCCESS ){
			if ( ADS1015_setup_mux(I2Cx, I2C_ADS1015_addr, ADS1015_AINp0_AINnGND) == SUCCESS ){
				if (ADS1015_setup_conv_mode(I2Cx, I2C_ADS1015_addr, ADS1015_SINGLE_SHOT_MODE) == SUCCESS ){
					if ( ADS1015_setup_conv_data_rate(I2Cx, I2C_ADS1015_addr, ADS1015_1600_SPS) == SUCCESS ){
						if ( ADS1015_setup_comp_mode(I2Cx, I2C_ADS1015_addr, ADS1015_TRADITIONAL_COMP) == SUCCESS ){
							if ( ADS1015_setup_comp_pol(I2Cx, I2C_ADS1015_addr, ADS1015_ACTIVE_LOW_POL) == SUCCESS ){
								if ( ADS1015_setup_latching_comp(I2Cx, I2C_ADS1015_addr, ADS1015_NONLATCHING_COMP) == SUCCESS ){
									error_I2C = ADS1015_setup_comp_queue(I2Cx, I2C_ADS1015_addr, ADS1015_DISABLE_COMPARATOR);
								}
							}
						}
					}
				}
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}


	if( error_I2C == ERROR ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon = ERROR;
	}else{
		pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon = SUCCESS;
	}

	return error_I2C;
}

/** @brief  DeInit I2C GPIO Expander TCA9539. Through I2C write the default value of registers
    @param 	*pmm_ptr - pointer to structure which contain all information about PMM.
    @param  *I2Cx - Number I2C bus.
	@param  tca9539_addr - I2C address TCA9539.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_DeInit_I2C_GPIOExt (_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tca9539_addr){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	//Setup INA231
	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if ( TCA9539_conf_IO_dir_input( I2Cx, tca9539_addr, TCA9539_IO_ALL ) == SUCCESS ){
				error_I2C = TCA9539_Set_output_pin( I2Cx, tca9539_addr, TCA9539_IO_ALL );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if( error_I2C == ERROR ){
		#ifdef DEBUGprintf
			Error_Handler();
        #endif
	    if(tca9539_addr == PMM_I2CADDR_GPIOExt1){
	        pmm_ptr->Error_I2C_GPIO_Ext1 = ERROR;
	    }else if(tca9539_addr == PMM_I2CADDR_GPIOExt2 ){
	        pmm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
	    }

	}else{
	    if(tca9539_addr == PMM_I2CADDR_GPIOExt1){
	        pmm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;
	    }else if(tca9539_addr == PMM_I2CADDR_GPIOExt2 ){
	        pmm_ptr->Error_I2C_GPIO_Ext2 = SUCCESS;
	    }
	}

	return error_I2C;
}


/** @brief  Hard reset TCA9539 GPIOExt, reset pin TCA9539  pull down than no pull is free.
    @param 	tca9539_addr - I2C address GPIO Expander.
	@retval None
*/
void PMM_HARD_Reset_I2C_GPIOExt( uint8_t tca9539_addr ){

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	if( tca9539_addr == PMM_I2CADDR_GPIOExt1){

		GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_2);
		LL_mDelay(1);
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_2);

		GPIO_InitStruct.Pin =  LL_GPIO_PIN_2;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	}else if( tca9539_addr == PMM_I2CADDR_GPIOExt2 ){

	    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
	    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_1);
	    LL_mDelay(1);
	    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_1);

	    GPIO_InitStruct.Pin =  LL_GPIO_PIN_1;
	    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	    LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	}

}

/** @brief  Reset pin TCA9539 GPIOExt pull down.
    @param 	tca9539_addr - I2C address GPIO Expander.
	@retval None
*/
void PMM_Reset_pin_Pull_Down_I2C_GPIOExt( uint8_t tca9539_addr ){

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	if( tca9539_addr == PMM_I2CADDR_GPIOExt1){

		GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
		GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_2);

	}else if( tca9539_addr == PMM_I2CADDR_GPIOExt2 ){
		//TO DO need write
	}
}

/** @brief  Reset pin TCA9539 GPIOExt no pull is free.
    @param 	tca9539_addr - I2C address GPIO Expander.
	@retval None
*/
void PMM_Reset_pin_Free_I2C_GPIOExt( uint8_t tca9539_addr ){

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	if( tca9539_addr == PMM_I2CADDR_GPIOExt1){

		GPIO_InitStruct.Pin =  LL_GPIO_PIN_2;
		GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
		LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	}else if( tca9539_addr == PMM_I2CADDR_GPIOExt2 ){
		//need write
	}
}
