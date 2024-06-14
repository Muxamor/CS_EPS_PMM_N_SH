#ifndef INC_PDM_PDM_CONFIG_H_
#define INC_PDM_PDM_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************** Configuration for PDM  ***********************/

#define PDM_I2Cx_PORT 				I2C4 //I2C GPIO extender TCA9539 in PDM

#define PDM_I2CADDR_GPIOExt1 		0x74 //I2C GPIO extender TCA9539 in PDM
#define PDM_I2CADDR_GPIOExt2 		0x75//I2C GPIO extender TCA9539 in PDM

#define PDM_I2CADDR_I2C_MUX1		0x70//I2C bus multiplexor TCA9548 in PDM
#define PDM_I2CADDR_I2C_MUX2		0x71//I2C bus multiplexor TCA9548 in PDM

#define PDM_I2CADDR_TMP1075_1 		0x48 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_2 		0x49 //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_3 		0x4A //Temp. sensor in PDM
#define PDM_I2CADDR_TMP1075_4 		0x4B //Temp. sensor in PDM

#define PDM_I2CADDR_PWR_Mon_Ch1 	0x40 //Power Monitor INA231 power channel 1 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch2 	0x41 //Power Monitor INA231 power channel 2 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch3  	0x42 //Power Monitor INA231 power channel 3 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch4 	0x43 //Power Monitor INA231 power channel 4 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch5 	0x44 //Power Monitor INA231 power channel 5 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch6 	0x45 //Power Monitor INA231 power channel 6 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch7 	0x46 //Power Monitor INA231 power channel 7 in PDM H1H2
#define PDM_I2CADDR_PWR_Mon_Ch8 	0x47 //Power Monitor INA231 power channel 8 in PDM Wired channel №1
#define PDM_I2CADDR_PWR_Mon_Ch9 	0x48 //Power Monitor INA231 power channel 8 in PDM Wired channel №2
#define PDM_I2CADDR_PWR_Mon_Ch10 	0x49 //Power Monitor INA231 power channel 8 in PDM Wired channel №3
#define PDM_I2CADDR_PWR_Mon_Ch11 	0x4A //Power Monitor INA231 power channel 8 in PDM Wired channel №4
#define PDM_I2CADDR_PWR_Mon_Ch12 	0x4B //Power Monitor INA231 power channel 8 in PDM High Current Channel channel №1

#define pdm_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pdm_i2c_delay_att_conn		3 	 //ms Delay between attempts connection through I2C in millisecond

//Later transfer to structure PDM
#define PDM_PCB_Assembled_PWR_CH1 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH2 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH3 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH4 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH5 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH6 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH7 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH8 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH9 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH10 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH11 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled
#define PDM_PCB_Assembled_PWR_CH12 		0x01 // 1 - PWR channel is assembled on the PCB, 0 - no assembled

#define PDM_PWR_Ch_quantity  		12 // Total quantity power channels on the PDM
 
#define PDM_PWR_Channel_1 			0x00
#define PDM_PWR_Channel_2 			0x01
#define PDM_PWR_Channel_3 			0x02
#define PDM_PWR_Channel_4 			0x03
#define PDM_PWR_Channel_5 			0x04
#define PDM_PWR_Channel_6 			0x05
#define PDM_PWR_Channel_7 			0x06
#define PDM_PWR_Channel_8 			0x07
#define PDM_PWR_Channel_9 			0x08
#define PDM_PWR_Channel_10 			0x09
#define PDM_PWR_Channel_11 			0x0A
#define PDM_PWR_Channel_12 			0x0B

typedef struct{

	I2C_TypeDef *I2Cx_GPIO_Ext;
	uint8_t I2C_addr_GPIO_Ext;
	uint16_t pin_EN_eFuse;
	uint16_t pin_PG_eFuse;

	I2C_TypeDef *I2Cx_PWR_Mon;
	uint8_t I2C_addr_PWR_Mon;
	uint16_t PWR_Mon_Max_Current_int16;
	float PWR_Mon_Max_Current_float;
	uint16_t PWR_Mon_Rshunt_int16;
	float PWR_Mon_Rshunt_float;

	uint8_t PWR_Mon_Convr_Time;
	uint8_t PWR_Mon_Aver_Count;
	uint8_t PWR_Mon_ADC_Range;
	uint8_t PWR_Mon_Mode;

	I2C_TypeDef *I2Cx_I2C_MUX;
	uint8_t I2C_addr_I2C_MUX;
	uint8_t I2C_MUX_Ch;

} _PDM_table;

_PDM_table PDM__Table(uint8_t number_pwr_channel);

#ifdef __cplusplus
}
#endif

#endif /* INC_PDM_PDM_CONFIG_H_ */
