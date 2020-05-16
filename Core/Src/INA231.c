/*
 * INA231.c
 *
 *  Created on: 9 мая 2020 г.
 *      Author: ftor
 */
#include "INA231.h"
#include "Error_Handler.h"
#include "i2c_comm.h"
#include "stm32l4xx.h"


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
ErrorStatus INA231_read_calibr_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){
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
ErrorStatus INA231_read_alert_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){
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
ErrorStatus INA231_read_bus_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

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
ErrorStatus INA231_read_shunt_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

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




/** @brief	Read "Shunt Voltage Overvoltage" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the shunt voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_sol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)(read_reg >> 15);

	return SUCCESS;
}


/** @brief	Read "Shunt Voltage Undervoltage" bit. Setting this bit high configures the ALERT pin to
  	  	  	be asserted when the shunt voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_sul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 14) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Bus Voltage Overvoltage" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_bol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 13) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Bus Voltage Undervoltage" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_bul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 12) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Power Overlimit" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the power calculation exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 11) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Conversion Ready" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the Conversion Ready Flag bit (CVRF, bit 3) is asserted, indicating
  			that the device is ready for the next conversion
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_cnvr(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 10) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Alert Function Flag" bit. Although only one alert function at a time can be monitored
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
ErrorStatus INA231_read_aff(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 4) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Conversion Ready Flag" bit. Although the INA231 can be read at any time, and the data from
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
ErrorStatus INA231_read_cvrf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 3) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Math Overflow Flag" bit. This bit is set to 1 if an arithmetic operation results in
  			an overflow error; it indicates that current and power data may be invalid.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_ovf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 2) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Alert Polarity" bit. Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_read_apol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

	uint16_t read_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	*read_data = (uint8_t)((read_reg >> 1) & 0x01);

	return SUCCESS;
}


/** @brief	Read "Alert Latch Enable" bit. Configures the latching feature of the ALERT pin and flag bits.
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
ErrorStatus INA231_read_len(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data){

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
ErrorStatus INA231_write_calibr_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data){
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
ErrorStatus INA231_write_alert_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data){
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
ErrorStatus INA231_write_bus_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data){

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
ErrorStatus INA231_write_shunt_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data){

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





/** @brief	Set "Shunt Voltage Overvoltage" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the shunt voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_sol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 15) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Reset "Shunt Voltage Overvoltage" bit.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_sol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 15) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Shunt Voltage Undervoltage" bit. Setting this bit high configures the ALERT pin to
  	  	  	be asserted when the shunt voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_sul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 14) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Shunt Voltage Undervoltage" bit.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_sul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 14) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Bus Voltage Overvoltage" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_bol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 13) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Bus Voltage Overvoltage" bit.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_bol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 13) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Bus Voltage Undervoltage" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_bul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 12) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Bus Voltage Undervoltage" bit.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_bul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 12) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Power Overlimit" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the power calculation exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 11) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Power Overlimit" bit.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 11) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Conversion Ready" bit. Setting this bit high configures the ALERT pin to
  			be asserted when the Conversion Ready Flag bit (CVRF, bit 3) is asserted, indicating
  			that the device is ready for the next conversion
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_cnvr(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 10) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Conversion Ready" bit.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_cnvr(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 10) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


///** @brief	Set "Alert Function Flag" bit. Although only one alert function at a time can be monitored
//  			at the ALERT pin, the Conversion Ready bit (CNVR, bit 10) can also be enabled to assert the
//  			ALERT pin. Reading the Alert Function Flag bit after an alert can help determine if the alert
//  			function was the source of the alert. When the Alert Latch Enable bit is set to Latch mode,
//  			the Alert Function Flag bit clears only when the Mask/Enable register is read. When the
//  			Alert Latch Enable bit is set to Transparent mode, the Alert Function Flag bit is cleared
//			after the next conversion that does not result in an alert condition.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	I2C_INA231_addr - 7-bit device address.
//	@retval 0-OK, -1-ERROR_N
//*/
//ErrorStatus INA231_set_aff(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){
//
//	uint16_t read_reg;
//	uint16_t write_reg;
//
//	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
//		return ERROR_N;
//	}
//	write_reg = (uint8_t)((1 << 4) | read_reg);
//
//	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}
//
///** @brief	Reset "Alert Function Flag" bit.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	I2C_INA231_addr - 7-bit device address.
//	@retval 0-OK, -1-ERROR_N
//*/
//ErrorStatus INA231_reset_aff(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){
//
//	uint16_t read_reg;
//	uint16_t write_reg;
//
//	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
//		return ERROR_N;
//	}
//	write_reg = (uint8_t)((0 << 4) | read_reg);
//
//	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}


///** @brief	Set "Conversion Ready Flag" bit. Although the INA231 can be read at any time, and the data from
//  			the last conversion are available, this bit is provided to help coordinate single-shot or triggered
//  			conversions. This bit is set after all conversions, averaging, and	multiplications are complete.
//  			This bit clears under the following conditions in single-shot mode:
//			1) Writing to the Configuration register (except for power-down or disable selections)
//			2) Reading the Mask/Enable register
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	I2C_INA231_addr - 7-bit device address.
//	@retval 0-OK, -1-ERROR_N
//*/
//ErrorStatus INA231_set_cvrf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){
//
//	uint16_t read_reg;
//	uint16_t write_reg;
//
//	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
//		return ERROR_N;
//	}
//	write_reg = (uint8_t)((1 << 3) | read_reg);
//
//	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}
//
///** @brief	Reset "Conversion Ready Flag" bit.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	I2C_INA231_addr - 7-bit device address.
//	@retval 0-OK, -1-ERROR_N
//*/
//ErrorStatus INA231_reset_cvrf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){
//
//	uint16_t read_reg;
//	uint16_t write_reg;
//
//	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
//		return ERROR_N;
//	}
//	write_reg = (uint8_t)((0 << 3) | read_reg);
//
//	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}


///** @brief	Set "Math Overflow Flag" bit. This bit is set to 1 if an arithmetic operation results in
//  			an overflow error; it indicates that current and power data may be invalid.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	I2C_INA231_addr - 7-bit device address.
//	@retval 0-OK, -1-ERROR_N
//*/
//ErrorStatus INA231_set_ovf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){
//
//	uint16_t read_reg;
//	uint16_t write_reg;
//
//	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
//		return ERROR_N;
//	}
//	write_reg = (uint8_t)((1 << 2) | read_reg);
//
//	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}
//
///** @brief	Reset "Math Overflow Flag" bit.
//	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
//	@param 	I2C_INA231_addr - 7-bit device address.
//	@retval 0-OK, -1-ERROR_N
//*/
//ErrorStatus INA231_reset_ovf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){
//
//	uint16_t read_reg;
//	uint16_t write_reg;
//
//	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
//		return ERROR_N;
//	}
//	write_reg = (uint8_t)((0 << 2) | read_reg);
//
//	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}


/** @brief	Set "Alert Polarity" bit. Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_apol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((1 << 1) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Res "Alert Polarity" bit. Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_apol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)((0 << 1) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Set "Alert Latch Enable" bit. Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_set_len(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)(1 | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Reset "Alert Latch Enable" bit. Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_reset_len(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

	uint16_t read_reg;
	uint16_t write_reg;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
		return ERROR_N;
	}
	write_reg = (uint8_t)(0 | read_reg);

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
	write_reg = (uint8_t)((1 << 15) | read_reg);

	if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}




/** @brief	Initialization of INA231. Setting calibration register for starting measuring.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	shunt - value of shunt resistor (Ohm float).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_init_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float shunt){

	uint16_t calibration = (uint16_t)(167.77216 / (max_current * shunt));

	if(INA231_write_calibr_reg(I2Cx, I2C_INA231_addr, calibration) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Initialization of INA231. Setting calibration register for starting measuring.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	shunt - value of shunt resistor (mOhm uint16_t).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_init_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t shunt){

	uint16_t calibration = (uint16_t)(167772160 / (max_current * shunt));

	if(INA231_write_calibr_reg(I2Cx, I2C_INA231_addr, calibration) != SUCCESS){
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
ErrorStatus INA231_meas_current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current){

	uint16_t current_reg;
	float lsb = max_current / 32768;

	if(INA231_read_current_reg(I2Cx, I2C_INA231_addr, &current_reg) != SUCCESS){
		return ERROR_N;
	}
	*current = (float)(lsb * current_reg);

	return SUCCESS;
}

/** @brief	Current measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	*current - pointer to store measured current value in milliAmps.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_current_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *current){

	uint16_t current_reg;
	uint16_t lsb = (max_current * 1000) / 32768;

	if(INA231_read_current_reg(I2Cx, I2C_INA231_addr, &current_reg) != SUCCESS){
		return ERROR_N;
	}
	*current = (uint16_t)(lsb * current_reg);

	return SUCCESS;
}

/** @brief	Power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	*power - pointer to store measured power value in Watts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *power){

	uint16_t power_reg;
	float lsb = (max_current * 0.025) / 32768;

	if(INA231_read_power_reg(I2Cx, I2C_INA231_addr, &power_reg) != SUCCESS){
		return ERROR_N;
	}
	*power = (float)(lsb * power_reg);

	return SUCCESS;
}

/** @brief	Power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	*power - pointer to store measured power value in milliWatts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_power_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *power){

	uint16_t power_reg;
	uint16_t lsb = (max_current * 100) / (32768 * 4);

	if(INA231_read_power_reg(I2Cx, I2C_INA231_addr, &power_reg) != SUCCESS){
		return ERROR_N;
	}
	*power = (uint16_t)(lsb * power_reg);

	return SUCCESS;
}

/** @brief	Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_bus_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *bus_voltage){

	uint16_t bus_volt_reg;
	float lsb = 0.00125;

	if(INA231_read_bus_volt_reg(I2Cx, I2C_INA231_addr, &bus_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*bus_voltage = (float)(lsb * bus_volt_reg);

	return SUCCESS;
}

/** @brief	Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in milliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_bus_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint32_t *bus_voltage){

	uint16_t bus_volt_reg;

	if(INA231_read_bus_volt_reg(I2Cx, I2C_INA231_addr, &bus_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*bus_voltage = (uint32_t)((bus_volt_reg * 10) / 8);

	return SUCCESS;
}


/** @brief	Shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_shunt_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *shunt_voltage){

	uint16_t shunt_volt_reg;
	float lsb = 0.0000025;

	if(INA231_read_shunt_volt_reg(I2Cx, I2C_INA231_addr, &shunt_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*shunt_voltage = (float)(lsb * shunt_volt_reg);

	return SUCCESS;
}

/** @brief	Shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in milliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_meas_shunt_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint32_t *shunt_voltage){

	uint16_t shunt_volt_reg;

	if(INA231_read_shunt_volt_reg(I2Cx, I2C_INA231_addr, &shunt_volt_reg) != SUCCESS){
		return ERROR_N;
	}

	*shunt_voltage = (uint32_t)(shunt_volt_reg / 400);

	return SUCCESS;
}
