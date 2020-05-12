/*
 * TIM_delay.c
 *
 *  Created on: May 5, 2020
 *      Author: ftor
 */
#include "TIM_delay.h"

uint8_t TIM_finished = 0;

void TIM2_init(uint32_t delay){

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	TIM_finished = 0;

	TIM2->PSC = 39999;
	TIM2->ARR = 2 * delay;

	TIM2->CR1 |= TIM_CR1_OPM;

	TIM2->EGR |= TIM_EGR_UG;	// ОЧЕНЬ
	TIM2->SR = 0;				// ВАЖНО
	TIM2->DIER |= TIM_DIER_UIE; // ИМЕННО
	NVIC_EnableIRQ(TIM2_IRQn);  // ТАКАЯ
								// ПОСЛЕДОВАТЕЛЬНОСТЬ
	TIM2->CR1 |= TIM_CR1_CEN;   // КОМАНД!!! (иначе прерывание срабатывает сразу после его разрешения)
}

//void TIM2_IRQHandler(void){
//	TIM_finished = 1;
//	TIM2->SR &= ~TIM_SR_UIF;
//	TIM2->DIER &= ~TIM_DIER_UIE; // без этого зацикливается в прерывании
//}
