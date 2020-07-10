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
void PBM_GetTelemetry(I2C_TypeDef *I2Cx, _PBM *pbm) {

	uint8_t i = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		PBM_ReadBatteryData(I2Cx, pbm, i);
		PBM_ReadTempSensor(I2Cx, pbm, i);
		PBM_ReadGPIO(I2Cx, pbm, i);
	}
	PBM_CalcTotalCapacityPBM(pbm);
	PBM_CheckCapacityPBM(I2Cx, pbm);
}
