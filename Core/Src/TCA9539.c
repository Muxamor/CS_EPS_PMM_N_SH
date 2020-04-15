

#include "TCA9539.h"


/*
    @brief  Checking flag status any LL function with signature uint32_t LL_SOME_FUNC(I2C_TypeDef *I2Cx)
	@param  (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *) - pointer to LL function.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  flag_state - (SET or RESET) value to be compared result of (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *).
	@retval	0 - OK, -1-Error
*/
int8_t TCA9539_I2C_check_flag(uint32_t (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *), I2C_TypeDef *I2Cx, uint8_t flag_state){

	for(uint32_t counter = 0; I2C_FRAM_I2C_check_flag(I2Cx) == flag_state; counter++){
		if(counter == TCA9539_I2C_TIMEOUT){

			return -1;
		}
	}

	return 0;
}


/** @brief	Reading two byte register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param 	reg_id - register's address.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_I2C_Read_word(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t reg_id, uint16_t *data){

	uint8_t high_byte, low_byte;

	SlaveAddr = SlaveAddr << 1;

    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != 0){
    	return -1;
    }

    LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_TransmitData8(I2Cx, reg_id);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != 0){
    	return -1;
    }
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_RESTART_7BIT_READ);

    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    high_byte = LL_I2C_ReceiveData8(I2Cx);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != 0){
    	return -1;
    }
    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    low_byte = LL_I2C_ReceiveData8(I2Cx);

    LL_I2C_GenerateStopCondition(I2Cx);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_ClearFlag_STOP(I2Cx);

    *data =  ( ( (uint16_t)high_byte ) << 8 ) | ( (uint16_t)low_byte );

    return 0;
}


/** @brief	Writing two byte register.
	@param *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param reg_id - register address.
	@param SlaveAddr - 8-bit device address.
	@param data - 2-byte data to write
	@retval	0-OK, -1-Error
*/
int8_t TCA9539_I2C_Write_word(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t reg_id, uint16_t data){

	uint8_t low_byte = (uint8_t) data;
    uint8_t high_byte = (uint8_t)(data >> 8);

    SlaveAddr = SlaveAddr << 1;

    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != 0){
    	return -1;
    }
    LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 3, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_TransmitData8(I2Cx, reg_id);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_TransmitData8(I2Cx, high_byte);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_TransmitData8(I2Cx, low_byte);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != 0){
    	return -1;
    }
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_GenerateStopCondition(I2Cx);
    if(TCA9539_I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != 0){
    	return -1;
    }

    LL_I2C_ClearFlag_STOP(I2Cx);

    return 0;
}


/** @brief	Reading two byte configuration register. Shows the directions of the I/O pins.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from configuration register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_IO_dir_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t *read_data){

	return TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Configuration_Port, read_data);
}


/** @brief	Reading two byte output register. Shows the outgoing logic levels of the pins defined
	as outputs by the Configuration register. Reading from this register reflects the value that
	is in the flip-flop controlling the output selection, not the actual pin value.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from output register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_output_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t *read_data){

	return TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Output_Port, read_data);
}


/** @brief	Reading two byte input register. Reflects the incoming logic levels of the pins,
	regardless of whether the pin is defined as an input or an output by the Configuration register.
	The default value is determined by the externally applied logic level.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from input register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_input_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t *read_data){

	return TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Input_Port, read_data);
}


/** @brief	Reading two byte of polarity inversion register. Allows Polarity Inversion of pins defined as
	inputs by the Configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	*read_data - pointer to variable where will be store data from polarity inversion register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_IO_pol_reg(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t *read_data){

	return TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Polarity_Inversion_Port, read_data);
}


/** @brief	Reading single bit value of configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin, uint8_t *read_data){

	uint16_t read_register;

	if(pin == TCA9539_IO_ALL){
		return -1;
	}

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Configuration_Port, &read_register) != 0){
		return -1;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return 0;
}


/** @brief	Reading single bit value of output register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_P00 - TCA9539_P07, TCA9539_P10 - TCA9539_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of output register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_output_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin, uint8_t *read_data){

	uint16_t read_register;

	if(pin == TCA9539_IO_ALL){
		return -1;
	}

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Output_Port, &read_register) != 0){
		return -1;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return 0;
}


/** @brief	Reading single bit value of input register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of input register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_input_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin, uint8_t *read_data){

	uint16_t read_register;

	if(pin == TCA9539_IO_ALL){
		return -1;
	}

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Input_Port, &read_register) != 0){
		return -1;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return 0;
}


/** @brief	Reading single bit value of polarity inversion register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17).
	@param 	*read_data - pointer to variable where will be store value of pin of polarity inversion register.
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin, uint8_t *read_data){

	uint16_t read_register;

	if(pin == TCA9539_IO_ALL){
		return -1;
	}

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Polarity_Inversion_Port, &read_register) != 0){
		return -1;
	}

	*read_data = (uint8_t)((read_register & pin) == pin ? 1 : 0);

	return 0;
}


/** @brief	Set pin as output.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, <0-Error
*/
int8_t TCA9539_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin){

	uint16_t read_register;

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Configuration_Port, &read_register) != 0){
		return -1;
	}
	if(TCA9539_I2C_Write_word(I2Cx, tca9539_addr, TCA9539_Configuration_Port, (~pin) & read_register) != 0){
		return -1;
	}

	return 0;
}

/** @brief	Set pin as input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin){

	uint16_t read_register;

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Configuration_Port, &read_register) != 0){
		return -1;
	}
	if(TCA9539_I2C_Write_word(I2Cx, tca9539_addr, TCA9539_Configuration_Port, pin | read_register) != 0){
		return -1;
	}

	return 0;
}


/** @brief	Set normal polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin){

	uint16_t read_register;

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Polarity_Inversion_Port, &read_register) != 0){
		return -1;
	}
	if(TCA9539_I2C_Write_word(I2Cx, tca9539_addr, TCA9539_Polarity_Inversion_Port, (~pin) & read_register) != 0){
		return -1;
	}

	return 0;
}


/** @brief	Set inverted polarity to pin input.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin){

	uint16_t read_register;

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Polarity_Inversion_Port, &read_register) != 0){
		return -1;
	}
	if(TCA9539_I2C_Write_word(I2Cx, tca9539_addr, TCA9539_Polarity_Inversion_Port, pin | read_register) != 0){
		return -1;
	}

	return 0;
}



/** @brief	Set high level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin){

	uint16_t read_register;

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Output_Port, &read_register) != 0){
		return -1;
	}
	if(TCA9539_I2C_Write_word(I2Cx, tca9539_addr, TCA9539_Output_Port, pin | read_register) != 0){
		return -1;
	}

	return 0;
}


/** @brief	Set low level to output pin.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_addr - 8-bit device address.
	@param 	pin - pin number (TCA9539_IO_P00 - TCA9539_IO_P07, TCA9539_IO_P10 - TCA9539_IO_P17, TCA9539_IO_ALL).
	@retval 0-OK, -1-Error
*/
int8_t TCA9539_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t tca9539_addr, uint16_t pin){

	uint16_t read_register;

	if(TCA9539_I2C_Read_word(I2Cx, tca9539_addr, TCA9539_Output_Port, &read_register) != 0){
		return -1;
	}
	if(TCA9539_I2C_Write_word(I2Cx, tca9539_addr, TCA9539_Output_Port, (~pin) & read_register) != 0){
		return -1;
	}

	return 0;
}
