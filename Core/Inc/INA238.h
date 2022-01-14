#ifndef CS_EPS_PMM_N_SH_V_INA238_H
#define CS_EPS_PMM_N_SH_V_INA238_H

#ifdef __cplusplus
extern "C" {
#endif

#define INA238_CONFIG_REG_ADDR			0x00
#define INA238_ADC_CONFIG_REG_ADDR	    0x01
#define INA238_SHUNT_CAL_REG_ADDR		0x02
#define INA238_VSHUNT_REG_ADDR  		0x04
#define INA238_VBUS_REG_ADDR			0x05
#define INA238_DIETEMP_REG_ADDR			0x06
#define INA238_CURRENT_REG_ADDR			0x07
#define INA238_POWER_REG_ADDR			0x08
#define INA238_DIAG_ALRT_REG_ADDR		0x0B
#define INA238_SOVL_REG_ADDR		    0x0C
#define INA238_SUVL_REG_ADDR		    0x0D
#define INA238_BOVL_REG_ADDR		    0x0E
#define INA238_BUVL_REG_ADDR		    0x0F
#define INA238_TEMP_LIMIT_REG_ADDR	    0x10
#define INA238_PWR_LIMIT_REG_ADDR		0x11
#define INA238_MANUFACTURER_ID_REG_ADDR 0x3E
#define INA238_DEVICE_ID_REG_ADDR		0x3F

#define INA238_AVERAGING_COUNT_1		0x00
#define INA238_AVERAGING_COUNT_4		0x01
#define INA238_AVERAGING_COUNT_16		0x02
#define INA238_AVERAGING_COUNT_64		0x03
#define INA238_AVERAGING_COUNT_128		0x04
#define INA238_AVERAGING_COUNT_256		0x05
#define INA238_AVERAGING_COUNT_512		0x06
#define INA238_AVERAGING_COUNT_1024		0x07

#define INA238_CONVERSION_TIME_50us		0x00
#define INA238_CONVERSION_TIME_84us		0x01
#define INA238_CONVERSION_TIME_150us	0x02
#define INA238_CONVERSION_TIME_280us	0x03
#define INA238_CONVERSION_TIME_540us	0x04
#define INA238_CONVERSION_TIME_1052us	0x05
#define INA238_CONVERSION_TIME_2074us	0x06
#define INA238_CONVERSION_TIME_4120us	0x07

//#define INA238_SHUTDOWN						 0x00
#define INA238_BUS_VOLTAGE_TRIGGERED	    	 0x01

#define INA238_SHUNT_VOLTAGE_TRIGGERED			 0x02
#define INA238_SHUNT_AND_BUS_TRIGGERED			 0x03
#define INA238_TEMPERATURE_TRIGGERED			 0x04
#define INA238_TEMPERATURE_AND_BUS_TRIGGERED	 0x05
#define INA238_TEMPERATURE_AND_SHUNT_TRIGGERED	 0x06
#define INA238_BUS_SHUNT_TEMPERATURE_TRIGGERED	 0x07
#define INA238_SHUTDOWN						     0x08
#define INA238_BUS_VOLTAGE_CONTINUOUS			 0x09
#define INA238_SHUNT_VOLTAGE_CONTINUOUS		     0x0A
#define INA238_SHUNT_AND_BUS_CONTINUOUS			 0x0B
#define INA238_TEMPERATURE_CONTINUOUS			 0x0C
#define INA238_BUS_AND_TEMPERATURE_CONTINUOUS	 0x0D
#define INA238_TEMPERATURE_AND_SHUNT_CONTINUOUS	 0x0E
#define INA238_BUS_SHUNT_TEMPERATURE_CONTINUOUS	 0x0F

#define INA238_ADC_RANGE_163mv	 0x00
#define INA238_ADC_RANGE_40mv	 0x01

ErrorStatus INA238_Read_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t INA238_addr_reg, uint16_t *read_data);
ErrorStatus INA238_Write_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t INA238_addr_reg, uint16_t write_data);

//************** Configuration (CONFIG) Register (Address = 0h)
ErrorStatus INA238_Hard_Reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr);
ErrorStatus INA238_Setup_CONVDLY(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t conv_delay);
ErrorStatus INA238_Get_CONVDLY(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *conv_delay);
ErrorStatus INA238_Setup_ADCRANGE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t adc_range);
ErrorStatus INA238_Get_ADCRANGE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *adc_range);

