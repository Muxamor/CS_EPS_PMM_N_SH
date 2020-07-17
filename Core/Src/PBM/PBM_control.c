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
 @param 	pbm[] - structure data for all PBM modules.
 @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	ErrorStatus
 */
ErrorStatus PBM_ReadGPIO(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = -1;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_Table(PBM_number);

	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = PCA9534_read_input_reg(I2Cx, pbm_table.GPIO_Addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		if (pbm[PBM_number].PCA9534_ON_Heat_1 == (data8 & PCA9534_IO_P00)) {
			pbm[PBM_number].Error_Heat_1 = SUCCESS;
		} else {
			PBM_SetStateHeatBranch(I2Cx, pbm, PBM_number, PBM_BRANCH_1, pbm[PBM_number].PCA9534_ON_Heat_1);
			pbm[PBM_number].Error_Heat_1 = ERROR;
		}
		if (pbm[PBM_number].PCA9534_ON_Heat_2 == ((data8 & PCA9534_IO_P05) >> 5)) {
			pbm[PBM_number].Error_Heat_2 = SUCCESS;
		} else {
			PBM_SetStateHeatBranch(I2Cx, pbm, PBM_number, PBM_BRANCH_2, pbm[PBM_number].PCA9534_ON_Heat_2);
			pbm[PBM_number].Error_Heat_2 = ERROR;
		}

		//pbm[PBM_number].PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
		pbm[PBM_number].PCA9534_P1 = (uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
		pbm[PBM_number].PCA9534_P2 = (uint8_t) ((data8 & PCA9534_IO_P02) >> 2);
		pbm[PBM_number].PCA9534_TempSens_State_2 = !(uint8_t) ((data8 & PCA9534_IO_P03) >> 3);
		pbm[PBM_number].PCA9534_PIO_1 = (uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
		//pbm[PBM_number].PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
		pbm[PBM_number].PCA9534_TempSens_State_1 = !(uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
		pbm[PBM_number].PCA9534_PIO_2 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;
	}

	else {
		pbm[PBM_number].Error_PCA9534 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}
}

/** @brief	Read temperature all TMP1075 sensor for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	pbm[] - structure data for all PBM modules.
 @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	ErrorStatus
 */
ErrorStatus PBM_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = -1;
	int8_t Error_count = 0;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_Table(PBM_number);

	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_1_Addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM_number].TMP1075_temp_1 = data8;
		pbm[PBM_number].Error_TMP1075_1 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].TMP1075_temp_1 = 0x7F;
		pbm[PBM_number].Error_TMP1075_1 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	Error = -1;
	count = 0;
	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_2_Addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM_number].TMP1075_temp_2 = data8;
		pbm[PBM_number].Error_TMP1075_2 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].TMP1075_temp_2 = 0x7F;
		pbm[PBM_number].Error_TMP1075_2 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	Error = -1;
	count = 0;
	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_3_Addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM_number].TMP1075_temp_3 = data8;
		pbm[PBM_number].Error_TMP1075_3 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].TMP1075_temp_3 = 0x7F;
		pbm[PBM_number].Error_TMP1075_3 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	Error = -1;
	count = 0;
	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_4_Addr, &data8);
		count++;
	}
	if (Error == SUCCESS) {
		pbm[PBM_number].TMP1075_temp_4 = data8;
		pbm[PBM_number].Error_TMP1075_4 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].TMP1075_temp_4 = 0x7F;
		pbm[PBM_number].Error_TMP1075_4 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}
	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/** @brief	Read data from two DS2777 for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	pbm[] - structure data for all PBM modules.
 @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	ErrorStatus
 */
