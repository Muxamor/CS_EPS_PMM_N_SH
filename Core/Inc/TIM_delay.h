/*
 * TIM_delay.h
 *
 *  Created on: May 5, 2020
 *      Author: ftor
 */

#ifndef INC_TIM_DELAY_H_
#define INC_TIM_DELAY_H_

#include "main.h"

ErrorStatus TIM_init(TIM_TypeDef *TIMx, uint32_t delay, uint8_t loop_mode);
ErrorStatus TIM_DeInit(TIM_TypeDef *TIMx);

void TIM2_IRQHandler();
void TIM4_IRQHandler();
void TIM5_IRQHandler();

#endif /* INC_TIM_DELAY_H_ */