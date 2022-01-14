#include "stm32l4xx.h"
#include "stm32l4xx_ll_i2c.h"
#include "i2c_comm.h"




/** @brief  Checking flag status any LL function with signature uint32_t LL_SOME_FUNC(I2C_TypeDef *I2Cx)
	@param  (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *) - pointer to LL function.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  flag_state - (SET or RESET) value to be compared result of (*I2C_check_flag)(I2C_TypeDef *).
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_check_flag(uint32_t (*I2C_check_flag)(I2C_TypeDef *), I2C_TypeDef *I2Cx, uint8_t flag_state){
	uint32_t counter =0;

	for( counter = 0; I2C_check_flag(I2Cx) == flag_state; counter++){
		if(counter == I2C_TIMEOUT){

			return ERROR_N;
		}
	}

	return SUCCESS;
}

/** @brief  Clear flag status if the previous attempt to exchange was not successful.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@retval none.
*/
void I2C_Clear_Error_Flags(I2C_TypeDef *I2Cx){

	LL_I2C_ClearFlag_TXE(I2Cx);
	LL_I2C_ClearFlag_NACK(I2Cx);
	LL_I2C_ClearFlag_BERR(I2Cx);
	LL_I2C_ClearFlag_STOP(I2Cx);

	if(LL_I2C_IsActiveFlag_ARLO(I2Cx) == SET){
		LL_I2C_ClearFlag_ARLO(I2Cx);
	}
	if(LL_I2C_IsActiveFlag_BUSY(I2Cx) == SET){
		LL_I2C_Disable(I2Cx);
		LL_I2C_Enable(I2Cx);
	}
}

