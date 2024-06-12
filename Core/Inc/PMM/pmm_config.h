#ifndef INC_PMM_PMM_CONFIG_H_
#define INC_PMM_PMM_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************** Configuration for PMM module ***********************/

#ifndef VERSION_FW_MAJOR
  #define  VERSION_FW_MAJOR 0  // version major update  0..255
#endif

#ifndef VERSION_FW_MINOR
#define  VERSION_FW_MINOR 0 // version minor update  0..255
#endif


#define PMM_I2Cx_TMP1075 				I2C3 //Temp. sensor in PMM CPU block
#define PMM_I2CADDR_TMP1075 			0x48 //Temp. sensor in PMM CPU block

#define PMM_I2Cx_GPIOExt1 				I2C3 //I2C GPIO extender TCA9539 in PMM CPU block
#define PMM_I2CADDR_GPIOExt1 			0x74 //I2C GPIO extender TCA9539 in PMM CPU block

#define PMM_I2Cx_FRAM1 					I2C3 //FRAM in PMM CPU block
#define PMM_I2CADDR_FRAM1 				0x50 //FRAM in PMM CPU block

#define PMM_I2Cx_FRAM2 					I2C3 //FRAM in PMM CPU block
#define PMM_I2CADDR_FRAM2 				0x51 //FRAM in PMM CPU block

#define PMM_I2Cx_DeployADC 				I2C4 //I2C ADC ADS1015 in PMM Deploy block
#define PMM_I2CADDR_DeployADC 			0x48 //I2C ADC ADS1015 in PMM Deploy block

#define PMM_I2Cx_DeployGPIOExt			I2C4 //I2C GPIO extender PCA9534 in PMM Deploy block
#define PMM_I2CADDR_DeployGPIOExt 		0x38 //I2C GPIO extender PCA9534 in PMM Deploy block

#define PMM_I2Cx_VBAT1_eF_INA238		I2C4 //I2C INA231 line VBAT1 Main Efuse
#define PMM_I2CADDR_VBAT1_eF_INA238 	0x4A //I2C INA231 line VBAT1 Main Efuse

#define PMM_I2Cx_VBAT2_eF_INA238		I2C4 //I2C INA231 line VBAT2 Main Efuse
#define PMM_I2CADDR_VBAT2_eF_INA238 	0x4F //I2C INA231 line VBAT2 Main Efuse


#define FRAM_SIZE_64KB					0x2000

#define PMM_CAN_Exch_Data_Check_Time_Gap    150000 // 2m 30s in milliseconds (real 5m 00s after first 2m 30s reboot power CAN)
#define PMM_ReInit_Time_Gap  		        300000 // 5m00s in milliseconds
#define PMM_UART_Exch_Data_Gap      		45000 // 45s in milliseconds


#define pmm_i2c_attempt_conn		3 	 // Quantity attempt to connection through I2C
#define pmm_i2c_delay_att_conn		3 	 //ms Delay between attempts connection through I2C in millisecond

#define pmm_uart_attempt_conn		3 	 // Quantity attempt to connection through UART
#define pmm_uart_delay_att_conn		3 	 //ms Delay between attempts connection through UART in millisecond

#define PMM_Deploy_Power_Gen_EDGE   3000 // in milliWatts EDGE power generation from solar panel
#define PMM_Deploy_Time_Delay       1800000 //in milliseconds 1800000 = 30 minuts
#define PMM_Deploy_Burn_time_1      3500  // in milisecand
#define PMM_Deploy_Burn_time_2      4500  // in milisecand
#define PMM_Deploy_Burn_time_3      5500  // in milisecand

#define PMM_Deploy_Burn_Attempt_1   0x00
#define PMM_Deploy_Burn_Attempt_2   0x01
#define PMM_Deploy_Burn_Attempt_3   0x03

#define PAM                         0x00
#define PDM                         0x01

#define CPUmain 					0
#define CPUbackup					1

#define CPU_Clock_80MHz             80000000
#define CPU_Clock_16MHz             16000000

#define CPUmain_Active				0
#define CPUbackup_Active			1

#define EPS_COMBAT_MODE             0x00
#define EPS_SERVICE_MODE            0xFF

#define UART_EPS_CPUm_Addr 			0x01
#define UART_EPS_CPUb_Addr 			0x02

#define PMM_PWR_Ch_CANmain 			0x00
#define PMM_PWR_Ch_CANbackup		0x01
#define PMM_PWR_Ch_VBAT1_eF			0x02
#define PMM_PWR_Ch_VBAT2_eF			0x04
#define PMM_PWR_Ch_PBMs_Logic		0x06
#define PMM_PWR_Ch_Deploy_Logic		0x07
#define PMM_PWR_Ch_Deploy_Power		0x08
#define PMM_Reset_CANmain			0x09
#define PMM_Reset_CANbackup			0x0A


#define PMM_PWR_Deploy_Ch1          0x0C
#define PMM_PWR_Deploy_Ch2          0x0D
#define PMM_PWR_Deploy_Ch3          0x0E
#define PMM_PWR_Deploy_Ch4          0x0F

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

	uint8_t PWR_Mon_Convr_Time;
	uint8_t PWR_Mon_Aver_Count;
	uint8_t PWR_Mon_ADC_Range;
	uint8_t PWR_Mon_Mode;


}_PMM_table;


_PMM_table PMM__Table(uint8_t number_pwr_channel);

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_CONFIG_H_ */
