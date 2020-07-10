#include "main.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "DS2777.h"
#include "SetupPeriph.h"
#include "PBM_init.h"
#include "PBM_config.h"
#include "PBM_struct.h"
#include "PBM_control.h"
#include "Error_Handler.h"

/** @brief	Read state pin PCA9534 for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	none
 */
void PBM_ReadGPIO(I2C_TypeDef *I2Cx, _PBM *pbm, uint8_t PBM) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = -1;
	uint8_t addr = 0;

	if (PBM > PBM_QUANTITY - 1) {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
	}SW_TMUX1209_I2C_main_PBM();

	PBM_GPIO_Table(PBM, &addr);

	while ((Error != 0) && (count < 3)) {
		Error = PCA9534_read_input_reg(I2Cx, addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		if (pbm[PBM].PCA9534_ON_Heat_1 == (data8 & PCA9534_IO_P00)) {
			pbm[PBM].Error_Heat_1 = SUCCESS;
		} else {
			PBM_SetStateHeatBranch(I2C4, pbm, PBM, PBM_BRANCH_1, pbm[PBM].PCA9534_ON_Heat_1);
			pbm[PBM].Error_Heat_1 = 1;
		}
		if (pbm[PBM].PCA9534_ON_Heat_2 == ((data8 & PCA9534_IO_P05) >> 5)) {
			pbm[PBM].Error_Heat_2 = SUCCESS;
		} else {
			PBM_SetStateHeatBranch(I2C4, pbm, PBM, PBM_BRANCH_2, pbm[PBM].PCA9534_ON_Heat_2);
			pbm[PBM].Error_Heat_2 = 1;
		}

		pbm[PBM].PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
		pbm[PBM].PCA9534_P1 = (uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
		pbm[PBM].PCA9534_P2 = (uint8_t) ((data8 & PCA9534_IO_P02) >> 2);
		pbm[PBM].PCA9534_AlarmState_2 = !(uint8_t) ((data8 & PCA9534_IO_P03) >> 3);
		pbm[PBM].PCA9534_PIO_1 = (uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
		pbm[PBM].PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
		pbm[PBM].PCA9534_AlarmState_1 = !(uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
		pbm[PBM].PCA9534_PIO_2 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

		pbm[PBM].Error_PCA9534 = SUCCESS;

	}

	else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_PCA9534 = 1;
	}
}

/** @brief	Read temperature TMP1075 sensor for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	none
 */
void PBM_ReadTempSensor(I2C_TypeDef *I2Cx, _PBM *pbm, uint8_t PBM) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = -1;
	uint8_t addr = 0;

	SW_TMUX1209_I2C_main_PBM();

	PBM_Temp_Table(PBM, 1, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = TMP1075_read_int8_temperature(I2Cx, addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM].TMP1075_temp_1 = data8;
		pbm[PBM].Error_TMP1075_1 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_1 = 1;
	}
	Error = -1;
	count = 0;
	PBM_Temp_Table(PBM, 2, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = TMP1075_read_int8_temperature(I2Cx, addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM].TMP1075_temp_2 = data8;
		pbm[PBM].Error_TMP1075_2 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_2 = 1;
	}
	Error = -1;
	count = 0;
	PBM_Temp_Table(PBM, 3, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = TMP1075_read_int8_temperature(I2Cx, addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM].TMP1075_temp_3 = data8;
		pbm[PBM].Error_TMP1075_3 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_3 = 1;
	}
	Error = -1;
	count = 0;
	PBM_Temp_Table(PBM, 4, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = TMP1075_read_int8_temperature(I2Cx, addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM].TMP1075_temp_4 = data8;
		pbm[PBM].Error_TMP1075_4 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_4 = 1;
	}
}

/** @brief	Read data from two DS2777 for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	none
 */
void PBM_ReadBatteryData(I2C_TypeDef *I2Cx, _PBM *pbm, uint8_t PBM) {

	int8_t Error = -1;
	float Temp = 0;
	uint8_t count = 0;
	uint8_t addr = 0;
	DS2777_StatusReg Struct;

	SW_TMUX1209_I2C_main_PBM();

	PBM_DS2777_Table(PBM, PBM_BRANCH_1, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = DS2777_ReadStatus(I2Cx, addr, &Struct);
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};
		pbm[PBM].Branch_1_DchgEnableBit = Struct.DchgEnableBit;
		pbm[PBM].Branch_1_ChgEnableBit = Struct.ChgEnableBit;
		pbm[PBM].Branch_1_DchgControlFlag = Struct.DchgControlFlag;
		pbm[PBM].Branch_1_ChgControlFlag = Struct.ChgControlFlag;
		pbm[PBM].Branch_1_LrnFlag = Struct.LrnFlag;
		pbm[PBM].Branch_1_StbEmptyFlag = Struct.StbEmptyFlag;
		pbm[PBM].Branch_1_ActEmptyFlag = Struct.ActEmptyFlag;
		pbm[PBM].Branch_1_ChgTerminateFlag = Struct.ChgTerminateFlag;
		Error = -1;
		if (DS2777_ReadStbRelCapacity(I2Cx, addr, &pbm[PBM].Branch_1_RelativeCapacity) == SUCCESS) {
			if (DS2777_ReadAccmCharge(I2Cx, addr, &pbm[PBM].Branch_1_AbcoluteCapacity) == SUCCESS) {
				if (DS2777_ReadAgeScalar(I2Cx, addr, &pbm[PBM].Branch_1_AgeScalar) == SUCCESS) {
					if (DS2777_ReadCycleCounter(I2Cx, addr, &pbm[PBM].Branch_1_CycleCounter) == SUCCESS) {
						Error = DS2777_ReadTemperature(I2Cx, addr, &Temp);
						pbm[PBM].Branch_1_Temperature = (int8_t) Temp;
					}
				}
			}
		}
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};

		Error = -1;
		if (DS2777_ReadCurrent(I2Cx, addr, &pbm[PBM].Branch_1_Current) == SUCCESS) {
			if (DS2777_ReadAverageCurrent(I2Cx, addr, &pbm[PBM].Branch_1_AverageCurrent) == SUCCESS) {
				if (DS2777_ReadVoltageHi(I2Cx, addr, &pbm[PBM].Branch_1_VoltageHi) == SUCCESS) {
					Error = DS2777_ReadVoltageLo(I2Cx, addr, &pbm[PBM].Branch_1_VoltageLo);
				}
			}
		}
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};

	}
	if (Error == 0) {
		pbm[PBM].Error_DS2777_1 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_DS2777_1 = 1;
	}

	Error = -1;
	count = 0;
	PBM_DS2777_Table(PBM, PBM_BRANCH_2, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = DS2777_ReadStatus(I2Cx, addr, &Struct);
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};
		pbm[PBM].Branch_2_DchgEnableBit = Struct.DchgEnableBit;
		pbm[PBM].Branch_2_ChgEnableBit = Struct.ChgEnableBit;
		pbm[PBM].Branch_2_DchgControlFlag = Struct.DchgControlFlag;
		pbm[PBM].Branch_2_ChgControlFlag = Struct.ChgControlFlag;
		pbm[PBM].Branch_2_LrnFlag = Struct.LrnFlag;
		pbm[PBM].Branch_2_StbEmptyFlag = Struct.StbEmptyFlag;
		pbm[PBM].Branch_2_ActEmptyFlag = Struct.ActEmptyFlag;
		pbm[PBM].Branch_2_ChgTerminateFlag = Struct.ChgTerminateFlag;
		Error = -1;
		if (DS2777_ReadStbRelCapacity(I2Cx, addr, &pbm[PBM].Branch_2_RelativeCapacity) == SUCCESS) {
			if (DS2777_ReadAccmCharge(I2Cx, addr, &pbm[PBM].Branch_2_AbcoluteCapacity) == SUCCESS) {
				if (DS2777_ReadAgeScalar(I2Cx, addr, &pbm[PBM].Branch_2_AgeScalar) == SUCCESS) {
					if (DS2777_ReadCycleCounter(I2Cx, addr, &pbm[PBM].Branch_2_CycleCounter) == SUCCESS) {
						Error = DS2777_ReadTemperature(I2Cx, addr, &Temp);
						pbm[PBM].Branch_2_Temperature = (int8_t) Temp;
					}
				}
			}
		}
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};

		Error = -1;
		if (DS2777_ReadCurrent(I2Cx, addr, &pbm[PBM].Branch_2_Current) == SUCCESS) {
			if (DS2777_ReadAverageCurrent(I2Cx, addr, &pbm[PBM].Branch_2_AverageCurrent) == SUCCESS) {
				if (DS2777_ReadVoltageHi(I2Cx, addr, &pbm[PBM].Branch_2_VoltageHi) == SUCCESS) {
					Error = DS2777_ReadVoltageLo(I2Cx, addr, &pbm[PBM].Branch_2_VoltageLo);
				}
			}
		}
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};
	}
	if (Error == 0) {
		pbm[PBM].Error_DS2777_2 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_DS2777_2 = 1;
	}
}

