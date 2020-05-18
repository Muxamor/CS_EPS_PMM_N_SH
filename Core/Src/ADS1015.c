#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "ADS1015.h"


/** @brief	Converting RAW code ADC to Volts.
	@param 	LSB_ADC_SIZE - //LSB quant size ADC with diff. FSR
			ADS1015_LSB_SIZE_FSR_6144mV = 0.003 V - with FSR  ±6.144V
			ADS1015_LSB_SIZE_FSR_4096mV = 0.002 V - with FSR  ±4.096V
			ADS1015_LSB_SIZE_FSR_2048mV = 0.001 V - with FSR  ±2.048V
			ADS1015_LSB_SIZE_FSR_1024mV = 0.000500 V - with FSR  ±1.0424V
			ADS1015_LSB_SIZE_FSR_512mV	= 0.000250 V - with FSR  ±0.512V
			ADS1015_LSB_SIZE_FSR_256mV	= 0.000125 V - with FSR  ±0.256V
	@param 	raw_adc - raw ADC code from conversion register.
	@retval Voltage value in float format.
*/
float ADS1015_raw_to_Volts(float LSB_ADC_SIZE, int16_t raw_adc){

	return ( ( (float)(raw_adc>>4) ) * LSB_ADC_SIZE);
}


/** @brief	Converting volts to ADC code(raw).
	@param	LSB_ADC_SIZE - //LSB quant size ADC with diff. FSR
			ADS1015_LSB_SIZE_FSR_6144mV = 0.003 V - with FSR  ±6.144V
			ADS1015_LSB_SIZE_FSR_4096mV = 0.002 V - with FSR  ±4.096V
			ADS1015_LSB_SIZE_FSR_2048mV = 0.001 V - with FSR  ±2.048V
			ADS1015_LSB_SIZE_FSR_1024mV = 0.000500 V - with FSR  ±1.0424V
			ADS1015_LSB_SIZE_FSR_512mV	= 0.000250 V - with FSR  ±0.512V
			ADS1015_LSB_SIZE_FSR_256mV	= 0.000125 V - with FSR  ±0.256V
	@param 	volts - voltage value that will be converted to ADC code.
	@retval ADC code in RAW format
*/
int16_t ADS1015_Volts_to_raw(float LSB_ADC_SIZE, float volts ){

	return ( (int16_t) ( ( (int16_t)( volts / LSB_ADC_SIZE ) ) << 4) );
}


