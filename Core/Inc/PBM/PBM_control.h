
#ifndef INC_PBM_CONTROL_H_
#define INC_PBM_CONTROL_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PBM_ReadGPIO(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_ReadBatteryData(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);

ErrorStatus PBM_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);

ErrorStatus PBM_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);

ErrorStatus PBM_CheckHeatOFF(_PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_CheckChargeDischargeState(_PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_CheckCapacityPBM(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

void  PBM_CalcTotalCapacityPBM(_PBM pbm[], uint8_t PBM_number);

uint8_t PBM_CheckSaveSetupFlag(_PBM pbm[]);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_control_H_ */
