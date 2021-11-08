
#ifndef INC_PBM_T1_CONFIG_H_
#define INC_PBM_T1_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************** Configuration for PBM module ***********************/

#define PBM_T1_I2C_PORT                		I2C4 	//Number of I2C port for PBM.
#define PBM_T1_I2C_ATTEMPT_CONN             3 	    //Quantity attempt to connection through I2C.
#define PBM_T1_i2c_delay_att_conn			3 	 	//ms Delay between attempts connection through I2C in millisecond
#define PBM_T1_QUANTITY 					0x04 	// Total quantity power channels on the PDM

#define PBM_T1_1_TCA9548A_ADDR              0x70 	//Address GPIO mux PCA9534ARGTR Battery Module №1.
#define PBM_T1_2_TCA9548A_ADDR              0x71 	//Address GPIO mux PCA9534ARGTR Battery Module №2.
#define PBM_T1_3_TCA9548A_ADDR              0x72 	//Address GPIO mux PCA9534ARGTR Battery Module №3.
#define PBM_T1_4_TCA9548A_ADDR              0x73 	//Address GPIO mux PCA9534ARGTR Battery Module №4.

#define PBM_T1_PCA9534_ADDR               	0x38 	//Address GPIO mux PCA9534ARGTR Battery Module.

#define PBM_T1_INA_ADDR_1					0x40 	//Address INA mux 1 Battery Module.
#define PBM_T1_INA_ADDR_2					0x41 	//Address INA mux 2 Battery Module.

#define PBM_T1_TMP1075_ADDR_1               0x48 	//Address Temperature Sensor 1 TMP1075 Battery Module.
#define PBM_T1_TMP1075_ADDR_2               0x49 	//Address Temperature Sensor 2 TMP1075 Battery Module.
#define PBM_T1_TMP1075_ADDR_3               0x4A 	//Address Temperature Sensor 3 TMP1075 Battery Module.
#define PBM_T1_TMP1075_ADDR_4               0x4B 	//Address Temperature Sensor 4 TMP1075 Battery Module.

#define PBM_T1_TMP1075_TEMP_HI             	10.0 	//Hi limit for comparison TMP1075 Battery Module in °C.
#define PBM_T1_TMP1075_TEMP_LO             	2.0 	//Lo limit for comparison TMP1075 Battery Module in °C.
#define PBM_T1_TMP1075_Hi_Limit				20.0	//Hi error limit for comparison TMP1075 Battery Module in °C.

#define PBM_T1_1                			0x00 	//Designation for choose PBM №1.
#define PBM_T1_2                			0x01 	//Designation for choose PBM №2.
#define PBM_T1_3                			0x02	//Designation for choose PBM №3.
#define PBM_T1_4                			0x03	//Designation for choose PBM №3.

#define PBM_T1_BRANCH_1                		0x01 	//Designation for choose branch №1 PBM.
#define PBM_T1_BRANCH_2                		0x02 	//Designation for choose branch №2 PBM.
#define PBM_T1_BRANCH_ALL                	0x00 	//Designation for choose all branch PBM.

#define PBM_T1_ON_HEAT                		0x01 	//Enable heat branch.
#define PBM_T1_OFF_HEAT                		0x00 	//Disable heat branch.

#define PBM_T1_ON_CHARGE               		0x01 	//Enable charge branch.
#define PBM_T1_OFF_CHARGE              		0x00 	//Disable charge branch.

#define PBM_T1_ON_DISCHARGE                	0x01 	//Enable discharge branch.
#define PBM_T1_OFF_DISCHARGE               	0x00 	//Disable discharge branch.

#define PBM_T1_ON_SDWN_CHRG          		0x01 	//Enable emergency charge.
#define PBM_T1_OFF_SDWN_CHRG         		0x00 	//Disable emergency charge.

#define PBM_T1_R_SENSE           			10 		//Resistance of current shunt im mOhm.
#define PBM_T1_MAX_BATT_CAP           		3100 	//Maximum branch battery capacity in mAh.

#define PBM_T1_NORMAL_ENERGY_EDGE           7200 	//Voltage in mV for hysteresis loop warning low energy PBM.
#define PBM_T1_LOW_ENERGY_EDGE              6500 	//Voltage in mV for warning low energy PBM.
#define PBM_T1_ZERO_ENERGY_EDGE             6000 	//Voltage in mV for off all payload.



typedef struct{

	uint8_t  I2C_MUX_Addr;
	uint8_t  GPIO_Addr;
	uint8_t  INA_1_Addr;
	uint8_t  INA_2_Addr;
	uint8_t  TEMP_SENSOR_1_Addr;
	uint8_t  TEMP_SENSOR_2_Addr;
	uint8_t  TEMP_SENSOR_3_Addr;
	uint8_t  TEMP_SENSOR_4_Addr;

	uint8_t  GPIO_INPUT_PIN;

} _PBM_T1_table;

_PBM_T1_table PBM_T1_Table(uint8_t PBM_number);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_CONFIG_H_ */
