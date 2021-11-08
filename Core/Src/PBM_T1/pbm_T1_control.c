#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "MAX17320.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_control.h"
#include "Error_Handler.h"

/** @brief	Read state pin PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadGPIO(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH2);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay( PBM_T1_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS) {
		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			if(PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00|PCA9534_IO_P02|PCA9534_IO_P03|PCA9534_IO_P05) == SUCCESS){
				if (PCA9534_conf_IO_dir_input(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_INPUT_PIN) == SUCCESS) {
					Error = PCA9534_conf_IO_pol_normal(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_INPUT_PIN);
				}
			}
			if (Error == SUCCESS) {
				Error = PCA9534_read_input_reg(I2Cx, pbm_table.GPIO_Addr, &data8);
			}

			if (Error != SUCCESS) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH2);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Error_Heat_1 = ERROR;
		pbm[PBM_number].Error_Heat_2 = ERROR;
		pbm[PBM_number].Error_SHDN_Chrg_1 = SUCCESS;
		pbm[PBM_number].Error_SHDN_Chrg_2 = SUCCESS;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {
		if (pbm[PBM_number].PCA9534_ON_Heat_1 == (data8 & PCA9534_IO_P00)) {
			pbm[PBM_number].Error_Heat_1 = SUCCESS;
		} else {
		    //Try update pin value
			PBM_T1_SetStateHeatBranch(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_1, pbm[PBM_number].PCA9534_ON_Heat_1);
			pbm[PBM_number].Error_Heat_1 = ERROR;
		}

		if (pbm[PBM_number].PCA9534_ON_Heat_2 == ((data8 & PCA9534_IO_P03) >> 3)) {
			pbm[PBM_number].Error_Heat_2 = SUCCESS;
		} else {
            //Try update pin value
			PBM_T1_SetStateHeatBranch(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_2, pbm[PBM_number].PCA9534_ON_Heat_2);
			pbm[PBM_number].Error_Heat_2 = ERROR;
		}

		if (pbm[PBM_number].PCA9534_SHDN_Chrg_1 == ((data8 & PCA9534_IO_P02) >> 2)) {
			pbm[PBM_number].Error_SHDN_Chrg_1 = SUCCESS;
		} else {
		    //Try update pin value
			PBM_T1_SetStateShdnChrg(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_1, pbm[PBM_number].PCA9534_SHDN_Chrg_1);
			pbm[PBM_number].Error_SHDN_Chrg_1 = ERROR;
		}

		if (pbm[PBM_number].PCA9534_SHDN_Chrg_2 == ((data8 & PCA9534_IO_P05) >> 5)) {
			pbm[PBM_number].Error_SHDN_Chrg_2 = SUCCESS;
		} else {
		    //Try update pin value
			PBM_T1_SetStateShdnChrg(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_1, pbm[PBM_number].PCA9534_SHDN_Chrg_2);
			pbm[PBM_number].Error_SHDN_Chrg_2 = ERROR;
		}

		pbm[PBM_number].PCA9534_TempSens_State_1 = !(uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
		pbm[PBM_number].PCA9534_TempSens_State_2 = !(uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
		pbm[PBM_number].PCA9534_P6 = (uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
		pbm[PBM_number].PCA9534_P7 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
        pbm[PBM_number].Error_Heat_1 = ERROR;
        pbm[PBM_number].Error_Heat_2 = ERROR;
		pbm[PBM_number].Error_SHDN_Chrg_1 = ERROR;
		pbm[PBM_number].Error_SHDN_Chrg_2 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}
}

/** @brief	Read temperature all TMP1075 sensor for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH0);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_1_Addr, &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
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

		Error = ERROR_N;
		count = 0;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_2_Addr, &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
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
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH0);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_TMP1075_1 = ERROR;
		pbm[PBM_number].Error_TMP1075_2 = ERROR;
	}else{
		i = 0;
		Error = ERROR_N;
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH3);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay( PBM_T1_i2c_delay_att_conn );
			}
		}

		Error_I2C_MUX = Error;
	}

	if (Error == SUCCESS ){
		Error = ERROR_N;
		count = 0;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_3_Addr, &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
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

		Error = ERROR_N;
		count = 0;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_4_Addr, &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
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
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH3);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_TMP1075_3 = ERROR;
		pbm[PBM_number].Error_TMP1075_4 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Read data from two DS2777 for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadBatteryData(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t count = 0;
	MAX17320_BranchData Struct;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH6);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error_count = 0;
			Error = MAX17320_Read_Br_ProtStatus_Reg (I2Cx, &Struct);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_1_OVP = Struct.OVP;
			pbm[PBM_number].Branch_1_UVP = Struct.UVP;
			pbm[PBM_number].Branch_1_OCCP = Struct.OCCP;
			pbm[PBM_number].Branch_1_ODCP = Struct.ODCP;
			pbm[PBM_number].Branch_1_PreqF = Struct.PreqF;

			Error = MAX17320_Read_Balancing_Reg (I2Cx, &Struct);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_1_BalCell1 = Struct.BalCell1;
			pbm[PBM_number].Branch_1_BalCell2 = Struct.BalCell2;

			Error = MAX17320_Read_Br_Voltage_mV (I2Cx, &Struct, MAX17320_2S_batt);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_1_VoltageHi = Struct.Cell4_mV;
			pbm[PBM_number].Branch_1_VoltageLo = Struct.Cell1_mV;

			Error = MAX17320_Read_Br_Current_mA (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_1_Current = Struct.Curr_mA;
			pbm[PBM_number].Branch_1_AverageCurrent = Struct.AvgCurr_mA;

			Error = MAX17320_Read_Br_Pwr_mW (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_1_Power = Struct.Pwr_mW;
			pbm[PBM_number].Branch_1_AveragePower = Struct.AvgPwr_mW;

			Error = MAX17320_Read_Br_Temp_Dg (I2Cx, &Struct, MAX17320_0_termistors);
			if (Error != SUCCESS) {
				Error_count++;
				Struct.DieTemp_Dg = 0x7F;
				continue;
			}
			pbm[PBM_number].Branch_1_Temperature = Struct.DieTemp_Dg;

			Error = MAX17320_Read_Br_GaugeOut_mAh (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_1_AbcoluteCapacity = Struct.RepCap_mAh;
			pbm[PBM_number].Branch_1_RelativeCapacity = Struct.RepSOC_Per;
			pbm[PBM_number].Branch_1_AgeScalar = Struct.Age_Dg;
			pbm[PBM_number].Branch_1_CycleCounter = Struct.Cycles_Cn;

			if (Error_count != SUCCESS){
				Error = ERROR_N;
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		if (Error == SUCCESS) {
			pbm[PBM_number].Error_MAX17320_1 = SUCCESS;
		} else {
			pbm[PBM_number].Error_MAX17320_1 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH6);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_MAX17320_1 = ERROR;
	}else{
		i = 0;
		Error = ERROR_N;
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay( PBM_T1_i2c_delay_att_conn );
			}
		}

		Error_I2C_MUX = Error;
	}

	if (Error == SUCCESS ){
		Error = ERROR_N;
		count = 0;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error_count = 0;
			Error = MAX17320_Read_Br_ProtStatus_Reg (I2Cx, &Struct);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_2_OVP = Struct.OVP;
			pbm[PBM_number].Branch_2_UVP = Struct.UVP;
			pbm[PBM_number].Branch_2_OCCP = Struct.OCCP;
			pbm[PBM_number].Branch_2_ODCP = Struct.ODCP;
			pbm[PBM_number].Branch_2_PreqF = Struct.PreqF;

			Error = MAX17320_Read_Balancing_Reg (I2Cx, &Struct);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_2_BalCell1 = Struct.BalCell1;
			pbm[PBM_number].Branch_2_BalCell2 = Struct.BalCell2;

			Error = MAX17320_Read_Br_Voltage_mV (I2Cx, &Struct, MAX17320_2S_batt);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_2_VoltageHi = Struct.Cell4_mV;
			pbm[PBM_number].Branch_2_VoltageLo = Struct.Cell1_mV;

			Error = MAX17320_Read_Br_Current_mA (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_2_Current = Struct.Curr_mA;
			pbm[PBM_number].Branch_2_AverageCurrent = Struct.AvgCurr_mA;

			Error = MAX17320_Read_Br_Pwr_mW (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_2_Power = Struct.Pwr_mW;
			pbm[PBM_number].Branch_2_AveragePower = Struct.AvgPwr_mW;

			Error = MAX17320_Read_Br_Temp_Dg (I2Cx, &Struct, MAX17320_0_termistors);
			if (Error != SUCCESS) {
				Error_count++;
				Struct.DieTemp_Dg = 0x7F;
				continue;
			}
			pbm[PBM_number].Branch_2_Temperature = Struct.DieTemp_Dg;

			Error = MAX17320_Read_Br_GaugeOut_mAh (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}
			pbm[PBM_number].Branch_2_AbcoluteCapacity = Struct.RepCap_mAh;
			pbm[PBM_number].Branch_2_RelativeCapacity = Struct.RepSOC_Per;
			pbm[PBM_number].Branch_2_AgeScalar = Struct.Age_Dg;
			pbm[PBM_number].Branch_2_CycleCounter = Struct.Cycles_Cn;

			if (Error_count != SUCCESS){
				Error = ERROR_N;
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		if (Error == SUCCESS) {
			pbm[PBM_number].Error_MAX17320_2 = SUCCESS;
		} else {
			pbm[PBM_number].Error_MAX17320_2 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}
	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_MAX17320_2 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	ON/OFF heat for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_T1_BRANCH_1, PBM_T1_BRANCH_2 or PBM_T1_BRANCH_ALL).
	@param 	State - select state heat (PBM_T1_ON_HEAT or PBM_T1_OFF_HEAT).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH2);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){

			if( pbm[PBM_number].PCA9534_ON_Heat_1 != State ){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}

			pbm[PBM_number].PCA9534_ON_Heat_1 = State;

			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

				if (State == PBM_T1_ON_HEAT) {
					Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00);
				} else {
					Error = PCA9534_Reset_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00);
				}

				if (Error == SUCCESS) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00);
				}

				if (Error == SUCCESS) {
					Error =  PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P00, &data8);
				}

				if (Error != SUCCESS) {
					count++;
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].PCA9534_ON_Heat_1 == data8 ) {
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

		Error = ERROR_N;
		count = 0;
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {

			if(pbm[PBM_number].PCA9534_ON_Heat_2 != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}

			pbm[PBM_number].PCA9534_ON_Heat_2 = State;

			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				if (State == PBM_T1_ON_HEAT) {
					Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P03);
				} else {
					Error = PCA9534_Reset_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P03);
				}

				if (Error == SUCCESS) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P03);
				}

				if (Error == SUCCESS) {
					Error =  PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P03, &data8);
				}

				if (Error != 0) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].PCA9534_ON_Heat_2 == data8) {
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
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH2);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Error_Heat_1 = ERROR;
		pbm[PBM_number].Error_Heat_2 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	ON/OFF shutdown charge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_T1_BRANCH_1, PBM_T1_BRANCH_2 or PBM_T1_BRANCH_ALL).
	@param 	State - select state heat (PBM_T1_ON_SHDN_CHRG or PBM_T1_OFF_SHDN_CHRG).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateShdnChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH2);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){

			if( pbm[PBM_number].PCA9534_SHDN_Chrg_1 != State ){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}

			pbm[PBM_number].PCA9534_SHDN_Chrg_1 = State;

			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

				if (State == PBM_T1_ON_HEAT) {
					Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P02);
				} else {
					Error = PCA9534_Reset_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P02);
				}

				if (Error == SUCCESS) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P02);
				}

				if (Error == SUCCESS) {
					Error =  PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P02, &data8);
				}

				if (Error != SUCCESS) {
					count++;
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].PCA9534_SHDN_Chrg_1 == data8 ) {
					pbm[PBM_number].Error_SHDN_Chrg_1 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_SHDN_Chrg_1 = ERROR;
				}
				pbm[PBM_number].Error_PCA9534 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_PCA9534 = ERROR;
				pbm[PBM_number].Error_SHDN_Chrg_1 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}

		Error = ERROR_N;
		count = 0;
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {

			if(pbm[PBM_number].PCA9534_SHDN_Chrg_2 != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}

			pbm[PBM_number].PCA9534_SHDN_Chrg_2 = State;

			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				if (State == PBM_T1_ON_HEAT) {
					Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05);
				} else {
					Error = PCA9534_Reset_output_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05);
				}

				if (Error == SUCCESS) {
					Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05);
				}

				if (Error == SUCCESS) {
					Error =  PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, PCA9534_IO_P05, &data8);
				}

				if (Error != 0) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].PCA9534_SHDN_Chrg_2 == data8) {
					pbm[PBM_number].Error_SHDN_Chrg_2 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_SHDN_Chrg_2 = ERROR;
				}
				pbm[PBM_number].Error_PCA9534 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_PCA9534 = ERROR;
				pbm[PBM_number].Error_SHDN_Chrg_2 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH2);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Error_SHDN_Chrg_1 = ERROR;
		pbm[PBM_number].Error_SHDN_Chrg_2 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	ON/OFF Charge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_T1_BRANCH_1, PBM_T1_BRANCH_2 or PBM_T1_BRANCH_ALL).
	@param 	State - select state charge (PBM_T1_ON_CHARGE or PBM_T1_OFF_CHARGE).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = 0;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };
	MAX17320_RegDataEEPROME data_reg;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)) {
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, TCA9548_CH6);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		Error_I2C_MUX = Error;
	}

	if (Error == SUCCESS ){
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)) {
			if(pbm[PBM_number].Branch_1_ChgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch_1_ChgEnableBit = State;
			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_CHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch_1_ChgControlFlag = data_reg.CHGs;
				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch_1_ChgControlFlag == State) {
					pbm[PBM_number].Error_Charge_1 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_Charge_1 = ERROR;
				}
				pbm[PBM_number].Error_MAX17320_1 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_MAX17320_1 = ERROR;
				pbm[PBM_number].Error_Charge_1 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH6);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_MAX17320_1 = ERROR;
		pbm[PBM_number].Error_Charge_1 = ERROR;
	}else{
		i = 0;
		Error = ERROR_N;
	}

	if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay( PBM_T1_i2c_delay_att_conn );
			}
		}

		Error_I2C_MUX = Error;
	}

	if (Error == SUCCESS ){
		Error = ERROR_N;
		count = 0;
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			if(pbm[PBM_number].Branch_2_ChgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch_2_ChgEnableBit = State;

			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_CHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch_2_ChgControlFlag = data_reg.CHGs;

				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch_2_ChgControlFlag == State) {
					pbm[PBM_number].Error_Charge_2 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_Charge_2 = ERROR;
				}
				pbm[PBM_number].Error_MAX17320_2 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_MAX17320_2 = ERROR;
				pbm[PBM_number].Error_Charge_2 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_MAX17320_2 = ERROR;
		pbm[PBM_number].Error_Charge_2 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}
	if (Error_count != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	ON/OFF Discharge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2 or PBM_BRANCH_ALL).
	@param 	State - select state charge (PBM_ON_DISCHARGE or PBM_OFF_DISCHARGE).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = 0;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };
	MAX17320_RegDataEEPROME data_reg;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)) {
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, TCA9548_CH6);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		Error_I2C_MUX = Error;
	}
	if (Error == SUCCESS ){
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)) {
			if(pbm[PBM_number].Branch_1_DchgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch_1_DchgEnableBit = State;
			Error = ERROR_N;
			while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_DCHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch_1_ChgControlFlag = data_reg.DISs;
				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch_1_DchgControlFlag == State) {
					pbm[PBM_number].Error_Discharge_1 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_Discharge_1 = ERROR;
				}
				pbm[PBM_number].Error_MAX17320_1 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_MAX17320_1 = ERROR;
				pbm[PBM_number].Error_Discharge_1 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH6);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_MAX17320_1 = ERROR;
		pbm[PBM_number].Error_Discharge_1 = ERROR;
	}else{
		i = 0;
		Error = ERROR_N;
	}

	if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay( PBM_T1_i2c_delay_att_conn );
			}
		}

		Error_I2C_MUX = Error;
	}

	if (Error == SUCCESS ){
		Error = ERROR_N;
		count = 0;
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			if(pbm[PBM_number].Branch_2_DchgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch_2_DchgEnableBit = State;

			while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_DCHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch_2_DchgControlFlag = data_reg.DISs;
				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch_2_DchgControlFlag == State) {
					pbm[PBM_number].Error_Discharge_2 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_Discharge_2 = ERROR;
				}
				pbm[PBM_number].Error_MAX17320_2 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_MAX17320_2 = ERROR;
				pbm[PBM_number].Error_Discharge_2 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_MAX17320_2 = ERROR;
		pbm[PBM_number].Error_Discharge_2 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}
	if (Error_count != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Check auto heat OFF.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckHeatOFF(_PBM_T1 pbm[], uint8_t PBM_number) {

	uint8_t count = 0;
	int8_t Error_count = 0;

	if ((pbm[PBM_number].TMP1075_temp_1 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_1 == 1)) {
		count++;
	}
	if ((pbm[PBM_number].TMP1075_temp_2 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_1 == 1)) {
		count++;
	}
	if (count >= 1) { // off heat if at least one temp sense dont work right
		PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_number, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT);
		PBM_T1_TempSensorInit(pbm, PBM_number);
		pbm[PBM_number].PCA9534_ON_Heat_1 = 1;
		pbm[PBM_number].Error_Heat_1 = ERROR;
		Error_count = Error_count + 1;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	count = 0;
	if ((pbm[PBM_number].TMP1075_temp_3 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_2 == 1)) {
		count++;
	}
	if ((pbm[PBM_number].TMP1075_temp_4 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_2 == 1)) {
		count++;
	}
	if (count >= 1) { // off heat if at least one temp sense dont work right
		PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_number, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT);
		PBM_T1_TempSensorInit(pbm, PBM_number);
		pbm[PBM_number].PCA9534_ON_Heat_2 = 1;
		pbm[PBM_number].Error_Heat_2 = ERROR;
		Error_count = Error_count + 1;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if (Error_count != SUCCESS) {
		return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Check state charge/discharge keys for all PBM.
 	@param 	pbm[] - structure data for all PBM modules.
  	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckChargeDischargeState(_PBM_T1 pbm[], uint8_t PBM_number) {

	int8_t Error_count = 0;

	if((pbm[PBM_number].Branch_1_ChgEnableBit == pbm[PBM_number].Branch_1_ChgControlFlag) && (pbm[PBM_number].Error_MAX17320_1 == SUCCESS)){
		pbm[PBM_number].Error_Charge_1 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Charge_1 = ERROR;
	}
	if((pbm[PBM_number].Branch_2_ChgEnableBit == pbm[PBM_number].Branch_2_ChgControlFlag) && (pbm[PBM_number].Error_MAX17320_2 == SUCCESS)){
		pbm[PBM_number].Error_Charge_2 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Charge_2 = ERROR;
	}
	if((pbm[PBM_number].Branch_1_DchgEnableBit == pbm[PBM_number].Branch_1_DchgControlFlag) && (pbm[PBM_number].Error_MAX17320_1 == SUCCESS)){
		pbm[PBM_number].Error_Discharge_1 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Discharge_1 = ERROR;
	}
	if((pbm[PBM_number].Branch_2_DchgEnableBit == pbm[PBM_number].Branch_2_DchgControlFlag) && (pbm[PBM_number].Error_MAX17320_2 == SUCCESS)){
		pbm[PBM_number].Error_Discharge_2 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Discharge_2 = ERROR;
	}

	if (Error_count != SUCCESS) {
	    return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Check and correct capacity selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckCapacity(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number) {

	int16_t AbsoluteCapacity = 0;
	float Capacity = 0, Voltage = 0;
	float Error = 0;
	int8_t Error_i2c = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	int8_t Error_count = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while( ( Error_i2c != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_i2c = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, TCA9548_CH6);
		if( Error_i2c != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_i2c;

	if (Error == SUCCESS ){
		if (pbm[PBM_number].Error_MAX17320_1 == 0) {
			Capacity = ((float) (pbm[PBM_number].Branch_1_AbcoluteCapacity) / PBM_T1_MAX_BATT_CAP * 100.0f); // (3000.0 * 100.0) convert to %
			Voltage = ((float) (pbm[PBM_number].Branch_1_VoltageHi + pbm[PBM_number].Branch_1_VoltageLo)) / 2.0f;
			Voltage = (float) (Voltage - 2500.0f) * 100.0f / 1650.0f; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0f) * 100.0f;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 100)) {
				AbsoluteCapacity = (int16_t) ((Voltage * PBM_T1_MAX_BATT_CAP) / 100);
				MAX17320_WriteAccmCharge(I2Cx, PBM_T1_MAX_BATT_CAP, PBM_T1_R_SENSE, AbsoluteCapacity);
			}
		} else {
			Error_count = Error_count + 1;
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH6);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		i = 0;
		Error_i2c = ERROR_N;
		while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			Error_i2c = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);
			if( Error_i2c != SUCCESS ){
				i++;
				LL_mDelay( PBM_T1_i2c_delay_att_conn );
			}
		}
	}

	Error_I2C_MUX = Error_i2c;

	if (Error == SUCCESS ){
		if (pbm[PBM_number].Error_MAX17320_2 == 0) {
			Capacity = ((float) (pbm[PBM_number].Branch_2_AbcoluteCapacity) / PBM_T1_MAX_BATT_CAP * 100.0f); // in %
			Voltage = ((float) (pbm[PBM_number].Branch_2_VoltageHi + pbm[PBM_number].Branch_2_VoltageLo)) / 2.0f;
			Voltage = (float) (Voltage - 2500.0f) * 100.0f / 1650.0f; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0f) * 100.0f;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 100)) {
				AbsoluteCapacity = (int16_t) ((Voltage * PBM_T1_MAX_BATT_CAP) / 100);
				MAX17320_WriteAccmCharge(I2Cx, PBM_T1_MAX_BATT_CAP, PBM_T1_R_SENSE, AbsoluteCapacity);
		}
		} else {
				Error_count = Error_count + 1;
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH5);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
		return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Calculate full capacity selected PBM.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
*/
void PBM_T1_CalcTotalCapacity(_PBM_T1 pbm[], uint8_t PBM_number) {

	int16_t TotalAbsCapacity = 0;
	uint8_t TotalRelCapacity = 0;

	if ((pbm[PBM_number].Branch_1_DchgControlFlag == 1) && (pbm[PBM_number].Error_MAX17320_1 == 0)) {
		TotalAbsCapacity = pbm[PBM_number].Branch_1_AbcoluteCapacity;
		TotalRelCapacity = pbm[PBM_number].Branch_1_RelativeCapacity;
	}
	if ((pbm[PBM_number].Branch_2_DchgControlFlag == 1) && (pbm[PBM_number].Error_MAX17320_2 == 0)) {
		TotalAbsCapacity = TotalAbsCapacity + pbm[PBM_number].Branch_2_AbcoluteCapacity;
		TotalRelCapacity = TotalRelCapacity + pbm[PBM_number].Branch_2_RelativeCapacity;
	}
	pbm[PBM_number].TotalRelativeCapacity = TotalRelCapacity / 2;
	pbm[PBM_number].TotalAbcoluteCapacity = TotalAbsCapacity;
}

