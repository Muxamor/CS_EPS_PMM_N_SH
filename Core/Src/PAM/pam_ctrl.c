#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TCA6424A.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "INA231.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_ctrl.h"


/** @brief  Set state (enable/disable) power supply PAM.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  pwr_source_num - source (PWR_DC_DC, PWR_LDO).
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Set_state_PWR_Supply( _PAM *pam_ptr, uint8_t pwr_source_num, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i=0;
	_PAM_table pam_table;

	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	pam_table = PAM__Table(pwr_source_num, PAM_TYPE_PWR_CH_Supply);

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

	if( pwr_source_num == PAM_PWR_DC_DC ){
        if( pam_ptr->State_DC_DC != state_channel){
            pam_ptr->PAM_save_conf_flag = 1; //Need save configure in FRAM.
        }
		pam_ptr->State_DC_DC = state_channel;

	}else if( pwr_source_num == PAM_PWR_LDO ){
	    if( pam_ptr->State_LDO != state_channel ){
            pam_ptr->PAM_save_conf_flag = 1; //Need save configure in FRAM.
        }
		pam_ptr->State_LDO = state_channel;
	}

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

 	//Enable/Disable Efuse power channel.
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

		if( state_channel == ENABLE ){
			if ( TCA6424A_Set_output_pin( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF) == SUCCESS ){
				error_I2C = TCA6424A_conf_IO_dir_output( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF);
			}

		}else{ //Disable power channel
			if ( TCA6424A_Reset_output_pin( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF) == SUCCESS ){
				error_I2C = TCA6424A_conf_IO_dir_output( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF);
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay(pam_i2c_delay_att_conn);
		}
	}

    if( (state_channel == ENABLE) && (pam_ptr->PAM_save_conf_flag == 1) && (error_I2C == SUCCESS) ) {
        LL_mDelay(40); //Delay for startup power supply
    }

	if( error_I2C != SUCCESS ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_GPIO_Ext = ERROR;

        if( pwr_source_num == PAM_PWR_DC_DC ){
            pam_ptr->Error_State_DC_DC = ERROR;

        }else if( pwr_source_num == PAM_PWR_LDO ){
            pam_ptr->Error_State_LDO = ERROR;
        }
	}

	return error_I2C;
}


/** @brief  Checking the state of the power supply of PAM. OK- If there is no difference between
			the set value and the actual value set. ERROR - If there are differences between
			the set value and the actual set.
			actual value set - Get by reading the real value I2C GPIO Expander.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  pwr_source_num - number channel on/off
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Check_state_PWR_Supply( _PAM *pam_ptr, uint8_t pwr_source_num ) {

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N;
    uint8_t pin_state = 0;
    _PAM_table pam_table;

    SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

    pam_table = PAM__Table(pwr_source_num, PAM_TYPE_PWR_CH_Supply);

    while((error_I2C != SUCCESS) && (i < pam_i2c_attempt_conn)){

        error_I2C = TCA6424A_read_input_pin(pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF, &pin_state);

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay(pam_i2c_delay_att_conn);
        }
    }

    if( error_I2C == SUCCESS ){

        pam_ptr->Error_I2C_GPIO_Ext = SUCCESS;

        if( pwr_source_num == PAM_PWR_DC_DC ){
            if( pam_ptr->State_DC_DC != pin_state ){
                pam_ptr->Error_State_DC_DC = ERROR;
            }else{
                pam_ptr->Error_State_DC_DC = SUCCESS;
            }

        }else if( pwr_source_num == PAM_PWR_LDO ){
            if( pam_ptr->State_LDO != pin_state ){
                pam_ptr->Error_State_LDO = ERROR;
            }else{
                pam_ptr->Error_State_LDO = SUCCESS;
            }
        }

    }else{
        #ifdef DEBUGprintf
            Error_Handler();
        #endif

        pam_ptr->Error_I2C_GPIO_Ext = ERROR;

        if( pwr_source_num == PAM_PWR_DC_DC ){
            pam_ptr->Error_State_DC_DC = ERROR;

        }else if( pwr_source_num == PAM_PWR_LDO ){
            pam_ptr->Error_State_LDO = ERROR;
        }
    }

    return error_I2C;
}


/** @brief  Get Power Good power PAM
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  pwr_source_num - source (PWR_DC_DC, PWR_LDO).
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_PG_PWR_Supply( _PAM *pam_ptr, uint8_t pwr_source_num ){

	uint8_t i = 0;
	uint8_t read_val_pin_PG_eF = 0;
	int8_t error_I2C = ERROR_N;
	_PAM_table pam_table;

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

	//Get real state value pins TCA9539.
	pam_table = PAM__Table(pwr_source_num, PAM_TYPE_PWR_CH_Supply);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read real value input pin PG.

		if( TCA6424A_conf_IO_dir_input( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID ) == SUCCESS) {

			error_I2C = TCA6424A_read_input_pin(pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID, &read_val_pin_PG_eF );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	if( error_I2C == SUCCESS  ){

        pam_ptr->Error_I2C_GPIO_Ext = SUCCESS;

        if( pwr_source_num == PAM_PWR_DC_DC ){
            if( pam_ptr->State_DC_DC == ENABLE ){
                pam_ptr->PG_DC_DC = !read_val_pin_PG_eF;
            }else{
                pam_ptr->PG_DC_DC = SUCCESS;
            }

        }else if( pwr_source_num == PAM_PWR_LDO ){
            if( pam_ptr->State_LDO == ENABLE ){
                pam_ptr->PG_LDO = !read_val_pin_PG_eF;
            }else{
                pam_ptr->PG_LDO = SUCCESS;
            }
        }

	}else{
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_GPIO_Ext = ERROR;
	}

	return error_I2C;
}


/** @brief  Get State Ideal Diode power input PAM
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  pwr_source_num - source (PAM_PWR_IN_Ch1 - PAM_PWR_IN_Ch6).
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_State_ID_PWR_Ch_In( _PAM *pam_ptr, uint8_t pwr_source_num ){

    uint8_t i = 0;
    uint8_t read_val_pin_State_ID = 0;
    int8_t error_I2C = ERROR_N;
    _PAM_table pam_table;

    SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

    //Get real state value pins TCA6424A.
    pam_table = PAM__Table(pwr_source_num, PAM_TYPE_PWR_CH_IN);

    i=0;
    error_I2C = ERROR_N;

    while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read real value input pin PG.

        if( TCA6424A_conf_IO_dir_input( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID ) == SUCCESS) {
            error_I2C = TCA6424A_read_input_pin(pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID, &read_val_pin_State_ID );
        }

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pam_i2c_delay_att_conn );
        }
    }

    if( error_I2C == SUCCESS  ){

        if((pam_ptr->State_DC_DC == ENABLE) || (pam_ptr->State_LDO == ENABLE)){
       		pam_ptr->PWR_IN_Channel[pwr_source_num].State_ID_In = read_val_pin_State_ID;
       	}
        pam_ptr->Error_I2C_GPIO_Ext = SUCCESS;

    }else{
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        pam_ptr->Error_I2C_GPIO_Ext = ERROR;
    }

    return error_I2C;
}


/** @brief  Get temperature from TMP1075 sensor.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_Temperature( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

	int8_t temp_value = 0;
	uint8_t i = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N;

	// Switch MUX to PAM I2C bus on PMM
	SW_TMUX1209_I2C_main_PAM();

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	//Read temperature
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = TMP1075_read_int8_temperature( I2Cx, tmp1075_addr, &temp_value);

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pam_i2c_delay_att_conn );
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_MUX_2 = ERROR;
	}else{
		pam_ptr->Error_I2C_MUX_2 = SUCCESS;
	}

	switch( tmp1075_addr ){

		case PAM_I2CADDR_TMP1075_1:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[0] = 0x7F;
				pam_ptr->Error_temp_sensor_1 = ERROR;
			}else{
				pam_ptr->Temp_sensor[0] = temp_value;
				pam_ptr->Error_temp_sensor_1 = SUCCESS; //No error
			}
			break;

		case PAM_I2CADDR_TMP1075_2:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[1] = 0x7F;
				pam_ptr->Error_temp_sensor_2 = ERROR;
			}else{
				pam_ptr->Temp_sensor[1] = temp_value;
				pam_ptr->Error_temp_sensor_2 = SUCCESS; //No error
			}
			break;

		case PAM_I2CADDR_TMP1075_3:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[2] = 0x7F;
				pam_ptr->Error_temp_sensor_3 = ERROR;
			}else{
				pam_ptr->Temp_sensor[2] = temp_value;
				pam_ptr->Error_temp_sensor_3 = SUCCESS; //No error
			}
			break;

		case PAM_I2CADDR_TMP1075_4:
			if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pam_ptr->Temp_sensor[3] = 0x7F;
				pam_ptr->Error_temp_sensor_4 = ERROR;
			}else{
				pam_ptr->Temp_sensor[3] = temp_value;
				pam_ptr->Error_temp_sensor_4 = SUCCESS; //No error
			}
			break;

		default:
			break;
	}

	return error_I2C;
}

/** @brief  Get value current, voltage and power of input Power channel
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_pwr_ch - number power channel( PAM_PWR_IN_Ch1 - PAM_PWR_IN_Ch6) .
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_PWR_CH_IN_I_V_P( _PAM *pam_ptr, uint8_t num_pwr_ch){

	uint8_t i = 0;
	int8_t Error_I2C_MUX = ERROR_N;
	int8_t error_I2C = ERROR_N;
	int16_t val_current = 0;
	uint16_t val_bus_voltage = 0;
	uint16_t val_power = 0;
	_PAM_table pam_table;


	if( num_pwr_ch > PAM_PWR_IN_Ch_quantity ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	// Switch MUX to PAM I2C bus on PAM
	SW_TMUX1209_I2C_main_PAM();

	pam_table = PAM__Table(num_pwr_ch, PAM_TYPE_PWR_CH_IN);

	//Enable I2C MUX channel
	i=0;
	error_I2C = ERROR_N;
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

		error_I2C = TCA9548_Enable_I2C_ch( pam_table.I2Cx_PORT, pam_table.I2C_addr_I2C_MUX_PWR, pam_table.I2C_MUX_Ch );

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	Error_I2C_MUX = error_I2C;

	//Read INA231
	if( error_I2C == SUCCESS ){

		i=0;
		error_I2C = ERROR_N;

		while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read temperature.

			error_I2C = INA231_Get_I_V_P_int16( pam_table.I2Cx_PORT, pam_table.I2C_addr_PWR_Mon, pam_table.PWR_Mon_Max_Current_int16, &val_current, &val_bus_voltage, &val_power );

			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( pam_i2c_delay_att_conn );
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch( pam_table.I2Cx_PORT, pam_table.I2C_addr_I2C_MUX_PWR, pam_table.I2C_MUX_Ch );

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_MUX_1 = ERROR;
	}else{
		pam_ptr->Error_I2C_MUX_1 = SUCCESS;
	}

	if( error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C INA231 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		val_bus_voltage = 0;
		val_current = 0;
		val_power = 0;
		pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon = ERROR;

	}else{
		if(val_bus_voltage < 5 ){ //If power less than 5mV equate to zero.
			val_bus_voltage = 0;
		}

		if(val_power < 5 ){ //If power less than 5mW equate to zero.
			val_power = 0;
		}

		pam_ptr->PWR_IN_Channel[num_pwr_ch].Error_PWR_Mon = SUCCESS;
	}

	pam_ptr->PWR_IN_Channel[num_pwr_ch].Voltage_val = val_bus_voltage;
	pam_ptr->PWR_IN_Channel[num_pwr_ch].Current_val = val_current;
	pam_ptr->PWR_IN_Channel[num_pwr_ch].Power_val = val_power;

	return error_I2C;
}

/** @brief  Set state power channel (enable/disable) for get telemetry SP.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_pwr_ch - number channel SP (PAM_PWR_TM_SP_Ch1 - PAM_PWR_TM_SP_Ch6).
	@param  state_channel - 0- DISABLE power channel, 1 - ENABLE power channel.:
								ENABLE
								DISABLE
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Set_state_PWR_TM_SP_CH( _PAM *pam_ptr, uint8_t num_pwr_ch, uint8_t state_channel ){

	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	uint8_t i=0;
	_PAM_table pam_table;

	if( (state_channel != ENABLE) && (state_channel != DISABLE) ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	pam_table = PAM__Table(num_pwr_ch, PAM_TYPE_PWR_CH_TM_SP);

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

	if( pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out != state_channel){
		pam_ptr->PAM_save_conf_flag = 1; //Need save configure in FRAM.
	}

	pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out = state_channel;

	//Write to I2C GPIO Extender.
	i=0;
 	error_I2C = ERROR_N;

 	//Enable/Disable Efuse power SP channel.
	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

		if( state_channel == ENABLE ){
			if ( TCA6424A_Set_output_pin( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF) == SUCCESS ){
				error_I2C = TCA6424A_conf_IO_dir_output( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF);
			}

		}else{ //Disable power channel
			if ( TCA6424A_Reset_output_pin( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF) == SUCCESS ){
				error_I2C = TCA6424A_conf_IO_dir_output( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF);
			}
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay(pam_i2c_delay_att_conn);
		}
	}

    if( error_I2C != SUCCESS ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_GPIO_Ext = ERROR;
		pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].Error_State_eF_out = ERROR;
    }

	return error_I2C;
}

/** @brief  Checking the state state power channel for get telemetry SP . OK- If there is no difference between
			the set value and the actual value set. ERROR - If there are differences between
			the set value and the actual set.
			actual value set - Get by reading the real value I2C GPIO Expander.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_pwr_ch - number channel on/off (PAM_PWR_TM_SP_Ch1 - PAM_PWR_TM_SP_Ch6).
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Check_state_PWR_TM_SP_CH( _PAM *pam_ptr, uint8_t num_pwr_ch ) {

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N;
    uint8_t pin_state = 0;
    _PAM_table pam_table;

    SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

    pam_table = PAM__Table(num_pwr_ch, PAM_TYPE_PWR_CH_TM_SP);

    while((error_I2C != SUCCESS) && (i < pam_i2c_attempt_conn)){

        error_I2C = TCA6424A_read_input_pin(pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_Enable_eF, &pin_state);

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay(pam_i2c_delay_att_conn);
        }
    }

    if( error_I2C == SUCCESS ){

        pam_ptr->Error_I2C_GPIO_Ext = SUCCESS;

        if( pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out != pin_state ){
        	pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].Error_State_eF_out = ERROR;

        }else{
        	pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].Error_State_eF_out = SUCCESS;
        }

    }else{
        #ifdef DEBUGprintf
            Error_Handler();
        #endif

        pam_ptr->Error_I2C_GPIO_Ext = ERROR;
        pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].Error_State_eF_out = ERROR;
    }

    return error_I2C;
}

/** @brief  Get Power Good state power channel for get telemetry SP
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_pwr_ch - number channel on/off (PAM_PWR_TM_SP_Ch1 - PAM_PWR_TM_SP_Ch6).
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_Get_PG_PWR_TM_SP_Ch( _PAM *pam_ptr, uint8_t num_pwr_ch ){

	uint8_t i = 0;
	uint8_t read_val_pin_PG_eF = 0;
	int8_t error_I2C = ERROR_N;
	_PAM_table pam_table;

	SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

	//Get real state value pins TCA9539.
	pam_table = PAM__Table(num_pwr_ch, PAM_TYPE_PWR_CH_TM_SP);

	i=0;
 	error_I2C = ERROR_N;

	while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){///Read real value input pin PG.

		if( TCA6424A_conf_IO_dir_input( pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID ) == SUCCESS) {

			error_I2C = TCA6424A_read_input_pin(pam_table.I2Cx_PORT, pam_table.I2C_addr_GPIO_Ext, pam_table.pin_State_ID, &read_val_pin_PG_eF );
		}

		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay( pam_i2c_delay_att_conn );
		}
	}

	if( error_I2C == SUCCESS  ){

        pam_ptr->Error_I2C_GPIO_Ext = SUCCESS;

        if( pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out == ENABLE ){
        	pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].PG_eF_out = !read_val_pin_PG_eF;
        }else{
        	pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].PG_eF_out = SUCCESS;
        }

	}else{
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pam_ptr->Error_I2C_GPIO_Ext = ERROR;
	}

	return error_I2C;
}
