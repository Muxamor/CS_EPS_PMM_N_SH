
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
 * 3. Подумать над тем чтобы функцию PDM_Get_temp_all_sensor убрать в функцию PDM_Get_telemetry.
 * 4. В PDM_Set_state_PWR_CH подумать над тем чтобы выставлять ошибку канал если  получили ошибку связи GPIO expander
 */
/*****************************************/


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
	_PDM_table pdm_table;


	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		return ERROR_N;
	}

	if( number_pwr_channel > PDM_PWR_Channel_6 ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	pdm_table = PDM__Table(number_pwr_channel);
	
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

		if( PDM_Check_state_PWR_CH( pdm_ptr, number_pwr_channel ) != SUCCESS ){
			return ERROR_N;
		}

	}else{ //Error I2C GPIO Expander

		if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

		switch(number_pwr_channel){

			case PDM_PWR_Channel_1:
				pdm_ptr->Error_PWR_CH1_State_eF_in = 1; //Error
				pdm_ptr->Error_PWR_CH1_State_eF_out = 1;//Error
				break;

			case PDM_PWR_Channel_2:
				pdm_ptr->Error_PWR_CH2_State_eF_in = 1; //Error
				pdm_ptr->Error_PWR_CH2_State_eF_out = 1;//Error
				break;

			case PDM_PWR_Channel_3:
				pdm_ptr->Error_PWR_CH3_State_eF_in = 1; //Error
				pdm_ptr->Error_PWR_CH3_State_eF_out = 1;//Error
				break;

			case PDM_PWR_Channel_4:
				pdm_ptr->Error_PWR_CH4_State_eF_in = 1; //Error
				pdm_ptr->Error_PWR_CH4_State_eF_out = 1;//Error
				break;

			case PDM_PWR_Channel_5:
				pdm_ptr->Error_PWR_CH5_State_eF_in = 1; //Error
				pdm_ptr->Error_PWR_CH5_State_eF_out = 1;//Error
				break;

			case PDM_PWR_Channel_6:
				pdm_ptr->Error_PWR_CH6_State_eF_in = 1; //Error
				pdm_ptr->Error_PWR_CH6_State_eF_out = 1;//Error
				break;

			default:
				break;
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
	_PDM_table pdm_table;

	if( number_pwr_channel > PDM_PWR_Channel_6 ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	//Get real state value pins TCA9539. 	
	pdm_table = PDM__Table(number_pwr_channel);

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

		switch(number_pwr_channel){

			case PDM_PWR_Channel_1:

				if( pdm_ptr->PWR_CH1_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH1_State_eF_in = ERROR; //Error
				}else{
					pdm_ptr->Error_PWR_CH1_State_eF_in = SUCCESS;
				}

				if( pdm_ptr->PWR_CH1_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH1_State_eF_out = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH1_State_eF_out = SUCCESS;
				}
				break;

			case PDM_PWR_Channel_2:

				if( pdm_ptr->PWR_CH2_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH2_State_eF_in = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH2_State_eF_in = SUCCESS;
				}

				if( pdm_ptr->PWR_CH2_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH2_State_eF_out = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH2_State_eF_out = SUCCESS;
				}
				break;

			case PDM_PWR_Channel_3:

				if( pdm_ptr->PWR_CH3_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH3_State_eF_in = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH3_State_eF_in = SUCCESS;
				}

				if( pdm_ptr->PWR_CH3_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH3_State_eF_out = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH3_State_eF_out = SUCCESS;
				}
				break;

			case PDM_PWR_Channel_4:

				if( pdm_ptr->PWR_CH4_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH4_State_eF_in = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH4_State_eF_in = SUCCESS;
				}

				if( pdm_ptr->PWR_CH4_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH4_State_eF_out = ERROR;
				}else{
					pdm_ptr->Error_PWR_CH4_State_eF_out = SUCCESS;
				}
				break;

			case PDM_PWR_Channel_5:

				if( pdm_ptr->PWR_CH5_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH5_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH5_State_eF_in = SUCCESS;
				}

				if( pdm_ptr->PWR_CH5_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH5_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH5_State_eF_out = SUCCESS;
				}
				break;

			case PDM_PWR_Channel_6:

				if( pdm_ptr->PWR_CH6_State_eF_in != read_val_pin_EN_in_eF ){
					pdm_ptr->Error_PWR_CH6_State_eF_in = 1;
				}else{
					pdm_ptr->Error_PWR_CH6_State_eF_in = SUCCESS;
				}

				if( pdm_ptr->PWR_CH6_State_eF_out != read_val_pin_EN_out_eF ){
					pdm_ptr->Error_PWR_CH6_State_eF_out = 1;
				}else{
					pdm_ptr->Error_PWR_CH6_State_eF_out = SUCCESS;
				}
				break;

			default:
				break;
		}

	}else{

		if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		}else if( pdm_table.I2C_addr_GPIO_Ext == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = ERROR;
		}

		switch(number_pwr_channel){

			case PDM_PWR_Channel_1:
				pdm_ptr->Error_PWR_CH1_State_eF_in = ERROR; //Error
				pdm_ptr->Error_PWR_CH1_State_eF_out = ERROR;//Error
				break;

			case PDM_PWR_Channel_2:
				pdm_ptr->Error_PWR_CH2_State_eF_in = ERROR; //Error
				pdm_ptr->Error_PWR_CH2_State_eF_out = ERROR;//Error
				break;

			case PDM_PWR_Channel_3:
				pdm_ptr->Error_PWR_CH3_State_eF_in = ERROR; //Error
				pdm_ptr->Error_PWR_CH3_State_eF_out = ERROR;//Error
				break;

			case PDM_PWR_Channel_4:
				pdm_ptr->Error_PWR_CH4_State_eF_in = ERROR; //Error
				pdm_ptr->Error_PWR_CH4_State_eF_out = ERROR;//Error
				break;

			case PDM_PWR_Channel_5:
				pdm_ptr->Error_PWR_CH5_State_eF_in = ERROR; //Error
				pdm_ptr->Error_PWR_CH5_State_eF_out = ERROR;//Error
				break;

			case PDM_PWR_Channel_6:
				pdm_ptr->Error_PWR_CH6_State_eF_in = ERROR; //Error
				pdm_ptr->Error_PWR_CH6_State_eF_out = ERROR;//Error
				break;

			default:
				break;
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
	_PDM_table pdm_table;


	SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM 

	//Get real state value pins TCA9539. 	
	pdm_table = PDM__Table(number_pwr_channel);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read real value input pin PG.  

		if( TCA9539_read_input_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_PG_in_eFuse, &read_val_pin_PG_in_eF) == SUCCESS ){

			error_I2C = TCA9539_read_input_pin( pdm_table.I2Cx_GPIO_Ext, pdm_table.I2C_addr_GPIO_Ext, pdm_table.pin_PG_out_eFuse, &read_val_pin_PG_out_eF);
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
				pdm_ptr->PWR_CH1_PG_eF_in = SUCCESS;  // OK because power channel is not Enable
				pdm_ptr->PWR_CH1_PG_eF_out = SUCCESS;
			}
			break;

		case  PDM_PWR_Channel_2:
			if( (pdm_ptr->PWR_CH2_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH2_State_eF_out == ENABLE) ){
				 pdm_ptr->PWR_CH2_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH2_PG_eF_out = read_val_pin_PG_out_eF;
			}else{
				pdm_ptr->PWR_CH2_PG_eF_in = SUCCESS;
				pdm_ptr->PWR_CH2_PG_eF_out = SUCCESS;
			}
			break;

		case  PDM_PWR_Channel_3:
			if( (pdm_ptr->PWR_CH3_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH3_State_eF_out == ENABLE) ){
				 pdm_ptr->PWR_CH3_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH3_PG_eF_out = read_val_pin_PG_out_eF;
			}else{
				pdm_ptr->PWR_CH3_PG_eF_in = SUCCESS;
				pdm_ptr->PWR_CH3_PG_eF_out = SUCCESS;
			}
			break;

		case  PDM_PWR_Channel_4:
			if( (pdm_ptr->PWR_CH4_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH4_State_eF_out == ENABLE) ){
				 pdm_ptr->PWR_CH4_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH4_PG_eF_out = read_val_pin_PG_out_eF;
			}else{
				pdm_ptr->PWR_CH4_PG_eF_in = SUCCESS;
				pdm_ptr->PWR_CH4_PG_eF_out = SUCCESS;
			}
			break;

		case  PDM_PWR_Channel_5:
			if( (pdm_ptr->PWR_CH5_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH5_State_eF_out == ENABLE) ){
				 pdm_ptr->PWR_CH5_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH5_PG_eF_out = read_val_pin_PG_out_eF;
			}else{
				pdm_ptr->PWR_CH5_PG_eF_in = SUCCESS;
				pdm_ptr->PWR_CH5_PG_eF_out = SUCCESS;
			}
			break;

		case  PDM_PWR_Channel_6:
			if( (pdm_ptr->PWR_CH6_State_eF_in == ENABLE ) && (pdm_ptr->PWR_CH6_State_eF_out == ENABLE) ){
				 pdm_ptr->PWR_CH6_PG_eF_in = read_val_pin_PG_in_eF;  //0-OK, 1-ERROR. Power good channel status input eFuse
				 pdm_ptr->PWR_CH6_PG_eF_out = read_val_pin_PG_out_eF;
			}else{
				pdm_ptr->PWR_CH6_PG_eF_in = SUCCESS;
				pdm_ptr->PWR_CH6_PG_eF_out = SUCCESS;
			}
			break;

		default:
			error_I2C = ERROR_N;
			break;
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
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
//                              PDM_PWR_Channel_1
//								PDM_PWR_Channel_2
//								PDM_PWR_Channel_3
//								PDM_PWR_Channel_4
//								PDM_PWR_Channel_5
//								PDM_PWR_Channel_6
//
//
//ErrorStatus PDM_Get_I_V_P( _PDM *pdm_ptr, uint8_t number_pwr_channel){
//
//	//I2C_TypeDef *I2Cx, uint8_t power_mon_addr, uint16_t max_current, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch
//	uint8_t i = 0;
//	int16_t val_current = 0;
//	uint16_t val_bus_voltage = 0;
//	uint16_t val_power = 0;
//	int8_t Error_I2C_MUX = ERROR_N;
//	int8_t error_I2C = ERROR_N;
//
//	// Switch MUX to PDM I2C bus on PMM
//	SW_TMUX1209_I2C_main_PDM();
//
//	//Enable I2C MUX channel
//	i=0;
//	error_I2C = ERROR_N;
//	while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.
//
//		error_I2C = TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );
//
//		if( error_I2C != SUCCESS ){
//			i++;
//			LL_mDelay( pdm_i2c_delay_att_conn );
//		}
//	}
//
//	Error_I2C_MUX = error_I2C;
//
//	//Read temperature
//	if( error_I2C == SUCCESS ){
//
//		i=0;
//		error_I2C = ERROR_N;
//
//		while( ( error_I2C != SUCCESS ) && ( i < pdm_i2c_attempt_conn ) ){///Read temperature.
//
//			error_I2C = INA231_Get_I_V_P_int16( I2Cx, power_mon_addr, max_current, &val_current, &val_bus_voltage, &val_power );
//
//			if( error_I2C != SUCCESS ){
//				i++;
//				LL_mDelay( pdm_i2c_delay_att_conn );
//			}
//		}
//	}
//
//	//Disable I2C MUX channel.
//	//Note: Do not check the error since it doesn’t matter anymore.
//	TCA9548_Disable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );
//
//	//Parse error
//	if( Error_I2C_MUX == ERROR_N ){
//		pdm_ptr->Error_I2C_MUX = ERROR;
//	}else{
//		pdm_ptr->Error_I2C_MUX = SUCCESS;
//	}

//	switch( power_mon_addr ){
//
//		case PDM_I2CADDR_TMP1075_1:
//			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
//				pdm_ptr->Temp_sensor_1 = 0x7F;
//				pdm_ptr->Error_temp_sensor_1 = ERROR;
//			}else{
//				pdm_ptr->Temp_sensor_1 = temp_value;
//				pdm_ptr->Error_temp_sensor_1 = SUCCESS; //No error
//			}
//			break;
//
//		default:
//			break;
//	}

	//return error_I2C;
//}



