#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "pdm_struct.h"
#include "pdm_config.h"
#include "TCA9539.h"
#include "pdm.h"


/** @brief  Set state (enable/disable) power channel.
	@param  *pdm_ptr - pointer to struct which save all information about PDM.
	@param  number_pwr_channel - number channel on/off:
								PDM_PWR_Channel_1 
								PDM_PWR_Channel_2
								PDM_PWR_Channel_3
								PDM_PWR_Channel_4
								PDM_PWR_Channel_5
								PDM_PWR_Channel_6 

	@param  state_channel - 0- disable power channel, 1 - enable power channel. (ENABLE, DISABLE).
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Set_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel, uint8_t state_channel ){

	I2C_TypeDef *I2Cx;
	uint8_t I2C_tca9539_addr = 0;
	uint8_t error_I2C = 1 ; //0-OK 1-Error
	uint8_t i=0;
	uint16_t pin_in_eFuse = 0;
	uint16_t pin_out_eFuse = 0;

	if( (state_channel != ENABLE) || (state_channel != DISABLE) ){
		return ERROR_N;
	}
	
	switch(number_pwr_channel){

		case PDM_PWR_Channel_1:
			
			I2Cx = PDM_I2Cx_GPIOExt1;
			I2C_tca9539_addr = PDM_I2CADDR_GPIOExt1;
			pin_in_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch1 (U19)
			pin_out_eFuse = TCA9539_IO_P02; 		//EN_eFusee_out_ch1 (U19)

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH1_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH1_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH1_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH1_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_2:

			I2Cx = PDM_I2Cx_GPIOExt2;
			I2C_tca9539_addr = PDM_I2CADDR_GPIOExt2;
			pin_in_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch2 (U20)
			pin_out_eFuse = TCA9539_IO_P02;			//EN_eFusee_out_ch2 (U20)

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH2_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH2_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH2_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH2_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_3:

			I2Cx = PDM_I2Cx_GPIOExt1;
			I2C_tca9539_addr = PDM_I2CADDR_GPIOExt1;
			pin_in_eFuse = TCA9539_IO_P05;			//EN_eFusee_in_ch3 (U19)
			pin_out_eFuse = TCA9539_IO_P07; 		//EN_eFusee_out_ch3 (U19)

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH3_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH3_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH3_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH3_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_4:

			I2Cx = PDM_I2Cx_GPIOExt2;
			I2C_tca9539_addr = PDM_I2CADDR_GPIOExt2;
			pin_in_eFuse = TCA9539_IO_P05;			//EN_eFusee_in_ch4 (U20)
			pin_out_eFuse = TCA9539_IO_P07;			//EN_eFusee_out_ch4 (U20)

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH4_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH4_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH4_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH4_State_eF_out = DISABLE; 
			}
			break;

		case PDM_PWR_Channel_5:

			I2Cx = PDM_I2Cx_GPIOExt1;
			I2C_tca9539_addr = PDM_I2CADDR_GPIOExt1;
			pin_in_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch5 (U19)
			pin_out_eFuse = TCA9539_IO_P14;			//EN_eFusee_out_ch5 (U19)

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH5_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH5_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH5_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH5_State_eF_out = DISABLE; 
			} 
			break;

		case PDM_PWR_Channel_6:

			I2Cx = PDM_I2Cx_GPIOExt2;
			I2C_tca9539_addr = PDM_I2CADDR_GPIOExt2;
			pin_in_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch6 (U20)
			pin_out_eFuse = TCA9539_IO_P14;			//EN_eFusee_out_ch6 (U20)

			if( state_channel == ENABLE){
				pdm_ptr->PWR_CH6_State_eF_in = ENABLE; 
				pdm_ptr->PWR_CH6_State_eF_out = ENABLE; 
			}else{ // DISABLE
				pdm_ptr->PWR_CH6_State_eF_in = DISABLE; 
				pdm_ptr->PWR_CH6_State_eF_out = DISABLE; 
			}
			break;

		default:
			return ERROR_N;
	}


	//Write to I2C GPIO Extender.
	error_I2C = 0;

	for( i = 0; i < pdm_i2c_attempt_conn; i++ ){ //Enable/Disable input Efuse power channel.

		if( state_channel == ENABLE ){
			if( TCA9539_Set_output_pin( I2Cx, I2C_tca9539_addr, pin_in_eFuse )  == SUCCESS ){
				break;
			}

		}else if( state_channel == DISABLE ){
			if( TCA9539_Reset_output_pin( I2Cx, I2C_tca9539_addr, pin_in_eFuse )  == SUCCESS ){
				break;
			}
		}

		LL_mDelay( pdm_i2c_delay_att_conn );

		if( i == (pdm_i2c_attempt_conn - 1) ) {
			error_I2C = 1; 
		}
	}

	if( error_I2C == 0 ){//Enable/Disable output Efuse power channel.
		LL_mDelay(20);

		for( i = 0; i < pdm_i2c_attempt_conn; i++ ){
			if( state_channel == ENABLE ){
				if( TCA9539_Set_output_pin( I2Cx, I2C_tca9539_addr, pin_out_eFuse )  == SUCCESS ){
					break;
				}
				
			}else if( state_channel == DISABLE ){
				if( TCA9539_Reset_output_pin( I2Cx, I2C_tca9539_addr, pin_out_eFuse )  == SUCCESS ){
					break;
				}
			}

			LL_mDelay( pdm_i2c_delay_att_conn );

			if( i == (pdm_i2c_attempt_conn - 1) ) {
				error_I2C = 1; 
			}
		}
	}

	if( error_I2C == 1 ){

		if( I2C_tca9539_addr == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = 1;

		}else if( I2C_tca9539_addr == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = 1;
		}

	}else{ //?????????????????????? Тут ли это делать ?

		if( I2C_tca9539_addr == PDM_I2CADDR_GPIOExt1 ){
			pdm_ptr->Error_I2C_GPIO_Ext1 = 0;

		}else if( I2C_tca9539_addr == PDM_I2CADDR_GPIOExt2 ){
			pdm_ptr->Error_I2C_GPIO_Ext2 = 0;
		}

	}

	return SUCCESS;
}

