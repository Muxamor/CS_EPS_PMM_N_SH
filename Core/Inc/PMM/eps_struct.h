
#ifndef INC_PMM_EPS_STRUCT_H_
#define INC_PMM_EPS_STRUCT_H_

#include "pmm_struct.h"
#include "pdm_struct.h"
#include "PBM_struct.h"
//#include "pam_struct.h"

typedef struct{

	_PMM *const eps_pmm_ptr;
	_PDM *const eps_pdm_ptr;
	//_PAM * const eps_pbm_ptr;
	_PBM *const eps_pbm_ptr;

}_EPS_Param;

#endif /* INC_PMM_EPS_STRUCT_H_ */