/*void Error_Counter(PBM_Data_All Data, int16_t *Er_PBM_1, int16_t *Er_PBM_2, int16_t *Er_PBM_3) {
 *Er_PBM_1 = *Er_PBM_1 + Data.PBM_Data_1.Error_DS2777_1 + Data.PBM_Data_1.Error_DS2777_2 + Data.PBM_Data_1.Error_TMP1075_1 + Data.PBM_Data_1.Error_TMP1075_2
 + Data.PBM_Data_1.Error_TMP1075_3 + Data.PBM_Data_1.Error_TMP1075_4 + Data.PBM_Data_1.Error_PCA9534;
 *Er_PBM_2 = *Er_PBM_2 + Data.PBM_Data_2.Error_DS2777_1 + Data.PBM_Data_2.Error_DS2777_2 + Data.PBM_Data_2.Error_TMP1075_1 + Data.PBM_Data_2.Error_TMP1075_2
 + Data.PBM_Data_2.Error_TMP1075_3 + Data.PBM_Data_2.Error_TMP1075_4 + Data.PBM_Data_2.Error_PCA9534;
 *Er_PBM_3 = *Er_PBM_3 + Data.PBM_Data_3.Error_DS2777_1 + Data.PBM_Data_3.Error_DS2777_2 + Data.PBM_Data_3.Error_TMP1075_1 + Data.PBM_Data_3.Error_TMP1075_2
 + Data.PBM_Data_3.Error_TMP1075_3 + Data.PBM_Data_3.Error_TMP1075_4 + Data.PBM_Data_3.Error_PCA9534;

 }*/

