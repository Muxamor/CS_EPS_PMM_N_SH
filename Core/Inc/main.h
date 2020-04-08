/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx.h"
#include "stm32l4xx_ll_iwdg.h"
#include "stm32l4xx_ll_crs.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_dma.h"
#include "stm32l4xx_ll_rtc.h"
#include "stm32l4xx_ll_tim.h"
#include "stm32l4xx_ll_lpuart.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_i2c.h"
#include "stm32l4xx_ll_gpio.h"



/* Private defines -----------------------------------------------------------*/
#define DCDC_b_PG_CPUm_Pin LL_GPIO_PIN_3
#define DCDC_b_PG_CPUm_GPIO_Port GPIOE
#define eF_PG_LBat_CPUm_Pin LL_GPIO_PIN_4
#define eF_PG_LBat_CPUm_GPIO_Port GPIOE
#define DCDC_m_PG_CPUm_Pin LL_GPIO_PIN_2
#define DCDC_m_PG_CPUm_GPIO_Port GPIOE
#define eF_PG_Deploy_LP_CPUm_Pin LL_GPIO_PIN_5
#define eF_PG_Deploy_LP_CPUm_GPIO_Port GPIOE
#define EEPROM_2_WC_CPUm_Pin LL_GPIO_PIN_8
#define EEPROM_2_WC_CPUm_GPIO_Port GPIOC
#define EEPROM_1_WC_CPUm_Pin LL_GPIO_PIN_7
#define EEPROM_1_WC_CPUm_GPIO_Port GPIOC
#define I2C_CPUm_MUX_I2C2_Pin LL_GPIO_PIN_6
#define I2C_CPUm_MUX_I2C2_GPIO_Port GPIOC
#define CPUm_RF_WG_1_Pin LL_GPIO_PIN_5
#define CPUm_RF_WG_1_GPIO_Port GPIOF
#define UART_mod_CPUm_OE_Pin LL_GPIO_PIN_15
#define UART_mod_CPUm_OE_GPIO_Port GPIOD
#define CPUm_RT_FL_EPS_2_Pin LL_GPIO_PIN_14
#define CPUm_RT_FL_EPS_2_GPIO_Port GPIOD
#define CPUm_RT_FL_EPS_1_Pin LL_GPIO_PIN_13
#define CPUm_RT_FL_EPS_1_GPIO_Port GPIOD
#define I2C_GPIO_Ext_PDM_PAM_Reset_Pin LL_GPIO_PIN_1
#define I2C_GPIO_Ext_PDM_PAM_Reset_GPIO_Port GPIOC
#define I2C_GPIO_Ext_PMM_Reset_Pin LL_GPIO_PIN_2
#define I2C_GPIO_Ext_PMM_Reset_GPIO_Port GPIOC
#define CPUm_RF_OP_1_Pin LL_GPIO_PIN_4
#define CPUm_RF_OP_1_GPIO_Port GPIOA
#define OE_Buffer_2_CPUm_Pin LL_GPIO_PIN_12
#define OE_Buffer_2_CPUm_GPIO_Port GPIOF
#define CPUm_BAT_Low_Pin LL_GPIO_PIN_12
#define CPUm_BAT_Low_GPIO_Port GPIOD
#define CPUm_set_mode3_Pin LL_GPIO_PIN_11
#define CPUm_set_mode3_GPIO_Port GPIOD
#define CPUm_set_mode2_Pin LL_GPIO_PIN_10
#define CPUm_set_mode2_GPIO_Port GPIOD
#define I2C_CPUm_MUX_A0_Pin LL_GPIO_PIN_3
#define I2C_CPUm_MUX_A0_GPIO_Port GPIOC
#define CPUm_RF_OP_2_Pin LL_GPIO_PIN_5
#define CPUm_RF_OP_2_GPIO_Port GPIOA
#define I2C_CPUm_MUX_A1_Pin LL_GPIO_PIN_4
#define I2C_CPUm_MUX_A1_GPIO_Port GPIOC
#define OE_Buffer_1_CPUm_Pin LL_GPIO_PIN_11
#define OE_Buffer_1_CPUm_GPIO_Port GPIOF
#define OE_Buffer_RF_CPUm_Pin LL_GPIO_PIN_13
#define OE_Buffer_RF_CPUm_GPIO_Port GPIOF
#define CPUm_set_mode1_Pin LL_GPIO_PIN_9
#define CPUm_set_mode1_GPIO_Port GPIOD
#define I2C_GPIO_Ext_PMM_INT_Pin LL_GPIO_PIN_8
#define I2C_GPIO_Ext_PMM_INT_GPIO_Port GPIOD
#define CPUm_RF_WG_2_Pin LL_GPIO_PIN_3
#define CPUm_RF_WG_2_GPIO_Port GPIOA
#define I2C_CPUm_MUX_EN_Pin LL_GPIO_PIN_5
#define I2C_CPUm_MUX_EN_GPIO_Port GPIOC
#define PG_Efuse_2_Vbat1_CPUm_Pin LL_GPIO_PIN_8
#define PG_Efuse_2_Vbat1_CPUm_GPIO_Port GPIOE
#define PG_Efuse_2_Vbat2_CPUm_Pin LL_GPIO_PIN_10
#define PG_Efuse_2_Vbat2_CPUm_GPIO_Port GPIOE
#define PG_DC_eFuse_FLT_CAN_b_CPUm_Pin LL_GPIO_PIN_12
#define PG_DC_eFuse_FLT_CAN_b_CPUm_GPIO_Port GPIOE
#define CPUm_RF_WG_1A1_Pin LL_GPIO_PIN_1
#define CPUm_RF_WG_1A1_GPIO_Port GPIOA
#define OFF_CPUb_eF_1_Pin LL_GPIO_PIN_0
#define OFF_CPUb_eF_1_GPIO_Port GPIOB
#define OFF_CPUb_eF_2_Pin LL_GPIO_PIN_1
#define OFF_CPUb_eF_2_GPIO_Port GPIOB
#define PG_Efuse_1_Vbat1_CPUm_Pin LL_GPIO_PIN_7
#define PG_Efuse_1_Vbat1_CPUm_GPIO_Port GPIOE
#define PG_Efuse_1_Vbat2_CPUm_Pin LL_GPIO_PIN_9
#define PG_Efuse_1_Vbat2_CPUm_GPIO_Port GPIOE
#define PG_DC_eFuse_FLT_CAN_m_CPUm_Pin LL_GPIO_PIN_11
#define PG_DC_eFuse_FLT_CAN_m_CPUm_GPIO_Port GPIOE
#define PG_DC_eFuse_FLT_I2C_sys_CPUm_Pin LL_GPIO_PIN_13
#define PG_DC_eFuse_FLT_I2C_sys_CPUm_GPIO_Port GPIOE
#define PG_DC_eFuse__3_3V_BUS_CPUm_Pin LL_GPIO_PIN_14
#define PG_DC_eFuse__3_3V_BUS_CPUm_GPIO_Port GPIOE
#define PG_DC_eFuse__5V_BUS_CPUm_Pin LL_GPIO_PIN_15
#define PG_DC_eFuse__5V_BUS_CPUm_GPIO_Port GPIOE



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
