/*
 * PBM_config.h
 *
 *  Created on: Apr 23, 2020
 *      Author: A Morsin
 */

#ifndef INC_PBM_CONFIG_H_
#define INC_PBM_CONFIG_H_


/*********************** Configuration for PBM module ***********************/

#define PBM_1_DS2777_ADDR_1                 0x50 	//Address DS2777 mux ¹1 Battery Module ¹1.
#define PBM_1_DS2777_ADDR_2                 0x58 	//Address DS2777 mux ¹2 Battery Module ¹1.
#define PBM_2_DS2777_ADDR_1                 0x70 	//Address DS2777 mux ¹1 Battery Module ¹2.
#define PBM_2_DS2777_ADDR_2                 0x70 	//Address DS2777 mux ¹2 Battery Module ¹2.
#define PBM_3_DS2777_ADDR_1                 0x70 	//Address DS2777 mux ¹1 Battery Module ¹3.
#define PBM_3_DS2777_ADDR_2                 0x70 	//Address DS2777 mux ¹2 Battery Module ¹3.

#define PBM_1_PCA9534_ADDR                  0x38 	//Address GPIO mux PCA9534ARGTR Battery Module ¹1.
#define PBM_2_PCA9534_ADDR                  0x39 	//Address GPIO mux PCA9534ARGTR Battery Module ¹2.
#define PBM_3_PCA9534_ADDR                  0x3A 	//Address GPIO mux PCA9534ARGTR Battery Module ¹3.

#define PBM_1_TMP1075_ADDR_1                0x48 	//Address Temperature Sensor 1 TMP1075 Battery Module ¹1.
#define PBM_1_TMP1075_ADDR_2                0x49 	//Address Temperature Sensor 2 TMP1075 Battery Module ¹1.
#define PBM_1_TMP1075_ADDR_3                0x4A 	//Address Temperature Sensor 3 TMP1075 Battery Module ¹1.
#define PBM_1_TMP1075_ADDR_4                0x4B 	//Address Temperature Sensor 4 TMP1075 Battery Module ¹1.

#define PBM_2_TMP1075_ADDR_1                0x4C 	//Address Temperature Sensor 1 TMP1075 Battery Module ¹1.
#define PBM_2_TMP1075_ADDR_2                0x4D 	//Address Temperature Sensor 2 TMP1075 Battery Module ¹1.
#define PBM_2_TMP1075_ADDR_3                0x4E 	//Address Temperature Sensor 3 TMP1075 Battery Module ¹1.
#define PBM_2_TMP1075_ADDR_4                0x4F 	//Address Temperature Sensor 4 TMP1075 Battery Module ¹1.

#define PBM_3_TMP1075_ADDR_1                0x40 	//Address Temperature Sensor 1 TMP1075 Battery Module ¹1.
#define PBM_3_TMP1075_ADDR_2                0x42 	//Address Temperature Sensor 2 TMP1075 Battery Module ¹1.
#define PBM_3_TMP1075_ADDR_3                0x44 	//Address Temperature Sensor 3 TMP1075 Battery Module ¹1.
#define PBM_3_TMP1075_ADDR_4                0x46 	//Address Temperature Sensor 4 TMP1075 Battery Module ¹1.

#define PBM_TMP1075_TEMP_HI                	5.0 	//Hi limit for comparison TMP1075 Battery Module in °C.
#define PBM_TMP1075_TEMP_LO                	2.0 	//Lo limit for comparison TMP1075 Battery Module in °C.

#define PBM_1                				0x01 	//Designation for choose PBM ¹1.
#define PBM_2                				0x02 	//Designation for choose PBM ¹2.
#define PBM_3                				0x03	//Designation for choose PBM ¹3.
#define PBM_ALL                				0x00 	//Designation for choose all PBM.

#define PBM_BRANCH_1                		0x01 	//Designation for choose branch ¹1 PBM.
#define PBM_BRANCH_2                		0x02 	//Designation for choose branch ¹2 PBM.
#define PBM_BRANCH_ALL                		0x00 	//Designation for choose all branch PBM.

#define PBM_ON_HEAT                			0x01 	//Enable heat branch.
#define PBM_OFF_HEAT                		0x00 	//Disable heat branch.

#define PBM_ON_CHARGE                		0x01 	//Enable charge branch.
#define PBM_OFF_CHARGE                		0x00 	//Disable charge branch.

#define PBM_ON_DISCHARGE                	0x01 	//Enable discharge branch.
#define PBM_OFF_DISCHARGE                	0x00 	//Disable discharge branch.


#endif /* INC_PBM_CONFIG_H_ */
