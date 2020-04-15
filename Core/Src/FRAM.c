#include "FRAM.h"

/*
    @brief  Checking flag status any LL function with signature uint32_t LL_SOME_FUNC(I2C_TypeDef *I2Cx)
	@param  (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *) - pointer to LL function.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  flag_state - (SET or RESET) value to be compared result of (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *).
	@retval	0 - Timeout OK, -1 - Timeout error.
*/
int8_t FRAM_I2C_check_flag(uint32_t (*I2C_FRAM_I2C_check_flag)(I2C_TypeDef *), I2C_TypeDef *I2Cx, uint8_t flag_state){

	uint32_t counter = 0;

	for(counter = 0; I2C_FRAM_I2C_check_flag(I2Cx) == flag_state; counter++){
		if(counter == FRAM_I2C_TIMEOUT){
			return -1;
		}
	}

	return 0;
}

/*
    @brief	Writing array or struct into FRAM by address reg_addr.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param 	reg_addr - register's address.
	@param  *data - pointer to data which would be wrote into FRAM.
	@param  data_size - size of array or struct.
	@retval 0 - Writing success (OK), -1 - TIMEOUT_ERROR
*/
int8_t FRAM_I2C_array_write(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint16_t reg_addr, uint8_t *data, uint32_t data_size){

	uint8_t addr_low = (uint8_t)reg_addr;
	uint8_t addr_high = (uint8_t)(reg_addr >> 8);

	SlaveAddr = SlaveAddr << 1;

	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;
	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, data_size + 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_TransmitData8(I2Cx, addr_high);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_TransmitData8(I2Cx, addr_low);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	for(int i = 0; i < data_size; i++){
		LL_I2C_TransmitData8(I2Cx, *(data + i));
		if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
			return TIMEOUT_ERROR;
	}

	LL_I2C_GenerateStopCondition(I2Cx);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_ClearFlag_STOP(I2Cx);

	return OK;
}


/*
    @brief	Reading single byte from FRAM by address reg_addr.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param 	reg_addr - register's address.
	@param  *data - pointer to variable where would be written data from FRAM.
	@retval 0 - Reading success (OK), -1 - TIMEOUT_ERROR
*/
int8_t FRAM_I2C_byte_read(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint16_t reg_addr, uint8_t *data){

	uint8_t addr_low = (uint8_t)reg_addr;
	uint8_t addr_high = (uint8_t)(reg_addr >> 8);
	uint8_t receive_data;

	SlaveAddr = SlaveAddr << 1;

	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE); //LL_I2C_GENERATE_START_READ
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_TransmitData8(I2Cx, addr_high);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_TransmitData8(I2Cx, addr_low);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ); //LL_I2C_MODE_SOFTEND
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_NACK);
	receive_data = LL_I2C_ReceiveData8(I2Cx);

	LL_I2C_GenerateStopCondition(I2Cx);
	if(FRAM_I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) == TIMEOUT_ERROR)
		return TIMEOUT_ERROR;

	LL_I2C_ClearFlag_STOP(I2Cx);

	*data = receive_data;

	return OK;
}


/** @brief  Set write protection for both FRAM.
	@param  access_flag - FRAM_WRITE_PROTECTION_ENABLE or FRAM_WRITE_PROTECTION_DISABLE.
	@retval	0 - OK, -1 - ERROR
*/
int8_t FRAM_set_write_access(uint8_t access_flag){

	if(access_flag == FRAM_WRITE_PROTECTION_DISABLE){
		LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_3);
		LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_15);

	}else if(access_flag == FRAM_WRITE_PROTECTION_ENABLE){
		LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_3);
		LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_15);

	}else{

		return -1;
	}

	return OK;
}


/** @brief	Writing data to three segments of FRAMT.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_fram_addr - pointer to struct with configuration data.
	@param 	*ptr_data - pointer to data which would be wrote to FRAM.
	@param 	data_size - data size in bytes.
	@retval 0 - Writing success, -1 - ERROR
*/
int8_t FRAM_triple_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size ){

	int8_t error_status = 0;
	/* Triple write data*/
	error_status += FRAM_I2C_array_write(I2Cx, I2C_fram_addr, FRAM_Addr_segment_1, ptr_data, data_size);
	error_status += FRAM_I2C_array_write(I2Cx, I2C_fram_addr, FRAM_Addr_segment_2, ptr_data, data_size);
	error_status += FRAM_I2C_array_write(I2Cx, I2C_fram_addr, FRAM_Addr_segment_3, ptr_data, data_size);

	if(error_status != 0 ){
		return FRAM_ERROR;
	}

	return OK;
}


