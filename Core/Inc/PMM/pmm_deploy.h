
#ifndef INC_PMM_PMM_DEPLOY_H_
#define INC_PMM_PMM_DEPLOY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
int8_t PCA_default();
int8_t TCA_default();
int8_t PMM_Enable_eF_Deploy_L();
int8_t PMM_Enable_eF_Deploy_P();
int8_t PMM_Disable_eF_Deploy_L();
int8_t PMM_Disable_eF_Deploy_P();
int8_t PMM_check_switch(uint8_t switch_num, uint8_t *state);
ErrorStatus PMM_check_wire(uint32_t burn_time, uint8_t wire_num);
ErrorStatus PMM_burn_the_wire(uint32_t burn_const_time, uint32_t burn_time1, uint32_t burn_time2, uint32_t burn_time3, uint8_t wire_num);
int8_t PMM_Deploy();

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_DEPLOY_H_ */
