

#ifndef INC_PMM_PMM_INIT_IC_H_
#define INC_PMM_PMM_INIT_IC_H_

ErrorStatus PMM_init_TMP1075(_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr);
ErrorStatus PMM_init_PWR_Mon_INA231( _PMM *pmm_ptr, uint8_t num_pwr_ch);
ErrorStatus ADS1015_init( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr);


#endif /* INC_PMM_PMM_INIT_IC_H_ */
