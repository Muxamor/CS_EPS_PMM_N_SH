/*
 * PBM_init.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_INIT_H_
#define INC_PBM_INIT_H_

#include "PBM_struct.h"


void PBM_Init(_PBM *pbm);

void PBM_TempSensorInit(_PBM *pbm, uint8_t PBM);

void PBM_GPIO_Table(uint8_t PBM, uint8_t *Addr);

void PBM_Temp_Table(uint8_t PBM, uint8_t number, uint8_t *Addr);

void PBM_DS2777_Table(uint8_t PBM, uint8_t branch, uint8_t *Addr);

#endif /* INC_PBM_INIT_H_ */
