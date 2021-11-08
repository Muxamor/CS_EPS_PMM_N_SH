#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_init_IC.h"

/** @brief	Initialize single TMP1075 sensor.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	AddrTMP1075 - 7-bit device address.
    @retval 0-OK, ERROR_N-Error
 */
ErrorStatus PBM_T1_TMP1075_InitState(I2C_TypeDef *I2Cx, uint8_t AddrTMP1075) {

	int8_t Error = ERROR_N;
	uint8_t count = 0;

	while ((Error != 0) && (count < PBM_T1_I2C_ATTEMPT_CONN)) {
		if (TMP1075_set_mode(I2Cx, AddrTMP1075, TMP1075_CONTINUOUS_CONV) == SUCCESS) {
			if (TMP1075_ALERT_active_level(I2Cx, AddrTMP1075, TMP1075_ACTIVE_HIGH) == SUCCESS) {
				if (TMP1075_set_mode_ALERT_pin(I2Cx, AddrTMP1075, TMP1075_COMPARATOR_MODE) == SUCCESS) {
					if (TMP1075_set_time_conversion(I2Cx, AddrTMP1075, TMP1075_CR_MEDIUM) == SUCCESS) {
						if (TMP1075_ALERT_sensitivity(I2Cx, AddrTMP1075, TMP1075_TWO_FAULT) == SUCCESS) {
							if (TMP1075_set_low_limit(I2Cx, AddrTMP1075, PBM_T1_TMP1075_TEMP_LO) == SUCCESS) {
								Error = TMP1075_set_high_limit(I2Cx, AddrTMP1075, PBM_T1_TMP1075_TEMP_HI);
							}
						}
					}
				}
			}
		}
		if (Error != SUCCESS) {
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
			count++;
			continue;
		};
	}

	return Error;
}

