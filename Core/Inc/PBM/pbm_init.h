
#ifndef INC_PBM_INIT_H_
#define INC_PBM_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PBM/pbm_struct.h"

ErrorStatus PBM_Init(_PBM pbm[]);

ErrorStatus PBM_Re_Init(_PBM pbm[]);

ErrorStatus PBM_TempSensorInit(_PBM pbm[], uint8_t PBM_number);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_INIT_H_ */
