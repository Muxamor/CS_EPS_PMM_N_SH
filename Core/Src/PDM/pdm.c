#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "pdm_struct.h"
#include "pdm_config.h"
#include "TCA9539.h"
#include "TMP1075.h"
#include "pdm.h"


/** @brief  Set state (enable/disable) power channel.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  number_pwr_channel - number of channel :
								PDM_PWR_Channel_1 
								PDM_PWR_Channel_2
								PDM_PWR_Channel_3
								PDM_PWR_Channel_4
								PDM_PWR_Channel_5
								PDM_PWR_Channel_6 

	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE 
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Set_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i=0;
	_PDM_pins pdm_pins;


	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		return ERROR_N;
	}

	if( number_pwr_channel > PDM_PWR_Channel_6 ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	pdm_pins = PDM_pin_table(number_pwr_channel);
	
	switch(number_pwr_channel){

		case PDM_PWR_Channel_1:

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH1_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH1_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH1_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH1_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_2:

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH2_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH2_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH2_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH2_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_3:

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH3_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH3_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH3_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH3_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_4:

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH4_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH4_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH4_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH4_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_5:

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH5_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH5_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH5_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH5_State_eF_out = DISABLE; 
			} 
			break;

		case PDM_PWR_Channel_6:

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH6_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH6_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH6_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH6_State_eF_out = DISABLE; 
			}
			break;

		default:
			break;
	}

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( state_channel == ENABLE ){

			error_I2C = TCA9539_conf_IO_dir_input( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse ); // Input pin -  because hardware auto-enable

		}else if( state_channel == DISABLE ){
			if ( TCA9539_Reset_output_pin( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse ) == SUCCESS ){

				error_I2C = TCA9539_conf_IO_dir_output( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse );
			}
		}
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}

	if( error_I2C == SUCCESS ){//Enable/Disable OUTPUT Efuse power channel.

		LL_mDelay(40); //Delay for startup power supply

		i=0;
 		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			if( state_channel == ENABLE ){
				error_I2C = TCA9539_conf_IO_dir_input( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse ); // Input pin -  because hardware auto-enable
				

			}else if( state_channel == DISABLE ){
				if ( TCA9539_Reset_output_pin( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse ) == SUCCESS ){

					error_I2C = TCA9539_conf_IO_dir_output( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse );
				}
			}
 
			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pdm_i2c_delay_att_conn );
			}
		}
	}

	if( error_I2C == SUCCESS ){

		if( PDM_Check_state_PWR_CH( pdm_ptr, number_pwr_channel ) != SUCCESS ){
			return ERROR_N;
		}

		/*if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;

		}else if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = SUCCESS;
		} */

	}else{ //Error I2C GPIO Expander

		if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief  Checking the state of the power channel. OK- If there is no difference between
			the set value and the actual value set. ERROR - If there are differences between 
			the set value and the actual set.
			actual value set - Get by reading the real value I2C GPIO Expander.
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  number_pwr_channel - number channel on/off:
								PDM_PWR_Channel_1 
								PDM_PWR_Channel_2
								PDM_PWR_Channel_3
								PDM_PWR_Channel_4
								PDM_PWR_Channel_5
								PDM_PWR_Channel_6 
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Check_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel ){

	uint8_t read_val_pin_EN_in_eF = 0;
	uint8_t read_val_pin_EN_out_eF = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1 - ERROR_N
	uint8_t i = 0;
	_PDM_pins pdm_pins;

	if( number_pwr_channel > PDM_PWR_Channel_6 ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	//Get real state value pins TCA9539. 	
	pdm_pins = PDM_pin_table(number_pwr_channel);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read real value output pin.  

		error_I2C =  TCA9539_read_input_pin( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_in_eFuse, &read_val_pin_EN_in_eF);

		if( error_I2C == SUCCESS ){
			 error_I2C = TCA9539_read_input_pin( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_EN_out_eFuse, &read_val_pin_EN_out_eF);
		}
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}


	if( error_I2C == SUCCESS){

		if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;

		}else if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = SUCCESS;
		}

		switch(number_pwr_channel){

			case PDM_PWR_Channel_1:

				if( pdm_ptr->PWR_CH1_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH1_State_eF_in = 1; //Error
				}else{
					pdm_ptr->Error_PWR_CH1_State_eF_in = 0; //OK
				}

				if( pdm_ptr->PWR_CH1_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH1_State_eF_out = 1;//Error
				}else{
					pdm_ptr->Error_PWR_CH1_State_eF_out = 0;//OK
				}
				break;

			case PDM_PWR_Channel_2:

				if( pdm_ptr->PWR_CH2_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH2_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH2_State_eF_in = 0;
				}

				if( pdm_ptr->PWR_CH2_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH2_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH2_State_eF_out = 0;
				}
				break;

			case PDM_PWR_Channel_3:

				if( pdm_ptr->PWR_CH3_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH3_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH3_State_eF_in = 0;
				}

				if( pdm_ptr->PWR_CH3_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH3_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH3_State_eF_out = 0;
				}
				break;

			case PDM_PWR_Channel_4:

				if( pdm_ptr->PWR_CH4_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH4_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH4_State_eF_in = 0;
				}

				if( pdm_ptr->PWR_CH4_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH4_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH4_State_eF_out = 0;
				}
				break;

			case PDM_PWR_Channel_5:

				if( pdm_ptr->PWR_CH5_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH5_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH5_State_eF_in = 0;
				}

				if( pdm_ptr->PWR_CH5_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH5_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH5_State_eF_out = 0;
				}
				break;

			case PDM_PWR_Channel_6:

				if( pdm_ptr->PWR_CH6_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH6_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH6_State_eF_in = 0;
				}

				if( pdm_ptr->PWR_CH6_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH6_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH6_State_eF_out = 0;
				}
				break;

			default:
				break;
		}

	}else{

		if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_pins.I2C_tca9539_addr == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

		return ERROR_N;
	}

	return SUCCESS;
}



