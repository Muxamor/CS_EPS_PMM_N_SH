#include "TMP1075.h"
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
		Error = Error + PBM_CheckCapacityPBM(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_CheckHeatOFF(pbm, i);
		Error = Error + PBM_CheckChargeDischargeState(pbm, i);

        PBM_CalcTotalCapacityPBM(pbm, i);
	}

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}
