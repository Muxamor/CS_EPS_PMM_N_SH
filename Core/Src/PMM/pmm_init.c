
#include "stm32l4xx.h"
#include "stm32l4xx_ll_gpio.h"
#include "pmm_struct.h"
#include "pmm_init.h"


/** @rief  Inicialization PMM module (include IC ) .
	@param  None
	@retval 1 - CPU Main mode, 0 - CPU Backup mode.
 */
void Init_PMM(_PMM *pmm){

	pmm->Main_Backup_mode_CPU = PMM_Detect_MasterBackupCPU();

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
		return 1;  //Main mode CPU
	} else{
		return 0;  //Backup mode CPU
	}

}