/** @brief  Get Power Good power channel status 
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@param  number_pwr_channel - number channel on/off:
								PDM_PWR_Channel_1 
								PDM_PWR_Channel_2
								PDM_PWR_Channel_3
								PDM_PWR_Channel_4
								PDM_PWR_Channel_5
								PDM_PWR_Channel_6 
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_PG_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel ){


	uint8_t i = 0;
	uint8_t read_val_pin_PG_in_eF = 1;
	uint8_t read_val_pin_PG_out_eF = 1;
	int8_t error_I2C = ERROR_N; 
	_PDM_pins pdm_pins;


	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	//Get real state value pins TCA9539. 	
	pdm_pins = PDM_pin_table(number_pwr_channel);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read real value input pin PG.  

		error_I2C =  TCA9539_read_input_pin( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_PG_in_eFuse, &read_val_pin_PG_in_eF);

		if( error_I2C == SUCCESS ){
			 error_I2C = TCA9539_read_input_pin( pdm_pins.I2Cx, pdm_pins.I2C_tca9539_addr, pdm_pins.pin_PG_out_eFuse, &read_val_pin_PG_out_eF);
		}
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}


		switch( number_pwr_channel ){

		case  PDM_PWR_Channel_1:
			if( (pdm_ptr->PWR_CH1_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH1_State_eF_out == ENABLE) ){

				 pdm_ptr->PWR_CH1_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH1_PG_eF_out = read_val_pin_PG_out_eF;
			}else{

				pdm_ptr->PWR_CH1_PG_eF_in = 0;  // OK because power channel is not Enable
				pdm_ptr->PWR_CH1_PG_eF_out = 0;
			}
			break;

		case  PDM_PWR_Channel_2:
			if( (pdm_ptr->PWR_CH2_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH2_State_eF_out == ENABLE) ){

				 pdm_ptr->PWR_CH2_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH2_PG_eF_out = read_val_pin_PG_out_eF;
			}else{

				pdm_ptr->PWR_CH2_PG_eF_in = 0;
				pdm_ptr->PWR_CH2_PG_eF_out = 0;
			}
			break;

		case  PDM_PWR_Channel_3:
			if( (pdm_ptr->PWR_CH3_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH3_State_eF_out == ENABLE) ){

				 pdm_ptr->PWR_CH3_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH3_PG_eF_out = read_val_pin_PG_out_eF;
			}else{

				pdm_ptr->PWR_CH3_PG_eF_in = 0;
				pdm_ptr->PWR_CH3_PG_eF_out = 0;
			}
			break;

		case  PDM_PWR_Channel_4:
			if( (pdm_ptr->PWR_CH4_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH4_State_eF_out == ENABLE) ){

				 pdm_ptr->PWR_CH4_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH4_PG_eF_out = read_val_pin_PG_out_eF;
			}else{

				pdm_ptr->PWR_CH4_PG_eF_in = 0;
				pdm_ptr->PWR_CH4_PG_eF_out = 0;
			}
			break;

		case  PDM_PWR_Channel_5:
			if( (pdm_ptr->PWR_CH5_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH5_State_eF_out == ENABLE) ){

				 pdm_ptr->PWR_CH5_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH5_PG_eF_out = read_val_pin_PG_out_eF;
			}else{

				pdm_ptr->PWR_CH5_PG_eF_in = 0;
				pdm_ptr->PWR_CH5_PG_eF_out = 0;
			}
			break;

		case  PDM_PWR_Channel_6:
			if( (pdm_ptr->PWR_CH6_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH6_State_eF_out == ENABLE) ){

				 pdm_ptr->PWR_CH6_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH6_PG_eF_out = read_val_pin_PG_out_eF;
			}else{

				pdm_ptr->PWR_CH6_PG_eF_in = 0;
				pdm_ptr->PWR_CH6_PG_eF_out = 0;
			}
			break;

		default:
			error_I2C = ERROR_N;
			break;
	


		}

	return error_I2C;
}

/** @brief  Get Power Good power channel status 
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*//*
ErrorStatus PDM_Get_PG_ALL_PWR_CH( _PDM *pdm_ptr ){
	
}

*/



