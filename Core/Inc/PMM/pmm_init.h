

#ifndef INC_PMM_PMM_INIT_H_
#define INC_PMM_PMM_INIT_H_

#include "pmm_struct.h"
#include "uart_eps_comm.h"

ErrorStatus PMM_init(_PMM *pmm_ptr);

uint8_t PMM_Detect_MasterBackupCPU(void);

void PMM_Check_Active_CPU(_PMM *pmm_ptr, _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm);

#endif /* INC_PMM_PMM_INIT_H_ */
