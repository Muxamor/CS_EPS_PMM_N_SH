/*
 * PBM_control.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_CONTROL_H_
#define INC_PBM_CONTROL_H_

ErrorStatus PBM_ReadGPIO(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_ReadTempSensors(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_ReadBatteryData(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number);

ErrorStatus PBM_SetStateHeatBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);

ErrorStatus PBM_SetStateChargeBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);

ErrorStatus PBM_SetStateDischargeBranch(I2C_TypeDef *I2Cx, _PBM pbm[], uint8_t PBM_number, uint8_t Branch, uint8_t State);

ErrorStatus PBM_CheckHeatOFF(_PBM pbm[]);

ErrorStatus PBM_CheckCapacityPBM(I2C_TypeDef *I2Cx, _PBM pbm[]);

ErrorStatus PBM_CalcTotalCapacityPBM(_PBM pbm[]);

#endif /* INC_PBM_control_H_ */