/** @brief	ON/OFF heat for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state heat (PBM_ON_HEAT or PBM_OFF_HEAT).
 @retval 	none
 */
void PBM_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM *pbm, uint8_t PBM, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	uint8_t data8 = 0;
	uint8_t addr = 0;
	uint8_t count = 0;
	uint8_t InputPin = PCA9534_IO_P01 | PCA9534_IO_P02 | PCA9534_IO_P03 | PCA9534_IO_P04 | PCA9534_IO_P06 | PCA9534_IO_P07;

	SW_TMUX1209_I2C_main_PBM();
	PBM_GPIO_Table(PBM, &addr);
	while ((Error != 0) && (count < 3)) {
		Error = PCA9534_conf_IO_dir_input(I2Cx, addr, InputPin);
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};
	}
	Error = -1;
	if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < 3)) {
			if (State == PBM_ON_HEAT) {
				Error = PCA9534_conf_IO_dir_output(I2Cx, addr, PCA9534_IO_P00);
				if (Error != 0) {
					LL_mDelay(PBM_i2c_delay_att_conn);
					count++;
					continue;
				};
				Error = PCA9534_Set_output_pin(I2Cx, addr, PCA9534_IO_P00);
			} else {
				Error = PCA9534_conf_IO_dir_input(I2Cx, addr, PCA9534_IO_P00);
			}
			if (Error != 0) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
				continue;
			};
			Error = PCA9534_read_input_reg(I2Cx, addr, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM].PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
			if (pbm[PBM].PCA9534_ON_Heat_1 == State) {
				pbm[PBM].Error_Heat_1 = SUCCESS;
			} else {
				pbm[PBM].Error_Heat_1 = 1;
			}
			pbm[PBM].Error_PCA9534 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			pbm[PBM].Error_PCA9534 = 1;
			pbm[PBM].Error_Heat_1 = 1;
		}
		Error = -1;
		count = 0;
	}
	if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < 3)) {
			if (State == PBM_ON_HEAT) {
				Error = PCA9534_conf_IO_dir_output(I2Cx, addr, PCA9534_IO_P05);
				if (Error != 0) {
					LL_mDelay(PBM_i2c_delay_att_conn);
					count++;
					continue;
				};
				Error = PCA9534_Set_output_pin(I2Cx, addr, PCA9534_IO_P05);
			} else {
				Error = PCA9534_conf_IO_dir_input(I2Cx, addr, PCA9534_IO_P05);
			}
			if (Error != 0) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
				continue;
			};
			Error = PCA9534_read_input_reg(I2Cx, addr, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM].PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
			if (pbm[PBM].PCA9534_ON_Heat_2 == State) {
				pbm[PBM].Error_Heat_2 = SUCCESS;
			} else {
				pbm[PBM].Error_Heat_2 = 1;
			}
			pbm[PBM].Error_PCA9534 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			pbm[PBM].Error_PCA9534 = 1;
			pbm[PBM].Error_Heat_2 = 1;
		}
	}
}

