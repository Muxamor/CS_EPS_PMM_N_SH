#ifndef INC_PBM_T1_CONTROL_H_
#define INC_PBM_T1_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PBM_T1/pbm_T1_struct.h"

ErrorStatus PBM_T1_ReadStateHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number);

ErrorStatus PBM_T1_CheckStateCmdHeat(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number);

ErrorStatus PBM_T1_ReadStateEmergChrg(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number);

ErrorStatus PBM_T1_ReadHeatTempSensors( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number, uint8_t Sensor_number);

ErrorStatus PBM_T1_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t temp_number);

ErrorStatus PBM_T1_ReadHeatPwrMon(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number);

ErrorStatus PBM_T1_ReadBatteryTelemetry(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number);

ErrorStatus PBM_T1_SetStateHeat( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number, uint8_t State);

ErrorStatus PBM_T1_SetStateEmergChrg( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State);

ErrorStatus PBM_T1_SetStateChargeBranch( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State);

ErrorStatus PBM_T1_SetStateDischargeBranch( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State);

ErrorStatus PBM_T1_SetStateAutoCorrectCapacity(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint8_t State);

ErrorStatus PBM_T1_CheckOverHeat(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat_number);

ErrorStatus PBM_T1_CheckChargeDischargeState(_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number);

ErrorStatus PBM_T1_CorrectCapacity( I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number, uint16_t Max_cap);

void  PBM_T1_CalcTotalCapacity(_PBM_T1 pbm[], uint8_t PBM_number);

void PBM_T1_CheckLowLevelEnergy(_PBM_T1 pbm[], uint8_t PBM_number);

ErrorStatus PBM_T1_ResetBranch(I2C_TypeDef *I2Cx,_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch_number);

ErrorStatus PBM_T1_CheckResetPreqChrg(I2C_TypeDef *I2Cx,_PBM_T1 pbm[], uint8_t PBM_number, uint8_t Branch);

uint8_t PBM_T1_CheckSaveSetupFlag(_PBM_T1 pbm[]);

void PBM_T1_ClearSaveSetupFlag(_PBM_T1 pbm[]);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_control_H_ */
