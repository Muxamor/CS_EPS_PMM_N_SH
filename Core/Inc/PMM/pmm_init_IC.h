#ifndef INC_PMM_PMM_INIT_IC_H_
#define INC_PMM_PMM_INIT_IC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/pmm_struct.h"

ErrorStatus PMM_init_TMP1075(_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr);
ErrorStatus PMM_Power_Down_TMP1075(_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tmp1075_addr);
ErrorStatus PMM_init_PWR_Mon_INA231( _PMM *pmm_ptr, uint8_t num_pwr_ch);
ErrorStatus PMM_PWR_Down_PWR_Mon_INA231( _PMM *pmm_ptr, uint8_t num_pwr_ch);
ErrorStatus ADS1015_init( _PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t I2C_ADS1015_addr);
ErrorStatus PMM_DeInit_I2C_GPIOExt (_PMM *pmm_ptr, I2C_TypeDef *I2Cx, uint8_t tca9539_addr);
void PMM_HARD_Reset_I2C_GPIOExt( uint8_t tca9539_addr );
void PMM_Reset_pin_Pull_Down_I2C_GPIOExt( uint8_t tca9539_addr );
void PMM_Reset_pin_Free_I2C_GPIOExt( uint8_t tca9539_addr );

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_INIT_IC_H_ */
