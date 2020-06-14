
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "pdm_struct.h"
#include "pdm_config.h"
#include "TCA9539.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "INA231.h"
#include "pdm_ctrl.h"

/*********************** TODO *********************/
/**
 * 1. В функции PDM_Get_PG_PWR_CH прописать установку напрвления пинов перед тем как читать.(Защита от радиации)

 */
/*****************************************/


/** @brief  Set state (enable/disable) power channel.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  num_pwr_ch - number of channel 
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE 
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Set_state_PWR_CH( _PDM *pdm_ptr, uint8_t num_pwr_ch, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i=0;
	_PDM_table pdm_table;


	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		return ERROR_N;
	}

	if( num_pwr_ch > PDM_PWR_Ch_quantity ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	pdm_table = PDM__Table(num_pwr_ch);

	if( state_channel == ENABLE){
		pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_in = ENABLE; 
		pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_out = ENABLE; 
			
	}else{
		pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_in = DISABLE; 
		pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_out = DISABLE; 
	}
	

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

 	//Enable/Disable INPUT Efuse power channel.
	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){

		if( state_channel == ENABLE ){
			error_I2C = TCA9539_conf_IO_dir_input( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_in_eFuse ); // Input pin -  because hardware auto-enable

		}else if( state_channel == DISABLE ){
			if ( TCA9539_Reset_output_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_in_eFuse ) == SUCCESS ){

				error_I2C = TCA9539_conf_IO_dir_output( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_in_eFuse );
			}
		}
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}

	//Enable/Disable OUTPUT Efuse power channel.
	if( error_I2C == SUCCESS ){
		LL_mDelay(40); //Delay for startup power supply

		i=0;
 		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			if( state_channel == ENABLE ){
				error_I2C = TCA9539_conf_IO_dir_input( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_out_eFuse ); // Input pin -  because hardware auto-enable
				

			}else if( state_channel == DISABLE ){
				if ( TCA9539_Reset_output_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_out_eFuse ) == SUCCESS ){

					error_I2C = TCA9539_conf_IO_dir_output( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_out_eFuse );
				}
			}
 
			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pdm_i2c_delay_att_conn );
			}
		}
	}

	if( error_I2C == SUCCESS ){

		if( PDM_Check_state_PWR_CH( pdm_ptr, num_pwr_ch ) != SUCCESS ){
			return ERROR_N;
		}

	}else{ //Error I2C GPIO Expander

		if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

		pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_in = ERROR; 
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_out = ERROR;

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief  Checking the state of the power channel. OK- If there is no difference between
			the set value and the actual value set. ERROR - If there are differences between 
			the set value and the actual set.
			actual value set - Get by reading the real value I2C GPIO Expander.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  num_pwr_ch - number channel on/off
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Check_state_PWR_CH( _PDM *pdm_ptr, uint8_t num_pwr_ch ){

	uint8_t read_val_pin_EN_in_eF = 0;
	uint8_t read_val_pin_EN_out_eF = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1 - ERROR_N
	uint8_t i = 0;
	_PDM_table pdm_table;

	if( num_pwr_ch > PDM_PWR_Ch_quantity ){
		return ERROR_N;
	}

	// Switch MUX to PDM I2C bus on PMM 
	SW_TMUX1209_I2C_main_PDM(); 

	//Get real state value pins TCA9539. 	
	pdm_table = PDM__Table(num_pwr_ch);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read real value output pin.  

		error_I2C =  TCA9539_read_input_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_in_eFuse, &read_val_pin_EN_in_eF);

		if( error_I2C == SUCCESS ){
			 error_I2C = TCA9539_read_input_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_EN_out_eFuse, &read_val_pin_EN_out_eF);
		}
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}


	if( error_I2C == SUCCESS){

		if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;

		}else if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = SUCCESS;
		}

		if( pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_in  != read_val_pin_EN_in_eF ){
			pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_in = ERROR; 
		}else{
			pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_in = SUCCESS;
		}

		if( pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_out  != read_val_pin_EN_out_eF ){
			pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_out = ERROR; 
		}else{
			pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_out = SUCCESS;
		}


	}else{

		if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

		pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_in = ERROR; 
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_State_eF_out = ERROR;

	}

	return error_I2C;
}



/** @brief  Get Power Good power channel status 
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  num_pwr_ch - number power channel
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_PG_PWR_CH( _PDM *pdm_ptr, uint8_t num_pwr_ch ){

	uint8_t i = 0;
	uint8_t read_val_pin_PG_in_eF = 1;
	uint8_t read_val_pin_PG_out_eF = 1;
	int8_t error_I2C = ERROR_N; 
	_PDM_table pdm_table;

	if( num_pwr_ch > PDM_PWR_Ch_quantity ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	//Get real state value pins TCA9539. 	
	pdm_table = PDM__Table(num_pwr_ch);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read real value input pin PG.  
//Add set direction!!!!
		if( TCA9539_read_input_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_PG_in_eFuse, &read_val_pin_PG_in_eF) == SUCCESS ){

			error_I2C = TCA9539_read_input_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_PG_out_eFuse, &read_val_pin_PG_out_eF);
		}
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}

	if( error_I2C == SUCCESS  ){

		if( (pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_in  == ENABLE ) && (pdm_ptr->PWR_Channel[num_pwr_ch].State_eF_out == ENABLE) ){

			pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
			pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_out = read_val_pin_PG_out_eF;

		}else{

			pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_in = SUCCESS;  // OK because power channel is DISABLE
			pdm_ptr->PWR_Channel[num_pwr_ch].PG_eF_out = SUCCESS;

		}
	}else{
		
		if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

	}

	return error_I2C;
}


/** @brief  Get temperature from TMP1075 sensor.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address 
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_Temperature( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

	int8_t temp_value;
	uint8_t i = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N; 

	// Switch MUX to PDM I2C bus on PMM
	SW_TMUX1209_I2C_main_PDM();

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

	//Read temperature
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = TMP1075_read_int8_temperature( I2Cx,tmp1075_addr, &temp_value);

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
		pdm_ptr->Error_I2C_MUX = ERROR;
	}else{
		pdm_ptr->Error_I2C_MUX = SUCCESS;
	}

	switch( tmp1075_addr ){

		case PDM_I2CADDR_TMP1075_1:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				pdm_ptr->Temp_sensor_1 = 0x7F;
				pdm_ptr->Error_temp_sensor_1 = ERROR;
			}else{
				pdm_ptr->Temp_sensor_1 = temp_value;
				pdm_ptr->Error_temp_sensor_1 = SUCCESS; //No error
			}
			break;

		case PDM_I2CADDR_TMP1075_2:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				pdm_ptr->Temp_sensor_2 = 0x7F;
				pdm_ptr->Error_temp_sensor_2 = ERROR;
			}else{
				pdm_ptr->Temp_sensor_2 = temp_value;
				pdm_ptr->Error_temp_sensor_2 = SUCCESS; //No error
			}
			break;

		case PDM_I2CADDR_TMP1075_3:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				pdm_ptr->Temp_sensor_3 = 0x7F;
				pdm_ptr->Error_temp_sensor_3 = ERROR;
			}else{
				pdm_ptr->Temp_sensor_3 = temp_value;
				pdm_ptr->Error_temp_sensor_3 = SUCCESS; //No error
			}
			break;

		case PDM_I2CADDR_TMP1075_4:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				pdm_ptr->Temp_sensor_4 = 0x7F;
				pdm_ptr->Error_temp_sensor_4 = ERROR;
			}else{
				pdm_ptr->Temp_sensor_3 = temp_value;
				pdm_ptr->Error_temp_sensor_4 = SUCCESS; //No error
			}
			break;

		default:
			break;
	}

	return error_I2C;
}


/** @brief  Get value current, voltage and power of Power channel
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  num_pwr_ch - number power channel.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_I_V_P( _PDM *pdm_ptr, uint8_t num_pwr_ch){

	uint8_t i = 0;
	int16_t val_current = 0;
	uint16_t val_bus_voltage = 0;
	uint16_t val_power = 0;
	_PDM_table pdm_table;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N;
	

	if( num_pwr_ch > PDM_PWR_Ch_quantity ){
		return ERROR_N;
	}

	// Switch MUX to PDM I2C bus on PMM 
	SW_TMUX1209_I2C_main_PDM(); 

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

	//Read temperature
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = INA231_Get_I_V_P_int16( pdm_table.I2Cx_PWR_Mon, pdm_table.I2C_addr_PWR_Mon, pdm_table.PWR_Mon_Max_Current_int16, &val_current, &val_bus_voltage, &val_power );

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
		pdm_ptr->Error_I2C_MUX = ERROR;
	}else{
		pdm_ptr->Error_I2C_MUX = SUCCESS;
	}

	if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C INA231 or I2C MUX
		pdm_ptr->PWR_Channel[num_pwr_ch].Voltage_val = 0;
		pdm_ptr->PWR_Channel[num_pwr_ch].Current_val = 0;
		pdm_ptr->PWR_Channel[num_pwr_ch].Power_val = 0;
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;
	}else{
		pdm_ptr->PWR_Channel[num_pwr_ch].Voltage_val = val_bus_voltage;
		pdm_ptr->PWR_Channel[num_pwr_ch].Current_val = val_current;
		pdm_ptr->PWR_Channel[num_pwr_ch].Power_val = val_power;
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
	}

	return error_I2C;
}


