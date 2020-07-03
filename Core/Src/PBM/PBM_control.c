#include "main.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "DS2777.h"
#include "SetupPeriph.h"
#include "PBM_config.h"
#include "PBM_struct.h"
#include "PBM_control.h"
#include "Error_Handler.h"

/** @brief	Read state pin PCA9534 for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_ReadGPIO(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM) {
!!!!!!!!!!!!!!!Задать дерекшин ЗАчем функция
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = -1;

	SW_TMUX1209_I2C_main_PBM();

	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = PCA9534_read_input_reg(I2Cx, PBM_1_PCA9534_ADDR, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_1.PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
			Data->PBM_Data_1.PCA9534_P1 = (uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
			Data->PBM_Data_1.PCA9534_P2 = (uint8_t) ((data8 & PCA9534_IO_P02) >> 2);
			Data->PBM_Data_1.PCA9534_AlarmState_2 = (uint8_t) ((data8 & PCA9534_IO_P03) >> 3);
			Data->PBM_Data_1.PCA9534_PIO_1 = (uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
			Data->PBM_Data_1.PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
			Data->PBM_Data_1.PCA9534_AlarmState_1 = (uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
			Data->PBM_Data_1.PCA9534_PIO_2 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

			Data->PBM_Data_1.Error_PCA9534 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_PCA9534 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = PCA9534_read_input_reg(I2Cx, PBM_2_PCA9534_ADDR, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_2.PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
			Data->PBM_Data_2.PCA9534_P1 = (uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
			Data->PBM_Data_2.PCA9534_P2 = (uint8_t) ((data8 & PCA9534_IO_P02) >> 2);
			Data->PBM_Data_2.PCA9534_AlarmState_2 = (uint8_t) ((data8 & PCA9534_IO_P03) >> 3);
			Data->PBM_Data_2.PCA9534_PIO_1 = (uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
			Data->PBM_Data_2.PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
			Data->PBM_Data_2.PCA9534_AlarmState_1 = (uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
			Data->PBM_Data_2.PCA9534_PIO_2 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

			Data->PBM_Data_2.Error_PCA9534 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_PCA9534 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = PCA9534_read_input_reg(I2Cx, PBM_3_PCA9534_ADDR, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_3.PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
			Data->PBM_Data_3.PCA9534_P1 = (uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
			Data->PBM_Data_3.PCA9534_P2 = (uint8_t) ((data8 & PCA9534_IO_P02) >> 2);
			Data->PBM_Data_3.PCA9534_AlarmState_2 = (uint8_t) ((data8 & PCA9534_IO_P03) >> 3);
			Data->PBM_Data_3.PCA9534_PIO_1 = (uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
			Data->PBM_Data_3.PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
			Data->PBM_Data_3.PCA9534_AlarmState_1 = (uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
			Data->PBM_Data_3.PCA9534_PIO_2 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

			Data->PBM_Data_3.Error_PCA9534 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_PCA9534 = 1;
		}
	}
}

/** @brief	Read temperature TMP1075 sensor for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_ReadTempSensor(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = -1;

	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_1_TMP1075_ADDR_1, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_1.TMP1075_temp_1 = data8;
			Data->PBM_Data_1.Error_TMP1075_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_TMP1075_1 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_1_TMP1075_ADDR_2, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_1.TMP1075_temp_2 = data8;
			Data->PBM_Data_1.Error_TMP1075_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_TMP1075_2 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_1_TMP1075_ADDR_3, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_1.TMP1075_temp_3 = data8;
			Data->PBM_Data_1.Error_TMP1075_3 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_TMP1075_3 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_1_TMP1075_ADDR_4, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_1.TMP1075_temp_4 = data8;
			Data->PBM_Data_1.Error_TMP1075_4 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_TMP1075_4 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_2_TMP1075_ADDR_1, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_2.TMP1075_temp_1 = data8;
			Data->PBM_Data_2.Error_TMP1075_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_1 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_2_TMP1075_ADDR_2, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_2.TMP1075_temp_2 = data8;
			Data->PBM_Data_2.Error_TMP1075_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_2 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_2_TMP1075_ADDR_3, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_2.TMP1075_temp_3 = data8;
			Data->PBM_Data_2.Error_TMP1075_3 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_3 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_2_TMP1075_ADDR_4, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_2.TMP1075_temp_4 = data8;
			Data->PBM_Data_2.Error_TMP1075_4 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_4 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_3_TMP1075_ADDR_1, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_3.TMP1075_temp_1 = data8;
			Data->PBM_Data_3.Error_TMP1075_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_1 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_3_TMP1075_ADDR_2, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_3.TMP1075_temp_2 = data8;
			Data->PBM_Data_3.Error_TMP1075_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_2 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_3_TMP1075_ADDR_3, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_3.TMP1075_temp_3 = data8;
			Data->PBM_Data_3.Error_TMP1075_3 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_3 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = TMP1075_read_int8_temperature(I2Cx, PBM_3_TMP1075_ADDR_4, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			Data->PBM_Data_3.TMP1075_temp_4 = data8;
			Data->PBM_Data_3.Error_TMP1075_4 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_4 = 1;
		}
	}
}

/** @brief	Read data from two DS2777 for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_ReadBatteryData(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM) {

	int8_t Error = -1;
	float Temp = 0;
	uint8_t count = 0;
	DS2777_StatusReg Struct;

	SW_TMUX1209_I2C_main_PBM();
	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_ReadStatus(I2Cx, PBM_1_DS2777_ADDR_1, &Struct);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_1.Branch_1_DchgEnableBit = Struct.DchgEnableBit;
			Data->PBM_Data_1.Branch_1_ChgEnableBit = Struct.ChgEnableBit;
			Data->PBM_Data_1.Branch_1_DchgControlFlag = Struct.DchgControlFlag;
			Data->PBM_Data_1.Branch_1_ChgControlFlag = Struct.ChgControlFlag;
			Data->PBM_Data_1.Branch_1_LrnFlag = Struct.LrnFlag;
			Data->PBM_Data_1.Branch_1_StbEmptyFlag = Struct.StbEmptyFlag;
			Data->PBM_Data_1.Branch_1_ActEmptyFlag = Struct.ActEmptyFlag;
			Data->PBM_Data_1.Branch_1_ChgTerminateFlag = Struct.ChgTerminateFlag;
			Error = DS2777_ReadStbRelCapacity(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_RelativeCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAccmCharge(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_AbcoluteCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAgeScalar(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_AgeScalar);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadCycleCounter(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_CycleCounter);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadTemperature(I2Cx, PBM_1_DS2777_ADDR_1, &Temp);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_1.Branch_1_Temperature = (int8_t) Temp;
			Error = DS2777_ReadCurrent(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_Current);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAverageCurrent(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_AverageCurrent);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageHi(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_VoltageHi);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageLo(I2Cx, PBM_1_DS2777_ADDR_1, &Data->PBM_Data_1.Branch_1_VoltageLo);
			count++;
		}
		if (Error == 0) {
			Data->PBM_Data_1.Error_DS2777_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_DS2777_1 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_ReadStatus(I2Cx, PBM_1_DS2777_ADDR_2, &Struct);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_1.Branch_2_DchgEnableBit = Struct.DchgEnableBit;
			Data->PBM_Data_1.Branch_2_ChgEnableBit = Struct.ChgEnableBit;
			Data->PBM_Data_1.Branch_2_DchgControlFlag = Struct.DchgControlFlag;
			Data->PBM_Data_1.Branch_2_ChgControlFlag = Struct.ChgControlFlag;
			Data->PBM_Data_1.Branch_2_LrnFlag = Struct.LrnFlag;
			Data->PBM_Data_1.Branch_2_StbEmptyFlag = Struct.StbEmptyFlag;
			Data->PBM_Data_1.Branch_2_ActEmptyFlag = Struct.ActEmptyFlag;
			Data->PBM_Data_1.Branch_2_ChgTerminateFlag = Struct.ChgTerminateFlag;
			Error = DS2777_ReadStbRelCapacity(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_RelativeCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAccmCharge(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_AbcoluteCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAgeScalar(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_AgeScalar);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadCycleCounter(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_CycleCounter);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadTemperature(I2Cx, PBM_1_DS2777_ADDR_2, &Temp);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_1.Branch_2_Temperature = (int8_t) Temp;
			Error = DS2777_ReadCurrent(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_Current);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAverageCurrent(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_AverageCurrent);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageHi(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_VoltageHi);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageLo(I2Cx, PBM_1_DS2777_ADDR_2, &Data->PBM_Data_1.Branch_2_VoltageLo);
			count++;
		}
		if (Error == 0) {
			Data->PBM_Data_1.Error_DS2777_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_1.Error_DS2777_2 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_ReadStatus(I2Cx, PBM_2_DS2777_ADDR_1, &Struct);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_2.Branch_1_DchgEnableBit = Struct.DchgEnableBit;
			Data->PBM_Data_2.Branch_1_ChgEnableBit = Struct.ChgEnableBit;
			Data->PBM_Data_2.Branch_1_DchgControlFlag = Struct.DchgControlFlag;
			Data->PBM_Data_2.Branch_1_ChgControlFlag = Struct.ChgControlFlag;
			Data->PBM_Data_2.Branch_1_LrnFlag = Struct.LrnFlag;
			Data->PBM_Data_2.Branch_1_StbEmptyFlag = Struct.StbEmptyFlag;
			Data->PBM_Data_2.Branch_1_ActEmptyFlag = Struct.ActEmptyFlag;
			Data->PBM_Data_2.Branch_1_ChgTerminateFlag = Struct.ChgTerminateFlag;
			Error = DS2777_ReadStbRelCapacity(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_RelativeCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAccmCharge(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_AbcoluteCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAgeScalar(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_AgeScalar);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadCycleCounter(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_CycleCounter);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadTemperature(I2Cx, PBM_2_DS2777_ADDR_1, &Temp);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_2.Branch_1_Temperature = (int8_t) Temp;
			Error = DS2777_ReadCurrent(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_Current);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAverageCurrent(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_AverageCurrent);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageHi(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_VoltageHi);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageLo(I2Cx, PBM_2_DS2777_ADDR_1, &Data->PBM_Data_2.Branch_1_VoltageLo);
			count++;
		}
		if (Error == 0) {
			Data->PBM_Data_2.Error_DS2777_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_DS2777_1 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_ReadStatus(I2Cx, PBM_2_DS2777_ADDR_2, &Struct);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_2.Branch_2_DchgEnableBit = Struct.DchgEnableBit;
			Data->PBM_Data_2.Branch_2_ChgEnableBit = Struct.ChgEnableBit;
			Data->PBM_Data_2.Branch_2_DchgControlFlag = Struct.DchgControlFlag;
			Data->PBM_Data_2.Branch_2_ChgControlFlag = Struct.ChgControlFlag;
			Data->PBM_Data_2.Branch_2_LrnFlag = Struct.LrnFlag;
			Data->PBM_Data_2.Branch_2_StbEmptyFlag = Struct.StbEmptyFlag;
			Data->PBM_Data_2.Branch_2_ActEmptyFlag = Struct.ActEmptyFlag;
			Data->PBM_Data_2.Branch_2_ChgTerminateFlag = Struct.ChgTerminateFlag;
			Error = DS2777_ReadStbRelCapacity(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_RelativeCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAccmCharge(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_AbcoluteCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAgeScalar(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_AgeScalar);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadCycleCounter(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_CycleCounter);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadTemperature(I2Cx, PBM_2_DS2777_ADDR_2, &Temp);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_2.Branch_2_Temperature = (int8_t) Temp;
			Error = DS2777_ReadCurrent(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_Current);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAverageCurrent(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_AverageCurrent);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageHi(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_VoltageHi);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageLo(I2Cx, PBM_2_DS2777_ADDR_2, &Data->PBM_Data_2.Branch_2_VoltageLo);
			count++;
		}
		if (Error == 0) {
			Data->PBM_Data_2.Error_DS2777_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_DS2777_2 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_ReadStatus(I2Cx, PBM_3_DS2777_ADDR_1, &Struct);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_3.Branch_1_DchgEnableBit = Struct.DchgEnableBit;
			Data->PBM_Data_3.Branch_1_ChgEnableBit = Struct.ChgEnableBit;
			Data->PBM_Data_3.Branch_1_DchgControlFlag = Struct.DchgControlFlag;
			Data->PBM_Data_3.Branch_1_ChgControlFlag = Struct.ChgControlFlag;
			Data->PBM_Data_3.Branch_1_LrnFlag = Struct.LrnFlag;
			Data->PBM_Data_3.Branch_1_StbEmptyFlag = Struct.StbEmptyFlag;
			Data->PBM_Data_3.Branch_1_ActEmptyFlag = Struct.ActEmptyFlag;
			Data->PBM_Data_3.Branch_1_ChgTerminateFlag = Struct.ChgTerminateFlag;
			Error = DS2777_ReadStbRelCapacity(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_RelativeCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAccmCharge(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_AbcoluteCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAgeScalar(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_AgeScalar);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadCycleCounter(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_CycleCounter);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadTemperature(I2Cx, PBM_3_DS2777_ADDR_1, &Temp);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_3.Branch_1_Temperature = (int8_t) Temp;
			Error = DS2777_ReadCurrent(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_Current);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAverageCurrent(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_AverageCurrent);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageHi(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_VoltageHi);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageLo(I2Cx, PBM_3_DS2777_ADDR_1, &Data->PBM_Data_3.Branch_1_VoltageLo);
			count++;
		}
		if (Error == 0) {
			Data->PBM_Data_3.Error_DS2777_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_DS2777_1 = 1;
		}
		Error = -1;
		count = 0;
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_ReadStatus(I2Cx, PBM_3_DS2777_ADDR_2, &Struct);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_3.Branch_2_DchgEnableBit = Struct.DchgEnableBit;
			Data->PBM_Data_3.Branch_2_ChgEnableBit = Struct.ChgEnableBit;
			Data->PBM_Data_3.Branch_2_DchgControlFlag = Struct.DchgControlFlag;
			Data->PBM_Data_3.Branch_2_ChgControlFlag = Struct.ChgControlFlag;
			Data->PBM_Data_3.Branch_2_LrnFlag = Struct.LrnFlag;
			Data->PBM_Data_3.Branch_2_StbEmptyFlag = Struct.StbEmptyFlag;
			Data->PBM_Data_3.Branch_2_ActEmptyFlag = Struct.ActEmptyFlag;
			Data->PBM_Data_3.Branch_2_ChgTerminateFlag = Struct.ChgTerminateFlag;
			Error = DS2777_ReadStbRelCapacity(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_RelativeCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAccmCharge(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_AbcoluteCapacity);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAgeScalar(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_AgeScalar);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadCycleCounter(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_CycleCounter);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadTemperature(I2Cx, PBM_3_DS2777_ADDR_2, &Temp);
			if (Error != 0) {
				break;
			};
			Data->PBM_Data_3.Branch_2_Temperature = (int8_t) Temp;
			Error = DS2777_ReadCurrent(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_Current);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadAverageCurrent(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_AverageCurrent);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageHi(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_VoltageHi);
			if (Error != 0) {
				break;
			};
			Error = DS2777_ReadVoltageLo(I2Cx, PBM_3_DS2777_ADDR_2, &Data->PBM_Data_3.Branch_2_VoltageLo);
			count++;
		}
		if (Error == 0) {
			Data->PBM_Data_3.Error_DS2777_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_DS2777_2 = 1;
		}
		Error = -1;
		count = 0;
	}

}

void Error_Counter(PBM_Data_All Data, int16_t *Er_PBM_1, int16_t *Er_PBM_2, int16_t *Er_PBM_3) {
	*Er_PBM_1 = *Er_PBM_1 + Data.PBM_Data_1.Error_DS2777_1 + Data.PBM_Data_1.Error_DS2777_2 + Data.PBM_Data_1.Error_TMP1075_1 + Data.PBM_Data_1.Error_TMP1075_2
			+ Data.PBM_Data_1.Error_TMP1075_3 + Data.PBM_Data_1.Error_TMP1075_4 + Data.PBM_Data_1.Error_PCA9534;
	*Er_PBM_2 = *Er_PBM_2 + Data.PBM_Data_2.Error_DS2777_1 + Data.PBM_Data_2.Error_DS2777_2 + Data.PBM_Data_2.Error_TMP1075_1 + Data.PBM_Data_2.Error_TMP1075_2
			+ Data.PBM_Data_2.Error_TMP1075_3 + Data.PBM_Data_2.Error_TMP1075_4 + Data.PBM_Data_2.Error_PCA9534;
	*Er_PBM_3 = *Er_PBM_3 + Data.PBM_Data_3.Error_DS2777_1 + Data.PBM_Data_3.Error_DS2777_2 + Data.PBM_Data_3.Error_TMP1075_1 + Data.PBM_Data_3.Error_TMP1075_2
			+ Data.PBM_Data_3.Error_TMP1075_3 + Data.PBM_Data_3.Error_TMP1075_4 + Data.PBM_Data_3.Error_PCA9534;

}
/** @brief	ON/OFF heat for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state heat (PBM_ON_HEAT or PBM_OFF_HEAT).
 @retval 	none
 */
