

#ifndef INC_PDM_PDM_INIT_IC_H_
#define INC_PDM_PDM_INIT_IC_H_


ErrorStatus PDM_init_TMP1075( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch );
ErrorStatus PDM_init_INA231( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t INA231_addr, uint16_t max_current, uint16_t Rshunt, uint8_t i2c_mux_addr, uint8_t mux_ch );




#endif /* INC_PDM_PDM_INIT_IC_H_ */
