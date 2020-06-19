#ifndef INC_PMM_PMM_CTRL_H_
#define INC_PMM_PMM_CTRL_H_


ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel );
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel );
ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( uint8_t active_CPU );


#endif /* INC_PMM_PMM_CTRL_H_ */
