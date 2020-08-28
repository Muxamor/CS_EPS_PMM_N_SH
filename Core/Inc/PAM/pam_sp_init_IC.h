
#ifndef CS_EPS_PMM_N_SH_V_PAM_SP_INIT_IC_H
#define CS_EPS_PMM_N_SH_V_PAM_SP_INIT_IC_H

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PAM_SP_init_TMP1075(_PAM *pam_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch );

#ifdef __cplusplus
}
#endif

#endif //CS_EPS_PMM_N_SH_V_PAM_SP_INIT_IC_H