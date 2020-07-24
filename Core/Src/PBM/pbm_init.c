#include "stm32l4xx.h"
#include "PBM/pbm_config.h"
#include "PBM/pbm_struct.h"
#include "PBM/pbm_init_IC.h"
#include "PBM/pbm_init.h"
#include "PBM/pbm_control.h"
#include "Error_Handler.h"

/** @brief	Initialize TMP1075 sensor for selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
    @retval Error status
 */
ErrorStatus PBM_TempSensorInit(_PBM pbm[], uint8_t PBM_number) {

	_PBM_table pbm_table = { 0 };
	int8_t Error = SUCCESS;

	pbm_table = PBM_Table(PBM_number);

	if (TMP1075_InitState(PBM_I2C_PORT, pbm_table.TEMP_SENSOR_1_Addr) == SUCCESS) {
		pbm[PBM_number].Error_TMP1075_1 = SUCCESS;
	} else {
		Error = ERROR_N;
		pbm[PBM_number].Error_TMP1075_1 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if (TMP1075_InitState(PBM_I2C_PORT, pbm_table.TEMP_SENSOR_2_Addr) == SUCCESS) {
		pbm[PBM_number].Error_TMP1075_2 = SUCCESS;
	} else {
        Error = Error + ERROR_N;
		pbm[PBM_number].Error_TMP1075_2 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if (TMP1075_InitState(PBM_I2C_PORT, pbm_table.TEMP_SENSOR_3_Addr) == SUCCESS) {
		pbm[PBM_number].Error_TMP1075_3 = SUCCESS;
	} else {
        Error = Error + ERROR_N;
		pbm[PBM_number].Error_TMP1075_3 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
	}

	if (TMP1075_InitState(PBM_I2C_PORT, pbm_table.TEMP_SENSOR_4_Addr) == SUCCESS) {
		pbm[PBM_number].Error_TMP1075_4 = SUCCESS;
	} else {
        Error = Error + ERROR_N;
		pbm[PBM_number].Error_TMP1075_4 = ERROR;
		#ifdef DEBUGprintf
			Error_Handler();
	    #endif
	}

    if( Error != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	First Initialize selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_Init(_PBM pbm[]) {

	uint8_t i = 0;
	uint8_t Error = SUCCESS;

	for (i = 0; i < PBM_QUANTITY; i++) {

        Error += PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, pbm[i].Branch_1_ChgEnableBit);
        Error += PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, pbm[i].Branch_2_ChgEnableBit);
        Error += PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, pbm[i].Branch_1_DchgEnableBit);
        Error += PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, pbm[i].Branch_2_DchgEnableBit);

		if (pbm[i].PCA9534_ON_Heat_1 == 1) {
            Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, PBM_ON_HEAT);
		} else {
            Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, PBM_OFF_HEAT);
		}
		if (pbm[i].PCA9534_ON_Heat_2 == 1) {
            Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, PBM_ON_HEAT);
		} else {
            Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, PBM_OFF_HEAT);
		}
        Error += PBM_TempSensorInit(pbm, i);
	}

    if( Error != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Periodic Initialize selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @retval none
 */
ErrorStatus PBM_Re_Init(_PBM pbm[]) {

	uint8_t i = 0;
	int8_t Error = SUCCESS;

	for (i = 0; i < PBM_QUANTITY; i++) {

        Error += PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, pbm[i].Branch_1_ChgEnableBit);
        Error += PBM_SetStateChargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, pbm[i].Branch_2_ChgEnableBit);
        Error += PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, pbm[i].Branch_1_DchgEnableBit);
        Error += PBM_SetStateDischargeBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, pbm[i].Branch_2_DchgEnableBit);

		if (pbm[i].PCA9534_ON_Heat_1 == 1) {
		    Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, PBM_ON_HEAT);
		} else {
		    Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_1, PBM_OFF_HEAT);
		}
		if (pbm[i].PCA9534_ON_Heat_2 == 1) {
		    Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, PBM_ON_HEAT);
		} else {
		    Error += PBM_SetStateHeatBranch(PBM_I2C_PORT, pbm, i, PBM_BRANCH_2, PBM_OFF_HEAT);
		}
		if (pbm[i].PCA9534_TempSens_State_1 == 0 && pbm[i].PCA9534_TempSens_State_2 == 0) {
		    Error += PBM_TempSensorInit(pbm, i);
		}
	}

    if( Error != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

