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

/** @brief	Initialize TMP1075 sensor for selected heat.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
   	@param 	Heat_Branch - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  temp_number - I2C sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval Error status
 */
ErrorStatus PBM_T1_Init_Heat_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat, uint8_t temp_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);
		if( Error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_I2C;

	if (Error_I2C == SUCCESS ){

		//Setup TMP1075
		i=0;
		Error_I2C = ERROR_N;

		while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

			Error_I2C = PBM_T1_Heat_TMP1075_InitState(I2Cx, pbm_table.TempSens_Heat_Addr[temp_number]);

			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_Heat);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Heat[Heat].Heat_TMP1075[temp_number] = 0x7F;
		pbm[PBM_number].Heat[Heat].Error_Heat_TMP1075[temp_number]= ERROR;
	}else{
		pbm[PBM_number].Heat[Heat].Heat_TMP1075[temp_number] = 0x00;
		pbm[PBM_number].Heat[Heat].Error_Heat_TMP1075[temp_number] = SUCCESS; //No error
	}

	if( Error_I2C != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Initialize TMP1075 sensor for selected PBM.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
   	@param 	Heat_Branch - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
	@param  temp_number - I2C sensor number (PBM_T1_TEMPSENS_1, PBM_T1_TEMPSENS_2).
	@retval Error status
 */
/*ErrorStatus PBM_T1_Init_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t temp_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, 0);

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_TempSens);
		if( Error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_I2C;

	if (Error_I2C == SUCCESS ){

		//Setup TMP1075
		i=0;
		Error_I2C = ERROR_N;

		while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

			//Error_I2C = PBM_T1_TMP1075_InitState(I2Cx, pbm_table.TEMP_SENSOR_Addr[temp_number]);

			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_TempSens);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].TMP1075_Temp[temp_number] = 0x7F;
		pbm[PBM_number].Error_TMP1075[temp_number]= ERROR;
	}else{
		pbm[PBM_number].TMP1075_Temp[temp_number] = 0x00;
		pbm[PBM_number].Error_TMP1075[temp_number] = SUCCESS; //No error
	}

	if( Error_I2C != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}*/

/** @brief	Initialize power monitor INA238 for selected heat.
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
   	@param 	Heat_Branch - select Branch (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval Error status
 */
ErrorStatus PBM_T1_Init_Heat_INA238(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t Heat) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	SW_TMUX1209_I2C_main_PBM();

	pbm_table = PBM_T1_Table(PBM_number, 0, Heat);

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_PwrMon);
		if( Error_I2C != SUCCESS ){
			i++;
			LL_mDelay(PBM_T1_i2c_delay_att_conn);
		}
	}

	Error_I2C_MUX = Error_I2C;

	if (Error_I2C == SUCCESS ){

		//Setup TMP1075
		i=0;
		Error_I2C = ERROR_N;

		while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){

			Error_I2C = PBM_T1_Heat_INA238_InitState(I2Cx, pbm_table.PwrMon_Addr);

			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, pbm_table.I2C_MUX_Ch_PwrMon);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}


	if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Heat[Heat].Error_INA238 = ERROR;
	}else{
		pbm[PBM_number].Heat[Heat].Error_INA238 = SUCCESS; //No error
	}

	if( Error_I2C != SUCCESS){
        #ifdef DEBUGprintf
            Error_Handler();
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Initialize TCA9548 for selected PBM.
   	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
    @retval Error status
 */
ErrorStatus PBM_T1_Init_I2CMux(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t error_I2C = ERROR_N; //0-OK -1-ERROR_N

	pbm_table = PBM_T1_Table(PBM_number, 0, 0);

	SW_TMUX1209_I2C_main_PBM();

	//Enable I2C MUX channel

	while( ( error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		error_I2C = TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, TCA9548_ALL_CHANNELS);
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

	uint8_t PBM_Number = 0;
	uint8_t Error = SUCCESS;
	uint8_t Branch = 0, Heat = 0, TempSens = 0;

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

			Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);
	}

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		for(Branch = 0; Branch < PBM_T1_BRANCH_QUANTITY; Branch++){

			Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, pbm[PBM_Number].Branch[Branch].ChgEnableBit);
			Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, pbm[PBM_Number].Branch[Branch].DchgEnableBit);

    		if (pbm[PBM_Number].Branch[Branch].PCA9534_Emerg_Chrg == ENABLE) {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, PBM_T1_ON_EMERG_CHRG);
    		} else {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, Branch, PBM_T1_OFF_EMERG_CHRG);
    		}
		}

        //Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, 1, PBM_T1_I2C_MUX_CH_Br1);
        //Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, 1, PBM_T1_I2C_MUX_CH_Br1);

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

        /*for(TempSens = 0; TempSens < PBM_T1_TEMPSENS_QUANTITY; TempSens++){
        	Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, TempSens);
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

