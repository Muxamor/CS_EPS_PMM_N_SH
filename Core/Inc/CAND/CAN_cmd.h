/*
 * CAN_cmd.h
 *
 *  Created on: Apr 28, 2020
 *      Author: Ivan
 */

#ifndef INC_CAND_CAN_CMD_H_
#define INC_CAND_CAN_CMD_H_



/*
typedef enum
{
  DISABLE = 0,
  ENABLE = !DISABLE
} FunctionalState;*/



void CAN_Var4_cmd_parser(uint64_t *cmd_status, _PDM *pdm_ptr);
void CAN_Var5_fill_telemetry_const(void);


#endif /* INC_CAND_CAN_CMD_H_ */
