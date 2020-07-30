

#ifndef CS_EPS_PMM_N_SH_PMM_DEPLOY_H
#define CS_EPS_PMM_N_SH_PMM_DEPLOY_H

#ifdef __cplusplus
extern "C" {
#endif




void PMM_Deploy( _EPS_Param eps_p);
void PMM_Deploy_Burn_PWR_Ch( _EPS_Param eps_p, uint8_t attepmt_burn ,uint8_t burn_pwr_ch_num );
ErrorStatus PMM_Deploy_check_Lim_SW( _EPS_Param eps_p, uint8_t burn_pwr_ch_num );




#ifdef __cplusplus
}
#endif


#endif //CS_EPS_PMM_N_SH_PMM_DEPLOY_H
