

#ifndef INC_PMM_PMM_INIT_H_
#define INC_PMM_PMM_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/eps_struct.h"

ErrorStatus PMM_init(_PMM *pmm_ptr);
uint8_t PMM_Detect_MasterBackupCPU(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_INIT_H_ */