ErrorStatus PBM_ReadBatteryData(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number) {

	int8_t Error = -1;
	int8_t Error_count = 0;
	float Temp = 0;
	uint8_t count = 0;
	DS2777_StatusReg Struct;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_Table(PBM_number);

	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = DS2777_ReadStatus(I2Cx, pbm_table.BRANCH_1_Addr, &Struct);
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		}
		//pbm[PBM_number].Branch_1_DchgEnableBit = Struct.DchgEnableBit;
		//pbm[PBM_number].Branch_1_ChgEnableBit = Struct.ChgEnableBit;
		pbm[PBM_number].Branch_1_DchgControlFlag = Struct.DchgControlFlag;
		pbm[PBM_number].Branch_1_ChgControlFlag = Struct.ChgControlFlag;
		pbm[PBM_number].Branch_1_LrnFlag = Struct.LrnFlag;
		pbm[PBM_number].Branch_1_StbEmptyFlag = Struct.StbEmptyFlag;
		pbm[PBM_number].Branch_1_ActEmptyFlag = Struct.ActEmptyFlag;
		pbm[PBM_number].Branch_1_ChgTerminateFlag = Struct.ChgTerminateFlag;
		Error = -1;
		if (DS2777_ReadStbRelCapacity(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_RelativeCapacity) == SUCCESS) {
			if (DS2777_ReadAccmCharge(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_AbcoluteCapacity) == SUCCESS) {
				if (DS2777_ReadAgeScalar(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_AgeScalar) == SUCCESS) {
					if (DS2777_ReadCurrent(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_Current) == SUCCESS) {
						if (DS2777_ReadAverageCurrent(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_AverageCurrent) == SUCCESS) {
							if (DS2777_ReadVoltageHi(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_VoltageHi) == SUCCESS) {
								if (DS2777_ReadVoltageLo(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_VoltageLo) == SUCCESS) {
									if (DS2777_ReadCycleCounter(I2Cx, pbm_table.BRANCH_1_Addr, &pbm[PBM_number].Branch_1_CycleCounter) == SUCCESS) {
										Error = DS2777_ReadTemperature(I2Cx, pbm_table.BRANCH_1_Addr, &Temp);
										if(Error == SUCCESS) {
											pbm[PBM_number].Branch_1_Temperature = (int8_t) Temp;
										} else {
											pbm[PBM_number].Branch_1_Temperature = 0x7F;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (Error != SUCCESS) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		}
	}

	if (Error == SUCCESS) {
		pbm[PBM_number].Error_DS2777_1 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Branch_1_Temperature = 0x7F;
		pbm[PBM_number].Error_DS2777_1 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	Error = -1;
	count = 0;
	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = DS2777_ReadStatus(I2Cx, pbm_table.BRANCH_2_Addr, &Struct);
		if (Error != SUCCESS) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		}
		//pbm[PBM_number].Branch_2_DchgEnableBit = Struct.DchgEnableBit;
		//pbm[PBM_number].Branch_2_ChgEnableBit = Struct.ChgEnableBit;
		pbm[PBM_number].Branch_2_DchgControlFlag = Struct.DchgControlFlag;
		pbm[PBM_number].Branch_2_ChgControlFlag = Struct.ChgControlFlag;
		pbm[PBM_number].Branch_2_LrnFlag = Struct.LrnFlag;
		pbm[PBM_number].Branch_2_StbEmptyFlag = Struct.StbEmptyFlag;
		pbm[PBM_number].Branch_2_ActEmptyFlag = Struct.ActEmptyFlag;
		pbm[PBM_number].Branch_2_ChgTerminateFlag = Struct.ChgTerminateFlag;
		Error = -1;
		if (DS2777_ReadStbRelCapacity(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_RelativeCapacity) == SUCCESS) {
			if (DS2777_ReadAccmCharge(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_AbcoluteCapacity) == SUCCESS) {
				if (DS2777_ReadAgeScalar(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_AgeScalar) == SUCCESS) {
					if (DS2777_ReadCurrent(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_Current) == SUCCESS) {
						if (DS2777_ReadAverageCurrent(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_AverageCurrent) == SUCCESS) {
							if (DS2777_ReadVoltageHi(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_VoltageHi) == SUCCESS) {
								if (DS2777_ReadVoltageLo(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_VoltageLo) == SUCCESS) {
									if (DS2777_ReadCycleCounter(I2Cx, pbm_table.BRANCH_2_Addr, &pbm[PBM_number].Branch_2_CycleCounter) == SUCCESS) {
										Error = DS2777_ReadTemperature(I2Cx, pbm_table.BRANCH_2_Addr, &Temp);
										if(Error == SUCCESS) {
											pbm[PBM_number].Branch_2_Temperature = (int8_t) Temp;
										} else {
											pbm[PBM_number].Branch_2_Temperature = 0x7F;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if (Error != SUCCESS) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		}
	}
	if (Error == SUCCESS) {
		pbm[PBM_number].Error_DS2777_2 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Branch_2_Temperature = 0x7F;
		pbm[PBM_number].Error_DS2777_2 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
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
 @param 	pbm[] - structure data for all PBM modules.
 @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state heat (PBM_ON_HEAT or PBM_OFF_HEAT).
 @retval 	ErrorStatus
 */
ErrorStatus PBM_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	int8_t Error_count = 0;
	uint8_t data8 = 0;
	uint8_t count = 0;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_Table(PBM_number);

	while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
		Error = PCA9534_conf_IO_dir_input(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_INPUT_PIN);
		if (Error != 0) {
			LL_mDelay(PBM_i2c_delay_att_conn);
			count++;
			continue;
		};
	}

	Error = -1;
	if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
			if (State == PBM_ON_HEAT) {
				if (PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00) == SUCCESS) {
					Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00);
				}
			} else {
				Error = PCA9534_conf_IO_dir_input(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00);
			}
			if (Error != SUCCESS) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
				continue;
			}
			Error = PCA9534_read_input_reg(I2Cx, pbm_table.GPIO_Addr, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM_number].PCA9534_ON_Heat_1 = (uint8_t) (data8 & PCA9534_IO_P00);
			if (pbm[PBM_number].PCA9534_ON_Heat_1 == State) {
				pbm[PBM_number].Error_Heat_1 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_Heat_1 = ERROR;
			}
			pbm[PBM_number].Error_PCA9534 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_PCA9534 = ERROR;
			pbm[PBM_number].Error_Heat_1 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	Error = -1;
	count = 0;
	if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
			if (State == PBM_ON_HEAT) {
				if (PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05) == SUCCESS) {
					Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05);
				}
			} else {
				Error = PCA9534_conf_IO_dir_input(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05);
			}
			if (Error != 0) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
				continue;
			};
			Error = PCA9534_read_input_reg(I2Cx, pbm_table.GPIO_Addr, &data8);
			count++;
		}
		if (Error == SUCCESS) {
			pbm[PBM_number].PCA9534_ON_Heat_2 = (uint8_t) ((data8 & PCA9534_IO_P05) >> 5);
			if (pbm[PBM_number].PCA9534_ON_Heat_2 == State) {
				pbm[PBM_number].Error_Heat_2 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_Heat_2 = ERROR;
			}
			pbm[PBM_number].Error_PCA9534 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_PCA9534 = ERROR;
			pbm[PBM_number].Error_Heat_2 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif

		}
	}
	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/** @brief	ON/OFF Charge for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	pbm[] - structure data for all PBM modules.
 @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state charge (PBM_ON_CHARGE or PBM_OFF_CHARGE).
 @retval 	ErrorStatus
 */
ErrorStatus PBM_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	int8_t Error_count = 0;
	uint8_t count = 0;
	DS2777_StatusReg Struct;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_Table(PBM_number);

	if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != SUCCESS) && (count < PBM_I2C_ATTEMPT_CONN)) {
			Error = DS2777_EnableChg(I2Cx, pbm_table.BRANCH_1_Addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, pbm_table.BRANCH_1_Addr, &Struct);
			if (Error != SUCCESS) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
			}
		}
		if (Error == SUCCESS) {
			pbm[PBM_number].Branch_1_ChgEnableBit = Struct.ChgEnableBit;
			if (pbm[PBM_number].Branch_1_ChgControlFlag == State) {
				pbm[PBM_number].Error_Charge_1 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_Charge_1 = ERROR;
			}
			pbm[PBM_number].Error_DS2777_1 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_DS2777_1 = ERROR;
			pbm[PBM_number].Error_Charge_1 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	Error = -1;
	count = 0;
	if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != SUCCESS) && (count < PBM_I2C_ATTEMPT_CONN)) {
			Error = DS2777_EnableChg(I2Cx, pbm_table.BRANCH_2_Addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, pbm_table.BRANCH_2_Addr, &Struct);
			if (Error != SUCCESS) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
			}
		}
		if (Error == SUCCESS) {
			pbm[PBM_number].Branch_2_ChgEnableBit = Struct.ChgEnableBit;
			if (pbm[PBM_number].Branch_2_ChgControlFlag == State) {
				pbm[PBM_number].Error_Charge_2 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_Charge_2 = ERROR;
			}
			pbm[PBM_number].Error_DS2777_2 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_DS2777_2 = ERROR;
			pbm[PBM_number].Error_Charge_2 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}
	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/** @brief	ON/OFF Disharge for selected branch for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	pbm[] - structure data for all PBM modules.
 @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
 @param 	State - select state charge (PBM_ON_DISCHARGE or PBM_OFF_DISCHARGE).
 @retval 	ErrorStatus
 */
ErrorStatus PBM_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = -1;
	int8_t Error_count = 0;
	uint8_t count = 0;
	DS2777_StatusReg Struct;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_Table(PBM_number);

	if ((Branch == PBM_BRANCH_1) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
			Error = DS2777_EnableDchg(I2Cx, pbm_table.BRANCH_1_Addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, pbm_table.BRANCH_1_Addr, &Struct);
			if (Error != SUCCESS) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
			}
		}
		if (Error == SUCCESS) {
			pbm[PBM_number].Branch_1_DchgEnableBit = Struct.DchgEnableBit;
			if (pbm[PBM_number].Branch_1_DchgControlFlag == State) {
				pbm[PBM_number].Error_Discharge_1 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_Discharge_1 = ERROR;
			}
			pbm[PBM_number].Error_DS2777_1 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_DS2777_1 = ERROR;
			pbm[PBM_number].Error_Discharge_1 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	Error = -1;
	count = 0;
	if ((Branch == PBM_BRANCH_2) | (Branch == PBM_BRANCH_ALL)) {
		while ((Error != 0) && (count < PBM_I2C_ATTEMPT_CONN)) {
			Error = DS2777_EnableDchg(I2Cx, pbm_table.BRANCH_2_Addr, State);
			LL_mDelay(10);
			Error = Error + DS2777_ReadStatus(I2Cx, pbm_table.BRANCH_2_Addr, &Struct);
			if (Error != SUCCESS) {
				LL_mDelay(PBM_i2c_delay_att_conn);
				count++;
			}
		}
		if (Error == SUCCESS) {
			pbm[PBM_number].Branch_2_DchgEnableBit = Struct.DchgEnableBit;
			if (pbm[PBM_number].Branch_2_DchgControlFlag == State) {
				pbm[PBM_number].Error_Discharge_2 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_Discharge_2 = ERROR;
			}
			pbm[PBM_number].Error_DS2777_2 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_DS2777_2 = ERROR;
			pbm[PBM_number].Error_Discharge_2 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}
	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/** @brief	Check auto heat OFF.
 @param 	pbm[] - structure data for all PBM modules.
 @retval 	ErrorStatus
 */
ErrorStatus PBM_CheckHeatOFF(_PBM pbm[]) {

	uint8_t count = 0;
	uint8_t Hi_Limit = (uint8_t) (PBM_TMP1075_TEMP_HI + 2);
	uint8_t i = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		if ((pbm[i].TMP1075_temp_1 >= Hi_Limit) && (pbm[i].PCA9534_TempSens_State_1 == 1)) {
			count++;
		}
		if ((pbm[i].TMP1075_temp_2 >= Hi_Limit) && (pbm[i].PCA9534_TempSens_State_1 == 1)) {
			count++;
		}
		if ((pbm[i].TMP1075_temp_3 >= Hi_Limit) && (pbm[i].PCA9534_TempSens_State_2 == 1)) {
			count++;
		}
		if ((pbm[i].TMP1075_temp_4 >= Hi_Limit) && (pbm[i].PCA9534_TempSens_State_2 == 1)) {
			count++;
		}
		if (count >= 2) {
			PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_ALL, PBM_OFF_HEAT);
			PBM_TempSensorInit(pbm, i);
			pbm[i].PCA9534_ON_Heat_1 = 1;
			pbm[i].PCA9534_ON_Heat_2 = 1;
			pbm[i].Error_Heat_1 = ERROR;
			pbm[i].Error_Heat_2 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			return ERROR_N;
		}
		count = 0;
	}
	return SUCCESS;
}

