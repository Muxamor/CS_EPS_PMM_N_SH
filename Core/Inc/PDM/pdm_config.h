/*
 * pdm_config.h
 *
 *  Created on: Apr 29, 2020
 *      Author: Ivan
 */

#ifndef INC_PDM_PDM_CONFIG_H_
#define INC_PDM_PDM_CONFIG_H_

/*********************** Configuration for PMM module ***********************/

#define PDM_I2Cx_TMP1075_1 			I2C4 //Temp. sensor in PDM module
#define PDM_I2CADDR_TMP1075_1 		0x48 //Temp. sensor in PDM module

#define PDM_I2Cx_TMP1075_2 			I2C4 //Temp. sensor in PDM module
#define PDM_I2CADDR_TMP1075_2 		0x49 //Temp. sensor in PDM module

#define PDM_I2Cx_TMP1075_3 			I2C4 //Temp. sensor in PDM module
#define PDM_I2CADDR_TMP1075_3 		0x4A //Temp. sensor in PDM module

#define PDM_I2Cx_TMP1075_4 			I2C4 //Temp. sensor in PDM module
#define PDM_I2CADDR_TMP1075_4 		0x4B //Temp. sensor in PDM module

#define PDM_PWR_Channel_1 			0x01 
#define PDM_PWR_Channel_2 			0x02
#define PDM_PWR_Channel_3 			0x03
#define PDM_PWR_Channel_4 			0x04
#define PDM_PWR_Channel_5 			0x05
#define PDM_PWR_Channel_6 			0x06
#define PDM_PWR_Channel_7 			0x07
#define PDM_PWR_Channel_ALL 		0xFF

#endif /* INC_PDM_PDM_CONFIG_H_ */
