

#ifndef INC_PBM_INIT_IC_H_
#define INC_PBM_INIT_IC_H_

#ifdef __cplusplus
extern "C" {
#endif

ErrorStatus TMP1075_InitState(I2C_TypeDef *I2Cx, uint8_t AddrTMP1075);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_INIT_IC_H_ */
