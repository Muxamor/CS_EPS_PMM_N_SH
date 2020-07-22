
#ifndef DS2777_H
#define DS2777_H

#ifdef __cplusplus
extern "C" {
#endif

// Default setups for EEROM 1 address from 0x60 to 0x7F DS2777 PBM_v1.0
#define DS2777_ControlReg       	0x0C  		// Control Register
#define DS2777_AccumBias        	0x02   		// Accumulation Bias Register
#define DS2777_AgingCap        		0x1C20 		// Aging Capacity Register
#define DS2777_ChgVolt        		0xD3   		// Charge Voltage Register
#define DS2777_MinChgCurr        	0x18   		// Minimum Charge Current Register
#define DS2777_ActEmptVolt        	0x9A   		// Active-Empty Voltage Register
#define DS2777_ActEmptCurr        	0x25   		// Active-Empty Current Register
#define DS2777_ActEmpt40        	0x6E   		// Active-Empty 40 Register
#define DS2777_SensRes        		0x42  		// Sense Resistor Prime Register
#define DS2777_Full40        		0x1C20   	// Full 40 Register
#define DS2777_FullSlope4        	0x10   		// Full Segment 4 Slope Register
#define DS2777_FullSlope3       	0x17   		// Full Segment 3 Slope Register
#define DS2777_FullSlope2        	0x3C   		// Full Segment 2 Slope Register
#define DS2777_FullSlope1        	0x49  		// Full Segment 1 Slope Register
#define DS2777_AESlope4        		0x6 		// AE Segment 4 Slope Register
#define DS2777_AESlope3        		0x16  		// AE Segment 3 Slope Register
#define DS2777_AESlope2        		0x2C 		// AE Segment 2 Slope Register
#define DS2777_AESlope1        		0xE2 		// AE Segment 1 Slope Register
#define DS2777_SESlope4        		0x03  		// SE Segment 4 Slope Register
#define DS2777_SESlope3        		0x05  		// SE Segment 3 Slope Register
#define DS2777_SESlope2        		0x08  		// SE Segment 2 Slope Register
#define DS2777_SESlope1        		0x1B  		// SE Segment 1 Slope Register
#define DS2777_SensResGain      	0x1423   	// Sense-Resistor Gain Register
#define DS2777_SensResTemp        	0x02   		// Sense-Resistor Temperature Coefficient Register
#define DS2777_CurrOffset        	0x00  		// Current Offset Bias Register
#define DS2777_TBP34        		0x1E   		// TBP34 Register
#define DS2777_TBP23        		0x14   		// TBP23 Register
#define DS2777_TBP12        		0x0A   		// TBP12 Register
#define DS2777_RrtcThres        	0x5B  		// Protector Threshold Register

#define DS2777_Recall_EEPROM_0      0xB2  		// Address for recall data from EEPROM block 0
#define DS2777_Recall_EEPROM_1      0xB4  		// Address for recall data from EEPROM block 1
#define DS2777_Copy_EEPROM_0      	0x42  		// Address for copy data to EEPROM block 0
#define DS2777_Copy_EEPROM_1      	0x44  		// Address for copy data to EEPROM block 1
#define DS2777_Lock_EEPROM_0       	0x63  		// Address for lock data in EEPROM block 0
#define DS2777_Lock_EEPROM_1      	0x66  		// Address for lock data in EEPROM block 1

typedef struct  // Struct for EEPROM 1 DS2777
{
	uint8_t ControlReg;   	// Control Register
	uint8_t AccumBias;   	// Accumulation Bias Register
	uint16_t AgingCap;   	// Aging Capacity Register
	uint8_t ChgVolt;   		// Charge Voltage Register
	uint8_t MinChgCurr;   	// Minimum Charge Current Register
	uint8_t ActEmptVolt;   	// Active-Empty Voltage Register
	uint8_t ActEmptCurr;   	// Active-Empty Current Register
	uint8_t ActEmpt40;   	// Active-Empty 40 Register
	uint8_t SensRes;   		// Sense Resistor Prime Register
	uint16_t Full40;   		// Full 40 Register
	uint8_t FullSlope4;   	// Full Segment 4 Slope Register
	uint8_t FullSlope3;   	// Full Segment 3 Slope Register
	uint8_t FullSlope2;   	// Full Segment 2 Slope Register
	uint8_t FullSlope1;   	// Full Segment 1 Slope Register
	uint8_t AESlope4;   	// AE Segment 4 Slope Register
	uint8_t AESlope3;   	// AE Segment 3 Slope Register
	uint8_t AESlope2;   	// AE Segment 2 Slope Register
	uint8_t AESlope1;   	// AE Segment 1 Slope Register
	uint8_t SESlope4;   	// SE Segment 4 Slope Register
	uint8_t SESlope3;   	// SE Segment 3 Slope Register
	uint8_t SESlope2;   	// SE Segment 2 Slope Register
	uint8_t SESlope1;   	// SE Segment 1 Slope Register
	uint16_t SensResGain;   // Sense-Resistor Gain Register
	uint8_t SensResTemp;   	// Sense-Resistor Temperature Coefficient Register
	uint8_t CurrOffset;   	// Current Offset Bias Register
	uint8_t TBP34;   		// TBP34 Register
	uint8_t TBP23;   		// TBP23 Register
	uint8_t TBP12;   		// TBP12 Register
	uint8_t RrtcThres;   	// Protector Threshold Register
} DS2777_EEPROM_1;

typedef struct // The Status register reports events detected by the Li+ safety circuit
{
	uint8_t DchgEnableBit :1; 		// Discharge Enable Bit must be set to allow to drive the charge FET to the ON state
	uint8_t ChgEnableBit :1; 		// Charge Enable Bit must be set to allow to drive the charge FET to the ON state.
	uint8_t DchgControlFlag :1; // Discharge Control Flag indicates the logic state of the DC pin driver. The DC flag is read-only.
	uint8_t ChgControlFlag :1; // Charge Control Flag indicates the logic state of the CC pin driver. The CC flag is read-only.
	uint8_t LrnFlag :1; // LEARN Flag indicates that the current-charge cycle can be used to learn the battery capacity. The SEF flag is read-only.
	uint8_t StbEmptyFlag :1; // Standby-Empty Flag is set to indicate power battery is less than 10%. The SEF flag is read-only.
	uint8_t ActEmptyFlag :1; // Active-Empty Flag is set to indicate that the battery is at or below the active-empty point. The AEF flag is read-only.
	uint8_t ChgTerminateFlag :1; // Charge-Termination Flag is set to indicate that the battery charged.  CHGTF is read-only.
} DS2777_StatusReg;

ErrorStatus DS2777_RecallData(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AddrEEPROM);

ErrorStatus DS2777_CopyData(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AddrEEPROM);

ErrorStatus DS2777_LockData(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AddrEEPROM);

ErrorStatus DS2777_ReadStatus(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, DS2777_StatusReg *Struct);

ErrorStatus DS2777_EnableChgDchg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, DS2777_StatusReg *Struct);

