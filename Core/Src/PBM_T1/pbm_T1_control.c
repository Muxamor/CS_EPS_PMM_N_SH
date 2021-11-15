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

/** @brief	Read state temperature sensors from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2 or PBM_T1_BRANCH_ALL).
    @param  i2c_mux_ch  - Number channel MUX
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateTempSense(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t i2c_mux_ch) {

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
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {

		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			pbm[PBM_number].PCA9534_TempSens_State_1 = !(uint8_t) ((data8 & PCA9534_IO_P01) >> 1);
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].PCA9534_TempSens_State_2 = !(uint8_t) ((data8 & PCA9534_IO_P04) >> 4);
		}

		pbm[PBM_number].PCA9534_P6 = (uint8_t) ((data8 & PCA9534_IO_P06) >> 6);
		pbm[PBM_number].PCA9534_P7 = (uint8_t) ((data8 & PCA9534_IO_P07) >> 7);

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}
}

/** @brief	Read state heat from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2 or PBM_T1_BRANCH_ALL).
    @param  i2c_mux_ch  - Number channel MUX
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t i2c_mux_ch) {

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
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			pbm[PBM_number].Error_Heat_1 = ERROR;
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Heat_2 = ERROR;
		}
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			if (pbm[PBM_number].PCA9534_ON_Heat_1 == (data8 & PCA9534_IO_P00)) {
				pbm[PBM_number].Error_Heat_1 = SUCCESS;
			} else {
			    //Try update pin value
				PBM_T1_SetStateHeatBranch(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_1, pbm[PBM_number].PCA9534_ON_Heat_1, PBM_T1_I2C_MUX_CH_GPIO);
				pbm[PBM_number].Error_Heat_1 = ERROR;
			}
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			if (pbm[PBM_number].PCA9534_ON_Heat_2 == ((data8 & PCA9534_IO_P03) >> 3)) {
				pbm[PBM_number].Error_Heat_2 = SUCCESS;
			} else {
	            //Try update pin value
				PBM_T1_SetStateHeatBranch(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_2, pbm[PBM_number].PCA9534_ON_Heat_2, PBM_T1_I2C_MUX_CH_GPIO);
				pbm[PBM_number].Error_Heat_2 = ERROR;
			}
		}

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			pbm[PBM_number].Error_Heat_1 = ERROR;
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Heat_2 = ERROR;
		}
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}
}

/** @brief	Read state emergency charge from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2 or PBM_T1_BRANCH_ALL).
    @param  i2c_mux_ch  - Number channel MUX
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateEmergChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t i2c_mux_ch) {

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
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			pbm[PBM_number].Error_Emerg_Chrg_1 = ERROR;
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Emerg_Chrg_2 = ERROR;
		}
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {

		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			if (pbm[PBM_number].PCA9534_Emerg_Chrg_1 == ((data8 & PCA9534_IO_P02) >> 2)) {
				pbm[PBM_number].Error_Emerg_Chrg_1 = SUCCESS;
			} else {
				//Try update pin value
				PBM_T1_SetStateEmergChrg(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_1, pbm[PBM_number].PCA9534_Emerg_Chrg_1, PBM_T1_I2C_MUX_CH_GPIO);
				pbm[PBM_number].Error_Emerg_Chrg_1 = ERROR;
			}
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			if (pbm[PBM_number].PCA9534_Emerg_Chrg_2 == ((data8 & PCA9534_IO_P05) >> 5)) {
				pbm[PBM_number].Error_Emerg_Chrg_2 = SUCCESS;
			} else {
				//Try update pin value
				PBM_T1_SetStateEmergChrg(I2Cx, pbm, PBM_number, PBM_T1_BRANCH_2, pbm[PBM_number].PCA9534_Emerg_Chrg_2, PBM_T1_I2C_MUX_CH_GPIO);
				pbm[PBM_number].Error_Emerg_Chrg_2 = ERROR;
			}
		}

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){
			pbm[PBM_number].Error_Emerg_Chrg_1 = ERROR;
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Emerg_Chrg_2 = ERROR;
		}
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
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t tmp1075_addr, uint8_t i2c_mux_ch) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = TMP1075_read_int8_temperature(I2Cx, tmp1075_addr, &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	switch( tmp1075_addr ){

		case PBM_T1_TMP1075_ADDR_1:
			if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_1 = 0x7F;
				pbm[PBM_number].Error_TMP1075_1 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_1 = data8;
				pbm[PBM_number].Error_TMP1075_1 = SUCCESS; //No error
			}
			break;

		case PBM_T1_TMP1075_ADDR_2:
			if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_2 = 0x7F;
				pbm[PBM_number].Error_TMP1075_2 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_2 = data8;
				pbm[PBM_number].Error_TMP1075_2 = SUCCESS; //No error
			}
			break;

		case PBM_T1_TMP1075_ADDR_3:
			if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_3 = 0x7F;
				pbm[PBM_number].Error_TMP1075_3 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_3 = data8;
				pbm[PBM_number].Error_TMP1075_3 = SUCCESS; //No error
			}
			break;

		case PBM_T1_TMP1075_ADDR_4:
			if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_4 = 0x7F;
				pbm[PBM_number].Error_TMP1075_4 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_4 = data8;
				pbm[PBM_number].Error_TMP1075_4 = SUCCESS; //No error
			}
			break;

		default:
			break;
	}

	if (Error != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Read data from two MAX17320 for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2).
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadBatteryTelemetry(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t i2c_mux_ch) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t count = 0;
	uint8_t j = 0;
	MAX17320_BranchData Struct = {0};
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch); // TCA9548_CH6 TCA9548_CH5
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

			Error = MAX17320_Read_Balancing_Reg (I2Cx, &Struct);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}

			Error = MAX17320_Read_Br_Voltage_mV (I2Cx, &Struct, MAX17320_2S_batt);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}

			Error = MAX17320_Read_Br_Current_mA (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}


			Error = MAX17320_Read_Br_Pwr_mW (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}


			Error = MAX17320_Read_Br_Temp_Dg (I2Cx, &Struct, MAX17320_0_termistors);
			if (Error != SUCCESS) {
				Error_count++;
				Struct.DieTemp_Dg = 0x7F;
				continue;
			}


			Error = MAX17320_Read_Br_GaugeOut_mAh (I2Cx, &Struct, PBM_T1_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}

			if (Error_count != SUCCESS){
				Error = ERROR_N;
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
		if (Branch == PBM_T1_BRANCH_1){
			j = 0;
		}
		if (Branch == PBM_T1_BRANCH_2) {
			j = 1;
		}
		pbm[PBM_number].Branch[j].OVP = Struct.OVP;
		pbm[PBM_number].Branch[j].UVP = Struct.UVP;
		pbm[PBM_number].Branch[j].OCCP = Struct.OCCP;
		pbm[PBM_number].Branch[j].ODCP = Struct.ODCP;
		pbm[PBM_number].Branch[j].PreqF = Struct.PreqF;
		pbm[PBM_number].Branch[j].BalCell1 = Struct.BalCell1;
		pbm[PBM_number].Branch[j].BalCell2 = Struct.BalCell2;
		pbm[PBM_number].Branch[j].VoltageHi = Struct.Cell4_mV;
		pbm[PBM_number].Branch[j].VoltageLo = Struct.Cell1_mV;
		pbm[PBM_number].Branch[j].Current = Struct.Curr_mA;
		pbm[PBM_number].Branch[j].AverageCurrent = Struct.AvgCurr_mA;
		pbm[PBM_number].Branch[j].Power = Struct.Pwr_mW;
		pbm[PBM_number].Branch[j].AveragePower = Struct.AvgPwr_mW;
		pbm[PBM_number].Branch[j].Temperature = Struct.DieTemp_Dg;
		pbm[PBM_number].Branch[j].AbcoluteCapacity_mAh = Struct.RepCap_mAh;
		pbm[PBM_number].Branch[j].RelativeCapacity_Perc = Struct.RepSOC_Per;
		pbm[PBM_number].Branch[j].AgeScalar = Struct.Age_Dg;
		pbm[PBM_number].Branch[j].CycleCounter = Struct.Cycles_Cn;
		if (Error == SUCCESS) {
			if (Branch == PBM_T1_BRANCH_1){
				pbm[PBM_number].Error_MAX17320_1 = SUCCESS;

			}
			if (Branch == PBM_T1_BRANCH_2) {
				pbm[PBM_number].Error_MAX17320_2 = SUCCESS;
			}
		} else {
			if (Branch == PBM_T1_BRANCH_1){
				pbm[PBM_number].Error_MAX17320_1 = ERROR;
			}
			if (Branch == PBM_T1_BRANCH_2) {
				pbm[PBM_number].Error_MAX17320_2 = ERROR;
			}
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		if (Branch == PBM_T1_BRANCH_1){
			pbm[PBM_number].Error_MAX17320_1 = ERROR;
		}
		if (Branch == PBM_T1_BRANCH_2) {
			pbm[PBM_number].Error_MAX17320_2 = ERROR;
		}
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
	@param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2 or PBM_T1_BRANCH_ALL).
	@param 	State - select state heat (PBM_T1_ON_HEAT or PBM_T1_OFF_HEAT).
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State, uint8_t i2c_mux_ch) {

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
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch); //TCA9548_CH2
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Heat_1 = ERROR;
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Heat_2 = ERROR;
		}
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	ON/OFF emergency charge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2 or PBM_T1_BRANCH_ALL).
	@param 	State - select state heat (PBM_T1_ON_SHDN_CHRG or PBM_T1_OFF_SHDN_CHRG).
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateEmergChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State, uint8_t i2c_mux_ch) {

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
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch); //TCA9548_CH2
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)){

			if( pbm[PBM_number].PCA9534_Emerg_Chrg_1 != State ){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}

			pbm[PBM_number].PCA9534_Emerg_Chrg_1 = State;

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
				if (pbm[PBM_number].PCA9534_Emerg_Chrg_1 == data8 ) {
					pbm[PBM_number].Error_Emerg_Chrg_1 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_Emerg_Chrg_1 = ERROR;
				}
				pbm[PBM_number].Error_PCA9534 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_PCA9534 = ERROR;
				pbm[PBM_number].Error_Emerg_Chrg_1 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}

		Error = ERROR_N;
		count = 0;
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {

			if(pbm[PBM_number].PCA9534_Emerg_Chrg_2 != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}

			pbm[PBM_number].PCA9534_Emerg_Chrg_2 = State;

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
				if (pbm[PBM_number].PCA9534_Emerg_Chrg_2 == data8) {
					pbm[PBM_number].Error_Emerg_Chrg_2 = SUCCESS;
				} else {
					Error_count = Error_count + 1;
					pbm[PBM_number].Error_Emerg_Chrg_2 = ERROR;
				}
				pbm[PBM_number].Error_PCA9534 = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Error_PCA9534 = ERROR;
				pbm[PBM_number].Error_Emerg_Chrg_2 = ERROR;
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		if ((Branch == PBM_T1_BRANCH_1) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Emerg_Chrg_1 = ERROR;
		}
		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			pbm[PBM_number].Error_Emerg_Chrg_2 = ERROR;
		}
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
	@param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2 or PBM_T1_BRANCH_ALL).
	@param 	State - select state charge (PBM_T1_ON_CHARGE or PBM_T1_OFF_CHARGE).
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State, uint8_t i2c_mux_ch) {

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
			Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch);
			if( Error != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		Error_I2C_MUX = Error;
	}

	if (Error == SUCCESS ){
		if (Branch == PBM_T1_BRANCH_1) {
			if(pbm[PBM_number].Branch[1].ChgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch[1].ChgEnableBit = State;
			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_CHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch[1].ChgControlFlag = data_reg.CHGs;
				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch[1].ChgControlFlag == State) {
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

		if ((Branch == PBM_T1_BRANCH_2) || (Branch == PBM_T1_BRANCH_ALL)) {
			if(pbm[PBM_number].Branch[2].ChgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch[2].ChgEnableBit = State;
			Error = ERROR_N;
			while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_CHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch[2].ChgControlFlag = data_reg.CHGs;

				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch[2].ChgControlFlag == State) {
					pbm[PBM_number].Error_Charge_2 = SUCCESS;
				} else {
					pbm[PBM_number].Error_Charge_2 = ERROR;
				}
				pbm[PBM_number].Error_MAX17320_2 = SUCCESS;
			} else {
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		if (Branch == PBM_T1_BRANCH_1) {
			pbm[PBM_number].Error_MAX17320_1 = ERROR;
			pbm[PBM_number].Error_Charge_1 = ERROR;
		}
		if (Branch == PBM_T1_BRANCH_2) {
			pbm[PBM_number].Error_MAX17320_2 = ERROR;
			pbm[PBM_number].Error_Charge_2 = ERROR;
		}
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
	@param 	Branch - select Branch (PBM_Branch[1], PBM_Branch[2]).
	@param 	State - select state charge (PBM_ON_DISCHARGE or PBM_OFF_DISCHARGE).
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State, uint8_t i2c_mux_ch) {

	int8_t Error = ERROR_N;
	//int8_t Error_count = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = 0;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };
	MAX17320_RegDataEEPROME data_reg;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch); //TCA9548_CH6 TCA9548_CH5
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}

		Error_I2C_MUX = Error;
	}
	if (Error == SUCCESS ){
		if (Branch == PBM_T1_BRANCH_1) {
			if(pbm[PBM_number].Branch[1].DchgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch[1].DchgEnableBit = State;
			Error = ERROR_N;
			while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_DCHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch[1].ChgControlFlag = data_reg.DISs;
				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (Branch == PBM_T1_BRANCH_1) {
					if (pbm[PBM_number].Branch[1].DchgControlFlag == State) {
						pbm[PBM_number].Error_Discharge_1 = SUCCESS;
					} else {
						pbm[PBM_number].Error_Discharge_1 = ERROR;
					}
				}
				pbm[PBM_number].Error_MAX17320_1 = SUCCESS;
			} else {
				if (Branch == PBM_T1_BRANCH_1) {
					pbm[PBM_number].Error_MAX17320_1 = ERROR;
					pbm[PBM_number].Error_Discharge_1 = ERROR;
				}
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
		if (Branch == PBM_T1_BRANCH_2) {
			if(pbm[PBM_number].Branch[2].DchgEnableBit != State){
				pbm[PBM_number].PBM_save_conf_flag = 1;
			}
			pbm[PBM_number].Branch[2].DchgEnableBit = State;
			Error = ERROR_N;
			while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
				Error = MAX17320_Write_ON_OFF_DCHRG_FET(I2Cx, State);
				LL_mDelay(10);
				Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
				pbm[PBM_number].Branch[2].DchgControlFlag = data_reg.DISs;
				if (Error != SUCCESS) {
					LL_mDelay(PBM_T1_i2c_delay_att_conn);
					count++;
				}
			}

			if (Error == SUCCESS) {
				if (pbm[PBM_number].Branch[2].DchgControlFlag == State) {
					pbm[PBM_number].Error_Discharge_2 = SUCCESS;
				} else {
					pbm[PBM_number].Error_Discharge_2 = ERROR;
				}
				pbm[PBM_number].Error_MAX17320_2 = SUCCESS;
			} else {
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		if (Branch == PBM_T1_BRANCH_1) {
			pbm[PBM_number].Error_MAX17320_1 = ERROR;
			pbm[PBM_number].Error_Discharge_1 = ERROR;
		}
		if (Branch == PBM_T1_BRANCH_2) {
			pbm[PBM_number].Error_MAX17320_2 = ERROR;
			pbm[PBM_number].Error_Discharge_2 = ERROR;
		}
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Check auto heat OFF.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2).
    @param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckOverHeat(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t i2c_mux_ch) {

	uint8_t count = 0;
	int8_t Error_count = 0;

	if (Branch == PBM_T1_BRANCH_1) {
		if ((pbm[PBM_number].TMP1075_temp_1 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_1 == 1)) {
			count++;
		}
		if ((pbm[PBM_number].TMP1075_temp_2 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_1 == 1)) {
			count++;
		}
	}
	if (Branch == PBM_T1_BRANCH_2) {
		if ((pbm[PBM_number].TMP1075_temp_3 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_2 == 1)) {
			count++;
		}
		if ((pbm[PBM_number].TMP1075_temp_4 >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].PCA9534_TempSens_State_2 == 1)) {
			count++;
		}
	}

	if (count >= 1) { // off heat if at least one temp sense dont work right
		PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_number, Branch, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		if (Branch == PBM_T1_BRANCH_1) {
			PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_number, PBM_T1_TMP1075_ADDR_1, i2c_mux_ch); //TCA9548_CH0
			PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_number, PBM_T1_TMP1075_ADDR_2, i2c_mux_ch);
			pbm[PBM_number].PCA9534_ON_Heat_1 = 1;
			pbm[PBM_number].Error_Heat_1 = ERROR;
		}
		if (Branch == PBM_T1_BRANCH_2) {
	        PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_number, PBM_T1_TMP1075_ADDR_3, i2c_mux_ch); //TCA9548_CH3
	        PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_number, PBM_T1_TMP1075_ADDR_4, i2c_mux_ch);
			pbm[PBM_number].PCA9534_ON_Heat_2 = 1;
			pbm[PBM_number].Error_Heat_2 = ERROR;
		}
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

	if((pbm[PBM_number].Branch[1].ChgEnableBit == pbm[PBM_number].Branch[1].ChgControlFlag) && (pbm[PBM_number].Error_MAX17320_1 == SUCCESS)){
		pbm[PBM_number].Error_Charge_1 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Charge_1 = ERROR;
	}
	if((pbm[PBM_number].Branch[2].ChgEnableBit == pbm[PBM_number].Branch[2].ChgControlFlag) && (pbm[PBM_number].Error_MAX17320_2 == SUCCESS)){
		pbm[PBM_number].Error_Charge_2 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Charge_2 = ERROR;
	}
	if((pbm[PBM_number].Branch[1].DchgEnableBit == pbm[PBM_number].Branch[1].DchgControlFlag) && (pbm[PBM_number].Error_MAX17320_1 == SUCCESS)){
		pbm[PBM_number].Error_Discharge_1 = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Error_Discharge_1 = ERROR;
	}
	if((pbm[PBM_number].Branch[2].DchgEnableBit == pbm[PBM_number].Branch[2].DchgControlFlag) && (pbm[PBM_number].Error_MAX17320_2 == SUCCESS)){
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
	@param 	Branch - select Branch (PBM_Branch[1], PBM_Branch[2]).
	@param  i2c_mux_ch  - Number channel MUX
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CorrectCapacity(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t i2c_mux_ch) {

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
		Error_i2c = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch);//TCA9548_CH6 TCA9548_CH5
		if( Error_i2c != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_i2c;

	if (Error == SUCCESS ){
		if ((Branch == PBM_T1_BRANCH_1) && (pbm[PBM_number].Error_MAX17320_1 == 0)) {
			Capacity = ((float) (pbm[PBM_number].Branch[1].AbcoluteCapacity_mAh) / PBM_T1_MAX_BATT_CAP * 100.0f); // (3000.0 * 100.0) convert to %
			Voltage = ((float) (pbm[PBM_number].Branch[1].VoltageHi + pbm[PBM_number].Branch[1].VoltageLo)) / 2.0f;
			Voltage = (float) (Voltage - 2500.0f) * 100.0f / 1650.0f; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0f) * 100.0f;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 100)) {
				AbsoluteCapacity = (int16_t) ((Voltage * PBM_T1_MAX_BATT_CAP) / 100);
				MAX17320_WriteAccmCharge(I2Cx, PBM_T1_MAX_BATT_CAP, PBM_T1_R_SENSE, AbsoluteCapacity);
			}
		}
		if ((Branch == PBM_T1_BRANCH_2) && (pbm[PBM_number].Error_MAX17320_2 == 0)) {
			Capacity = ((float) (pbm[PBM_number].Branch[2].AbcoluteCapacity_mAh) / PBM_T1_MAX_BATT_CAP * 100.0f); // in %
			Voltage = ((float) (pbm[PBM_number].Branch[2].VoltageHi + pbm[PBM_number].Branch[2].VoltageLo)) / 2.0f;
			Voltage = (float) (Voltage - 2500.0f) * 100.0f / 1650.0f; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0f) * 100.0f;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 100)) {
				AbsoluteCapacity = (int16_t) ((Voltage * PBM_T1_MAX_BATT_CAP) / 100);
				MAX17320_WriteAccmCharge(I2Cx, PBM_T1_MAX_BATT_CAP, PBM_T1_R_SENSE, AbsoluteCapacity);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{

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

	if ((pbm[PBM_number].Branch[1].DchgControlFlag == 1) && (pbm[PBM_number].Error_MAX17320_1 == 0)) {
		TotalAbsCapacity = pbm[PBM_number].Branch[1].AbcoluteCapacity_mAh;
		TotalRelCapacity = pbm[PBM_number].Branch[1].RelativeCapacity_Perc;
	}
	if ((pbm[PBM_number].Branch[2].DchgControlFlag == 1) && (pbm[PBM_number].Error_MAX17320_2 == 0)) {
		TotalAbsCapacity = TotalAbsCapacity + pbm[PBM_number].Branch[2].AbcoluteCapacity_mAh;
		TotalRelCapacity = TotalRelCapacity + pbm[PBM_number].Branch[2].RelativeCapacity_Perc;
	}
	pbm[PBM_number].TotalRelativeCapacity_Perc = TotalRelCapacity / 2;
	pbm[PBM_number].TotalAbcoluteCapacity_mAh = TotalAbsCapacity;
}

/** @brief	Check level battery energy for selected PBM.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
*/
void PBM_T1_CheckLowLevelEnergy(_PBM_T1 pbm[], uint8_t PBM_number) {

	int16_t Voltage_Branch_1 = 0, Voltage_Branch_2 = 0;

	Voltage_Branch_1 = pbm[PBM_number].Branch[1].VoltageHi + pbm[PBM_number].Branch[1].VoltageLo;
	Voltage_Branch_2 = pbm[PBM_number].Branch[2].VoltageHi + pbm[PBM_number].Branch[2].VoltageLo;

	if (((Voltage_Branch_1 <= PBM_T1_LOW_ENERGY_EDGE) && ((pbm[PBM_number].Error_MAX17320_1 == SUCCESS) && (pbm[PBM_number].Branch[1].DchgControlFlag == ENABLE))) ||
			((Voltage_Branch_2 <= PBM_T1_LOW_ENERGY_EDGE) && ((pbm[PBM_number].Error_MAX17320_2 == SUCCESS) && (pbm[PBM_number].Branch[2].DchgControlFlag == ENABLE)))) {

	    pbm[PBM_number].Low_Energy_Flag = 1;

	}else if( pbm[PBM_number].Low_Energy_Flag == 1 ){

	    if((Voltage_Branch_1 >= PBM_T1_NORMAL_ENERGY_EDGE) || (pbm[PBM_number].Error_MAX17320_1 == ERROR) || (pbm[PBM_number].Branch[1].DchgControlFlag == DISABLE)) {
			if ((Voltage_Branch_2 >= PBM_T1_NORMAL_ENERGY_EDGE) || (pbm[PBM_number].Error_MAX17320_2 == ERROR) || (pbm[PBM_number].Branch[2].DchgControlFlag == DISABLE)){
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
