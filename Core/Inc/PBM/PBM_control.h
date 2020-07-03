/*
 * PBM_control.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_CONTROL_H_
#define INC_PBM_CONTROL_H_

void PBM_ReadGPIO(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM);

void PBM_ReadTempSensor(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM);

void PBM_ReadBatteryData(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM);

void PBM_SetStateHeatBranch(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM, uint8_t Branch, uint8_t State);

void PBM_SetStateChargeBranch(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM, uint8_t Branch, uint8_t State);

void PBM_SetStateDischargeBranch(I2C_TypeDef *I2Cx, PBM_Data_All *Data, uint8_t PBM, uint8_t Branch, uint8_t State);

void PBM_CheckHeatOFF(PBM_Data_All *Data);

void PBM_CheckCapacityPBM(I2C_TypeDef *I2Cx, PBM_Data_All *Data);

void PBM_CalcTotalCapacityPBM(PBM_Data_All *Data);

#endif /* INC_PBM_control_H_ */
