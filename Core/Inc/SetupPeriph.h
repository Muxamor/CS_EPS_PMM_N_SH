#ifndef __SETUPPERIPH_H
#define __SETUPPERIPH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx_ll_i2c.h"

extern uint32_t SysTick_Counter;

void LL_Init(void);
void SystemClock_Config(uint8_t CPU_Speed);
void I2C3_Init(uint8_t CPU_Speed);
void I2C3_DeInit(void);
void I2C4_Init(uint8_t CPU_Speed);
void I2C4_DeInit(void);
void I2C_ReInit(I2C_TypeDef *I2Cx, uint8_t CPU_Speed);
void I2C_Bus_SoftwareReset(I2C_TypeDef *I2Cx, uint8_t number_cycle, uint8_t CPU_Speed);
void LPUART1_Init(void);
void LPUART1_DeInit(void);
void USART3_Init(void);
void USART3_DeInit(void);
void UART5_Init(void);
void Setup_UART_Interrupt( void);
void GPIO_Init(void);
void PWM_Init_Ch3_Ch4( uint32_t freq, uint32_t duty_cycle, uint16_t tim_divider);
void PWM_DeInit_Ch3_Ch4( void );
int8_t CAN_init_eps(CAN_TypeDef *can_ref);
int8_t CAN_DeInit_eps(CAN_TypeDef *can_ref);
void IWDG_Init(uint16_t period);

/*For control multiplexor TMUX1209  need use: */
#define ENABLE_TMUX1209_I2C();        LL_GPIO_SetOutputPin(GPIOF, LL_GPIO_PIN_0);
#define DISABLE_TMUX1209_I2C();       LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_0);

#define SW_TMUX1209_I2C_main_PAM();   LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_12); \
                                       LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);

#define SW_TMUX1209_I2C_main_PDM();   LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_12); \
                                       LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);

#define SW_TMUX1209_I2C_main_PBM();   LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_12); \
                                       LL_GPIO_ResetOutputPin(GPIOG, LL_GPIO_PIN_13);

#define SW_TMUX1209_I2C_main_PMM();   LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_12); \
                                       LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);

/*For control pin PBM Energy Level LOW  need use:			*/
#define PBM_Low_Energy_Set_pin();      	    LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_12);
#define PBM_Low_Energy_Reset_pin();         LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_12);

/* Pins for poll request from BRK */
#define PMM_RT_FL_EPS1_Set_pin();     	    LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_13);
#define PMM_RT_FL_EPS1_Reset_pin();         LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_13);
#define PMM_RT_FL_EPS2_Set_pin();      		LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_14);
#define PMM_RT_FL_EPS2_Reset_pin();     	LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_14);

/*The table below gives the allowed values of the pre-emption priority and subpriority according
 to the Priority Grouping configuration performed by NVIC_PriorityGroupConfig function
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_4  |                 0-15              |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority
  ===========================================================================================================================*/

#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

#ifdef __cplusplus
}
#endif

#endif /* SETUPPERIPH_H_ */