ErrorStatus DS2777_EnableChg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t State);

ErrorStatus DS2777_EnableDchg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t State);

ErrorStatus DS2777_ReadActAbsCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *RAAC);

ErrorStatus DS2777_ReadActRelCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *RARC);

ErrorStatus DS2777_ReadStbAbsCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *RSAC);

ErrorStatus DS2777_ReadStbRelCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *RSRC);

ErrorStatus DS2777_ReadAverageCurrent(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *AverCurrent);

ErrorStatus DS2777_ReadCurrent(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *Current);

ErrorStatus DS2777_ReadTemperature(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, float *Temp);

ErrorStatus DS2777_ReadVoltageHi(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *Volt1);

ErrorStatus DS2777_ReadVoltageLo(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *Volt2);

ErrorStatus DS2777_ReadAccmCharge(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, int16_t *AccmCharge);

ErrorStatus DS2777_WriteAccmCharge(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t AccmCharge);

ErrorStatus DS2777_ReadAgeScalar(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *AgeScalar);

ErrorStatus DS2777_WriteAgeScalar(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t AgeScalar);

ErrorStatus DS2777_ReadSpecFeatReg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *SHA_IDLE, uint8_t *PIOB);

ErrorStatus DS2777_WriteSpecFeatReg(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t SHA_IDLE, uint8_t PIOB);

ErrorStatus DS2777_ReadCycleCounter(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *CycleCounter);

ErrorStatus DS2777_WriteCycleCounter(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t CycleCounter);

ErrorStatus DS2777_ReadEEPROMState(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint8_t *CopyFlag, uint8_t *Block_0_LockFlag, uint8_t *Block_1_LockFlag);

ErrorStatus DS2777_ReadFullCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *FullCapacity);

ErrorStatus DS2777_ReadActEmptyCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *ActEmptyCapacity);

ErrorStatus DS2777_ReadStbEmptyCapacity(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, uint16_t *StbEmptyCapacity);

ErrorStatus DS2777_ReadEEPROM_1(I2C_TypeDef *I2Cx, uint8_t AddrDS2777, DS2777_EEPROM_1 *Data);

#ifdef __cplusplus
}
#endif

#endif /* DS2777_H */
