#include "main.h"
#include "SetupPeriph.h"
#include "TMP1075.h"
#include "PCA9534.h"
#include "PBM_config.h"
#include "PBM_init_IC.h"

/** @brief	Initialize single TMP1075 sensor.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	AddrTMP1075 - 7-bit device address.
 @retval 0-OK, ERROR_N-Error
 */
ErrorStatus TMP1075_InitState(I2C_TypeDef *I2Cx, uint8_t AddrTMP1075) {

	int8_t Error = -1;
	uint8_t count = 0;

	SW_TMUX1209_I2C_main_PBM();

	while ((Error != 0) && (count < 3)) {
		Error = TMP1075_set_mode(I2Cx, AddrTMP1075, TMP1075_CONTINUOS_CONV);
		if (Error != 0) {
			break;
		};
		Error = TMP1075_ALERT_active_level(I2Cx, AddrTMP1075, TMP1075_ACTIVE_HIGH);
		if (Error != 0) {
			break;
		};
		Error = TMP1075_set_mode_ALERT_pin(I2Cx, AddrTMP1075, TMP1075_COMPARATOR_MODE);
		if (Error != 0) {
			break;
		};
		Error = TMP1075_set_time_conversion(I2Cx, AddrTMP1075, TMP1075_CR_MEDIUM);
		if (Error != 0) {
			break;
		};
		Error = TMP1075_ALERT_sensitivity(I2Cx, AddrTMP1075, TMP1075_TWO_FAULT);
		if (Error != 0) {
			break;
		};
		Error = TMP1075_set_low_limit(I2Cx, AddrTMP1075, PBM_TMP1075_TEMP_LO);
		if (Error != 0) {
			break;
		};
		Error = TMP1075_set_high_limit(I2Cx, AddrTMP1075, PBM_TMP1075_TEMP_HI);
		count++;
	}
	if (Error != 0) {
		return ERROR_N;
	}
	return SUCCESS;
}

/** @brief	Initialize PCA9534 GPIO mux.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	AddrPCA9534 - 7-bit device address.
 @retval 0-OK, ERROR_N-Error
 */
ErrorStatus PCA9534_InitState(I2C_TypeDef *I2Cx, uint8_t AddrPCA9534) {

	int8_t Error = -1;
	uint8_t count = 0;
	uint8_t InputPin = 0, OutputPin = 0;

	SW_TMUX1209_I2C_main_PBM();

	while ((Error != 0) && (count < 3)) {
		InputPin = PCA9534_IO_P01 | PCA9534_IO_P02 | PCA9534_IO_P03 | PCA9534_IO_P04 | PCA9534_IO_P06 | PCA9534_IO_P07;
		Error = PCA9534_conf_IO_dir_input(I2Cx, AddrPCA9534, InputPin);
		if (Error != 0) {
			break;
		};
		OutputPin = PCA9534_IO_P00 | PCA9534_IO_P05;
		Error = PCA9534_conf_IO_dir_output(I2Cx, AddrPCA9534, OutputPin);
		if (Error != 0) {
			break;
		};

		Error = PCA9534_Reset_output_pin(I2Cx, AddrPCA9534, PCA9534_IO_P00);
		if (Error != 0) {
			break;
		};
		Error = PCA9534_Reset_output_pin(I2Cx, AddrPCA9534, PCA9534_IO_P05);
		count++;
	}

	if (Error != 0) {
		return ERROR_N;
	}
	return SUCCESS;
}
