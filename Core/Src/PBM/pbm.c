#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "PBM/pbm_config.h"
#include "PBM/pbm_struct.h"
#include "PBM/pbm_control.h"
#include "PBM/pbm.h"


/** @brief	Read data for all PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	*Data - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_Get_Telemetry(_PBM pbm[]) {

	uint8_t i = 0;
	int8_t Error = SUCCESS;

	for (i = 0; i < PBM_QUANTITY; i++) {
		Error = PBM_ReadBatteryData(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_ReadTempSensors(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_ReadGPIO(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_CheckCapacity(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_CheckHeatOFF(pbm, i);
		Error = Error + PBM_CheckChargeDischargeState(pbm, i);

        PBM_CalcTotalCapacity(pbm, i);
        PBM_CheckLevelEnergy(pbm, i);
	}

	if((pbm[0].Low_Energy_Flag + pbm[1].Low_Energy_Flag + pbm[2].Low_Energy_Flag) >= 1){
		PBM_Low_Energy_Set_pin();
	} else {
		PBM_Low_Energy_Reset_pin();
	}

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}
