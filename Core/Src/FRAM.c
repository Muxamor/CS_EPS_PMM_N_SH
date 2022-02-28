#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "i2c_comm.h"
#include "FRAM.h"


/** @brief	Erasing all FRAM's cells. (write 0x00)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_fram_addr - I2C address FRAM
	@param 	fram_size - size of FRAM in bytes.
			FRAM_SIZE_64KB : 0x2000
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus FRAM_erase(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint32_t fram_size){

	ErrorStatus error_status = 0;
	uint32_t i = 0;

    FRAM_control_write_access(FRAM_WRITE_PROTECTION_DISABLE);

	for(i = 0; i < fram_size; i++){
		error_status += I2C_Write_byte_St(I2Cx, I2C_fram_addr, I2C_SIZE_REG_ADDR_U16, i, 0x00);
	}

    FRAM_control_write_access(FRAM_WRITE_PROTECTION_ENABLE);

	if(error_status != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Check if fram is empty
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  I2C_addr_fram_main - I2C address main FRAM
	@param  I2C_addr_fram_backup - I2C address backup FRAM
	@param 	fram_size - size of FRAM in bytes.
			FRAM_SIZE_64KB : 0x2000
	@fram_status  pointer to show status FRAM :  0 - FRAM is empty, 1 - FRAM is not empty,
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus FRAM_Detect_Empty( I2C_TypeDef *I2Cx_fram, uint8_t I2C_addr_fram, uint32_t fram_size, uint8_t *fram_status ) {

	ErrorStatus error_status = SUCCESS;
	uint32_t i = 0;
	uint8_t read_byte;
	uint16_t read_sum = 0;

	for( i = 0; i < 128; i++  ){
		error_status = FRAM_majority_read_byte(I2Cx_fram, I2C_addr_fram, i, &read_byte);
		read_sum += read_byte;
		if( error_status != SUCCESS){
            return ERROR_N;  // FRAM ERROR_N
        }
	}

	if( read_sum == 0 ){
	    *fram_status = 0;  // FRAM is empty

	}else if( read_sum == (0xFF * 128) ){ // FRAM is empty bat all 0xFF

        error_status = FRAM_erase(I2Cx_fram, I2C_addr_fram, fram_size); // ckear FRAM write 0x00
        if(error_status != SUCCESS){
            return ERROR_N;
        }else{
            *fram_status = 0;  // FRAM is empty
        }

    }else{
	    *fram_status = 1;  // FRAM is no empty
	}

	return SUCCESS;
}


/** @brief  Set write protection for both FRAM.
	@param  access_flag - FRAM_WRITE_PROTECTION_ENABLE or FRAM_WRITE_PROTECTION_DISABLE.
	@retval	0 - SUCCESS, -1 - ERROR
*/
ErrorStatus FRAM_control_write_access( uint8_t access_flag){

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
	@param 	I2C_fram_addr - I2C address FRAM
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
	@param 	I2C_fram_addr - I2C address FRAM
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
				return ERROR_N;
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
			return ERROR_N;
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

	uint8_t seg1_byte = 114;
	uint8_t seg2_byte = 15;
	uint8_t seg3_byte = 65;

    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t error_status = SUCCESS;
    uint8_t i = 0;


    error_I2C = ERROR_N;
    while( ( error_I2C != SUCCESS ) && ( i < fram_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

        error_I2C = I2C_Read_byte_St_ReSt(I2Cx, i2c_fram_addr, I2C_SIZE_REG_ADDR_U16, ((uint32_t)(FRAM_Addr_segment_1) + offset), &seg1_byte);

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( fram_i2c_delay_att_conn );
        }
    }

    error_status = error_I2C;

    error_I2C = ERROR_N;
    while( ( error_I2C != SUCCESS ) && ( i < fram_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

        error_I2C = I2C_Read_byte_St_ReSt(I2Cx, i2c_fram_addr, I2C_SIZE_REG_ADDR_U16, ((uint32_t)(FRAM_Addr_segment_2) + offset), &seg2_byte);

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( fram_i2c_delay_att_conn );
        }
    }

    error_status = error_status + error_I2C;

    error_I2C = ERROR_N;
    while( ( error_I2C != SUCCESS ) && ( i < fram_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

        error_I2C = I2C_Read_byte_St_ReSt(I2Cx, i2c_fram_addr, I2C_SIZE_REG_ADDR_U16, ((uint32_t)(FRAM_Addr_segment_3) + offset), &seg3_byte);

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( fram_i2c_delay_att_conn );
        }
    }

    error_status = error_status + error_I2C;

    if( error_status != -3 ){

        if( seg1_byte == seg2_byte ){
            *read_byte = seg1_byte;

        }else if( seg1_byte == seg3_byte ){
            *read_byte = seg1_byte;

        }else if( seg2_byte == seg3_byte ){
            *read_byte = seg2_byte;

        }else{
            return ERROR_N;
        }

    }else{
        return ERROR_N;
    }

	return SUCCESS;
}
