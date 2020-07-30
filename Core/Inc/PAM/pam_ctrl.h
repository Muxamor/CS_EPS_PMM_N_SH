
#ifndef INC_PAM_PAM_CTRL_H_
#define INC_PAM_PAM_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PAM_Set_state_PWR_Supply( _PAM *pam_ptr, uint8_t pwr_source_num, uint8_t state_channel);
ErrorStatus PAM_Get_PG_PWR_Supply( _PAM *pam_ptr, uint8_t pwr_source_num);
ErrorStatus PAM_Get_Temperature( _PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch);
ErrorStatus PAM_Get_PWR_CH_I_V_P( _PAM *pam_ptr, uint8_t num_pwr_ch);


#ifdef __cplusplus
}
#endif

#endif /* INC_PAM_PAM_CTRL_H_ */
