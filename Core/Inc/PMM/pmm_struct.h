
#ifndef INC_PMM_PMM_STRUCT_H_
#define INC_PMM_PMM_STRUCT_H_

#include "stm32l4xx.h"

typedef struct{

	uint8_t Main_Backup_mode_CPU 	:1; //= 1-Main, =0-Backup mode CPU. 
	uint8_t Detect_Active_CPU 		:1;	//= 0-Main CPU is active, Backup CPU is passive.
										//= 1-Main CPU is passive, Backup CPU is active.
	uint8_t CAN_constatnt_mode		:1;	//= 1-Constant mode CANreg5 is ON, 0-Constant CANreg5 is OFF.
	uint8_t	:5;

	uint8_t EPS_Mode;                   // Mode of operation EPS: 0xFF - Service mode.

	
	uint16_t PWR_State_CANmain			:1; //0-DISABLE, 1-ENABLE. State power main CAN.
	uint16_t PWR_State_CANbackup		:1; //0-DISABLE, 1-ENABLE. State power backup CAN.
	uint16_t PWR_State_Vbat1_eF1   		:1; //0-DISABLE, 1-ENABLE. State power battery bus output 1 eFuse 1 
	uint16_t PWR_State_Vbat1_eF2		:1; //0-DISABLE, 1-ENABLE. State power battery bus output 1 eFuse 2 
	uint16_t PWR_State_Vbat2_eF1     	:1; //0-DISABLE, 1-ENABLE. State power battery bus output 2 eFuse 1 
	uint16_t PWR_State_Vbat2_eF2		:1; //0-DISABLE, 1-ENABLE. State power battery bus output 2 eFuse 2
	uint16_t PWR_State_PBMs_Logic		:1; //0-DISABLE, 1-ENABLE. State power PBM -modules logic power 
	uint16_t PWR_State_Deploy_Logic 	:1; //0-DISABLE, 1-ENABLE. State power Deploy logic 
	uint16_t PWR_State_Deploy_Power 	:1; //0-DISABLE, 1-ENABLE. State power Deploy power 
	uint16_t PWR_State_5V_Bus 			:1; //0-DISABLE, 1-ENABLE. State power 5V bus 
	uint16_t PWR_State_3_3V_Bus 		:1; //0-DISABLE, 1-ENABLE. State power 3.3V bus 
	uint16_t :5;

	uint32_t Error_TMP1075_sensor			:1;	//=0 -OK, = 1-error temp. sensor TMP1075
	uint32_t Error_PWR_State_CANmain		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_CANbackup		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_Vbat1_eF1		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_Vbat1_eF2		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_Vbat2_eF1		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_Vbat2_eF2		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_PBMs_Logic		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_Deploy_Logic	:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_Deploy_Power	:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_5V_Bus			:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_State_3_3V_Bus		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_I2C_GPIO_Ext1 			:1;	//=0 -OK, = 1-error I2C GPIO Expander 1 TCA9539 (U3)
	uint32_t Error_I2C_GPIO_Ext2 			:1;
	uint32_t Error_FRAM1		 			:1; //=0 -OK, = 1-error FRAM1 no connection
	uint32_t Error_FRAM2		 			:1; //=0 -OK, = 1-error FRAM2 no connection
	uint32_t :16;

	uint32_t reboot_counter_CPUm; 			//increment this counter after each reboot CPUm(power up).
	uint32_t reboot_counter_CPUb; 			//increment this counter after each reboot CPUb(power up).


}_PMM;

#endif /* INC_PMM_PMM_STRUCT_H_ */
