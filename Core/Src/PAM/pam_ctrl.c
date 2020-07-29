#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TCA9539.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "INA231.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_ctrl.h"


/** @brief  Set state (enable/disable) power PAM.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  pwr_source - source (PWR_DC_DC, PWR_LDO).
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Set_state_source_PWR( _PAM *pam_ptr, uint8_t pwr_source, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i=0;
	_PAM_table pam_table;

	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	pam_table = PAM__Table(pwr_source);

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

	if((pwr_source == PAM_PWR_DC_DC) && (pam_ptr->State_DC_DC != state_channel)){
		pam_ptr->PAM_save_conf_flag = 1; //Need save configure in FRAM.
		pam_ptr->State_DC_DC = state_channel;
	}

	if((pwr_source == PAM_PWR_LDO) && (pam_ptr->State_LDO != state_channel)){
		pam_ptr->PAM_save_conf_flag = 1; //Need save configure in FRAM.
		pam_ptr->State_LDO = state_channel;
	}

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

 	//Enable/Disable Efuse power channel.
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

		if( state_channel == ENABLE ){
			if ( TCA9539_Set_output_pin( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF) == SUCCESS ){
				error_I2C = TCA9539_conf_IO_dir_output( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF);
				 if(pam_ptr->PAM_save_conf_flag == 1 ) {
					 LL_mDelay(40); //Delay for startup power supply
				 }
			}
		}else{ //Disable power channel
			if ( TCA9539_Reset_output_pin( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF) == SUCCESS ){
				error_I2C = TCA9539_conf_IO_dir_output( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF);
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay(pam_i2c_delay_att_conn);
		}
	}

	if( error_I2C != SUCCESS ){

		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_GPIO_Ext = ERROR;

		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief  Get Power Good power PAM
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  pwr_source - source (PWR_DC_DC, PWR_LDO).
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_PG_PWR_CH( _PAM *pam_ptr, uint8_t pwr_source ){

	uint8_t i = 0;
	uint8_t read_val_pin_PG_eF = 0;
	int8_t error_I2C = ERROR_N;
	_PAM_table pam_table;

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PDM I2C bus on PMM

	//Get real state value pins TCA9539.
	pam_table = PAM__Table(pwr_source);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read real value input pin PG.

		if( TCA9539_conf_IO_dir_input( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID ) == SUCCESS) {

			error_I2C = TCA9539_read_input_pin(pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID, &read_val_pin_PG_eF );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	if( error_I2C == SUCCESS  ){

		if(pwr_source == PAM_PWR_DC_DC){

			pam_ptr->PG_DC_DC = read_val_pin_PG_eF;

		} else if(pwr_source == PAM_PWR_LDO){

			pam_ptr->PG_LDO = read_val_pin_PG_eF;
		}

	}else{
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_GPIO_Ext = ERROR;
	}

	return error_I2C;
}

/** @brief  Get temperature from TMP1075 sensor.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_Temperature( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

	int8_t temp_value = 0;
	uint8_t i = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N;

	// Switch MUX to PDM I2C bus on PMM
	SW_TMUX1209_I2C_main_PAM();

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	//Read temperature
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = TMP1075_read_int8_temperature( I2Cx, tmp1075_addr, &temp_value);

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pam_i2c_delay_att_conn );
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
		pam_ptr->Error_I2C_MUX_2 = ERROR;
	}else{
		pam_ptr->Error_I2C_MUX_2 = SUCCESS;
	}

	switch( tmp1075_addr ){

		case PAM_I2CADDR_TMP1075_1:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[0] = 0x7F;
				pam_ptr->Error_temp_sensor_1 = ERROR;
			}else{
				pam_ptr->Temp_sensor[0] = temp_value;
				pam_ptr->Error_temp_sensor_1 = SUCCESS; //No error
			}
			break;

		case PAM_I2CADDR_TMP1075_2:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[1] = 0x7F;
				pam_ptr->Error_temp_sensor_2 = ERROR;
			}else{
				pam_ptr->Temp_sensor[1] = temp_value;
				pam_ptr->Error_temp_sensor_2 = SUCCESS; //No error
			}
			break;

		case PAM_I2CADDR_TMP1075_3:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[2] = 0x7F;
				pam_ptr->Error_temp_sensor_3 = ERROR;
			}else{
				pam_ptr->Temp_sensor[2] = temp_value;
				pam_ptr->Error_temp_sensor_3 = SUCCESS; //No error
			}
			break;

		case PAM_I2CADDR_TMP1075_4:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[3] = 0x7F;
				pam_ptr->Error_temp_sensor_4 = ERROR;
			}else{
				pam_ptr->Temp_sensor[3] = temp_value;
				pam_ptr->Error_temp_sensor_4 = SUCCESS; //No error
			}
			break;

		default:
			break;
	}

	return error_I2C;
}

/** @brief  Get value current, voltage and power of Power channel
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_pwr_ch - number power channel.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_PWR_CH_I_V_P( _PAM *pam_ptr, uint8_t num_pwr_ch){

	uint8_t i = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N;
	int16_t val_current = 0;
	uint16_t val_bus_voltage = 0;
	uint16_t val_power = 0;
	_PAM_table pam_table;


	if( num_pwr_ch > PAM_PWR_IN_Ch_quantity ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	// Switch MUX to PAM I2C bus on PAM
	SW_TMUX1209_I2C_main_PAM();

	pam_table = PAM__Table(num_pwr_ch);

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( pam_table.I2Cx_PORT, pam_table.I2C_addr_I2C_MUX_PWR, pam_table.I2C_MUX_Ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	//Read INA231
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = INA231_Get_I_V_P_int16( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Max_Current_int16, &val_current, &val_bus_voltage, &val_power );

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

	if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C INA231 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		val_bus_voltage = 0;
		val_current = 0;
		val_power = 0;
		pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;

	}else{
		if(val_bus_voltage < 5 ){ //If power less than 5mV equate to zero.
			val_bus_voltage = 0;
		}

		if(val_power < 5 ){ //If power less than 5mW equate to zero.
			val_power = 0;
		}

		pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
	}

	pam_ptr->PWR_IN_Channel[num_pwr_ch].Voltage_val = val_bus_voltage;
	pam_ptr->PWR_IN_Channel[num_pwr_ch].Current_val = val_current;
	pam_ptr->PWR_IN_Channel[num_pwr_ch].Power_val = val_power;


	return error_I2C;
}


