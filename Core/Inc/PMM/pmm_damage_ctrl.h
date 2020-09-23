//
// Created by Ivan on 15.08.2020.
//

#ifndef CS_EPS_PMM_N_SH_V_PMM_DAMAGE_CTRL_H
#define CS_EPS_PMM_N_SH_V_PMM_DAMAGE_CTRL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/eps_struct.h"
#include "uart_eps_comm.h"

    extern uint32_t PMM_Start_Time_Check_CAN;
    extern uint32_t PMM_Start_Time_Check_UART_PassiveCPU;

void PMM_Damage_Check_CAN_m_b( _EPS_Param eps_p );
ErrorStatus PMM_Damage_Check_UART_m_b_ActiveCPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p );
void PMM_Damage_Check_UART_m_b_PassiveCPU( _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _EPS_Param eps_p );
void PMM_Portecion_PWR_OFF_CAN_m_b( _EPS_Param eps_p );
void PMM_Portecion_PWR_OFF_BRC_m_b( _EPS_Param eps_p );
void PMM_ReInit_EPS( _EPS_Param eps_p );
void PMM_ZERO_Energy_PWR_OFF_SubSystem( _EPS_Param eps_p );


#ifdef __cplusplus
}
#endif


#endif //CS_EPS_PMM_N_SH_V_PMM_DAMAGE_CTRL_H
