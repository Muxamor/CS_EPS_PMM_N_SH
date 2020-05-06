
#include "TMP1075.h"
#include "pmm_init_IC.h"



ErrorStatus PMM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){
 	
 	if( TMP1075_set_mode(I2Cx, tmp1075_addr, TMP1075_CONTINUOS_CONV) != SUCCESS ){
 		return ERROR_N;
 	}

 	if( TMP1075_set_time_conversion(I2Cx, tmp1075_addr, TMP1075_CR_MEDIUM) != SUCCESS ){
 		return ERROR_N;
 	}

 	if( TMP1075_disable_ALERT_pin( I2Cx, tmp1075_addr) != SUCCESS ){
 		return ERROR_N;
 	}

	return SUCCESS;
}
