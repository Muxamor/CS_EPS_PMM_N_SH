
#ifndef INC_PBM_T1_CONTROL_H_
#define INC_PBM_T1_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PBM_T1/pbm_T1_struct.h"

ErrorStatus PBM_T1_ReadGPIO(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number);
ErrorStatus PBM_T1_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number);
ErrorStatus PBM_T1_ReadBatteryData(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number);
ErrorStatus PBM_T1_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);
ErrorStatus PBM_T1_SetStateShdnChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);
ErrorStatus PBM_T1_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);
ErrorStatus PBM_T1_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);
ErrorStatus PBM_T1_CheckHeatOFF(_PBM_T1 pbm[], uint8_t PBM_number);
ErrorStatus PBM_T1_CheckChargeDischargeState(_PBM_T1 pbm[], uint8_t PBM_number);
ErrorStatus PBM_T1_CheckCapacity(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number);
void  PBM_T1_CalcTotalCapacity(_PBM_T1 pbm[], uint8_t PBM_number);
void PBM_T1_CheckLevelEnergy(_PBM_T1 pbm[], uint8_t PBM_number);
uint8_t PBM_T1_CheckSaveSetupFlag(_PBM_T1 pbm[]);
void PBM_T1_ClearSaveSetupFlag(_PBM_T1 pbm[]);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_control_H_ */
