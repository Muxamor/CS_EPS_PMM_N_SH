

#ifndef INC_PDM_PDM_INIT_IC_H_
#define INC_PDM_PDM_INIT_IC_H_


int8_t PDM_init_TMP1075(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t i2c_mux_addr, uint8_t mux_ch);
//ErrorStatus PDM_init_I2C_GPIOExt(I2C_TypeDef *I2Cx, uint8_t tca9539_addr);



#endif /* INC_PDM_PDM_INIT_IC_H_ */
