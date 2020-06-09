#include "stm32l4xx.h"
#include "stm32l4xx_ll_i2c.h"
#include "i2c_comm.h"
#include "TCA9548.h"


///** @brief	Read control register.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	SlaveAddr - 8-bit device address.
//	@param 	*data - pointer to variable where will be store control register data.
//	@retval 0 - SUCCESS, -1 - ERROR
//*/
//ErrorStatus TCA9548_Read_config(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t *data){
//
//	uint8_t receive_data = 0;
//	SlaveAddr = (uint8_t)(SlaveAddr << 1);
//
//	//Clear flags if the previous attempt to exchange was not successful.
//	LL_I2C_ClearFlag_TXE(I2Cx);
//	LL_I2C_ClearFlag_NACK(I2Cx);
//	LL_I2C_ClearFlag_BERR(I2Cx);
//	LL_I2C_ClearFlag_STOP(I2Cx);
//
//
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
//		return ERROR_N;
//	}
//
//	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
//		return ERROR_N;
//	}
//
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){  // ???
//		return ERROR_N;
//	}
//
//	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)1, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_NOSTARTSTOP);
//
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
//		return ERROR_N;
//	}
//	receive_data = LL_I2C_ReceiveData8(I2Cx);
//
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
//		return ERROR_N;
//	}
//
//	LL_I2C_ClearFlag_STOP(I2Cx);
//
//	*data = receive_data;
//
//	return SUCCESS;
//}


///** @brief	Write control register.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	SlaveAddr - 8-bit device address.
//	@param 	data - data to be writing.
//	@retval 0-OK, ERROR_N-Error
//*/
//ErrorStatus TCA9548_Write_config(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t data){
//
//	SlaveAddr = (uint8_t)(SlaveAddr << 1);
//
//	//Clear flags if the previous attempt to exchange was not successful.
//	LL_I2C_ClearFlag_TXE(I2Cx);
//	LL_I2C_ClearFlag_NACK(I2Cx);
//	LL_I2C_ClearFlag_BERR(I2Cx);
//	LL_I2C_ClearFlag_STOP(I2Cx);
//
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
//		return ERROR_N;
//	}
//	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE); //LL_I2C_MODE_SOFTEND
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
//		return ERROR_N;
//	}
//
//	LL_I2C_TransmitData8(I2Cx, data);
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
//		return ERROR_N;
//	}
//
//	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
//		return ERROR_N;
//	}
//
//	LL_I2C_ClearFlag_STOP(I2Cx);
//
//	return SUCCESS;
//}


/** @brief	Turn on the channel.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	ch - channel number (TCA9548_CH0...TCA9548_CH8).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9548_Set_channel(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t ch){

	uint8_t reg_read;
	uint8_t reg_write;

	//-----------if we can set several active channels---------
	/*
	if(TCA9548_Read_config(I2Cx, tca9548_I2C_addr, &reg_read) != SUCCESS){
		return ERROR_N;
	}

	reg_write = (uint8_t)(reg_read | ch);

	if(TCA9548_Write_config(I2Cx, tca9548_I2C_addr, reg_write ) != SUCCESS){
		return ERROR_N;
	}
	*/
	//-----------if we can set only one active channel------------
	if(TCA9548_Write_config(I2Cx, tca9548_I2C_addr, ch ) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Turn off the channel.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9539_I2C_addr - 8-bit device address.
	@param 	ch - channel number (TCA9548_CH0...TCA9548_CH8, TCA9539_IO_ALL).
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9548_Reset_channel(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t ch){

	uint8_t reg_read;
	uint8_t reg_write;

	//------------if we can set several active channels---------------
	/*
	if(TCA9548_Read_config(I2Cx, tca9548_I2C_addr, &reg_read) != SUCCESS){
		return ERROR_N;
	}

	reg_write = (uint8_t)(reg_read & (~ch));

	if(TCA9548_Write_config(I2Cx, tca9548_I2C_addr, reg_write ) != SUCCESS){
		return ERROR_N;
	}
	*/
	//--------------if we can set only one active channel------------------
	if(TCA9548_Write_config(I2Cx, tca9548_I2C_addr, ch ) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Read status of single channel.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	tca9548_I2C_addr - 8-bit device address.
	@param 	ch_num - channel number which will be checked.
	@param 	*ch_status - pointer to variable where will be store channel status.
	@retval 0-OK, ERROR_N-Error
*/
ErrorStatus TCA9548_Read_channel(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t ch_num, uint8_t *ch_status){

	uint8_t reg_read;

	if(TCA9548_Read_config(I2Cx, tca9548_I2C_addr, &reg_read) != SUCCESS){
		return ERROR_N;
	}

	*ch_status = ch_num & reg_read;

	return SUCCESS;
}
