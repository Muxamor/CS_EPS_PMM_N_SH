

#ifndef INC_PMM_PMM_INIT_H_
#define INC_PMM_PMM_INIT_H_

#include "pmm_struct.h"

void Init_PMM(_PMM *pmm);

uint8_t PMM_Detect_MasterBackupCPU(void);
ErrorStatus PMM_Detect_PowerRebootCPU(uint8_t *rebot_pwr);


#endif /* INC_PMM_PMM_INIT_H_ */
