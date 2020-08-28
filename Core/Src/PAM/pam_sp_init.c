#include "stm32l4xx.h"
#include "Error_Handler.h"

#include "PAM/pam_struct.h"
#include "PAM/pam_sp_init_IC.h"
#include "PAM/pam_sp_init.h"


/** @brief Init Solar panel.
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  number_sp - number solar panel.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_SP_init(_PAM *pam_ptr, uint8_t number_sp){

    uint8_t i=0;
    int8_t error_status = ERROR_N;
    int8_t error_count = SUCCESS;
    _PAM_SP_table pam_sp_table = {0};

    pam_sp_table = PAM_SP_Table(number_sp);

    if( pam_ptr->PWR_Channel_TM_SP[pam_sp_table.PWR_number_ch_TM_SP].State_eF_out == ENABLE ){

        for( i = 0; i < pam_sp_table.temp_sensor_quantity; i++ ){
            error_status = ERROR_N;
            error_status = PAM_SP_init_TMP1075(pam_ptr, pam_sp_table.PAM_I2Cx_PORT, pam_sp_table.SP_I2C_addr_temp_senor[i], pam_sp_table.PAM_I2C_addr_I2C_MUX, pam_sp_table.PAM_I2C_MUX_Ch);

            if( error_status == ERROR_N ){
                pam_ptr->Solar_Panel[number_sp].Error_temp_sensor[i] = ERROR;
            }else{
                pam_ptr->Solar_Panel[number_sp].Error_temp_sensor[i] = SUCCESS;
            }
            error_count =  error_count + error_status;
        }
    }else{
        for( i = 0; i < pam_sp_table.temp_sensor_quantity; i++ ){
            pam_ptr->Solar_Panel[number_sp].Error_temp_sensor[i] = SUCCESS;
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


