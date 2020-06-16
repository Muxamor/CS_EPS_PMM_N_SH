#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "TCA6424A.h"


/** @brief	Write state of pin to three registers.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param  pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@param  port_addr - first register address.
	@param  state - pin state (0 or 1).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_Write_three_reg(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint32_t port_addr, uint8_t state){
	uint8_t reg_read;
	uint8_t reg_write;
	uint8_t port_pin;
	uint8_t i = 0;

	for(i = 0; i < 3; i++){

		port_pin = (uint8_t)(pin & 0xFF);

		if(port_pin != 0){

			if(I2C_Read_byte_St_ReSt(I2Cx, I2C_TCA6424A_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)port_addr + i, &reg_read) != SUCCESS){
				return ERROR_N;
			}

			if(state == 1){
				reg_write = (uint8_t)(reg_read | pin);
			}else{
				reg_write = (uint8_t)(reg_read & (~pin));
			}

			if(I2C_Write_byte_St(I2Cx, I2C_TCA6424A_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)port_addr + i, reg_write ) != SUCCESS){
				return ERROR_N;
			}
		}

		pin = pin >> 8;
	}

	return SUCCESS;
}


/** @brief	Reading three registers.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from configuration registers.
	@param  port_addr - first register address
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_Read_three_reg(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data, uint32_t port_addr){

	uint8_t reg_read;
	uint8_t i = 0;
	*read_data = 0;

	for(i = 0; i < 3; i++){

		if(I2C_Read_byte_St_ReSt(I2Cx, I2C_TCA6424A_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)port_addr + i, &reg_read) != SUCCESS){
			return ERROR_N;
		}

		*read_data |= (uint32_t)(reg_read << (8 * i));
	}

	return SUCCESS;
}


/** @brief	Reading three configuration registers. Shows the directions of the I/O pins.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from configuration registers.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_IO_dir_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data){

	return TCA6424A_Read_three_reg(I2Cx, I2C_TCA6424A_addr, read_data, TCA6424A_CONFIG_PORT0_ADDR);
}


/** @brief	Reading three output registers. Shows the outgoing logic levels of the pins defined
			as outputs by the Configuration registers.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_output_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data){

	return TCA6424A_Read_three_reg(I2Cx, I2C_TCA6424A_addr, read_data, TCA6424A_OUTPUT_PORT0_ADDR);
}


/** @brief	Reading three input registers. Reflects the incoming real logic levels of the pins,
			regardless of whether the pin is defined as an input or an output by the Configuration registers.
			The default value is determined by the externally applied logic level.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from input register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_input_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data){

	return TCA6424A_Read_three_reg(I2Cx, I2C_TCA6424A_addr, read_data, TCA6424A_INPUT_PORT0_ADDR);
}


/** @brief	Reading three polarity inversion registers. Allows Polarity Inversion of pins defined as
			inputs by the Configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from polarity inversion register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_IO_pol_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data){

	return TCA6424A_Read_three_reg(I2Cx, I2C_TCA6424A_addr, read_data, TCA6424A_POL_INV_PORT0_ADDR);
}


/** @brief	Reading bit value of configuration register Port0 and Port1.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17, TCA6424A_IO_P20...TCA6424A_IO_P27).
	@param 	*pin_state - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state){

	uint32_t read_data = 0;

	if(TCA6424A_read_IO_dir_regs(I2Cx, I2C_TCA6424A_addr, &read_data) != SUCCESS){
		return ERROR_N;
	}

	*pin_state = (read_data & pin) == pin ? 1 : 0;
	return SUCCESS;
}


/** @brief	Reading bit value of output register Port0 and Port1.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17, TCA6424A_IO_P20...TCA6424A_IO_P27).
	@param 	*pin_state - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state){

	uint32_t read_data = 0;

	if(TCA6424A_read_output_regs(I2Cx, I2C_TCA6424A_addr, &read_data) != SUCCESS){
		return ERROR_N;
	}

	*pin_state = (read_data & pin) == pin ? 1 : 0;
	return SUCCESS;
}


/** @brief	Reading bit value of input register Port0 and Port1.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17, TCA6424A_IO_P20...TCA6424A_IO_P27).
	@param 	*pin_state - pointer to variable 1 or 0 where will be store value of pin of input register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_input_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state){

	uint32_t read_data = 0;

	if(TCA6424A_read_input_regs(I2Cx, I2C_TCA6424A_addr, &read_data) != SUCCESS){
		return ERROR_N;
	}

	*pin_state = (read_data & pin) == pin ? 1 : 0;
	return SUCCESS;
}


/** @brief	Reading bit value of polarity inversion register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17, TCA6424A_IO_P20...TCA6424A_IO_P27).
	@param 	*pin_state - pointer to variable where will be store value of pin of polarity inversion register.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state){

	uint32_t read_data = 0;

	if(TCA6424A_read_IO_pol_regs(I2Cx, I2C_TCA6424A_addr, &read_data) != SUCCESS){
		return ERROR_N;
	}

	*pin_state = (read_data & pin) == pin ? 1 : 0;
	return SUCCESS;
}


/** @brief	Set pin as output.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin){

	return TCA6424A_Write_three_reg(I2Cx, I2C_TCA6424A_addr, pin, TCA6424A_CONFIG_PORT0_ADDR, 0);
}


/** @brief	Set pin as input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin){

	return TCA6424A_Write_three_reg(I2Cx, I2C_TCA6424A_addr, pin, TCA6424A_CONFIG_PORT0_ADDR, 1);
}

/** @brief	Set normal polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin){

	return TCA6424A_Write_three_reg(I2Cx, I2C_TCA6424A_addr, pin, TCA6424A_POL_INV_PORT0_ADDR, 0);
}


/** @brief	Set inverted polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin){

	return TCA6424A_Write_three_reg(I2Cx, I2C_TCA6424A_addr, pin, TCA6424A_POL_INV_PORT0_ADDR, 1);
}


/** @brief	Set high level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin){

	return TCA6424A_Write_three_reg(I2Cx, I2C_TCA6424A_addr, pin, TCA6424A_OUTPUT_PORT0_ADDR, 0);
}


/** @brief	Set low level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_TCA6424A_addr - 8-bit device address.
	@param 	pin - pin number (TCA6424A_IO_P00...TCA6424A_IO_P07, TCA6424A_IO_P10...TCA6424A_IO_P17,
			TCA6424A_IO_P20...TCA6424A_IO_P27, TCA6424A_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA6424A_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin){

	return TCA6424A_Write_three_reg(I2Cx, I2C_TCA6424A_addr, pin, TCA6424A_OUTPUT_PORT0_ADDR, 1);
}
