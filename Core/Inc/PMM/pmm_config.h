

#ifndef INC_PMM_PMM_CONFIG_H_
#define INC_PMM_PMM_CONFIG_H_


/*********************** Configuration for PMM module ***********************/

#define PMM_I2Cx_TMP1075 				I2C3 //Temp. sensor in PMM CPU block
#define PMM_I2CADDR_TMP1075 			0x48 //Temp. sensor in PMM CPU block

#define PMM_I2Cx_GPIOExt1 				I2C3 //I2C GPIO extender TCA9539 in PMM CPU block
#define PMM_I2CADDR_GPIOExt1 			0x74 //I2C GPIO extender TCA9539 in PMM CPU block

#define PMM_I2Cx_GPIOExt2 				I2C3 //I2C GPIO extender TCA9539 in PMM CPU block
#define PMM_I2CADDR_GPIOExt2 			0x75 //I2C GPIO extender TCA9539 in PMM CPU block

#define PMM_I2Cx_FRAM1 					I2C3 //FRAM in PMM CPU block
#define PMM_I2CADDR_FRAM1 				0x50 //FRAM in PMM CPU block

#define PMM_I2Cx_FRAM2 					I2C3 //FRAM in PMM CPU block
#define PMM_I2CADDR_FRAM2 				0x51 //FRAM in PMM CPU block

#define PMM_I2Cx_DeployADC 				I2C4 //I2C ADC ADS1015 in PMM Deploy block
#define PMM_I2CADDR_DeployADC 			0x48 //I2C ADC ADS1015 in PMM Deploy block

#define PMM_I2Cx_DeployGPIOExt			I2C4 //I2C GPIO extender PCA9534 in PMM Deploy block
#define PMM_I2CADDR_DeployGPIOExt 		0x38 //I2C GPIO extender PCA9534 in PMM Deploy block

#define PMM_I2Cx_PowerADC				I2C4 //I2C ADC ADS1015 in PMM Power supply block
#define PMM_I2CADDR_PowerADC 			0x49 //I2C ADC ADS1015 in PMM Power supply block

#define PMM_I2Cx_VBAT1_eF1_INA231		I2C4 //I2C INA231 line VBAT1 Main Efuse
#define PMM_I2CADDR_VBAT1_eF1_INA231 	0x40 //I2C INA231 line VBAT1 Main Efuse

#define PMM_I2Cx_VBAT1_eF2_INA231		I2C4 //I2C INA231 line VBAT1 Backup Efuse
#define PMM_I2CADDR_VBAT1_eF2_INA231 	0x44 //I2C INA231 line VBAT1 Backup Efuse

#define PMM_I2Cx_VBAT2_eF1_INA231		I2C4 //I2C INA231 line VBAT2 Main Efuse
#define PMM_I2CADDR_VBAT2_eF1_INA231 	0x41 //I2C INA231 line VBAT2 Main Efuse

#define PMM_I2Cx_VBAT2_eF2_INA231		I2C4 //I2C INA231 line VBAT2 Backup Efuse
#define PMM_I2CADDR_VBAT2_eF2_INA231 	0x45 //I2C INA231 line VBAT2 Backup Efuse

#define FRAM_SIZE_64KB					0x2000


#define pmm_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pmm_i2c_delay_att_conn		5 	 //ms Delay between attempts connection through I2C in millisecond


#define Service_Mode_EPS			0xFF


#define CPUmain 					0x00
#define CPUbackup					0x01

#define PMM_PWR_Ch_CANmain 			0x00
#define PMM_PWR_Ch_CANbackup		0x01
#define PMM_PWR_Ch_VBAT1_eF1		0x02
#define PMM_PWR_Ch_VBAT1_eF2		0x03
#define PMM_PWR_Ch_VBAT2_eF1		0x04
#define PMM_PWR_Ch_VBAT2_eF2		0x05
#define PMM_PWR_Ch_PBMs_Logic		0x06
#define PMM_PWR_Ch_Deploy_Logic		0x07
#define PMM_PWR_Ch_Deploy_Power		0x08
#define PMM_PWR_Ch_5V_Bus			0x09
#define PMM_PWR_Ch_3_3V_Bus			0x10
#define PMM_PWR_Ch_I2C_Bus			0x11


typedef struct{

	I2C_TypeDef *I2Cx_GPIO_Ext;
	uint8_t I2C_addr_GPIO_Ext;
	uint16_t pin_GPIO_Ext;

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

}_PMM_table;


_PMM_table PMM__Table(uint8_t number_pwr_channel);

#endif /* INC_PMM_PMM_CONFIG_H_ */
