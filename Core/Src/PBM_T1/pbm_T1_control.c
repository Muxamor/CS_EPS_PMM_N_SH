#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "MAX17320.h"
#include "INA238.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PBM_T1/pbm_T1_init_IC.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_control.h"
#include "Error_Handler.h"

/** @brief	Read state temperature sensors from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay( PBM_T1_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS) {
		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			if(PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_OUTPUT_PIN) == SUCCESS){
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

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

		pbm[PBM_number].Heat[Heat].PCA9534_Heat_State = !(uint8_t) (((data8 & pbm_table.GPIO_Pin_Heat_State) >> pbm_table.GPIO_Pin_Shift_Heat_State));

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
    @param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_CheckStateCmdHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay( PBM_T1_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS) {
		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			if(PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_OUTPUT_PIN) == SUCCESS){
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {
		if (pbm[PBM_number].Heat[Heat].PCA9534_ON_Heat_CMD == ((data8 & pbm_table.GPIO_Pin_Heat_CMD) >> pbm_table.GPIO_Pin_Shift_Heat_CMD)) {
			pbm[PBM_number].Heat[Heat].Error_Heat = SUCCESS;
		} else {
		    //Try update pin value
			PBM_T1_SetStateHeat(I2Cx, pbm, PBM_number, Heat, pbm[PBM_number].Heat[Heat].PCA9534_ON_Heat_CMD);
			pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;
		}

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;

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
    @param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateEmergChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay( PBM_T1_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS) {
		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			if(PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_OUTPUT_PIN) == SUCCESS){
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
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = ERROR;

	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {

		if (pbm[PBM_number].Branch[Branch].PCA9534_Emerg_Chrg == ((data8 & pbm_table.GPIO_Pin_EmergChrg) >> pbm_table.GPIO_Pin_Shift_EmergChrg)) {
			pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = SUCCESS;
		} else {
			//Try update pin value
			PBM_T1_SetStateEmergChrg(I2Cx, pbm, PBM_number, Branch, pbm[PBM_number].Branch[Branch].PCA9534_Emerg_Chrg);
			pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = ERROR;
		}

		pbm[PBM_number].Error_PCA9534 = SUCCESS;

		return SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = ERROR;

		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}
}

/** @brief	Read temperature heat TMP1075 sensor for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  temp_number - I2C sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadHeatTempSensors(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat, uint8_t temp_number) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TempSens_Heat_Addr[temp_number], &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Heat[Heat].Heat_TMP1075[temp_number] = 0x7F;
		pbm[PBM_number].Heat[Heat].Error_Heat_TMP1075[temp_number] = ERROR;
	}else{
		pbm[PBM_number].Heat[Heat].Heat_TMP1075[temp_number] = data8;
		pbm[PBM_number].Heat[Heat].Error_Heat_TMP1075[temp_number] = SUCCESS; //No error
	}

	if (Error != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Read temperature TMP1075 sensor for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  temp_number - I2C sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval 	ErrorStatus
 */
/*ErrorStatus PBM_T1_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t temp_number) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, 0);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_TempSens);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TEMP_SENSOR_Addr[temp_number], &data8);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_TempSens);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].TMP1075_Temp[temp_number] = 0x7F;
		pbm[PBM_number].Error_TMP1075[temp_number] = ERROR;
	}else{
		pbm[PBM_number].TMP1075_Temp[temp_number] = data8;
		pbm[PBM_number].Error_TMP1075[temp_number] = SUCCESS; //No error
	}

	if (Error != SUCCESS) {
        return ERROR_N;
	}

    return SUCCESS;
}*/

