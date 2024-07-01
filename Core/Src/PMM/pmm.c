#include "stm32l4xx.h"
#include "stm32l4xx_ll_iwdg.h"
#include "Error_Handler.h"
#include "SetupPeriph.h"
#include "stm32l4xx_ll_cortex.h"
#include "stm32l4xx_ll_tim.h"
#include "tim_pwm.h"
#include "PMM/pmm_config.h"
#include "PMM/pmm_struct.h"
#include "PMM/pmm_ctrl.h"
#include "PMM/pmm.h"


/** @brief  Get all telemetry data from PMM
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
ErrorStatus PMM_Get_Telemetry( _PMM *pmm_ptr ){

	int8_t error_status = SUCCESS;

	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANmain );
	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_CANbackup );
	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT1_eF );
	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_VBAT2_eF );
	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_PBMs_Logic );
	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Logic );
	error_status += PMM_Check_state_PWR_CH( pmm_ptr, PMM_PWR_Ch_Deploy_Power );

	error_status += PMM_Get_PG_all_PWR_CH( pmm_ptr );

	//Get value Current, Voltage, Power  of power channel VBAT1 and VBAT2.
	error_status += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT1_eF );
	//error_status += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT1_eF2 );
	error_status += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT2_eF );
	//error_status += PMM_Get_PWR_CH_VBAT_I_V_P( pmm_ptr, PMM_PWR_Ch_VBAT2_eF2 );

	//Get temperature value.
	error_status += PMM_Get_Temperature( pmm_ptr, PMM_I2Cx_TMP1075, PMM_I2CADDR_TMP1075 );

	//Get Current Power supply main and backup.
	//error_status += PMM_Get_PWR_Supply_m_b_I( pmm_ptr, PMM_I2Cx_PowerADC, PMM_I2CADDR_PowerADC);

	if(error_status != SUCCESS){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief  Set CPU speed ONLY FOR DEPLOY STAGE = 0 AND POWER MAIN CAN = DISABLE
	@param  *pmm_ptr - pointer to struct which contain all information about PMM.
    @param   speed_mode - CPU_Clock_16MHz or CPU_Clock_80MHz
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
void PMM_CPU_SPEED_MODE( _PMM *pmm_ptr, uint32_t speed_mode ){

    LL_IWDG_ReloadCounter(IWDG);
    I2C3_DeInit();
    I2C4_DeInit();
    LL_SYSTICK_DisableIT();
    PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
    PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);
    PWM_DeInit_Ch3_Ch4( );

    if( speed_mode == CPU_Clock_16MHz ){
        SystemClock_Config(CPU_Clock_16MHz);
    }else{
        SystemClock_Config(CPU_Clock_80MHz);
    }

    if( pmm_ptr->PWR_OFF_Passive_CPU == ENABLE ){
        PWM_Init_Ch3_Ch4(100000, 50, 0); //F=100kHz, Duty = 50%, tim divider=0
        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
        PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);
    }
    I2C3_Init();
    I2C4_Init();
    LL_SYSTICK_EnableIT();
    LL_IWDG_ReloadCounter(IWDG);
}
