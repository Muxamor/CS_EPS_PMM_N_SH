
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "INA231.h"
#include "ADS1015.h"
#include "pmm_struct.h"
#include "pmm_config.h"

#include "pmm_ctrl.h"

/**********************TO DO ***********************************/
/*1. Change PMM_Set_MUX_CAN_CPUm_CPUb after 08.06
 *
 *
 *
 */


/** @brief  Set state (enable/disable) PMM power channel.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_channel - number of channel PMM :
								PMM_PWR_Ch_CANmain
								PMM_PWR_Ch_CANbackup
								PMM_PWR_Ch_VBAT1_eF1 
								PMM_PWR_Ch_VBAT1_eF2 
								PMM_PWR_Ch_VBAT2_eF1 
								PMM_PWR_Ch_VBAT2_eF2 
								PMM_PWR_Ch_PBMs_Logic
								PMM_PWR_Ch_Deploy_Logic
								PMM_PWR_Ch_Deploy_Power
								PMM_PWR_Ch_5V_Bus				
								PMM_PWR_Ch_3_3V_Bus		
								PMM_PWR_Ch_I2C_Bus	
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	
	_PMM_table pmm_table;

	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

 	pmm_table = PMM__Table( num_pwr_channel );

	if(num_pwr_channel == PMM_PWR_Ch_CANmain ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_CANmain = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_CANmain = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_CANbackup ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_CANbackup = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_CANbackup = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF1 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat1_eF1 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat1_eF1 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF2 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat1_eF2 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat1_eF2 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF1 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat2_eF1 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat2_eF1 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF2 ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Vbat2_eF2 = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Vbat2_eF2 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_PBMs_Logic ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_PBMs_Logic = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_PBMs_Logic = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Logic ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Deploy_Logic = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Deploy_Logic = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Power ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_Deploy_Power = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_Deploy_Power = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_5V_Bus ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_5V_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_5V_Bus = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_3_3V_Bus ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_3_3V_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_3_3V_Bus = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Ch_I2C_Bus ){
		if( state_channel == ENABLE ){
			pmm_ptr->PWR_Ch_State_I2C_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_Ch_State_I2C_Bus = DISABLE;
		}

	}else{
		return ERROR_N;
	}

	pmm_ptr->PMM_save_conf_flag = 1;//Need save configure in FRAM.

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( state_channel == ENABLE ){
			error_I2C = TCA9539_Set_output_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );

		}else if( state_channel == DISABLE ){
			error_I2C = TCA9539_Reset_output_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );
		}

		if( error_I2C == SUCCESS ){
			error_I2C = TCA9539_conf_IO_dir_output( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	LL_mDelay(20); //Delay for startup power supply
	error_I2C = PMM_Check_state_PWR_CH( pmm_ptr, num_pwr_channel );
	
	#ifdef DEBUGprintf
		if (error_I2C == ERROR){
			Error_Handler();
		}
	#endif

	return error_I2C;
}



/** @brief  Checking the state of the PMM power channel.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_channel - number of channel PMM :
								PMM_PWR_Ch_CANmain
								PMM_PWR_Ch_CANbackup
								PMM_PWR_Ch_VBAT1_eF1 
								PMM_PWR_Ch_VBAT1_eF2 
								PMM_PWR_Ch_VBAT2_eF1 
								PMM_PWR_Ch_VBAT2_eF2
								PMM_PWR_Ch_PBMs_Logic
								PMM_PWR_Ch_Deploy_Logic
								PMM_PWR_Ch_Deploy_Power
								PMM_PWR_Ch_5V_Bus				
								PMM_PWR_Ch_3_3V_Bus	
								PMM_PWR_Ch_I2C_Bus	
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	uint8_t read_val_pin_EN;
	_PMM_table pmm_table;

	pmm_table = PMM__Table(  num_pwr_channel );

	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9539_read_input_pin( pmm_table.I2Cx_GPIO_Ext, pmm_table.I2C_addr_GPIO_Ext, pmm_table.pin_GPIO_Ext, &read_val_pin_EN);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}
	

	if( error_I2C == SUCCESS ){

		pmm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;

		if(num_pwr_channel == PMM_PWR_Ch_CANmain ){

			if( pmm_ptr->PWR_Ch_State_CANmain == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_CANmain = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_CANmain = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_CANbackup ){

			if( pmm_ptr->PWR_Ch_State_CANbackup == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_CANbackup = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_CANbackup = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF1 ){

			if( pmm_ptr->PWR_Ch_State_Vbat1_eF1 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF2 ){

			if( pmm_ptr->PWR_Ch_State_Vbat1_eF2 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF1 ){

			if( pmm_ptr->PWR_Ch_State_Vbat2_eF1 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF2 ){

			if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_PBMs_Logic ){

			if( pmm_ptr->PWR_Ch_State_PBMs_Logic == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_PBMs_Logic = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_PBMs_Logic = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Logic ){

			if( pmm_ptr->PWR_Ch_State_Deploy_Logic == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Deploy_Logic = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Deploy_Logic = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Power ){

			if( pmm_ptr->PWR_Ch_State_Deploy_Power == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_Deploy_Power = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_Deploy_Power = ERROR; ///0-ERROR
			}
		
		}else if( num_pwr_channel == PMM_PWR_Ch_5V_Bus ){

			if( pmm_ptr->PWR_Ch_State_5V_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_5V_Bus = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_5V_Bus = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_3_3V_Bus ){

			if( pmm_ptr->PWR_Ch_State_3_3V_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_3_3V_Bus = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_3_3V_Bus = ERROR; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Ch_I2C_Bus ){

			if( pmm_ptr->PWR_Ch_State_I2C_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_Ch_State_I2C_Bus = SUCCESS; ///0-OK
			}else{
				pmm_ptr->Error_PWR_Ch_State_I2C_Bus = ERROR; ///0-ERROR
			}
		}

	}else{
		pmm_ptr->Error_I2C_GPIO_Ext1 = ERROR;

		if(num_pwr_channel == PMM_PWR_Ch_CANmain ){
			pmm_ptr->Error_PWR_Ch_State_CANmain = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_CANbackup ){
			pmm_ptr->Error_PWR_Ch_State_CANbackup = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF1 ){
			pmm_ptr->Error_PWR_Ch_State_Vbat1_eF1 = ERROR; ///0-ERROR
		
		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT1_eF2 ){
			pmm_ptr->Error_PWR_Ch_State_Vbat1_eF2 = ERROR; ///0-ERROR
	
		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF1 ){
			pmm_ptr->Error_PWR_Ch_State_Vbat2_eF1 = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_VBAT2_eF2 ){
			pmm_ptr->Error_PWR_Ch_State_Vbat2_eF2 = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_PBMs_Logic ){
			pmm_ptr->Error_PWR_Ch_State_PBMs_Logic = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Logic ){
			pmm_ptr->Error_PWR_Ch_State_Deploy_Logic = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_Deploy_Power ){
			pmm_ptr->Error_PWR_Ch_State_Deploy_Power = ERROR; ///0-ERROR
		
		}else if( num_pwr_channel == PMM_PWR_Ch_5V_Bus ){
			pmm_ptr->Error_PWR_Ch_State_5V_Bus = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_3_3V_Bus ){
			pmm_ptr->Error_PWR_Ch_State_3_3V_Bus = ERROR; ///0-ERROR

		}else if( num_pwr_channel == PMM_PWR_Ch_I2C_Bus ){
			pmm_ptr->Error_PWR_Ch_State_I2C_Bus = ERROR; ///0-ERROR
		}
	}
	
	return error_I2C;
}


/** @brief  Get Power Good all power channel status on PMM.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PDM_Get_PG_all_PWR_CH( _PMM *pmm_ptr ){

	//int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	//Power supply Main and Backup working all the time and have no on/off pin.
	pmm_ptr->PWR_Supply_Main_PG = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_2));
	pmm_ptr->PWR_Supply_Backup_PG = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_3));

	//Read PG PMM power chanels
	if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){
		pmm_ptr->PWR_Ch_PG_PBMs_Logic = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_4));
	}else{
		pmm_ptr->PWR_Ch_PG_PBMs_Logic = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_Deploy_Power == ENABLE ){
		pmm_ptr->PWR_Ch_PG_Deploy_LP = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_5));
	}else{
		pmm_ptr->PWR_Ch_PG_Deploy_LP = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == ENABLE ){
		pmm_ptr->PWR_Ch_PG_Vbat2_eF2 = LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_7);
	}else{
		pmm_ptr->PWR_Ch_PG_Vbat2_eF2 = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_Vbat2_eF1 == ENABLE ){
		pmm_ptr->PWR_Ch_PG_Vbat2_eF1 = LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_8);
	}else{
		pmm_ptr->PWR_Ch_PG_Vbat2_eF1 = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_Vbat1_eF2 == ENABLE ){
		pmm_ptr->PWR_Ch_PG_Vbat1_eF2 = LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_9);
	}else{
		pmm_ptr->PWR_Ch_PG_Vbat1_eF2 = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_Vbat1_eF1 == ENABLE ){
		pmm_ptr->PWR_Ch_PG_Vbat1_eF1 = LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_10);
	}else{
		pmm_ptr->PWR_Ch_PG_Vbat1_eF1 = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_CANmain == ENABLE ){
		pmm_ptr->PWR_Ch_PG_CANmain = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_11));
	}else{
		pmm_ptr->PWR_Ch_PG_CANmain = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_CANbackup == ENABLE ){
		pmm_ptr->PWR_Ch_PG_CANbackup = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_12));
	}else{
		pmm_ptr->PWR_Ch_PG_CANbackup = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_I2C_Bus == ENABLE ){
		pmm_ptr->PWR_Ch_PG_I2C_Bus = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_13));
	}else{
		pmm_ptr->PWR_Ch_PG_I2C_Bus = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_3_3V_Bus == ENABLE ){
		pmm_ptr->PWR_Ch_PG_3_3V_Bus = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_14));
	}else{
		pmm_ptr->PWR_Ch_PG_3_3V_Bus = SUCCESS;  // OK because power channel is DISABLE
	}

	if( pmm_ptr->PWR_Ch_State_5V_Bus == ENABLE ){
		pmm_ptr->PWR_Ch_PG_5V_Bus = !(LL_GPIO_IsInputPinSet(GPIOE, LL_GPIO_PIN_15));
	}else{
		pmm_ptr->PWR_Ch_PG_5V_Bus = SUCCESS;  // OK because power channel is DISABLE
	}

	return SUCCESS;
}


/** @brief  Get temperature from TMP1075 sensor.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_Temperature( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr ){

	int8_t temp_value;
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

		error_I2C = TMP1075_read_int8_temperature( I2Cx,tmp1075_addr, &temp_value);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}


	if( error_I2C == ERROR ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pmm_ptr->Temp_sensor = 0x7F;
		pmm_ptr->Error_TMP1075_sensor = ERROR;
	}else{
		pmm_ptr->Error_TMP1075_sensor = SUCCESS;
		pmm_ptr->Temp_sensor = temp_value;
	}

	return error_I2C;
}



/** @brief  Get value current, voltage and power of VBAT Power channel
	@param  *pdm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_ch - number power channel.
							PMM_PWR_Ch_VBAT1_eF1
							PMM_PWR_Ch_VBAT1_eF2
							PMM_PWR_Ch_VBAT2_eF1
							PMM_PWR_Ch_VBAT2_eF2
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_PWR_CH_VBAT_I_V_P( _PMM *pmm_ptr, uint8_t num_pwr_ch){

	uint8_t i = 0;
	int8_t error_I2C = ERROR_N;
	int16_t val_current = 0;
	uint16_t val_bus_voltage = 0;
	uint16_t val_power = 0;
	_PMM_table pmm_table;
	
	
	// Switch MUX to PDM I2C bus on PMM 
	SW_TMUX1209_I2C_main_PMM(); 

	pmm_table = PMM__Table(num_pwr_ch);

	//Read INA231
	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

		error_I2C = INA231_Get_I_V_P_int16( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_Max_Current_int16, &val_current, &val_bus_voltage, &val_power );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if(val_bus_voltage < 5 ){ //If power less than 5mV equate to zero.
		val_bus_voltage = 0;
	}

	if(val_power < 5 ){ //If power less than 5mW equate to zero.
		val_power = 0;
	}


	if( error_I2C == ERROR_N ){//Error I2C INA231 
		val_current = 0;
		val_bus_voltage = 0;
		val_power = 0;
	}

	if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF1 ){
		pmm_ptr->PWR_Ch_Vbat1_eF1_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat1_eF1_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat1_eF1_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat1_eF1 = ERROR; 
		}
		
	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT1_eF2 ){
		pmm_ptr->PWR_Ch_Vbat1_eF2_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat1_eF2_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat1_eF2_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat1_eF2 = ERROR; 
		}
	
	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF1 ){
		pmm_ptr->PWR_Ch_Vbat2_eF1_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat2_eF1_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat2_eF1_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat2_eF1 = ERROR; 
		}

	}else if( num_pwr_ch == PMM_PWR_Ch_VBAT2_eF2 ){
		pmm_ptr->PWR_Ch_Vbat2_eF2_Voltage_val = val_bus_voltage;
		pmm_ptr->PWR_Ch_Vbat2_eF2_Current_val = val_current; 
		pmm_ptr->PWR_Ch_Vbat2_eF2_Power_val = val_power; 

		if( error_I2C == SUCCESS ){
			pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = SUCCESS; 

		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			pmm_ptr->Error_PWR_Mon_Vbat2_eF2 = ERROR; 
		}

	}

	return error_I2C;
}



/** @brief  Average 16 measuring for ADS1015.
	@param  *I2Cx - Number I2C bus.
	@param  I2C_ADS1015_addr - I2C addres
	@param  num_ch_mux - number channel mux ADC.
	@param  average_num - average number.
	@param  *average_data - pointer to get average data. 
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_ADS1015_average_meas(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t num_ch_mux, uint8_t average_num, float *average_data){

	uint8_t i = 0;
	uint8_t j = 0;
	int8_t error_I2C = ERROR_N;
	uint8_t conv_status = 0;
	uint8_t timeout = 0;
	float data = 0.0;
	float total_data = 0.0;

	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

		error_I2C = ADS1015_setup_mux(I2Cx, I2C_ADS1015_addr, num_ch_mux );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if(error_I2C != SUCCESS ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	for(j = 0; j < average_num; j++ ){

		//Read INA231
		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = ADS1015_start_single_conv(I2Cx, I2C_ADS1015_addr);

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pmm_i2c_delay_att_conn );
			}
		}


		if( error_I2C == SUCCESS ){

			timeout = 0;
			error_I2C = ERROR_N;

			while(!conv_status){
				error_I2C = ADS1015_read_conv_status(I2Cx, I2C_ADS1015_addr, &conv_status);

				timeout++;

				if(timeout == 255){
					#ifdef DEBUGprintf
						Error_Handler();
					#endif
					return ERROR_N;
				}
			}
			
			ADS1015_read_Volts_float(I2Cx, I2C_ADS1015_addr, &data);
			total_data = total_data + data;
		}else{
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			return ERROR_N;
		}	
	}

	*average_data = total_data / average_num;

	return SUCCESS;
}


/** @brief  Get current input and output eFuse of power supply main and backup.
	@param  *pdm_ptr - pointer to struct which contain all information about PMM.
	@param  *I2Cx - number I2C channel.
	@param  I2C_ADS1015_addr - I2C address ADS1015.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_PWR_Supply_m_b_I( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr){

	//uint8_t i = 0;
	int8_t error_I2C = ERROR_N;
	float ch0_meas = 0.0;
	float ch1_meas = 0.0;
	float ch2_meas = 0.0;
	float ch3_meas = 0.0;
	int16_t Backup_eF_in_Current_val = 0;
	int16_t Backup_eF_out_Current_val = 0;
	int16_t Main_eF_in_Current_val = 0;
	int16_t Main_eF_out_Current_val = 0;

	SW_TMUX1209_I2C_main_PMM(); // Switch MUX to pmm I2C bus on PMM

	if( PMM_ADS1015_average_meas(I2Cx, I2C_ADS1015_addr, ADS1015_AINp0_AINnGND, 16, &ch0_meas ) == SUCCESS ){
		if( PMM_ADS1015_average_meas(I2Cx, I2C_ADS1015_addr, ADS1015_AINp1_AINnGND, 16, &ch1_meas ) == SUCCESS ){
			if( PMM_ADS1015_average_meas(I2Cx, I2C_ADS1015_addr, ADS1015_AINp2_AINnGND, 16, &ch2_meas ) == SUCCESS ){
				 error_I2C = PMM_ADS1015_average_meas(I2Cx, I2C_ADS1015_addr, ADS1015_AINp3_AINnGND, 16, &ch3_meas );
			}
		}
	}

	if( error_I2C == SUCCESS) {

		pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon = SUCCESS;

		Main_eF_in_Current_val = (int16_t)(( ch1_meas * 495.0 ) - 13.0 ); //Current in mA. (-13) - correction coefficient
		Main_eF_out_Current_val = (int16_t)(( ch3_meas * 2778.0 ) - 13.0 ); //Current in mA. (-13) - correction coefficient


		Backup_eF_in_Current_val = (int16_t)(( ch0_meas * 495.0 ) - 13.0 ); //Current in mA. (-13) - correction coefficient
		Backup_eF_out_Current_val = (int16_t)(( ch2_meas * 2778.0 ) - 13.0 ); //Current in mA. (-13) - correction coefficient

		if( (Main_eF_out_Current_val - Main_eF_in_Current_val) < 5 ){ // Small currents are poorly measured. This is incorrect measurement protection
			pmm_ptr->PWR_Supply_Main_eF_in_Current_val = 0;
			pmm_ptr->PWR_Supply_Main_eF_out_Current_val = 0;
		}else{
			pmm_ptr->PWR_Supply_Main_eF_in_Current_val = Main_eF_in_Current_val;
			pmm_ptr->PWR_Supply_Main_eF_out_Current_val = Main_eF_out_Current_val;
		}

		if( (Backup_eF_out_Current_val - Backup_eF_in_Current_val) < 5 ){// Small currents are poorly measured. This is incorrect measurement protection
			pmm_ptr->PWR_Supply_Backup_eF_in_Current_val = 0;
			pmm_ptr->PWR_Supply_Backup_eF_out_Current_val = 0;
		}else{
			pmm_ptr->PWR_Supply_Backup_eF_in_Current_val = Backup_eF_in_Current_val;
			pmm_ptr->PWR_Supply_Backup_eF_out_Current_val = Backup_eF_out_Current_val;
		}

	}else{
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pmm_ptr->Error_PWR_Supply_m_b_Curr_Mon = ERROR;
		pmm_ptr->PWR_Supply_Main_eF_in_Current_val = 0;
		pmm_ptr->PWR_Supply_Main_eF_out_Current_val = 0;
		pmm_ptr->PWR_Supply_Backup_eF_in_Current_val = 0;
		pmm_ptr->PWR_Supply_Backup_eF_out_Current_val = 0;
	}

	return error_I2C;
}


/** @brief  Setup multiplexor. CAN bus switching between CPUm and CPUb.
	@param  num_CAN_pwr_channel - number of channel :
								CPUmain
								CPUbackup
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( _PMM *pmm_ptr ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;


	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( pmm_ptr->Active_CPU == 0){ //Active CPU

			if( pmm_ptr->Main_Backup_mode_CPU == 0 ){ //Setting mane CPU
				error_I2C = TCA9539_Reset_output_pin( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );
			}else{//Setting backup CPU
				error_I2C = TCA9539_Set_output_pin( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );
			}

			error_I2C = TCA9539_conf_IO_dir_output( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );

		}else{ //Passive CPU
			error_I2C = TCA9539_conf_IO_dir_input( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	return error_I2C;
}



