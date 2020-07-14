#ifndef INC_PMM_PMM_CTRL_H_
#define INC_PMM_PMM_CTRL_H_


ErrorStatus PMM_Set_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel, uint8_t state_channel );
ErrorStatus PMM_Check_state_PWR_CH( _PMM *pmm_ptr, uint8_t num_pwr_channel );
ErrorStatus PDM_Get_PG_all_PWR_CH( _PMM *pmm_ptr );
ErrorStatus PMM_Get_Temperature( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr );
ErrorStatus PMM_Get_PWR_CH_VBAT_I_V_P( _PMM *pmm_ptr, uint8_t num_pwr_ch);

ErrorStatus PMM_ADS1015_average_meas(I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr, uint8_t num_ch_mux, uint8_t average_num, float *average_data);
ErrorStatus PMM_Get_PWR_Supply_m_b_I( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr);

ErrorStatus PMM_Set_MUX_CAN_CPUm_CPUb( _PMM *pmm_ptr );
//ErrorStatus PMM_Switch_Active_CPU(uint8_t set_active_CPU,  _UART_EPS_COMM *UART_Main_eps_comm, _UART_EPS_COMM *UART_Backup_eps_comm, _PMM *pmm_ptr,_PDM *pdm_ptr, _PBM pbm_mas[]);


#endif /* INC_PMM_PMM_CTRL_H_ */
