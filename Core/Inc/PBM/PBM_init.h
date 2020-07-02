/*
 * PBM_init.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_INIT_H_
#define INC_PBM_INIT_H_

#include "PBM_struct.h"


void PBM_Init(PBM_Data_All *Data, uint8_t PBM);

void PBM_GPIOInit(PBM_Data_All *Data, uint8_t PBM);

void PBM_TempSensorInit(PBM_Data_All *Data, uint8_t PBM);


#endif /* INC_PBM_INIT_H_ */
