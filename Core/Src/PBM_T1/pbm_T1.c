#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_struct.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PBM_T1/pbm_T1_init.h"
#include "PBM_T1/pbm_T1.h"
#include  <stdio.h>
#include "Error_Handler.h"

/** @brief	Read data for all PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	*Data - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_T1_Get_Telemetry(_PBM_T1 pbm[], _PMM *pmm_ptr) {

	uint8_t PBM_Number = 0;
	uint8_t Low_Energy_Flag_counter = 0;
	int8_t Error = SUCCESS;
	uint8_t Branch_number = 0, Heat_number = 0, Heat_TempSens_number = 0;

	if(pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE){

		for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

			for( Branch_number = 0; Branch_number < PBM_T1_BRANCH_QUANTITY; Branch_number++){

				Error = Error + PBM_T1_ReadBatteryTelemetry(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number);
				Error = Error + PBM_T1_CheckChargeDischargeState(pbm, PBM_Number, Branch_number);
				Error = Error + PBM_T1_ReadStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number);
				Error = Error + PBM_T1_CheckResetPreqChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch_number);
				if ( Error != SUCCESS){
					#ifdef DEBUGprintf
						Error_Handler();
						printf("Error Get Telemetry PBM number: %d, Branch number: %d \n", PBM_Number, Branch_number);
					#endif
				}
			}

			for( Heat_number = 0; Heat_number < PBM_T1_HEAT_QUANTITY; Heat_number++){

				for( Heat_TempSens_number = 0; Heat_TempSens_number < PBM_T1_HEAT_TEMPSENS_QUANTITY; Heat_TempSens_number++){
					Error = Error + PBM_T1_ReadHeatTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number, Heat_TempSens_number);
					if ( Error != SUCCESS){
						#ifdef DEBUGprintf
							Error_Handler();
							printf("Error Get Telemetry PBM number: %d, Heat temp. sensor number: %d \n", PBM_Number, Heat_TempSens_number);
						#endif
					}
				}
				Error = Error + PBM_T1_ReadStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number);
				Error = Error + PBM_T1_ReadHeatPwrMon(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number);
				Error = Error + PBM_T1_CheckStateCmdHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat_number);
				Error = Error + PBM_T1_CheckOverHeat(pbm, PBM_Number, Heat_number);

				if ( Error != SUCCESS){
					#ifdef DEBUGprintf
						Error_Handler();
						printf("Error Get Telemetry PBM number: %d, Heat number: %d \n", PBM_Number, Heat_number);
					#endif
				}
			}


			/*for(TempSens_number = 0; TempSens_number < PBM_T1_TEMPSENS_QUANTITY; TempSens_number++){
				Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens_number);
			}*/

			PBM_T1_CalcTotalCapacity(pbm, PBM_Number);
			PBM_T1_CheckLowLevelEnergy(pbm, PBM_Number);
		}

		for ( Low_Energy_Flag_counter = 0, PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {
			Low_Energy_Flag_counter = Low_Energy_Flag_counter + pbm[PBM_Number].Low_Energy_Flag;
		}

		if(Low_Energy_Flag_counter >= 1){
			PBM_Low_Energy_Set_pin();
		} else {
			PBM_Low_Energy_Reset_pin();
		}
	} else {

		PBM_T1_EraseData(pbm);
	}

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}