/** @brief	ON/OFF Charge for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state charge (PBM_ON_CHARGE or PBM_OFF_CHARGE).
 @retval 	none
 */
void PBM_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM *pbm, uint8_t PBM, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	uint8_t count = 0;
	uint8_t addr = 0;
	DS2777_StatusReg Struct;

	SW_TMUX1209_I2C_main_PBM();

	PBM_DS2777_Table(PBM, PBM_BRANCH_1, &addr);
	if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_EnableChg(I2Cx, addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, addr, &Struct);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM].Branch_1_ChgEnableBit = Struct.ChgEnableBit;
			if (pbm[PBM].Branch_1_ChgEnableBit == State) {
				pbm[PBM].Error_Charge_1 = SUCCESS;
			} else {
				pbm[PBM].Error_Charge_1 = 1;
			}
			pbm[PBM].Error_DS2777_1 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			pbm[PBM].Error_DS2777_1 = 1;
			pbm[PBM].Error_Charge_1 = 1;
		}
		Error = -1;
		count = 0;
	}
	PBM_DS2777_Table(PBM, PBM_BRANCH_2, &addr);
	if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_EnableChg(I2Cx, addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, addr, &Struct);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM].Branch_2_ChgEnableBit = Struct.ChgEnableBit;
			if (pbm[PBM].Branch_2_ChgEnableBit == State) {
				pbm[PBM].Error_Charge_2 = SUCCESS;
			} else {
				pbm[PBM].Error_Charge_2 = 1;
			}
			pbm[PBM].Error_DS2777_2 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			pbm[PBM].Error_DS2777_2 = 1;
			pbm[PBM].Error_Charge_2 = 1;
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
void PBM_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM *pbm, uint8_t PBM, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	uint8_t count = 0;
	uint8_t addr = 0;
	DS2777_StatusReg Struct;

	SW_TMUX1209_I2C_main_PBM();

	PBM_DS2777_Table(PBM, PBM_BRANCH_1, &addr);
	if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_EnableDchg(I2Cx, addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, addr, &Struct);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM].Branch_1_DchgEnableBit = Struct.DchgEnableBit;
			if (pbm[PBM].Branch_1_DchgEnableBit == State) {
				pbm[PBM].Error_Discharge_1 = SUCCESS;
			} else {
				pbm[PBM].Error_Discharge_1 = 1;
			}
			pbm[PBM].Error_DS2777_1 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			pbm[PBM].Error_DS2777_1 = 1;
			pbm[PBM].Error_Discharge_1 = 1;
		}
		Error = -1;
		count = 0;
	}
	PBM_DS2777_Table(PBM, PBM_BRANCH_2, &addr);
	if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < 3)) {
			Error = DS2777_EnableDchg(I2Cx, addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, addr, &Struct);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM].Branch_2_DchgEnableBit = Struct.DchgEnableBit;
			if (pbm[PBM].Branch_2_DchgEnableBit == State) {
				pbm[PBM].Error_Discharge_2 = SUCCESS;
			} else {
				pbm[PBM].Error_Discharge_2 = 1;
			}
			pbm[PBM].Error_DS2777_2 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			pbm[PBM].Error_DS2777_2 = 1;
			pbm[PBM].Error_Discharge_2 = 1;
		}
	}
}

