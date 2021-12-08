#ifndef INC_PBM_T1_INIT_H_
#define INC_PBM_T1_INIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PBM_T1/pbm_T1_struct.h"
#include "PMM/pmm_struct.h"

ErrorStatus PBM_T1_Init( _PBM_T1 pbm[] );

ErrorStatus PBM_T1_Re_Init( _PBM_T1 pbm[], _PMM *pmm_ptr);

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_INIT_H_ */
