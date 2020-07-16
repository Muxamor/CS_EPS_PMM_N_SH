#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "DS2777.h"

/**
 * @brief  Recalls the data of the targeted EEPROM block to its shadow RAM mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AddrEEPROM - Address of EEPROM block, 0xB2 - EEPROM block 0, 0xB4 - EEPROM block 1
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_RecallData(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AddrEEPROM) {

	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0xFE, AddrEEPROM);
}

/**
 * @brief  Copy the shadow RAM to the target EEPROM block mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AddrEEPROM - Address of EEPROM block, 0x42 - EEPROM block 0, 0x44 - EEPROM block 1
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_CopyData(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AddrEEPROM) {

	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0xFE, AddrEEPROM);
}

/**
 * @brief  Locks (write protects) the targeted EEPROM block mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AddrEEPROM - Address of EEPROM block, 0x63 - EEPROM block 0, 0x66 - EEPROM block 1
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_LockData(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AddrEEPROM) {

	I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x1F, 0x40);
	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0xFE, AddrEEPROM);
}

/**
 * @brief  Read battery status mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Data - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadStatus(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, DS2777_StatusReg *Struct) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x00, &Data) == 0) {
		Struct->DchgEnableBit = (Data & 0x0100) >> 8;
		Struct->ChgEnableBit = (Data & 0x0200) >> 9;
		Struct->DchgControlFlag = (Data & 0x0400) >> 10;
		Struct->ChgControlFlag = (Data & 0x0800) >> 11;
		Struct->LrnFlag = (Data & 0x10) >> 4;
		Struct->StbEmptyFlag = (Data & 0x20) >> 5;
		Struct->ActEmptyFlag = (Data & 0x40) >> 6;
		Struct->ChgTerminateFlag = (Data & 0x80) >> 7;
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Enable charge/discharge keys mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Struct - Returned struct with status data
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_EnableChgDchg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, DS2777_StatusReg *Struct) {

	uint8_t Reg_TX = 0;

	Reg_TX = (uint8_t) (Struct->DchgEnableBit | (Struct->ChgEnableBit << 1));
	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x00, Reg_TX);
}

/**
 * @brief  Enable charge keys mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  State - On - 1, Off - 0.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_EnableChg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t State) {

	uint8_t Reg_TX = 0;
	DS2777_StatusReg Struct;

	DS2777_ReadStatus(I2Cx, AddrDS2777, &Struct);

	Reg_TX = (uint8_t) ((State << 1) | Struct.DchgEnableBit);
	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x00, Reg_TX);
}

/**
 * @brief  Enable discharge keys mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  State - On - 1, Off - 0.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_EnableDchg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t State) {

	uint8_t Reg_TX = 0;
	DS2777_StatusReg Struct;

	DS2777_ReadStatus(I2Cx, AddrDS2777, &Struct);

	Reg_TX = (uint8_t) (State | (Struct.ChgEnableBit << 1));
	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x00, Reg_TX);
}

/**
 * @brief  Register Active Absolute Capacity reports the capacity available under the current temperature to the active-empty point in mAh.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  RAAC - Returned Active Absolute Capacity data in mAh
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadActAbsCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *RAAC) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x02, &Data) == 0) {
		*RAAC = (uint16_t) (((float) Data) * 1.6);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Register Active Relative Capacity reports the capacity available under the current temperature to the standby-empty point in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  RARC - Returned Active Relative Capacity data in %
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadActRelCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *RARC) {

	uint8_t Data = 0;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x06, &Data) == 0) {
		*RARC = Data;
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Register Standby Absolute Capacity reports the capacity available under the current temperature to the standby-empty point in mAh.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  RAAC - returned Standby Absolute Capacity data in mAh
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadStbAbsCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *RSAC) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x04, &Data) == 0) {
		*RSAC = (uint16_t) (((float) Data) * 1.6);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Register Standby Relative Capacity reports the capacity available under the current temperature to the standby-empty point in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  RSRC - returned Standby Relative Capacity data in %
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadStbRelCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *RSRC) {

	uint8_t Data = 0;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x07, &Data) == 0) {
		*RSRC = Data;
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read an average current level to/from battery branch over the preceding 28.16s in mA.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AverCurrent - Returned average current in mA. Current format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadAverageCurrent(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *AverCurrent) {

	uint16_t Data = 0;
	int16_t Datas = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x08, &Data) == 0) {
		Datas = (int16_t) Data;
		*AverCurrent = (int16_t) ((((float) Datas) * 1.5625 / 15));
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read a current level to/from battery branch over the preceding 3.52s in mA.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Current - Returned current in mA. Current format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadCurrent(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *Current) {

	uint16_t Data = 0;
	int16_t Datas = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x0E, &Data) == 0) {
		Datas = (int16_t) Data;
		*Current = (int16_t) ((((float) Datas) * 1.5625 / 15));
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read a temperature of DS2777 in �C.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Temp - Returned temperature of DS2777 in �C. Temperature format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadTemperature(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, float *Temp) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x0A, &Data) == 0) {
		*Temp = (((float) Data) * 0.125 / 32);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read a voltage of high battery (on 8.4V) in mV.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Volt1 - Returned voltage of lower battery (on 8.4V) in mV. Voltage format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadVoltageHi(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *Volt1) {

	uint16_t Data = 0;
	int16_t Datas = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x1C, &Data) == 0) {
		Datas = (int16_t) Data;
		*Volt1 = (int16_t) ((((float) Datas) * 4.883 / 32));
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read a voltage of lower battery (on GND) in mV.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Volt2 - Returned voltage of lower battery (on GND) in mV. Voltage format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadVoltageLo(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *Volt2) {

	uint16_t Data = 0;
	int16_t Datas = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x0C, &Data) == 0) {
		Datas = (int16_t) Data;
		*Volt2 = (int16_t) ((((float) Datas) * 4.883 / 32));
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read an accumulate charge capacity of battery's in mAh.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AccmCharge - Returned an accumulate charge of battery's in mAh. Accumulate charge format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadAccmCharge(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *AccmCharge) {

	uint16_t Data = 0;
	int16_t Datas = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x10, &Data) == 0) {
		Datas = (int16_t) Data;
		*AccmCharge = (int16_t) ((((float) Datas) * 6.25 / 15.0));
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Write an accumulate charge capacity of battery's in mAh.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AccmCharge - Value an accumulate charge of battery's in mAh. Accumulate charge format twos-complement.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_WriteAccmCharge(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t AccmCharge) {

	AccmCharge = (uint16_t) (((float) AccmCharge) * 15.0 / 6.25);
	return I2C_Write_word_u16_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x10, AccmCharge);
}

/**
 * @brief  Read adjusts the cell capacity estimation in % (calculated by DS2777).
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AgeScalar - Returned adjusts the cell capacity estimation in %.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadAgeScalar(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *AgeScalar) {

	uint8_t Data = 0;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x14, &Data) == 0) {
		*AgeScalar = (uint8_t) (((float) Data) * 0.78);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Write adjusts the cell capacity estimation in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  AccmCharge - Adjusts the cell capacity estimation in %.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_WriteAgeScalar(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AgeScalar) {

	AgeScalar = (uint8_t) (((float) AgeScalar) / 0.78);
	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x14, AgeScalar);
}

/**
 * @brief  Read special feature register.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  SHA_IDLE - Returned logic 1 while an SHA calculation is in progress, 0 when the calculation is complete.
 * @param  PIOB - Returned the logic level forced on the PIO pin.
 * @retval 0 - SUCCESS, -1 - ERROR
 */

