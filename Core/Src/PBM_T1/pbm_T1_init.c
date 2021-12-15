#include "stm32l4xx.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_struct.h"
#include "PBM_T1/pbm_T1_init_IC.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PBM_T1/pbm_T1_control.h"
#include "Error_Handler.h"

/** @brief	First Initialize selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_T1_Init( _PBM_T1 pbm[] ) {

	uint8_t PBM_Number = 0;
	uint8_t Error = SUCCESS;
	uint8_t Branch_number = 0, Heat_number = 0, Heat_TempSens_number = 0;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

			Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);
	}

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		for( Branch_number = 0; Branch_number < PBM_T1_BRANCH_QUANTITY; Branch_number++){

			pbm[PBM_Number].Branch[Branch_number].Counter_Reset_PreqF = 0;

			Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, pbm[PBM_Number].Branch[Branch_number].ChgEnableBit);
			Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, pbm[PBM_Number].Branch[Branch_number].DchgEnableBit);

    		if ( pbm[PBM_Number].Branch[Branch_number].PCA9534_Emerg_Chrg_Key == ENABLE) {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, PBM_T1_ON_EMERG_CHRG);
    		} else {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, PBM_T1_OFF_EMERG_CHRG);
    		}
		}

        for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){

			Error += PBM_T1_Init_Heat_INA238(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number);

			if ( pbm[PBM_Number].Heat[Heat_number].PCA9534_ON_Heat_CMD == ENABLE) {
                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, PBM_T1_ON_HEAT);
    		} else {
                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, PBM_T1_OFF_HEAT);
    		}

           	for( Heat_TempSens_number = 0; Heat_TempSens_number < PBM_T1_HEAT_TEMPSENS_QUANTITY; Heat_TempSens_number++){
        		Error += PBM_T1_Init_Heat_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, Heat_TempSens_number);
        	}
        }

		/*for(TempSens_number = 0; TempSens_number < PBM_T1_TEMPSENS_QUANTITY; TempSens_number++){
        	Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens_number);
        }*/

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

	uint8_t PBM_Number = 0;
	int8_t Error = SUCCESS;
	uint8_t Branch_number = 0, Heat_number = 0, Heat_TempSens_number = 0;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){

			Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);
		}

	}

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){

			for( Branch_number = 0; Branch_number < PBM_T1_BRANCH_QUANTITY; Branch_number++){

				Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, pbm[PBM_Number].Branch[Branch_number].ChgEnableBit);
				Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, pbm[PBM_Number].Branch[Branch_number].DchgEnableBit);

	    		if ( pbm[PBM_Number].Branch[Branch_number].PCA9534_Emerg_Chrg_Key == ENABLE) {
	                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, PBM_T1_ON_EMERG_CHRG);
	    		} else {
	                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number, PBM_T1_OFF_EMERG_CHRG);
	    		}
			}

			for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){

				Error += PBM_T1_Init_Heat_INA238(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number);

	           	if ( pbm[PBM_Number].Heat[Heat_number].PCA9534_ON_Heat_CMD == ENABLE) {
	                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, PBM_T1_ON_HEAT);
	    		} else {
	                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, PBM_T1_OFF_HEAT);
	    		}

	        	if( pbm[PBM_Number].Heat[Heat_number].PCA9534_Heat_State == 0 ){
	        		for( Heat_TempSens_number = 0; Heat_TempSens_number < PBM_T1_HEAT_TEMPSENS_QUANTITY; Heat_TempSens_number++){
	        			Error += PBM_T1_Init_Heat_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, Heat_TempSens_number);
	        		}
	        	}
	        }

	        /*for(TempSens_number = 0; TempSens_number < PBM_T1_TEMPSENS_QUANTITY; TempSens_number++){
	        	Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens_number);
	        }*/
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

