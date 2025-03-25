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

ErrorStatus INA231_Read_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t INA231_addr_reg, uint16_t *read_data);
ErrorStatus INA231_Write_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t INA231_addr_reg, uint16_t write_data);

ErrorStatus INA231_Power_Reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Setup_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t avg_mode);
ErrorStatus INA231_Get_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *avg_mode);
ErrorStatus INA231_Setup_VbusCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t conv_time);
ErrorStatus INA231_Get_VbusCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *conv_time);
ErrorStatus INA231_Setup_VshCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t conv_time);
ErrorStatus INA231_Get_VshCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *conv_time);
ErrorStatus INA231_Setup_Mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t mode);
ErrorStatus INA231_Get_Mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *mode);

ErrorStatus INA231_Set_SOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_SOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Set_SUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_SUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Set_BOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_BOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Set_BUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_BUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Set_POL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_POL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Set_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Get_Alert_Config(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data);
ErrorStatus INA231_Set_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Set_LEN(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA231_Reset_LEN(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);

ErrorStatus INA231_Set_Calibration_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float Rshunt);
ErrorStatus INA231_Set_Calibration_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t Rshunt);

ErrorStatus INA231_Get_Current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current);
ErrorStatus INA231_Get_Current_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current);
ErrorStatus INA231_Get_Power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *power);
ErrorStatus INA231_Get_Power_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *power);
ErrorStatus INA231_Get_BusV_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *bus_voltage);
ErrorStatus INA231_Get_BusV_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *bus_voltage);
ErrorStatus INA231_Get_ShuntV_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *shunt_voltage);
ErrorStatus INA231_Get_ShuntV_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, int16_t *shunt_voltage);
ErrorStatus INA231_Get_I_V_P_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current, float *bus_voltage, float *power);
ErrorStatus INA231_Get_I_V_P_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current, uint16_t *bus_voltage, uint16_t *power);

#ifdef __cplusplus
}
#endif

#endif /* INC_INA231_H_ */
