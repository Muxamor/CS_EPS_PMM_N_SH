/*
 * pmm_struct.h
 *
 *  Created on: Apr 23, 2020
 *      Author: Ivan
 */

#ifndef INC_PMM_PMM_STRUCT_H_
#define INC_PMM_PMM_STRUCT_H_

#include "stm32l4xx.h"

typedef struct{

	uint8_t Main_Backup_mode_CPU 	:1; //= 1-Main, =0-Backup mode CPU. 
	uint8_t Detect_Active_CPU 		:1;	//= 1-Main CPU is activ, Backup CPU is passive. 
										//= 0-Main CPU is passive, Backup CPU is activ. 
	uint8_t CAN_constatnt_mode		:1;	//= 1-Constant mode CANreg5 is ON, 0-Constant CANreg5 is OFF.
	uint8_t	:5;

	uint8_t EPS_Mode;                   // Mode of operation EPS

	
	uint8_t PWR_State_CANmain		:1; 	//0-DISABLE, 1-ENABLE. State power main CAN.
	uint8_t PWR_State_CANbackup		:1; 	//0-DISABLE, 1-ENABLE. State power backup CAN.
	uint8_t PWR_State_Vbat1_eF1     :1;
	uint8_t PWR_State_Vbat1_eF2		:1;
	uint8_t PWR_State_Vbat2_eF1     :1;
	uint8_t PWR_State_Vbat2_eF2		:1;
	uint8_t :2;
	
	uint32_t Error_TMP1075_senser		:1;	//=0 -OK, = 1-error temp. sensor TMP1075
	uint32_t Error_PWR_State_CANmain	:1;
	uint32_t Error_PWR_State_CANbackup	:1;
	uint32_t Error_I2C_GPIO_Ext1 		:1;	//=0 -OK, = 1-error I2C GPIO Expander 1 TCA9539 (U3)
	uint32_t Error_I2C_GPIO_Ext2 		:1;
	uint32_t :27;

	uint32_t reboot_counter; 			//increment this counter after each rebootv (power up).


}_PMM;

#endif /* INC_PMM_PMM_STRUCT_H_ */
