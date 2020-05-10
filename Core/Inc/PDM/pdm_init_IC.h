

#ifndef INC_PDM_PDM_INIT_IC_H_
#define INC_PDM_PDM_INIT_IC_H_


ErrorStatus PDM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr);
ErrorStatus PDM_init_I2C_GPIOExt(I2C_TypeDef *I2Cx, uint8_t tca9539_addr);



#endif /* INC_PDM_PDM_INIT_IC_H_ */
