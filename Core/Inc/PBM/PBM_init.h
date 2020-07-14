/*
 * PBM_init.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_INIT_H_
#define INC_PBM_INIT_H_

#include "PBM_struct.h"

ErrorStatus PBM_Init(_PBM pbm[]);

ErrorStatus PBM_Re_Init(_PBM pbm[]);

ErrorStatus PBM_TempSensorInit(_PBM pbm[], uint8_t PBM_number);

#endif /* INC_PBM_INIT_H_ */
