
#include "TIM_delay.h"

/** @brief	Start countdown in millis. After the specified number of milliseconds has elapsed,
 			an interrupt will occur with the raising of the global flag.
	@param  *TIMx - pointer to timer struct (TIM2, TIM5(up to month), TIM4(up to 30 sec) - generate TIM_finished flag;
			TIM15, TIM16, TIM17(up to 30 sec) - need to check TIMx->CNT register. If CNT == 0 then counter finished)).
    @param  delay - amount of millis until raising interrupt.
*/
ErrorStatus TIM_init(TIM_TypeDef *TIMx, uint32_t delay){

	TIM2_finished = 0;
	TIM4_finished = 0;
	TIM5_finished = 0;
	LL_TIM_InitTypeDef TIM_InitStruct = {0};

	if(TIMx == TIM2){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	}
	else if(TIMx == TIM4){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	}
	else if(TIMx == TIM5){
		LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
	}
	else if(TIMx == TIM15){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM15);
	}
	else if(TIMx == TIM16){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM16);
	}
	else if(TIMx == TIM17){
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM17);
	}
	else{
		return ERROR_N;
	}

	// (PSC + 1) * ARR = (AHB_freq / 1000) (for 1 milli)    (AHB_freq = 80MHz)
	TIM_InitStruct.Prescaler = 39999;
	TIM_InitStruct.Autoreload = 2 * delay;

	LL_TIM_Init(TIMx, &TIM_InitStruct);
	LL_TIM_SetOnePulseMode(TIMx, LL_TIM_ONEPULSEMODE_SINGLE);

	LL_TIM_GenerateEvent_UPDATE(TIMx);
	LL_TIM_ClearFlag_UPDATE(TIMx);

	LL_TIM_EnableIT_UPDATE(TIMx);

	if(TIMx == TIM2){
//		NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
		NVIC_EnableIRQ(TIM2_IRQn);
	}
	else if(TIMx == TIM4){
//		NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
		NVIC_EnableIRQ(TIM4_IRQn);
	}
	else if(TIMx == TIM5){
//		NVIC_SetPriority(TIM5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
		NVIC_EnableIRQ(TIM5_IRQn);
	}

	LL_TIM_EnableCounter(TIMx);

	if(TIMx == TIM15 || TIMx == TIM16 || TIMx == TIM17){
		LL_mDelay(1);
	}

	return SUCCESS;
}


ErrorStatus TIM_DeInit(TIM_TypeDef *TIMx){
	if(TIMx == TIM2){
		LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM2);
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM2);
	}
	else if(TIMx == TIM4){
		LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM4);
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM4);
	}
	else if(TIMx == TIM5){
		LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_TIM5);
		LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_TIM5);
	}
	else if(TIMx == TIM15){
		LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM15);
		LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM15);
	}
	else if(TIMx == TIM16){
		LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM16);
		LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM16);
	}
	else if(TIMx == TIM17){
		LL_APB2_GRP1_ForceReset(LL_APB2_GRP1_PERIPH_TIM17);
		LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_TIM17);
	}
	else{
		return ERROR_N;
	}

	if(TIMx == TIM2){
		NVIC_DisableIRQ(TIM2_IRQn);
	}
	else if(TIMx == TIM4){
		NVIC_DisableIRQ(TIM4_IRQn);
	}
	else if(TIMx == TIM5){
		NVIC_DisableIRQ(TIM5_IRQn);
	}


	LL_TIM_DisableCounter(TIMx);

	return SUCCESS;
}



void TIM2_IRQHandler(void){
	TIM2_finished = 1;
	LL_TIM_ClearFlag_UPDATE(TIM2);
	LL_TIM_DisableIT_UPDATE(TIM2);
	//	printf("TIM2 handler\n");
}

void TIM4_IRQHandler(void){
	TIM4_finished = 1;
	LL_TIM_ClearFlag_UPDATE(TIM4);
	LL_TIM_DisableIT_UPDATE(TIM4);
	//	printf("TIM4 handler\n");
}
void TIM5_IRQHandler(void){
	TIM5_finished = 1;
	LL_TIM_ClearFlag_UPDATE(TIM5);
	LL_TIM_DisableIT_UPDATE(TIM5);
//	printf("TIM5 handler\n");
}
