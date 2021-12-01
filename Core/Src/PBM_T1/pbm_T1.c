#include "stm32l4xx_ll_gpio.h"
#include "SetupPeriph.h"
#include "TCA9548.h"
#include "MAX17320.h"
#include "PBM_T1/pbm_T1_config.h"
#include "PBM_T1/pbm_T1_struct.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PBM_T1/pbm_T1.h"


/** @brief	Read data for all PBM.
    @param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	*Data - structure data for all PBM modules.
    @retval 	none
 */
ErrorStatus PBM_T1_Get_Telemetry(_PBM_T1 pbm[]) {

	uint8_t PBM_Number = 0;
	uint8_t Low_Energy_Flag_counter = 0;
	int8_t Error = SUCCESS;
	uint8_t Branch = 0, Heat = 0, TempSens = 0;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		for(Branch = 0; Branch < PBM_T1_BRANCH_QUANTITY; Branch++){

			Error = Error + PBM_T1_ReadBatteryTelemetry(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch);
			Error = Error + PBM_T1_CheckChargeDischargeState(pbm, PBM_Number, Branch);
			Error = Error + PBM_T1_ReadStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch);
			Error = Error + PBM_T1_CheckResetPreqChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch);

		}

		for(Heat = 0; Heat < PBM_T1_HEAT_QUANTITY; Heat++){

			for(TempSens = 0; TempSens < PBM_T1_HEAT_TEMPSENS_QUANTITY; TempSens++){
				Error = Error + PBM_T1_ReadHeatTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat, TempSens);
			}
			Error = Error + PBM_T1_ReadStateHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat);
			Error = Error + PBM_T1_ReadHeatPwrMon(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat);
			Error = Error + PBM_T1_CheckStateCmdHeat(PBM_T1_I2C_PORT, pbm, PBM_Number, Heat);
			Error = Error + PBM_T1_CheckOverHeat(pbm, PBM_Number, Heat);
		}

		/*for(TempSens = 0; TempSens < PBM_T1_TEMPSENS_QUANTITY; TempSens++){
        	Error = Error + PBM_T1_ReadTempSensors(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens);
        }*/

		PBM_T1_CorrectCapacity(PBM_T1_I2C_PORT, pbm, 0, 0);

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

	if (Error != SUCCESS) {
		return ERROR_N;
	}
    return SUCCESS;
}
