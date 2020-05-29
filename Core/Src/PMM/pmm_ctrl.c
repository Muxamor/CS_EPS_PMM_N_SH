
#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
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
								PMM_PWR_CANmain
								PMM_PWR_CANbackup
								PMM_PWR_VBAT_1_eF_1 
								PMM_PWR_VBAT_1_eF_2 
								PMM_PWR_VBAT_2_eF_1 
								PMM_PWR_VBAT_2_eF_2 
								PMM_PWR_PBMs_Logic
								PMM_PWR_5V_Bus				
								PMM_PWR_3_3V_Bus			
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	I2C_TypeDef *I2Cx;
	uint8_t tca9539_I2C_addr;
	uint16_t tca9539_pin_num;


	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		return ERROR_N;
	}


	I2Cx = PMM_I2Cx_GPIOExt1;
	tca9539_I2C_addr = PMM_I2CADDR_GPIOExt1;

	if(num_pwr_channel == PMM_PWR_CANmain ){
		tca9539_pin_num = TCA9539_IO_P17;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_CANmain = ENABLE;
		}else{
			pmm_ptr->PWR_State_CANmain = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_CANbackup ){
		tca9539_pin_num = TCA9539_IO_P15;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_CANbackup = ENABLE;
		}else{
			pmm_ptr->PWR_State_CANbackup = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_VBAT_1_eF_1 ){
		tca9539_pin_num = TCA9539_IO_P00;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_Vbat1_eF1 = ENABLE;
		}else{
			pmm_ptr->PWR_State_Vbat1_eF1 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_VBAT_1_eF_2 ){
		tca9539_pin_num = TCA9539_IO_P01;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_Vbat1_eF2 = ENABLE;
		}else{
			pmm_ptr->PWR_State_Vbat1_eF2 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_VBAT_2_eF_1 ){
		tca9539_pin_num = TCA9539_IO_P02;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_Vbat2_eF1 = ENABLE;
		}else{
			pmm_ptr->PWR_State_Vbat2_eF1 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_VBAT_2_eF_2 ){
		tca9539_pin_num = TCA9539_IO_P03;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_Vbat2_eF2 = ENABLE;
		}else{
			pmm_ptr->PWR_State_Vbat2_eF2 = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_PBMs_Logic ){
		tca9539_pin_num = TCA9539_IO_P04;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_PBMs_Logic = ENABLE;
		}else{
			pmm_ptr->PWR_State_PBMs_Logic = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Deploy_Logic ){
		tca9539_pin_num = TCA9539_IO_P10;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_Deploy_Logic = ENABLE;
		}else{
			pmm_ptr->PWR_State_Deploy_Logic = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_Deploy_Power ){
		tca9539_pin_num = TCA9539_IO_P11;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_Deploy_Power = ENABLE;
		}else{
			pmm_ptr->PWR_State_Deploy_Power = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_5V_Bus ){
		tca9539_pin_num = TCA9539_IO_P05;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_5V_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_State_5V_Bus = DISABLE;
		}

	}else if( num_pwr_channel == PMM_PWR_3_3V_Bus ){
		tca9539_pin_num = TCA9539_IO_P06;

		if( state_channel == ENABLE ){
			pmm_ptr->PWR_State_3_3V_Bus = ENABLE;
		}else{
			pmm_ptr->PWR_State_3_3V_Bus = DISABLE;
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
		error_I2C = PMM_Check_state_PWR_CH( pmm_ptr, num_pwr_channel );

	}

	return error_I2C;
}



