
#ifndef INC_PMM_EPS_STRUCT_H_
#define INC_PMM_EPS_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PMM/pmm_struct.h"
#include "PDM/pdm_struct.h"
#include "PAM/pam_struct.h"
#include "PBM/pbm_struct.h"


typedef struct{

	uint8_t Req_SW_Active_CPU :1; // 0 - no request, 1 - Received a request to switch the active CPU
	uint8_t :7;

	uint8_t Set_Active_CPU; // Need set active  0x00-Main CPU, 0x01- Backup CPU 

}_EPS_Service;


typedef struct{

	_PMM *const eps_pmm_ptr;
	_PDM *const eps_pdm_ptr;
	_PAM *const eps_pam_ptr;
	_PBM *const eps_pbm_ptr;
	_EPS_Service *const eps_serv_ptr;

}_EPS_Param;

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_EPS_STRUCT_H_ */
