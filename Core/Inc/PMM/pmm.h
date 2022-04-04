#ifndef INC_PMM_PMM_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PMM_Get_Telemetry( _PMM *pmm_ptr );
void PMM_CPU_SPEED_MODE( _PMM *pmm_ptr, uint32_t speed_mode );

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_H_ */
