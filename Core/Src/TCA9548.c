#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "TCA9548.h"



/** @brief	Read control register TCA9548
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	*data_reg - pointer to variable where will be store value register.
	@retval 0-OK, ERROR_N-Error I2C bus
*/
ErrorStatus TCA9548_Read_reg(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t *data_reg){

	if( I2C_Read_byte_directly_St(I2Cx, tca9548_I2C_addr, data_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Write control register TCA9548
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	*data_reg - data to write in register TCA9548.
	@retval 0-OK, ERROR_N-Error I2C bus
*/
ErrorStatus TCA9548_Write_reg(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t data_reg){

	if( I2C_Write_byte_directly_St(I2Cx, tca9548_I2C_addr, data_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Turn ENABLE the I2C channel.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	I2C_ch - channel number:
					TCA9548_CH0
					TCA9548_CH1
					TCA9548_CH2
					TCA9548_CH3
					TCA9548_CH4
					TCA9548_CH5
					TCA9548_CH6
					TCA9548_CH7
					TCA9548_ALL_CHANNELS
	@retval 0-OK, ERROR_N-Error I2C bus
*/
ErrorStatus TCA9548_Enable_I2C_ch(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t I2C_ch){

	uint8_t reg_read;
	uint8_t reg_write;

	if( I2C_Read_byte_directly_St(I2Cx, tca9548_I2C_addr, &reg_read) != SUCCESS){
		return ERROR_N;
	}

	reg_write = (uint8_t)(reg_read | I2C_ch);

	if( I2C_Write_byte_directly_St(I2Cx, tca9548_I2C_addr, reg_write) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Turn off the channel.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	I2C_ch - channel number:
					TCA9548_CH0
					TCA9548_CH1
					TCA9548_CH2
					TCA9548_CH3
					TCA9548_CH4
					TCA9548_CH5
					TCA9548_CH6
					TCA9548_CH7
					TCA9548_ALL_CHANNELS
	@retval 0-OK, ERROR_N-Error I2C bus
*/
ErrorStatus TCA9548_Disable_I2C_ch( I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t I2C_ch ){

	uint8_t reg_read;
	uint8_t reg_write;

	if( I2C_Read_byte_directly_St( I2Cx, tca9548_I2C_addr, &reg_read ) != SUCCESS){
		return ERROR_N;
	}

	reg_write = (uint8_t)(reg_read & (~I2C_ch));


	if( I2C_Write_byte_directly_St( I2Cx, tca9548_I2C_addr, reg_write ) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read status of single channel.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9548_I2C_addr - 8-bit device address.
	@param 	I2C_ch - channel number which will be checked:
					TCA9548_CH0
					TCA9548_CH1
					TCA9548_CH2
					TCA9548_CH3
					TCA9548_CH4
					TCA9548_CH5
					TCA9548_CH6
					TCA9548_CH7
	@param 	*ch_status - pointer to variable where will be store channel status. 1-ENABLE, 0-DISABLE
	@retval 0-OK, ERROR_N-Error I2C bus
*/
ErrorStatus TCA9548_Get_state_I2C_ch(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t I2C_ch, uint8_t *ch_state){

	uint8_t reg_read;

	if( I2C_Read_byte_directly_St( I2Cx, tca9548_I2C_addr, &reg_read ) != SUCCESS){
		return ERROR_N;
	}

	*ch_state =  (uint8_t)( ( reg_read & I2C_ch ) == I2C_ch ? 1 : 0 );

	return SUCCESS;
}
