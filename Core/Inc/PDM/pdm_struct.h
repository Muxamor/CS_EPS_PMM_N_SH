#ifndef INC_PDM_PDM_STRUCT_H_
#define INC_PDM_PDM_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PDM/pdm_config.h"

//#pragma pack(push, 1)

typedef struct{
	
	uint16_t Voltage_val;
	int16_t Current_val;
	uint32_t Power_val;

	uint8_t State_eF  		:1; //0-DISABLE, 1-ENABLE. State power channel input Efuse // Need to save in flash.
	//uint8_t State_eF_out 		:1; //0-DISABLE, 1-ENABLE. State power channel output Efuse // Need to save in flash.
	uint8_t PG_eF			:1; //0-OK, 1-ERROR. Power good channel status input eFuse
	//uint8_t PG_eF_out 	 		:1; //0-OK, 1-ERROR. Power good channel status output eFuse
	uint8_t :6;

	uint8_t Error_State_eF  	:1; //0-OK, 1-ERROR.
	//uint8_t Error_State_eF_out 	:1;
	uint8_t Error_PWR_Mon 		:1;
	uint8_t :6;

}_PDM_PWR_CH;

typedef struct{

	_PDM_PWR_CH PWR_Channel[PDM_PWR_Ch_quantity];

	int8_t Temp_sensor[4]; //PDM temperature

	uint8_t Error_I2C_GPIO_Ext1 	:1;
	uint8_t Error_I2C_GPIO_Ext2 	:1;
	uint8_t Error_I2C_MUX_1		 	:1;
	uint8_t Error_I2C_MUX_2		 	:1;
	uint8_t Error_temp_sensor_1 	:1;
	uint8_t Error_temp_sensor_2 	:1;
	uint8_t Error_temp_sensor_3 	:1;
	uint8_t Error_temp_sensor_4		:1;
	//uint8_t :1;
	
	uint8_t PDM_save_conf_flag  	:1;// 0-Note need save, 1-Need save configure PDM to FRAM.
	uint8_t :7;

	uint8_t PDM_ID_module;        //identifier of PDM // Need to save in flash
	uint8_t Alignment_structure1; //Alignment

	uint16_t Reserve_1;
	uint16_t Reserve_2;
}_PDM;

//#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* INC_PDM_PDM_STRUCT_H_ */
