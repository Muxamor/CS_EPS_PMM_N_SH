#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TCA9548.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_struct.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PBM_T1/pbm_T1.h"


/** @brief	Read data for all PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	*Data - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_T1_Get_Telemetry(_PBM_T1 pbm[]) {

	uint8_t PBM_Number = 0;
	uint8_t Low_Energy_Flag_counter = 0;
	int8_t Error = SUCCESS;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		Error = PBM_T1_ReadBatteryTelemetry(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_I2C_MUX_CH_Br1);
		Error = PBM_T1_ReadBatteryTelemetry(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_I2C_MUX_CH_Br2);
		Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_1, PBM_T1_I2C_MUX_CH_Temp1);
		Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_2, PBM_T1_I2C_MUX_CH_Temp1);
		Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_3, PBM_T1_I2C_MUX_CH_Temp2);
		Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_4, PBM_T1_I2C_MUX_CH_Temp2);
		Error = Error + PBM_T1_ReadStateTempSense(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_ALL, PBM_T1_I2C_MUX_CH_GPIO);
		Error = Error + PBM_T1_ReadStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_ALL, PBM_T1_I2C_MUX_CH_GPIO);
		Error = Error + PBM_T1_ReadStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_ALL, PBM_T1_I2C_MUX_CH_GPIO);
		//Error = Error + PBM_CheckCapacity(PBM_T1_I2C_PORT, pbm, PBM_Number);
		Error = Error + PBM_T1_CheckOverHeat(pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_I2C_MUX_CH_Temp1);
		Error = Error + PBM_T1_CheckOverHeat(pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_I2C_MUX_CH_Temp2);
		Error = Error + PBM_T1_CheckChargeDischargeState(pbm, PBM_Number);

        PBM_T1_CalcTotalCapacity(pbm, PBM_Number);
        PBM_T1_CheckLowLevelEnergy(pbm, PBM_Number);
	}

	for ( Low_Energy_Flag_counter = 0, PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {
		Low_Energy_Flag_counter = Low_Energy_Flag_counter + pbm[PBM_Number].Low_Energy_Flag;
	}

	if(Low_Energy_Flag_counter >= 1){
		PBM_Low_Energy_Set_pin();
	} else {
		PBM_Low_Energy_Reset_pin();
	}

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}
