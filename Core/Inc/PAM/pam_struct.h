
#ifndef INC_PAM_PAM_STRUCT_H_
#define INC_PAM_PAM_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PAM/pam_config.h"

typedef struct{

	uint8_t State_ID [PAM_SP_ID_quantity];		    		//SP state ideal diode 0-Open, 1-Close.
	int8_t Temp_sensor [PAM_SP_Temp_sensor_quantity]; 			//SP temperature
	uint8_t Error_temp_sensor [PAM_SP_Temp_sensor_quantity];

	//uint8_t PG_eF_out:1;										//0-OK, 1-ERROR. Fault status channel output eFuse.
	//uint8_t State_eF_out:1;										//0-DISABLE, 1-ENABLE. Set State power channel output Efuse. // Need to save in flash
	uint8_t Error_State_eF_in:1; 								//0-OK, 1-ERROR.
	uint8_t Error_I2C_GPIO_Ext:1;								//0-OK, 1-ERROR.
	uint8_t :4;

	uint8_t PAM_ID_SP;

}_PAM_SP;

typedef struct{
	
	uint16_t Voltage_val;
	int16_t Current_val;
	uint16_t Power_val;

	uint8_t State_ID_In  		:1; //0-Open, 1-Close. Show State power channel input ideal diode.
	uint8_t Error_PWR_Mon 		:1;
	uint8_t :6;

    uint8_t Alignment_structure; 	//Alignment

}_PAM_PWR_IN_CH;

typedef struct{

	_PAM_PWR_IN_CH PWR_IN_Channel[PAM_PWR_IN_Ch_quantity];
	_PAM_SP SP_Channel [PAM_SP_quantity];

	int8_t Temp_sensor[4]; //PAM temperature

   // uint8_t State_eF_L_out_SP 			:1; // Need to save in flash ??????????????????????

	uint16_t State_DC_DC 			:1; // Need to save in flash
	uint16_t State_LDO 				:1; // Need to save in flash
	uint16_t PG_DC_DC 				:1;
	uint16_t PG_LDO 				:1;
    uint16_t Error_State_DC_DC      :1;
    uint16_t Error_State_LDO        :1;
	uint16_t Error_I2C_GPIO_Ext	 	:1;
	uint16_t Error_I2C_MUX_1		:1;
	uint16_t Error_I2C_MUX_2		:1;
	uint16_t Error_temp_sensor_1 	:1;
	uint16_t Error_temp_sensor_2 	:1;
	uint16_t Error_temp_sensor_3 	:1;
	uint16_t Error_temp_sensor_4	:1;
    uint16_t PAM_save_conf_flag		:1;
	uint16_t :2;

	uint8_t PAM_ID_module;        //identifier of PAM
	uint8_t Alignment_structure; //Alignment

	uint16_t reserved1;
	uint16_t reserved2;

}_PAM;

//#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* INC_PAM_PAM_STRUCT_H_ */
