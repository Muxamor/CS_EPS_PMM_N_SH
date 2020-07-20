#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "INA231.h"


/** @brief	Read "configuration" register.This register resets all registers and
			controls shunt voltage and bus voltage, ADC conversion times and averaging,
			as well as the device operating mode.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store data from "configuration" register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "shunt voltage" register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store shunt voltage measurement data.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_shunt_volt_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_SHUNT_VOLTAGE_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "bus voltage" register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store bus voltage measurement data.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_bus_volt_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_BUS_VOLTAGE_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "power" register. This register contains the value of the
			calculated power being delivered to the	load.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store calculated power data.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_power_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_POWER_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "current" register. This register contains the value of the
			calculated current flowing through the shunt resistor.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store calculated current data.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_current_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CURRENT_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "calibration" register. This register sets the full-scale range
			and LSB of the current and power measurements. This register sets the
			overall system calibration.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store calibration setting data.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_calibration_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CALIBRATION_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "mask/enable" register. This register sets the alert configuration
			and conversion ready flag.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store alert configuration data.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_mask_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read "alert limit" register. This register contains the limit value to
			compare to the selected alert function.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store alert limit value.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_alert_lim_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_ALERT_LIMIT_REG_ADDR, read_data) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read averaging mode. These bits set the number of samples that are collected and averaged together
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual averaging mode.
			Possible values:
				0x00 - INA231_AVERAGING_NUM_1
				0x01 - INA231_AVERAGING_NUM_4
				0x02 - INA231_AVERAGING_NUM_16
				0x03 - INA231_AVERAGING_NUM_64
				0x04 - INA231_AVERAGING_NUM_128
				0x05 - INA231_AVERAGING_NUM_256
				0x06 - INA231_AVERAGING_NUM_512
				0x07 - INA231_AVERAGING_NUM_1024
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_aver_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}

	*read_data = (uint8_t)((read_reg >> 9) & 0x07);

	return SUCCESS;
}


/** @brief	Read bus voltage conversion time. These bits set the conversion time for the bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual conversion time.
			Possible values:
				0x00 - INA231_CONVERSION_TIME_140us
				0x01 - INA231_CONVERSION_TIME_204us
				0x02 - INA231_CONVERSION_TIME_332us
				0x03 - INA231_CONVERSION_TIME_588us
				0x04 - INA231_CONVERSION_TIME_1100us
				0x05 - INA231_CONVERSION_TIME_2116us
				0x06 - INA231_CONVERSION_TIME_4156us
				0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_bus_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 6) & 0x07);

	return SUCCESS;
}


/** @brief	Read shunt voltage conversion time. These bits set the conversion time for the shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual conversion time.
				Possible values:
				0x00 - INA231_CONVERSION_TIME_140us
				0x01 - INA231_CONVERSION_TIME_204us
				0x02 - INA231_CONVERSION_TIME_332us
				0x03 - INA231_CONVERSION_TIME_588us
				0x04 - INA231_CONVERSION_TIME_1100us
				0x05 - INA231_CONVERSION_TIME_2116us
				0x06 - INA231_CONVERSION_TIME_4156us
				0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_shunt_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 3) & 0x07);

	return SUCCESS;
}


/** @brief	Read operation mode. These bits select continuous, triggered, or power-down mode of operation.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual operation mode.
			Possible values:
				0x00 - INA231_POWER_DOWN(not used)
				0x01 - INA231_SHUNT_VOLTAGE_TRIGGERED
				0x02 - INA231_BUS_VOLTAGE_TRIGGERED
				0x03 - INA231_SHUNT_AND_BUS_TRIGGERED
				0x04 - INA231_POWER_DOWN
				0x05 - INA231_SHUNT_VOLTAGE_CONTINUOUS
				0x06 - INA231_BUS_VOLTAGE_CONTINUOUS
				0x07 - INA231_SHUNT_AND_BUS_CONTINUOUS
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)(read_reg & 0x07);

	return SUCCESS;
}




/** @brief	Read "Shunt Voltage Overvoltage" bit (Mask/Enable Register - SOL). Setting this bit high configures the ALERT pin to
  			be asserted when the shunt voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_shunt_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)(read_reg >> 15);

	return SUCCESS;
}


/** @brief	Read "Shunt Voltage Undervoltage" bit (Mask/Enable Register - SUL). Setting this bit high configures the ALERT pin to
  	  	  	be asserted when the shunt voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_shunt_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 14) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Bus Voltage Overvoltage" bit (Mask/Enable Register - BOL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_bus_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 13) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Bus Voltage Undervoltage" bit (Mask/Enable Register - BUL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_bus_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 12) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Power Overlimit" bit (Mask/Enable Register - POL). Setting this bit high configures the ALERT pin to
  			be asserted when the power calculation exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_power_overlimit(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 11) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Conversion Ready" bit (Mask/Enable Register - CNVR). Setting this bit high configures the ALERT pin to
  			be asserted when the Conversion Ready Flag bit (CVRF, bit 3) is asserted, indicating
  			that the device is ready for the next conversion
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_conv_ready_alert(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 10) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Alert Function Flag" bit (Mask/Enable Register - AFF). Although only one alert function at a time can be monitored
  			at the ALERT pin, the Conversion Ready bit (CNVR, bit 10) can also be enabled to assert the
  			ALERT pin. Reading the Alert Function Flag bit after an alert can help determine if the alert
  			function was the source of the alert. When the Alert Latch Enable bit is set to Latch mode,
  			the Alert Function Flag bit clears only when the Mask/Enable register is read. When the
  			Alert Latch Enable bit is set to Transparent mode, the Alert Function Flag bit is cleared
			after the next conversion that does not result in an alert condition.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_alert_function(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 4) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Conversion Ready Flag" bit (Mask/Enable Register - CVRF). Although the INA231 can be read at any time, and the data from
  			the last conversion are available, this bit is provided to help coordinate single-shot or triggered
  			conversions. This bit is set after all conversions, averaging, and	multiplications are complete.
  			This bit clears under the following conditions in single-shot mode:
			1) Writing to the Configuration register (except for power-down or disable selections)
			2) Reading the Mask/Enable register
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_conv_ready(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 3) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Math Overflow Flag" bit (Mask/Enable Register - OVF). This bit is set to 1 if an arithmetic operation results in
  			an overflow error; it indicates that current and power data may be invalid.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_math_overflow(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 2) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Alert Polarity" bit (Mask/Enable Register - APOL). Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_alert_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 1) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Alert Latch Enable" bit (Mask/Enable Register - LEN). Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_alert_latch(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)(read_reg & 0x01);

	return SUCCESS;
}






/** @brief	Write "Configuration" register. This register resets all registers and
			controls shunt voltage and bus voltage, ADC conversion times and averaging,
			as well as the device operating mode.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will be write to "Configuration" register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data){

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_data) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Write "Calibration" register. This register sets the full-scale range
			and LSB of the current and power measurements. This register sets the
			overall system calibration.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will be write to "Calibration" register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_calibration_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data){

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CALIBRATION_REG_ADDR, write_data) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Write "Mask/Enable" register. This register sets the alert configuration
			and conversion ready flag.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will be write to "Mask/Enable" register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_mask_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data){

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_data) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Write "Alert Limit" register. This register contains the limit value to
			compare to the selected alert function.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will be write to "Alert Limit" register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_alert_lim_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data){

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_ALERT_LIMIT_REG_ADDR, write_data) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}



/** @brief	Write averaging mode. These bits set the number of samples that are collected and averaged together
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will set averaging mode.
			0x00 - INA231_AVERAGING_NUM_1
			0x01 - INA231_AVERAGING_NUM_4
			0x02 - INA231_AVERAGING_NUM_16
			0x03 - INA231_AVERAGING_NUM_64
			0x04 - INA231_AVERAGING_NUM_128
			0x05 - INA231_AVERAGING_NUM_256
			0x06 - INA231_AVERAGING_NUM_512
			0x07 - INA231_AVERAGING_NUM_1024
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_aver_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((write_data << 9) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Write bus voltage conversion time. These bits set the conversion time for the bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will set bus voltage conversion time.
			0x00 - INA231_CONVERSION_TIME_140us
			0x01 - INA231_CONVERSION_TIME_204us
			0x02 - INA231_CONVERSION_TIME_332us
			0x03 - INA231_CONVERSION_TIME_588us
			0x04 - INA231_CONVERSION_TIME_1100us
			0x05 - INA231_CONVERSION_TIME_2116us
			0x06 - INA231_CONVERSION_TIME_4156us
			0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_bus_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((write_data << 6) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Write shunt voltage conversion time. These bits set the conversion time for the shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will set shunt voltage conversion time.
			0x00 - INA231_CONVERSION_TIME_140us
			0x01 - INA231_CONVERSION_TIME_204us
			0x02 - INA231_CONVERSION_TIME_332us
			0x03 - INA231_CONVERSION_TIME_588us
			0x04 - INA231_CONVERSION_TIME_1100us
			0x05 - INA231_CONVERSION_TIME_2116us
			0x06 - INA231_CONVERSION_TIME_4156us
			0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_shunt_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((write_data << 3) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Write operation mode. These bits select continuous, triggered, or power-down mode of operation.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will set operation mode.
			0x00 - INA231_POWER_DOWN(not used)
			0x01 - INA231_SHUNT_VOLTAGE_TRIGGERED
			0x02 - INA231_BUS_VOLTAGE_TRIGGERED
			0x03 - INA231_SHUNT_AND_BUS_TRIGGERED
			0x04 - INA231_POWER_DOWN
			0x05 - INA231_SHUNT_VOLTAGE_CONTINUOUS
			0x06 - INA231_BUS_VOLTAGE_CONTINUOUS
			0x07 - INA231_SHUNT_AND_BUS_CONTINUOUS
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_write_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)(write_data | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}





/** @brief	Set "Shunt Voltage Overvoltage" bit (Mask/Enable Register - SOL). Setting this bit high configures the ALERT pin to
  			be asserted when the shunt voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_shunt_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 15) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Reset "Shunt Voltage Overvoltage" bit (Mask/Enable Register - SOL).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_shunt_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 15)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Shunt Voltage Undervoltage" bit (Mask/Enable Register - SUL). Setting this bit high configures the ALERT pin to
  	  	  	be asserted when the shunt voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_shunt_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 14) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Shunt Voltage Undervoltage" bit (Mask/Enable Register - SUL).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_shunt_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 14)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Bus Voltage Overvoltage" bit (Mask/Enable Register - BOL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_bus_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 13) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Bus Voltage Overvoltage" bit (Mask/Enable Register - BOL).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_bus_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 13)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Bus Voltage Undervoltage" bit (Mask/Enable Register - BUL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_bus_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 12) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Bus Voltage Undervoltage" bit (Mask/Enable Register - BUL).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_bus_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 12)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Power Overlimit" bit (Mask/Enable Register - POL). Setting this bit high configures the ALERT pin to
  			be asserted when the power calculation exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_power_overlimit(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 11) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Power Overlimit" bit (Mask/Enable Register - POL).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_power_overlimit(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 11)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Conversion Ready" bit (Mask/Enable Register - CNVR). Setting this bit high configures the ALERT pin to
  			be asserted when the Conversion Ready Flag bit (CVRF, bit 3) is asserted, indicating
  			that the device is ready for the next conversion
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_conv_ready_alert(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 10) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Conversion Ready" bit (Mask/Enable Register - CNVR).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_conv_ready_alert(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 10)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Alert Polarity" bit (Mask/Enable Register - APOL). Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_alert_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 1) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Res "Alert Polarity" bit (Mask/Enable Register - APOL). Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_alert_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 1)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Alert Latch Enable" bit (Mask/Enable Register - LEN). Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_alert_latch(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 0) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Alert Latch Enable" bit (Mask/Enable Register - LEN). Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_alert_latch(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((~(1 << 0)) & read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Power reset of INA231. Setting this bit to 1 generates a system reset that is the same
  			as a power-on reset; all registers are reset to default values. This bit self-clears.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_power_reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint16_t)((1 << 15) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Initialization of INA231. Setting calibration register for starting measuring.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	Rshunt - value of shunt resistor (Ohm float).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_calibration_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float Rshunt){

	uint16_t calibration = (uint16_t)( 167.77216 / (max_current * Rshunt) );

	if(INA231_write_calibration_reg(I2Cx, I2C_INA231_addr, calibration) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Initialization of INA231. Setting calibration register for starting measuring.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	Rshunt - value of shunt resistor (mOhm uint16_t).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_calibration_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t Rshunt){

	uint16_t calibration = 0;

	calibration = (uint16_t)( 167772160 / (max_current * Rshunt) );

	if(INA231_write_calibration_reg(I2Cx, I2C_INA231_addr, calibration) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Current measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	*current - pointer to store measured current value in Amps.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current){

	uint16_t current_reg = 0;
	int16_t sign_current_reg = 0;
	float current_lsb = max_current / 32768.0;

	if(INA231_read_current_reg(I2Cx, I2C_INA231_addr, &current_reg) != SUCCESS){
		return ERROR_N;
	}

	sign_current_reg = (int16_t)current_reg;

	*current = (float)( current_lsb * ((float)sign_current_reg) );

	return SUCCESS;
}


/** @brief	Current measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	*current - pointer to store measured current value in milliAmps.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Current_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current){

	uint16_t current_reg = 0;
	int16_t sign_current_reg = 0;

	float current_lsb = ((float)max_current) / 32768.0; //Current in mA

	if(INA231_read_current_reg(I2Cx, I2C_INA231_addr, &current_reg) != SUCCESS){
		return ERROR_N;
	}

	sign_current_reg = (int16_t)current_reg;

	*current = (int16_t)( current_lsb * ((float)sign_current_reg));

	return SUCCESS;
}

/** @brief	Power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	*power - pointer to store measured power value in Watts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *power){

	uint16_t power_reg = 0;

	float power_lsb = (max_current / 32768.0) * 25.0; // 25 times see Datasheet

	if(INA231_read_power_reg(I2Cx, I2C_INA231_addr, &power_reg) != SUCCESS){
		return ERROR_N;
	}

	*power = (float)( power_lsb * ((float)power_reg) );

	return SUCCESS;
}

/** @brief	Power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	*power - pointer to store measured power value in milliWatts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Power_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *power){

	uint16_t power_reg = 0;

	float power_lsb = ( ((float)max_current) / 32768.0 ) * 25.0; // 25 times see Datasheet, Current in mA

	if(INA231_read_power_reg(I2Cx, I2C_INA231_addr, &power_reg) != SUCCESS){
		return ERROR_N;
	}

	*power = (uint16_t)( power_lsb * ((float)power_reg));

	return SUCCESS;
}

/** @brief	Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_bus_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *bus_voltage){

	uint16_t bus_volt_reg = 0;
	float voltage_lsb = 0.00125; //Fixed value 1.25mV

	if(INA231_read_bus_volt_reg(I2Cx, I2C_INA231_addr, &bus_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*bus_voltage = (float)( voltage_lsb * ((float)bus_volt_reg) );

	return SUCCESS;
}

/** @brief	Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in milliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_bus_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *bus_voltage){

	uint16_t bus_volt_reg = 0;
	float voltage_lsb = 1.25; //Fixed value 1.25mV

	if(INA231_read_bus_volt_reg(I2Cx, I2C_INA231_addr, &bus_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*bus_voltage = (uint16_t)( voltage_lsb * ((float)bus_volt_reg)); //*1000 convert to mVolts

	return SUCCESS;
}


/** @brief	Shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_shunt_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *shunt_voltage){

	uint16_t shunt_volt_reg = 0;
	int16_t sign_shunt_volt_reg = 0;

	float voltage_lsb = 0.0000025; //Fixed value 2.5uV

	if(INA231_read_shunt_volt_reg(I2Cx, I2C_INA231_addr, &shunt_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

	*shunt_voltage = (float)( voltage_lsb * ((float)sign_shunt_volt_reg) );

	return SUCCESS;
}

/** @brief	Shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in milliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_shunt_V_int16( I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, int16_t *shunt_voltage ){

	uint16_t shunt_volt_reg = 0;
	int16_t sign_shunt_volt_reg = 0;
	float voltage_lsb = 0.0025; //Fixed value 2.5uV

	if(INA231_read_shunt_volt_reg( I2Cx, I2C_INA231_addr, &shunt_volt_reg) != SUCCESS ){
		return ERROR_N;
	}

	sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

	*shunt_voltage = (int16_t)( voltage_lsb * ((float)sign_shunt_volt_reg));

	return SUCCESS;
}

/** @brief	Current, voltage and power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*current - pointer to store measured current value in Amps.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@param 	*power - pointer to store measured power value in Watts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_I_V_P_float( I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current, float *bus_voltage, float *power ){

	uint16_t current_reg = 0;
	int16_t sign_current_reg = 0;
	uint16_t power_reg = 0;
	uint16_t bus_volt_reg = 0;
	float current_lsb = max_current / 32768.0;
	float power_lsb = current_lsb * 25.0;

	//Get current value
	if(INA231_read_current_reg(I2Cx, I2C_INA231_addr, &current_reg) != SUCCESS){
		return ERROR_N;
	}

	sign_current_reg = (int16_t)current_reg;

	*current = (float)( current_lsb * ((float)sign_current_reg) );

	//Get voltage value
	if(INA231_read_bus_volt_reg(I2Cx, I2C_INA231_addr, &bus_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*bus_voltage = (float)(0.00125 * ((float)bus_volt_reg) );

	//Get power value
	if(INA231_read_power_reg(I2Cx, I2C_INA231_addr, &power_reg) != SUCCESS){
		return ERROR_N;
	}

	*power = (float)( power_lsb * ((float)power_reg) );

	return SUCCESS;
}

/** @brief Get value Current, Voltage and Power.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*current - pointer to store measured current value in milliAmps.
	@param 	*bus_voltage - pointer to store measured bus voltage value in milliVolts.
	@param 	*power - pointer to store measured power value in milliWatts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_I_V_P_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current, uint16_t *bus_voltage, uint16_t *power){

	uint16_t current_reg = 0;
	int16_t sign_current_reg = 0;
	uint16_t power_reg = 0;
	uint16_t bus_volt_reg = 0;
	float current_lsb = ((float)max_current) / 32768.0;
	float power_lsb = current_lsb * 25.0;

	//Get current value
	if(INA231_read_current_reg( I2Cx, I2C_INA231_addr, &current_reg ) != SUCCESS ){
		return ERROR_N;
	}

	sign_current_reg = (int16_t)current_reg;

	*current = (int16_t)( current_lsb * ((float)sign_current_reg) );

	//Get voltage value
	if(INA231_read_bus_volt_reg( I2Cx, I2C_INA231_addr, &bus_volt_reg ) != SUCCESS ){
		return ERROR_N;
	}

	*bus_voltage = (uint16_t)( 1.25 * ((float)bus_volt_reg) );

	//Get power value
	if(INA231_read_power_reg( I2Cx, I2C_INA231_addr, &power_reg ) != SUCCESS ){
		return ERROR_N;
	}

	*power = (uint16_t)( power_lsb * ((float)power_reg) );

	return SUCCESS;
}
