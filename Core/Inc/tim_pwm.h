#ifndef INC_TIM_PWM_H_
#define INC_TIM_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

void PWM_start_channel(TIM_TypeDef *TIMx, uint32_t Channel);
void PWM_stop_channel(TIM_TypeDef *TIMx, uint32_t Channel);
void PWM_start_CH3_CH4(TIM_TypeDef *TIMx);
void PWM_stop_CH3_CH4(TIM_TypeDef *TIMx);

#ifdef __cplusplus
}
#endif

#endif
