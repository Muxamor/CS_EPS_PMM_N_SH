
#ifndef CS_EPS_PMM_N_SH_PMM_DEPLOY_H
#define CS_EPS_PMM_N_SH_PMM_DEPLOY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/eps_struct.h"

void PMM_Deploy( _EPS_Param eps_p);
ErrorStatus PMM_Deploy_Burn_Procedure( _EPS_Param eps_p, uint8_t burn_pwr_ch_num );
ErrorStatus PMM_Deploy_Burn_PWR_Ch( _EPS_Param eps_p, uint8_t attempt_burn , uint8_t burn_pwr_ch_num, uint8_t *ret_state_limit_switch_1, uint8_t *ret_state_limit_switch_2);
ErrorStatus PMM_Deploy_check_Lim_SW( _EPS_Param eps_p, uint8_t burn_pwr_ch_num, uint8_t *ret_state_limit_switch_1,  uint8_t *ret_state_limit_switch_2);
ErrorStatus PMM_Deploy_Get_Exit_LSW( _EPS_Param eps_p, uint8_t *ret_exit_LSW_1, uint8_t *ret_exit_LSW_2 );

#ifdef __cplusplus
}
#endif


#endif //CS_EPS_PMM_N_SH_PMM_DEPLOY_H
