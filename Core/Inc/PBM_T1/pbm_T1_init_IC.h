
#ifndef INC_PBM_T1_INIT_IC_H_
#define INC_PBM_T1_INIT_IC_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus PBM_T1_Init_Heat_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat, uint8_t temp_number);

ErrorStatus PBM_T1_Init_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t temp_number);

ErrorStatus PBM_T1_Init_Heat_INA238(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat);

ErrorStatus PBM_T1_Init_I2CMux(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_INIT_IC_H_ */
