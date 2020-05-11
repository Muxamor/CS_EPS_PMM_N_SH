/*
 * pdm.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Ivan
 */

#ifndef INC_PDM_PDM_H_
#define INC_PDM_PDM_H_




ErrorStatus PDM_Set_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel, uint8_t state_channel );
ErrorStatus PDM_Check_state_PWR_CH( _PDM *pdm_ptr, uint8_t number_pwr_channel );


#endif /* INC_PDM_PDM_H_ */
