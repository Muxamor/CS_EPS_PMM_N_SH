

#ifndef INC_PMM_PMM_INIT_H_
#define INC_PMM_PMM_INIT_H_

#include "pmm_struct.h"
#include "eps_struct.h"

ErrorStatus PMM_init(_PMM *pmm_ptr);
uint8_t PMM_Detect_MasterBackupCPU(void);

void PMM_Init_ActiveCPUblock( _EPS_Param eps_p );
void PMM_Init_PassiveCPUblock(void);

#endif /* INC_PMM_PMM_INIT_H_ */