/** @brief	Check state charge/discharge keys for all PBM.
 @param 	pbm[] - structure data for all PBM modules.
 @retval 	ErrorStatus
 */
ErrorStatus PBM_CheckChargeDischargeState(_PBM pbm[]) {

	uint8_t i = 0;
	int8_t Error_count = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {

		if((pbm[i].Branch_1_ChgEnableBit == pbm[i].Branch_1_ChgControlFlag) && (pbm[i].Error_DS2777_1 == SUCCESS)){
			pbm[i].Error_Charge_1 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[i].Error_Charge_1 = ERROR;
		}
		if((pbm[i].Branch_2_ChgEnableBit == pbm[i].Branch_2_ChgControlFlag) && (pbm[i].Error_DS2777_2 == SUCCESS)){
			pbm[i].Error_Charge_2 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[i].Error_Charge_2 = ERROR;
		}
		if((pbm[i].Branch_1_DchgEnableBit == pbm[i].Branch_1_DchgControlFlag) && (pbm[i].Error_DS2777_1 == SUCCESS)){
			pbm[i].Error_Discharge_1 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[i].Error_Discharge_1 = ERROR;
		}
		if((pbm[i].Branch_2_DchgEnableBit == pbm[i].Branch_2_DchgControlFlag) && (pbm[i].Error_DS2777_2 == SUCCESS)){
			pbm[i].Error_Discharge_2 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[i].Error_Discharge_2 = ERROR;
		}
	}
	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/** @brief	Check and correct capacity all PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	pbm[] - structure data for all PBM modules.
 @retval 	ErrorStatus
 */
ErrorStatus PBM_CheckCapacityPBM(I2C_TypeDef *I2Cx, _PBM pbm[]) {

	int16_t AbcoluteCapacity = 0;
	float Capacity = 0, Voltage = 0;
	float Error = 0;
	int8_t Error_count = 0;
	uint8_t i = 0;
	_PBM_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	for (i = 0; i < PBM_QUANTITY; i++) {
		pbm_table = PBM_Table(i);
		if (pbm[i].Error_DS2777_1 == 0) {
			Capacity = ((float) (pbm[i].Branch_1_AbcoluteCapacity) / 3000.0 * 100.0); // in %
			Voltage = ((float) (pbm[i].Branch_1_VoltageHi + pbm[i].Branch_1_VoltageLo)) / 2.0;
			Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0) * 100.0;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 100)) {
				AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
				DS2777_WriteAccmCharge(I2Cx, pbm_table.BRANCH_1_Addr, AbcoluteCapacity);
			}
		} else {
			Error_count = Error_count + 1;
		}

		if (pbm[i].Error_DS2777_2 == 0) {
			Capacity = ((float) (pbm[i].Branch_2_AbcoluteCapacity) / 3000.0 * 100.0); // in %
			Voltage = ((float) (pbm[i].Branch_2_VoltageHi + pbm[i].Branch_2_VoltageLo)) / 2.0;
			Voltage = (float) (Voltage - 2500.0) * 100.0 / 1650.0; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0) * 100.0;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 100)) {
				AbcoluteCapacity = (int16_t) ((Voltage * 3000) / 100);
				DS2777_WriteAccmCharge(I2Cx, pbm_table.BRANCH_2_Addr, AbcoluteCapacity);
			}
		} else {
				Error_count = Error_count + 1;
		}
	}
	if (Error_count == SUCCESS) {
		return SUCCESS;
	} else {
		return ERROR_N;
	}
}

