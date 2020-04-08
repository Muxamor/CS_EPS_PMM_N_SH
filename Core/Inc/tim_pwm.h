
#ifndef INC_TIM_PWM_H_
#define INC_TIM_PWM_H_


void PWM_start_channel(TIM_TypeDef *TIMx, uint32_t Channel);
void PWM_stop_channel(TIM_TypeDef *TIMx, uint32_t Channel);
void PWM_start_CH3_CH4(TIM_TypeDef *TIMx);
void PWM_stop_CH3_CH4(TIM_TypeDef *TIMx);


#endif
