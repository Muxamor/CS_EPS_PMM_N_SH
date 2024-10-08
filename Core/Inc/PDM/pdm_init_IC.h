#ifndef INC_PDM_PDM_INIT_IC_H_
#define INC_PDM_PDM_INIT_IC_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PDM_init_TMP1075( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch );
ErrorStatus PDM_Power_Down_TMP1075( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch );
ErrorStatus PDM_init_PWR_Mon_INA238( _PDM *pdm_ptr, uint8_t number_pwr_channel);
ErrorStatus PDM_PWR_Down_PWR_Mon_INA238( _PDM *pdm_ptr, uint8_t num_pwr_ch);

#ifdef __cplusplus
}
#endif

#endif /* INC_PDM_PDM_INIT_IC_H_ */
