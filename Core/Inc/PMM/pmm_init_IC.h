

#ifndef INC_PMM_PMM_INIT_IC_H_
#define INC_PMM_PMM_INIT_IC_H_

ErrorStatus pmm_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr);
ErrorStatus PMM_default_init_I2C_GPIOExt1 (I2C_TypeDef *I2Cx, uint8_t tca9539_addr); //TEMP Fn

#endif /* INC_PMM_PMM_INIT_IC_H_ */
