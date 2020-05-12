
#include "stm32l4xx_ll_utils.h"
#include "i2c_comm.h"
#include  "FRAM.h"
#include  "PMM/pmm_savedata.h"



// !!!!!!!!!!!!!!!! MOVE THIS to PMM  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/** @brief	Writing to FRAM selected fields from four struct.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	*struct_1 - pointer to struct with configuration data which would be wrote into FRAM.
	@param 	*struct_2 - pointer to struct with configuration data which would be wrote into FRAM.
	@param 	*struct_3 - pointer to struct with configuration data which would be wrote into FRAM.
	@param  *struct_4 - pointer to struct with configuration data which would be wrote into FRAM.
	@retval 0 - Writing success(SUCCESS),
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

	if(FRAM_triple_verif_write_data(I2Cx, i2c_addr_fram1, ptr_fram_struct_write, sizeof(fram_struct_write)) != SUCCESS){
		error_status = -1;
	}

	if(FRAM_triple_verif_write_data(I2Cx, i2c_addr_fram2, ptr_fram_struct_write, sizeof(fram_struct_write)) != SUCCESS){

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
	@retval 0 - Reading success (SUCCESS),
		   -1 - FRAM_1_DEAD,
		   -3 - BOTH_FRAM_DEAD.
*/
int8_t FRAM_read_data(I2C_TypeDef *I2Cx, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, data_struct_1 *struct_1, data_struct_2 *struct_2, data_struct_3 *struct_3, data_struct_4 *struct_4){

	int8_t error_status = 0;
	FRAM_STRUCT fram_struct_read = {0};

	if( FRAM_majority_read_data(I2Cx, i2c_addr_fram1, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != SUCCESS ){
	//if( FRAM_majority_read_data_two_fram(I2Cx, i2c_addr_fram1, i2c_addr_fram2, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != SUCCESS ){

		error_status = -1;

		LL_mDelay(50);

		if( FRAM_majority_read_data(I2Cx, i2c_addr_fram2, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != SUCCESS ){
		//if( FRAM_majority_read_data_two_fram(I2Cx, i2c_addr_fram2, i2c_addr_fram1, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != SUCCESS ){
			return -3;
		}

	}else{
		error_status = SUCCESS;
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
