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
ErrorStatus PDM_Get_temp_all_sensor( _PDM *pdm_ptr );
ErrorStatus PDM_Get_temp_TMP1075( I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, _PDM *pdm_ptr );


#endif /* INC_PDM_PDM_H_ */
