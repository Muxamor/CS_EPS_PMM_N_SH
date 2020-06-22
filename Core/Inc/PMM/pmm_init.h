

#ifndef INC_PMM_PMM_INIT_H_
#define INC_PMM_PMM_INIT_H_

#include "pmm_struct.h"

ErrorStatus PMM_init(_PMM *pmm_ptr);

uint8_t PMM_Detect_MasterBackupCPU(void);
ErrorStatus PMM_Detect_PowerRebootCPU(uint8_t *pwr_reboot);


#endif /* INC_PMM_PMM_INIT_H_ */
