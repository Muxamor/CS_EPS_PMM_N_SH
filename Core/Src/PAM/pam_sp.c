#include "stm32l4xx.h"
#include "PAM/pam_struct.h"
#include "PAM/pam_sp_ctrl.h"
#include "PAM/pam_sp.h"


/** @brief Get temperature a solar panel.
	@param  *pam_ptr - pointer to struct which contain all information about solar panels.
	@param  number_sp - number solar panel.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PAM_SP_Get_Telemetry(_PAM *pam_ptr, uint8_t number_sp){

    int8_t error_status = SUCCESS;

    error_status = PAM_SP_Get_Temperature( pam_ptr, number_sp);

    if(error_status != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}