void PBM_SetStateHeatBranch(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	uint8_t data8 = 0;
	uint8_t count = 0;
	uint8_t InputPin = PCA9534_IO_P01 | PCA9534_IO_P02 | PCA9534_IO_P03 | PCA9534_IO_P04 | PCA9534_IO_P06 | PCA9534_IO_P07;

	SW_TMUX1209_I2C_main_PBM();
	while ((Error != 0) && (count < 3)) {
		Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_1_PCA9534_ADDR, InputPin);
		if (Error != 0) {
			break;
		};
		Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_2_PCA9534_ADDR, InputPin);
		if (Error != 0) {
			break;
		};
		Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_3_PCA9534_ADDR, InputPin);
	}

	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				if (State == PBM_ON_HEAT) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, PBM_1_PCA9534_ADDR, PCA9534_IO_P00);
					if (Error != 0) {
						break;
					};
					Error = PCA9534_Set_output_pin(I2Cx, PBM_1_PCA9534_ADDR, PCA9534_IO_P00);
				} else {
					Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_1_PCA9534_ADDR, PCA9534_IO_P00);
				}
				if (Error != 0) {
					break;
				};
				Error = PCA9534_read_input_reg(I2Cx, PBM_1_PCA9534_ADDR, &data8);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_1.PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
				if (Data->PBM_Data_1.PCA9534_ON_Heat_1 == State) {
					Data->PBM_Data_1.Error_Heat_1 = SUCCESS;
				} else {
					Data->PBM_Data_1.Error_Heat_1 = 1;
				}
				Data->PBM_Data_1.Error_PCA9534 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_1.Error_PCA9534 = 1;
				Data->PBM_Data_1.Error_Heat_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				if (State == PBM_ON_HEAT) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, PBM_1_PCA9534_ADDR, PCA9534_IO_P05);
					if (Error != 0) {
						break;
					};
					Error = PCA9534_Set_output_pin(I2Cx, PBM_1_PCA9534_ADDR, PCA9534_IO_P05);
				} else {
					Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_1_PCA9534_ADDR, PCA9534_IO_P05);
				}
				if (Error != 0) {
					break;
				};
				Error = PCA9534_read_input_reg(I2Cx, PBM_1_PCA9534_ADDR, &data8);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_1.PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
				if (Data->PBM_Data_1.PCA9534_ON_Heat_2 == 1) {
					Data->PBM_Data_1.Error_Heat_2 = SUCCESS;
				} else {
					Data->PBM_Data_1.Error_Heat_2 = 1;
				}
				Data->PBM_Data_1.Error_PCA9534 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_1.Error_PCA9534 = 1;
				Data->PBM_Data_1.Error_Heat_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				if (State == PBM_ON_HEAT) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, PBM_2_PCA9534_ADDR, PCA9534_IO_P00);
					if (Error != 0) {
						break;
					};
					Error = PCA9534_Set_output_pin(I2Cx, PBM_2_PCA9534_ADDR, PCA9534_IO_P00);
				} else {
					Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_2_PCA9534_ADDR, PCA9534_IO_P00);
				}
				if (Error != 0) {
					break;
				};
				Error = PCA9534_read_input_reg(I2Cx, PBM_2_PCA9534_ADDR, &data8);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_2.PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
				if (Data->PBM_Data_2.PCA9534_ON_Heat_1 == State) {
					Data->PBM_Data_2.Error_Heat_1 = SUCCESS;
				} else {
					Data->PBM_Data_2.Error_Heat_1 = 1;
				}
				Data->PBM_Data_2.Error_PCA9534 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_2.Error_PCA9534 = 1;
				Data->PBM_Data_2.Error_Heat_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				if (State == PBM_ON_HEAT) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, PBM_2_PCA9534_ADDR, PCA9534_IO_P05);
					if (Error != 0) {
						break;
					};
					Error = PCA9534_Set_output_pin(I2Cx, PBM_2_PCA9534_ADDR, PCA9534_IO_P05);
				} else {
					Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_2_PCA9534_ADDR, PCA9534_IO_P05);
				}
				if (Error != 0) {
					break;
				};
				Error = PCA9534_read_input_reg(I2Cx, PBM_2_PCA9534_ADDR, &data8);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_2.PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
				if (Data->PBM_Data_2.PCA9534_ON_Heat_2 == 1) {
					Data->PBM_Data_2.Error_Heat_2 = SUCCESS;
				} else {
					Data->PBM_Data_2.Error_Heat_2 = 1;
				}
				Data->PBM_Data_2.Error_PCA9534 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_2.Error_PCA9534 = 1;
				Data->PBM_Data_2.Error_Heat_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				if (State == PBM_ON_HEAT) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, PBM_3_PCA9534_ADDR, PCA9534_IO_P00);
					if (Error != 0) {
						break;
					};
					Error = PCA9534_Set_output_pin(I2Cx, PBM_3_PCA9534_ADDR, PCA9534_IO_P00);
				} else {
					Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_3_PCA9534_ADDR, PCA9534_IO_P00);
				}
				if (Error != 0) {
					break;
				};
				Error = PCA9534_read_input_reg(I2Cx, PBM_3_PCA9534_ADDR, &data8);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_3.PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
				if (Data->PBM_Data_3.PCA9534_ON_Heat_1 == State) {
					Data->PBM_Data_3.Error_Heat_1 = SUCCESS;
				} else {
					Data->PBM_Data_3.Error_Heat_1 = 1;
				}
				Data->PBM_Data_3.Error_PCA9534 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_3.Error_PCA9534 = 1;
				Data->PBM_Data_3.Error_Heat_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				if (State == PBM_ON_HEAT) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, PBM_3_PCA9534_ADDR, PCA9534_IO_P05);
					if (Error != 0) {
						break;
					};
					Error = PCA9534_Set_output_pin(I2Cx, PBM_3_PCA9534_ADDR, PCA9534_IO_P05);
				} else {
					Error = PCA9534_conf_IO_dir_input(I2Cx, PBM_3_PCA9534_ADDR, PCA9534_IO_P05);
				}
				if (Error != 0) {
					break;
				};
				Error = PCA9534_read_input_reg(I2Cx, PBM_3_PCA9534_ADDR, &data8);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_3.PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
				if (Data->PBM_Data_3.PCA9534_ON_Heat_2 == 1) {
					Data->PBM_Data_3.Error_Heat_2 = SUCCESS;
				} else {
					Data->PBM_Data_3.Error_Heat_2 = 1;
				}
				Data->PBM_Data_3.Error_PCA9534 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_3.Error_PCA9534 = 1;
				Data->PBM_Data_3.Error_Heat_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
}

