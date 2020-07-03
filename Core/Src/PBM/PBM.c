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
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_GetTelemetry(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM) {

	PBM_ReadBatteryData(I2Cx, Data, PBM);
	PBM_ReadTempSensor(I2Cx, Data, PBM);
	PBM_ReadGPIO(I2Cx, Data, PBM);
	PBM_CalcTotalCapacityPBM(Data);
	PBM_CheckCapacityPBM(I2Cx, Data);
}
