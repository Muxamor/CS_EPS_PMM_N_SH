/*
 * PBM_init_IC.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_INIT_IC_H_
#define INC_PBM_INIT_IC_H_

ErrorStatus TMP1075_InitState(I2C_TypeDef *I2Cx, uint8_t AddrTMP1075);

ErrorStatus PCA9534_InitState(I2C_TypeDef *I2Cx, uint8_t AddrPCA9534);

#endif /* INC_PBM_INIT_IC_H_ */
