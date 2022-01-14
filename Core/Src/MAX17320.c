#include "stm32l4xx.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_utils.h"
#include "i2c_comm.h"
#include "MAX17320.h"


/** @brief	Reading two byte register from MAX17320. ( St_ReSt - generate Start and Restart )
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	SlaveAddr - 8-bit device address.
	@param 	reg_addr - 8,16,24,32-bit Registry address on the remote device
	@param 	*data - pointer to variable u16 where would be written data from the remote device.
	@retval 0 - SUCCESS, -1 - ERROR
*/
ErrorStatus I2C_Read_MAX17320(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint32_t reg_addr, uint16_t *data){

	uint8_t high_byte = 0, low_byte = 0;

	//SlaveAddr = (uint8_t)(SlaveAddr << 1);
	I2C_Clear_Error_Flags(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_BUSY, I2Cx, SET) != SUCCESS){
		return ERROR_N;
	}
	LL_I2C_HandleTransfer(I2Cx, (uint32_t)SlaveAddr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_I2C_TransmitData8(I2Cx, (uint8_t)(reg_addr) );
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TC, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_I2C_HandleTransfer(I2Cx, (uint32_t)(SlaveAddr + 1), LL_I2C_ADDRSLAVE_7BIT, (uint32_t)2, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_RESTART_7BIT_READ); //LL_I2C_MODE_SOFTEND
	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	high_byte = LL_I2C_ReceiveData8(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_RXNE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	low_byte = LL_I2C_ReceiveData8(I2Cx);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_STOP, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}
	LL_I2C_ClearFlag_STOP(I2Cx);
	//*data =  (uint16_t)( ( high_byte  << 8 ) | low_byte );
	*data =  (uint16_t)( ( low_byte  << 8 ) | high_byte );

	return SUCCESS;
}

