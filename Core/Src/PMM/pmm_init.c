#include "stm32l4xx.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "tim_pwm.h"
#include "PMM/pmm_struct.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_init_IC.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm_init.h"


//*******TO DO ******//
//1. Поудмать над тем чтобы было две функции инит плата PMM и блок CPU.

/** @brief  Initialisation PMM board
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N
*/
ErrorStatus PMM_init(_PMM *pmm_ptr){

	int8_t error_status = SUCCESS;

	//Init. Active CPU and PMM
	if( (pmm_ptr->Active_CPU == CPUmain_Active && pmm_ptr->Main_Backup_mode_CPU == CPUmain) || (pmm_ptr->Active_CPU == CPUbackup_Active && pmm_ptr->Main_Backup_mode_CPU == CPUbackup) ){

		//Powe oFF Passive CPU
        if(pmm_ptr->PWR_OFF_Passive_CPU == ENABLE){
        	PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0
            PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
            PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
        }else{
        	PWM_DeInit_Ch3_Ch4( );
            PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
            PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
        }

        ENABLE_TMUX1209_I2C();

        //Set CAN mux at active CPU
        PMM_Set_MUX_CAN_CPUm_CPUb( pmm_ptr );

        //Set state power channel PMM
		if( pmm_ptr->PWR_Ch_State_CANmain == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_CANbackup == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Vbat1_eF1 == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Vbat1_eF2 == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Vbat2_eF1 == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Vbat2_eF2 == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_PBMs_Logic == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_PBMs_Logic, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_PBMs_Logic, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Deploy_Logic == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Logic, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Logic, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_Deploy_Power == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Power, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Power, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_5V_Bus == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_5V_Bus, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_5V_Bus, DISABLE );
		}

		if( pmm_ptr->PWR_Ch_State_3_3V_Bus == ENABLE){
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_3_3V_Bus, ENABLE );
		}else{
			error_status += PMM_Set_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_3_3V_Bus, DISABLE );
		}


		error_status += PMM_init_TMP1075( pmm_ptr, PMM_I2Cx_TMP1075, PMM_I2CADDR_TMP1075 );

		error_status += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT1_eF1);
		error_status += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2);
		error_status += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT2_eF1);
		error_status += PMM_init_PWR_Mon_INA231( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2);
	
		error_status += ADS1015_init( pmm_ptr, PMM_I2Cx_PowerADC, PMM_I2CADDR_PowerADC);
	
	//Init Passive CPU and PMM
	}else{
		DISABLE_TMUX1209_I2C();
        PMM_DeInit_I2C_GPIOExt( pmm_ptr, PMM_I2Cx_GPIOExt1, PMM_I2CADDR_GPIOExt1);
		PMM_HARD_Reset_I2C_GPIOExt( PMM_I2CADDR_GPIOExt1); //If program reset doesn't work use HW reset

		error_status = PMM_Power_Down_TMP1075( pmm_ptr, PMM_I2Cx_TMP1075, PMM_I2CADDR_TMP1075);
	}

	if(error_status != SUCCESS){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	return SUCCESS;
}


/** @rief  Detect Main or Backup mode CPU block PMM .
	@param  None
	@retval 1 - CPU Main mode, 0 - CPU Backup mode.
 */
uint8_t PMM_Detect_MasterBackupCPU(void){

	uint8_t mode = 0;

	mode = LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_9)\
			+LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_10)\
			+LL_GPIO_IsInputPinSet(GPIOD, LL_GPIO_PIN_11);
	
	if( mode >= 2 ){
		return 0;  //Main mode CPU
	}else{
		return 1;  //Backup mode CPU
	}

}





