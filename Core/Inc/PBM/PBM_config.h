/*
 * PBM_config.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_CONFIG_H_
#define INC_PBM_CONFIG_H_


/*********************** Configuration for PBM module ***********************/

#define PBM_I2C_PORT                		I2C4 	//Number of I2C port for PBM.
#define PBM_I2C_ATTEMPT_CONN                3 	    //Quantity attempt to connection through I2C.
#define PBM_i2c_delay_att_conn				5 	 	//ms Delay between attempts connection through I2C in millisecond
#define PBM_QUANTITY 						0x03 // Total quantity power channels on the PDM

#define PBM_1_DS2777_ADDR_1                 0x60 	//Address DS2777 mux �1 Battery Module �1.
#define PBM_1_DS2777_ADDR_2                 0x61 	//Address DS2777 mux �2 Battery Module �1.
#define PBM_2_DS2777_ADDR_1                 0x62 	//Address DS2777 mux �1 Battery Module �2.
#define PBM_2_DS2777_ADDR_2                 0x63 	//Address DS2777 mux �2 Battery Module �2.
#define PBM_3_DS2777_ADDR_1                 0x64 	//Address DS2777 mux �1 Battery Module �3.
#define PBM_3_DS2777_ADDR_2                 0x65 	//Address DS2777 mux �2 Battery Module �3.

#define PBM_1_PCA9534_ADDR                  0x38 	//Address GPIO mux PCA9534ARGTR Battery Module �1.
#define PBM_2_PCA9534_ADDR                  0x39 	//Address GPIO mux PCA9534ARGTR Battery Module �2.
#define PBM_3_PCA9534_ADDR                  0x3A 	//Address GPIO mux PCA9534ARGTR Battery Module �3.

#define PBM_1_TMP1075_ADDR_1                0x48 	//Address Temperature Sensor 1 TMP1075 Battery Module �1.
#define PBM_1_TMP1075_ADDR_2                0x49 	//Address Temperature Sensor 2 TMP1075 Battery Module �1.
#define PBM_1_TMP1075_ADDR_3                0x4A 	//Address Temperature Sensor 3 TMP1075 Battery Module �1.
#define PBM_1_TMP1075_ADDR_4                0x4B 	//Address Temperature Sensor 4 TMP1075 Battery Module �1.

#define PBM_2_TMP1075_ADDR_1                0x4C 	//Address Temperature Sensor 1 TMP1075 Battery Module �1.
#define PBM_2_TMP1075_ADDR_2                0x4D 	//Address Temperature Sensor 2 TMP1075 Battery Module �1.
#define PBM_2_TMP1075_ADDR_3                0x4E 	//Address Temperature Sensor 3 TMP1075 Battery Module �1.
#define PBM_2_TMP1075_ADDR_4                0x4F 	//Address Temperature Sensor 4 TMP1075 Battery Module �1.

#define PBM_3_TMP1075_ADDR_1                0x40 	//Address Temperature Sensor 1 TMP1075 Battery Module �1.
#define PBM_3_TMP1075_ADDR_2                0x42 	//Address Temperature Sensor 2 TMP1075 Battery Module �1.
#define PBM_3_TMP1075_ADDR_3                0x44 	//Address Temperature Sensor 3 TMP1075 Battery Module �1.
#define PBM_3_TMP1075_ADDR_4                0x46 	//Address Temperature Sensor 4 TMP1075 Battery Module �1.

#define PBM_TMP1075_TEMP_HI                	8.0 	//Hi limit for comparison TMP1075 Battery Module in �C.
#define PBM_TMP1075_TEMP_LO                	2.0 	//Lo limit for comparison TMP1075 Battery Module in �C.

#define PBM_1                				0x00 	//Designation for choose PBM �1.
#define PBM_2                				0x01 	//Designation for choose PBM �2.
#define PBM_3                				0x02	//Designation for choose PBM �3.

#define PBM_BRANCH_1                		0x01 	//Designation for choose branch �1 PBM.
#define PBM_BRANCH_2                		0x02 	//Designation for choose branch �2 PBM.
#define PBM_BRANCH_ALL                		0x00 	//Designation for choose all branch PBM.

#define PBM_ON_HEAT                			0x01 	//Enable heat branch.
#define PBM_OFF_HEAT                		0x00 	//Disable heat branch.

#define PBM_ON_CHARGE                		0x01 	//Enable charge branch.
#define PBM_OFF_CHARGE                		0x00 	//Disable charge branch.

#define PBM_ON_DISCHARGE                	0x01 	//Enable discharge branch.
#define PBM_OFF_DISCHARGE                	0x00 	//Disable discharge branch.

typedef struct{

	uint8_t  BRANCH_1_Addr;
	uint8_t  BRANCH_2_Addr;
	uint8_t  GPIO_Addr;
	uint8_t  TEMP_SENSOR_1_Addr;
	uint8_t  TEMP_SENSOR_2_Addr;
	uint8_t  TEMP_SENSOR_3_Addr;
	uint8_t  TEMP_SENSOR_4_Addr;

	uint8_t  GPIO_INPUT_PIN;

} _PBM_table;

_PBM_table PBM_Table(uint8_t PBM_number);


#endif /* INC_PBM_CONFIG_H_ */
