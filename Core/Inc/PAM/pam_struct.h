
#ifndef INC_PAM_PAM_STRUCT_H_
#define INC_PAM_PAM_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "pam_config.h"

typedef struct{

	int8_t Temp_sensor[4]; 				//SP temperature

	uint8_t State_ID_out_1			:1; //SP state ideal diode
	uint8_t State_ID_out_2			:1; //SP state ideal diode
	uint8_t State_ID_out_3			:1; //SP state ideal diode
	uint8_t State_ID_out_4			:1; //SP state ideal diode
	uint8_t State_ID_out_5			:1; //SP state ideal diode
	uint8_t State_ID_out_6			:1; //SP state ideal diode

	uint8_t State_eF_out 			:1; //0-DISABLE, 1-ENABLE. State power channel output Efuse.
	uint8_t PG_eF_out				:1; //0-OK, 1-ERROR. Fault status channel output eFuse.
	uint8_t :2;

	uint8_t Error_temp_sensor_1 	:1;
	uint8_t Error_temp_sensor_2 	:1;
	uint8_t Error_temp_sensor_3 	:1;
	uint8_t Error_temp_sensor_4		:1;
	uint8_t Error_I2C_GPIO_Ext	 	:1;
	uint8_t :3;
	

}_PAM_SP;

typedef struct{
	
	uint16_t Voltage_val;
	uint16_t Current_val;
	uint16_t Power_val;

	uint8_t State_ID_In  		:1; //0-Open, 1-Close. Show State power channel input ideal diode.
	uint8_t Error_PWR_Mon 		:1;
	uint8_t :6;

}_PAM_PWR_IN_CH;

typedef struct{

	_PAM_PWR_IN_CH PWR_IN_Channel[PAM_PWR_IN_Ch_quantity];
	//_PAM_SP SP_Channel [PAM_SP_quantity];

	int8_t Temp_sensor[4]; //PAM temperature

	uint8_t State_DC_DC 			:1; // Need to save in flash
	uint8_t State_LDO 				:1; // Need to save in flash
	uint8_t PG_DC_DC 				:1;
	uint8_t PG_LDO 					:1;
	uint8_t PAM_save_conf_flag		:1;
	uint8_t :3;
	
	uint8_t Error_I2C_GPIO_Ext	 	:1;
	uint8_t Error_I2C_MUX_1		 	:1;
	uint8_t Error_I2C_MUX_2		 	:1;
	uint8_t Error_temp_sensor_1 	:1;
	uint8_t Error_temp_sensor_2 	:1;
	uint8_t Error_temp_sensor_3 	:1;
	uint8_t Error_temp_sensor_4		:1;
	uint8_t :1;

	uint16_t reserved1;
	uint16_t reserved2;

}_PAM;

//#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* INC_PDM_PDM_STRUCT_H_ */
