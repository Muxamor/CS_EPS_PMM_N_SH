
#ifndef INC_PMM_PMM_DEPLOY_H_
#define INC_PMM_PMM_DEPLOY_H_
#include "main.h"
int8_t PCA_default();
int8_t TCA_default();
int8_t PMM_Enable_eF_Deploy_L();
int8_t PMM_Enable_eF_Deploy_P();
int8_t PMM_Disable_eF_Deploy_L();
int8_t PMM_Disable_eF_Deploy_P();
int8_t PMM_check_switch(uint8_t switch_num, uint8_t *state);
int8_t PMM_burn_the_wire(uint32_t burn_time1, uint32_t burn_time2, uint8_t wire_num);
int8_t PMM_Deploy();

#endif /* INC_PMM_PMM_DEPLOY_H_ */
