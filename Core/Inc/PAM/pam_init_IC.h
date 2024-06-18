#ifndef INC_PAM_PAM_INIT_IC_H_
#define INC_PAM_PAM_INIT_IC_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PAM_init_TMP1075(_PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch);
ErrorStatus PAM_init_PWR_Mon_INA238(_PAM *pam_ptr, uint8_t num_pwr_ch);

#ifdef __cplusplus
}
#endif

#endif /* INC_PAM_PAM_INIT_IC_H_ */
