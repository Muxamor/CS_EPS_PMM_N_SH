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

	// (PSC + 1) * ARR = (AHB_freq(80MHz) / 1000) (for 1 milli)
	TIM2->PSC = 39999;
	TIM2->ARR = 2 * delay;

	TIM2->CR1 |= TIM_CR1_OPM;

	TIM2->EGR |= TIM_EGR_UG;
	TIM2->SR = 0;
	TIM2->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);

	TIM2->CR1 |= TIM_CR1_CEN;

	//LL analog works incorrect

//	LL_TIM_InitTypeDef TIM_InitStruct = {0};
//	TIM_finished = 0;
//
//	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
//
//	TIM_InitStruct.Prescaler = 39999;
//	TIM_InitStruct.Autoreload = 2 * delay;
//	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
//
//	LL_TIM_SetOnePulseMode(TIM2, LL_TIM_ONEPULSEMODE_SINGLE);
//
////	LL_TIM_GenerateEvent_UPDATE(TIM2);
//	LL_TIM_Init(TIM2, &TIM_InitStruct);
//	LL_TIM_ClearFlag_UPDATE(TIM2);
//
//	LL_TIM_EnableIT_UPDATE(TIM2);
//
//	NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
//	NVIC_EnableIRQ(TIM2_IRQn);
//
//	LL_TIM_EnableCounter(TIM2);

}