/** @brief	Reading single straight byte without register address  in devices ( St- generate only Read Start )
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  *data - pointer to variable u8 where would be written data from the remote device.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Read_byte_directly_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t *data){

	uint8_t receive_data = 0;
	uint32_t SlaveAddr1;

	SlaveAddr1 = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr1, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	//LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
	receive_data = LL_I2C_ReceiveData8(I2Cx);

	//LL_I2C_GenerateStopCondition(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	*data = receive_data;

	return SUCCESS;
}



/** @brief	Reading single byte  ( St_ReSt - generate Start and Restart )
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  size_reg_addr - size of reg_addr in byte if:
				reg_addr = U8 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U8 	
				reg_addr = U16 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U16 
				reg_addr = U24 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U24 
				reg_addr = U32 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U32
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device
	@param  *data - pointer to variable u8 where would be written data from the remote device.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Read_byte_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr , uint32_t reg_addr, uint8_t *data){

	if( size_reg_addr == 0 || size_reg_addr > 4 ){ 
		return ERROR_N;
	} 

	uint8_t receive_data = 0;
	uint8_t i = 0; 
	int8_t  j = 0;
	uint32_t SlaveAddr1;

	SlaveAddr1 = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr1, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)size_reg_addr, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	for( i = size_reg_addr , j = size_reg_addr-1 ; i != 0; i--, j-- ){ //high byte is sent first

		LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr >> (j*8)) );

		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr1, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
	
	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	//LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
	receive_data = LL_I2C_ReceiveData8(I2Cx);

	//LL_I2C_GenerateStopCondition(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	*data = receive_data;

	return SUCCESS;
}



/** @brief	Reading two byte register. ( St_ReSt - generate Start and Restart )
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  size_reg_addr - size of reg_addr in byte if:
				reg_addr = U8 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U8 	
				reg_addr = U16 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U16 
				reg_addr = U24 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U24 
				reg_addr = U32 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U32
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device
	@param 	*data - pointer to variable u16 where would be written data from the remote device.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Read_word_u16_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t *data){

	if( size_reg_addr == 0 || size_reg_addr > 4 ){ 
		return ERROR_N;
	} 

	uint8_t high_byte = 0, low_byte = 0;
	uint8_t i = 0;
	int8_t  j = 0;

	SlaveAddr = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)size_reg_addr, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}


	for( i = size_reg_addr , j = size_reg_addr-1 ; i != 0; i--, j-- ){ //high byte is sent first

		LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr >> (j*8)) );

		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_RESTART_7BIT_READ); //LL_I2C_MODE_SOFTEND

	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	//LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
	high_byte = LL_I2C_ReceiveData8(I2Cx);
	
	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	// LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
	low_byte = LL_I2C_ReceiveData8(I2Cx);

	//LL_I2C_GenerateStopCondition(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	*data =  (uint16_t)( ( high_byte  << 8 ) | low_byte );

	return SUCCESS;
}


/** @brief	Reading three byte register. ( St_ReSt - generate Start and Restart )
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  size_reg_addr - size of reg_addr in byte if:
				reg_addr = U8 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U8
				reg_addr = U16 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U16
				reg_addr = U24 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U24
				reg_addr = U32 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U32
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device
	@param 	*data - pointer to variable u16 where would be written data from the remote device.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Read_word_u24_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint32_t *data){

    if( size_reg_addr == 0 || size_reg_addr > 4 ){
        return ERROR_N;
    }

    uint8_t high_byte = 0, middle_byte = 0, low_byte = 0;
    uint8_t i = 0;
    int8_t  j = 0;

    SlaveAddr = (uint8_t)(SlaveAddr << 1);

    //Clear flags if the previous attempt to exchange was not successful.
    I2C_Clear_Error_Flags(I2Cx);

    if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
        return ERROR_N;
    }

    LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)size_reg_addr, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
        return ERROR_N;
    }


    for( i = size_reg_addr , j = size_reg_addr-1 ; i != 0; i--, j-- ){ //high byte is sent first

        LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr >> (j*8)) );

        if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
            return ERROR_N;
        }
    }

    if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
        return ERROR_N;
    }

    LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)3, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_RESTART_7BIT_READ); //LL_I2C_MODE_SOFTEND

    if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
        return ERROR_N;
    }
    //LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    high_byte = LL_I2C_ReceiveData8(I2Cx);

    if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
        return ERROR_N;
    }
    //LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    middle_byte = LL_I2C_ReceiveData8(I2Cx);

    if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
        return ERROR_N;
    }
    // LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    low_byte = LL_I2C_ReceiveData8(I2Cx);

    //LL_I2C_GenerateStopCondition(I2Cx);
    if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
        return ERROR_N;
    }

    LL_I2C_ClearFlag_STOP(I2Cx);

    *data =  (uint32_t)( (high_byte << 16) | (middle_byte << 8) | low_byte );

    return SUCCESS;
}




/**@brief	Writing uint8_t data (byte) straight without register address  in devices. (St generate only one start)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  data - uint8_t data to be writing
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Write_byte_directly_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t data){

	SlaveAddr = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}
	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE); //LL_I2C_MODE_SOFTEND
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_TransmitData8(I2Cx, data);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	return 0;
}


/**@brief	Writing uint8_t data (byte) to address reg_addr. (St generate only one start)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  size_reg_addr - size of reg_addr in byte if:
				reg_addr = U8 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U8 	
				reg_addr = U16 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U16 
				reg_addr = U24 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U24 
				reg_addr = U32 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U32
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device
	@param  data - uint8_t data to be writing
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Write_byte_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint8_t data){

	if( size_reg_addr == 0 || size_reg_addr > 4 ){ 
		return ERROR_N;
	} 
	
	uint8_t i = 0;
	int8_t  j = 0;

	SlaveAddr = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}
	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)(size_reg_addr+1), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE); //LL_I2C_MODE_SOFTEND
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	for( i = size_reg_addr , j = size_reg_addr-1 ; i != 0; i--, j-- ){ //high byte is sent first

		LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr >> (j*8)) );

		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	LL_I2C_TransmitData8(I2Cx, data);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	//if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
	//	return ERROR_N;
	//}

	//LL_I2C_GenerateStopCondition(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	return 0;
}

/**@brief	Writing uint16_t data  by address reg_addr. (St generate only one start)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  size_reg_addr - size of reg_addr in byte if:
				reg_addr = U8 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U8 	
				reg_addr = U16 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U16 
				reg_addr = U24 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U24 
				reg_addr = U32 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U32
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device
	@param  data - uint16_t data to be writing
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Write_word_u16_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t data){

	if( size_reg_addr == 0 || size_reg_addr > 4 ){ 
		return ERROR_N;
	} 

	uint8_t low_byte = (uint8_t) data;
	uint8_t high_byte = (uint8_t)(data >> 8);
	uint8_t i = 0;
	int8_t  j = 0;

	SlaveAddr = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != 0){
		return ERROR_N;
	}
	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)(size_reg_addr+2), LL_I2C_MODE_AUTOEND , LL_I2C_GENERATE_START_WRITE); ////LL_I2C_MODE_SOFTEND
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	for( i = size_reg_addr , j = size_reg_addr-1 ; i != 0; i--, j-- ){ //high byte is sent first

		LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr >> (j*8)) );

		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	LL_I2C_TransmitData8(I2Cx, high_byte );
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_TransmitData8(I2Cx, low_byte );
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	//if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
	//	return ERROR_N;
	//}

	//LL_I2C_GenerateStopCondition(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	return SUCCESS;
}


/**@brief	Writing uint8_t array by address reg_addr. (St generate only one start)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param  size_reg_addr - size of reg_addr in byte if:
				reg_addr = U8 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U8 	
				reg_addr = U16 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U16 
				reg_addr = U24 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U24 
				reg_addr = U32 	-> 	size_reg_addr = I2C_SIZE_REG_ADDR_U32
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device.
	@param  *data - pointer at the array u8  which would be wrote to the remote device.
	@param  data_size - size of array or struct.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Write_array_u8_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint8_t *data, uint32_t data_size){
	
	if( size_reg_addr == 0 || size_reg_addr > 4 ){ 
		return ERROR_N;
	} 

	uint32_t i = 0;
	int8_t j = 0;

	SlaveAddr = (uint8_t)(SlaveAddr << 1);

	//Clear flags if the previous attempt to exchange was not successful.
	I2C_Clear_Error_Flags(I2Cx);

	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)(data_size + size_reg_addr), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE); // LL_I2C_MODE_SOFTEND
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	for( i = size_reg_addr , j = size_reg_addr-1 ; i != 0; i--, j-- ){ //high byte is sent first

		LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr >> (j*8)) );

		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	for( i = 0; i < data_size; i++){
		LL_I2C_TransmitData8(I2Cx, *(data + i));
		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	//if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
	//	return ERROR_N;
	//}

	//LL_I2C_GenerateStopCondition(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_ClearFlag_STOP(I2Cx);

	return SUCCESS;
}

