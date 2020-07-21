
#ifndef INC_PMM_PMM_STRUCT_H_
#define INC_PMM_PMM_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx.h"

//#pragma pack(push, 1)

typedef struct{

	uint32_t PWR_Ch_State_CANmain				:1; //0-DISABLE, 1-ENABLE. State power main CAN.
	uint32_t PWR_Ch_State_CANbackup				:1; //0-DISABLE, 1-ENABLE. State power backup CAN.
	uint32_t PWR_Ch_State_Vbat1_eF1   			:1; //0-DISABLE, 1-ENABLE. State power battery bus output 1 eFuse 1 
	uint32_t PWR_Ch_State_Vbat1_eF2				:1; //0-DISABLE, 1-ENABLE. State power battery bus output 1 eFuse 2 
	uint32_t PWR_Ch_State_Vbat2_eF1     		:1; //0-DISABLE, 1-ENABLE. State power battery bus output 2 eFuse 1 
	uint32_t PWR_Ch_State_Vbat2_eF2				:1; //0-DISABLE, 1-ENABLE. State power battery bus output 2 eFuse 2
	uint32_t PWR_Ch_State_PBMs_Logic			:1; //0-DISABLE, 1-ENABLE. State power PBM -modules logic power 
	uint32_t PWR_Ch_State_Deploy_Logic 			:1; //0-DISABLE, 1-ENABLE. State power Deploy logic 
	uint32_t PWR_Ch_State_Deploy_Power 			:1; //0-DISABLE, 1-ENABLE. State power Deploy power 
	uint32_t PWR_Ch_State_5V_Bus 				:1; //0-DISABLE, 1-ENABLE. State power 5V bus (Norbi not use)
	uint32_t PWR_Ch_State_3_3V_Bus 				:1; //0-DISABLE, 1-ENABLE. State power 3.3V bus (Norbi not use)
	uint32_t PWR_Ch_State_I2C_Bus 				:1; //0-DISABLE, 1-ENABLE. State power I2C bus.(Norbi not use)
	uint32_t PWR_Supply_Main_PG 				:1; //0-OK, 1-ERROR. Power good Main power supply 3.3V
	uint32_t PWR_Supply_Backup_PG 				:1; //0-OK, 1-ERROR. Power good Backup power supply 3.3V
	uint32_t PWR_Ch_PG_CANmain					:1; //0-OK, 1-ERROR. Power good main CAN power ch.
	uint32_t PWR_Ch_PG_CANbackup				:1; //0-OK, 1-ERROR. Power good backup CAN power ch.
	uint32_t PWR_Ch_PG_Vbat1_eF1 				:1; //0-OK, 1-ERROR. Power good VBAT1 eF1 channel status.
	uint32_t PWR_Ch_PG_Vbat1_eF2 				:1; //0-OK, 1-ERROR. Power good VBAT1 eF2 channel status.
	uint32_t PWR_Ch_PG_Vbat2_eF1 				:1; //0-OK, 1-ERROR. Power good VBAT2 eF1 channel status.
	uint32_t PWR_Ch_PG_Vbat2_eF2 				:1; //0-OK, 1-ERROR. Power good VBAT2 eF2 channel status.
	uint32_t PWR_Ch_PG_PBMs_Logic 				:1; //0-OK, 1-ERROR. Power good PBM logic power.
	uint32_t PWR_Ch_PG_Deploy_LP				:1; //0-OK, 1-ERROR. Power good Deploy power. (Norbi burning eFuse)
	uint32_t PWR_Ch_PG_5V_Bus 					:1; //0-OK, 1-ERROR. Power good 5V power. (Norbi not use)
	uint32_t PWR_Ch_PG_3_3V_Bus 				:1; //0-OK, 1-ERROR. Power good 3.3V power. (Norbi not use)
	uint32_t PWR_Ch_PG_I2C_Bus 					:1; //0-OK, 1-ERROR. Power good I2C power. (Norbi not use)
	uint32_t :7;


	uint32_t Error_TMP1075_sensor				:1;	//=0 -OK, = 1-error temp. sensor TMP1075
	uint32_t Error_PWR_Ch_State_CANmain			:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_CANbackup		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_Vbat1_eF1		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_Vbat1_eF2		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_Vbat2_eF1		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_Vbat2_eF2		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_PBMs_Logic		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_Deploy_Logic	:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_Deploy_Power	:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_5V_Bus			:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_3_3V_Bus		:1; //=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Ch_State_I2C_Bus			:1;	//=0 -OK, = 1-error at output GPIO Ext1
	uint32_t Error_PWR_Mon_Vbat1_eF1			:1; //=0 -OK, = 1-error I2C INA231
	uint32_t Error_PWR_Mon_Vbat1_eF2			:1; //=0 -OK, = 1-error I2C INA231
	uint32_t Error_PWR_Mon_Vbat2_eF1			:1; //=0 -OK, = 1-error I2C INA231
	uint32_t Error_PWR_Mon_Vbat2_eF2			:1; //=0 -OK, = 1-error I2C INA231
	uint32_t Error_I2C_GPIO_Ext1 				:1;	//=0 -OK, = 1-error I2C GPIO Expander 1 TCA9539 (U3)
	uint32_t Error_I2C_GPIO_Ext2 				:1;
	uint32_t Error_PWR_Supply_m_b_Curr_Mon		:1; //=0 -OK, = 1-error I2C Current meas. ADC (U65).
	uint32_t Error_FRAM1		 				:1; //=0 -OK, = 1-error FRAM1 no connection
	uint32_t Error_FRAM2		 				:1; //=0 -OK, = 1-error FRAM2 no connection
	uint32_t Error_UART_port_M					:1; //=0 -OK, = 1-error UART main (LPUART1) port
	uint32_t Error_UART_port_B					:1; //=0 -OK, = 1-error UART backup (USART3) port
	uint32_t Error_CAN_port_M					:1; //=0 -OK, = 1-error UART main (CAN1) port
	uint32_t Error_CAN_port_B					:1; //=0 -OK, = 1-error UART backup (CAN2) port
	uint32_t :6;

	uint32_t reboot_counter_CPUm; 			//increment this counter after each reboot CPUm(power up).
	uint32_t reboot_counter_CPUb; 			//increment this counter after each reboot CPUb(power up).

	uint16_t PWR_Ch_Vbat1_eF1_Voltage_val;
	uint16_t PWR_Ch_Vbat1_eF2_Voltage_val;
	int16_t  PWR_Ch_Vbat1_eF1_Current_val;
	int16_t  PWR_Ch_Vbat1_eF2_Current_val;
	uint16_t PWR_Ch_Vbat1_eF1_Power_val;
	uint16_t PWR_Ch_Vbat1_eF2_Power_val;

	uint16_t PWR_Ch_Vbat2_eF1_Voltage_val;
	uint16_t PWR_Ch_Vbat2_eF2_Voltage_val;
	int16_t  PWR_Ch_Vbat2_eF1_Current_val;
	int16_t  PWR_Ch_Vbat2_eF2_Current_val;
	uint16_t PWR_Ch_Vbat2_eF1_Power_val;
	uint16_t PWR_Ch_Vbat2_eF2_Power_val; 

	int16_t  PWR_Supply_Main_eF_in_Current_val;
	int16_t  PWR_Supply_Main_eF_out_Current_val;
	int16_t  PWR_Supply_Backup_eF_in_Current_val;
	int16_t  PWR_Supply_Backup_eF_out_Current_val;

	uint8_t Main_Backup_mode_CPU 	:1; //= 1-Main, =0-Backup mode CPU. 
	uint8_t Active_CPU		 		:1;	//= 0-Main CPU is active, Backup CPU is passive.
										//= 1-Main CPU is passive, Backup CPU is active.
	uint8_t CAN_constatnt_mode		:1;	//= 1-Constant mode CANreg5 is ON, 0-Constant CANreg5 is OFF.
	uint8_t	:5;

	uint8_t EPS_Mode;                   // Mode of operation EPS: 0xFF - Service mode.

	uint8_t PMM_save_conf_flag  	:1;// 0-Note need save, 1-Need save configure PDM to FRAM and sent to neighboring CPU
	uint8_t :7;

	int8_t Temp_sensor; //PMM temperature

	uint16_t Reserve_1;
	uint16_t Reserve_2;

}_PMM;

//#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_STRUCT_H_ */
