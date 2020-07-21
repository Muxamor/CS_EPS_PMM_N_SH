#ifndef INC_ADS1015_H_
#define INC_ADS1015_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
typedef enum {
  ERROR_N = -1,
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

#define ADS1015_CONVERSION_REG_ADDR				0x00
#define ADS1015_CONFIG_REG_ADDR			 	 	0x01
#define ADS1015_LOW_THRESH_REG_ADDR				0x02
#define ADS1015_HIGH_THRESH_REG_ADDR			0x03

#define ADS1015_SINGLE_CONVERSION				0x01

#define ADS1015_AINp0_AINn1						0x00
#define ADS1015_AINp0_AINn3						0x01
#define ADS1015_AINp1_AINn3						0x02
#define ADS1015_AINp2_AINn3						0x03
#define ADS1015_AINp0_AINnGND					0x04
#define ADS1015_AINp1_AINnGND					0x05
#define ADS1015_AINp2_AINnGND					0x06
#define ADS1015_AINp3_AINnGND					0x07

#define ADS1015_FSR_6144mV						0x00 //FSR  ±6.144V
#define ADS1015_FSR_4096mV						0x01 //FSR  ±4.096V
#define ADS1015_FSR_2048mV						0x02 //FSR  ±2.048V
#define ADS1015_FSR_1024mV						0x03 //FSR  ±1.0424V
#define ADS1015_FSR_512mV						0x04 //FSR  ±0.512V
#define ADS1015_FSR_256mV						0x05 //FSR  ±0.256V
//#define ADS1015_FSR_256mV						0x06
//#define ADS1015_FSR_256mV						0x07


#define ADS1015_LSB_SIZE_FSR_6144mV			    0.003 		// 3mV LSB size ADC with FSR  ±6.144V
#define ADS1015_LSB_SIZE_FSR_4096mV				0.002 		// 2mV LSB size ADC with FSR  ±4.096V
#define ADS1015_LSB_SIZE_FSR_2048mV				0.001 		// 1mV LSB size ADC with FSR  ±2.048V
#define ADS1015_LSB_SIZE_FSR_1024mV				0.000500 	// 0.5mV LSB size ADC with FSR  ±1.0424V
#define ADS1015_LSB_SIZE_FSR_512mV				0.000250 	// 0.250 mV LSB size ADC with FSR  ±0.512V
#define ADS1015_LSB_SIZE_FSR_256mV				0.000125 	// 0.125 mV LSB size ADC with FSR  ±0.256V


#define ADS1015_CONT_CONV_MODE					0x00
#define ADS1015_SINGLE_SHOT_MODE				0x01

#define ADS1015_128_SPS							0x00
#define ADS1015_250_SPS							0x20
#define ADS1015_490_SPS							0x40
#define ADS1015_920_SPS							0x60
#define ADS1015_1600_SPS						0x80
#define ADS1015_2400_SPS						0xA0
#define ADS1015_3300_SPS						0xC0

#define ADS1015_TRADITIONAL_COMP				0x00
#define ADS1015_WINDOW_COMP						0x10

#define ADS1015_ACTIVE_LOW_POL					0x00
#define ADS1015_ACTIVE_HIGH_POL					0x08

#define ADS1015_NONLATCHING_COMP				0x00
#define ADS1015_LATCHING_COMP					0x04

#define ADS1015_1_CONVERSION					0x00
#define ADS1015_2_CONVERSION					0x01
#define ADS1015_3_CONVERSION					0x02
#define ADS1015_DISABLE_COMPARATOR				0x03


float ADS1015_raw_to_Volts(float LSB_ADC_SIZE, int16_t raw_adc);
int16_t ADS1015_Volts_to_raw(float LSB_ADC_SIZE, float volts );

ErrorStatus ADS1015_get_lsb(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *lsb);

ErrorStatus ADS1015_read_mVolts_int16(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *meas_voltage);
ErrorStatus ADS1015_read_Volts_float(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *meas_voltage);

ErrorStatus ADS1015_read_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint16_t *config);
ErrorStatus ADS1015_read_conv_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *data);
ErrorStatus ADS1015_read_lo_thresh_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *read_data);
ErrorStatus ADS1015_read_lo_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *read_data);
ErrorStatus ADS1015_read_hi_thresh_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, int16_t *read_data);
ErrorStatus ADS1015_read_hi_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float *read_data);

ErrorStatus ADS1015_read_conv_status(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *status);
ErrorStatus ADS1015_read_mux(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_gain_FSR(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_conv_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_conv_data_rate(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_comp_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_comp_pol(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_latching_comp(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);
ErrorStatus ADS1015_read_comp_queue(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t *config);

ErrorStatus ADS1015_setup_default_values(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr);
ErrorStatus ADS1015_write_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint16_t config);
ErrorStatus ADS1015_start_single_conv(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr);
ErrorStatus ADS1015_setup_mux(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t config);
ErrorStatus ADS1015_setup_gain_FSR(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t config);
ErrorStatus ADS1015_setup_conv_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode);
ErrorStatus ADS1015_setup_conv_data_rate(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t rate);
ErrorStatus ADS1015_setup_comp_mode(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode);
ErrorStatus ADS1015_setup_comp_pol(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t polarity);
ErrorStatus ADS1015_setup_latching_comp(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode);
ErrorStatus ADS1015_setup_comp_queue(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t mode);
ErrorStatus ADS1015_setup_lo_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float th_val_volts);
ErrorStatus ADS1015_setup_hi_thresh_val(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, float th_val_volts);

#ifdef __cplusplus
}
#endif

#endif /* INC_ADS1015_H_ */
