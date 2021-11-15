
#ifndef INC_PBM_T1_INIT_H_
#define INC_PBM_T1_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PBM_T1/pbm_T1_struct.h"
#include "PMM/pmm_struct.h"

ErrorStatus PBM_T1_Init( _PBM_T1 pbm[] );

ErrorStatus PBM_T1_Re_Init( _PBM_T1 pbm[], _PMM *pmm_ptr);

ErrorStatus PBM_T1_Init_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t tmp1075_addr, uint8_t i2c_mux_ch);

ErrorStatus PBM_T1_Init_I2CMux(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_INIT_H_ */
