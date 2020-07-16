#include "main.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "DS2777.h"
#include "SetupPeriph.h"
#include "PBM_config.h"
#include "PBM_struct.h"
#include "PBM_control.h"

/** @brief	Read data for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @retval 	none
 */
ErrorStatus PBM_GetTelemetry(_PBM pbm[]) {

	uint8_t i = 0;
	int8_t Error = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		Error = PBM_ReadBatteryData(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_ReadTempSensors(PBM_I2C_PORT, pbm, i);
		Error = Error + PBM_ReadGPIO(PBM_I2C_PORT, pbm, i);
	}
	Error = Error + PBM_CalcTotalCapacityPBM(pbm);
	Error = Error + PBM_CheckCapacityPBM(PBM_I2C_PORT, pbm);
	Error = Error + PBM_CheckHeatOFF(pbm);
	Error = Error + PBM_CheckChargeDischargeState(pbm);

	if (Error == SUCCESS) {
		return SUCCESS;
	}else {
		return ERROR_N;
	}
}
