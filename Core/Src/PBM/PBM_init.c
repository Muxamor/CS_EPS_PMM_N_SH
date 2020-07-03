#include "main.h"
#include "PBM_config.h"
#include "PBM_struct.h"
#include "PBM_init_IC.h"
#include "PBM_init.h"
#include "PBM_control.h"
#include "Error_Handler.h"

/** @brief	Initialize PCA9534 sensor for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
// void PBM_GPIOInit(PBM_Data_All *Data, uint8_t PBM) {
// 	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
// 		if (PCA9534_InitState(I2C4, PBM_1_PCA9534_ADDR) == SUCCESS) {
// 			Data->PBM_Data_1.Error_PCA9534 = SUCCESS;
// 			Data->PBM_Data_1.Error_Heat_1 = SUCCESS;
// 			Data->PBM_Data_1.Error_Heat_2 = SUCCESS;
// 		} else {
// #ifdef DEBUGprintf
// 			Error_Handler();
// #endif
// 			Data->PBM_Data_1.Error_PCA9534 = 1;
// 			Data->PBM_Data_1.Error_Heat_1 = 1;
// 			Data->PBM_Data_1.Error_Heat_2 = 1;
// 		}
// 	}
// 	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
// 		if (PCA9534_InitState(I2C4, PBM_2_PCA9534_ADDR) == SUCCESS) {
// 			Data->PBM_Data_2.Error_PCA9534 = SUCCESS;
// 			Data->PBM_Data_2.Error_Heat_1 = SUCCESS;
// 			Data->PBM_Data_2.Error_Heat_2 = SUCCESS;
// 		} else {
// 			#ifdef DEBUGprintf
// 				Error_Handler();
// 			#endif
// 			Data->PBM_Data_2.Error_PCA9534 = 1;
// 			Data->PBM_Data_2.Error_Heat_1 = 1;
// 			Data->PBM_Data_2.Error_Heat_2 = 1;
// 		}
// 	}
// 	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
// 		if (PCA9534_InitState(I2C4, PBM_3_PCA9534_ADDR) == SUCCESS) {
// 			Data->PBM_Data_3.Error_PCA9534 = SUCCESS;
// 			Data->PBM_Data_3.Error_Heat_1 = SUCCESS;
// 			Data->PBM_Data_3.Error_Heat_2 = SUCCESS;
// 		} else {
// #ifdef DEBUGprintf
// 			Error_Handler();
// #endif
// 			Data->PBM_Data_3.Error_PCA9534 = 1;
// 			Data->PBM_Data_3.Error_Heat_1 = 1;
// 			Data->PBM_Data_3.Error_Heat_2 = 1;
// 		}
// 	}
// }

/** @brief	Initialize TMP1075 sensor for selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_TempSensorInit(PBM_Data_All *Data, uint8_t PBM) {

	if ((PBM == PBM_1) | (PBM == PBM_ALL)) {
		if (TMP1075_InitState(I2C4, PBM_1_TMP1075_ADDR_1) == SUCCESS) {
			Data->PBM_Data_1.Error_TMP1075_1 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			Data->PBM_Data_1.Error_TMP1075_1 = ERROR;
		}

		if (TMP1075_InitState(I2C4, PBM_1_TMP1075_ADDR_2) == SUCCESS) {
			Data->PBM_Data_1.Error_TMP1075_2 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			Data->PBM_Data_1.Error_TMP1075_2 = ERROR;
		}

		if (TMP1075_InitState(I2C4, PBM_1_TMP1075_ADDR_3) == SUCCESS) {
			Data->PBM_Data_1.Error_TMP1075_3 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
			Data->PBM_Data_1.Error_TMP1075_3 = 1;
		}

		if (TMP1075_InitState(I2C4, PBM_1_TMP1075_ADDR_4) == SUCCESS) {
			Data->PBM_Data_1.Error_TMP1075_4 = SUCCESS;
		} else {
			#ifdef DEBUGprintf
			Error_Handler();
			#endif
			Data->PBM_Data_1.Error_TMP1075_4 = ERROR;
		}
	}
	

	if ((PBM == PBM_2) | (PBM == PBM_ALL)) {
		if (TMP1075_InitState(I2C4, PBM_2_TMP1075_ADDR_1) == SUCCESS) {
			Data->PBM_Data_2.Error_TMP1075_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_1 = 1;
		}
		if (TMP1075_InitState(I2C4, PBM_2_TMP1075_ADDR_2) == SUCCESS) {
			Data->PBM_Data_2.Error_TMP1075_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_2 = 1;
		}
		if (TMP1075_InitState(I2C4, PBM_2_TMP1075_ADDR_3) == SUCCESS) {
			Data->PBM_Data_2.Error_TMP1075_3 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_3 = 1;
		}
		if (TMP1075_InitState(I2C4, PBM_2_TMP1075_ADDR_4) == SUCCESS) {
			Data->PBM_Data_2.Error_TMP1075_4 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_2.Error_TMP1075_4 = 1;
		}
	}
	if ((PBM == PBM_3) | (PBM == PBM_ALL)) {
		if (TMP1075_InitState(I2C4, PBM_3_TMP1075_ADDR_1) == SUCCESS) {
			Data->PBM_Data_3.Error_TMP1075_1 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_1 = 1;
		}
		if (TMP1075_InitState(I2C4, PBM_3_TMP1075_ADDR_2) == SUCCESS) {
			Data->PBM_Data_3.Error_TMP1075_2 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_2 = 1;
		}
		if (TMP1075_InitState(I2C4, PBM_3_TMP1075_ADDR_3) == SUCCESS) {
			Data->PBM_Data_3.Error_TMP1075_3 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_3 = 1;
		}
		if (TMP1075_InitState(I2C4, PBM_3_TMP1075_ADDR_4) == SUCCESS) {
			Data->PBM_Data_3.Error_TMP1075_4 = SUCCESS;
		} else {
#ifdef DEBUGprintf
			Error_Handler();
#endif
			Data->PBM_Data_3.Error_TMP1075_4 = 1;
		}
	}
}

/** @brief	Initialize selected PBM.
 @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
 @param 	*Data - structure data for all PBM modules.
 @param 	PBM - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
 @retval 	none
 */
void PBM_Init(PBM_Data_All *Data, uint8_t PBM){
	//!!!!!!!!!!!!Вписать сразу для трех
	//PBM_GPIOInit(Data, PBM);
	PBM_SetStateHeatBranch(I2C4, Data, PBM_1, PBM_BRANCH_ALL, PBM_ON_HEAT);
	PBM_TempSensorInit(Data, PBM);
}