/** @brief	Writing data to three segments of FRAMT  with verification.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_fram_addr - pointer to struct with configuration data.
	@param 	*ptr_data - pointer to data which would be wrote into FRAM.
	@param 	data_size - struct size in bytes.
	@retval 0 - Writing success, -1 - ERROR
*/
int8_t FRAM_triple_verif_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size ){

	uint8_t ptr_read_byte = 0;
	uint32_t i = 0;
	/* Triple write data*/
	FRAM_I2C_array_write(I2Cx, I2C_fram_addr, FRAM_Addr_segment_1, ptr_data, data_size);
	FRAM_I2C_array_write(I2Cx, I2C_fram_addr, FRAM_Addr_segment_2, ptr_data, data_size);
	FRAM_I2C_array_write(I2Cx, I2C_fram_addr, FRAM_Addr_segment_3, ptr_data, data_size);

	/* Verification of recorded data */
	for( i = 0; i < data_size; i++ ){

		if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, &ptr_read_byte ) != OK ){//first reading attempt

			if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, &ptr_read_byte ) != OK ){ //second reading attempt
				return FRAM_ERROR;
			}
		}

		if( *(ptr_data + i) != ptr_read_byte ){
			return FRAM_ERROR;
		}
	}

	return OK;
}


/** @brief	Majority reading three segments from one FRAM
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param  I2C_fram_addr - I2C address FRAM
	@param 	*ptr_data - pointer to data where would be written data from FRAM.
	@retval 0 - Reading success (OK), -1 - ERROR
*/
int8_t FRAM_majority_read_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size ){

	uint32_t i = 0;

	for( i = 0; i < data_size; i++ ){
		if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, ptr_data + i) != OK ){//first reading attempt
			
			if( FRAM_majority_read_byte(I2Cx, I2C_fram_addr, i, ptr_data + i) != OK ){ //second reading attempt
				return FRAM_ERROR;
			}
		}
	}

	return OK;
}


/** @brief	Majority reading from two FRAMs if is one of byte is broken
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param  I2C_addr_fram_main - I2C address main FRAM
    @param  I2C_addr_fram_backup - I2C address backup FRAM
	@param 	*ptr_data - pointer to struct where would be written data from FRAM.
	@param 	data_size - size of data for write.
	@retval 0 - OK, -1 - ERROR
*/
int8_t FRAM_majority_read_data_two_fram(I2C_TypeDef *I2Cx, uint8_t I2C_addr_fram_main, uint8_t I2C_addr_fram_backup, uint8_t *ptr_data, uint32_t data_size ){

	uint32_t i = 0;

	for( i = 0; i < data_size; i++){

		if( FRAM_majority_read_byte(I2Cx, I2C_addr_fram_main, i, ptr_data + i) != OK ){

			if( FRAM_majority_read_byte(I2Cx, I2C_addr_fram_backup, i, ptr_data + i) != OK ){
				return FRAM_ERROR;
			}
		}
	}

	return OK;
}


/*
    @brief	Reading single byte from three segments of FRAM with subsequent comparison of them by
    majoritary method.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	i2c_fram_addr - I2C address FRAM
	@param 	offset - offset from begin of struct's address.
	@param 	*read_byte - pointer to variable where would be written data from FRAM.
	@retval 0 - Reading success,
		   -1 - FRAM_ERROR, //probably FRAM is dead
*/
int8_t FRAM_majority_read_byte(I2C_TypeDef *I2Cx, uint8_t i2c_fram_addr, uint16_t offset, uint8_t *read_byte){

	uint8_t seg1_byte;
	uint8_t seg2_byte;
	uint8_t seg3_byte;

	FRAM_I2C_byte_read(I2Cx, i2c_fram_addr, FRAM_Addr_segment_1 + offset, &seg1_byte);
	FRAM_I2C_byte_read(I2Cx, i2c_fram_addr, FRAM_Addr_segment_2 + offset, &seg2_byte);
	FRAM_I2C_byte_read(I2Cx, i2c_fram_addr, FRAM_Addr_segment_3 + offset, &seg3_byte);

	if(seg1_byte == seg2_byte){
		*read_byte = seg1_byte;

	}else if (seg1_byte == seg3_byte){
		*read_byte = seg1_byte;

	}else if(seg2_byte == seg3_byte){
		*read_byte = seg2_byte;

	}else{
		return FRAM_ERROR;
	}

	return OK;
}

