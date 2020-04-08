/*
 * TMP1075.h
 *
 *  Created on: Apr 7, 2020
 *      Author: ftor
 */

#ifndef INC_TMP1075_H_
#define INC_TMP1075_H_


#include "main.h"

#define TMP1075_SHUTDOWN_MODE            1
#define TMP1075_CONTINUOS_CONVERSION     0

#define TMP1075_CR_VERY_FAST	0  // 27.5 ms
#define TMP1075_CR_FAST    		1  // 55 ms
#define TMP1075_CR_MEDIUM    	2  // 110 ms
#define TMP1075_CR_SLOW    		3  // 220 ms

#define TMP1075_ACTIVE_LOW 		0
#define TMP1075_ACTIVE_HIGH 	1

#define TMP1075_ONE_FAULT   	0
#define TMP1075_TWO_FAULT   	1
#define TMP1075_THREE_FAULT 	3
#define TMP1075_FOUR_FAULT  	4


int8_t I2C_Read_word(I2C_TypeDef *I2Cx, uint8_t reg_id, uint8_t SlaveAddr, uint16_t *data);
int8_t I2C_Write_word(I2C_TypeDef *I2Cx, uint8_t reg_id, uint8_t SlaveAddr, uint16_t data);


int8_t TMP1075_read_id(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data);
int8_t TMP1075_read_raw_temperature(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data);
float  TMP1075_raw_to_float(uint16_t ADC_CODE);
int8_t TMP1075_raw_to_int(uint16_t ADC_CODE);
uint16_t TMP1075_float_to_binary(float val);
int8_t TMP1075_read_config(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data);
int8_t TMP1075_set_mode(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode);
int8_t TMP1075_set_time_conversion(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t time);
int8_t TMP1075_one_shot_conversion_start(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr);
int8_t TMP1075_set_mode_ALERT_pin(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode);
int8_t TMP1075_ALERT_active_level(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode);
int8_t TMP1075_ALERT_sensitivity(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode);
int8_t TMP1075_set_low_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, float low_limit);
int8_t TMP1075_set_high_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, float high_limit);
int8_t TMP1075_get_low_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data);
int8_t TMP1075_get_high_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data);



#endif /* INC_TMP1075_H_ */
