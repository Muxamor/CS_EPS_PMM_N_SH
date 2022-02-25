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
#include "PBM_T1/pbm_T1_control.h"
#include "Error_Handler.h"

/** @brief	Read state temperature sensors from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Heat_number- select heat number (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

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
			if (PCA9534_conf_IO_dir_input(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_State) == SUCCESS) {
				Error = PCA9534_conf_IO_pol_normal(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_State);
			}

			if (Error == SUCCESS) {
				Error = PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_State, &data8);
			}

			if (Error != SUCCESS) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if(Error_I2C_MUX != SUCCESS) {
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {
		pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_State = !(data8);
		pbm[PBM_number].Error_PCA9534 = SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	Read state heat from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Heat_number - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_CheckStateCmdHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

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

			Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_CMD);

			if (Error == SUCCESS) {
				Error = PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_Heat_CMD, &data8);
			}

			if (Error != SUCCESS) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {
		if ( pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_CMD == (data8)) {
			pbm[PBM_number].Heat[Heat_number].Error_Heat = SUCCESS;
		} else {
		   pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;
		}
		pbm[PBM_number].Error_PCA9534 = SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	Read state emergency charge from PCA9534 for selected PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm[] - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
    @param 	Branch_number - select Branch number (PBM_BRANCH_1, PBM_BRANCH_2).
    @retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadStateEmergChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number) {

	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

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
			Error = PCA9534_conf_IO_dir_output(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_EmergChrg);

			if (Error == SUCCESS) {
				Error = PCA9534_read_input_pin(I2Cx, pbm_table.GPIO_Addr, pbm_table.GPIO_Pin_EmergChrg, &data8);
			}

			if (Error != SUCCESS) {
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = ERROR;
        #ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error == SUCCESS) {

		if ( pbm[PBM_number].Branch[Branch_number].PCA9534_Emerg_Chrg_Cmd == (data8)) {
			pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = SUCCESS;
		} else {
			pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = ERROR;
		}
		pbm[PBM_number].Error_PCA9534 = SUCCESS;

	}else{
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	Read temperature heat TMP1075 sensor for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat_number - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  Sensor_number- I2C temp. sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadHeatTempSensors( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number, uint8_t Sensor_number) {

	int8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

	//Enable I2C MUX channel
	while (( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN )){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS){
		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {

			Error = TMP1075_read_int8_temperature(I2Cx, pbm_table.TempSens_Heat_Addr[Sensor_number], &data8);

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
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error != SUCCESS){
		pbm[PBM_number].Heat[Heat_number].Heat_TMP1075[Sensor_number] = 0x7F;
		pbm[PBM_number].Heat[Heat_number].Error_Heat_TMP1075[Sensor_number] = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Heat[Heat_number].Heat_TMP1075[Sensor_number] = data8;
		pbm[PBM_number].Heat[Heat_number].Error_Heat_TMP1075[Sensor_number] = SUCCESS; //No error
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
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


	if( Error == ERROR_N){
		pbm[PBM_number].TMP1075_Temp[temp_number] = 0x7F;
		pbm[PBM_number].Error_TMP1075[temp_number] = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}else{
		pbm[PBM_number].TMP1075_Temp[temp_number] = data8;
		pbm[PBM_number].Error_TMP1075[temp_number] = SUCCESS; //No error
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX == ERROR_N)) {
		return ERROR_N;
	}
    return SUCCESS;
}*/

/** @brief	Read data from power monitor INA238 for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat_number - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@retval ErrorStatus
 */
