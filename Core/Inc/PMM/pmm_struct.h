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
	uint8_t	:6;	

	uint8_t CAN_constatnt_mode;			//= 1-Constant mode CANreg5 is ON, 0-Constant CANreg5 is OFF. 

	uint32_t error_TMP1075_senser:1;	//= 1-error temp. sensor TMP1075, =0 -OK
	uint32_t :31;

	uint32_t reboot_counter; 			//increment this counter after each rebootv (power up).


}_PMM;

#endif /* INC_PMM_PMM_STRUCT_H_ */
