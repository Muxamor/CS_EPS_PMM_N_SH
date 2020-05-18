/*
 * TIM_delay.h
 *
 *  Created on: May 5, 2020
 *      Author: ftor
 */

#ifndef INC_TIM_DELAY_H_
#define INC_TIM_DELAY_H_

#include "main.h"

uint8_t TIM_finished;

void TIM2_init(uint32_t delay);
void TIM2_IRQHandler();


#endif /* INC_TIM_DELAY_H_ */
