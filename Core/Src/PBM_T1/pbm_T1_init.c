#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_struct.h"
#include "PBM_T1/pbm_T1_init_IC.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PMM/pmm_struct.h"
#include "TCA9548.h"
#include "Error_Handler.h"

/** @brief	Initialize TMP1075 sensor for selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
    @retval Error status
 */
ErrorStatus PBM_T1_TempSensorInit(_PBM_T1 pbm[], uint8_t PBM_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error = SUCCESS;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	pbm_table = PBM_T1_Table(PBM_number);

	SW_TMUX1209_I2C_main_PBM();

	//Enable I2C MUX channel

	while( ( error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		error_I2C = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH0);
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = error_I2C;

	if (error_I2C == SUCCESS ){
		if (PBM_T1_TMP1075_InitState(PBM_T1_I2C_PORT, pbm_table.TEMP_SENSOR_1_Addr) == SUCCESS) {
			pbm[PBM_number].Error_TMP1075_1 = SUCCESS;
		} else {
			Error = ERROR_N;
			pbm[PBM_number].Error_TMP1075_1 = ERROR;
			pbm[PBM_number].TMP1075_temp_1 = 0x7F;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}

		if (PBM_T1_TMP1075_InitState(PBM_T1_I2C_PORT, pbm_table.TEMP_SENSOR_2_Addr) == SUCCESS) {
			pbm[PBM_number].Error_TMP1075_2 = SUCCESS;
		} else {
			Error = Error + ERROR_N;
			pbm[PBM_number].Error_TMP1075_2 = ERROR;
			pbm[PBM_number].TMP1075_temp_2 = 0x7F;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH0);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_TMP1075_1 = ERROR;
		pbm[PBM_number].Error_TMP1075_2 = ERROR;
	}else{
		i = 0;
		error_I2C = ERROR_N;
		while( ( error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
			error_I2C = TCA9548_Enable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH3);
			if( error_I2C != SUCCESS ){
				i++;
				LL_mDelay( PBM_T1_i2c_delay_att_conn );
			}
		}

		Error_I2C_MUX = error_I2C;
	}

	if (error_I2C == SUCCESS ){
		if (PBM_T1_TMP1075_InitState(PBM_T1_I2C_PORT, pbm_table.TEMP_SENSOR_3_Addr) == SUCCESS) {
			pbm[PBM_number].Error_TMP1075_3 = SUCCESS;
		} else {
			Error = Error + ERROR_N;
			pbm[PBM_number].Error_TMP1075_3 = ERROR;
			pbm[PBM_number].TMP1075_temp_3 = 0x7F;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}

		if (PBM_T1_TMP1075_InitState(PBM_T1_I2C_PORT, pbm_table.TEMP_SENSOR_4_Addr) == SUCCESS) {
			pbm[PBM_number].Error_TMP1075_4 = SUCCESS;
		} else {
			Error = Error + ERROR_N;
			pbm[PBM_number].Error_TMP1075_4 = ERROR;
			pbm[PBM_number].TMP1075_temp_4 = 0x7F;
			#ifdef DEBUGprintf
				Error_Handler();
			#endif
		}
	}
	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_CH3);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		pbm[PBM_number].Error_TMP1075_3 = ERROR;
		pbm[PBM_number].Error_TMP1075_4 = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

    if( Error != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Initialize TCA9548 for selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
    @retval Error status
 */
ErrorStatus PBM_T1_I2CMuxInit(_PBM_T1 pbm[], uint8_t PBM_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	pbm_table = PBM_T1_Table(PBM_number);

	SW_TMUX1209_I2C_main_PBM();

	//Enable I2C MUX channel

	while( ( error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		error_I2C = TCA9548_Disable_I2C_ch(PBM_T1_I2C_PORT, pbm_table.I2C_MUX_Addr, TCA9548_ALL_CHANNELS);
		if( error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	if( error_I2C == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
		return ERROR_N;
	}

    return SUCCESS;
}

/** @brief	First Initialize selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_T1_Init( _PBM_T1 pbm[] ) {

	uint8_t i = 0;
	uint8_t Error = SUCCESS;

	for (i = 0; i < PBM_T1_QUANTITY; i++) {

        Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, pbm[i].Branch_2_ChgEnableBit);
        Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, 1);
        Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, pbm[i].Branch_2_DchgEnableBit);
        Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, 1);

        Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT);


		if (pbm[i].PCA9534_ON_Heat_1 == ENABLE) {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT);
		} else {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT);
		}
		if (pbm[i].PCA9534_ON_Heat_2 == ENABLE) {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT);
		} else {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT);
		}
		if (pbm[i].PCA9534_SHDN_Chrg_1 == ENABLE) {
            Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT);
		} else {
            Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT);
		}
		if (pbm[i].PCA9534_SHDN_Chrg_2 == ENABLE) {
            Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT);
		} else {
            Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT);
		}
        Error += PBM_T1_TempSensorInit(pbm, i);
        Error += PBM_T1_I2CMuxInit(pbm, i);

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
ErrorStatus PBM_T1_Re_Init(_PBM_T1 pbm[], _PMM *pmm_ptr) {

	uint8_t i = 0;
	int8_t Error = SUCCESS;

	for (i = 0; i < PBM_T1_QUANTITY; i++) {

        Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, pbm[i].Branch_1_ChgEnableBit);
        Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, pbm[i].Branch_2_ChgEnableBit);
        Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, pbm[i].Branch_1_DchgEnableBit);
        Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, pbm[i].Branch_2_DchgEnableBit);

        if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){

            if( pbm[i].PCA9534_ON_Heat_1 == 1 ){
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT);
            }else{
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT);
            }
            if( pbm[i].PCA9534_ON_Heat_2 == 1 ){
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT);
            }else{
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT);
            }
    		if (pbm[i].PCA9534_SHDN_Chrg_1 == ENABLE) {
                Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT);
    		} else {
                Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT);
    		}
    		if (pbm[i].PCA9534_SHDN_Chrg_2 == ENABLE) {
                Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT);
    		} else {
                Error += PBM_T1_SetStateShdnChrg(PBM_T1_I2C_PORT, pbm, i, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT);
    		}
            if( pbm[i].PCA9534_TempSens_State_1 == 0 && pbm[i].PCA9534_TempSens_State_2 == 0 ){
                Error += PBM_T1_TempSensorInit(pbm, i);
            }
            Error += PBM_T1_I2CMuxInit(pbm, i);
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