//************** ADC Configuration (ADC_CONFIG) Register (Address = 1h)
ErrorStatus INA238_Setup_MODE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t mode);
ErrorStatus INA238_Get_MODE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *mode);
ErrorStatus INA238_Setup_VBUSCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t vbusct);
ErrorStatus INA238_Get_VBUSCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *vbusct);
ErrorStatus INA238_Setup_VSHCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t vshct);
ErrorStatus INA238_Get_VSHCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *vshct);
ErrorStatus INA238_Setup_VTCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t vtct);
ErrorStatus INA238_Get_VTCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *vtct);
ErrorStatus INA238_Setup_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t avg);
ErrorStatus INA238_Get_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *avg);

//************** Shunt Calibration (SHUNT_CAL) Register (Address = 2h)
ErrorStatus INA238_Setup_Calibration_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float Rshunt);
ErrorStatus INA238_Setup_Calibration_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t max_exp_current_mA, uint16_t Rshunt);

//************** Shunt Voltage Measurement (VSHUNT) Register (Address = 4h) [reset = 0h]
ErrorStatus INA238_Get_Shunt_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *shunt_voltage);
ErrorStatus INA238_Get_Shunt_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, int16_t *shunt_voltage);

//**************  Bus Voltage Measurement (VBUS) Register (Address = 5h)
ErrorStatus INA238_Get_Bus_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *bus_voltage);
ErrorStatus INA238_Get_Bus_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t *bus_voltage);

//************** Temperature Measurement (DIETEMP) Register (Address = 6h)
ErrorStatus INA238_Get_Temperature_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *temperature);

//************** Current Result (CURRENT) Register (Address = 7h)
ErrorStatus INA238_Get_Current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *current);
ErrorStatus INA238_Get_Current_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, int16_t max_exp_current_mA, int16_t *current);

//************** Power Result (POWER) Register (Address = 8h) [reset = 0h]
ErrorStatus INA238_Get_Power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *power);
ErrorStatus INA238_Get_Power_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, int16_t max_exp_current_mA, int16_t *power);

//************** Diagnostic Flags and Alert (DIAG_ALRT) Register (Address = Bh)
ErrorStatus INA238_Set_ALATCH(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr);
ErrorStatus INA238_Reset_ALATCH(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA238_Set_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr);
ErrorStatus INA238_Reset_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA238_Set_SLOWALERT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr);
ErrorStatus INA238_Reset_SLOWALERT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA238_Set_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr);
ErrorStatus INA238_Reset_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr);
ErrorStatus INA238_Get_ALERT_Flags(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t *alert_flags );

//************** Shunt Overvoltage Threshold (SOVL) Register (Address = Ch)
ErrorStatus INA238_Setup_Shunt_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float shunt_ovl);
ErrorStatus INA238_Get_Shunt_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *shunt_ovl);

//**************  Shunt Undervoltage Threshold (SUVL) Register (Address = Dh)
ErrorStatus INA238_Setup_Shunt_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float shunt_uvl);
ErrorStatus INA238_Get_Shunt_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *shunt_uvl);

//************** Bus Overvoltage Threshold (BOVL) Register (Address = Eh)
ErrorStatus INA238_Setup_Bus_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float bus_ovl);
ErrorStatus INA238_Get_Bus_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *bus_ovl);

//**************  Bus Undervoltage Threshold (BUVL) Register (Address = Fh)
ErrorStatus INA238_Setup_Bus_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float bus_uvl);
ErrorStatus INA238_Get_Bus_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *bus_uvl);

//************** Temperature Over-Limit Threshold (TEMP_LIMIT) Register
ErrorStatus INA238_Setup_TOL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float tol);
ErrorStatus INA238_Get_TOL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *tol);

//************** Power Over-Limit Threshold (PWR_LIMIT) Register (Address = 11h) )
ErrorStatus INA238_Setup_POL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float tol);
ErrorStatus INA238_Get_POL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *pol);


ErrorStatus INA238_Get_I_V_P_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *current, float *bus_voltage, float *power );
ErrorStatus INA238_Get_I_V_P_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t max_exp_current_mA, int16_t *current, uint16_t *bus_voltage, uint16_t *power);


#ifdef __cplusplus
}
#endif















#endif //CS_EPS_PMM_N_SH_V_INA238_H