ErrorStatus PBM_T1_ReadHeatPwrMon(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number) {

	int16_t current = 0;
	uint16_t bus_voltage = 0, power = 0, alert_flags = 0;
	uint8_t count = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

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
	if( Error_I2C_MUX != SUCCESS){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if (Error != SUCCESS){
		pbm[PBM_number].Heat[Heat_number].HeatCurrent = 0;
		pbm[PBM_number].Heat[Heat_number].HeatVoltage = 0;
		pbm[PBM_number].Heat[Heat_number].HeatPower = 0;
		pbm[PBM_number].Heat[Heat_number].Error_INA238 = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{

		pbm[PBM_number].Heat[Heat_number].HeatVoltage = bus_voltage;
		pbm[PBM_number].Heat[Heat_number].HeatPower = power;
		if(((alert_flags & 0x0200) >> 9) == 1){
			pbm[PBM_number].Heat[Heat_number].HeatCurrent = -255;
			pbm[PBM_number].Heat[Heat_number].Error_INA238 = ERROR;
		} else {
			pbm[PBM_number].Heat[Heat_number].HeatCurrent = current;
			pbm[PBM_number].Heat[Heat_number].Error_INA238 = SUCCESS; //No error
		}
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	Read data from two MAX17320 for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_ReadBatteryTelemetry(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number) {

	int8_t Error = ERROR_N;
	uint8_t count = 0;
	MAX17320_BranchData Struct = {0};
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

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
			Error = MAX17320_Read_Br_ProtStatus_Reg (I2Cx, &Struct);
			Error = Error + MAX17320_Read_Balancing_Reg (I2Cx, &Struct, PBM_T1_BRANCH_BAT_QUANTITY);
			Error = Error + MAX17320_Read_Br_Voltage_mV (I2Cx, &Struct, PBM_T1_BRANCH_BAT_QUANTITY);
			Error = Error + MAX17320_Read_Br_Current_mA (I2Cx, &Struct, PBM_T1_MAX17320_R_SENSE);
			Error = Error + MAX17320_Read_Br_Pwr_mW (I2Cx, &Struct, PBM_T1_MAX17320_R_SENSE);
			Error = Error + MAX17320_Read_Br_Temp_Dg (I2Cx, &Struct, MAX17320_0_termistors);
			Error = Error + MAX17320_Read_Br_GaugeOut_mAh (I2Cx, &Struct, PBM_T1_MAX17320_R_SENSE);

			if (Error != SUCCESS){
				Error = ERROR_N;
				count++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}

		pbm[PBM_number].Branch[Branch_number].OVP = Struct.OVP;
		pbm[PBM_number].Branch[Branch_number].UVP = Struct.UVP;
		pbm[PBM_number].Branch[Branch_number].OCCP = Struct.OCCP;
		pbm[PBM_number].Branch[Branch_number].ODCP = Struct.ODCP;
		pbm[PBM_number].Branch[Branch_number].PreqF = Struct.PreqF;
		pbm[PBM_number].Branch[Branch_number].LDet = Struct.LDet;
		pbm[PBM_number].Branch[Branch_number].ChgControlFlag = Struct.ChgControlFlag;
		pbm[PBM_number].Branch[Branch_number].DchgControlFlag = Struct.DchgControlFlag;
		pbm[PBM_number].Branch[Branch_number].OC = Struct.OC;
		pbm[PBM_number].Branch[Branch_number].OD = Struct.OD;
		pbm[PBM_number].Branch[Branch_number].SC = Struct.SC;
		if(PBM_T1_BRANCH_BAT_QUANTITY == 1){
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 2){
			pbm[PBM_number].Branch[Branch_number].Voltage[1] = Struct.Cell4_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			pbm[PBM_number].Branch[Branch_number].BalCell[1] = Struct.BalCell4;
			pbm[PBM_number].Branch[Branch_number].BalCell[0] = Struct.BalCell1;
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 3){
			pbm[PBM_number].Branch[Branch_number].Voltage[2] = Struct.Cell4_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[1] = Struct.Cell3_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			pbm[PBM_number].Branch[Branch_number].BalCell[2] = Struct.BalCell4;
			pbm[PBM_number].Branch[Branch_number].BalCell[1] = Struct.BalCell3;
			pbm[PBM_number].Branch[Branch_number].BalCell[0] = Struct.BalCell1;
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 4){
			pbm[PBM_number].Branch[Branch_number].Voltage[3] = Struct.Cell4_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[2] = Struct.Cell2_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[1] = Struct.Cell3_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			pbm[PBM_number].Branch[Branch_number].BalCell[3] = Struct.BalCell4;
			pbm[PBM_number].Branch[Branch_number].BalCell[2] = Struct.BalCell2;
			pbm[PBM_number].Branch[Branch_number].BalCell[1] = Struct.BalCell3;
			pbm[PBM_number].Branch[Branch_number].BalCell[0] = Struct.BalCell1;
		}
		pbm[PBM_number].Branch[Branch_number].MaxVCELL = Struct.MaxVCELL_mV;
		pbm[PBM_number].Branch[Branch_number].MinVCELL = Struct.MinVCELL_mV;
		pbm[PBM_number].Branch[Branch_number].Current = Struct.Curr_mA;
		pbm[PBM_number].Branch[Branch_number].AverageCurrent = Struct.AvgCurr_mA;
		pbm[PBM_number].Branch[Branch_number].MaxCurr = Struct.MaxCurr_mA;
		pbm[PBM_number].Branch[Branch_number].MinCurr = Struct.MinCurr_mA;
		pbm[PBM_number].Branch[Branch_number].Power = Struct.Pwr_mW;
		pbm[PBM_number].Branch[Branch_number].AveragePower = Struct.AvgPwr_mW;
		pbm[PBM_number].Branch[Branch_number].Temperature = Struct.DieTemp_Dg;
		pbm[PBM_number].Branch[Branch_number].AbcoluteCapacity_mAh = Struct.RepCap_mAh;
		pbm[PBM_number].Branch[Branch_number].RelativeCapacity_Perc = Struct.RepSOC_Per;
		pbm[PBM_number].Branch[Branch_number].AgeScalar = Struct.Age_Dg;
		pbm[PBM_number].Branch[Branch_number].CycleCounter = Struct.Cycles_Cn;
		pbm[PBM_number].Branch[Branch_number].RCell = Struct.RCell_mOhm;
		pbm[PBM_number].Branch[Branch_number].LeakCurr = Struct.LeakCurr_mA;
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS) {
	    pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = ERROR;
		#ifdef DEBUGprintf
	        Error_Handler();
		#endif
	} else {
	    pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	Erase data for all PBM.
	@param 	pbm[] - structure data for all PBM modules.
	@retval 	ErrorStatus
 */
void PBM_T1_EraseData(_PBM_T1 pbm[]) {

	uint8_t PBM_number = 0, Branch_number = 0, Heat_number = 0, Heat_TempSens_number = 0;

	for (PBM_number = 0; PBM_number < PBM_T1_QUANTITY; PBM_number++) {

		pbm[PBM_number].TotalAbcoluteCapacity_mAh = 0;
		pbm[PBM_number].TotalRelativeCapacity_Perc = 0;

		for( Branch_number = 0; Branch_number < PBM_T1_BRANCH_QUANTITY; Branch_number++){

			if(PBM_T1_BRANCH_BAT_QUANTITY == 1){
				pbm[PBM_number].Branch[Branch_number].Voltage[0] = 0;
			} else if(PBM_T1_BRANCH_BAT_QUANTITY == 2){
				pbm[PBM_number].Branch[Branch_number].Voltage[1] = 0;
				pbm[PBM_number].Branch[Branch_number].Voltage[0] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[1] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[0] = 0;
			} else if(PBM_T1_BRANCH_BAT_QUANTITY == 3){
				pbm[PBM_number].Branch[Branch_number].Voltage[2] = 0;
				pbm[PBM_number].Branch[Branch_number].Voltage[1] = 0;
				pbm[PBM_number].Branch[Branch_number].Voltage[0] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[2] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[1] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[0] = 0;
			} else if(PBM_T1_BRANCH_BAT_QUANTITY == 4){
				pbm[PBM_number].Branch[Branch_number].Voltage[3] = 0;
				pbm[PBM_number].Branch[Branch_number].Voltage[2] = 0;
				pbm[PBM_number].Branch[Branch_number].Voltage[1] = 0;
				pbm[PBM_number].Branch[Branch_number].Voltage[0] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[3] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[2] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[1] = 0;
				pbm[PBM_number].Branch[Branch_number].BalCell[0] = 0;
			}

			pbm[PBM_number].Branch[Branch_number].Current = 0;
			pbm[PBM_number].Branch[Branch_number].AverageCurrent = 0;
			pbm[PBM_number].Branch[Branch_number].Power = 0;
			pbm[PBM_number].Branch[Branch_number].AveragePower = 0;
			pbm[PBM_number].Branch[Branch_number].Temperature = 0;
			pbm[PBM_number].Branch[Branch_number].AbcoluteCapacity_mAh = 0;
			pbm[PBM_number].Branch[Branch_number].RelativeCapacity_Perc = 0;

		}

		for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){

			pbm[PBM_number].Heat[Heat_number].HeatCurrent = 0;
			pbm[PBM_number].Heat[Heat_number].HeatVoltage = 0;
			pbm[PBM_number].Heat[Heat_number].HeatPower = 0;

			for( Heat_TempSens_number = 0; Heat_TempSens_number < PBM_T1_HEAT_TEMPSENS_QUANTITY; Heat_TempSens_number++){

				pbm[PBM_number].Heat[Heat_number].Heat_TMP1075[Heat_TempSens_number] = 0;
			}
		}
	}
}

/** @brief	ON/OFF heat for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat_number - select heat number (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param 	State - select state heat (PBM_T1_ON_HEAT or PBM_T1_OFF_HEAT).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateHeat( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number, uint8_t State) {

	int8_t Error = ERROR_N;
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO); //TCA9548_CH2
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if( pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_CMD != State ){
		pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_CMD = State;
		pbm[PBM_number].PBM_save_conf_flag = 1;
	}

	if (Error == SUCCESS ){

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
	}


	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS ) {
	    pbm[PBM_number].Error_PCA9534 = ERROR;
	    pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;
		#ifdef DEBUGprintf
	        Error_Handler();
		#endif

	} else {
	    if ( pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_CMD == data8 ) {
	        pbm[PBM_number].Heat[Heat_number].Error_Heat = SUCCESS;
	    } else {
	        pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;
	    }
	    pbm[PBM_number].Error_PCA9534 = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	ON/OFF emergency charge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@param 	State - select state heat (PBM_T1_ON_SHDN_CHRG or PBM_T1_OFF_SHDN_CHRG).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateEmergChrg( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State) {

	int8_t Error = ERROR_N;
	uint8_t data8 = 0;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO); //TCA9548_CH2
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if( pbm[PBM_number].Branch[Branch_number].PCA9534_Emerg_Chrg_Cmd != State ){
		pbm[PBM_number].Branch[Branch_number].PCA9534_Emerg_Chrg_Cmd = State;
		pbm[PBM_number].PBM_save_conf_flag = 1;
	}

	if (Error == SUCCESS ){

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
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_GPIO);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_PCA9534 = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif

	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS) {
	    pbm[PBM_number].Error_PCA9534 = ERROR;
	    pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = ERROR;
		#ifdef DEBUGprintf
	        Error_Handler();
		#endif

	} else {
	    if ( pbm[PBM_number].Branch[Branch_number].PCA9534_Emerg_Chrg_Cmd == data8 ) {
	        pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = SUCCESS;
	    } else {
	        pbm[PBM_number].Branch[Branch_number].Error_Emerg_Chrg = ERROR;
	    }
	    pbm[PBM_number].Error_PCA9534 = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}


/** @brief	ON/OFF Charge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@param 	State - select state charge (PBM_T1_ON_CHARGE or PBM_T1_OFF_CHARGE).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateChargeBranch( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State) {

	int8_t Error = ERROR_N;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = 0;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };
	MAX17320_RegDataEEPROME data_reg;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

	//Enable I2C MUX channel
	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if( pbm[PBM_number].Branch[Branch_number].ChgEnableCmd != State){
	    pbm[PBM_number].Branch[Branch_number].ChgEnableCmd = State;
	    pbm[PBM_number].PBM_save_conf_flag = 1;
	}

	if (Error == SUCCESS ){

		Error = ERROR_N;
		while ((Error != SUCCESS) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = MAX17320_Write_ON_OFF_CHRG_FET(I2Cx, State);
			LL_mDelay(10);
			Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
			pbm[PBM_number].Branch[Branch_number].ChgControlFlag = data_reg.CHGs;
			if (Error != SUCCESS) {
			    Error = ERROR_N;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
				count++;
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_Charge = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS ) {
	    pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = ERROR;
	    pbm[PBM_number].Branch[Branch_number].Error_Charge = ERROR;
		#ifdef DEBUGprintf
	        Error_Handler();
		#endif

	} else {
	    if ( pbm[PBM_number].Branch[Branch_number].ChgControlFlag == State) {
	        pbm[PBM_number].Branch[Branch_number].Error_Charge = SUCCESS;
	    } else {
	        pbm[PBM_number].Branch[Branch_number].Error_Charge = ERROR;
	    }
	    pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	ON/OFF Discharge for selected branch for selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch (PBM_BRANCH[1], PBM_BRANCH[2]).
	@param 	State - select state charge (PBM_ON_DISCHARGE or PBM_OFF_DISCHARGE).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateDischargeBranch( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State) {

	int8_t Error = ERROR_N;
	uint8_t count = 0;
	int8_t Error_I2C_MUX = 0;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };
	MAX17320_RegDataEEPROME data_reg;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch); //TCA9548_CH6 TCA9548_CH5
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if( pbm[PBM_number].Branch[Branch_number].DchgEnableCmd != State){
	    pbm[PBM_number].Branch[Branch_number].DchgEnableCmd = State;
	    pbm[PBM_number].PBM_save_conf_flag = 1;
	}

	if (Error == SUCCESS ){

		Error = ERROR_N;
		while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
			Error = MAX17320_Write_ON_OFF_DCHRG_FET(I2Cx, State);
			LL_mDelay(10);
			Error = Error + MAX17320_Read_HProtCfg2Reg (I2Cx, &data_reg);
			pbm[PBM_number].Branch[Branch_number].DchgControlFlag = data_reg.DISs;
			if (Error != SUCCESS) {
			    Error = ERROR_N;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
				count++;
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX != SUCCESS){
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = ERROR;
		pbm[PBM_number].Branch[Branch_number].Error_Discharge = ERROR;
		#ifdef DEBUGprintf
		    Error_Handler();
		#endif
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if (Error != SUCCESS) {
	    pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = ERROR;
	    pbm[PBM_number].Branch[Branch_number].Error_Discharge = ERROR;
		#ifdef DEBUGprintf
	        Error_Handler();
		#endif

	} else {
	    if ( pbm[PBM_number].Branch[Branch_number].DchgControlFlag == State) {
	        pbm[PBM_number].Branch[Branch_number].Error_Discharge = SUCCESS;
	    } else {
	        pbm[PBM_number].Branch[Branch_number].Error_Discharge = ERROR;
	    }
	    pbm[PBM_number].Branch[Branch_number].Error_MAX17320 = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	ON/OFF auto correction capacity selected branch for selected PBM.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@param 	State - select state heat (PBM_T1_ON_SHDN_CHRG or PBM_T1_OFF_SHDN_CHRG).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_SetStateAutoCorrectCapacity(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State) {

	int8_t Error = 0;

	if( pbm[PBM_number].Branch[Branch_number].Auto_Corr_Capacity_Cmd != State ){
		pbm[PBM_number].Branch[Branch_number].Auto_Corr_Capacity_Cmd = State;
		pbm[PBM_number].PBM_save_conf_flag = 1;
	}

	Error = PBM_T1_CorrectCapacity(I2Cx, pbm, PBM_number, Branch_number, PBM_T1_MAX_BATT_CAP);

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}


/** @brief	Check auto heat OFF.
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Heat_number - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckOverHeat(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number) {

	uint8_t count = 0 , tempsense = 0;
	int8_t Error = 0;
	_PBM_T1_table pbm_table = { 0 };

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat_number);

	for ( tempsense = 0; tempsense < PBM_T1_HEAT_TEMPSENS_QUANTITY; tempsense++ ){

		if ((pbm[PBM_number].Heat[Heat_number].Heat_TMP1075[tempsense] >= PBM_T1_TMP1075_Hi_Limit) && (pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_State == 1)) {
			count++;
		}

		if (count >= 1) { // off heat if at least one temp sense dont work right
			PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_number, Heat_number, PBM_T1_OFF_HEAT);
			PBM_T1_Init_Heat_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_number, Heat_number, pbm_table.TempSens_Heat_Addr[tempsense]); //TCA9548_CH0
			pbm[PBM_number].Heat[Heat_number].PCA9534_Heat_CMD = 1;
			pbm[PBM_number].Heat[Heat_number].Error_Heat = ERROR;

			Error = ERROR_N;
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
  	@param 	Branch_number - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
	@retval 	ErrorStatus
 */
ErrorStatus PBM_T1_CheckChargeDischargeState(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number) {

	uint8_t Error = 0;

	if((pbm[PBM_number].Branch[Branch_number].ChgEnableCmd == pbm[PBM_number].Branch[Branch_number].ChgControlFlag) && (pbm[PBM_number].Branch[Branch_number].Error_MAX17320 == SUCCESS)){

		pbm[PBM_number].Branch[Branch_number].Error_Charge = SUCCESS;
	} else {
		Error = Error + 1;
		pbm[PBM_number].Branch[Branch_number].Error_Charge = ERROR;
	}
	if((pbm[PBM_number].Branch[Branch_number].DchgEnableCmd == pbm[PBM_number].Branch[Branch_number].DchgControlFlag) && (pbm[PBM_number].Branch[Branch_number].Error_MAX17320 == SUCCESS)){

		pbm[PBM_number].Branch[Branch_number].Error_Discharge = SUCCESS;
	} else {
		Error = Error + 1;
		pbm[PBM_number].Branch[Branch_number].Error_Discharge = ERROR;
	}

	if (Error != SUCCESS) {
	    return ERROR_N;
	}
	return SUCCESS;
}

/** @brief	Check and correct capacity selected PBM.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch_number (PBM_BRANCH_1, PBM_BRANCH_2).
	@param 	Max_cap - Maximum capacity of battery cell in mAh.
	@retval ErrorStatus
 */
ErrorStatus PBM_T1_CorrectCapacity( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint16_t Max_cap) {

	uint16_t AbsoluteCapacity = 0;
	float Voltage = 0;
	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	MAX17320_BranchData Struct = {0};
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);//TCA9548_CH6 TCA9548_CH5
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = MAX17320_Read_Br_Voltage_mV (I2Cx, &Struct, PBM_T1_BRANCH_BAT_QUANTITY);
		if(PBM_T1_BRANCH_BAT_QUANTITY == 1){
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			Voltage = (float) (pbm[PBM_number].Branch[Branch_number].Voltage[0]);
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 2){
			pbm[PBM_number].Branch[Branch_number].Voltage[1] = Struct.Cell4_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			Voltage = ((float) (pbm[PBM_number].Branch[Branch_number].Voltage[0] + pbm[PBM_number].Branch[Branch_number].Voltage[1])) / 2.0f;
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 3){
			pbm[PBM_number].Branch[Branch_number].Voltage[2] = Struct.Cell4_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[1] = Struct.Cell2_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			Voltage = ((float) (pbm[PBM_number].Branch[Branch_number].Voltage[0] + pbm[PBM_number].Branch[Branch_number].Voltage[1] +
					pbm[PBM_number].Branch[Branch_number].Voltage[2])) / 3.0f;
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 4){
			pbm[PBM_number].Branch[Branch_number].Voltage[3] = Struct.Cell4_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[2] = Struct.Cell3_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[1] = Struct.Cell2_mV;
			pbm[PBM_number].Branch[Branch_number].Voltage[0] = Struct.Cell1_mV;
			Voltage = ((float) (pbm[PBM_number].Branch[Branch_number].Voltage[0] + pbm[PBM_number].Branch[Branch_number].Voltage[1] +
								pbm[PBM_number].Branch[Branch_number].Voltage[2] + pbm[PBM_number].Branch[Branch_number].Voltage[3])) / 4.0f;
		}

		Voltage = (float) (Voltage - 2500.0f) * 100.0f / 1650.0f; // in %
		AbsoluteCapacity = (uint16_t) ((Voltage * Max_cap) / 100);
		Error = Error + MAX17320_WriteAccmCharge(I2Cx, AbsoluteCapacity, PBM_T1_MAX17320_R_SENSE);
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
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
	    if ((pbm[PBM_number].Branch[Branch_Number].DchgControlFlag == 1) && (pbm[PBM_number].Branch[Branch_Number].Error_MAX17320 == SUCCESS)) {
			TotalAbsCapacity = TotalAbsCapacity + pbm[PBM_number].Branch[Branch_Number].AbcoluteCapacity_mAh;
			TotalRelCapacity = TotalRelCapacity + pbm[PBM_number].Branch[Branch_Number].RelativeCapacity_Perc;
		}
	}
	pbm[PBM_number].TotalRelativeCapacity_Perc = TotalRelCapacity / PBM_T1_BRANCH_QUANTITY;
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

		if(PBM_T1_BRANCH_BAT_QUANTITY == 1){
			Voltage_Branch = pbm[PBM_number].Branch[Branch_Number].Voltage[0];
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 2){
			Voltage_Branch = pbm[PBM_number].Branch[Branch_Number].Voltage[0] + pbm[PBM_number].Branch[Branch_Number].Voltage[1];
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 3){
			Voltage_Branch = pbm[PBM_number].Branch[Branch_Number].Voltage[0] + pbm[PBM_number].Branch[Branch_Number].Voltage[1] +
					pbm[PBM_number].Branch[Branch_Number].Voltage[2];
		} else if(PBM_T1_BRANCH_BAT_QUANTITY == 4){
			Voltage_Branch = pbm[PBM_number].Branch[Branch_Number].Voltage[0] + pbm[PBM_number].Branch[Branch_Number].Voltage[1] +
					pbm[PBM_number].Branch[Branch_Number].Voltage[2] + pbm[PBM_number].Branch[Branch_Number].Voltage[3];
		}

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

	} else if (count_reset_low_energy == PBM_T1_BRANCH_QUANTITY){
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

	} else if (count_reset_zero_energy == PBM_T1_BRANCH_QUANTITY){
		pbm[PBM_number].Zero_Energy_Flag = 0;
	}
}

/** @brief	Reset MAX17320  for selected branch.
 	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch_number - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
*/
ErrorStatus PBM_T1_ResetBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number) {

	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch_number, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);//TCA9548_CH6 TCA9548_CH5
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		Error = MAX17320_Write_FullReset (I2Cx);
		LL_mDelay(200);
		Error = Error + PBM_T1_CorrectCapacity(I2Cx, pbm, PBM_number, Branch_number, PBM_T1_MAX_BATT_CAP);
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief	Check and reset MAX17320 if prequal charge timer error for selected branch.
 	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	pbm[] - structure data for all PBM modules.
	@param 	PBM_number - select PBM (PBM_T1_1, PBM_T1_2, PBM_T1_3, PBM_T1_4).
	@param 	Branch - select Branch (PBM_BRANCH_1, PBM_BRANCH_2).
