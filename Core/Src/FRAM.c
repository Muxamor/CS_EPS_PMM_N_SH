
#include "stm32l4xx_ll_gpio.h"
#include "i2c_comm.h"
#include "FRAM.h"

/** @brief  Set write protection for both FRAM.
	@param  access_flag - FRAM_WRITE_PROTECTION_ENABLE or FRAM_WRITE_PROTECTION_DISABLE.
	@retval	0 - SUCCESS, -1 - ERROR
*/
ErrorStatus FRAM_set_write_access(uint8_t access_flag){

	if(access_flag == FRAM_WRITE_PROTECTION_DISABLE){
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_3);
		LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_15);

	}else if(access_flag == FRAM_WRITE_PROTECTION_ENABLE){
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_3);
		LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_15);

	}else{

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing data to three segments of FRAMT.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_fram_addr - pointer to struct with configuration data.
	@param 	*ptr_data - pointer to data which would be wrote to FRAM.
	@param 	data_size - data size in bytes.
	@retval 0 - Writing success, -1 - ERROR
*/
ErrorStatus FRAM_triple_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size ){

	ErrorStatus error_status = 0;
	/* Triple write data*/
	error_status += I2C_Write_array_u8_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, (uint32_t)FRAM_Addr_segment_1, ptr_data, data_size);
	error_status += I2C_Write_array_u8_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, (uint32_t)FRAM_Addr_segment_2, ptr_data, data_size);
	error_status += I2C_Write_array_u8_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, (uint32_t)FRAM_Addr_segment_3, ptr_data, data_size);

	if(error_status != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}



/** @brief	Writing data to three segments of FRAMT  with verification.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_fram_addr - pointer to struct with configuration data.
	@param 	*ptr_data - pointer to data which would be wrote into FRAM.
	@param 	data_size - struct size in bytes.
	@retval 0 - Writing success, -1 - ERROR
*/
ErrorStatus FRAM_triple_verif_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size ){

	uint8_t ptr_read_byte = 0;
	uint32_t i = 0;
	/* Triple write data*/
	I2C_Write_array_u8_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, (uint32_t)FRAM_Addr_segment_1, ptr_data, data_size);
	I2C_Write_array_u8_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, (uint32_t)FRAM_Addr_segment_2, ptr_data, data_size);
	I2C_Write_array_u8_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, (uint32_t)FRAM_Addr_segment_3, ptr_data, data_size);

	/* Verification of recorded data */
	for( i = 0; i < data_size; i++ ){

		if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, &ptr_read_byte ) != SUCCESS ){//first reading attempt

			if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, &ptr_read_byte ) != SUCCESS ){ //second reading attempt
				return ERROR_N;
			}
		}

		if( *(ptr_data + i) != ptr_read_byte ){
			return ERROR_N;
		}
	}

	return SUCCESS;
}


/** @brief	Majority reading three segments from one FRAM
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  I2C_fram_addr - I2C address FRAM
	@param 	*ptr_data - pointer to data where would be written data from FRAM.
	@retval 0 - Reading success (SUCCESS), -1 - ERROR
*/
ErrorStatus FRAM_majority_read_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size ){

	uint32_t i = 0;

	for( i = 0; i < data_size; i++ ){
		if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, ptr_data + i) != SUCCESS ){//first reading attempt
			
			if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, ptr_data + i) != SUCCESS ){ //second reading attempt
				return ERROR_N;
			}
		}
	}

	return SUCCESS;
}


/** @brief	Majority reading from two FRAMs if is one of byte is broken
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  I2C_addr_fram_main - I2C address main FRAM
	@param  I2C_addr_fram_backup - I2C address backup FRAM
	@param 	*ptr_data - pointer to struct where would be written data from FRAM.
	@param 	data_size - size of data for write.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus FRAM_majority_read_data_two_fram(I2C_TypeDef *I2Cx, uint8_t I2C_addr_fram_main, uint8_t I2C_addr_fram_backup, uint8_t *ptr_data, uint32_t data_size ){

	uint32_t i = 0;

	for( i = 0; i < data_size; i++){

		if( FRAM_majority_read_byte(I2Cx, I2C_addr_fram_main, i, ptr_data + i) != SUCCESS ){

			if( FRAM_majority_read_byte(I2Cx, I2C_addr_fram_backup, i, ptr_data + i) != SUCCESS ){
				return ERROR_N;
			}
		}
	}

	return SUCCESS;
}


/** @brief	Reading single byte from three segments of FRAM with subsequent comparison of them by
	majoritary method.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	i2c_fram_addr - I2C address FRAM
	@param 	offset - offset from begin of struct's address.
	@param 	*read_byte - pointer to variable where would be written data from FRAM.
	@retval 0 - Reading success, -1 - FRAM_ERROR, //probably FRAM is dead
*/
ErrorStatus FRAM_majority_read_byte(I2C_TypeDef *I2Cx, uint8_t i2c_fram_addr, uint32_t offset, uint8_t *read_byte){

	uint8_t seg1_byte;
	uint8_t seg2_byte;
	uint8_t seg3_byte;

	I2C_Read_byte_St_ReSt(I2Cx, i2c_fram_addr, I2C_SIZE_REG_ADDR_U16, ((uint32_t)(FRAM_Addr_segment_1) + offset), &seg1_byte);
	I2C_Read_byte_St_ReSt(I2Cx, i2c_fram_addr, I2C_SIZE_REG_ADDR_U16, ((uint32_t)(FRAM_Addr_segment_2) + offset), &seg2_byte);
	I2C_Read_byte_St_ReSt(I2Cx, i2c_fram_addr, I2C_SIZE_REG_ADDR_U16, ((uint32_t)(FRAM_Addr_segment_3) + offset), &seg3_byte);

	if(seg1_byte == seg2_byte){
		*read_byte = seg1_byte;

	}else if (seg1_byte == seg3_byte){
		*read_byte = seg1_byte;

	}else if(seg2_byte == seg3_byte){
		*read_byte = seg2_byte;

	}else{
		return ERROR_N;
	}

	return SUCCESS;
}
