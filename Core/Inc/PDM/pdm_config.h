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


 
#define pdm_i2c_attempt_conn			3 // Quantity attempt to connection through I2C
#define pdm_i2c_delay_att_conn			5 //ms Delay between attempts connection through I2C in millisecond


#endif /* INC_PDM_PDM_CONFIG_H_ */