*/
ErrorStatus PBM_T1_CheckResetPreqChrg(I2C_TypeDef *I2Cx,_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch) {

	int8_t Error = ERROR_N;
	int8_t Error_I2C_MUX = ERROR_N;
	uint8_t i = 0;
	_PBM_T1_table pbm_table = { 0 };

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, Branch, 0);

	//Enable I2C MUX channel

	while( ( Error != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);//TCA9548_CH6 TCA9548_CH5
		if( Error != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error;

	if (Error == SUCCESS ){
		if ((pbm[PBM_number].Branch[Branch].Error_MAX17320 == 0) && (pbm[PBM_number].Branch[Branch].PreqF == 1) && (pbm[PBM_number].Branch[Branch].Counter_Reset_PreqF < 5 )) {
			pbm[PBM_number].Branch[Branch].Counter_Reset_PreqF++;

			MAX17320_Write_FullReset (I2Cx);
		}

	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Branch);

	//Parse error
	if( Error_I2C_MUX != SUCCESS ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	if ((Error != SUCCESS) || (Error_I2C_MUX != SUCCESS)) {
		return ERROR_N;
	}
    return SUCCESS;
}

/** @brief    Check flag to save setting.
    @param     pbm[] - structure data for all PBM modules.
    @retval    1- need to save, 0 - not need save
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
