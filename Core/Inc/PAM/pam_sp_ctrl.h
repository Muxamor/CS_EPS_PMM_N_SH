#ifndef CS_EPS_PMM_N_SH_V_PAM_SP_CTRL_H
#define CS_EPS_PMM_N_SH_V_PAM_SP_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PAM_SP_Get_State_ID(_PAM *pam_ptr, uint8_t number_sp);
ErrorStatus PAM_SP_Read_GPIO_State_ID( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t gpio_ext_addr,  uint8_t gpio_pin_number, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch, uint8_t* ID_value );
ErrorStatus PAM_SP_Get_Temperature(_PAM *pam_ptr, uint8_t number_sp);
ErrorStatus PAM_SP_Read_Temp_Sensor( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch, int8_t* temp_value );

#ifdef __cplusplus
}
#endif


#endif //CS_EPS_PMM_N_SH_V_PAM_SP_CTRL_H
