

#ifndef INC_INA231_H_
#define INC_INA231_H_

#ifdef __cplusplus
extern "C" {
#endif

#define INA231_CONFIGURATION_REG_ADDR			0x00
#define INA231_SHUNT_VOLTAGE_REG_ADDR			0x01
#define INA231_BUS_VOLTAGE_REG_ADDR				0x02
#define INA231_POWER_REG_ADDR					0x03
#define INA231_CURRENT_REG_ADDR					0x04
#define INA231_CALIBRATION_REG_ADDR				0x05
#define INA231_MASK_REG_ADDR					0x06
#define INA231_ALERT_LIMIT_REG_ADDR				0x07

#define INA231_AVERAGING_NUM_1					0x00
#define INA231_AVERAGING_NUM_4					0x01
#define INA231_AVERAGING_NUM_16					0x02
#define INA231_AVERAGING_NUM_64					0x03
#define INA231_AVERAGING_NUM_128				0x04
#define INA231_AVERAGING_NUM_256				0x05
#define INA231_AVERAGING_NUM_512				0x06
#define INA231_AVERAGING_NUM_1024				0x07

#define INA231_CONVERSION_TIME_140us			0x00
#define INA231_CONVERSION_TIME_204us			0x01
#define INA231_CONVERSION_TIME_332us			0x02
#define INA231_CONVERSION_TIME_588us			0x03
#define INA231_CONVERSION_TIME_1100us			0x04
#define INA231_CONVERSION_TIME_2116us			0x05
#define INA231_CONVERSION_TIME_4156us			0x06
#define INA231_CONVERSION_TIME_8244us			0x07


//#define INA231_POWER_DOWN						0x00
#define INA231_SHUNT_VOLTAGE_TRIGGERED			0x01
#define INA231_BUS_VOLTAGE_TRIGGERED			0x02
#define INA231_SHUNT_AND_BUS_TRIGGERED			0x03
#define INA231_POWER_DOWN						0x04
#define INA231_SHUNT_VOLTAGE_CONTINUOUS			0x05
#define INA231_BUS_VOLTAGE_CONTINUOUS			0x06
#define INA231_SHUNT_AND_BUS_CONTINUOUS			0x07



ErrorStatus INA231_read_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_shunt_volt_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_bus_volt_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_power_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_current_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_calibration_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_mask_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_alert_lim_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);


ErrorStatus INA231_read_aver_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_bus_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_shunt_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);


ErrorStatus INA231_read_shunt_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_shunt_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_bus_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_bus_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_power_overlimit(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_conv_ready_alert(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_alert_function(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_conv_ready(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_math_overflow(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_alert_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_alert_latch(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);


ErrorStatus INA231_write_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);
ErrorStatus INA231_write_calibration_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);
ErrorStatus INA231_write_mask_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);
ErrorStatus INA231_write_alert_lim_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);


ErrorStatus INA231_write_aver_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);
ErrorStatus INA231_write_bus_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);
ErrorStatus INA231_write_shunt_conv_time(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);
ErrorStatus INA231_write_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);


ErrorStatus INA231_set_shunt_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_shunt_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_shunt_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_shunt_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_bus_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_bus_overvoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_bus_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_bus_undervoltage(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_power_overlimit(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_power_overlimit(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_conv_ready_alert(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_conv_ready_alert(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_alert_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_alert_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_alert_latch(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_alert_latch(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);

ErrorStatus INA231_power_reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);

ErrorStatus INA231_Set_calibration_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float Rshunt);
ErrorStatus INA231_Set_calibration_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t Rshunt);

ErrorStatus INA231_Get_Current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current);
ErrorStatus INA231_Get_Current_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current);
ErrorStatus INA231_Get_Power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *power);
ErrorStatus INA231_Get_Power_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *power);
ErrorStatus INA231_Get_bus_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *bus_voltage);
ErrorStatus INA231_Get_bus_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *bus_voltage);
ErrorStatus INA231_Get_shunt_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *shunt_voltage);
ErrorStatus INA231_Get_shunt_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, int16_t *shunt_voltage);
ErrorStatus INA231_Get_I_V_P_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current, float *bus_voltage, float *power);
ErrorStatus INA231_Get_I_V_P_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current, uint16_t *bus_voltage, uint16_t *power);

#ifdef __cplusplus
}
#endif

#endif /* INC_INA231_H_ */
