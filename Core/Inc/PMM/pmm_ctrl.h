#ifndef INC_PMM_PMM_CTRL_H_
#define INC_PMM_PMM_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/pmm_struct.h"

ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel );
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel );
ErrorStatus PMM_Get_PG_all_PWR_CH( _PMM *pmm_ptr );
ErrorStatus PMM_Get_Temperature( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr );
ErrorStatus PMM_Get_PWR_CH_VBAT_I_V_P( _PMM *pmm_ptr, uint8_t num_pwr_ch);

ErrorStatus PMM_ADS1015_average_meas(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t num_ch_mux, uint8_t average_num, float *average_data);
ErrorStatus PMM_Get_PWR_Supply_m_b_I( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr);

ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( _PMM *pmm_ptr );

ErrorStatus PMM_Reset_Modules( _PMM *pmm_ptr, uint8_t modules );

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_CTRL_H_ */