/** @brief	Read data from power monitor INA238 for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadHeatPwrMon(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat) {

	int16_t current = 0;
	uint16_t bus_voltage = 0, power = 0, alert_flags = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_PwrMon);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

			Error = INA238_Get_I_V_P_int16(I2Cx, pbm_table.PwrMon_Addr, PBM_T1_INA238_MAX_CURRENT, &current, &bus_voltage, &power);

			if( Error != SUCCESS ) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	if (Error == SUCCESS ){
			Error = INA238_Get_ALERT_Flags(I2Cx, pbm_table.PwrMon_Addr, &alert_flags);
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_PwrMon);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error == ERROR_N || Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Heat[Heat].HeatCurrent = 0;
		pbm[PBM_number].Heat[Heat].HeatVoltage = 0;
		pbm[PBM_number].Heat[Heat].HeatPower = 0;
		pbm[PBM_number].Heat[Heat].Error_INA238 = ERROR;
	}else{

		pbm[PBM_number].Heat[Heat].HeatVoltage = bus_voltage;
		pbm[PBM_number].Heat[Heat].HeatPower = power;
		if(((alert_flags & 0x0200) >> 9) == 1){
			pbm[PBM_number].Heat[Heat].HeatCurrent = -255;
			pbm[PBM_number].Heat[Heat].Error_INA238 = ERROR; //No error
		} else {
			pbm[PBM_number].Heat[Heat].HeatCurrent = current;
			pbm[PBM_number].Heat[Heat].Error_INA238 = SUCCESS; //No error
		}
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
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadBatteryTelemetry(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t count = 0;
	MAX17320_BranchData Struct = {0};
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch); // TCA9548_CH6 TCA9548_CH5
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

			Error = MAX17320_Read_Br_Voltage_mV (I2Cx, &Struct, PBM_T1_BRANCH_BAT_QUANTITY);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}

			Error = MAX17320_Read_Br_Current_mA (I2Cx, &Struct, PBM_T1_MAX17320_R_SENSE);
			if (Error != SUCCESS) {
				Error_count++;
				continue;
			}


			Error = MAX17320_Read_Br_Pwr_mW (I2Cx, &Struct, PBM_T1_MAX17320_R_SENSE);
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


			Error = MAX17320_Read_Br_GaugeOut_mAh (I2Cx, &Struct, PBM_T1_MAX17320_R_SENSE);
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

		pbm[PBM_number].Branch[Branch].OVP = Struct.OVP;
		pbm[PBM_number].Branch[Branch].UVP = Struct.UVP;
		pbm[PBM_number].Branch[Branch].OCCP = Struct.OCCP;
		pbm[PBM_number].Branch[Branch].ODCP = Struct.ODCP;
		pbm[PBM_number].Branch[Branch].PreqF = Struct.PreqF;
		pbm[PBM_number].Branch[Branch].BalCell1 = Struct.BalCell1;
		pbm[PBM_number].Branch[Branch].BalCell2 = Struct.BalCell2;
		pbm[PBM_number].Branch[Branch].LDet = Struct.LDet;
		pbm[PBM_number].Branch[Branch].Voltage[1] = Struct.Cell4_mV;
		pbm[PBM_number].Branch[Branch].Voltage[0] = Struct.Cell1_mV;
		pbm[PBM_number].Branch[Branch].MaxVCELL = Struct.MaxVCELL_mV;
		pbm[PBM_number].Branch[Branch].MinVCELL = Struct.MinVCELL_mV;
		pbm[PBM_number].Branch[Branch].Current = Struct.Curr_mA;
		pbm[PBM_number].Branch[Branch].AverageCurrent = Struct.AvgCurr_mA;
		pbm[PBM_number].Branch[Branch].MaxCurr = Struct.MaxCurr_mA;
		pbm[PBM_number].Branch[Branch].MinCurr = Struct.MinCurr_mA;
		pbm[PBM_number].Branch[Branch].Power = Struct.Pwr_mW;
		pbm[PBM_number].Branch[Branch].AveragePower = Struct.AvgPwr_mW;
		pbm[PBM_number].Branch[Branch].Temperature = Struct.DieTemp_Dg;
		pbm[PBM_number].Branch[Branch].AbcoluteCapacity_mAh = Struct.RepCap_mAh;
		pbm[PBM_number].Branch[Branch].RelativeCapacity_Perc = Struct.RepSOC_Per;
		pbm[PBM_number].Branch[Branch].AgeScalar = Struct.Age_Dg;
		pbm[PBM_number].Branch[Branch].CycleCounter = Struct.Cycles_Cn;
		pbm[PBM_number].Branch[Branch].RCell = Struct.RCell_mOhm;
		pbm[PBM_number].Branch[Branch].LeakCurr = Struct.LeakCurr_mA;

		if (Error == SUCCESS) {
			pbm[PBM_number].Branch[Branch].Error_MAX17320 = SUCCESS;

		} else {
			pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_count != SUCCESS) {
		pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
        return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	ON/OFF heat for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param 	State - select state heat (PBM_T1_ON_HEAT or PBM_T1_OFF_HEAT).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat, uint8_t State) {

	int8_t Error = ERROR_N;
	int8_t Error_count = 0;
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO); //TCA9548_CH2
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){

		if( pbm[PBM_number].Heat[Heat].PCA9534_ON_Heat_CMD != State ){
			pbm[PBM_number].PBM_save_conf_flag = 1;
		}

		pbm[PBM_number].Heat[Heat].PCA9534_ON_Heat_CMD = State;

		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

			if (State == PBM_T1_ON_HEAT) {
				Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_CMD);
			} else {
				Error = PCA9534_Reset_output_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_CMD);
			}
				if (Error == SUCCESS) {
				Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_CMD);
			}
			if (Error == SUCCESS) {
				Error =  PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_CMD, &data8);
			}
			if (Error != SUCCESS) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		if (Error == SUCCESS) {
			if (pbm[PBM_number].Heat[Heat].PCA9534_ON_Heat_CMD == data8 ) {
				pbm[PBM_number].Heat[Heat].Error_Heat = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;
			}
			pbm[PBM_number].Error_PCA9534 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Error_PCA9534 = ERROR;
			pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}


	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;

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
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@param 	State - select state heat (PBM_T1_ON_SHDN_CHRG or PBM_T1_OFF_SHDN_CHRG).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateEmergChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State) {

	int8_t Error = ERROR_N;
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO); //TCA9548_CH2
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){

		if( pbm[PBM_number].Branch[Branch].PCA9534_Emerg_Chrg != State ){
			pbm[PBM_number].PBM_save_conf_flag = 1;
		}

		pbm[PBM_number].Branch[Branch].PCA9534_Emerg_Chrg = State;

		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

			if (State == PBM_T1_ON_HEAT) {
				Error = PCA9534_Set_output_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_EmergChrg);
			} else {
				Error = PCA9534_Reset_output_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_EmergChrg);
			}

			if (Error == SUCCESS) {
				Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_EmergChrg);
			}

			if (Error == SUCCESS) {
				Error =  PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_EmergChrg, &data8);
			}

			if (Error != SUCCESS) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		if (Error == SUCCESS) {
			if (pbm[PBM_number].Branch[Branch].PCA9534_Emerg_Chrg == data8 ) {
				pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = SUCCESS;
			} else {
				pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = ERROR;
			}
			pbm[PBM_number].Error_PCA9534 = SUCCESS;
		} else {
			pbm[PBM_number].Error_PCA9534 = ERROR;
			pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Branch[Branch].Error_Emerg_Chrg = ERROR;

	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS) {
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	ON/OFF Charge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
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

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		if(pbm[PBM_number].Branch[Branch].ChgEnableBit != State){
			pbm[PBM_number].PBM_save_conf_flag = 1;
		}
		pbm[PBM_number].Branch[Branch].ChgEnableBit = State;
		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = MAX17320_Write_ON_OFF_CHRG_FET(I2Cx, State);
			LL_mDelay(10);
			Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
			pbm[PBM_number].Branch[Branch].ChgControlFlag = data_reg.CHGs;
			if (Error != SUCCESS) {
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
				count++;
			}
		}

		if (Error == SUCCESS) {
			if (pbm[PBM_number].Branch[Branch].ChgControlFlag == State) {
				pbm[PBM_number].Branch[Branch].Error_Charge = SUCCESS;
			} else {
				Error_count = Error_count + 1;
				pbm[PBM_number].Branch[Branch].Error_Charge = ERROR;
			}
			pbm[PBM_number].Branch[Branch].Error_MAX17320 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
			pbm[PBM_number].Branch[Branch].Error_Charge = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
		pbm[PBM_number].Branch[Branch].Error_Charge = ERROR;
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
	@param 	Branch - select Branch (PBM_BRANCH[1], PBM_BRANCH[2]).
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

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch); //TCA9548_CH6 TCA9548_CH5
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		if(pbm[PBM_number].Branch[Branch].DchgEnableBit != State){
			pbm[PBM_number].PBM_save_conf_flag = 1;
		}
		pbm[PBM_number].Branch[Branch].DchgEnableBit = State;
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = MAX17320_Write_ON_OFF_DCHRG_FET(I2Cx, State);
			LL_mDelay(10);
			Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
			pbm[PBM_number].Branch[Branch].DchgControlFlag = data_reg.DISs;
			if (Error != SUCCESS) {
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
				count++;
			}
		}

		if (Error == SUCCESS) {
			if (pbm[PBM_number].Branch[Branch].DchgControlFlag == State) {
				pbm[PBM_number].Branch[Branch].Error_Discharge = SUCCESS;
			} else {
				pbm[PBM_number].Branch[Branch].Error_Discharge = ERROR;
			}
			pbm[PBM_number].Branch[Branch].Error_MAX17320 = SUCCESS;
		} else {
			Error_count = Error_count + 1;
			pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
			pbm[PBM_number].Branch[Branch].Error_Discharge = ERROR;

			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Branch[Branch].Error_MAX17320 = ERROR;
		pbm[PBM_number].Branch[Branch].Error_Discharge = ERROR;
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
	@param 	Heat - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckOverHeat(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat) {

	uint8_t count = 0 , tempsense = 0;
	int8_t Error = 0;
	_PBM_T1_table pbm_table = { 0 };

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	for ( tempsense = 0; tempsense < PBM_T1_HEAT_TEMPSENS_QUANTITY; tempsense++ ){

		if ((pbm[PBM_number].Heat[Heat].Heat_TMP1075[tempsense] >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].Heat[Heat].PCA9534_Heat_State == 1)) {
			count++;
		}

		if (count >= 1) { // off heat if at least one temp sense dont work right
			PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_number, Heat, PBM_T1_OFF_HEAT);
			PBM_T1_Init_Heat_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_number, Heat, pbm_table.TempSens_Heat_Addr[tempsense]); //TCA9548_CH0
			pbm[PBM_number].Heat[Heat].PCA9534_ON_Heat_CMD = 1;
			pbm[PBM_number].Heat[Heat].Error_Heat = ERROR;

			Error = ERROR;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	if (Error != SUCCESS) {
		return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Check state charge/discharge keys for all PBM.
 	@param 	pbm[] - structure data for all PBM modules.
  	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
  	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckChargeDischargeState(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch) {

	int8_t Error_count = 0;

	if((pbm[PBM_number].Branch[Branch].ChgEnableBit == pbm[PBM_number].Branch[Branch].ChgControlFlag) && (pbm[PBM_number].Branch[Branch].Error_MAX17320 == SUCCESS)){

		pbm[PBM_number].Branch[Branch].Error_Charge = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Branch[Branch].Error_Charge = ERROR;
	}
	if((pbm[PBM_number].Branch[Branch].DchgEnableBit == pbm[PBM_number].Branch[Branch].DchgControlFlag) && (pbm[PBM_number].Branch[Branch].Error_MAX17320 == SUCCESS)){

		pbm[PBM_number].Branch[Branch].Error_Discharge = SUCCESS;
	} else {
		Error_count = Error_count + 1;
		pbm[PBM_number].Branch[Branch].Error_Discharge = ERROR;
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
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CorrectCapacity(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch) {

	uint16_t AbsoluteCapacity = 0;
	float Capacity = 0, Voltage = 0;
	float Error = 0;
	int8_t Error_i2c = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error_i2c != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_i2c = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);//TCA9548_CH6 TCA9548_CH5
		if( Error_i2c != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_i2c;

	if (Error == SUCCESS ){
		if (pbm[PBM_number].Branch[Branch].Error_MAX17320 == 0) {
			Capacity = ((float) (pbm[PBM_number].Branch[Branch].AbcoluteCapacity_mAh) / PBM_T1_MAX_BATT_CAP * 100.0f); // (3000.0 * 100.0) convert to %
			Voltage = ((float) (pbm[PBM_number].Branch[Branch].Voltage[0] + pbm[PBM_number].Branch[Branch].Voltage[1])) / 2.0f;
			Voltage = (float) (Voltage - 2500.0f) * 100.0f / 1650.0f; // in %
			Error = (float) (Voltage / Capacity);
			Error = (float) (Error - 1.0f) * 100.0f;  // in %
			if ((Error > 25) || (Error < -25) || (Capacity > 110)) {
				AbsoluteCapacity = (uint16_t) ((Voltage * PBM_T1_MAX_BATT_CAP) / 100);
				Error_i2c = MAX17320_WriteAccmCharge(I2Cx, AbsoluteCapacity, PBM_T1_MAX17320_R_SENSE);
			}
		}

	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error_i2c != SUCCESS) {
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
	uint8_t Branch_Number = 0;

	for (Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
		if ((pbm[PBM_number].Branch[Branch_Number].DchgControlFlag == 1) && (pbm[PBM_number].Branch[Branch_Number].Error_MAX17320 == 0)) {
			TotalAbsCapacity = TotalAbsCapacity + pbm[PBM_number].Branch[Branch_Number].AbcoluteCapacity_mAh;
			TotalRelCapacity = TotalRelCapacity + pbm[PBM_number].Branch[Branch_Number].RelativeCapacity_Perc;
		}
	}
	pbm[PBM_number].TotalRelativeCapacity_Perc = TotalRelCapacity / (Branch_Number + 1);
	pbm[PBM_number].TotalAbcoluteCapacity_mAh = TotalAbsCapacity;
}

/** @brief	Check level battery energy for selected PBM.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
*/
void PBM_T1_CheckLowLevelEnergy(_PBM_T1 pbm[], uint8_t PBM_number) {

	int16_t Voltage_Branch = 0;
	uint8_t Branch_Number = 0;
	uint8_t count_low_energy = 0;
	uint8_t count_reset_low_energy = 0;
	uint8_t count_zero_energy = 0;
	uint8_t count_reset_zero_energy = 0;

	for (Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){

		Voltage_Branch = pbm[PBM_number].Branch[Branch_Number].Voltage[0] + pbm[PBM_number].Branch[Branch_Number].Voltage[1];

		if ((Voltage_Branch <= PBM_T1_LOW_ENERGY_EDGE) && (pbm[PBM_number].Branch[Branch_Number].Error_MAX17320 == SUCCESS) &&
				(pbm[PBM_number].Branch[Branch_Number].DchgControlFlag == ENABLE)) {

			count_low_energy++;

		}else if( pbm[PBM_number].Low_Energy_Flag == 1 ){

		    if((Voltage_Branch >= PBM_T1_NORMAL_ENERGY_EDGE) || (pbm[PBM_number].Branch[Branch_Number].Error_MAX17320 == ERROR) ||
		    		(pbm[PBM_number].Branch[Branch_Number].DchgControlFlag == DISABLE)) {

		    	count_reset_low_energy++;
			}
		}
	}
	if (count_low_energy != 0){

		pbm[PBM_number].Low_Energy_Flag = 1;

	} else if (count_reset_low_energy == (PBM_T1_BRANCH_QUANTITY + 1)){

		pbm[PBM_number].Low_Energy_Flag = 0;
	}

	for (Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
		if( (Voltage_Branch <= PBM_T1_ZERO_ENERGY_EDGE ) && (pbm[PBM_number].Low_Energy_Flag == 1) ){

			count_zero_energy++;

		}else if(pbm[PBM_number].Low_Energy_Flag == 0) {

			count_reset_zero_energy++;

		}
	}
	if (count_zero_energy != 0){

		pbm[PBM_number].Zero_Energy_Flag = 1;

	} else if (count_reset_zero_energy == (PBM_T1_BRANCH_QUANTITY + 1)){

		pbm[PBM_number].Zero_Energy_Flag = 0;
	}
}


/** @brief	Check and reset max17320 if prequal charge timer error for selected branch.
 	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
*/
ErrorStatus PBM_T1_CheckResetPreqChrg(I2C_TypeDef *I2Cx,_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch) {

	float Error = 0;
	int8_t Error_i2c = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	int8_t Error_count = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error_i2c != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_i2c = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);//TCA9548_CH6 TCA9548_CH5
		if( Error_i2c != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_i2c;

	if (Error == SUCCESS ){
		if ((pbm[PBM_number].Branch[Branch].Error_MAX17320 == 0) && (pbm[PBM_number].Branch[Branch].PreqF == 1) && (pbm[PBM_number].Branch[Branch].Counter_Reset_PreqF < 5 )) {
			pbm[PBM_number].Branch[Branch].Counter_Reset_PreqF++;
			MAX17320_Write_FullReset (I2Cx);

		}

	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

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