/** @brief	Calculate full capacity all PBM.
 @param 	pbm[] - structure data for all PBM modules.
 @retval 	ErrorStatus
 */
ErrorStatus PBM_CalcTotalCapacityPBM(_PBM pbm[]) {

	int16_t TotalAbsCapacity = 0;
	uint8_t TotalRelCapacity = 0;
	uint8_t i = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		if ((pbm[i].Branch_1_DchgControlFlag == 1) && (pbm[i].Error_DS2777_1 == 0)) {
			TotalAbsCapacity = pbm[i].Branch_1_AbcoluteCapacity;
			TotalRelCapacity = pbm[i].Branch_1_RelativeCapacity;
		}
		if ((pbm[i].Branch_2_DchgControlFlag == 1) && (pbm[i].Error_DS2777_2 == 0)) {
			TotalAbsCapacity = TotalAbsCapacity + pbm[i].Branch_2_AbcoluteCapacity;
			TotalRelCapacity = TotalRelCapacity + pbm[i].Branch_2_RelativeCapacity;
		}
		pbm[i].TotalRelativeCapacity = TotalRelCapacity / 2;
		pbm[i].TotalAbcoluteCapacity = TotalAbsCapacity;
		TotalAbsCapacity = 0;
		TotalRelCapacity = 0;
	}
	return SUCCESS;
}

/** @brief    Check flag to save setting.
@param     pbm[] - structure data for all PBM modules.
@retval    ErrorStatus
*/
int8_t PBM_CheckSaveSetupFlag(_PBM pbm[]){

    uint16_t i = 0;

    for( i = 0; i < PBM_QUANTITY; i++){

        if( pbm[i].PBM_save_conf_flag == 1){
            return 1;
        }
    }

    return 0;
}