/** @brief  Checking the state of the PMM power channel.
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@param  num_pwr_channel - number of channel PMM :
								PMM_PWR_CANmain
								PMM_PWR_CANbackup
								PMM_PWR_VBAT_1_eF_1 
								PMM_PWR_VBAT_1_eF_2 
								PMM_PWR_VBAT_2_eF_1 
								PMM_PWR_VBAT_2_eF_2
								PMM_PWR_PBMs_Logic
								PMM_PWR_Deploy_Logic
								PMM_PWR_Deploy_Power
								PMM_PWR_5V_Bus				
								PMM_PWR_3_3V_Bus	
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i = 0;
	uint16_t tca9539_pin_num;
	uint8_t read_val_pin_EN;


	if(num_pwr_channel == PMM_PWR_CANmain ){
		tca9539_pin_num = TCA9539_IO_P17;

	}else if( num_pwr_channel == PMM_PWR_CANbackup ){
		tca9539_pin_num = TCA9539_IO_P15;

	}else if( num_pwr_channel == PMM_PWR_VBAT_1_eF_1 ){
		tca9539_pin_num = TCA9539_IO_P00;

	}else if( num_pwr_channel == PMM_PWR_VBAT_1_eF_2 ){
		tca9539_pin_num = TCA9539_IO_P01;

	}else if( num_pwr_channel == PMM_PWR_VBAT_2_eF_1 ){
		tca9539_pin_num = TCA9539_IO_P02;

	}else if( num_pwr_channel == PMM_PWR_VBAT_2_eF_2 ){
		tca9539_pin_num = TCA9539_IO_P03;

	}else if( num_pwr_channel == PMM_PWR_PBMs_Logic ){
		tca9539_pin_num = TCA9539_IO_P04;

	}else if( num_pwr_channel == PMM_PWR_Deploy_Logic ){
		tca9539_pin_num = TCA9539_IO_P10;

	}else if( num_pwr_channel == PMM_PWR_Deploy_Power ){
		tca9539_pin_num = TCA9539_IO_P11;

	}else if( num_pwr_channel == PMM_PWR_5V_Bus ){
		tca9539_pin_num = TCA9539_IO_P05;

	}else if( num_pwr_channel == PMM_PWR_3_3V_Bus ){
		tca9539_pin_num = TCA9539_IO_P06;

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

		if(num_pwr_channel == PMM_PWR_CANmain ){

			if( pmm_ptr->PWR_State_CANmain == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_CANmain = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_CANmain = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_CANbackup ){

			if( pmm_ptr->PWR_State_CANbackup == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_CANbackup = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_CANbackup = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_VBAT_1_eF_1 ){

			if( pmm_ptr->PWR_State_Vbat1_eF1 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_Vbat1_eF1 = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_Vbat1_eF1 = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_VBAT_1_eF_2 ){

			if( pmm_ptr->PWR_State_Vbat1_eF2 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_Vbat1_eF2 = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_Vbat1_eF2 = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_VBAT_2_eF_1 ){

			if( pmm_ptr->PWR_State_Vbat2_eF1 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_Vbat2_eF1 = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_Vbat2_eF1 = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_VBAT_2_eF_2 ){

			if( pmm_ptr->PWR_State_Vbat2_eF2 == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_Vbat2_eF2 = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_Vbat2_eF2 = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_PBMs_Logic ){

			if( pmm_ptr->PWR_State_PBMs_Logic == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_PBMs_Logic = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_PBMs_Logic = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Deploy_Logic ){

			if( pmm_ptr->PWR_State_Deploy_Logic == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_Deploy_Logic = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_Deploy_Logic = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_Deploy_Power ){

			if( pmm_ptr->PWR_State_Deploy_Power == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_Deploy_Power = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_Deploy_Power = 1; ///0-ERROR
			}
		
		}else if( num_pwr_channel == PMM_PWR_5V_Bus ){

			if( pmm_ptr->PWR_State_5V_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_5V_Bus = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_5V_Bus = 1; ///0-ERROR
			}

		}else if( num_pwr_channel == PMM_PWR_3_3V_Bus ){

			if( pmm_ptr->PWR_State_3_3V_Bus == read_val_pin_EN ){
				pmm_ptr->Error_PWR_State_3_3V_Bus = 0; ///0-OK
			}else{
				pmm_ptr->Error_PWR_State_3_3V_Bus = 1; ///0-ERROR
			}

		}

	}else{
		pmm_ptr->Error_I2C_GPIO_Ext1 = ERROR;
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



