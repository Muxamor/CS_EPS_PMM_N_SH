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
#include "MAX17320.h"
#include "Error_Handler.h"

/** @brief	First Initialize selected PBM.
    @param 	pbm - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_T1_Init( _PBM_T1 pbm[] ) {

	uint8_t PBM_Number = 0;
	uint8_t Error = SUCCESS;
	uint8_t Branch = 0, Heat = 0, TempSens = 0;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

			Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);
	}

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		for(Branch = 0; Branch < PBM_T1_BRANCH_QUANTITY; Branch++){

			Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, 0);
			Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, 0);

			pbm[PBM_Number].Branch[Branch].Counter_Reset_PreqF = 0;

			//Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, pbm[PBM_Number].Branch[Branch].ChgEnableBit);
			//Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, pbm[PBM_Number].Branch[Branch].DchgEnableBit);

    		if (pbm[PBM_Number].Branch[Branch].PCA9534_Emerg_Chrg == ENABLE) {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, PBM_T1_ON_EMERG_CHRG);
    		} else {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, PBM_T1_OFF_EMERG_CHRG);
    		}
		}

        //MAX17320_WriteAccmCharge (PBM_T1_I2C_PORT, 3000, 10);

		for(Heat = 0; Heat < PBM_T1_HEAT_QUANTITY; Heat++){

			Error += PBM_T1_Init_Heat_INA238(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat);

			if (pbm[PBM_Number].Heat[Heat].PCA9534_ON_Heat_CMD == ENABLE) {
                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, PBM_T1_ON_HEAT);
    		} else {
                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, PBM_T1_OFF_HEAT);
    		}

           	for(TempSens = 0; TempSens < PBM_T1_HEAT_TEMPSENS_QUANTITY; TempSens++){
        		Error += PBM_T1_Init_Heat_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, TempSens);
        	}
        }


		PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, 0, 0, PBM_T1_ON_EMERG_CHRG);
        /*for(TempSens = 0; TempSens < PBM_T1_TEMPSENS_QUANTITY; TempSens++){
        	Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens);
        }*/

	}

	PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, 0, 0, PBM_T1_ON_EMERG_CHRG);

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
	uint8_t Branch = 0, Heat = 0, TempSens = 0;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){

			Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);
		}

	}

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){

			for(Branch = 0; Branch < PBM_T1_BRANCH_QUANTITY; Branch++){

				Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, pbm[PBM_Number].Branch[Branch].ChgEnableBit);
				Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, pbm[PBM_Number].Branch[Branch].DchgEnableBit);

	    		if (pbm[PBM_Number].Branch[Branch].PCA9534_Emerg_Chrg == ENABLE) {
	                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, PBM_T1_ON_EMERG_CHRG);
	    		} else {
	                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, PBM_T1_OFF_EMERG_CHRG);
	    		}
			}

			for(Heat = 0; Heat < PBM_T1_HEAT_QUANTITY; Heat++){

				Error += PBM_T1_Init_Heat_INA238(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat);

	           	if (pbm[PBM_Number].Heat[Heat].PCA9534_ON_Heat_CMD == ENABLE) {
	                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, PBM_T1_ON_HEAT);
	    		} else {
	                Error += PBM_T1_SetStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, PBM_T1_OFF_HEAT);
	    		}

	        	if( pbm[PBM_Number].Heat[Heat].PCA9534_Heat_State == 0 ){
	        		for(TempSens = 0; TempSens < PBM_T1_HEAT_TEMPSENS_QUANTITY; TempSens++){
	        			Error += PBM_T1_Init_Heat_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, TempSens);
	        		}
	        	}
	        }

	        /*for(TempSens = 0; TempSens < PBM_T1_TEMPSENS_QUANTITY; TempSens++){
	        	Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens);
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

