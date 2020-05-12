/*
 * DWT.h
 *
 *  Created on: May 2, 2020
 *      Author: ftor
 */

#ifndef INC_DWT_H_
#define INC_DWT_H_
#include "main.h"

void DWT_start();
void DWT_stop();
uint32_t DWT_get_clk_tick();
uint32_t DWT_get_milli_tick();
void DWT_reset_timer();
void DWT_delay(uint32_t delay);

#endif /* INC_DWT_H_ */
