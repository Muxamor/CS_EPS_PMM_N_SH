#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "Error_Handler.h"
#include "TCA9548.h"
#include "PAM/pam_config.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_init_IC.h"
#include "PAM/pam_ctrl.h"
#include "PAM/pam_init.h"


/** @brief  Initialisation PAM board
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PAM_init(_PAM *pam_ptr){

	int8_t error_status = SUCCESS;
	uint8_t num_pwr_ch;

	//Restoring power channels.
	error_status += PAM_Set_state_PWR_Supply(pam_ptr, PAM_PWR_DC_DC, pam_ptr->State_DC_DC);
	error_status += PAM_Set_state_PWR_Supply(pam_ptr, PAM_PWR_LDO, pam_ptr->State_LDO);

	error_status += PAM_Get_PG_PWR_Supply(pam_ptr, PAM_PWR_DC_DC);
	error_status += PAM_Get_PG_PWR_Supply(pam_ptr, PAM_PWR_LDO);

	error_status += PAM_Check_state_PWR_Supply(pam_ptr, PAM_PWR_DC_DC);
	error_status += PAM_Check_state_PWR_Supply(pam_ptr, PAM_PWR_LDO);

    if( (pam_ptr->State_DC_DC == ENABLE) && ( (pam_ptr->PG_DC_DC == ERROR) || (pam_ptr->Error_State_DC_DC == ERROR) ) && (pam_ptr->State_LDO == DISABLE) ){
        error_status += PAM_Set_state_PWR_Supply(pam_ptr, PAM_PWR_LDO, ENABLE);
        pam_ptr->State_LDO = ENABLE;
	}

    //Init TM SP power channels.
    for( num_pwr_ch = 0; num_pwr_ch < PAM_PWR_TM_SP_Ch_quantity; num_pwr_ch++ ){
        error_status += PAM_Set_state_PWR_TM_SP_CH(pam_ptr, num_pwr_ch, pam_ptr->PWR_Channel_TM_SP[num_pwr_ch].State_eF_out);
    }

    if( pam_ptr->State_DC_DC == ENABLE || pam_ptr->State_LDO == ENABLE ){

        //Disable all channels TCA9548 I2C MUX on PAM module.
        //Note: We don’t check return errors because in any case, trying to configure the chips behind the I2C MUX
        TCA9548_Disable_I2C_ch(PAM_I2C_PORT, PAM_I2CADDR_I2C_MUX_1, TCA9548_ALL_CHANNELS);
        TCA9548_Disable_I2C_ch(PAM_I2C_PORT, PAM_I2CADDR_I2C_MUX_2, TCA9548_ALL_CHANNELS);

        //Initialization temperature sensors.
        error_status += PAM_init_TMP1075(pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_1, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6);
        error_status += PAM_init_TMP1075(pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_2, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6);
        error_status += PAM_init_TMP1075(pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_3, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6);
        error_status += PAM_init_TMP1075(pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_4, PAM_I2CADDR_I2C_MUX_2, TCA9548_CH6);

        //Initialization Power monitor INA231 power channel
        for( num_pwr_ch = 0; num_pwr_ch < PAM_PWR_IN_Ch_quantity; num_pwr_ch++ ){
            error_status += PAM_init_PWR_Mon_INA231(pam_ptr, num_pwr_ch);
        }
    }


	if(error_status != SUCCESS){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief  Initialisation temperature sensors on chosen solar panel
	@param  *pam_ptr - pointer to struct which contain all information about PAM.
	@param  num_ch - number channel SP (PAM_SP_Channel_1 - PAM_SP_Channel_6).
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
//ErrorStatus PAM_init_SP_Temp_sensors(_PAM *pam_ptr, uint8_t num_ch){
//
//    int8_t error_status = SUCCESS;
//
//    _PAM_table pam_table;
//
//    pam_table = PAM__Table(num_ch);
//
//    error_status += PAM_init_TMP1075( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_1, PAM_I2CADDR_I2C_MUX_2, pam_table.I2C_MUX_Ch );
//
//    //	if((num_ch != PAM_SP_1) || (num_ch != PAM_SP_3)){ // there is no sensor with this address on the panel on Y+ Y-
//    //		error_status += PAM_init_TMP1075( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_2, PAM_I2CADDR_I2C_MUX_2, pam_table.I2C_MUX_Ch );
//    //	}
//    //
//    //	if((num_ch != PAM_SP_1) || (num_ch != PAM_SP_3)){ // there is no sensor with this address on the panel on Y+ Y-
//    //		error_status += PAM_init_TMP1075( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_3, PAM_I2CADDR_I2C_MUX_2, pam_table.I2C_MUX_Ch );
//    //	}
//
//    error_status += PAM_init_TMP1075( pam_ptr, PAM_I2C_PORT, PAM_I2CADDR_TMP1075_4, PAM_I2CADDR_I2C_MUX_2, pam_table.I2C_MUX_Ch );
//
//    if(error_status != SUCCESS){
//#ifdef DEBUGprintf
//        Error_Handler();
//#endif
//        return ERROR_N;
//    }
//
//    return SUCCESS;
//}


