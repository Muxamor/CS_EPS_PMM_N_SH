/*
 * TIM_delay.c
 *
 *  Created on: May 5, 2020
 *      Author: ftor
 */
#include "TIM_delay.h"

uint8_t TIM_finished = 0; // Global flag that indicates status of timer. 0 - still counting; 1 - finished


/** @brief	Start countdown in millis. After the specified number of milliseconds has elapsed,
 			an interrupt will occur with the raising of the global flag
    @param  delay - amount of millis untill raising interrupt.
*/
void TIM2_init(uint32_t delay){

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	TIM_finished = 0;

	TIM2->PSC = 39999;
	TIM2->ARR = 2 * delay;

	TIM2->CR1 |= TIM_CR1_OPM;

	TIM2->EGR |= TIM_EGR_UG;	//it
	TIM2->SR = 0;				//is
	TIM2->DIER |= TIM_DIER_UIE;	//very
	NVIC_EnableIRQ(TIM2_IRQn);	//important
								//to save
	TIM2->CR1 |= TIM_CR1_CEN;	//string sequence
}
