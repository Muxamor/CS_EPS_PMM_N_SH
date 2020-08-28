#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "TCA9548.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_sp_init_IC.h"


/** @brief  Init TMP1075 temp. sensor on solar panel module
	@param  *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_addr - I2C MUX address
	@param  i2c_mux_ch - Number channel MUX
	@retval 0 - SUCCESS, -1 - Error temp. sensor , -2  Error I2C MUX
*/
ErrorStatus PAM_SP_init_TMP1075(_PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch ){

    uint8_t i = 0;
    int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
    int8_t Error_I2C_MUX = ERROR_N;

    SW_TMUX1209_I2C_main_PAM(); // Switch MUX to PAM I2C bus on PMM

    //Enable I2C MUX channel
    i=0;
    error_I2C = ERROR_N;
    while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

        error_I2C = TCA9548_Enable_I2C_ch( I2Cx, i2c_mux_addr, i2c_mux_ch );

        if( error_I2C != SUCCESS ){
            i++;
            LL_mDelay( pam_i2c_delay_att_conn );
        }
    }

    Error_I2C_MUX = error_I2C;

    if (error_I2C == SUCCESS ){

        //Setup TMP1075
        i=0;
        error_I2C = ERROR_N;

        while( ( error_I2C != SUCCESS ) && ( i < pam_i2c_attempt_conn ) ){

            if (TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOUS_CONV) == SUCCESS ){
                if ( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) == SUCCESS ){

                    error_I2C = TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr);
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