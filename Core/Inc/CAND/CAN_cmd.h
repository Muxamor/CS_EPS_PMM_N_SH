
#ifndef INC_CAND_CAN_CMD_H_
#define INC_CAND_CAN_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/eps_struct.h"
/*
typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
} FunctionalState;*/


void CAN_Var4_cmd_parser(uint64_t *cmd_status, _EPS_Param eps_p );
void CAN_Var5_fill_telemetry( _EPS_Param eps_p );
void CAN_Var5_fill_telemetry_const(void);
void CAN_Var4_fill_telemetry( _EPS_Param eps_p );

#ifdef __cplusplus
}
#endif

#endif /* INC_CAND_CAN_CMD_H_ */
