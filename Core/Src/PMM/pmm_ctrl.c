
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
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
		}
	}
	
	return error_I2C;
}




/** @brief  Get value current, voltage and power of Power channel
	@param  *pdm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_ch - number power channel.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_PWR_CH_I_V_P( _PMM *pmm_ptr, uint8_t num_pwr_ch){

	uint8_t i = 0;
	int16_t val_current = 0;
	uint16_t val_bus_voltage = 0;
	uint16_t val_power = 0;
	_PMM_table pmm_table;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N;
	

	// Switch MUX to PDM I2C bus on PMM 
	SW_TMUX1209_I2C_main_PMM(); 

	pmm_table = PMM__Table(num_pwr_ch);

	//Read temperature
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = INA231_Get_I_V_P_int16( pmm_table.I2Cx_PWR_Mon, pmm_table.I2C_addr_PWR_Mon, pmm_table.PWR_Mon_Max_Current_int16, &val_current, &val_bus_voltage, &val_power );

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pmm_i2c_delay_att_conn );
			}
		}
	}


	if( error_I2C == ERROR_N ){//Error I2C INA231 
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pdm_ptr->PWR_Channel[num_pwr_ch].Voltage_val = 0;
		pdm_ptr->PWR_Channel[num_pwr_ch].Current_val = 0;
		pdm_ptr->PWR_Channel[num_pwr_ch].Power_val = 0;
		pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;

	}else{
		if(val_bus_voltage < 5 ){ //If power less than 5mV equate to zero.
			pdm_ptr->PWR_Channel[num_pwr_ch].Voltage_val = 0;
		}else{
			pdm_ptr->PWR_Channel[num_pwr_ch].Voltage_val = val_bus_voltage;
		}

		pdm_ptr->PWR_Channel[num_pwr_ch].Current_val = val_current;

		if(val_power < 5 ){ //If power less than 5mW equate to zero.
			pdm_ptr->PWR_Channel[num_pwr_ch].Power_val = 0;
		}else{
			pdm_ptr->PWR_Channel[num_pwr_ch].Power_val = val_power;
		}

		pdm_ptr->PWR_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
	}

	return error_I2C;
}


//********************change FN PMM_Set_MUX_CAN_CPUm_CPUb after 08.06 *********************//
/** @brief  Setup multiplexor. CAN bus switching between CPUm and CPUb.
	@param  num_CAN_pwr_channel - number of channel :
								CPUmain
								CPUbackup
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( uint8_t active_CPU ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;


	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( active_CPU == CPUmain ){
			error_I2C = TCA9539_conf_IO_dir_input( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );

		}else if(  active_CPU == CPUbackup ){
			error_I2C = TCA9539_Set_output_pin( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );
			error_I2C = TCA9539_conf_IO_dir_output( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, TCA9539_IO_P14|TCA9539_IO_P16 );

		}else{
			error_I2C = ERROR_N;
		}


		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	return error_I2C;
}



