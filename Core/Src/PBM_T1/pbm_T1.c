#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
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

	uint8_t i = 0;
	uint8_t Low_Energy_Flag_counter = 0;
	int8_t Error = SUCCESS;

	for (i = 0; i < PBM_T1_QUANTITY; i++) {
		Error = PBM_T1_ReadBatteryData(PBM_T1_I2C_PORT, pbm, i);
		Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, i);
		Error = Error + PBM_T1_ReadGPIO(PBM_T1_I2C_PORT, pbm, i);
		//Error = Error + PBM_CheckCapacity(PBM_T1_I2C_PORT, pbm, i);
		Error = Error + PBM_T1_CheckHeatOFF(pbm, i);
		Error = Error + PBM_T1_CheckChargeDischargeState(pbm, i);

        PBM_T1_CalcTotalCapacity(pbm, i);
        PBM_T1_CheckLevelEnergy(pbm, i);
	}

	for ( Low_Energy_Flag_counter = 0, i = 0; i < PBM_T1_QUANTITY; i++) {
		Low_Energy_Flag_counter = Low_Energy_Flag_counter + pbm[i].Low_Energy_Flag;
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
