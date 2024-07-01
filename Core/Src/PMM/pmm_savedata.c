#include <string.h>
#include "stm32l4xx_ll_utils.h"
#include "stm32l4xx_ll_iwdg.h"
#include "SetupPeriph.h"
#include "FRAM.h"
#include "Fn_CRC16.h"
#include "uart_eps_comm.h"
#include "Error_Handler.h"
#include "PMM/pmm_init.h"
#include "PMM/pmm_config.h"
#include "PBM_T1/pbm_T1_control.h"
#include "PMM/pmm_damage_ctrl.h"
#include "PMM/pmm_savedata.h"
#include  <stdio.h>


extern _UART_EPS_COMM *UART_M_eps_comm;
extern _UART_EPS_COMM *UART_B_eps_comm;


/** @brief	Save data EPS to a FRAM
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	i2c_addr_fram1 - I2C address FRAM  number 1
	@param 	i2c_addr_fram2 - I2C address FRAM  number 2
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_FRAM_write_data( I2C_TypeDef *I2Cx_fram1, I2C_TypeDef *I2Cx_fram2, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, _EPS_Param eps_p ) {

	int8_t error_status = SUCCESS;
    uint16_t crc_calc = 0;
    uint32_t i = 0;
    _FRAM_SAVE_DATA fram_data_write = {0};
	uint8_t *fram_data_write_ptr;
	uint8_t PBM_Number = 0, Branch_Number = 0, Heat_Number = 0;

	// ------- Filling of data to write in FRAM -------- //
	//PMM
    fram_data_write.FRAM_PMM_PWR_Ch_State_CANmain       = eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain;
    fram_data_write.FRAM_PMM_PWR_Ch_State_CANbackup     = eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup;
    fram_data_write.FRAM_PMM_PWR_Ch_State_Vbat1_eF     = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF;
   // fram_data_write.FRAM_PMM_PWR_Ch_State_Vbat1_eF2	    = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2;
    fram_data_write.FRAM_PMM_PWR_Ch_State_Vbat2_eF     = eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF;
   // fram_data_write.FRAM_PMM_PWR_Ch_State_Vbat2_eF2 	= eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2;
    fram_data_write.FRAM_PMM_PWR_Ch_State_PBMs_Logic    = eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic;
    fram_data_write.FRAM_PMM_PWR_Ch_State_Deploy_Logic 	= eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic;
   // fram_data_write.FRAM_PMM_PWR_Ch_State_Deploy_Power 	= eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Power;
    //fram_data_write.FRAM_PMM_PWR_Ch_State_5V_Bus 		= eps_p.eps_pmm_ptr->PWR_Ch_State_5V_Bus;
    //fram_data_write.FRAM_PMM_PWR_Ch_State_3_3V_Bus 		= eps_p.eps_pmm_ptr->PWR_Ch_State_3_3V_Bus;
    //fram_data_write.FRAM_PMM_PWR_Ch_State_I2C_Bus 		= eps_p.eps_pmm_ptr->PWR_Ch_State_I2C_Bus;
    fram_data_write.FRAM_PMM_PWR_OFF_Passive_CPU        = eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU;
    fram_data_write.FRAM_PMM_Active_CPU		 		    = eps_p.eps_pmm_ptr->Active_CPU;
    fram_data_write.FRAM_PMM_CAN_constatnt_mode		    = eps_p.eps_pmm_ptr->CAN_constatnt_mode;
    fram_data_write.FRAM_PMM_Deploy_Lim_SW_Exit_1       = eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1;
    fram_data_write.FRAM_PMM_Deploy_Lim_SW_Exit_2       = eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2;
    fram_data_write.FRAM_PMM_Deploy_Ch4_Lim_SW_1        = eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1;
    fram_data_write.FRAM_PMM_Deploy_Ch4_Lim_SW_2        = eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2;
    fram_data_write.FRAM_PMM_Deploy_Ch3_Lim_SW_1        = eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1;
    fram_data_write.FRAM_PMM_Deploy_Ch3_Lim_SW_2        = eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2;
    fram_data_write.FRAM_PMM_Deploy_Ch1_Lim_SW_1        = eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1;
    fram_data_write.FRAM_PMM_Deploy_Ch1_Lim_SW_2        = eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2;
    fram_data_write.FRAM_PMM_Deploy_Ch2_Lim_SW_1        = eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1;
    fram_data_write.FRAM_PMM_Deploy_Ch2_Lim_SW_2        = eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2;

    fram_data_write.FRAM_PMM_reboot_counter_CPUm        = eps_p.eps_pmm_ptr->reboot_counter_CPUm;
    fram_data_write.FRAM_PMM_reboot_counter_CPUb        = eps_p.eps_pmm_ptr->reboot_counter_CPUb;

    fram_data_write.FRAM_PMM_EPS_Mode                   = eps_p.eps_pmm_ptr->EPS_Mode;
    fram_data_write.FRAM_PMM_Deploy_stage               = eps_p.eps_pmm_ptr->Deploy_stage;

    //PDM
    fram_data_write.FRAM_PDM_PDM_ID_module = eps_p.eps_pdm_ptr->PDM_ID_module;

    for( i = 0; i < PDM_PWR_Ch_quantity; i++){
        fram_data_write.FRAM_PDM_PWR_Ch[i].State_eF  = eps_p.eps_pdm_ptr->PWR_Channel[i].State_eF;
    }

    //PAM
    fram_data_write.FRAM_PAM_State_DC_DC = eps_p.eps_pam_ptr->State_DC_DC;
    fram_data_write.FRAM_PAM_State_LDO = eps_p.eps_pam_ptr->State_LDO;
    fram_data_write.FRAM_PAM_PAM_ID_module              = eps_p.eps_pam_ptr->PAM_ID_module;
    for( i = 0; i < PAM_PWR_TM_SP_Ch_quantity; i++ ){
        fram_data_write.FRAM_PAM_PWR_Ch_TM_SP[i].State_eF_out = eps_p.eps_pam_ptr->PWR_Channel_TM_SP[i].State_eF_out;
    }

    //PBM
    for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++  ){
    	for(Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
    		fram_data_write.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].DchgEnableCmd = eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].DchgEnableCmd;
    		fram_data_write.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].ChgEnableCmd = eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].ChgEnableCmd;
    		fram_data_write.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Cmd = eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Cmd;
    		fram_data_write.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Cmd = eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Cmd;
    	}
    	for(Heat_Number = 0; Heat_Number < PBM_T1_HEAT_QUANTITY; Heat_Number++){
    		fram_data_write.FRAM_PBM_T1[PBM_Number].Heat[Heat_Number].PCA9534_ON_Heat_CMD = eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_Number].PCA9534_Heat_CMD;
    	}
    }
    // -------------------------------------- //

    fram_data_write_ptr = (uint8_t*)(&fram_data_write);

    // ------- Calc CRC data -------- //
    crc_calc = Crc16_1021(  fram_data_write_ptr, sizeof(fram_data_write) - 2 );
    fram_data_write.FRAM_DATA_CRC = crc_calc;
    // ------------------------------ //

    FRAM_control_write_access(FRAM_WRITE_PROTECTION_DISABLE);

	if( FRAM_triple_verif_write_data(I2Cx_fram1, i2c_addr_fram1, fram_data_write_ptr, sizeof(fram_data_write)) != SUCCESS){
		eps_p.eps_pmm_ptr->Error_FRAM1 = ERROR;
        error_status = error_status + ERROR_N;
	}else{
        eps_p.eps_pmm_ptr->Error_FRAM1 = SUCCESS;
        error_status = error_status + SUCCESS;
	}

	if( FRAM_triple_verif_write_data(I2Cx_fram2, i2c_addr_fram2, fram_data_write_ptr, sizeof(fram_data_write)) != SUCCESS){
        eps_p.eps_pmm_ptr->Error_FRAM2 = ERROR;
        error_status = error_status + ERROR_N;
	}else{
        eps_p.eps_pmm_ptr->Error_FRAM2 = SUCCESS;
        error_status = error_status + SUCCESS;
	}

    FRAM_control_write_access(FRAM_WRITE_PROTECTION_ENABLE);

	if( error_status != SUCCESS){
       return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Read data EPS from a FRAM
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	i2c_addr_fram1 - I2C address FRAM  number 1
	@param 	i2c_addr_fram2 - I2C address FRAM  number 2
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_FRAM_read_data( I2C_TypeDef *I2Cx_fram1, I2C_TypeDef *I2Cx_fram2, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, _EPS_Param eps_p ) {

    _FRAM_SAVE_DATA fram_data_read = {0};
    uint8_t *fram_data_read_ptr;
    uint32_t i = 0;
	uint8_t PBM_Number = 0, Branch_Number = 0, Heat_Number = 0;

    fram_data_read_ptr = (uint8_t*)(&fram_data_read);

    //Read data from FRAM1
    if( FRAM_majority_read_data(I2Cx_fram1, i2c_addr_fram1, fram_data_read_ptr, sizeof(fram_data_read)) == SUCCESS ){
        //if( FRAM_majority_read_data_two_fram(I2Cx_fram1, i2c_addr_fram1, i2c_addr_fram2, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != SUCCESS ){
        if( FRAM_Check_Data_CRC( fram_data_read_ptr, sizeof(fram_data_read) - 2, fram_data_read.FRAM_DATA_CRC ) == SUCCESS  ){
            eps_p.eps_pmm_ptr->Error_FRAM1 = SUCCESS;
        }else{
            eps_p.eps_pmm_ptr->Error_FRAM1 = ERROR;
        }
    }else{
        eps_p.eps_pmm_ptr->Error_FRAM1 = ERROR;
    }


    if( eps_p.eps_pmm_ptr->Error_FRAM1 != SUCCESS ){
        LL_mDelay(20);

        if( FRAM_majority_read_data(I2Cx_fram2, i2c_addr_fram2, fram_data_read_ptr, sizeof(fram_data_read)) == SUCCESS ){
            //if( FRAM_majority_read_data_two_fram(I2Cx_fram1, i2c_addr_fram1, i2c_addr_fram2, (uint8_t*)(&fram_struct_read), sizeof(fram_struct_read)) != SUCCESS ){
            if( FRAM_Check_Data_CRC( fram_data_read_ptr, sizeof(fram_data_read) - 2, fram_data_read.FRAM_DATA_CRC ) == SUCCESS  ){
                eps_p.eps_pmm_ptr->Error_FRAM2 = SUCCESS;
            }else{
                eps_p.eps_pmm_ptr->Error_FRAM2 = ERROR;
            }
        }else{
            eps_p.eps_pmm_ptr->Error_FRAM2 = ERROR;
        }
    }


	if( eps_p.eps_pmm_ptr->Error_FRAM1 == SUCCESS || eps_p.eps_pmm_ptr->Error_FRAM2 == SUCCESS ){

        // ------- Filling EPS structure from FRAM -------- //
        //PMM
        eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain = fram_data_read.FRAM_PMM_PWR_Ch_State_CANmain;
        eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup = fram_data_read.FRAM_PMM_PWR_Ch_State_CANbackup;
        eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF = fram_data_read.FRAM_PMM_PWR_Ch_State_Vbat1_eF;
       // eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat1_eF2 = fram_data_read.FRAM_PMM_PWR_Ch_State_Vbat1_eF2;
        eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF = fram_data_read.FRAM_PMM_PWR_Ch_State_Vbat2_eF;
        //eps_p.eps_pmm_ptr->PWR_Ch_State_Vbat2_eF2 = fram_data_read.FRAM_PMM_PWR_Ch_State_Vbat2_eF2;
        eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic = fram_data_read.FRAM_PMM_PWR_Ch_State_PBMs_Logic;
        eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Logic = fram_data_read.FRAM_PMM_PWR_Ch_State_Deploy_Logic;
        // eps_p.eps_pmm_ptr->PWR_Ch_State_Deploy_Power    = fram_data_read.FRAM_PMM_PWR_Ch_State_Deploy_Power;
        //eps_p.eps_pmm_ptr->PWR_Ch_State_5V_Bus = fram_data_read.FRAM_PMM_PWR_Ch_State_5V_Bus;
        //eps_p.eps_pmm_ptr->PWR_Ch_State_3_3V_Bus = fram_data_read.FRAM_PMM_PWR_Ch_State_3_3V_Bus;
        //eps_p.eps_pmm_ptr->PWR_Ch_State_I2C_Bus = fram_data_read.FRAM_PMM_PWR_Ch_State_I2C_Bus;
        eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU = fram_data_read.FRAM_PMM_PWR_OFF_Passive_CPU;
        eps_p.eps_pmm_ptr->Active_CPU = fram_data_read.FRAM_PMM_Active_CPU;
        eps_p.eps_pmm_ptr->CAN_constatnt_mode = fram_data_read.FRAM_PMM_CAN_constatnt_mode;
        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 = fram_data_read.FRAM_PMM_Deploy_Lim_SW_Exit_1;
        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 = fram_data_read.FRAM_PMM_Deploy_Lim_SW_Exit_2;
        eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_1 = fram_data_read.FRAM_PMM_Deploy_Ch4_Lim_SW_1;
        eps_p.eps_pmm_ptr->Deploy_Ch4_Lim_SW_2 = fram_data_read.FRAM_PMM_Deploy_Ch4_Lim_SW_2;
        eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_1 = fram_data_read.FRAM_PMM_Deploy_Ch3_Lim_SW_1;
        eps_p.eps_pmm_ptr->Deploy_Ch3_Lim_SW_2 = fram_data_read.FRAM_PMM_Deploy_Ch3_Lim_SW_2;
        eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_1 = fram_data_read.FRAM_PMM_Deploy_Ch1_Lim_SW_1;
        eps_p.eps_pmm_ptr->Deploy_Ch1_Lim_SW_2 = fram_data_read.FRAM_PMM_Deploy_Ch1_Lim_SW_2;
        eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_1 = fram_data_read.FRAM_PMM_Deploy_Ch2_Lim_SW_1;
        eps_p.eps_pmm_ptr->Deploy_Ch2_Lim_SW_2 = fram_data_read.FRAM_PMM_Deploy_Ch2_Lim_SW_2;

        eps_p.eps_pmm_ptr->reboot_counter_CPUm = fram_data_read.FRAM_PMM_reboot_counter_CPUm;
        eps_p.eps_pmm_ptr->reboot_counter_CPUb = fram_data_read.FRAM_PMM_reboot_counter_CPUb;

        eps_p.eps_pmm_ptr->EPS_Mode = fram_data_read.FRAM_PMM_EPS_Mode;
        eps_p.eps_pmm_ptr->Deploy_stage = fram_data_read.FRAM_PMM_Deploy_stage;

        //PDM
        eps_p.eps_pdm_ptr->PDM_ID_module = fram_data_read.FRAM_PDM_PDM_ID_module;

        for( i = 0; i < PDM_PWR_Ch_quantity; i++ ){
            eps_p.eps_pdm_ptr->PWR_Channel[i].State_eF = fram_data_read.FRAM_PDM_PWR_Ch[i].State_eF;
        }

        //PAM
        eps_p.eps_pam_ptr->State_DC_DC =  fram_data_read.FRAM_PAM_State_DC_DC;
        eps_p.eps_pam_ptr->State_LDO =  fram_data_read.FRAM_PAM_State_LDO;
        eps_p.eps_pam_ptr->PAM_ID_module = fram_data_read.FRAM_PAM_PAM_ID_module;
        for( i = 0; i < PAM_PWR_TM_SP_Ch_quantity; i++ ){
            eps_p.eps_pam_ptr->PWR_Channel_TM_SP[i].State_eF_out = fram_data_read.FRAM_PAM_PWR_Ch_TM_SP[i].State_eF_out;
        }

        //PBM
        for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++  ){
        	for(Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].DchgEnableCmd = fram_data_read.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].DchgEnableCmd;
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].ChgEnableCmd = fram_data_read.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].ChgEnableCmd;
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Cmd = fram_data_read.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Cmd;
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Cmd = fram_data_read.FRAM_PBM_T1[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Cmd;
        	}
        	for(Heat_Number = 0; Heat_Number < PBM_T1_HEAT_QUANTITY; Heat_Number++){
        		eps_p.eps_pbm_ptr[PBM_Number].Heat[Heat_Number].PCA9534_Heat_CMD = fram_data_read.FRAM_PBM_T1[PBM_Number].Heat[Heat_Number].PCA9534_ON_Heat_CMD;
        	}
        }

    }


    if( eps_p.eps_pmm_ptr->Error_FRAM1 == ERROR || eps_p.eps_pmm_ptr->Error_FRAM2 == ERROR ){
        return ERROR_N;
    }
    return SUCCESS;
}


/** @brief	Restore settings from FRAM
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_FRAM_Restore_Settings ( _EPS_Param eps_p ){

    int8_t /*I2C_Error_FRAM1 = 0, I2C_Error_FRAM2 = 0,*/ error_status = SUCCESS;
   // uint8_t FRAM1_status = 0, FRAM2_status = 0; // 0 - FRAM empty, 1 - FRAM no empty
	uint8_t i = 0;
    /*
     * It is not importent feature.
     * Remove because we can get error I2C FRAM when CPU is booting. (meeans nois power)
        I2C_Error_FRAM1 = ERROR_N; //0-OK -1-ERROR_N
        while( ( I2C_Error_FRAM1 != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

            I2C_Error_FRAM1 = FRAM_Detect_Empty(PMM_I2Cx_FRAM1, PMM_I2CADDR_FRAM1, FRAM_SIZE_64KB, &FRAM1_status );

            if( I2C_Error_FRAM1 != SUCCESS ){
                i++;
                LL_mDelay( pmm_i2c_delay_att_conn );
            }
        }

        if( I2C_Error_FRAM1 == ERROR_N){
            #ifdef DEBUGprintf
                printf("I2C_Error_FRAM1 == ERROR_N\n");
            #endif
            I2C3_DeInit();
            LL_mDelay( 500 );
            I2C3_Init();
            LL_mDelay( 400 );
        }

            I2C_Error_FRAM2 = ERROR_N; //0-OK -1-ERROR_N
            while( ( I2C_Error_FRAM2 != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

                I2C_Error_FRAM2 = FRAM_Detect_Empty(PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM2, FRAM_SIZE_64KB, &FRAM2_status );

                if( I2C_Error_FRAM2 != SUCCESS ){
                    i++;
                    LL_mDelay( pmm_i2c_delay_att_conn );
                }
            }

            if( I2C_Error_FRAM1 != SUCCESS){
                eps_p.eps_pmm_ptr->Error_FRAM1 = ERROR;
                #ifdef DEBUGprintf
                    printf("I2C_Error_FRAM1 == ERROR_N\n");
                #endif
            }else{
                eps_p.eps_pmm_ptr->Error_FRAM1 = SUCCESS;
            }

            if( I2C_Error_FRAM2 != SUCCESS){
                eps_p.eps_pmm_ptr->Error_FRAM2 = ERROR;
                #ifdef DEBUGprintf
                    printf("I2C_Error_FRAM2 == ERROR_N\n");
                #endif
            }else{
                eps_p.eps_pmm_ptr->Error_FRAM2 = SUCCESS;
            } */

    //if( ((FRAM1_status == 1) && ( I2C_Error_FRAM1 == SUCCESS)) || ((FRAM2_status == 1) && ( I2C_Error_FRAM2 == SUCCESS)) ){

        error_status = ERROR_N; //0-OK -1-ERROR_N
        while( ( error_status != SUCCESS ) && ( i < pmm_i2c_attempt_conn ) ){//Enable/Disable INPUT Efuse power channel.

            error_status = PMM_FRAM_read_data(PMM_I2Cx_FRAM1, PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM1, PMM_I2CADDR_FRAM2, eps_p);

            if( error_status != SUCCESS ){
                i++;
                LL_mDelay( pmm_i2c_delay_att_conn );
                if( i >= pmm_i2c_attempt_conn - 1 ){
                    I2C3_DeInit();
                    LL_mDelay( 700 );
                    I2C3_Init();
                    LL_mDelay( 700 );
                }
            }
        }
   // }


    if( /*I2C_Error_FRAM1 == ERROR_N || I2C_Error_FRAM2 == ERROR_N ||*/  error_status != SUCCESS ){
        #ifdef DEBUGprintf
            printf("ERROR I2C In Restore settings function\n");
        #endif
        return  ERROR_N;
    }

    return  SUCCESS;
}


