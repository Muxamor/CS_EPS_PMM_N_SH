/*
 * pdm_config.h
 *
 *  Created on: Apr 29, 2020
 *      Author: Ivan
 */

#ifndef INC_PDM_PDM_CONFIG_H_
#define INC_PDM_PDM_CONFIG_H_

/*********************** Configuration for PDM  ***********************/

#define PDM_I2Cx_GPIOExt1 			I2C4 //I2C GPIO extender TCA9539 in PDM
#define PDM_I2CADDR_GPIOExt1 		0x74 //I2C GPIO extender TCA9539 in PDM

#define PDM_I2Cx_GPIOExt2 			I2C4 //I2C GPIO extender TCA9539 in PDM
#define PDM_I2CADDR_GPIOExt2 		0x75//I2C GPIO extender TCA9539 in PDM

#define PDM_I2Cx_TMP1075_1 			I2C4 //Temp. sensor in PDM 
#define PDM_I2CADDR_TMP1075_1 		0x48 //Temp. sensor in PDM

#define PDM_I2Cx_TMP1075_2 			I2C4 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_2 		0x49 //Temp. sensor in PDM

#define PDM_I2Cx_TMP1075_3 			I2C4 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_3 		0x4A //Temp. sensor in PDM

#define PDM_I2Cx_TMP1075_4 			I2C4 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_4 		0x4B //Temp. sensor in PDM


 
#define pdm_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pdm_i2c_delay_att_conn		5 	 //ms Delay between attempts connection through I2C in millisecond



#define PDM_PWR_Channel_1 			0x01 
#define PDM_PWR_Channel_2 			0x02
#define PDM_PWR_Channel_3 			0x03
#define PDM_PWR_Channel_4 			0x04
#define PDM_PWR_Channel_5 			0x05
#define PDM_PWR_Channel_6 			0x06


typedef struct{

	I2C_TypeDef *I2Cx;
	uint8_t I2C_tca9539_addr;
	uint16_t pin_EN_in_eFuse;
	uint16_t pin_EN_out_eFuse;
	uint16_t pin_PG_in_eFuse;
	uint16_t pin_PG_out_eFuse;

} _PDM_pins;

_PDM_pins PDM_pin_table(uint8_t number_pwr_channel);


#endif /* INC_PDM_PDM_CONFIG_H_ */
