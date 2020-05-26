/*
 * pmm_ctrl.h
 *
 *  Created on: 25 мая 2020 г.
 *      Author: Ivan
 */

#ifndef INC_PMM_PMM_CTRL_H_
#define INC_PMM_PMM_CTRL_H_

ErrorStatus PMM_Set_state_PWR_CAN( _PMM *pmm_ptr, uint8_t num_CAN_pwr_channel, uint8_t state_channel );
ErrorStatus PMM_Check_state_PWR_CAN( _PMM *pmm_ptr, uint8_t num_CAN_pwr_channel );
#endif /* INC_PMM_PMM_CTRL_H_ */
