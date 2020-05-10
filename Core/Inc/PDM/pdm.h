/*
 * pdm.h
 *
 *  Created on: Apr 21, 2020
 *      Author: Ivan
 */

#ifndef INC_PDM_PDM_H_
#define INC_PDM_PDM_H_



#define PDM_PWR_Channel_1 			0x01 
#define PDM_PWR_Channel_2 			0x02
#define PDM_PWR_Channel_3 			0x03
#define PDM_PWR_Channel_4 			0x04
#define PDM_PWR_Channel_5 			0x05
#define PDM_PWR_Channel_6 			0x06

ErrorStatus PDM_ctrl_pwr_channel( _PDM *pdm_str_ptr, uint8_t number_pwr_channel, uint8_t state_channel );


#endif /* INC_PDM_PDM_H_ */