/** @brief  Get temperature from temp. sensor on PDM . 
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_temp_all_sensor( _PDM *pdm_ptr ){

	//SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 
	int8_t error_status; 

	error_status = PDM_Get_temp_TMP1075( PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1, pdm_ptr);
	error_status += PDM_Get_temp_TMP1075( PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2, pdm_ptr);
	error_status += PDM_Get_temp_TMP1075( PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3, pdm_ptr);
	error_status += PDM_Get_temp_TMP1075( PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4, pdm_ptr);

	if(error_status != SUCCESS ){
		return ERROR_N; 
	}

	return SUCCESS;
}


/** @brief  Get temperature from TMP1075 sensor. 
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address 
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_temp_TMP1075( I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, _PDM *pdm_ptr ){

	int8_t temp_value;
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; 

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read temperature.

		error_I2C = TMP1075_read_int8_temperature( I2Cx,tmp1075_addr, &temp_value);
 
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pdm_i2c_delay_att_conn );
		}
	}

	switch( tmp1075_addr ){

		case PDM_I2CADDR_TMP1075_1:

			if( error_I2C == SUCCESS ){
				pdm_ptr->Temp_sensor_1 = temp_value;
				pdm_ptr->Error_temp_sensor_1 = SUCCESS; //No error
			}else{ //Error case 
				pdm_ptr->Temp_sensor_1 = 0x7F;
				pdm_ptr->Error_temp_sensor_1 = ERROR; //No error
			}
			break;

		case PDM_I2CADDR_TMP1075_2:

			if(error_I2C == SUCCESS ){
				pdm_ptr->Temp_sensor_2 = temp_value;
				pdm_ptr->Error_temp_sensor_2 = SUCCESS; //No error
			}else{ //Error case 
				pdm_ptr->Temp_sensor_2 = 0x7F;
				pdm_ptr->Error_temp_sensor_2 = ERROR; //No error
			}
			break;

		case PDM_I2CADDR_TMP1075_3:

			if( error_I2C == SUCCESS){
				pdm_ptr->Temp_sensor_3 = temp_value;
				pdm_ptr->Error_temp_sensor_3 = SUCCESS; //No error
			}else{ //Error case 
				pdm_ptr->Temp_sensor_3 = 0x7F;
				pdm_ptr->Error_temp_sensor_3 = ERROR; //No error
			}
			break;

		case PDM_I2CADDR_TMP1075_4:

			if( error_I2C == SUCCESS ){
				pdm_ptr->Temp_sensor_3 = temp_value;
				pdm_ptr->Error_temp_sensor_3 = SUCCESS; //No error
			}else{ //Error case 
				pdm_ptr->Temp_sensor_3 = 0x7F;
				pdm_ptr->Error_temp_sensor_3 = ERROR; //No error
			}
			break;

		default:
			break;
	}

	return error_I2C;
}