/** @brief	Check auto heat OFF.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
void PBM_CheckHeatOFF(_PBM *pbm) {

	uint8_t count = 0;
	uint8_t Hi_Limit = (uint8_t) (PBM_TMP1075_TEMP_HI + 2);
	uint8_t i = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		if ((pbm[i].TMP1075_temp_1 >= Hi_Limit) && (pbm[i].PCA9534_AlarmState_1 == 1)) {
			count++;
		}
		if ((pbm[i].TMP1075_temp_2 >= Hi_Limit) && (pbm[i].PCA9534_AlarmState_1 == 1)) {
			count++;
		}
		if ((pbm[i].TMP1075_temp_3 >= Hi_Limit) && (pbm[i].PCA9534_AlarmState_2 == 1)) {
			count++;
		}
		if ((pbm[i].TMP1075_temp_4 >= Hi_Limit) && (pbm[i].PCA9534_AlarmState_2 == 1)) {
			count++;
		}
		if (count >= 3) {
			PBM_SetStateHeatBranch(I2C4, pbm, i, PBM_BRANCH_ALL, PBM_OFF_HEAT);
			pbm[i].Error_Heat_1 = 1;
			pbm[i].Error_Heat_2 = 1;
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
		}
	}
}

/** @brief	Check and correct capacity all PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
void PBM_CheckCapacityPBM(I2C_TypeDef *I2Cx, _PBM *pbm) {

	int16_t AbcoluteCapacity = 0;
	float Capacity = 0, Voltage = 0;
	float Error = 0;
	uint8_t addr = 0;
	uint8_t i = 0;

	SW_TMUX1209_I2C_main_PBM();

	for (i = 0; i < PBM_QUANTITY; i++) {
		PBM_DS2777_Table(i, PBM_BRANCH_1, &addr);
		if ((pbm[i].Branch_1_DchgEnableBit == 1) && (pbm[i].Branch_1_ChgEnableBit == 1) && (pbm[i].Error_DS2777_1 == 0)) {
			Capacity = ((float) (pbm[i].Branch_1_AbcoluteCapacity) / 3000.0 * 100.0); // in %
			Voltage = ((float) (pbm[i].Branch_1_VoltageHi + pbm[i].Branch_1_VoltageLo)) / 2.0;
			Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0) * 100.0;  // in %
			if ((Error > 25) || (Error < -25)) {
				AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
				DS2777_WriteAccmCharge(I2Cx, addr, AbcoluteCapacity);
			}
		}
		PBM_DS2777_Table(i, PBM_BRANCH_2, &addr);
		if ((pbm[i].Branch_2_DchgEnableBit == 1) && (pbm[i].Branch_2_ChgEnableBit == 1) && (pbm[i].Error_DS2777_2 == 0)) {
			Capacity = ((float) (pbm[i].Branch_2_AbcoluteCapacity) / 3000.0 * 100.0); // in %
			Voltage = ((float) (pbm[i].Branch_2_VoltageHi + pbm[i].Branch_2_VoltageLo)) / 2.0;
			Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0) * 100.0;  // in %
			if ((Error > 25) || (Error < -25)) {
				AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
				DS2777_WriteAccmCharge(I2Cx, addr, AbcoluteCapacity);
			}
		}
	}
}

/** @brief	Calculate full capacity all PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
void PBM_CalcTotalCapacityPBM(_PBM *pbm) {

	int16_t TotalAbsCapacity = 0;
	uint8_t TotalRelCapacity = 0;
	uint8_t i = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		if ((pbm[i].Branch_1_DchgEnableBit == 1) && (pbm[i].Error_DS2777_1 == 0)) {
			TotalAbsCapacity = pbm[i].Branch_1_AbcoluteCapacity;
			TotalRelCapacity = pbm[i].Branch_1_RelativeCapacity;
		}
		if ((pbm[i].Branch_2_DchgEnableBit == 1) && (pbm[i].Error_DS2777_2 == 0)) {
			TotalAbsCapacity = TotalAbsCapacity + pbm[i].Branch_2_AbcoluteCapacity;
			TotalRelCapacity = TotalRelCapacity + pbm[i].Branch_2_RelativeCapacity;
		}
		pbm[i].TotalRelativeCapacity = TotalRelCapacity / 2;
		pbm[i].TotalAbcoluteCapacity = TotalAbsCapacity;
		TotalAbsCapacity = 0;
		TotalRelCapacity = 0;
	}
}