/** @brief	Read ADS lsb size in float format.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*lsb - current LSB quant size ADC with diff. FSR. It will consist one of next values:
			ADS1015_LSB_SIZE_FSR_6144mV = 0.003 V - with FSR  ±6.144V
			ADS1015_LSB_SIZE_FSR_4096mV = 0.002 V - with FSR  ±4.096V
			ADS1015_LSB_SIZE_FSR_2048mV = 0.001 V - with FSR  ±2.048V
			ADS1015_LSB_SIZE_FSR_1024mV = 0.000500 V - with FSR  ±1.0424V
			ADS1015_LSB_SIZE_FSR_512mV	= 0.000250 V - with FSR  ±0.512V
			ADS1015_LSB_SIZE_FSR_256mV	= 0.000125 V - with FSR  ±0.256V
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_get_lsb(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *lsb){

	uint8_t ADC_FSR = -1;

	if(ADS1015_read_gain_FSR(I2Cx, I2C_ADS1015_addr, &ADC_FSR) != SUCCESS ){
		return ERROR_N;
	}
	switch (ADC_FSR) {
		case 0:
			*lsb = ADS1015_LSB_SIZE_FSR_6144mV;
			break;
		case 1:
			*lsb = ADS1015_LSB_SIZE_FSR_4096mV;
			break;
		case 2:
			*lsb = ADS1015_LSB_SIZE_FSR_2048mV;
			break;
		case 3:
			*lsb = ADS1015_LSB_SIZE_FSR_1024mV;
			break;
		case 4:
			*lsb = ADS1015_LSB_SIZE_FSR_512mV;
			break;
		case 5:
			*lsb = ADS1015_LSB_SIZE_FSR_256mV;
			break;
		case 6:
			*lsb = ADS1015_LSB_SIZE_FSR_256mV;
			break;
		case 7:
			*lsb = ADS1015_LSB_SIZE_FSR_256mV;
			break;
		default:
			return ERROR_N;
			break;
	}
	return SUCCESS;
}


/** @brief	Read voltage from ADC in millivolts int16 format.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*meas_voltage - pointer to store measured voltage in millivolts int16 format.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_mVolts_int16(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *meas_voltage){

	int16_t raw_adc = 0;
	float adc_meas_voltage = 0;
	float LSB_ADC_SIZE = 0;

	if(ADS1015_get_lsb(I2Cx, I2C_ADS1015_addr, &LSB_ADC_SIZE) != SUCCESS){
		return ERROR_N;
	}

	if(ADS1015_read_conv_reg(I2Cx, I2C_ADS1015_addr, &raw_adc) != SUCCESS ){
		return ERROR_N;
	}

	adc_meas_voltage = ADS1015_raw_to_Volts( LSB_ADC_SIZE, raw_adc );

	*meas_voltage = (int16_t)( adc_meas_voltage * 1000 );

	return SUCCESS;
}


/** @brief	Read voltage from ADC in float format.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*meas_voltage - pointer to store measured voltage in float format.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_Volts_float(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *meas_voltage){

	int16_t raw_adc = 0;
	float LSB_ADC_SIZE = 0;

	if(ADS1015_get_lsb(I2Cx, I2C_ADS1015_addr, &LSB_ADC_SIZE) != SUCCESS){
		return ERROR_N;
	}
	if(ADS1015_read_conv_reg(I2Cx, I2C_ADS1015_addr, &raw_adc) != SUCCESS ){
		return ERROR_N;
	}

	*meas_voltage = ADS1015_raw_to_Volts( LSB_ADC_SIZE, raw_adc);

	return SUCCESS;
}


/** @brief	Reading configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store value of configuration register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint16_t *config){

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, config) != 0){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Reading conversion register, RAW data ADC
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*read_data - pointer to variable where will be store value of conversion register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_conv_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *read_data){

	uint16_t data = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONVERSION_REG_ADDR, &data) != 0){
		return ERROR_N;
	}

	*read_data = (int16_t)data;

	return SUCCESS;
}


/** @brief	Reading low threshold register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*read_data - pointer to store value of low threshold register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_lo_thresh_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *read_data){

	uint16_t data;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_LOW_THRESH_REG_ADDR, &data) != SUCCESS){
		return ERROR_N;
	}

	*read_data = (int16_t)data;

	return SUCCESS;
}


/** @brief	Reading value in voltage low threshold register .
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*read_data - pointer to store value of low threshold register in voltage in float format.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_lo_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *read_data){

	int16_t raw_data_th_lo;
	float LSB_ADC_SIZE = 0;

	if(ADS1015_get_lsb(I2Cx, I2C_ADS1015_addr, &LSB_ADC_SIZE) != SUCCESS){
		return ERROR_N;
	}
	if( ADS1015_read_lo_thresh_reg(I2Cx, I2C_ADS1015_addr, &raw_data_th_lo) != SUCCESS ){
		return ERROR_N;
	}

	*read_data = ADS1015_raw_to_Volts( LSB_ADC_SIZE, raw_data_th_lo );

	return SUCCESS;
}


/** @brief	Reading high threshold register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*read_data - pointer to store value of high threshold register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_hi_thresh_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *read_data){

	uint16_t data;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_HIGH_THRESH_REG_ADDR, &data) != SUCCESS){
		return ERROR_N;
	}

	*read_data = (int16_t)data;

	return SUCCESS;
}


/** @brief	Reading value in voltage high threshold register .
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*read_data - pointer to store value of high threshold register in voltage in float format.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_hi_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *read_data){

	int16_t raw_data_th_hi;
	float LSB_ADC_SIZE = 0;

	if(ADS1015_get_lsb(I2Cx, I2C_ADS1015_addr, &LSB_ADC_SIZE) != SUCCESS){
		return ERROR_N;
	}
	if( ADS1015_read_hi_thresh_reg(I2Cx, I2C_ADS1015_addr, &raw_data_th_hi) != SUCCESS ){
		return ERROR_N;
	}

	*read_data = ADS1015_raw_to_Volts( LSB_ADC_SIZE, raw_data_th_hi );

	return SUCCESS;
}



/** @brief	Reading conversion status bit. This bit determines the operational status of the device. Operation status can only be written
			when in power-down state and has no effect when a conversion is ongoing.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*status - pointer to variable where will be store status bit of configuration register.
			0 : Device is currently performing a conversion
			1 : Device is not currently performing a conversion
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_conv_status(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *status){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*status = (uint8_t)((read_reg & 0x8000) >> 15);

	return SUCCESS;
}


/** @brief	Reading multiplexer configuration. These bits configure the input multiplexer. These bits serve no function on the
			ADS1013 and ADS1014.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store multiplexer configuration bits of configuration register.
			0 : AINP = AIN0 and AINN = AIN1 (default)
			1 : AINP = AIN0 and AINN = AIN3
			2 : AINP = AIN1 and AINN = AIN3
			3 : AINP = AIN2 and AINN = AIN3
			4 : AINP = AIN0 and AINN = GND
			5 : AINP = AIN1 and AINN = GND
			6 : AINP = AIN2 and AINN = GND
			7 : AINP = AIN3 and AINN = GND
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_mux(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x7000) >> 12);

	return SUCCESS;
}


/** @brief	Reading gain amplifier configuration. These bits set the FSR (Full-Scale Range) of the programmable gain amplifier.
			These bits serve no function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store gain amplifier bits of configuration register.
			0 : range voltage = ±6.144 V
			1 : range voltage = ±4.096 V
			2 : range voltage = ±2.048 V (default)
			3 : range voltage = ±1.024 V
			4 : range voltage = ±0.512 V
			5 : range voltage = ±0.256 V
			6 : range voltage = ±0.256 V
			7 : range voltage = ±0.256 V
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_gain_FSR(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x0E00) >> 9);

	return SUCCESS;
}


/** @brief	Reading device operating mode status. This bit controls the operating mode.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store operation mode bit of configuration register.
			0 : Continuous-conversion mode
			1 : Single-shot mode or power-down state (default)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_conv_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x0100) >> 8);

	return SUCCESS;
}


/** @brief	Reading data rate configuration. These bits control the data rate setting.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store data rate bits of configuration register.
			0 : 128 SPS
			1 : 250 SPS
			2 : 490 SPS
			3 : 920 SPS
			4 : 1600 SPS (default)
			5 : 2400 SPS
			6 : 3300 SPS
			7 : 3300 SPS
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_conv_data_rate(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;


	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x00E0) >> 5);

	return SUCCESS;
}


/** @brief	Reading comparator mode status. This bit configures the comparator operating mode. This bit serves no function on
			the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store comparator mode bit of configuration register.
			0 : Traditional comparator (default)
			1 : Window comparator
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_comp_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x0010) >> 4);

	return SUCCESS;
}


/** @brief	Reading comparator polarity status. This bit controls the polarity of the ALERT/RDY pin. This bit serves no
			function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store comparator polarity bit of configuration register.
			0 : Active low (default)
			1 : Active high
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_comp_pol(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x0008) >> 3);

	return SUCCESS;
}


/** @brief	Reading latching comparator status. This bit controls whether the ALERT/RDY pin latches after being asserted or
			clears after conversions are within the margin of the upper and lower threshold	values. This bit serves no function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store latching comparator bit of configuration register.
			0 : Nonlatching comparator . The ALERT/RDY pin does not latch when asserted(default).
			1 : Latching comparator. The asserted ALERT/RDY pin remains latched until
			conversion data are read by the master or an appropriate SMBus alert response
			is sent by the master. The device responds with its address, and it is the lowest
			address currently asserting the ALERT/RDY bus line
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_latching_comp(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)((read_reg & 0x04) >> 2);

	return SUCCESS;
}


/** @brief	Reading comparator queue and disable configuration. These bits perform two functions.
  			When set to 11, the comparator is disabled and
			the ALERT/RDY pin is set to a high-impedance state. When set to any other
			value, the ALERT/RDY pin and the comparator function are enabled, and the set
			value determines the number of successive conversions exceeding the upper or
			lower threshold required before asserting the ALERT/RDY pin. These bits serve
			no function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	*config - pointer to variable where will be store comparator queue bits of configuration register.
			0 : Assert after one conversion
			1 : Assert after two conversions
			2 : Assert after four conversions
			3 : Disable comparator and set ALERT/RDY pin to high-impedance (default)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_read_comp_queue(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	*config = (uint8_t)(read_reg & 0x03);

	return SUCCESS;
}


/** @brief	Setting default values for all registers.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_default_values(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr){

	if(ADS1015_write_config_reg(I2Cx, I2C_ADS1015_addr, 0x8583) != 0){
		return ERROR_N;
	}
	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_LOW_THRESH_REG_ADDR, 0x8000) != SUCCESS ){
		return ERROR_N;
	}
	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_HIGH_THRESH_REG_ADDR, 0x7FF0) != SUCCESS ){
		return ERROR_N;
	}
	return SUCCESS;
}


/** @brief	Write all configuration register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	config - variable that will be write to configuration register.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_write_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint16_t config){

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, config) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Starting single conversion (when in power-down state).
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_start_single_conv(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)(read_reg | (1 << 15));

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing multiplexer configuration. These bits configure the input multiplexer.
    		These bits serve no function on the ADS1013 and ADS1014.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	config - variable that will be write to configuration register.
			0 - ADS1015_AINp0_AINn1 : AINP = AIN0 and AINN = AIN1 (default)
			1 - ADS1015_AINp0_AINn3 : AINP = AIN0 and AINN = AIN3
			2 - ADS1015_AINp1_AINn3 : AINP = AIN1 and AINN = AIN3
			3 - ADS1015_AINp2_AINn3 : AINP = AIN2 and AINN = AIN3
			4 - ADS1015_AINp0_AINnGND : AINP = AIN0 and AINN = GND
			5 - ADS1015_AINp1_AINnGND : AINP = AIN1 and AINN = GND
			6 - ADS1015_AINp2_AINnGND : AINP = AIN2 and AINN = GND
			7 - ADS1015_AINp3_AINnGND : AINP = AIN3 and AINN = GND
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_mux(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(7 << 12))) | (config << 12));

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing gain amplifier configuration. These bits set the Full-Scale Range (FSR) of the programmable gain amplifier.
  	  	  	These bits serve no function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	config - variable that will be write to configuration register.
			0 - ADS1015_FSR_6144mV : range voltage = ±6.144 V
			1 - ADS1015_FSR_4096mV : range voltage = ±4.096 V
			2 - ADS1015_FSR_2048mV : range voltage = ±2.048 V (default)
			3 - ADS1015_FSR_1024mV : range voltage = ±1.024 V
			4 - ADS1015_FSR_512mV  : range voltage = ±0.512 V
			5 - ADS1015_FSR_256mV  : range voltage = ±0.256 V
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_gain_FSR(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t config){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(7 << 9))) | (config << 9));

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing operation mode configuration. This bit controls the operating mode
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	mode - variable that will be write to configuration register.
			0 - ADS1015_CONT_CONV_MODE : Continuous-conversion mode
			1 - ADS1015_SINGLE_SHOT_MODE : Single-shot mode or power-down state (default)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_conv_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(1 << 8))) | (mode << 8));

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing data rate configuration. These bits control the data rate setting
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	rate - variable that will be write to configuration register.
			0 - ADS1015_128_SPS  : 128 SPS
			1 - ADS1015_250_SPS  : 250 SPS
			2 - ADS1015_490_SPS  : 490 SPS
			3 - ADS1015_920_SPS  : 920 SPS
			4 - ADS1015_1600_SPS : 1600 SPS (default)
			5 - ADS1015_2400_SPS : 2400 SPS
			6 - ADS1015_3300_SPS : 3300 SPS
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_conv_data_rate(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t rate){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(7 << 5))) | rate);

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing comparator mode configuration. This bit configures the comparator operating mode.
  	  	  	This bit serves no function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	mode - variable that will be write to configuration register.
			0 - ADS1015_TRADITIONAL_COMP: Traditional comparator (default)
			1 - ADS1015_WINDOW_COMP : Window comparator
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_comp_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(1 << 4))) | mode);

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing comparator polarity configuration.This bit controls the polarity of the ALERT/RDY pin.
  			This bit serves no function on the ADS1013
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	polarity - variable that will be write to configuration register.
			0 - ADS1015_ACTIVE_LOW_POL : Active low (default)
			1 - ADS1015_ACTIVE_HIGH_POL : Active high
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_comp_pol(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t polarity){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(1 << 3))) | polarity);

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing latching comparator configuration. This bit controls whether the ALERT/RDY pin latches after being asserted or
			clears after conversions are within the margin of the upper and lower threshold values. This bit serves no function on the ADS1013
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	mode - variable that will be write to configuration register.
			0 - ADS1015_NONLATCHING_COMP : Nonlatching comparator . The ALERT/RDY pin does not latch when asserted
			(default).
			1 - ADS1015_LATCHING_COMP : Latching comparator. The asserted ALERT/RDY pin remains latched until
			conversion data are read by the master or an appropriate SMBus alert response
			is sent by the master. The device responds with its address, and it is the lowest
			address currently asserting the ALERT/RDY bus line
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_latching_comp(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(1 << 2))) | mode);

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing comparator queue configuration. These bits perform two functions.
  			When set to 11, the comparator is disabled and
			the ALERT/RDY pin is set to a high-impedance state. When set to any other
			value, the ALERT/RDY pin and the comparator function are enabled, and the set
			value determines the number of successive conversions exceeding the upper or
			lower threshold required before asserting the ALERT/RDY pin. These bits serve
			no function on the ADS1013.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	mode - variable that will be write to configuration register.
			0 - ADS1015_1_CONVERSION : Assert after one conversion
			1 - ADS1015_2_CONVERSION : Assert after two conversions
			2 - ADS1015_3_CONVERSION : Assert after four conversions
			3 - ADS1015_DISABLE_COMPARATOR : Disable comparator and set ALERT/RDY pin to high-impedance (default)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_comp_queue(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode){

	uint16_t read_reg = 0;

	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, &read_reg) != SUCCESS ){
		return ERROR_N;
	}
	read_reg = (uint16_t)((read_reg & (~(3 << 0))) | mode);

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_CONFIG_REG_ADDR, read_reg) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing value in voltage to low threshold configuration.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	th_val_volts - Low threshold value in Volts
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_lo_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float th_val_volts){

	int16_t raw_data;
	float LSB_ADC_SIZE = 0;

	if(ADS1015_get_lsb(I2Cx, I2C_ADS1015_addr, &LSB_ADC_SIZE) != SUCCESS ){
		return ERROR_N;
	}
	raw_data = ADS1015_Volts_to_raw( LSB_ADC_SIZE, th_val_volts );

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_LOW_THRESH_REG_ADDR, (uint16_t)raw_data ) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Writing value  in voltage to high threshold configuration.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_ADS1015_addr - 7-bit device address.
	@param 	th_val_volts - High threshold value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus ADS1015_setup_hi_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float th_val_volts){

	int16_t raw_data;
	float LSB_ADC_SIZE = 0;

	if(ADS1015_get_lsb(I2Cx, I2C_ADS1015_addr, &LSB_ADC_SIZE) != SUCCESS ){
		return ERROR_N;
	}
	raw_data = ADS1015_Volts_to_raw( LSB_ADC_SIZE, th_val_volts );

	if(I2C_Write_word_u16_St(I2Cx, I2C_ADS1015_addr, I2C_SIZE_REG_ADDR_U8, (uint32_t)ADS1015_HIGH_THRESH_REG_ADDR, (uint16_t)raw_data ) != SUCCESS ){
		return ERROR_N;
	}

	return SUCCESS;
}
