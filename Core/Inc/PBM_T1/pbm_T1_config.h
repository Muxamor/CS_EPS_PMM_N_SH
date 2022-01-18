#ifndef INC_PBM_T1_CONFIG_H_
#define INC_PBM_T1_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "TCA9548.h"

/*********************** Configuration for PBM module ***********************/

#define PBM_T1_I2C_PORT                		I2C4 	//Number of I2C port for PBM.
#define PBM_T1_I2C_ATTEMPT_CONN             3 	    //Quantity attempt to connection through I2C.
#define PBM_T1_i2c_delay_att_conn			3 	 	//ms Delay between attempts connection through I2C in millisecond
#define PBM_T1_QUANTITY 					0x04 	// Total quantity power modules PBM
#define PBM_T1_BRANCH_QUANTITY 				0x02 	// Total quantity branches on one PBM
#define PBM_T1_BRANCH_BAT_QUANTITY			2 	    // Total quantity battery in branch
#define PBM_T1_HEAT_QUANTITY 				0x02 	// Total quantity heaters on one PBM
#define PBM_T1_HEAT_TEMPSENS_QUANTITY		0x02 	// Total quantity temperature sensors on one heat
//#define PBM_T1_TEMPSENS_QUANTITY			0x00 	 Total quantity temperature sensors

#define PBM_T1_1_TCA9548A_ADDR              0x70 	//Address GPIO mux PCA9534ARGTR Battery Module №1.
#define PBM_T1_2_TCA9548A_ADDR              0x71 	//Address GPIO mux PCA9534ARGTR Battery Module №2.
#define PBM_T1_3_TCA9548A_ADDR              0x72 	//Address GPIO mux PCA9534ARGTR Battery Module №3.
#define PBM_T1_4_TCA9548A_ADDR              0x73 	//Address GPIO mux PCA9534ARGTR Battery Module №4.

#define PBM_T1_PCA9534_ADDR               	0x38 	//Address GPIO mux PCA9534ARGTR Battery Module.

#define PBM_T1_INA238_ADDR_1				0x40 	//Address INA mux 1 Battery Module.
#define PBM_T1_INA238_ADDR_2				0x41 	//Address INA mux 2 Battery Module.

#define PBM_T1_INA238_MAX_CURRENT			350 	//Maximum current on heat mA.
#define PBM_T1_INA238_RSHUNT				150 	//Resistance of current shunt heat im mOhm.

#define PBM_T1_TMP1075_HEAT_1_ADDR_1        0x48 	//Address Temperature Sensor 1 Heat 1.
#define PBM_T1_TMP1075_HEAT_1_ADDR_2        0x49 	//Address Temperature Sensor 2 Heat 1.
#define PBM_T1_TMP1075_HEAT_2_ADDR_1        0x4A 	//Address Temperature Sensor 1 Heat 2.
#define PBM_T1_TMP1075_HEAT_2_ADDR_2        0x4B 	//Address Temperature Sensor 2 Heat 2.

#define PBM_T1_TMP1075_TEMP_HI             	10.0 	//Hi limit for comparison TMP1075 Battery Module in °C.
#define PBM_T1_TMP1075_TEMP_LO             	2.0 	//Lo limit for comparison TMP1075 Battery Module in °C.
#define PBM_T1_TMP1075_Hi_Limit				20.0	//Hi error limit for comparison TMP1075 Battery Module in °C.

#define PBM_T1_1                			0x00 	//Designation for choose PBM №1.
#define PBM_T1_2                			0x01 	//Designation for choose PBM №2.
#define PBM_T1_3                			0x02	//Designation for choose PBM №3.
#define PBM_T1_4                			0x03	//Designation for choose PBM №3.

#define PBM_T1_BRANCH_1                		0x00 	//Designation for choose branch №1 PBM.
#define PBM_T1_BRANCH_2                		0x01 	//Designation for choose branch №2 PBM.
//#define PBM_T1_BRANCH_ALL                	0xff 	//Designation for choose all branch PBM.

#define PBM_T1_HEAT_1						0x00 	//Designation for choose heat №1 PBM.
#define PBM_T1_HEAT_2						0x01 	//Designation for choose heat №1 PBM.

#define PBM_T1_HEAT_TEMPSENS_1              0x00 	//Designation for choose temperature sensor №1 in heat branch.
#define PBM_T1_HEAT_TEMPSENS_2              0x01 	//Designation for choose temperature sensor №2 in heat branch.

#define PBM_T1_ON_HEAT                		0x01 	//Enable heat branch.
#define PBM_T1_OFF_HEAT                		0x00 	//Disable heat branch.

#define PBM_T1_ON_CHARGE               		0x01 	//Enable charge branch.
#define PBM_T1_OFF_CHARGE              		0x00 	//Disable charge branch.

