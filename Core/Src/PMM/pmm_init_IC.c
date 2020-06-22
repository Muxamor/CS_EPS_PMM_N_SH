
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "INA231.h"
#include "pmm_config.h"
#include "pmm_struct.h"

#include "pmm_init_IC.h"

/*********************** TODO *********************/
//1. Change or delate fn. PMM_default_init_I2C_GPIOExt1 - temporary function for debugging period

/**************************************************/

ErrorStatus PMM_init_TMP1075(_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

	
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	//Setup INA231
	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if (TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) == SUCCESS ){
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
		pmm_ptr->Error_TMP1075_sensor = 1;
	}else{
		pmm_ptr->Error_TMP1075_sensor = 0;
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

		if ( INA231_power_reset( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon) == SUCCESS ){
			if ( INA231_Set_calibration_int16( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_Max_Current_int16, pmm_table.PWR_Mon_Rshunt_int16) == SUCCESS ){
				if ( INA231_write_aver_mode( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_aver_mode) == SUCCESS ){
					if ( INA231_write_bus_conv_time( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_bus_conv_time ) == SUCCESS ){
						if ( INA231_write_shunt_conv_time( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_shunt_conv_time ) == SUCCESS ){

							error_I2C = INA231_write_mode( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_work_mode );
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




//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TEMP   FN !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/** @brief  Default Init I2C GPIO extenders ( Ext1 (U3) ) on PMM module.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tca9539_addr - I2C address GPIO Exneder
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_default_init_I2C_GPIOExt1 (I2C_TypeDef *I2Cx, uint8_t tca9539_addr){ 

	uint8_t i = 0;

	SW_TMUX1209_I2C_main_PMM();// Switch MUX to pmm I2C bus on PMM

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
