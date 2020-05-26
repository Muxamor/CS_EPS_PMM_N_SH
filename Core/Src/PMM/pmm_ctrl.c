
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9539.h"
#include "pmm_struct.h"
#include "pmm_config.h"

#include "pmm_ctrl.h"

/** @brief  Set state (enable/disable) CAN power channel.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_CAN_pwr_channel - number of channel :
								CANmain
								CANbackup
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_state_PWR_CAN( _PMM *pmm_ptr, uint8_t num_CAN_pwr_channel, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	I2C_TypeDef *I2Cx;
	uint8_t tca9539_I2C_addr;
	uint16_t tca9539_pin_num;


	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		return ERROR_N;
	}

	SW_TMUX1209_I2C_main_PMM();// Switch MUX to PDM I2C bus on PMM

	I2Cx = PMM_I2Cx_GPIOExt1;
	tca9539_I2C_addr = PMM_I2CADDR_GPIOExt1;

	if(num_CAN_pwr_channel == CANmain ){
		tca9539_pin_num = TCA9539_IO_P17;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_CANmain = ENABLE;
		}else{
			pmm_ptr->PWR_State_CANmain = DISABLE;
		}

	}else if( num_CAN_pwr_channel == CANbackup ){
		tca9539_pin_num = TCA9539_IO_P15;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_CANbackup = ENABLE;
		}else{
			pmm_ptr->PWR_State_CANbackup = DISABLE;
		}

	}else{
		return ERROR_N;
	}


	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		if( state_channel == ENABLE ){
			error_I2C = TCA9539_Set_output_pin( I2Cx, tca9539_I2C_addr, tca9539_pin_num );

		}else if( state_channel == DISABLE ){
			error_I2C = TCA9539_Reset_output_pin( I2Cx, tca9539_I2C_addr, tca9539_pin_num );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}

	if( error_I2C == SUCCESS ){
		i=0;
	 	error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

			error_I2C = TCA9539_conf_IO_dir_output( I2Cx, tca9539_I2C_addr, tca9539_pin_num );

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pmm_i2c_delay_att_conn );
			}
		}

	}


	if( error_I2C == SUCCESS ){
		error_I2C = PMM_Check_state_PWR_CAN( pmm_ptr, num_CAN_pwr_channel );

	}

	return error_I2C;
}


/** @brief  Checking the state of the CAN power.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_CAN_pwr_channel - number of channel :
								CANmain
								CANbackup
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Check_state_PWR_CAN( _PMM *pmm_ptr, uint8_t num_CAN_pwr_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	uint16_t tca9539_pin_num;
	uint8_t read_val_pin_EN;


	if(num_CAN_pwr_channel == CANmain ){
		tca9539_pin_num = TCA9539_IO_P17;

	}else if( num_CAN_pwr_channel == CANbackup ){
		tca9539_pin_num = TCA9539_IO_P15;

	}else{
		return ERROR_N;
	}


	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9539_read_input_pin( PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1, tca9539_pin_num, &read_val_pin_EN);

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pmm_i2c_delay_att_conn );
		}
	}
	

	if( error_I2C == SUCCESS ){

		pmm_ptr->Error_I2C_GPIO_Ext1 = SUCCESS;

		if(num_CAN_pwr_channel == CANmain ){

			if( pmm_ptr->PWR_State_CANmain == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_CANmain = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_CANmain = 1; ///0-ERROR
			}

		}else if( num_CAN_pwr_channel == CANbackup ){

			if( pmm_ptr->PWR_State_CANbackup == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_CANbackup = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_CANbackup = 1; ///0-ERROR
			}
		}

	}else{
		pmm_ptr->Error_I2C_GPIO_Ext1 = ERROR;
	}
	
	return error_I2C;
}