#define PBM_T1_ON_DISCHARGE                	0x01 	//Enable discharge branch.
#define PBM_T1_OFF_DISCHARGE               	0x00 	//Disable discharge branch.

#define PBM_T1_ON_EMERG_CHRG          		0x01 	//Enable emergency charge.
#define PBM_T1_OFF_EMERG_CHRG         		0x00 	//Disable emergency charge.

#define PBM_T1_ON_AUTO_CAP_CORRECT    		0x01 	//Enable auto correction capacity level.
#define PBM_T1_OFF_AUTO_CAP_CORRECT         0x00 	//Disable auto correction capacity level.

#define PBM_T1_MAX17320_R_SENSE    			10 		//Resistance of current shunt im mOhm.
#define PBM_T1_MAX_BATT_CAP           		3100 	//Maximum branch battery capacity in mAh.

#define PBM_T1_NORMAL_ENERGY_EDGE           7200 	//Voltage in mV for hysteresis loop warning low energy PBM.
#define PBM_T1_LOW_ENERGY_EDGE              6500 	//Voltage in mV for warning low energy PBM.
#define PBM_T1_ZERO_ENERGY_EDGE             6000 	//Voltage in mV for off all payload.

#define PBM_T1_I2C_MUX_CH_GPIO 				TCA9548_CH2 // Number I2C Mux channel for GPIO mux.
#define PBM_T1_I2C_MUX_CH_Br1 				TCA9548_CH6 // Number I2C Mux channel for MAX17320 mux branch 1.
#define PBM_T1_I2C_MUX_CH_Br2 				TCA9548_CH5 // Number I2C Mux channel for MAX17320 mux branch 2.
#define PBM_T1_I2C_MUX_CH_Heat1 			TCA9548_CH0 // Number I2C Mux channel for TMP1075 mux heat 1.
#define PBM_T1_I2C_MUX_CH_Heat2 			TCA9548_CH3 // Number I2C Mux channel for TMP1075 mux heat 2.
#define PBM_T1_I2C_MUX_CH_PwrMon1 			TCA9548_CH1 // Number I2C Mux channel for INA238 mux heat 1.
#define PBM_T1_I2C_MUX_CH_PwrMon2 			TCA9548_CH4 // Number I2C Mux channel for INA238 mux heat 2.

#define PBM_T1_GPIO_CH_Heat_CMD_1 			PCA9534_IO_P00 // Number GPIO Mux channel for set ON/OFF heat command branch 1.
#define PBM_T1_GPIO_CH_Heat_State_1			PCA9534_IO_P01 // Number GPIO Mux channel for read real heat state branch 1.
#define PBM_T1_GPIO_CH_Heat_CMD_2 			PCA9534_IO_P03 // Number GPIO Mux channel for set ON/OFF heat command branch 1.
#define PBM_T1_GPIO_CH_Heat_State_2			PCA9534_IO_P04 // Number GPIO Mux channel for read real heat state branch 1.
#define PBM_T1_GPIO_CH_EmergChrg_1 			PCA9534_IO_P02 // Number GPIO Mux channel for ON/OFF emergency charge branch 1.
#define PBM_T1_GPIO_CH_EmergChrg_2 			PCA9534_IO_P05 // Number GPIO Mux channel for ON/OFF emergency charge branch 2.

typedef struct{

	uint8_t  I2C_MUX_Addr;
	uint8_t  I2C_MUX_Ch_GPIO;
	uint8_t  I2C_MUX_Ch_Branch;
	uint8_t  I2C_MUX_Ch_Heat;
	uint8_t  I2C_MUX_Ch_PwrMon;
	uint8_t  I2C_MUX_Ch_TempSens;

	uint8_t  GPIO_Addr;
	uint8_t  PwrMon_Addr;

	uint8_t  INA238_Convr_Time;
	uint8_t  INA238_Aver_Count;
	uint8_t  INA238_ADC_Range;
	uint8_t  INA238_Mode;

	uint8_t  TMP1075_Mode;
	uint8_t  TMP1075_Alert_Level;
	uint8_t  TMP1075_Mode_Alert;
	uint8_t  TMP1075_Convr_Time;
	uint8_t  TMP1075_Alert_Sens;

	uint8_t  TempSens_Heat_Addr[PBM_T1_HEAT_TEMPSENS_QUANTITY];
	//uint8_t  TempSens_Addr[PBM_T1_TEMPSENS_QUANTITY];

	uint8_t  GPIO_Pin_Heat_CMD;
	uint8_t  GPIO_Pin_Heat_State;
	uint8_t  GPIO_Pin_TempSens;
	uint8_t  GPIO_Pin_EmergChrg;


} _PBM_T1_table;

_PBM_T1_table PBM_T1_Table(uint8_t PBM_number, uint8_t Branch, uint8_t Heat);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_CONFIG_H_ */
