/*
 * INA231.h
 *
 *  Created on: 13 мая 2020 г.
 *      Author: ftor
 */

#ifndef INC_INA231_H_
#define INC_INA231_H_

#include "main.h"

#define INA231_CONFIGURATION_REG_ADDR			0x00
#define INA231_SHUNT_VOLTAGE_REG_ADDR			0x01
#define INA231_BUS_VOLTAGE_REG_ADDR				0x02
#define INA231_POWER_REG_ADDR					0x03
#define INA231_CURRENT_REG_ADDR					0x04
#define INA231_CALIBRATION_REG_ADDR				0x05
#define INA231_MASK_REG_ADDR					0x06
#define INA231_ALERT_LIMIT_REG_ADDR				0x07

#define INA231_AVERAGING_NUM_1					0x00
#define INA231_AVERAGING_NUM_4					0x00
#define INA231_AVERAGING_NUM_16					0x00
#define INA231_AVERAGING_NUM_64					0x00
#define INA231_AVERAGING_NUM_128				0x00
#define INA231_AVERAGING_NUM_256				0x00
#define INA231_AVERAGING_NUM_512				0x00
#define INA231_AVERAGING_NUM_1024				0x00

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
ErrorStatus INA231_read_calibr_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_mask_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_read_alert_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);


ErrorStatus INA231_read_aver_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_bus_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_shunt_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);


ErrorStatus INA231_read_sol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_sul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_bol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_bul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_cnvr(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_aff(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_cvrf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_ovf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_apol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);
ErrorStatus INA231_read_len(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *read_data);


ErrorStatus INA231_write_config_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);
ErrorStatus INA231_write_calibr_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);
ErrorStatus INA231_write_mask_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);
ErrorStatus INA231_write_alert_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t write_data);


ErrorStatus INA231_write_aver_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);
ErrorStatus INA231_write_bus_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);
ErrorStatus INA231_write_shunt_ct(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);
ErrorStatus INA231_write_mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t write_data);


ErrorStatus INA231_set_sol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_sol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_sul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_sul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_bol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_bol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_bul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_bul(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_pol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_cnvr(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_cnvr(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
//ErrorStatus INA231_set_aff(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
//ErrorStatus INA231_reset_aff(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
//ErrorStatus INA231_set_cvrf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
//ErrorStatus INA231_reset_cvrf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
//ErrorStatus INA231_set_ovf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
//ErrorStatus INA231_reset_ovf(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_apol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_apol(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_set_len(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_reset_len(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);

ErrorStatus INA231_power_reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);

ErrorStatus INA231_init_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float shunt);
ErrorStatus INA231_init_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t shunt);
ErrorStatus INA231_meas_current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current);
ErrorStatus INA231_meas_current_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *current);
ErrorStatus INA231_meas_power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *power);
ErrorStatus INA231_meas_power_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *power);
ErrorStatus INA231_meas_bus_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *bus_voltage);
ErrorStatus INA231_meas_bus_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *bus_voltage);
ErrorStatus INA231_meas_shunt_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *shunt_voltage);
ErrorStatus INA231_meas_shunt_int(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *shunt_voltage);

#endif /* INC_INA231_H_ */
