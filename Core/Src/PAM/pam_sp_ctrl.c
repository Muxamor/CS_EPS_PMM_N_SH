#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "PCA9534.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_sp_ctrl.h"


/** @brief Get state Ideal Diode at a solar panel.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  number_sp - number solar panel.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_SP_Get_State_ID(_PAM *pam_ptr, uint8_t number_sp){

    uint8_t i=0;
    uint8_t Ideal_Diode_value = 0;
    int8_t error_status = ERROR_N;
    int8_t error_count = SUCCESS;
    _PAM_SP_table pam_sp_table = {0};

    pam_sp_table = PAM_SP_Table(number_sp);

    if( pam_ptr->PWR_Channel_TM_SP[pam_sp_table.PWR_number_ch_TM_SP].State_eF_out == ENABLE ){

        for( i = 0; i < pam_sp_table.ID_quantity; i++ ){
            error_status = ERROR_N;
            error_status = PAM_SP_Read_GPIO_State_ID( pam_ptr, pam_sp_table.PAM_I2Cx_PORT, pam_sp_table.SP_I2C_addr_GPIO_Ext,  pam_sp_table.Num_pin_GPIO_Ext_ID[i], pam_sp_table.PAM_I2C_addr_I2C_MUX, pam_sp_table.PAM_I2C_MUX_Ch, &Ideal_Diode_value );
            error_count =  error_count + error_status;

            if(error_status == SUCCESS ){
                pam_ptr->Solar_Panel[number_sp].State_ID[i] = Ideal_Diode_value;
                pam_ptr->Solar_Panel[number_sp].Error_I2C_GPIO_Ext1 = SUCCESS;
            }else{
                pam_ptr->Solar_Panel[number_sp].Error_I2C_GPIO_Ext1 = ERROR;
            }
        }
    }else{
        for( i = 0; i < pam_sp_table.ID_quantity; i++ ){
            pam_ptr->Solar_Panel[number_sp].State_ID[i] = 0;
        }
    }

    if(error_count != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}



/** @brief  Read GPIO extander and get Ideal diode state
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
    @retval  temp_value - pointer to the value of temperature.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_SP_Read_GPIO_State_ID( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t gpio_ext_addr,  uint8_t gpio_pin_number, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch, uint8_t* ID_value ){

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

            if(  PCA9534_conf_IO_dir_input(I2Cx, gpio_ext_addr, gpio_pin_number) == SUCCESS){
                if( PCA9534_conf_IO_pol_normal(I2Cx, gpio_ext_addr, gpio_pin_number) == SUCCESS ){
                    error_I2C = PCA9534_read_input_pin(I2Cx, gpio_ext_addr, gpio_pin_number, ID_value);
                }
            }

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

    if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
        return ERROR_N;
    }

    return SUCCESS;
}



/** @brief Get temperature a solar panel.
    @param  *pam_ptr - pointer to struct which contain all information about PAM.
    @param  number_sp - number solar panel.
    @retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_SP_Get_Temperature(_PAM *pam_ptr, uint8_t number_sp){

    int8_t temperature_value;
    uint8_t i=0;
    int8_t error_status = ERROR_N;
    int8_t error_count = SUCCESS;
    _PAM_SP_table pam_sp_table = {0};

    pam_sp_table = PAM_SP_Table(number_sp);

    if( pam_ptr->PWR_Channel_TM_SP[pam_sp_table.PWR_number_ch_TM_SP].State_eF_out == ENABLE ){

        for( i = 0; i < pam_sp_table.temp_sensor_quantity; i++ ){
            error_status = ERROR_N;
            error_status = PAM_SP_Read_Temp_Sensor( pam_ptr, pam_sp_table.PAM_I2Cx_PORT, pam_sp_table.SP_I2C_addr_temp_senor[i], pam_sp_table.PAM_I2C_addr_I2C_MUX, pam_sp_table.PAM_I2C_MUX_Ch, &temperature_value );

            if( error_status == ERROR_N ){
                pam_ptr->Solar_Panel[number_sp].Temp_value[i] = 0x7F;
                pam_ptr->Solar_Panel[number_sp].Error_temp_sensor[i] = ERROR;
            }else{
                pam_ptr->Solar_Panel[number_sp].Temp_value[i] = temperature_value;
                pam_ptr->Solar_Panel[number_sp].Error_temp_sensor[i] = SUCCESS;
            }
            error_count =  error_count + error_status;
        }

    }else{
        for( i = 0; i < pam_sp_table.temp_sensor_quantity; i++ ){
            if(pam_ptr->Solar_Panel[number_sp].Error_temp_sensor[i] == SUCCESS){
                pam_ptr->Solar_Panel[number_sp].Temp_value[i] = 0x00;
            }
        }
    }

    if(error_count != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief  Read temperature from a temperature sensor.  Get temperature solar panel.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch  - Number channel MUX
    @retval  temp_value - pointer to the value of temperature.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_SP_Read_Temp_Sensor( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch, int8_t* temp_value ){

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

            error_I2C = TMP1075_read_int8_temperature( I2Cx, tmp1075_addr, temp_value);

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

    if( (error_I2C == ERROR_N) || (Error_I2C_MUX == ERROR_N) ){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Erase data for all Solar panels .
	@param 	pam[] - structure data for all Solar panels.
 */
void PAM_SP_EraseData(_PAM *pam_ptr){

    uint8_t  num_sp = 0;
    uint8_t  i = 0;
    _PAM_SP_table pam_sp_table = {0};

    for( num_sp = 0; num_sp < PAM_SP_quantity; num_sp++ ){

        pam_sp_table = PAM_SP_Table(num_sp);

        for( i = 0; i < pam_sp_table.temp_sensor_quantity; i++ ){
            if(pam_ptr->Solar_Panel[num_sp].Error_temp_sensor[i] == SUCCESS){
                pam_ptr->Solar_Panel[num_sp].Temp_value[i] = 0x00;
            }
        }

        for( i = 0; i < pam_sp_table.ID_quantity; i++ ){
            pam_ptr->Solar_Panel[num_sp].State_ID[i] = 0;
        }
    }
}