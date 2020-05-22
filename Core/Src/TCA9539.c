#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "TCA9539.h"


/** @brief	Reading configuration register Port0 and Port1. Shows the directions of the I/O pins.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from configuration register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_IO_dir_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint16_t)((P1_reg << 8) | P0_reg);

	return SUCCESS;
}


/** @brief	Reading output register Port0 and Port1. Shows the outgoing logic levels of the pins defined
	as outputs by the Configuration register. Reading from this register reflects the value that
	is in the flip-flop controlling the output selection, not the actual pin value.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_output_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint16_t)((P1_reg << 8) | P0_reg);

	return SUCCESS;
}


/** @brief	Reading input register Port0 and Port1. Reflects the incoming real logic levels of the pins,
	regardless of whether the pin is defined as an input or an output by the Configuration register.
	The default value is determined by the externally applied logic level.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from input register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_input_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Input_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Input_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint16_t)((P1_reg << 8) | P0_reg);

	return SUCCESS;
}


/** @brief	Reading polarity inversion register Port0 and Port1. Allows Polarity Inversion of pins defined as
	inputs by the Configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from polarity inversion register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_IO_pol_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint16_t)((P1_reg << 8) | P0_reg);

	return SUCCESS;
}


/** @brief	Reading bit value of configuration register Port0 and Port1.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin, uint8_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;

	if(pin == TCA9539_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint8_t)(((uint16_t)((P1_reg << 8) | P0_reg) & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Reading bit value of output register Port0 and Port1.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_P00...TCA9539_P07, TCA9539_P10...TCA9539_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_output_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin, uint8_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;

	if(pin == TCA9539_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint8_t)(((uint16_t)((P1_reg << 8) | P0_reg) & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Reading bit value of input register Port0 and Port1.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17).
	@param 	*read_data - pointer to variable 1 or 0 where will be store value of pin of input register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_input_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin, uint8_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;

	if(pin == TCA9539_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Input_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Input_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint8_t)(((uint16_t)((P1_reg << 8) | P0_reg) & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Reading bit value of polarity inversion register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of polarity inversion register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin, uint8_t *read_data){

	uint8_t P0_reg = 0;
	uint8_t P1_reg = 0;

	if(pin == TCA9539_IO_ALL){
		return ERROR_N;
	}

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port0, &P0_reg) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port1, &P1_reg) != 0){
		return ERROR_N;
	}
	*read_data = (uint8_t)(((uint16_t)((P1_reg << 8) | P0_reg) & pin) == pin ? 1 : 0);

	return SUCCESS;
}


/** @brief	Set pin as output.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, <0-Error
*/
ErrorStatus TCA9539_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin){

	uint8_t P0_reg_read;
	uint8_t P1_reg_read;
	uint8_t P0_reg_write;
	uint8_t P1_reg_write;

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port0, &P0_reg_read) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port1, &P1_reg_read) != 0){
		return ERROR_N;
	}
	P0_reg_write = (uint8_t)(P0_reg_read & ((~pin) & 0xFF));
	P1_reg_write = (uint8_t)(P1_reg_read & ((~pin) >> 8));

	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port0, P0_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port1, P1_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}

/** @brief	Set pin as input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin){

	uint8_t P0_reg_read;
	uint8_t P1_reg_read;
	uint8_t P0_reg_write;
	uint8_t P1_reg_write;

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port0, &P0_reg_read) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port1, &P1_reg_read) != 0){
		return ERROR_N;
	}

	P0_reg_write = (uint8_t)(P0_reg_read | (pin & 0xFF));
	P1_reg_write = (uint8_t)(P1_reg_read | (pin >> 8));

	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port0, P0_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Configuration_Port1, P1_reg_write ) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set normal polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin){

	uint8_t P0_reg_read;
	uint8_t P1_reg_read;
	uint8_t P0_reg_write;
	uint8_t P1_reg_write;

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port0, &P0_reg_read) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port1, &P1_reg_read) != 0){
		return ERROR_N;
	}

	P0_reg_write = (uint8_t)(P0_reg_read & ((~pin) & 0xFF));
	P1_reg_write = (uint8_t)(P1_reg_read & ((~pin) >> 8));

	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port0, P0_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port1, P1_reg_write ) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set inverted polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin){

	uint8_t P0_reg_read;
	uint8_t P1_reg_read;
	uint8_t P0_reg_write;
	uint8_t P1_reg_write;

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port0, &P0_reg_read) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port1, &P1_reg_read) != 0){
		return ERROR_N;
	}

	P0_reg_write = (uint8_t)(P0_reg_read | (pin & 0xFF));
	P1_reg_write = (uint8_t)(P1_reg_read | (pin >> 8));

	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port0, P0_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Polarity_Inversion_Port1, P1_reg_write ) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}



/** @brief	Set high level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin){

	uint8_t P0_reg_read;
	uint8_t P1_reg_read;
	uint8_t P0_reg_write;
	uint8_t P1_reg_write;


	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port0, &P0_reg_read) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port1, &P1_reg_read) != 0){
		return ERROR_N;
	}

	P0_reg_write = (uint8_t)(P0_reg_read | (pin & 0xFF));
	P1_reg_write = (uint8_t)(P1_reg_read | (pin >> 8));

	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port0, P0_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port1, P1_reg_write ) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set low level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00...TCA9539_IO_P07, TCA9539_IO_P10...TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9539_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_I2C_addr, uint16_t pin){

	uint8_t P0_reg_read;
	uint8_t P1_reg_read;
	uint8_t P0_reg_write;
	uint8_t P1_reg_write;

	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port0, &P0_reg_read) != 0){
		return ERROR_N;
	}
	if(I2C_Read_byte_St_ReSt(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port1, &P1_reg_read) != 0){
		return ERROR_N;
	}

	P0_reg_write = (uint8_t)(P0_reg_read & ((~pin) & 0xFF));
	P1_reg_write = (uint8_t)(P1_reg_read & ((~pin) >> 8));

	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port0, P0_reg_write ) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_Write_byte_St(I2Cx, tca9539_I2C_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)TCA9539_Output_Port1, P1_reg_write ) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}
