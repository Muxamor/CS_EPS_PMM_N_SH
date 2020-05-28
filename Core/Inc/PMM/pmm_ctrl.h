#ifndef INC_PMM_PMM_CTRL_H_
#define INC_PMM_PMM_CTRL_H_


#define CPUmain 					0x00
#define CPUbackup					0x01

#define PMM_PWR_CANmain 			0x00
#define PMM_PWR_CANbackup			0x01
#define PMM_PWR_VBAT_1_eF_1			0x02
#define PMM_PWR_VBAT_1_eF_2			0x03
#define PMM_PWR_VBAT_2_eF_1			0x04
#define PMM_PWR_VBAT_2_eF_2			0x05
#define PMM_PWR_PBMs_Logic			0x06
#define PMM_PWR_Deploy_Logic		0x07
#define PMM_PWR_Deploy_Power		0x08
#define PMM_PWR_5V_Bus				0x09
#define PMM_PWR_3_3V_Bus			0x10

ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel );
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel );
ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( uint8_t active_CPU );


#endif /* INC_PMM_PMM_CTRL_H_ */