/**@brief	Writing two byte register from MAX17320. (St generate only one start)
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
ErrorStatus I2C_Write_MAX17320(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t data){

	if( size_reg_addr == 0 || size_reg_addr > 4 ){
		return ERROR_N;
	}

	//uint8_t low_byte = (uint8_t) data;
	//uint8_t high_byte = (uint8_t)(data >> 8);
	uint8_t low_byte = (uint8_t) (data>> 8);
	uint8_t high_byte = (uint8_t) data;
	uint8_t i = 0;
	int8_t  j = 0;

	//SlaveAddr = (uint8_t)(SlaveAddr << 1);

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

		LL_mDelay(5);
		if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
			return ERROR_N;
		}
	}

	LL_I2C_TransmitData8(I2Cx, high_byte );
	LL_mDelay(5);
	if(I2C_check_flag(LL_I2C_IsActiveFlag_TXE, I2Cx, RESET) != SUCCESS){
		return ERROR_N;
	}

	LL_I2C_TransmitData8(I2Cx, low_byte );
	LL_mDelay(5);
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


/**
 * @brief  Read Status Register (000h, 0B0h). The Status register maintains all flags related to alert thresholds and battery insertion or removal.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_StatusReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0x00, &Data) == 0) {
		Struct->POR = (Data & 0x0002) >> 1;
		Struct->Imn = (Data & 0x0004) >> 2;
		Struct->Imx = (Data & 0x0040) >> 6;
		Struct->dSOCi = (Data & 0x0080) >> 7;
		Struct->Vmn = (Data & 0x0100) >> 8;
		Struct->Tmn = (Data & 0x0200) >> 9;
		Struct->Smn = (Data & 0x0400) >> 10;
		Struct->Vmx = (Data & 0x1000) >> 12;
		Struct->Tmx = (Data & 0x2000) >> 13;
		Struct->Smx = (Data & 0x4000) >> 14;
		Struct->PA = (Data & 0x8000) >> 15;
		if (I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x00, (Data & 0x88BB)) == 0) {
			if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xB0, &Data) == 0) {
				Struct->Hib = (Data & 0x002) >> 1;

				return SUCCESS;
			} else {
				return ERROR_N;
				}
		} else {
			return ERROR_N;
		}
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nBattStatus Register (1A8h) with the permanent battery status information in Battery Module
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nBattStatusReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x16, 0xA8, &Data) == 0) {
		Struct->LeakCurr = (Data & 0x0ff);
		Struct->ChksumF = (Data & 0x0100) >> 8;
		Struct->LDet = (Data & 0x0200) >> 9;
		Struct->FETFo = (Data & 0x0400) >> 10;
		Struct->DFETFs = (Data & 0x0800) >> 11;
		Struct->CFETFs = (Data & 0x1000) >> 12;
		Struct->OTPF = (Data & 0x2000) >> 13;
		Struct->OVPF = (Data & 0x4000) >> 14;
		Struct->PermFail = (Data & 0x8000) >> 15;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nFaultLog Register (1AEh). This register has dual functionality depending on configuration settings.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nFaultLogReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x16, 0xAE, &Data) == 0) {
		Struct->ODCP = (Data & 0x0001);
		Struct->UVP = (Data & 0x0002) >> 1;
		Struct->Imbalance = (Data & 0x0004) >> 2;
		Struct->DieHot = (Data & 0x0008) >> 3;
		Struct->OCCP = (Data & 0x0010) >> 4;
		Struct->OVP = (Data & 0x0020) >> 5;
		Struct->TooColdC = (Data & 0x0040) >> 6;
		Struct->TooHotC = (Data & 0x0080) >> 7;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ProtStatus Register (0D9h). The Protection Status register contains the fault states of the protection state machine.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_ProtStatusReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xD9, &Data) == 0) {
		Struct->Ship = (Data & 0x0001);
		Struct->ResDFault = (Data & 0x0002) >> 1;
		Struct->ODCP = (Data & 0x0004) >> 2;
		Struct->UVP = (Data & 0x0008) >> 3;
		Struct->TooHotD = (Data & 0x0010) >> 4;
		Struct->DieHot = (Data & 0x0020) >> 5;
		Struct->PermFail = (Data & 0x0040) >> 6;
		Struct->Imbalance = (Data & 0x0080) >> 7;
		Struct->PreqF = (Data & 0x0100) >> 8;
		Struct->Qovflw = (Data & 0x0200) >> 9;
		Struct->OCCP = (Data & 0x0400) >> 10;
		Struct->OVP = (Data & 0x0800) >> 11;
		Struct->TooColdC = (Data & 0x1000) >> 12;
		Struct->Full = (Data & 0x2000) >> 13;
		Struct->TooHotC= (Data & 0x4000) >> 14;
		Struct->ChgWDT = (Data & 0x8000) >> 15;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ProtAlrt Register (0AFh). The Protection Alerts register contains a history of any protection events that have been logged by the device.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_HistoryProtStatusReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xAF, &Data) == 0) {
		Struct->HisLDet = (Data & 0x0001);
		Struct->HisResDFault = (Data & 0x0002) >> 1;
		Struct->HisODCP = (Data & 0x0004) >> 2;
		Struct->HisUVP = (Data & 0x0008) >> 3;
		Struct->HisTooHotD = (Data & 0x0010) >> 4;
		Struct->HisDieHot = (Data & 0x0020) >> 5;
		Struct->HisPermFail = (Data & 0x0040) >> 6;
		Struct->HisImbalance = (Data & 0x0080) >> 7;
		Struct->HisPreqF = (Data & 0x0100) >> 8;
		Struct->HisQovflw = (Data & 0x0200) >> 9;
		Struct->HisOCCP = (Data & 0x0400) >> 10;
		Struct->HisOVP = (Data & 0x0800) >> 11;
		Struct->HisTooColdC = (Data & 0x1000) >> 12;
		Struct->HisFull = (Data & 0x2000) >> 13;
		Struct->HisTooHotC= (Data & 0x4000) >> 14;
		Struct->HisChgWDT = (Data & 0x8000) >> 15;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Balancing Registers (1F5h). This registers contains a Balancing data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  Battery_number - number of battery in branch.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Balancing (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct, uint8_t Battery_number) {

	uint16_t Data = 0;
	if (I2C_Read_MAX17320(I2Cx, 0x16, 0xF5, &Data) == 0) {
		if(Battery_number == 2){
			Struct->BalCell1 = (Data & 0x0080) >> 7;
			Struct->BalCell4 = (Data & 0x0200) >> 9;
		} else if(Battery_number == 3){
			Struct->BalCell1 = (Data & 0x0080) >> 7;
			Struct->BalCell3 = (((Data & 0x0080) >> 7) & ((Data & 0x0100) >> 8));
			Struct->BalCell4 = (Data & 0x0200) >> 9;
		} else if(Battery_number == 4){
			Struct->BalCell1 = (Data & 0x0080) >> 7;
			Struct->BalCell2 = (Data & 0x0100) >> 8;
			Struct->BalCell3 = (((Data & 0x0080) >> 7) & ((Data & 0x0100) >> 8));
			Struct->BalCell4 = (Data & 0x0200) >> 9;
		}

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Clear ProtAlrt Register (0AFh). The Protection Alerts register contains a history of any protection events that have been logged by the device.
 * @param  I2Cx - Port I2C
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Clear_HistoryProtStatusReg (I2C_TypeDef *I2Cx) {

	if (I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0xAF, 0x0000) == 0) {

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read HProtCfg2 Register (1F1h). Register provides the status of the protection FETs and a variety of other functions.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_HProtCfg2Reg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xF1, &Data) == 0) {
		Struct->CHGs = (Data & 0x0001);
		Struct->DISs = (Data & 0x0002) >> 1;
		Struct->PBEN = (Data & 0x0008) >> 3;
		Struct->CPCfg = (Data & 0x0060) >> 5;
		Struct->CommOvrd = (Data & 0x0100) >> 8;
		Struct->AOLDO = (Data & 0xC000) >> 14;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Voltage Measurement Registers (01Ah, 019h, 0D1h-0D8h, 0DAh, 0DBh, 008h). This registers contains a Voltage Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_VoltageReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1A, &Data);
	Struct->VCell = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x19, &Data);
	Struct->AvgVCell = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD8, &Data);
	Struct->Cell1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD7, &Data);
	Struct->Cell2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD6, &Data);
	Struct->Cell3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD5, &Data);
	Struct->Cell4 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD4, &Data);
	Struct->AvgCell1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD3, &Data);
	Struct->AvgCell2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD2, &Data);
	Struct->AvgCell3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD1, &Data);
	Struct->AvgCell4 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDA, &Data);
	Struct->Batt = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDB, &Data);
	Struct->PCKP = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x08, &Data);
	Struct->MaxVCELL = (uint8_t) (Data & 0xFF00) >> 8;
	Struct->MinVCELL = (uint8_t) (Data & 0x00FF);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Current Measurement Registers (01Ch, 01Dh, 00Ah). This registers contains a Current Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_CurrentReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1C, &Data);
	Struct->Curr = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1D, &Data);
	Struct->AvgCurr = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0A, &Data);
	Struct->MaxCurr = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinCurr = (int8_t) (Data & 0x00FF);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Temperature Measurement Registers (01Bh, 016h, 009h, 034h, 040h, 133h-13Ah). This registers contains a Temperature Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_TempReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1B, &Data);
	Struct->Temp = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x16, &Data);
	Struct->AvgTA = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x09, &Data);
	Struct->MaxTemp = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinTemp = (int8_t) (Data & 0x00FF);
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x34, &Data);
	Struct->DieTemp = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x40, &Data);
	Struct->AvgDieTemp = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x3A, &Data);
	Struct->Temp1 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x39, &Data);
	Struct->Temp2 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x38, &Data);
	Struct->Temp3 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x37, &Data);
	Struct->Temp4 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x36, &Data);
	Struct->AvgTemp1 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x35, &Data);
	Struct->AvgTemp2 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x34, &Data);
	Struct->AvgTemp3 = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x33, &Data);
	Struct->AvgTemp4 = (int16_t) (Data);
	Data = 0;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Power Measurement Registers (0B1h, 0B3h). This registers contains a Power Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_PwrReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB1, &Data);
	Struct->Pwr = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB3, &Data);
	Struct->AvgPwr = (int16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Charge Parameters Registers (028h, 02Ah). This registers contains a Power Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_ChgParamReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x28, &Data);
	Struct->ChgCurr = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x2A, &Data);
	Struct->ChgVolt = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm Output Registers (005h-007h, 010h, 011h, 020h, 017h, 03Eh, 0BEh, 023h, 014h, 0B2h).
 * 		   This registers contains a ModelGauge m5 Algorithm Output Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_GaugeOutReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x05, &Data);
	Struct->RepCap = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x06, &Data);
	Struct->RepSOC = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x10, &Data);
	Struct->FullCapRep = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x11, &Data);
	Struct->TTE = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x20, &Data);
	Struct->TTF = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x07, &Data);
	Struct->Age = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x17, &Data);
	Struct->Cycles = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x3E, &Data);
	Struct->Timer = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xBE, &Data);
	Struct->TimerH = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x23, &Data);
	Struct->FullCapNom = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x14, &Data);
	Struct->RCell = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB2, &Data);
	Struct->VRipple = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm Output EEPROME Registers (1A4h, 1A5h, 1AAh, 1CAh). This registers contains a ModelGauge m5 Algorithm Output EEPROME Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nGaugeOutReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA4, &Data);
	Struct->Cycle = (uint16_t) (Data & 0xFFF8) >> 3;
	Struct->Fib = (uint8_t) (Data & 0x0007);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA5, &Data);
	Struct->FullCapNom = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xAA, &Data);
	Struct->AvgVCell = (uint16_t) (Data & 0xFF80) >> 7;
	Struct->AvgTA = (uint8_t) (Data & 0x007F);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xCF, &Data);
	Struct->RSense = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm nXTable Registers (180h-18Bh). This registers contains a ModelGauge m5 Algorithm nXTable Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nXTableReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x80, &Data);
	Struct->XTable0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x81, &Data);
	Struct->XTable1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x82, &Data);
	Struct->XTable2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x83, &Data);
	Struct->XTable3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x84, &Data);
	Struct->XTable4 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x85, &Data);
	Struct->XTable5 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x86, &Data);
	Struct->XTable6 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x87, &Data);
	Struct->XTable7 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x88, &Data);
	Struct->XTable8 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x89, &Data);
	Struct->XTable9 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x8A, &Data);
	Struct->XTable10 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x8B, &Data);
	Struct->XTable11 = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm nOCVTable Registers (180h-18Bh). This registers contains a ModelGauge m5 Algorithm nOCVTable Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nOCVTableReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x90, &Data);
	Struct->OCVTable0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x91, &Data);
	Struct->OCVTable1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x92, &Data);
	Struct->OCVTable2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x93, &Data);
	Struct->OCVTable3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x94, &Data);
	Struct->OCVTable4 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x95, &Data);
	Struct->OCVTable5 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x96, &Data);
	Struct->OCVTable6 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x97, &Data);
	Struct->OCVTable7 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x98, &Data);
	Struct->OCVTable8 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x99, &Data);
	Struct->OCVTable9 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x9A, &Data);
	Struct->OCVTable10 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x9B, &Data);
	Struct->OCVTable11 = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}


/**
 * @brief  Read ModelGauge m5 Algorithm nQRTable Registers (1A0h-1A3h). This registers contains a ModelGauge m5 Algorithm nQRTable Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nQRTableReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA0, &Data);
	Struct->QRTable00 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA1, &Data);
	Struct->QRTable10 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA2, &Data);
	Struct->QRTable20 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA3, &Data);
	Struct->QRTable30 = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm Input EEPROME Registers (1C6h, 19Eh, 1B3h, 19Ch, 1A6h, 1A7h). This registers contains a ModelGauge m5 Algorithm Input EEPROME Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nGaugeInReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC6, &Data);
	Struct->FullSOCThr = (uint16_t) (Data & 0xFFF8) >> 3;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x9E, &Data);
	Struct->VE = (uint16_t) (Data & 0xFFF8) >> 7;
	Struct->VR = (uint8_t) (Data & 0xFFF8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB3, &Data);
	Struct->DesignCap = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x9C, &Data);
	Struct->IChgTerm = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA6, &Data);
	Struct->RComp0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA7, &Data);
	Struct->TempCo = (uint16_t) (Data);


	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm Configuration Registers (1B0h, 1B8h, 1B9h, 1BAh, 1BBh, 19Dh, 1B2h, 1B6h, 19Fh, 1C7h, 1B7h, 1B1h).
 * 		   This registers contains a ModelGauge m5 Algorithm Configuration Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nGaugeConfigReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB0, &Data);
	Struct->TAlrtEn = (Data & 0x0001);
	Struct->dSOCen = (Data & 0x0002) >> 1;
	Struct->Aen = (Data & 0x0004) >> 2;
	Struct->FTHRM = (Data & 0x0008) >> 3;
	Struct->ALSH = (Data & 0x0020) >> 5;
	Struct->COMMSH = (Data & 0x0040) >> 6;
	Struct->AtRateEn = (Data & 0x0080) >> 7;
	Struct->DisBlockRead = (Data & 0x0200) >> 9;
	Struct->PBen = (Data & 0x0400) >> 10;
	Struct->FIFOen = (Data & 0x0800) >> 11;
	Struct->VS = (Data & 0x1000) >> 12;
	Struct->TS = (Data & 0x2000) >> 13;
	Struct->SS= (Data & 0x4000) >> 14;
	Struct->PAen = (Data & 0x8000) >> 15;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB8, &Data);
	Struct->enSBS = (Data & 0x0001);
	Struct->enAF = (Data & 0x0004) >> 2;
	Struct->enMC = (Data & 0x0008) >> 3;
	Struct->enDC = (Data & 0x0010) >> 4;
	Struct->enVE = (Data & 0x0020) >> 5;
	Struct->enDP = (Data & 0x0040) >> 6;
	Struct->enICT = (Data & 0x0080) >> 7;
	Struct->enLCfg = (Data & 0x0100) >> 8;
	Struct->enRCfg = (Data & 0x0200) >> 9;
	Struct->enFCfg = (Data & 0x0400) >> 10;
	Struct->enSHA = (Data & 0x2000) >> 13;
	Struct->enX = (Data & 0x4000) >> 14;
	Struct->enOCV = (Data & 0x8000) >> 15;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB9, &Data);
	Struct->enCTE = (Data & 0x0002) >> 1;
	Struct->enAT = (Data & 0x0008) >> 3;
	Struct->enTTF = (Data & 0x0010) >> 4;
	Struct->enProtChksm = (Data & 0x0040) >> 6;
	Struct->enJ  = (Data & 0x0080) >> 7;
	Struct->enProt = (Data & 0x0100) >> 8;
	Struct->enSC = (Data & 0x0200) >> 9;
	Struct->enJP = (Data & 0x0400) >> 10;
	Struct->enFTh = (Data & 0x2000) >> 13;
	Struct->enMtl = (Data & 0x4000) >> 14;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBA, &Data);
	Struct->FibScl = (Data & 0x0003);
	Struct->FibMax = (Data & 0x001C) >> 2;
	Struct->enFL = (Data & 0x0020) >> 5;
	Struct->enMet  = (Data & 0x0080) >> 7;
	Struct->enFC = (Data & 0x0200) >> 9;
	Struct->enVT = (Data & 0x0400) >> 10;
	Struct->enMMC = (Data & 0x0800) >> 11;
	Struct->enMMV = (Data & 0x1000) >> 12;
	Struct->enMMT = (Data & 0x2000) >> 13;
	Struct->enT = (Data & 0x8000) >> 15;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBB, &Data);
	Struct->HibScalar = (Data & 0x0007);
	Struct->HibExitTime = (Data & 0x0018) >> 3;
	Struct->HibThreshold = (Data & 0x0F00) >> 8;
	Struct->HibEnterTime  = (Data & 0x7000) >> 12;
	Struct->EnHib = (Data & 0x8000) >> 15;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x9D, &Data);
	Struct->CURR = (Data & 0x000F);
	Struct->VOLT = (Data & 0x0070) >> 4;
	Struct->MIX = (Data & 0x0780) >> 7;
	Struct->TEMP  = (Data & 0x3800) >> 11;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB2, &Data);
	Struct->SACFG = (Data & 0x0003);
	Struct->MR = (Data & 0x03E0) >> 5;
	Struct->FUS = (Data & 0xF000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB6, &Data);
	Struct->dt = (Data & 0x000F);
	Struct->dV = (Data & 0x01F0) >> 4;
	Struct->LOAD = (Data & 0xFE00) >> 9;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x9F, &Data);
	Struct->LS = (Data & 0x0070) >> 4;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC7, &Data);
	Struct->CVMixCapRatio = (uint8_t) ((Data & 0x00FF));
	Struct->CVHalfTime = (uint8_t) ((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB7, &Data);
	Struct->RepL_per_stg = (Data & 0x0007);
	Struct->MinSlopeX = (Data & 0x0078) >> 3;
	Struct->VoltLowOff = (Data & 0x0F80) >> 7;
	Struct->RepLow  = (Data & 0xF000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB1, &Data);
	Struct->NR = (Data & 0x0007);
	Struct->kDV = (Data & 0xFFF8) >> 3;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Configuration Registers (00Bh, 0ABh, 029h, 0A0h, 0A1h, 0B6h, 0B7h, 0D0h, 021h). This registers contains a Configuration Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_ConfigReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0B, &Data);
	Struct->PAen = (Data & 0x0001);
	Struct->Aen = (Data & 0x0004) >> 2;
	Struct->FTHRM = (Data & 0x0008) >> 3;
	Struct->ETHRM = (Data & 0x0010) >> 4;
	Struct->COMMSH = (Data & 0x0040) >> 6;
	Struct->SHIP = (Data & 0x0080) >> 7;
	Struct->DisBlockRead = (Data & 0x0200) >> 9;
	Struct->PBen = (Data & 0x0400) >> 10;
	Struct->DisLDO = (Data & 0x0800) >> 11;
	Struct->VS = (Data & 0x1000) >> 12;
	Struct->TS = (Data & 0x2000) >> 13;
	Struct->SS= (Data & 0x4000) >> 14;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAB, &Data);
	Struct->DRCfg = (Data & 0x000C) >> 2;
	Struct->TAlrtEn = (Data & 0x0040) >> 6;
	Struct->dSOCen = (Data & 0x0080) >> 7;
	Struct->POWR = (Data & 0x0F00) >> 8;
	Struct->ADCFIFOen = (Data & 0x1000) >> 12;
	Struct->AtRtEn = (Data & 0x2000) >> 13;
	Struct->POR_CMD = (Data & 0x8000) >> 15;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x29, &Data);
	Struct->CURR = (Data & 0x000F);
	Struct->VOLT = (Data & 0x0070) >> 4;
	Struct->MIX = (Data & 0x0780) >> 7;
	Struct->TEMP  = (Data & 0x3800) >> 11;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA0, &Data);
	Struct->dt = (Data & 0x000F);
	Struct->dV = (Data & 0x01F0) >> 4;
	Struct->LOAD = (Data & 0xFE00) >> 9;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA1, &Data);
	Struct->LS = (Data & 0x0070) >> 4;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB6, &Data);
	Struct->CVMixCapRatio = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB7, &Data);
	Struct->CVHalfTime = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD0, &Data);
	Struct->EmptySocHold = (Data & 0x001F);
	Struct->EmptyVHold = (Data & 0x0FE0) >> 4;
	Struct->HoldEn = (Data & 0x1000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x21, &Data);
	Struct->Device = (uint8_t)(Data & 0x000F);
	Struct->Revision = (uint16_t)((Data & 0xFFF0) >> 4);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read ModelGauge m5 Algorithm Additional Registers (00Ch, 0FFh, 0FBh, 04Dh, 04Eh, 01Fh, 00Eh, 00Dh, 02Bh, 04Ah, 03Dh).
 * 		   This registers contains a ModelGauge m5 Algorithm data which may be useful for debugging.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_GaugeAddReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0C, &Data);
	Struct->QRes = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xFF, &Data);
	Struct->VFSOC = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xFB, &Data);
	Struct->VFOCV = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x4D, &Data);
	Struct->QH = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x4E, &Data);
	Struct->QL = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1F, &Data);
	Struct->AvCap = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0E, &Data);
	Struct->AvSOC = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0D, &Data);
	Struct->MixSOC = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x2B, &Data);
	Struct->MixCap = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x4A, &Data);
	Struct->VFRemCap = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x3D, &Data);
	Struct->DNR = (Data & 0x0001);
	Struct->RelDt2 = (Data & 0x0040) >> 6;
	Struct->EDet = (Data & 0x0100) >> 8;
	Struct->RelDt  = (Data & 0x0200) >> 9;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nID Registers (1BCh-1BFh, 1CCh-1CEh, 1E8h-1EFh). This registers contains a a unique 64-bit identification value that is contained in the nROMID registers.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nIDReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBC, &Data);
	Struct->ROMID0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBD, &Data);
	Struct->ROMID1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBE, &Data);
	Struct->ROMID2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBF, &Data);
	Struct->ROMID3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xBF, &Data);
	Struct->ROMID3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xCC, &Data);
	Struct->ManfName0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xCD, &Data);
	Struct->ManfName1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xCE, &Data);
	Struct->ManfName2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE8, &Data);
	Struct->SerNum0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE9, &Data);
	Struct->SerNum1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xEA, &Data);
	Struct->SerNum2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xEB, &Data);
	Struct->DevName0 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xEC, &Data);
	Struct->DevName1 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xED, &Data);
	Struct->DevName2 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xEE, &Data);
	Struct->DevName3 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xEF, &Data);
	Struct->DevName4 = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read AtRate Registers (004h, 0DCh, 0DDh, 0DEh, 0DFh).
 * 		   The AtRate function allows the host software to see the theoretical remaining time or capacity for any given load current.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_AtRateReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x04, &Data);
	Struct->AtRate = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDC, &Data);
	Struct->AtQRes = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDD, &Data);
	Struct->AtTTE = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDE, &Data);
	Struct->AtAvSOC = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDF, &Data);
	Struct->AtAvCap = (uint16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nAlert Registers (18Ch, 18Dh, 18Eh, 18Fh). This registers allow interrupts to be generated.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nAlertReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x8C, &Data);
	Struct->VMIN = (uint8_t) (Data & 0x00FF);
	Struct->VMAX = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x8D, &Data);
	Struct->TMIN = (uint8_t) (Data & 0x00FF);
	Struct->TMAX = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x8E, &Data);
	Struct->CURRMIN = (uint8_t) (Data & 0x00FF);
	Struct->CURRMAX = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x8F, &Data);
	Struct->SMIN = (uint8_t) (Data & 0x00FF);
	Struct->SMAX = (uint8_t)((Data & 0xFF00) >> 8);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Alert Registers (001h, 002h, 003h, 0ACh). This registers allow interrupts to be generated.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_AlertReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x01, &Data);
	Struct->VMIN = (uint8_t) (Data & 0x00FF);
	Struct->VMAX = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x02, &Data);
	Struct->TMIN = (uint8_t) (Data & 0x00FF);
	Struct->TMAX = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAC, &Data);
	Struct->CURRMIN = (uint8_t) (Data & 0x00FF);
	Struct->CURRMAX = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x03, &Data);
	Struct->SMIN = (uint8_t) (Data & 0x00FF);
	Struct->SMAX = (uint8_t)((Data & 0xFF00) >> 8);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nProtect Registers (1B5h, 1D0h, 1D9h, 1DAh, 1D4h, 1DDh, 1DEh, 1D3h, 1D8h). This registers contain protection register data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nProtectReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xB5, &Data);
	Struct->NCELLS = (Data & 0x0003);
	Struct->NThrms = (Data & 0x001C) >> 2;
	Struct->CPCfg = (Data & 0x0300) >> 8;
	Struct->THType = (Data & 0x0800) >> 11;
	Struct->BtPkEn = (Data & 0x2000) >> 13;
	Struct->AOCfg = (Data & 0xC000) >> 14;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD0, &Data);
	Struct->UVShdn = (Data & 0x000F);
	Struct->UOCVP = (Data & 0x01F0) >> 4;
	Struct->UVPTh = (Data & 0xF800) >> 10;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD9, &Data);
	Struct->dHotChargeV = (Data & 0x0007);
	Struct->dColdChargeV = (Data & 0x0038) >> 3;
	Struct->dWarmChargeV = (Data & 0x00C0) >> 6;
	Struct->RoomChargeV = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDA, &Data);
	Struct->dOVPR = (Data & 0x000F);
	Struct->dOVP = (Data & 0x00F0) >> 4;
	Struct->ChgDetTh = (Data & 0x0700) >> 8;
	Struct->OVPPermFail = (Data & 0xF000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD4, &Data);
	Struct->Imbal = (Data & 0x001F);
	Struct->Rmismatch = (Data & 0x03E0) >> 5;
	Struct->BALCFG = (Data & 0x01C0) >> 10;
	Struct->Zener = (Data & 0x2000) >> 13;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDD, &Data);
	Struct->ODTH = (Data & 0x001F);
	Struct->SCTH = (Data & 0x03E0) >> 5;
	Struct->OCTH = (Data & 0x7C00) >> 10;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDE, &Data);
	Struct->OCDLY = (Data & 0x000F);
	Struct->SCDLY = (Data & 0x0F00) >> 8;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD3, &Data);
	Struct->ODCPTh = (uint8_t) (Data & 0x00FF);
	Struct->OCCPTh = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD8, &Data);
	Struct->HotCOEF = (Data & 0x0007);
	Struct->ColdCOEF = (Data & 0x0380) >> 3;
	Struct->WarmCOEF = (Data & 0x00C0) >> 6;
	Struct->RoomChgCurrent = (uint8_t)((Data & 0xFF00) >> 8);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nTemperature Protection Registers (1D1h, 1D5h, 1D2h, 1D6h). This registers contain temperature protection register data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nTempProtReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD1, &Data);
	Struct->T1 = (uint8_t) (Data & 0x00FF);
	Struct->T4 = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD5, &Data);
	Struct->T2 = (uint8_t) (Data & 0x00FF);
	Struct->T3 = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD2, &Data);
	Struct->Twarm = (uint8_t) (Data & 0x00FF);
	Struct->TpermFailHot = (uint8_t)((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xD6, &Data);
	Struct->DieTempTh = (Data & 0x000F);
	Struct->CurrDet = (Data & 0x00F0) >> 4;
	Struct->TooHotDchg = (Data & 0x0F00) >> 8;
	Struct->QovflwTh = (Data & 0xF000) >> 12;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nFault Timer Registers (1DCh, 1DFh, 1C9h, 16Fh). This registers contain fault timer register data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nFaultTimerReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDC, &Data);
	Struct->UVPTimer = (Data & 0x0003);
	Struct->TempTimer = (Data & 0x000C) >> 2;
	Struct->PermFailTimer = (Data & 0x0030) >> 4;
	Struct->OverCurrTimer = (Data & 0x01C0) >> 6;
	Struct->OVPTimer = (Data & 0x0600) >> 9;
	Struct->FullTimer = (Data & 0x3800) >> 11;
	Struct->CHGWDT = (Data & 0xC000) >> 14;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDF, &Data);
	Struct->CheckSum = (uint8_t) (Data & 0x00FF);
	Struct->LeakCurrTh = (Data & 0x0F00) >> 8;
	Struct->CEEn = (Data & 0x2000) >> 13;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC9, &Data);
	Struct->LeakFaultCfg = (Data & 0x0003);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x6F, &Data);
	Struct->LeakCurrent = (uint16_t)(Data & 0x7FFF);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nFault Timer Registers (1D7h). This registers configure and report various protection and alert statuses.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nProtCfgReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDC, &Data);
	Struct->FetPFEn = (Data & 0x0004) >> 2;
	Struct->UVRdy = (Data & 0x0008) >> 3;
	Struct->OvrdEn = (Data & 0x0010) >> 4;
	Struct->DeepShpEn = (Data & 0x0020) >> 5;
	Struct->PFEn = (Data & 0x0040) >> 6;
	Struct->PreqEn = (Data & 0x0100) >> 8;
	Struct->CmOvrdEn = (Data & 0x0400) >> 10;
	Struct->SCTest = (Data & 0x1800) >> 11;
	Struct->FullEn = (Data & 0x2000) >> 13;
	Struct->ChgWDTEn = (Data & 0x8000) >> 15;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Double Registers (00Fh, 013h, 018h, 01Eh, 03Ah, 012h, 022h, 032h, 042h, 0B8h, 0B9h, 036h, 0B5h, 0ADh, 0AEh, 0A4h-0AAh).
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_DoubleReg (I2C_TypeDef *I2Cx, MAX17320_RegData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0F, &Data);
	Struct->SACFG = (Data & 0x0003);
	Struct->MR = (Data & 0x03E0) >> 5;
	Struct->FUS = (Data & 0xF000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x13, &Data);
	Struct->FullSOCThr = (uint16_t) (Data & 0xFFF8) >> 3;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x18, &Data);
	Struct->DesignCap = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1E, &Data);
	Struct->IChgTerm = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x3A, &Data);
	Struct->VE = (uint16_t) (Data & 0xFFF8) >> 7;
	Struct->VR = (uint8_t) (Data & 0xFFF8);
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x12, &Data);
	Struct->QRTable00 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x22, &Data);
	Struct->QRTable10 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x32, &Data);
	Struct->QRTable20 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x42, &Data);
	Struct->QRTable30 = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB8, &Data);
	Struct->CGTempCo = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB9, &Data);
	Struct->AgeForecast = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x36, &Data);
	Struct->IAvgEmpty = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB5, &Data);
	Struct->CVMixCap = (uint8_t) ((Data & 0x00FF));
	Struct->CVHalfT = (uint8_t) ((Data & 0xFF00) >> 8);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAD, &Data);
	Struct->MinVolt = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAE, &Data);
	Struct->MinCurrent = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA4, &Data);
	Struct->MaxPeakPwr = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA5, &Data);
	Struct->SusPeakPwr = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA6, &Data);
	Struct->PackResist = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA7, &Data);
	Struct->SysResist = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA8, &Data);
	Struct->MinSysV = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xA9, &Data);
	Struct->MPPCurr = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAA, &Data);
	Struct->SPPCurr = (int16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read nDouble Registers (1A9h, 1ABh, 1ACh, 1ADh, 1AFh, 1C2h, 1C3h, 1C4h, 1C5h, 1CAh, 1DBh, 1E0h-1E3h, 1E6h, 1E7h).
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_nDoubleReg (I2C_TypeDef *I2Cx, MAX17320_RegDataEEPROME *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA9, &Data);
	Struct->FullCapRep = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xAB, &Data);
	Struct->MaxCurr = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinCurr = (int8_t) (Data & 0x00FF);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xAC, &Data);
	Struct->MaxVCELL = (uint8_t) (Data & 0xFF00) >> 8;
	Struct->MinVCELL = (uint8_t) (Data & 0x00FF);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xAD, &Data);
	Struct->MaxTemp = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinTemp = (int8_t) (Data & 0x00FF);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xAF, &Data);
	Struct->TimerH = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC2, &Data);
	Struct->PreChgCurr = (Data & 0x001F);
	Struct->HeatLim = (Data & 0x00E0) >> 5;
	Struct->PreQualVolt = (Data & 0x1F00) >> 8;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC3, &Data);
	Struct->ChgCtl = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC4, &Data);
	Struct->RGain = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC5, &Data);
	Struct->PackRes = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xC8, &Data);
	Struct->COff = (uint8_t) (Data & 0x003F);
	Struct->CGain  = (uint16_t) ((Data & 0xFFC0) >> 6);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xCA, &Data);
	Struct->ThermCfg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xDB, &Data);
	Struct->StepdV1 = (Data & 0x000F);
	Struct->StepdV0 = (Data & 0x00F0) >> 4;
	Struct->StepCurr2 = (Data & 0x0F00) >> 8;
	Struct->StepCurr1 = (Data & 0xF000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE0, &Data);
	Struct->DPLimit = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE1, &Data);
	Struct->ScOcvLim = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE2, &Data);
	Struct->CycleStart = (uint8_t) ((Data & 0x0FE0) >> 5);
	Struct->DeadTrgRt = (Data & 0xF000) >> 12;
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE3, &Data);
	Struct->Vdesign = (int8_t) (Data & 0xFF00) >> 8;
	Struct->Vminsys = (int8_t) (Data & 0x00FF);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE6, &Data);
	Struct->ManfctrDate = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xE7, &Data);
	Struct->FirstUsed = (uint16_t) (Data);


	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Full Reset MAX17320.
 * @param  I2Cx - Port I2C
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Write_FullReset (I2C_TypeDef *I2Cx) {

	uint16_t Data_key = 0, Data = 0;
	uint8_t POR_bit = 0;
	uint8_t attempt = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data_key);

	Data_key = (uint16_t)(Data_key & 0x0300);

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x60, 0x000F);
	LL_mDelay(10);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0xAB, 0x8000);
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAB, &Data);
	POR_bit = (Data & 0x8000) >> 15;
	while((POR_bit != 0) && (attempt < 50)){
		Data = 0;
		LL_mDelay(1);
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAB, &Data);
		POR_bit = (Data & 0x8000) >> 15;
		attempt++;
	}
	if(POR_bit != 0){
		Error--;
	}
	Data_key = (Data_key | 0x00F9);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  FUEL GAUGE Reset MAX17320.
 * @param  I2Cx - Port I2C
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Write_FuelGaugeReset (I2C_TypeDef *I2Cx) {

	uint16_t Data_key = 0, Data = 0;
	uint8_t POR_bit = 0;
	uint8_t attempt = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data_key);

	Data_key = (uint16_t)(Data_key & 0x0300);

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0xAB, 0x8000);
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAB, &Data);
	POR_bit = (Data & 0x8000) >> 15;
	while((POR_bit != 0) || (attempt < 10)){
		Data = 0;
		LL_mDelay(1);
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xAB, &Data);
		POR_bit = (Data & 0x8000) >> 15;
		attempt++;
	}
	if(POR_bit != 0){
		Error--;
	}
	Data_key = (Data_key | 0x00F9);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  ON/OFF charge FETs MAX17320.
 * @param  I2Cx - Port I2C
 * @param  Chg_key - MAX17320_ON_FET - 1, MAX17320_OFF_FET - 0;
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Write_ON_OFF_CHRG_FET (I2C_TypeDef *I2Cx, uint8_t Chg_key) {

	uint16_t Data_key = 0, Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data_key);

	Data_key = (uint16_t)(Data & 0x0300);

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data);

	Data = (uint16_t) (Data & 0xFEFF);
	Data = (uint16_t) (Data | (!Chg_key << 8));

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data);

	Data_key = (Data | 0x00F9);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);


	if (Error == 0 ) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  ON/OFF discharge FETs MAX17320.
 * @param  I2Cx - Port I2C
 * @param  Dchg_key - MAX17320_ON_FET - 0, MAX17320_OFF_FET - 1;
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Write_ON_OFF_DCHRG_FET (I2C_TypeDef *I2Cx, uint8_t Dchg_key) {

	uint16_t Data_key = 0, Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data_key);

	Data_key = (uint16_t)(Data & 0x0300);

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data);

	Data = (uint16_t)(Data & 0xFDFF);
	Data = (uint16_t) (Data | (!Dchg_key << 9));

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data);

	Data_key = (Data | 0x00F9);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);

	if (Error == 0 ) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branche Battery data.
 * @param  I2Cx - Port I2C
 * @param  Chg_key - MAX17320_ON_FET - 0, MAX17320_OFF_FET - 1;
 * @param  Dchg_key - MAX17320_ON_FET - 0, MAX17320_OFF_FET - 1;
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Data_Reg (I2C_TypeDef *I2Cx) {

	uint16_t Error = 0;
	MAX17320_RegData Data;

	//Error = Error + MAX17320_Read_StatusReg (I2Cx, &Data); // 12 bit --
	Error = Error + MAX17320_Read_ProtStatusReg (I2Cx, &Data); // 16 bit
	Error = Error + MAX17320_Read_HistoryProtStatusReg (I2Cx, &Data); // 16 bit
	Error = Error + MAX17320_Read_VoltageReg (I2Cx, &Data); // 26 byte
	Error = Error + MAX17320_Read_CurrentReg (I2Cx, &Data); // 6 byte
	Error = Error + MAX17320_Read_PwrReg (I2Cx, &Data); // 4 byte
	Error = Error + MAX17320_Read_TempReg (I2Cx, &Data); // 26 byte
	//Error = Error + MAX17320_Read_ChgParamReg (I2Cx, &Data); // 4 byte ---
	//Error = Error + MAX17320_Read_nGaugeAddReg (I2Cx, &Data); // 20 byte + 4 bit rep ????
	Error = Error + MAX17320_Read_GaugeOutReg (I2Cx, &Data); // 24 byte total 110 byte 48 bit

	if (Error == 0 ) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch ProtStatus Register (0D9h). The Protection Status register contains the fault states of the protection state machine.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_ProtStatus_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xD9, &Data) == 0) {
		Struct->Ship = (Data & 0x0001);
		Struct->ResDFault = (Data & 0x0002) >> 1;
		Struct->ODCP = (Data & 0x0004) >> 2;
		Struct->UVP = (Data & 0x0008) >> 3;
		Struct->TooHotD = (Data & 0x0010) >> 4;
		Struct->DieHot = (Data & 0x0020) >> 5;
		Struct->PermFail = (Data & 0x0040) >> 6;
		Struct->Imbalance = (Data & 0x0080) >> 7;
		Struct->PreqF = (Data & 0x0100) >> 8;
		Struct->Qovflw = (Data & 0x0200) >> 9;
		Struct->OCCP = (Data & 0x0400) >> 10;
		Struct->OVP = (Data & 0x0800) >> 11;
		Struct->TooColdC = (Data & 0x1000) >> 12;
		Struct->Full = (Data & 0x2000) >> 13;
		Struct->TooHotC= (Data & 0x4000) >> 14;
		Struct->ChgWDT = (Data & 0x8000) >> 15;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch ProtAlrt Register (0AFh). The Protection Alerts register contains a history of any protection events that have been logged by the device.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_HistoryProtStatus_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xAF, &Data) == 0) {
		Struct->HisLDet = (Data & 0x0001);
		Struct->HisResDFault = (Data & 0x0002) >> 1;
		Struct->HisODCP = (Data & 0x0004) >> 2;
		Struct->HisUVP = (Data & 0x0008) >> 3;
		Struct->HisTooHotD = (Data & 0x0010) >> 4;
		Struct->HisDieHot = (Data & 0x0020) >> 5;
		Struct->HisPermFail = (Data & 0x0040) >> 6;
		Struct->HisImbalance = (Data & 0x0080) >> 7;
		Struct->HisPreqF = (Data & 0x0100) >> 8;
		Struct->HisQovflw = (Data & 0x0200) >> 9;
		Struct->HisOCCP = (Data & 0x0400) >> 10;
		Struct->HisOVP = (Data & 0x0800) >> 11;
		Struct->HisTooColdC = (Data & 0x1000) >> 12;
		Struct->HisFull = (Data & 0x2000) >> 13;
		Struct->HisTooHotC= (Data & 0x4000) >> 14;
		Struct->HisChgWDT = (Data & 0x8000) >> 15;

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Balancing Registers (1F5h). This registers contains a Balancing data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  Battery_number - number of battery in branch.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Balancing_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Battery_number) {

	uint16_t Data = 0;
	if (I2C_Read_MAX17320(I2Cx, 0x6C, 0xF5, &Data) == 0) {
		if(Battery_number == 2){
			Struct->BalCell1 = (Data & 0x0080) >> 7;
			Struct->BalCell4 = (Data & 0x0200) >> 9;
		} else if(Battery_number == 3){
			Struct->BalCell1 = (Data & 0x0080) >> 7;
			Struct->BalCell3 = (((Data & 0x0080) >> 7) & ((Data & 0x0100) >> 8));
			Struct->BalCell4 = (Data & 0x0200) >> 9;
		} else if(Battery_number == 4){
			Struct->BalCell1 = (Data & 0x0080) >> 7;
			Struct->BalCell2 = (Data & 0x0100) >> 8;
			Struct->BalCell3 = (((Data & 0x0080) >> 7) & ((Data & 0x0100) >> 8));
			Struct->BalCell4 = (Data & 0x0200) >> 9;
		}

		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Voltage Measurement Registers (0D1h-0D8h, 0DAh, 008h). This registers contains a Voltage Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  ConfigBattBr - Number of battery in branch (MAX17320_2S_batt - 2 battery, MAX17320_3S_batt - 3 battery, MAX17320_4S_batt - 4 battery,)
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Voltage_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t ConfigBattBr) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD8, &Data);
	Data = (((Data & 0x00FF) << 8)|((Data & 0xFF00) >> 8));
	Struct->Cell1_Reg = (uint16_t) (Data);
	Data = 0;
	if ((ConfigBattBr == MAX17320_3S_batt) || (ConfigBattBr == MAX17320_4S_batt)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD6, &Data);
		Struct->Cell2_Reg = (uint16_t) (Data);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD2, &Data);
		Struct->AvgCell2_Reg = (uint16_t) (Data);
		Data = 0;
	}
	if (ConfigBattBr == MAX17320_4S_batt) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD5, &Data);
		Struct->Cell3_Reg = (uint16_t) (Data);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD1, &Data);
		Struct->AvgCell3_Reg = (uint16_t) (Data);
		Data = 0;
	}
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD7, &Data);
	Struct->Cell4_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD4, &Data);
	Struct->AvgCell1_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD1, &Data);
	Struct->AvgCell4_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDA, &Data);
	Struct->Batt_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x08, &Data);
	Struct->MaxVCELL_Reg = (uint8_t) (Data & 0xFF00) >> 8;
	Struct->MinVCELL_Reg = (uint8_t) (Data & 0x00FF);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Voltage Measurement in mV (0D1h-0D8h, 0DAh, 008h). This registers contains a Voltage Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  ConfigBattBr - Number of battery in branch (MAX17320_2S_batt - 2 battery, MAX17320_3S_batt - 3 battery, MAX17320_4S_batt - 4 battery,)
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Voltage_mV (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t ConfigBattBr) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD8, &Data);
	Struct->Cell1_mV = (uint16_t) (Data * MAX17320_LSB_mV);
	Data = 0;
	if ((ConfigBattBr == MAX17320_3S_batt) || (ConfigBattBr == MAX17320_4S_batt)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD6, &Data);
		Struct->Cell3_mV = (uint16_t) (Data * MAX17320_LSB_mV);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD2, &Data);
		Struct->AvgCell3_mV = (uint16_t) (Data * MAX17320_LSB_mV);
		Data = 0;
	}
	if (ConfigBattBr == MAX17320_4S_batt) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD5, &Data);
		Struct->Cell2_mV = (uint16_t) (Data * MAX17320_LSB_mV);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD1, &Data);
		Struct->AvgCell2_mV = (uint16_t) (Data * MAX17320_LSB_mV);
		Data = 0;
	}
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD7, &Data);
	Struct->Cell4_mV = (uint16_t) (Data * MAX17320_LSB_mV);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD4, &Data);
	Struct->AvgCell1_mV = (uint16_t) (Data * MAX17320_LSB_mV);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xD3, &Data);
	Struct->AvgCell4_mV = (uint16_t) (Data * MAX17320_LSB_mV);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xDA, &Data);
	Struct->Batt_mV = (uint16_t) (Data * 625 / 2000);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x08, &Data);
	Struct->MaxVCELL_mV = (uint32_t) (((Data & 0xFF00) >> 8) * 20); // LSB 20mV
	Struct->MinVCELL_mV = (uint32_t) ((Data & 0x00FF) * 20); // LSB 20mV

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Current Measurement Registers (01Ch, 01Dh, 00Ah). This registers contains a Current Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Current_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1C, &Data);
	Struct->Curr_Reg = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1D, &Data);
	Struct->AvgCurr_Reg = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0A, &Data);
	Struct->MaxCurr_Reg = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinCurr_Reg = (int8_t) (Data & 0x00FF);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Current Measurement in mA (01Ch, 01Dh, 00Ah). This registers contains a Current Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  Rsense - Value of current shunt in mOhm
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Current_mA (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Rsense) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1C, &Data);
	Struct->Curr_mA = (int16_t) ((int16_t) Data * MAX17320_LSB_mA / Rsense);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x1D, &Data);
	Struct->AvgCurr_mA = (int16_t) ((int16_t) Data * MAX17320_LSB_mA / Rsense);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x0A, &Data);
	Struct->MaxCurr_mA = (int32_t) (((Data & 0xFF00) >> 8) * 0.4 / Rsense * 1000);
	Struct->MinCurr_mA = (int32_t) ((int8_t)((Data & 0x00FF)) * 0.4 / Rsense * 1000);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Power Measurement Registers (0B1h, 0B3h). This registers contains a Power Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Pwr_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB1, &Data);
	Struct->Pwr_Reg = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB3, &Data);
	Struct->AvgPwr_Reg = (int16_t) (Data);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Power Measurement in mW (0B1h, 0B3h). This registers contains a Power Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  Rsense - Value of current shunt in mOhm
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Pwr_mW (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Rsense) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB1, &Data);
	Struct->Pwr_mW = (int16_t) ((int16_t) Data * 0.16 * Rsense);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB3, &Data);
	Struct->AvgPwr_mW = (int16_t) ((int16_t) Data * 0.16 * Rsense);

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Temperature Measurement Registers (01Bh, 016h, 009h, 034h, 040h, 133h-13Ah). This registers contains a Temperature Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  NumTermistors - Number of termistor in branch (MAX17320_0_termistors - MAX17320_4_termistors)
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Temp_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t NumTermistors) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x09, &Data);
	Struct->MaxTemp_Reg = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinTemp_Reg = (int8_t) (Data & 0x00FF);
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x34, &Data);
	Struct->DieTemp_Reg = (int16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x40, &Data);
	Struct->AvgDieTemp_Reg = (int16_t) (Data);
	Data = 0;
	if ((NumTermistors == MAX17320_1_termistors) || (NumTermistors == MAX17320_2_termistors)
			|| (NumTermistors == MAX17320_3_termistors) || (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x3A, &Data);
		Struct->Temp1_Reg = (int16_t) (Data);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x36, &Data);
		Struct->AvgTemp1_Reg = (int16_t) (Data);
		Data = 0;
	}
	if ((NumTermistors == MAX17320_2_termistors) || (NumTermistors == MAX17320_3_termistors)
			|| (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x39, &Data);
		Struct->Temp2_Reg = (int16_t) (Data);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x35, &Data);
		Struct->AvgTemp2_Reg = (int16_t) (Data);
		Data = 0;
	}
	if ((NumTermistors == MAX17320_3_termistors) || (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x38, &Data);
		Struct->Temp3_Reg = (int16_t) (Data);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x34, &Data);
		Struct->AvgTemp3_Reg = (int16_t) (Data);
		Data = 0;
	}
	if ((NumTermistors == MAX17320_3_termistors) || (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x37, &Data);
		Struct->Temp4_Reg = (int16_t) (Data);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x33, &Data);
		Struct->AvgTemp4_Reg = (int16_t) (Data);
		Data = 0;
	}

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch Temperature Measurement in degree ( 009h, 034h, 040h, 133h-13Ah). This registers contains a Temperature Measurement data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  NumTermistors - Number of termistor in branch (MAX17320_0_termistors - MAX17320_4_termistors)
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_Temp_Dg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t NumTermistors) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x09, &Data);
	Struct->MaxTemp_Dg = (int8_t) (Data & 0xFF00) >> 8;
	Struct->MinTemp_Dg = (int8_t) (Data & 0x00FF);
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x34, &Data);
	Struct->DieTemp_Dg = (int16_t) (Data * MAX17320_LSB_degree);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x40, &Data);
	Struct->AvgDieTemp_Dg = (int16_t) (Data * MAX17320_LSB_degree);
	Data = 0;
	if ((NumTermistors == MAX17320_1_termistors) || (NumTermistors == MAX17320_2_termistors)
			|| (NumTermistors == MAX17320_3_termistors) || (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x3A, &Data);
		Struct->Temp1_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x36, &Data);
		Struct->AvgTemp1_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
	}
	if ((NumTermistors == MAX17320_2_termistors) || (NumTermistors == MAX17320_3_termistors)
			|| (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x39, &Data);
		Struct->Temp2_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x35, &Data);
		Struct->AvgTemp2_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
	}
	if ((NumTermistors == MAX17320_3_termistors) || (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x38, &Data);
		Struct->Temp3_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x34, &Data);
		Struct->AvgTemp3_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
	}
	if ((NumTermistors == MAX17320_3_termistors) || (NumTermistors == MAX17320_4_termistors)) {
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x37, &Data);
		Struct->Temp4_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
		Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0x33, &Data);
		Struct->AvgTemp4_Dg = (int16_t) (Data * MAX17320_LSB_degree);
		Data = 0;
	}

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch ModelGauge m5 Algorithm Output Registers (005h-007h, 017h, 014h, 0B2h).
 * 		   This registers contains a ModelGauge m5 Algorithm Output Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_GaugeOut_Reg (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x05, &Data);
	Struct->RepCap_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x06, &Data);
	Struct->RepSOC_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x07, &Data);
	Struct->Age_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x17, &Data);
	Struct->Cycles_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x14, &Data);
	Struct->RCell_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB2, &Data);
	Struct->VRipple_Reg = (uint16_t) (Data);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA8, &Data);
	Struct->LeakCurr_Reg = (Data & 0x0ff);
	Struct->LDet = (Data & 0x0200) >> 9;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read Branch ModelGauge m5 Algorithm Output in real values (005h-007h, 017h, 014h, 0B2h).
 * 		   This registers contains a ModelGauge m5 Algorithm Output Registers data.
 * @param  I2Cx - Port I2C
 * @param  Struct - Returned struct with status data
 * @param  Rsense - Value of current shunt in mOhm
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_Read_Br_GaugeOut_mAh (I2C_TypeDef *I2Cx, MAX17320_BranchData *Struct, uint8_t Rsense) {

	uint16_t Data = 0;
	int8_t Error = 0;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x05, &Data);
	Struct->RepCap_mAh = (uint16_t) (Data * MAX17320_LSB_mAh / Rsense);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x06, &Data);
	Struct->RepSOC_Per = (uint16_t) (Data * MAX17320_LSB_degree);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x07, &Data);
	Struct->Age_Dg = (uint16_t) (Data * MAX17320_LSB_degree);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x17, &Data);
	Struct->Cycles_Cn = (uint16_t) (Data * 0.25);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x14, &Data);
	Struct->RCell_mOhm = (uint16_t) (Data * MAX17320_LSB_mOhm);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0xB2, &Data);
	Struct->VRipple_mV = (uint16_t) (Data * MAX17320_LSB_mVrip);
	Data = 0;
	Error = Error + I2C_Read_MAX17320(I2Cx, 0x16, 0xA8, &Data);
	Struct->LeakCurr_mA = (uint16_t)(((uint8_t)(Data & 0x0ff)) * MAX17320_LSB_mA / Rsense);
	Struct->LDet = (Data & 0x0200) >> 9;

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Write Branch ModelGauge m5 Algorithm Output mAh in real values (005h, 010h).
 * 		   This registers contains a ModelGauge m5 Algorithm Output Registers data.
 * @param  I2Cx - Port I2C
 * @param  Max_cap - Maximum value of capacity in mAh.
 * @param  AccmCharge - Value an accumulate charge of battery's in mAh.
 * @param  Rsense - Value of current shunt in mOhm
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus MAX17320_WriteAccmCharge (I2C_TypeDef *I2Cx, uint16_t AccmCharge, uint8_t Rsense) {

	uint16_t Data_key = 0, Data = 0;
	int8_t Error = 0;

	Data = AccmCharge * Rsense / MAX17320_LSB_mAh ;

	Error = Error + I2C_Read_MAX17320(I2Cx, 0x6C, 0x61, &Data_key);

	Data_key = (uint16_t)(Data_key & 0x0300);

	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x05, Data);
	Data_key = (Data_key | 0x00F9);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);
	LL_mDelay(1);
	Error = Error + I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x61, Data_key);

	/*if (I2C_Read_MAX17320(I2Cx, 0x6C, 0x10, &Data) == SUCCESS){
		Data = (uint16_t) (Data * MAX17320_LSB_mAh / Rsense);
		if(Data > Max_cap){
			Data = (uint16_t) (Max_cap * Rsense / MAX17320_LSB_mAh);
			if (I2C_Write_MAX17320(I2Cx, 0x6C, I2C_SIZE_REG_ADDR_U8, 0x10, Data) == SUCCESS ) {*/

				/*}
			}
		}*/

	if (Error == 0) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}


