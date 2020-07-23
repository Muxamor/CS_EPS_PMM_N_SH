#include "stm32l4xx_ll_gpio.h"
#include "TMP1075.h"
#include "SetupPeriph.h"
#include "pbm_config.h"
#include "pbm_struct.h"
#include "pbm_control.h"
#include "pbm.h"


/** @brief	Read data for all PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	*Data - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_GetTelemetry(_PBM pbm[]) {

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

	if((pbm[0].PBM_Low_Energy_Flag + pbm[1].PBM_Low_Energy_Flag + pbm[2].PBM_Low_Energy_Flag) >= 1){
		PBM_Set_Energy_Low_level_pin();
	} else {
		PBM_Reset_Energy_Low_level_pin();
	}

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}
