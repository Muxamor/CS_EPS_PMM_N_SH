#include "main.h"
#include "PBM_config.h"
#include "PBM_struct.h"
#include "PBM_init_IC.h"
#include "PBM_init.h"
#include "PBM_control.h"
#include "Error_Handler.h"

/** @brief	Initialize TMP1075 sensor for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @retval 	none
 */
void PBM_TempSensorInit(_PBM *pbm, uint8_t PBM) {

	uint8_t addr = 0;

	PBM_Temp_Table(PBM, 1, &addr);
	if (TMP1075_InitState(I2C4, addr) == SUCCESS) {
		pbm[PBM].Error_TMP1075_1 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_1 = ERROR;
	}
	PBM_Temp_Table(PBM, 2, &addr);
	if (TMP1075_InitState(I2C4, addr) == SUCCESS) {
		pbm[PBM].Error_TMP1075_2 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_2 = ERROR;
	}
	PBM_Temp_Table(PBM, 3, &addr);
	if (TMP1075_InitState(I2C4, addr) == SUCCESS) {
		pbm[PBM].Error_TMP1075_3 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_3 = 1;
	}
	PBM_Temp_Table(PBM, 4, &addr);
	if (TMP1075_InitState(I2C4, addr) == SUCCESS) {
		pbm[PBM].Error_TMP1075_4 = SUCCESS;
	} else {
		#ifdef DEBUGprintf
		Error_Handler();
		#endif
		pbm[PBM].Error_TMP1075_4 = ERROR;
	}
}

/** @brief	Initialize selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_Init(_PBM *pbm) {

	uint8_t i = 0;

	for (i = 0; i < PBM_QUANTITY; i++) {
		if (pbm[i].PCA9534_ON_Heat_1 == 1) {
			PBM_SetStateHeatBranch(I2C4, pbm, i, PBM_BRANCH_1, PBM_ON_HEAT);
		} else {
			PBM_SetStateHeatBranch(I2C4, pbm, i, PBM_BRANCH_1, PBM_OFF_HEAT);
		}
		if (pbm[i].PCA9534_ON_Heat_2 == 1) {
			PBM_SetStateHeatBranch(I2C4, pbm, i, PBM_BRANCH_2, PBM_ON_HEAT);
		} else {
			PBM_SetStateHeatBranch(I2C4, pbm, i, PBM_BRANCH_2, PBM_OFF_HEAT);
		}
		PBM_TempSensorInit(pbm, i);
	}
}

/** @brief	Initialize table selected GPIO mux for selected PBM.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @param  	Addr - Returned adress for selected PBM.
 @retval 	none
 */
void PBM_GPIO_Table(uint8_t PBM, uint8_t *Addr) {
	if (PBM == 0) {
		*Addr = PBM_1_PCA9534_ADDR;
	} else if (PBM == 1) {
		*Addr = PBM_2_PCA9534_ADDR;
	} else {
		*Addr = PBM_3_PCA9534_ADDR;
	}
}

/** @brief	Initialize table selected Temperature sensors mux for selected PBM.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @param 	Number - select number temperature sensors (1 - 4).
 @param  	Addr - Returned adress for selected PBM.
 @retval 	none
 */
void PBM_Temp_Table(uint8_t PBM, uint8_t number, uint8_t *Addr) {
	*Addr = 0;
	if (PBM == 0) {
		if (number == 1) {
			*Addr = PBM_1_TMP1075_ADDR_1;
		} else if (number == 2) {
			*Addr = PBM_1_TMP1075_ADDR_2;
		} else if (number == 3) {
			*Addr = PBM_1_TMP1075_ADDR_3;
		} else if (number == 4) {
			*Addr = PBM_1_TMP1075_ADDR_4;
		}

	} else if (PBM == 1) {
		if (number == 1) {
			*Addr = PBM_2_TMP1075_ADDR_1;
		} else if (number == 2) {
			*Addr = PBM_2_TMP1075_ADDR_2;
		} else if (number == 3) {
			*Addr = PBM_2_TMP1075_ADDR_3;
		} else if (number == 4) {
			*Addr = PBM_2_TMP1075_ADDR_4;
		}
	} else {
		if (number == 1) {
			*Addr = PBM_3_TMP1075_ADDR_1;
		} else if (number == 2) {
			*Addr = PBM_3_TMP1075_ADDR_2;
		} else if (number == 3) {
			*Addr = PBM_3_TMP1075_ADDR_3;
		} else if (number == 4) {
			*Addr = PBM_3_TMP1075_ADDR_4;
		}
	}
}

/** @brief	Initialize table selected DS2777 mux for selected PBM.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3).
 @param  	Addr - Returned adress for selected PBM.
 @retval 	none
 */
void PBM_DS2777_Table(uint8_t PBM, uint8_t branch, uint8_t *Addr) {
	if (PBM == 0) {
		if (branch == PBM_BRANCH_1) {
			*Addr = PBM_1_DS2777_ADDR_1;
		} else if (branch == PBM_BRANCH_2) {
			*Addr = PBM_1_DS2777_ADDR_2;
		}
	} else if (PBM == 1) {
		if (branch == PBM_BRANCH_1) {
			*Addr = PBM_2_DS2777_ADDR_1;
		} else if (branch == PBM_BRANCH_2) {
			*Addr = PBM_2_DS2777_ADDR_2;
		}
	} else {
		if (branch == PBM_BRANCH_1) {
			*Addr = PBM_3_DS2777_ADDR_1;
		} else if (branch == PBM_BRANCH_2) {
			*Addr = PBM_3_DS2777_ADDR_2;
		}
	}
}
