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

#define PDM_I2Cx_I2C_MUX			I2C4 //I2C bus multiplexor TCA9548 in PDM
#define PDM_I2CADDR_I2C_MUX 		0x70//I2C bus multiplexor TCA9548 in PDM

#define PDM_I2Cx_TMP1075_1 			I2C4 //Temp. sensor in PDM 
#define PDM_I2CADDR_TMP1075_1 		0x48 //Temp. sensor in PDM

#define PDM_I2Cx_TMP1075_2 			I2C4 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_2 		0x49 //Temp. sensor in PDM

#define PDM_I2Cx_TMP1075_3 			I2C4 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_3 		0x4A //Temp. sensor in PDM

#define PDM_I2Cx_TMP1075_4 			I2C4 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_4 		0x4B //Temp. sensor in PDM

#define PDM_I2Cx_PWR_Mon_Ch1 		I2C4 //Power Monitor INA231 power channel 1 in PDM
#define PDM_I2CADDR_PWR_Mon_Ch1 	0x40 //Power Monitor INA231 power channel 1 in PDM

#define PDM_I2Cx_PWR_Mon_Ch2  		I2C4 //Power Monitor INA231 power channel 2 in PDM
#define PDM_I2CADDR_PWR_Mon_Ch2 	0x41 //Power Monitor INA231 power channel 2 in PDM

#define PDM_I2Cx_PWR_Mon_Ch3  		I2C4 //Power Monitor INA231 power channel 3 in PDM
#define PDM_I2CADDR_PWR_Mon_Ch3  	0x44 //Power Monitor INA231 power channel 3 in PDM

#define PDM_I2Cx_PWR_Mon_Ch4  		I2C4 //Power Monitor INA231 power channel 4 in PDM
#define PDM_I2CADDR_PWR_Mon_Ch4 	0x45 //Power Monitor INA231 power channel 4 in PDM

#define PDM_I2Cx_PWR_Mon_Ch5  		I2C4 //Power Monitor INA231 power channel 5 in PDM
#define PDM_I2CADDR_PWR_Mon_Ch5 	0x42 //Power Monitor INA231 power channel 5 in PDM

#define PDM_I2Cx_PWR_Mon_Ch6  		I2C4 //Power Monitor INA231 power channel 6 in PDM
#define PDM_I2CADDR_PWR_Mon_Ch6 	0x46 //Power Monitor INA231 power channel 6 in PDM

#define pdm_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pdm_i2c_delay_att_conn		5 	 //ms Delay between attempts connection through I2C in millisecond


//!!!! Later transfer to structure PDM
#define PDM_PCB_Assembled_PWR_CH1 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH2 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH3 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH4 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH5 		0x00 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH6 		0x00 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
//!!!!!

#define PDM_PWR_Channel_1 			0x01 
#define PDM_PWR_Channel_2 			0x02
#define PDM_PWR_Channel_3 			0x03
#define PDM_PWR_Channel_4 			0x04
#define PDM_PWR_Channel_5 			0x05
#define PDM_PWR_Channel_6 			0x06


typedef struct{

	I2C_TypeDef *I2Cx_GPIO_Ext;
	uint8_t I2C_addr_GPIO_Ext;
	uint16_t pin_EN_in_eFuse;
	uint16_t pin_EN_out_eFuse;
	uint16_t pin_PG_in_eFuse;
	uint16_t pin_PG_out_eFuse;

	I2C_TypeDef *I2Cx_PWR_Mon;
	uint8_t I2C_addr_PWR_Mon;
	uint16_t PWR_Mon_Max_Current_int16;
	float PWR_Mon_Max_Current_float;
	uint16_t PWR_Mon_Rshunt_int16;
	float PWR_Mon_Rshunt_float;
	uint8_t PWR_Mon_aver_mode;
	uint8_t PWR_Mon_bus_conv_time;
	uint8_t PWR_Mon_shunt_conv_time; 
	uint8_t PWR_Mon_work_mode;

	I2C_TypeDef *I2Cx_I2C_MUX;
	uint8_t I2C_addr_I2C_MUX;
	uint8_t I2C_MUX_Ch;

} _PDM_table;

_PDM_table PDM__Table(uint8_t number_pwr_channel);


#endif /* INC_PDM_PDM_CONFIG_H_ */