// !!!!!!!!!!!!!!!! MOVE THIS to PMM  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/** @brief	Writing to FRAM selected fields from four struct.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	*struct_1 - pointer to struct with configuration data which would be wrote into FRAM.
	@param 	*struct_2 - pointer to struct with configuration data which would be wrote into FRAM.
	@param 	*struct_3 - pointer to struct with configuration data which would be wrote into FRAM.
	@param  *struct_4 - pointer to struct with configuration data which would be wrote into FRAM.
	@retval 0 - Writing success(OK),
		   -1 - FRAM_1_DEAD,
		   -2 - FRAM_2_DEAD,
		   -3 - BOTH_FRAM_DEAD.
*/
int8_t FRAM_save_data(I2C_TypeDef *I2Cx, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, data_struct_1 *struct_1, data_struct_2 *struct_2, data_struct_3 *struct_3, data_struct_4 *struct_4){

	int8_t error_status = 0;
	FRAM_STRUCT fram_struct_write = {0};
	uint8_t *ptr_fram_struct_write;

	// ------- Filling of FRAM_STRUCT -------- //
	// here you should change fields of FRAM_STRUCT. Do not forget change in "FRAM.h"
	fram_struct_write.var1 = struct_1->var1;
	fram_struct_write.var2 = struct_1->var2;
	fram_struct_write.var3 = struct_2->var3;
	fram_struct_write.var4 = struct_2->var4;
	fram_struct_write.var5 = struct_3->var5;
	fram_struct_write.var6 = struct_3->var4;
	fram_struct_write.var7 = struct_4->var3;
	fram_struct_write.var8 = struct_4->var1;
	// -------------------------------------- //

	FRAM_set_write_access(FRAM_WRITE_PROTECTION_DISABLE);

	ptr_fram_struct_write = (uint8_t*)(&fram_struct_write);

	if(FRAM_triple_verif_write_data(I2Cx, i2c_addr_fram1, ptr_fram_struct_write, sizeof(fram_struct_write)) != OK){
		error_status = -1;
	}

	if(FRAM_triple_verif_write_data(I2Cx, i2c_addr_fram2, ptr_fram_struct_write, sizeof(fram_struct_write)) != OK){

		if(error_status == -1){
			error_status = -3;
		}else{
			error_status = -2;
		}
	}

	FRAM_set_write_access(FRAM_WRITE_PROTECTION_ENABLE);

	return error_status;
}


/** @brief	Writing to FRAM selected fields from four struct.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param  i2c_addr_fram1 - I2C address main FRAM
    @param  i2c_addr_fram2 - I2C address backup FRAM
	@param 	*struct_1 - pointer to struct with configuration data which would be wrote into FRAM.
	@param 	*struct_2 - pointer to struct with configuration data which would be wrote into FRAM.
	@param 	*struct_3 - pointer to struct with configuration data which would be wrote into FRAM.
	@param  *struct_4 - pointer to struct with configuration data which would be wrote into FRAM.
	@retval 0 - Reading success (OK),
		   -1 - FRAM_1_DEAD,
		   -3 - BOTH_FRAM_DEAD.
*/
int8_t FRAM_read_data(I2C_TypeDef *I2Cx, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, data_struct_1 *struct_1, data_struct_2 *struct_2, data_struct_3 *struct_3, data_struct_4 *struct_4){

	int8_t error_status = 0;
	FRAM_STRUCT fram_struct_read = {0};

	if( FRAM_majority_read_data(I2Cx, i2c_addr_fram1, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != OK ){
	//if( FRAM_majority_read_data_two_fram(I2Cx, i2c_addr_fram1, i2c_addr_fram2, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != OK ){

		error_status = -1;

		LL_mDelay(50);

		if( FRAM_majority_read_data(I2Cx, i2c_addr_fram2, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != OK ){
		//if( FRAM_majority_read_data_two_fram(I2Cx, i2c_addr_fram2, i2c_addr_fram1, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != OK ){
    		return -3;
    	}

	}else{
		error_status = OK;
	}

	struct_1->var1 = fram_struct_read.var1;
	struct_1->var2 = fram_struct_read.var2;
	struct_2->var3 = fram_struct_read.var3;
	struct_2->var4 = fram_struct_read.var4;
	struct_3->var5 = fram_struct_read.var5;
	struct_3->var4 = fram_struct_read.var6;
	struct_4->var3 = fram_struct_read.var7;
	struct_4->var1 = fram_struct_read.var8;

	return error_status;
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
