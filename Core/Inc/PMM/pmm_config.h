

#ifndef INC_PMM_PMM_CONFIG_H_
#define INC_PMM_PMM_CONFIG_H_


/*********************** Configuration for PMM module ***********************/

#define PMM_I2Cx_TMP1075 			I2C3 //Temp. sensor in PMM CPU block
#define PMM_I2CADDR_TMP1075 		0x48 //Temp. sensor in PMM CPU block

#define PMM_I2Cx_GPIOExt1 			I2C3 //I2C GPIO extender TCA9539 in PMM CPU block
#define PMM_I2CADDR_GPIOExt1 		0x74 //I2C GPIO extender TCA9539 in PMM CPU block

#define PMM_I2Cx_FRAM1 				I2C3 //FRAM in PMM CPU block
#define PMM_I2CADDR_FRAM1 			0x50 //FRAM in PMM CPU block

#define PMM_I2Cx_FRAM2 				I2C3 //FRAM in PMM CPU block
#define PMM_I2CADDR_FRAM2 			0x51 //FRAM in PMM CPU block

#define PMM_I2Cx_DeployADC 			I2C4 //I2C ADC ADS1015 in PMM Deploy block
#define PMM_I2CADDR_DeployADC 		0x48 //I2C ADC ADS1015 in PMM Deploy block

#define PMM_I2Cx_DeployGPIOExt		I2C4 //I2C GPIO extender PCA9534 in PMM Deploy block
#define PMM_I2CADDR_DeployGPIOExt 	0x38 //I2C GPIO extender PCA9534 in PMM Deploy block

#define PMM_I2Cx_PowerADC			I2C4 //I2C ADC ADS1015 in PMM Power supply block
#define PMM_I2CADDR_PowerADC 		0x49 //I2C ADC ADS1015 in PMM Power supply block

#define PMM_I2Cx_VBAT1_M_INA231		I2C4 //I2C INA231 line VBAT1 Main Efuse
#define PMM_I2CADDR_VBAT1_M_INA231 	0x40 //I2C INA231 line VBAT1 Main Efuse

#define PMM_I2Cx_VBAT1_B_INA231		I2C4 //I2C INA231 line VBAT1 Backup Efuse
#define PMM_I2CADDR_VBAT1_B_INA231 	0x44 //I2C INA231 line VBAT1 Backup Efuse

#define PMM_I2Cx_VBAT2_M_INA231		I2C4 //I2C INA231 line VBAT2 Main Efuse
#define PMM_I2CADDR_VBAT2_M_INA231 	0x41 //I2C INA231 line VBAT2 Main Efuse

#define PMM_I2Cx_VBAT2_B_INA231		I2C4 //I2C INA231 line VBAT2 Backup Efuse
#define PMM_I2CADDR_VBAT2_B_INA231 	0x45 //I2C INA231 line VBAT2 Backup Efuse

#define FRAM_SIZE_64KB			0x2000

#define CANmain 	0x00
#define CANbackup	0x01

#define CPUmain 	0x00
#define CPUbackup	0x01

#define pmm_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pmm_i2c_delay_att_conn		5 	 //ms Delay between attempts connection through I2C in millisecond



#endif /* INC_PMM_PMM_CONFIG_H_ */