ErrorStatus DS2777_ReadSpecFeatReg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *SHA_IDLE, uint8_t *PIOB) {

	uint8_t Data = 0;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x15, &Data) == 0) {
		*SHA_IDLE = (uint8_t) ((Data & 0x02) >> 1);
		*PIOB = (uint8_t) (Data & 0x01);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Write special feature register.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  SHA_IDLE - Write 1 for an SHA calculation.
 * @param  PIOB - Write 0 forcing the PIO pin low, 1 to PIOB disables the output driver, allowing pin pulled high or used as an input.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_WriteSpecFeatReg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t SHA_IDLE, uint8_t PIOB) {

	uint8_t Reg_TX = 0;

	Reg_TX = (uint8_t) (((SHA_IDLE & 0x02) << 1) | (PIOB & 0x01));
	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x15, Reg_TX);
}

/**
 * @brief  Read cycle counter is an absolute count of the cumulative discharge cycles (calculated by DS2777).
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  CycleCounter - Returned value cycle counter.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadCycleCounter(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *CycleCounter) {

	uint8_t Data = 0;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x1E, &Data) == 0) {
		*CycleCounter = Data;
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Write adjusts the cell capacity estimation in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  CycleCounter - cycle counter is an absolute count of the cumulative discharge cycles.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_WriteCycleCounter(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t CycleCounter) {

	return I2C_Write_byte_St(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x1E, CycleCounter);
}

/**
 * @brief  Read access control of the EEPROM blocks.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  CopyFlag - Returned state of Copy Data command. 1 - writes to EEPROM are ignored, 0 - data can be written.
 * @param  Block_0_LockFlag - Returned 1 if EEPROM block 0 (addresses 20h-2Fh) is locked (read-only), 0 indicates is unlocked (read/write).
 * @param  Block_1_LockFlag - Returned 1 if EEPROM block 1 (addresses 60h-80h) is locked (read-only), 0 indicates is unlocked (read/write).
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadEEPROMState(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *CopyFlag, uint8_t *Block_0_LockFlag,
		uint8_t *Block_1_LockFlag) {

	uint8_t Data;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x1F, &Data) == 0) {
		*CopyFlag = (uint8_t) ((Data & 0x80) >> 7);
		*Block_1_LockFlag = (uint8_t) ((Data & 0x02) >> 1);
		*Block_0_LockFlag = (uint8_t) ((Data & 0x01));
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read full capacity of the battery at the present temperature in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  FullCapacity - Returned value full capacity of the battery at the present temperature in %.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadFullCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *FullCapacity) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x16, &Data) == 0) {
		*FullCapacity = (uint16_t) (((float) Data) * 100 / 32767);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read active empty capacity of the battery at the present temperature in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  FullCapacity - Returned value active empty capacity of the battery at the present temperature in %.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadActEmptyCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *ActEmptyCapacity) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x18, &Data) == 0) {
		*ActEmptyCapacity = (uint16_t) (((float) Data) * 100 / 8191);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read standby empty capacity of the battery at the present temperature in %.
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  FullCapacity - Returned value standby empty capacity of the battery at the present temperature in %.
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadStbEmptyCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *StbEmptyCapacity) {

	uint16_t Data = 0;

	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x1A, &Data) == 0) {
		*StbEmptyCapacity = (uint16_t) (((float) Data) * 100 / 8191);
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/**
 * @brief  Read all register data from EEPROM 1 mux DS2777 in Battery Module
 * @param  I2Cx - Port I2C
 * @param  AddrDS2777 - Address of DS2777
 * @param  Data - returned Struct with data in EEPROM 1
 * @retval 0 - SUCCESS, -1 - ERROR
 */
