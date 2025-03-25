#ifndef INC_PDM_PDM_CTRL_H_
#define INC_PDM_PDM_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PDM/pdm_struct.h"

ErrorStatus PDM_Set_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel, uint8_t state_channel );
ErrorStatus PDM_Check_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel );
ErrorStatus PDM_Get_PG_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel );
ErrorStatus PDM_Get_Temperature( _PDM *pdm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t i2c_mux_ch );
ErrorStatus PDM_Get_PWR_CH_I_V_P( _PDM *pdm_ptr, uint8_t number_pwr_channel);

#ifdef __cplusplus
}
#endif

#endif /* INC_PDM_PDM_CTRL_H_ */
