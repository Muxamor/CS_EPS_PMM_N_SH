
#ifndef INC_PMM_PMM_SW_CPU_H_
#define INC_PMM_PMM_SW_CPU_H_


#include "uart_eps_comm.h"

void PMM_Check_Active_CPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm,  _EPS_Param eps_p );
ErrorStatus PMM_Switch_Active_CPU( uint8_t set_active_CPU,  _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p );
void PMM_Set_mode_Active_CPU( _EPS_Param eps_p );

#endif /* INC_PMM_PMM_SW_CPU_H_ */
