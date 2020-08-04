

#ifndef INC_PAM_PAM_CONFIG_H_
#define INC_PAM_PAM_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx.h"

/*********************** Configuration for PAM  ***********************/
#define PAM_I2C_PORT 				I2C4 //Number of I2C port for PAM.

#define PAM_I2CADDR_TMP1075_1 		0x48 //Temp. sensor in PAM
#define PAM_I2CADDR_TMP1075_2 		0x49 //Temp. sensor in PAM
#define PAM_I2CADDR_TMP1075_3 		0x4A //Temp. sensor in PAM
#define PAM_I2CADDR_TMP1075_4 		0x4B //Temp. sensor in PAM

#define PAM_I2CADDR_GPIO_Ext 		0x22 //I2C GPIO extender TCA6424 in PAM
#define PAM_I2CADDR_I2C_MUX_1 		0x70 //I2C bus multiplexor TCA9548 in PAM
#define PAM_I2CADDR_I2C_MUX_2 		0x71 //I2C bus multiplexor TCA9548 in PAM

#define PAM_I2CADDR_PWR_Mon_Ch1 	0x40 //Power Monitor INA231 input power channel 1 in PAM
#define PAM_I2CADDR_PWR_Mon_Ch2 	0x41 //Power Monitor INA231 input power channel 1 in PAM
#define PAM_I2CADDR_PWR_Mon_Ch3 	0x44 //Power Monitor INA231 input power channel 1 in PAM
#define PAM_I2CADDR_PWR_Mon_Ch4 	0x45 //Power Monitor INA231 input power channel 1 in PAM
#define PAM_I2CADDR_PWR_Mon_Ch5 	0x42 //Power Monitor INA231 input power channel 1 in PAM
#define PAM_I2CADDR_PWR_Mon_Ch6 	0x46 //Power Monitor INA231 input power channel 1 in PAM

#define pam_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pam_i2c_delay_att_conn		3 	 //ms Delay between attempts connection through I2C in millisecond

#define PAM_PWR_IN_Ch_quantity  	0x06 // Total quantity power channels on the PAM
#define PAM_SP_quantity  			0x06 // Total quantity SP channels on the PAM
#define PAM_SP_Temp_sensor_quantity	0x04 // Total quantity Temp. sensor on one SP
#define PAM_SP_ID_quantity			0x06 // Total quantity Ideal Diode on one SP

#define PAM_PWR_IN_Channel_1 		0x00 //Panel Y+ (PAM 1.0 socket X1)
#define PAM_PWR_IN_Channel_2 		0x01 //Panel X- (PAM 1.0 socket X2)
#define PAM_PWR_IN_Channel_3 		0x02 //Panel Y- (PAM 1.0 socket X3)
#define PAM_PWR_IN_Channel_4 		0x03 //Panel X+ (PAM 1.0 socket X2)
#define PAM_PWR_IN_Channel_5 		0x04 //Folding Panel LT8490_1 (PAM 1.0 socket X5 and X6)
#define PAM_PWR_IN_Channel_6 		0x05 //Folding Panel LT8490_1 (PAM 1.0 socket X7 and X8)

#define PAM_PWR_DC_DC 				0x06
#define PAM_PWR_LDO 				0x07

typedef struct{

	I2C_TypeDef *I2Cx_PORT;
	uint8_t I2C_addr_GPIO_Ext;
	uint32_t pin_State_ID;
	uint32_t pin_Enable_eF;

	uint8_t I2C_addr_PWR_Mon;
	uint16_t PWR_Mon_Max_Current_int16;
	float PWR_Mon_Max_Current_float;
	uint16_t PWR_Mon_Rshunt_int16;
	float PWR_Mon_Rshunt_float;
	uint8_t PWR_Mon_aver_mode;
	uint8_t PWR_Mon_bus_conv_time;
	uint8_t PWR_Mon_shunt_conv_time;
	uint8_t PWR_Mon_work_mode;

	uint8_t I2C_addr_I2C_MUX_PWR;
	uint8_t I2C_addr_I2C_MUX_SP;
	uint8_t I2C_MUX_Ch;

} _PAM_table;

_PAM_table PAM__Table(uint8_t number_pwr_channel);

#ifdef __cplusplus
}
#endif

#endif /* INC_PAM_PAM_CONFIG_H_ */
