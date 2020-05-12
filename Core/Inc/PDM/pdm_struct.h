
#ifndef INC_PDM_PDM_STRUCT_H_
#define INC_PDM_PDM_STRUCT_H_

typedef struct{

	uint16_t PWR_CH1_Voltage;  //Channel voltage value in mV
	uint16_t PWR_CH2_Voltage;
	uint16_t PWR_CH3_Voltage;
	uint16_t PWR_CH4_Voltage;
	uint16_t PWR_CH5_Voltage;
	uint16_t PWR_CH6_Voltage;

	int16_t PWR_CH1_Current;  //Channel current value in mA
	int16_t PWR_CH2_Current;
	int16_t PWR_CH3_Current;
	int16_t PWR_CH4_Current;
	int16_t PWR_CH5_Current;
	int16_t PWR_CH6_Current;

	uint16_t PWR_CH1_Power;  //Channel power value in mW
	uint16_t PWR_CH2_Power;
	uint16_t PWR_CH3_Power;
	uint16_t PWR_CH4_Power;
	uint16_t PWR_CH5_Power;
	uint16_t PWR_CH6_Power;

	//State power channel which need ON or OFF 
	uint16_t PWR_CH1_State_eF_in  :1; //0-DISABLE, 1-ENABLE. State power channel input Efuse
	uint16_t PWR_CH1_State_eF_out :1; //0-DISABLE, 1-ENABLE. State power channel output Efuse
	uint16_t PWR_CH2_State_eF_in  :1;   
	uint16_t PWR_CH2_State_eF_out :1; 
	uint16_t PWR_CH3_State_eF_in  :1;   
	uint16_t PWR_CH3_State_eF_out :1; 	
	uint16_t PWR_CH4_State_eF_in  :1;   
	uint16_t PWR_CH4_State_eF_out :1; 
	uint16_t PWR_CH5_State_eF_in  :1;   
	uint16_t PWR_CH5_State_eF_out :1; 
	uint16_t PWR_CH6_State_eF_in  :1;   
	uint16_t PWR_CH6_State_eF_out :1; 
	uint16_t :4;

	//Shows Error if output pin value I2C GPIO Extender not same with PWR_CHx_State_eF_in and PWR_CHx_State_eF_out
	uint16_t Error_PWR_CH1_State_eF_in  :1; //0-OK, 1-ERROR.
	uint16_t Error_PWR_CH1_State_eF_out :1; 
	uint16_t Error_PWR_CH2_State_eF_in  :1;   
	uint16_t Error_PWR_CH2_State_eF_out :1; 
	uint16_t Error_PWR_CH3_State_eF_in  :1;   
	uint16_t Error_PWR_CH3_State_eF_out :1; 	
	uint16_t Error_PWR_CH4_State_eF_in  :1;   
	uint16_t Error_PWR_CH4_State_eF_out :1; 
	uint16_t Error_PWR_CH5_State_eF_in  :1;   
	uint16_t Error_PWR_CH5_State_eF_out :1; 
	uint16_t Error_PWR_CH6_State_eF_in  :1;   
	uint16_t Error_PWR_CH6_State_eF_out :1; 
	uint16_t :4;

	uint16_t PWR_CH1_PG_eF_in  :1; //0-OK, 1-ERROR. Power good channel status input eFuse  
	uint16_t PWR_CH1_PG_eF_out :1; //0-OK, 1-ERROR. Power good channel status output eFuse
	uint16_t PWR_CH2_PG_eF_in  :1;    
	uint16_t PWR_CH2_PG_eF_out :1; 
	uint16_t PWR_CH3_PG_eF_in  :1;    
	uint16_t PWR_CH3_PG_eF_out :1;
	uint16_t PWR_CH4_PG_eF_in  :1;    
	uint16_t PWR_CH4_PG_eF_out :1;
	uint16_t PWR_CH5_PG_eF_in  :1;    
	uint16_t PWR_CH5_PG_eF_out :1;
	uint16_t PWR_CH6_PG_eF_in  :1;    
	uint16_t PWR_CH6_PG_eF_out :1;
	uint16_t :4;

	int8_t Temp_sensor_1; //PDM temperature
	int8_t Temp_sensor_2;
	int8_t Temp_sensor_3;
	int8_t Temp_sensor_4;
	int8_t Temp_sensor_median; //PDM median temperature
	uint8_t PDM_addres_module;

	uint16_t Error_INA321_CH1    :1; // 0 - OK 1 - Error. Indicate error on the PDM board 
	uint16_t Error_INA321_CH2    :1;
	uint16_t Error_INA321_CH3    :1;
	uint16_t Error_INA321_CH4    :1;
	uint16_t Error_INA321_CH5 	 :1;
	uint16_t Error_INA321_CH6	 :1;
	uint16_t Error_I2C_GPIO_Ext1 :1;
	uint16_t Error_I2C_GPIO_Ext2 :1;
	uint16_t Error_I2C_MUX		 :1;
	uint16_t Error_temp_sensor_1 :1;
	uint16_t Error_temp_sensor_2 :1;
	uint16_t Error_temp_sensor_3 :1;
	uint16_t Error_temp_sensor_4 :1;
	uint16_t :3;
	
	
}_PDM;


#endif /* INC_PDM_PDM_STRUCT_H_ */
