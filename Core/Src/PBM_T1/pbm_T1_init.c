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
  	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param 	pbm - structure data for all PBM modules.
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3).
    @param  tmp1075_addr - I2C sensor address
	@param  i2c_mux_ch  - Number channel MUX
    @retval Error status
 */
ErrorStatus PBM_T1_Init_TMP1075(I2C_TypeDef *I2Cx, _PBM_T1 pbm[], uint8_t PBM_number, uint8_t tmp1075_addr, uint8_t i2c_mux_ch) {

	_PBM_T1_table pbm_table = { 0 };
	uint8_t i = 0;
	int8_t Error_I2C = ERROR_N; //0-OK -1-ERROR_N
	int8_t Error_I2C_MUX = ERROR_N;

	pbm_table = PBM_T1_Table(PBM_number);

	SW_TMUX1209_I2C_main_PBM();

	//Enable I2C MUX channel

	while( ( Error_I2C != SUCCESS ) && ( i < PBM_T1_I2C_ATTEMPT_CONN ) ){
		Error_I2C = TCA9548_Enable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch);
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

			Error_I2C = PBM_T1_TMP1075_InitState(I2Cx, tmp1075_addr);
			if( Error_I2C != SUCCESS ){
				i++;
				LL_mDelay(PBM_T1_i2c_delay_att_conn);
			}
		}
	}

	//Disable I2C MUX channel.
	//Note: Do not check the error since it doesn’t matter anymore.
	TCA9548_Disable_I2C_ch(I2Cx, pbm_table.I2C_MUX_Addr, i2c_mux_ch);

	//Parse error
	if( Error_I2C_MUX == ERROR_N ){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		pbm[PBM_number].Error_I2C_MUX = ERROR;
	}else{
		pbm[PBM_number].Error_I2C_MUX = SUCCESS;
	}

	switch( tmp1075_addr ){

		case PBM_T1_TMP1075_ADDR_1:
			if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_1 = 0x7F;
				pbm[PBM_number].Error_TMP1075_1 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_1 = 0x00;
				pbm[PBM_number].Error_TMP1075_1 = SUCCESS; //No error
			}
			break;

		case PBM_T1_TMP1075_ADDR_2:
			if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_2 = 0x7F;
				pbm[PBM_number].Error_TMP1075_2 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_2 = 0x00;
				pbm[PBM_number].Error_TMP1075_2 = SUCCESS; //No error
			}
			break;

		case PBM_T1_TMP1075_ADDR_3:
			if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_3 = 0x7F;
				pbm[PBM_number].Error_TMP1075_3 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_3 = 0x00;
				pbm[PBM_number].Error_TMP1075_3 = SUCCESS; //No error
			}
			break;

		case PBM_T1_TMP1075_ADDR_4:
			if( Error_I2C == ERROR_N || Error_I2C_MUX == ERROR_N ){//Error I2C TMP1075 or I2C MUX
				#ifdef DEBUGprintf
					Error_Handler();
				#endif
				pbm[PBM_number].TMP1075_temp_4 = 0x7F;
				pbm[PBM_number].Error_TMP1075_4 = ERROR;
			}else{
				pbm[PBM_number].TMP1075_temp_4 = 0x00;
				pbm[PBM_number].Error_TMP1075_4 = SUCCESS; //No error
			}
			break;

		default:
			break;
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

	pbm_table = PBM_T1_Table(PBM_number);

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

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);

        Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, pbm[PBM_Number].Branch[1].ChgEnableBit, PBM_T1_I2C_MUX_CH_Br1);
        Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, 1, PBM_T1_I2C_MUX_CH_Br2);
        Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, pbm[PBM_Number].Branch[1].DchgEnableBit, PBM_T1_I2C_MUX_CH_Br1);
        Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, 1, PBM_T1_I2C_MUX_CH_Br2);

       	if (pbm[PBM_Number].PCA9534_ON_Heat_1 == ENABLE) {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		} else {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		}
		if (pbm[PBM_Number].PCA9534_ON_Heat_2 == ENABLE) {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		} else {
            Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		}
		if (pbm[PBM_Number].PCA9534_Emerg_Chrg_1 == ENABLE) {
            Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		} else {
            Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		}
		if (pbm[PBM_Number].PCA9534_Emerg_Chrg_2 == ENABLE) {
            Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		} else {
            Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
		}
        Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_1, PBM_T1_I2C_MUX_CH_Temp1);
        Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_2, PBM_T1_I2C_MUX_CH_Temp1);
        Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_3, PBM_T1_I2C_MUX_CH_Temp2);
        Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_4, PBM_T1_I2C_MUX_CH_Temp2);

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

	for (PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++) {

		if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE ){

			Error += PBM_T1_Init_I2CMux(PBM_T1_I2C_PORT, pbm, PBM_Number);

			Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, pbm[PBM_Number].Branch[1].ChgEnableBit, PBM_T1_I2C_MUX_CH_Br1);
			Error += PBM_T1_SetStateChargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, pbm[PBM_Number].Branch[2].ChgEnableBit, PBM_T1_I2C_MUX_CH_Br2);
			Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, pbm[PBM_Number].Branch[1].DchgEnableBit, PBM_T1_I2C_MUX_CH_Br1);
			Error += PBM_T1_SetStateDischargeBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, pbm[PBM_Number].Branch[2].DchgEnableBit, PBM_T1_I2C_MUX_CH_Br2);

            if( pbm[PBM_Number].PCA9534_ON_Heat_1 == ENABLE ){
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
            }else{
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
            }
            if( pbm[PBM_Number].PCA9534_ON_Heat_2 == ENABLE ){
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
            }else{
                Error += PBM_T1_SetStateHeatBranch(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
            }
    		if (pbm[PBM_Number].PCA9534_Emerg_Chrg_1 == ENABLE) {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
    		} else {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_1, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
    		}
    		if (pbm[PBM_Number].PCA9534_Emerg_Chrg_2 == ENABLE) {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_ON_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
    		} else {
                Error += PBM_T1_SetStateEmergChrg(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_BRANCH_2, PBM_T1_OFF_HEAT, PBM_T1_I2C_MUX_CH_GPIO);
    		}
            if( pbm[PBM_Number].PCA9534_TempSens_State_1 == 0 ){
                Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_1, PBM_T1_I2C_MUX_CH_Temp1);
                Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_2, PBM_T1_I2C_MUX_CH_Temp1);
            }
            if( pbm[PBM_Number].PCA9534_TempSens_State_2 == 0 ){
                Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_3, PBM_T1_I2C_MUX_CH_Temp2);
                Error += PBM_T1_Init_TMP1075(PBM_T1_I2C_PORT, pbm, PBM_Number, PBM_T1_TMP1075_ADDR_4, PBM_T1_I2C_MUX_CH_Temp2);

            }
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

