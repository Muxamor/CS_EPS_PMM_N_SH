#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "PCA9534.h"


/** @brief	Reading configuration register. Shows the directions of the I/O pins.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	*read_data - pointer to variable where will be store data from configuration register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_IO_dir_reg(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t *read_data){

	return I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Configuration_Port, read_data);
}


/** @brief	Reading output register. Shows the outgoing logic levels of the pins defined
	as outputs by the Configuration register. 
	Note: Reading from this register reflects the value that is in the flip-flop controlling the output selection, not the actual pin value.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	*read_data - pointer to variable where will be store data from output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_output_reg(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t *read_data){

	return I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Output_Port, read_data);
}


/** @brief	Reading input register. Reflects the incoming real logic levels of the pins 
	regardless of whether the pin is defined as an input or an output by the Configuration register.
	The default value is determined by the externally applied logic level.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	*read_data - pointer to variable where will be store data from input register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_input_reg(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t *read_data){

	return I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Input_Port, read_data);
}


/** @brief	Reading polarity inversion register. Allows Polarity Inversion of pins defined as
	inputs by the Configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	*read_data - pointer to variable where will be store data from polarity inversion register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_IO_pol_reg(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t *read_data){

	return I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Polarity_Inversion_Port, read_data);
}


/** @brief	Reading bit value of configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07).
	@param 	*read_data - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin, uint8_t *read_data){

	uint8_t read_register;

	if(pin == PCA9534_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Configuration_Port, &read_register) != 0){
		return ERROR_N;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Reading bit value of output register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_P00...PCA9534_P07).
	@param 	*read_data - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_output_pin(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin, uint8_t *read_data){

	uint8_t read_register;

	if(pin == PCA9534_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Output_Port, &read_register) != 0){
		return ERROR_N;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Reading bit value of input register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07).
	@param 	*read_data - pointer to variable where will be store value of pin of input register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_input_pin(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin, uint8_t *read_data){

	uint8_t read_register;

	if(pin == PCA9534_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Input_Port, &read_register) != 0){
		return ERROR_N;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Reading bit value of polarity inversion register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07).
	@param 	*read_data - pointer to variable where will be store value of pin of polarity inversion register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin, uint8_t *read_data){

	uint8_t read_register;

	if(pin == PCA9534_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Polarity_Inversion_Port, &read_register) != 0){
		return ERROR_N;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Set pin as output.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07, PCA9534_IO_ALL).
	@retval 0-OK, <0-Error
*/
ErrorStatus PCA9534_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin){

	uint8_t read_register;

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Configuration_Port, &read_register) != 0){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Configuration_Port, (uint8_t) ((~pin) & read_register)) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Set pin as input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07, PCA9534_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin){

	uint8_t read_register = 0;

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Configuration_Port, &read_register) != 0){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Configuration_Port, pin | read_register) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set normal polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07, PCA9534_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin){

	uint8_t read_register;

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Polarity_Inversion_Port, &read_register) != 0){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Polarity_Inversion_Port, (uint8_t)((~pin) & read_register)) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set inverted polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07, PCA9534_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin){

	uint8_t read_register;

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Polarity_Inversion_Port, &read_register) != 0){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Polarity_Inversion_Port, pin | read_register) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}



/** @brief	Set high level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00...PCA9534_IO_P07, PCA9534_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin){

	uint8_t read_register;

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Output_Port, &read_register) != 0){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Output_Port, pin | read_register) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set low level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	PCA9534_addr - 7-bit device address.
	@param 	pin - pin number (PCA9534_IO_P00 - PCA9534_IO_P07, PCA9534_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus PCA9534_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t PCA9534_addr, uint8_t pin){

	uint8_t read_register;

	if(I2C_Read_byte_St_ReSt(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Output_Port, &read_register) != 0){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, PCA9534_addr, I2C_SIZE_REG_ADDR_U8, PCA9534_Output_Port, (uint8_t)((~pin) & read_register)) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}