/** @brief	Check level battery energy for selected PBM.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
*/
void PBM_T1_CheckLevelEnergy(_PBM_T1 pbm[], uint8_t PBM_number) {

	int16_t Voltage_Branch_1 = 0, Voltage_Branch_2 = 0;

	Voltage_Branch_1 = pbm[PBM_number].Branch_1_VoltageHi + pbm[PBM_number].Branch_1_VoltageLo;
	Voltage_Branch_2 = pbm[PBM_number].Branch_2_VoltageHi + pbm[PBM_number].Branch_2_VoltageLo;

	if (((Voltage_Branch_1 <= PBM_T1_LOW_ENERGY_EDGE) && ((pbm[PBM_number].Error_MAX17320_1 == SUCCESS) && (pbm[PBM_number].Branch_1_DchgControlFlag == ENABLE))) ||
			((Voltage_Branch_2 <= PBM_T1_LOW_ENERGY_EDGE) && ((pbm[PBM_number].Error_MAX17320_2 == SUCCESS) && (pbm[PBM_number].Branch_2_DchgControlFlag == ENABLE)))) {

	    pbm[PBM_number].Low_Energy_Flag = 1;

	}else if( pbm[PBM_number].Low_Energy_Flag == 1 ){

	    if((Voltage_Branch_1 >= PBM_T1_NORMAL_ENERGY_EDGE) || (pbm[PBM_number].Error_MAX17320_1 == ERROR) || (pbm[PBM_number].Branch_1_DchgControlFlag == DISABLE)) {
			if ((Voltage_Branch_2 >= PBM_T1_NORMAL_ENERGY_EDGE) || (pbm[PBM_number].Error_MAX17320_2 == ERROR) || (pbm[PBM_number].Branch_2_DchgControlFlag == DISABLE)){
				pbm[PBM_number].Low_Energy_Flag = 0;
			}
		}
	}


    if( ( (Voltage_Branch_1 <= PBM_T1_ZERO_ENERGY_EDGE) || (Voltage_Branch_2 <= PBM_T1_ZERO_ENERGY_EDGE) ) && (pbm[PBM_number].Low_Energy_Flag == 1) ){
	    pbm[PBM_number].Zero_Energy_Flag = 1;

	}else if(pbm[PBM_number].Low_Energy_Flag == 0) {
	    pbm[PBM_number].Zero_Energy_Flag = 0;
	}
}

/** @brief    Check flag to save setting.
    @param     pbm[] - structure data for all PBM modules.
    @retval    ErrorStatus
*/
uint8_t PBM_T1_CheckSaveSetupFlag(_PBM_T1 pbm[]){

    for( uint16_t i = 0; i < PBM_T1_QUANTITY; i++){

        if( pbm[i].PBM_save_conf_flag == 1){
            return 1;
        }
    }

    return 0;
}

/** @brief    Clear flag to save setting.
    @param     pbm[] - structure data for all PBM modules.
    @retval    ErrorStatus
*/
void PBM_T1_ClearSaveSetupFlag(_PBM_T1 pbm[]){

    for( uint16_t i = 0; i < PBM_T1_QUANTITY; i++){

        pbm[i].PBM_save_conf_flag = 0;
    }

}
