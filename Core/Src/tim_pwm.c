
//#include "stm32l496xx.h"
#include "stm32l4xx_ll_tim.h"
#include "tim_pwm.h"


void PWM_start_channel(TIM_TypeDef *TIMx, uint32_t Channel){
	LL_TIM_CC_EnableChannel(TIMx, Channel);
}

void PWM_stop_channel(TIM_TypeDef *TIMx, uint32_t Channel){
	LL_TIM_CC_DisableChannel(TIMx, Channel);

}

void PWM_start_CH3_CH4(TIM_TypeDef *TIMx){
	TIMx->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P; //| TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P;
}

void PWM_stop_CH3_CH4(TIM_TypeDef *TIMx){
	TIMx->CCER &= ~(TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P); //| TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P);
}


/* TO DO need do change freq.
void PWM_change_frec(TIM_TypeDef *TIMx, uint32_t Channel, uint16_t duty_cycle){
	if(channel == PWM_CH1) TIMx->CCR1 = F_CPU / ((TIMx_DEVIDER + 1) * SystemCoreClock) * duty_cycle / 100; // F_CPU / ((TIMx_DEVIDER + 1) * global_freq) - ������� ����
	else if(channel == PWM_CH2) TIMx->CCR2 = F_CPU / ((TIMx_DEVIDER + 1) * SystemCoreClock) * duty_cycle / 100; // PWM_freq * duty_cycle /100 - ������� ����������
	else if(channel == PWM_CH3) TIMx->CCR3 = F_CPU / ((TIMx_DEVIDER + 1) * SystemCoreClock) * duty_cycle / 100;
	else if(channel == PWM_CH4) TIMx->CCR4 = F_CPU / ((TIMx_DEVIDER + 1) * SystemCoreClock) * duty_cycle / 100;
}
*/