/** @brief	Restore settings from Neighbor СPU
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_Settings_From_NeighborCPU ( _EPS_Param eps_p ){

    int8_t error_status = SUCCESS;
    uint8_t i = 0;
    uint16_t size_struct = 0;
    uint8_t PBM_Number = 0, Branch_Number = 0;
    _PDM pdm_temp = {0}, *pdm_ptr_temp = &pdm_temp;
    _PMM pmm_temp = {0}, *pmm_ptr_temp = &pmm_temp;
    _PAM pam_temp = {0}, *pam_ptr_temp = &pam_temp;
    _PBM_T1 pbm_mas_temp[PBM_T1_QUANTITY] = {0};

    _EPS_Param eps_param_temp = { .eps_pmm_ptr = pmm_ptr_temp,
                                  .eps_pdm_ptr = pdm_ptr_temp,
                                  .eps_pam_ptr = pam_ptr_temp,
                                  .eps_pbm_ptr = pbm_mas_temp,
                                };

    i=0;
    error_status = ERROR_N;
    while( (  error_status != SUCCESS ) && ( i < 3 ) ){

    	eps_param_temp.eps_pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_Main_Backup_CPU();
        error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_PMM_struct, 0, UART_M_eps_comm, UART_B_eps_comm,  eps_param_temp);

        if( error_status != SUCCESS ){
            LL_IWDG_ReloadCounter(IWDG);
            i++;
            LL_mDelay( 500 );
			#ifdef DEBUGprintf
            	Error_Handler();
    		#endif
        }
    }

    if( ( error_status == SUCCESS ) && ( eps_param_temp.eps_pmm_ptr->Error_FRAM1 == SUCCESS || eps_param_temp.eps_pmm_ptr->Error_FRAM2 == SUCCESS ) ){

//        if( (eps_param_temp.eps_pmm_ptr->Active_CPU == CPUmain_Active && eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain ) ||
//                (eps_param_temp.eps_pmm_ptr->Active_CPU == CPUbackup_Active && eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){

        eps_param_temp.eps_pmm_ptr->Main_Backup_mode_CPU = PMM_Detect_Main_Backup_CPU();
        eps_param_temp.eps_pmm_ptr->PWR_OFF_Passive_CPU = DISABLE; //just in case
        eps_param_temp.eps_pmm_ptr->Error_CAN_port_M = SUCCESS;
        eps_param_temp.eps_pmm_ptr->Error_CAN_port_B = SUCCESS;
        eps_param_temp.eps_pmm_ptr->Error_UART_port_M = SUCCESS;
        eps_param_temp.eps_pmm_ptr->Error_UART_port_B = SUCCESS;
        size_struct = sizeof( pmm_temp );
        memcpy( eps_p.eps_pmm_ptr, eps_param_temp.eps_pmm_ptr, size_struct );

        i=0;
        error_status = ERROR_N;
        while( (  error_status != SUCCESS ) && ( i < 3 ) ){

            error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_PAM_struct, 0, UART_M_eps_comm, UART_B_eps_comm,  eps_param_temp);

            if( error_status != SUCCESS ){
                LL_IWDG_ReloadCounter(IWDG);
                i++;
                LL_mDelay( 500 );
			    #ifdef DEBUGprintf
                    Error_Handler();
    		    #endif
            }
        }

        if( error_status == SUCCESS ){
            size_struct = sizeof( pam_temp );
            memcpy( eps_p.eps_pam_ptr, eps_param_temp.eps_pam_ptr, size_struct );
        }

        i=0;
        error_status = ERROR_N;
        while( (  error_status != SUCCESS ) && ( i < 3 ) ){

            error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_PDM_struct, 0, UART_M_eps_comm, UART_B_eps_comm,  eps_param_temp);

            if( error_status != SUCCESS ){
                LL_IWDG_ReloadCounter(IWDG);
                i++;
                LL_mDelay( 500 );
			    #ifdef DEBUGprintf
                    Error_Handler();
    		    #endif
            }
        }

        if( error_status == SUCCESS ){
            size_struct = sizeof( pdm_temp );
            memcpy( eps_p.eps_pdm_ptr, eps_param_temp.eps_pdm_ptr, size_struct );
        }

        i=0;
        error_status = ERROR_N;
        while( (  error_status != SUCCESS ) && ( i < 3 ) ){

            error_status = UART_EPS_Send_CMD(UART_EPS_ID_CMD_Get_PBM_struct, 0, UART_M_eps_comm, UART_B_eps_comm,  eps_param_temp);

            if( error_status != SUCCESS ){
                LL_IWDG_ReloadCounter(IWDG);
                i++;
                LL_mDelay( 500 );
			    #ifdef DEBUGprintf
                Error_Handler();
    		    #endif
            }
        }

        if( error_status == SUCCESS ){
            size_struct = sizeof( pbm_mas_temp );
            memcpy( eps_p.eps_pbm_ptr, eps_param_temp.eps_pbm_ptr, size_struct );
        }


        eps_p.eps_pmm_ptr->PMM_save_conf_flag = SET;
    }

    if( error_status != SUCCESS ){

        eps_p.eps_pmm_ptr->PWR_Ch_State_CANmain = ENABLE;
        eps_p.eps_pmm_ptr->PWR_Ch_State_CANbackup = ENABLE;
        eps_p.eps_pmm_ptr->PWR_Ch_State_PBMs_Logic = ENABLE;
        eps_p.eps_pmm_ptr->Deploy_stage = 0;
        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_1 = 0;
        eps_p.eps_pmm_ptr->Deploy_Lim_SW_Exit_2 = 0;

        //Enable All BRK
        eps_p.eps_pdm_ptr->PWR_Channel[PDM_PWR_Channel_3].State_eF = ENABLE;
        eps_p.eps_pdm_ptr->PWR_Channel[PDM_PWR_Channel_4].State_eF = ENABLE;

        //PBM
        for( PBM_Number = 0; PBM_Number < PBM_T1_QUANTITY; PBM_Number++  ){
            for(Branch_Number = 0; Branch_Number < PBM_T1_BRANCH_QUANTITY; Branch_Number++){
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].DchgEnableCmd = ENABLE;
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].ChgEnableCmd = ENABLE;
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].PCA9534_Emerg_Chrg_Cmd = ENABLE;
                eps_p.eps_pbm_ptr[PBM_Number].Branch[Branch_Number].Auto_Corr_Capacity_Cmd = DISABLE;
            }
        }

       	#ifdef DEBUGprintf
            Error_Handler();
        #endif
    }

    return  SUCCESS;
}


/** @brief	Save of setting to FRAM for Active and Passive CPU and  Synchronization of settings from Active to Passive CPU.
	@param  eps_p - contain pointer to struct which contain all parameters EPS.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Sync_and_Save_Settings_A_P_CPU( _EPS_Param eps_p ){

    int8_t error_status = SUCCESS;

    #ifdef DEBUGprintf
        printf("Enter in PMM_Sync_and_Save_Settings_A_P_CPU\n");
    #endif

    //Check flag save settings for Active and Passive CPU
    if( (eps_p.eps_pmm_ptr->PMM_save_conf_flag == SET) || ( eps_p.eps_pdm_ptr->PDM_save_conf_flag == SET) || ( eps_p.eps_pam_ptr->PAM_save_conf_flag == SET) || (PBM_T1_CheckSaveSetupFlag( eps_p.eps_pbm_ptr ) == SET)){

        //Sending (sync) settings from Active to Passive CPU
        if( eps_p.eps_pmm_ptr->PWR_OFF_Passive_CPU == DISABLE ){
            if((eps_p.eps_pmm_ptr->Active_CPU == CPUmain_Active && eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (eps_p.eps_pmm_ptr->Active_CPU == CPUbackup_Active && eps_p.eps_pmm_ptr->Main_Backup_mode_CPU == CPUbackup)){

                //Check Errors UART ports and get reboot counter passive CPU.
                PMM_Damage_Check_UART_m_b_ActiveCPU(UART_M_eps_comm, UART_B_eps_comm, eps_p);

                if( eps_p.eps_pmm_ptr->PMM_save_conf_flag == SET ){
                    error_status += UART_EPS_Send_CMD(UART_EPS_ID_CMD_SAVE_PMM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_p);
                }

                if( eps_p.eps_pdm_ptr->PDM_save_conf_flag == SET ){
                    error_status += UART_EPS_Send_CMD(UART_EPS_ID_CMD_SAVE_PDM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_p);
                }

                if( eps_p.eps_pam_ptr->PAM_save_conf_flag == SET ){
                    error_status += UART_EPS_Send_CMD(UART_EPS_ID_CMD_SAVE_PAM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_p);
                }

                if( PBM_T1_CheckSaveSetupFlag(eps_p.eps_pbm_ptr) == SET ){
                    error_status += UART_EPS_Send_CMD(UART_EPS_ID_CMD_SAVE_PBM_struct, 0, UART_M_eps_comm, UART_B_eps_comm, eps_p);
                }
            }
        }

        //Save setting to FRAM for Active and Passive CPU
        error_status += PMM_FRAM_write_data(PMM_I2Cx_FRAM1, PMM_I2Cx_FRAM2, PMM_I2CADDR_FRAM1, PMM_I2CADDR_FRAM2, eps_p);

        eps_p.eps_pmm_ptr->PMM_save_conf_flag = RESET;
        eps_p.eps_pdm_ptr->PDM_save_conf_flag = RESET;
        eps_p.eps_pam_ptr->PAM_save_conf_flag = RESET;
        PBM_T1_ClearSaveSetupFlag( eps_p.eps_pbm_ptr );
    }

    if( error_status != SUCCESS ){
        #ifdef DEBUGprintf
            printf("Error I2C in save settings\n");
        #endif
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief  Check CRC data from FRAM
	@param  *data - pointer to data
    @param  data_size - data size
    @param  expected_crc - expected CRC
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus FRAM_Check_Data_CRC( uint8_t *data, uint16_t data_size, uint16_t expected_crc ){

    uint16_t crc_calc = 0;
    int8_t error_status = ERROR_N;

    crc_calc = Crc16_1021( data, data_size );

    if( crc_calc == expected_crc ){
        error_status = SUCCESS;
    }else{
        error_status = ERROR_N;
    }

    return error_status;
}