ErrorStatus DS2777_ReadEEPROM_1(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, DS2777_EEPROM_1 *Data) {

	uint8_t Data8;
	uint16_t Data16;

	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x60, &Data8) == 0) {
		Data->ControlReg = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x61, &Data8) == 0) {
		Data->AccumBias = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x62, &Data16) == 0) {
		Data->AgingCap = Data16;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x64, &Data8) == 0) {
		Data->ChgVolt = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x65, &Data8) == 0) {
		Data->MinChgCurr = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x66, &Data8) == 0) {
		Data->ActEmptVolt = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x67, &Data8) == 0) {
		Data->ActEmptCurr = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x68, &Data8) == 0) {
		Data->ActEmpt40 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x69, &Data8) == 0) {
		Data->SensRes = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x6A, &Data16) == 0) {
		Data->Full40 = Data16;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x6C, &Data8) == 0) {
		Data->FullSlope4 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x6D, &Data8) == 0) {
		Data->FullSlope3 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x6E, &Data8) == 0) {
		Data->FullSlope2 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x6F, &Data8) == 0) {
		Data->FullSlope1 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x70, &Data8) == 0) {
		Data->AESlope4 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x71, &Data8) == 0) {
		Data->AESlope3 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x72, &Data8) == 0) {
		Data->AESlope2 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x73, &Data8) == 0) {
		Data->AESlope1 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x74, &Data8) == 0) {
		Data->SESlope4 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x75, &Data8) == 0) {
		Data->SESlope3 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x76, &Data8) == 0) {
		Data->SESlope2 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x77, &Data8) == 0) {
		Data->SESlope1 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_word_u16_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x78, &Data16) == 0) {
		Data->SensResGain = Data16;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x7A, &Data8) == 0) {
		Data->SensResTemp = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x7B, &Data8) == 0) {
		Data->CurrOffset = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x7C, &Data8) == 0) {
		Data->TBP34 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x7D, &Data8) == 0) {
		Data->TBP23 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x7E, &Data8) == 0) {
		Data->TBP12 = Data8;
	} else {
		return ERROR_N;
	}
	if (I2C_Read_byte_St_ReSt(I2Cx, AddrDS2777, I2C_SIZE_REG_ADDR_U8, 0x7F, &Data8) == 0) {
		Data->RrtcThres = Data8;
	} else {
		return ERROR_N;
	}
	return SUCCESS;
}