/** @brief	ON/OFF Charge for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state charge (PBM_ON_CHARGE or PBM_OFF_CHARGE).
 @retval 	none
 */
void PBM_SetStateChargeBranch(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	uint8_t count = 0;
	DS2777_StatusReg Struct;

	SW_TMUX1209_I2C_main_PBM();
	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableChg(I2Cx, PBM_1_DS2777_ADDR_1, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_1_DS2777_ADDR_1, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_1.Branch_1_ChgEnableBit = Struct.ChgEnableBit;
				if (Data->PBM_Data_1.Branch_1_ChgEnableBit == State) {
					Data->PBM_Data_1.Error_Charge_1 = SUCCESS;
				} else {
					Data->PBM_Data_1.Error_Charge_1 = 1;
				}
				Data->PBM_Data_1.Error_DS2777_1 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_1.Error_DS2777_1 = 1;
				Data->PBM_Data_1.Error_Charge_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableChg(I2Cx, PBM_1_DS2777_ADDR_2, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_1_DS2777_ADDR_2, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_1.Branch_2_ChgEnableBit = Struct.ChgEnableBit;
				if (Data->PBM_Data_1.Branch_2_ChgEnableBit == State) {
					Data->PBM_Data_1.Error_Charge_2 = SUCCESS;
				} else {
					Data->PBM_Data_1.Error_Charge_2 = 1;
				}
				Data->PBM_Data_1.Error_DS2777_2 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_1.Error_DS2777_2 = 1;
				Data->PBM_Data_1.Error_Charge_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableChg(I2Cx, PBM_2_DS2777_ADDR_1, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_2_DS2777_ADDR_1, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_2.Branch_1_ChgEnableBit = Struct.ChgEnableBit;
				if (Data->PBM_Data_2.Branch_1_ChgEnableBit == State) {
					Data->PBM_Data_2.Error_Charge_1 = SUCCESS;
				} else {
					Data->PBM_Data_2.Error_Charge_1 = 1;
				}
				Data->PBM_Data_2.Error_DS2777_1 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_2.Error_DS2777_1 = 1;
				Data->PBM_Data_2.Error_Charge_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableChg(I2Cx, PBM_2_DS2777_ADDR_2, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_2_DS2777_ADDR_2, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_2.Branch_2_ChgEnableBit = Struct.ChgEnableBit;
				if (Data->PBM_Data_2.Branch_2_ChgEnableBit == State) {
					Data->PBM_Data_2.Error_Charge_2 = SUCCESS;
				} else {
					Data->PBM_Data_2.Error_Charge_2 = 1;
				}
				Data->PBM_Data_2.Error_DS2777_2 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_2.Error_DS2777_2 = 1;
				Data->PBM_Data_2.Error_Charge_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableChg(I2Cx, PBM_3_DS2777_ADDR_1, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_3_DS2777_ADDR_1, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_3.Branch_1_ChgEnableBit = Struct.ChgEnableBit;
				if (Data->PBM_Data_3.Branch_1_ChgEnableBit == State) {
					Data->PBM_Data_3.Error_Charge_1 = SUCCESS;
				} else {
					Data->PBM_Data_3.Error_Charge_1 = 1;
				}
				Data->PBM_Data_3.Error_DS2777_1 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_3.Error_DS2777_1 = 1;
				Data->PBM_Data_3.Error_Charge_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableChg(I2Cx, PBM_3_DS2777_ADDR_2, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_3_DS2777_ADDR_2, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_3.Branch_2_ChgEnableBit = Struct.ChgEnableBit;
				if (Data->PBM_Data_3.Branch_2_ChgEnableBit == State) {
					Data->PBM_Data_3.Error_Charge_2 = SUCCESS;
				} else {
					Data->PBM_Data_3.Error_Charge_2 = 1;
				}
				Data->PBM_Data_3.Error_DS2777_2 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_3.Error_DS2777_2 = 1;
				Data->PBM_Data_3.Error_Charge_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
}

/** @brief	ON/OFF Disharge for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state charge (PBM_ON_DISCHARGE or PBM_OFF_DISCHARGE).
 @retval 	none
 */
void PBM_SetStateDischargeBranch(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	uint8_t count = 0;
	DS2777_StatusReg Struct;

	SW_TMUX1209_I2C_main_PBM();
	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableDchg(I2Cx, PBM_1_DS2777_ADDR_1, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_1_DS2777_ADDR_1, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_1.Branch_1_DchgEnableBit = Struct.DchgEnableBit;
				if (Data->PBM_Data_1.Branch_1_DchgEnableBit == State) {
					Data->PBM_Data_1.Error_Discharge_1 = SUCCESS;
				} else {
					Data->PBM_Data_1.Error_Discharge_1 = 1;
				}
				Data->PBM_Data_1.Error_DS2777_1 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_1.Error_DS2777_1 = 1;
				Data->PBM_Data_1.Error_Discharge_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableDchg(I2Cx, PBM_1_DS2777_ADDR_2, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_1_DS2777_ADDR_2, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_1.Branch_2_DchgEnableBit = Struct.DchgEnableBit;
				if (Data->PBM_Data_1.Branch_2_DchgEnableBit == State) {
					Data->PBM_Data_1.Error_Discharge_2 = SUCCESS;
				} else {
					Data->PBM_Data_1.Error_Discharge_2 = 1;
				}
				Data->PBM_Data_1.Error_DS2777_2 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_1.Error_DS2777_2 = 1;
				Data->PBM_Data_1.Error_Discharge_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableDchg(I2Cx, PBM_2_DS2777_ADDR_1, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_2_DS2777_ADDR_1, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_2.Branch_1_DchgEnableBit = Struct.DchgEnableBit;
				if (Data->PBM_Data_2.Branch_1_DchgEnableBit == State) {
					Data->PBM_Data_2.Error_Discharge_1 = SUCCESS;
				} else {
					Data->PBM_Data_2.Error_Discharge_1 = 1;
				}
				Data->PBM_Data_2.Error_DS2777_1 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_2.Error_DS2777_1 = 1;
				Data->PBM_Data_2.Error_Discharge_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableDchg(I2Cx, PBM_2_DS2777_ADDR_2, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_2_DS2777_ADDR_2, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_2.Branch_2_DchgEnableBit = Struct.DchgEnableBit;
				if (Data->PBM_Data_2.Branch_2_DchgEnableBit == State) {
					Data->PBM_Data_2.Error_Discharge_2 = SUCCESS;
				} else {
					Data->PBM_Data_2.Error_Discharge_2 = 1;
				}
				Data->PBM_Data_2.Error_DS2777_2 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_2.Error_DS2777_2 = 1;
				Data->PBM_Data_2.Error_Discharge_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableDchg(I2Cx, PBM_3_DS2777_ADDR_1, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_3_DS2777_ADDR_1, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_3.Branch_1_DchgEnableBit = Struct.DchgEnableBit;
				if (Data->PBM_Data_3.Branch_1_DchgEnableBit == State) {
					Data->PBM_Data_3.Error_Discharge_1 = SUCCESS;
				} else {
					Data->PBM_Data_3.Error_Discharge_1 = 1;
				}
				Data->PBM_Data_3.Error_DS2777_1 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_3.Error_DS2777_1 = 1;
				Data->PBM_Data_3.Error_Discharge_1 = 1;
			}
			Error = -1;
			count = 0;
		}
		if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
			while ((Error != 0) && (count < 3)) {
				Error = DS2777_EnableDchg(I2Cx, PBM_3_DS2777_ADDR_2, State);
				LL_mDelay(10);
				Error = Error + DS2777_ReadStatus(I2Cx, PBM_3_DS2777_ADDR_2, &Struct);
				count++;
			}
			if (Error == SUCCESS) {
				Data->PBM_Data_3.Branch_2_DchgEnableBit = Struct.DchgEnableBit;
				if (Data->PBM_Data_3.Branch_2_DchgEnableBit == State) {
					Data->PBM_Data_3.Error_Discharge_2 = SUCCESS;
				} else {
					Data->PBM_Data_3.Error_Discharge_2 = 1;
				}
				Data->PBM_Data_3.Error_DS2777_2 = SUCCESS;
			} else {
#ifdef DEBUGprintf
				Error_Handler();
#endif
				Data->PBM_Data_3.Error_DS2777_2 = 1;
				Data->PBM_Data_3.Error_Discharge_2 = 1;
			}
			Error = -1;
			count = 0;
		}
	}
}

/** @brief	Check auto heat OFF.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
void PBM_CheckHeatOFF(PBM_Data_All *Data) {

	uint8_t count = 0;
	uint8_t Hi_Limit = (uint8_t) (PBM_TMP1075_TEMP_HI + 2);

	if ((Data->PBM_Data_1.TMP1075_temp_1 >= Hi_Limit) && (Data->PBM_Data_1.PCA9534_AlarmState_1 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_1.TMP1075_temp_2 >= Hi_Limit) && (Data->PBM_Data_1.PCA9534_AlarmState_1 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_1.TMP1075_temp_3 >= Hi_Limit) && (Data->PBM_Data_1.PCA9534_AlarmState_2 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_1.TMP1075_temp_4 >= Hi_Limit) && (Data->PBM_Data_1.PCA9534_AlarmState_2 == 0)) {
		count++;
	}
	if (count >= 3) {
		PBM_SetStateHeatBranch(I2C4, Data, PBM_1, PBM_BRANCH_ALL, PBM_OFF_HEAT);
		Data->PBM_Data_1.Error_Heat_1 = 1;
		Data->PBM_Data_1.Error_Heat_2 = 1;
#ifdef DEBUGprintf
		Error_Handler();
#endif
	}
	count = 0;

	if ((Data->PBM_Data_2.TMP1075_temp_1 >= Hi_Limit) && (Data->PBM_Data_2.PCA9534_AlarmState_1 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_2.TMP1075_temp_2 >= Hi_Limit) && (Data->PBM_Data_2.PCA9534_AlarmState_1 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_2.TMP1075_temp_3 >= Hi_Limit) && (Data->PBM_Data_2.PCA9534_AlarmState_2 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_2.TMP1075_temp_4 >= Hi_Limit) && (Data->PBM_Data_2.PCA9534_AlarmState_2 == 0)) {
		count++;
	}
	if (count >= 3) {
		PBM_SetStateHeatBranch(I2C4, Data, PBM_2, PBM_BRANCH_ALL, PBM_OFF_HEAT);
		Data->PBM_Data_2.Error_Heat_1 = 1;
		Data->PBM_Data_2.Error_Heat_2 = 1;
#ifdef DEBUGprintf
		Error_Handler();
#endif
	}
	count = 0;

	if ((Data->PBM_Data_3.TMP1075_temp_1 >= Hi_Limit) && (Data->PBM_Data_3.PCA9534_AlarmState_1 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_3.TMP1075_temp_2 >= Hi_Limit) && (Data->PBM_Data_3.PCA9534_AlarmState_1 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_3.TMP1075_temp_3 >= Hi_Limit) && (Data->PBM_Data_3.PCA9534_AlarmState_2 == 0)) {
		count++;
	}
	if ((Data->PBM_Data_3.TMP1075_temp_4 >= Hi_Limit) && (Data->PBM_Data_3.PCA9534_AlarmState_2 == 0)) {
		count++;
	}
	if (count >= 3) {
		PBM_SetStateHeatBranch(I2C4, Data, PBM_3, PBM_BRANCH_ALL, PBM_OFF_HEAT);
		Data->PBM_Data_3.Error_Heat_1 = 1;
		Data->PBM_Data_3.Error_Heat_2 = 1;
#ifdef DEBUGprintf
		Error_Handler();
#endif
	}
	count = 0;

}

/** @brief	Check and correct capacity all PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
void PBM_CheckCapacityPBM(I2C_TypeDef *I2Cx, PBM_Data_All *Data) {

	int16_t AbcoluteCapacity = 0;
	float Capacity = 0, Voltage = 0;
	float Error = 0;

	SW_TMUX1209_I2C_main_PBM();
	if ((Data->PBM_Data_1.Branch_1_DchgEnableBit == 1) && (Data->PBM_Data_1.Branch_1_ChgEnableBit == 1) && (Data->PBM_Data_1.Error_DS2777_1 == 0)) {
		Capacity = ((float) (Data->PBM_Data_1.Branch_1_AbcoluteCapacity) / 3000.0 * 100.0); // in %
		Voltage = ((float) (Data->PBM_Data_1.Branch_1_VoltageHi + Data->PBM_Data_1.Branch_1_VoltageLo)) / 2.0;
		Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
		Error = (float) (Voltage / Capacity);
		Error = (float) (Error - 1.0) * 100.0;  // in %
		if ((Error > 25) || (Error < -25)) {
			AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
			DS2777_WriteAccmCharge(I2Cx, PBM_1_DS2777_ADDR_1, AbcoluteCapacity);
		}
	}
	if ((Data->PBM_Data_1.Branch_2_DchgEnableBit == 1) && (Data->PBM_Data_1.Branch_2_ChgEnableBit == 1) && (Data->PBM_Data_1.Error_DS2777_2 == 0)) {
		Capacity = ((float) (Data->PBM_Data_1.Branch_2_AbcoluteCapacity) / 3000.0 * 100.0); // in %
		Voltage = ((float) (Data->PBM_Data_1.Branch_2_VoltageHi + Data->PBM_Data_1.Branch_2_VoltageLo)) / 2.0;
		Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
		Error = (float) (Voltage / Capacity);
		Error = (float) (Error - 1.0) * 100.0;  // in %
		if ((Error > 25) || (Error < -25)) {
			AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
			DS2777_WriteAccmCharge(I2Cx, PBM_1_DS2777_ADDR_2, AbcoluteCapacity);
		}
	}

	if ((Data->PBM_Data_2.Branch_1_DchgEnableBit == 1) && (Data->PBM_Data_2.Branch_1_ChgEnableBit == 1) && (Data->PBM_Data_2.Error_DS2777_1 == 0)) {
		Capacity = ((float) (Data->PBM_Data_2.Branch_1_AbcoluteCapacity) / 3000.0 * 100.0); // in %
		Voltage = ((float) (Data->PBM_Data_2.Branch_1_VoltageHi + Data->PBM_Data_2.Branch_1_VoltageLo)) / 2.0;
		Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
		Error = (float) (Voltage / Capacity);
		Error = (float) (Error - 1.0) * 100.0;  // in %
		if ((Error > 25) || (Error < -25)) {
			AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
			DS2777_WriteAccmCharge(I2Cx, PBM_2_DS2777_ADDR_1, AbcoluteCapacity);
		}
	}
	if ((Data->PBM_Data_2.Branch_2_DchgEnableBit == 1) && (Data->PBM_Data_2.Branch_2_ChgEnableBit == 1) && (Data->PBM_Data_2.Error_DS2777_2 == 0)) {
		Capacity = ((float) (Data->PBM_Data_2.Branch_2_AbcoluteCapacity) / 3000.0 * 100.0); // in %
		Voltage = ((float) (Data->PBM_Data_2.Branch_2_VoltageHi + Data->PBM_Data_2.Branch_2_VoltageLo)) / 2.0;
		Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
		Error = (float) (Voltage / Capacity);
		Error = (float) (Error - 1.0) * 100.0;  // in %
		if ((Error > 25) || (Error < -25)) {
			AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
			DS2777_WriteAccmCharge(I2Cx, PBM_2_DS2777_ADDR_2, AbcoluteCapacity);
		}
	}

	if ((Data->PBM_Data_3.Branch_1_DchgEnableBit == 1) && (Data->PBM_Data_3.Branch_1_ChgEnableBit == 1) && (Data->PBM_Data_3.Error_DS2777_1 == 0)) {
		Capacity = ((float) (Data->PBM_Data_3.Branch_1_AbcoluteCapacity) / 3000.0 * 100.0); // in %
		Voltage = ((float) (Data->PBM_Data_3.Branch_1_VoltageHi + Data->PBM_Data_3.Branch_1_VoltageLo)) / 2.0;
		Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
		Error = (float) (Voltage / Capacity);
		Error = (float) (Error - 1.0) * 100.0;  // in %
		if ((Error > 25) || (Error < -25)) {
			AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
			DS2777_WriteAccmCharge(I2Cx, PBM_3_DS2777_ADDR_1, AbcoluteCapacity);
		}
	}
	if ((Data->PBM_Data_3.Branch_2_DchgEnableBit == 1) && (Data->PBM_Data_3.Branch_2_ChgEnableBit == 1) && (Data->PBM_Data_3.Error_DS2777_2 == 0)) {
		Capacity = ((float) (Data->PBM_Data_3.Branch_2_AbcoluteCapacity) / 3000.0 * 100.0); // in %
		Voltage = ((float) (Data->PBM_Data_3.Branch_2_VoltageHi + Data->PBM_Data_3.Branch_2_VoltageLo)) / 2.0;
		Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
		Error = (float) (Voltage / Capacity);
		Error = (float) (Error - 1.0) * 100.0;  // in %
		if ((Error > 25) || (Error < -25)) {
			AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
			DS2777_WriteAccmCharge(I2Cx, PBM_3_DS2777_ADDR_2, AbcoluteCapacity);
		}
	}
}

/** @brief	Calculate full capacity all PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
void PBM_CalcTotalCapacityPBM(PBM_Data_All *Data) {

	int16_t TotalAbsCapacity = 0;
	uint8_t TotalRelCapacity = 0;

	if ((Data->PBM_Data_1.Branch_1_DchgEnableBit == 1) && (Data->PBM_Data_1.Branch_1_ChgEnableBit == 1) && (Data->PBM_Data_1.Error_DS2777_1 == 0)) {
		TotalAbsCapacity = Data->PBM_Data_1.Branch_1_AbcoluteCapacity;
		TotalRelCapacity = Data->PBM_Data_1.Branch_1_RelativeCapacity;
	}
	if ((Data->PBM_Data_1.Branch_2_DchgEnableBit == 1) && (Data->PBM_Data_1.Branch_2_ChgEnableBit == 1) && (Data->PBM_Data_1.Error_DS2777_2 == 0)) {
		TotalAbsCapacity = TotalAbsCapacity + Data->PBM_Data_1.Branch_2_AbcoluteCapacity;
		TotalRelCapacity = TotalRelCapacity + Data->PBM_Data_1.Branch_2_RelativeCapacity;
	}
	Data->PBM_Data_1.TotalAbcoluteCapacity = TotalAbsCapacity / 2;
	Data->PBM_Data_1.TotalRelativeCapacity = TotalRelCapacity / 2;
	TotalAbsCapacity = 0;
	TotalRelCapacity = 0;

	if ((Data->PBM_Data_2.Branch_1_DchgEnableBit == 1) && (Data->PBM_Data_2.Branch_1_ChgEnableBit == 1) && (Data->PBM_Data_2.Error_DS2777_1 == 0)) {
		TotalAbsCapacity = Data->PBM_Data_2.Branch_1_AbcoluteCapacity;
		TotalRelCapacity = Data->PBM_Data_2.Branch_1_RelativeCapacity;
	}
	if ((Data->PBM_Data_2.Branch_2_DchgEnableBit == 1) && (Data->PBM_Data_2.Branch_2_ChgEnableBit == 1) && (Data->PBM_Data_2.Error_DS2777_2 == 0)) {
		TotalAbsCapacity = TotalAbsCapacity + Data->PBM_Data_2.Branch_2_AbcoluteCapacity;
		TotalRelCapacity = TotalRelCapacity + Data->PBM_Data_2.Branch_2_RelativeCapacity;
	}
	Data->PBM_Data_2.TotalAbcoluteCapacity = TotalAbsCapacity / 2;
	Data->PBM_Data_2.TotalRelativeCapacity = TotalRelCapacity / 2;
	TotalAbsCapacity = 0;
	TotalRelCapacity = 0;

	if ((Data->PBM_Data_3.Branch_1_DchgEnableBit == 1) && (Data->PBM_Data_3.Branch_1_ChgEnableBit == 1) && (Data->PBM_Data_3.Error_DS2777_1 == 0)) {
		TotalAbsCapacity = Data->PBM_Data_3.Branch_1_AbcoluteCapacity;
		TotalRelCapacity = Data->PBM_Data_3.Branch_1_RelativeCapacity;
	}
	if ((Data->PBM_Data_3.Branch_2_DchgEnableBit == 1) && (Data->PBM_Data_3.Branch_2_ChgEnableBit == 1) && (Data->PBM_Data_3.Error_DS2777_2 == 0)) {
		TotalAbsCapacity = TotalAbsCapacity + Data->PBM_Data_3.Branch_2_AbcoluteCapacity;
		TotalRelCapacity = TotalRelCapacity + Data->PBM_Data_3.Branch_2_RelativeCapacity;
	}
	Data->PBM_Data_3.TotalAbcoluteCapacity = TotalAbsCapacity / 2;
	Data->PBM_Data_3.TotalRelativeCapacity = TotalRelCapacity / 2;

}
