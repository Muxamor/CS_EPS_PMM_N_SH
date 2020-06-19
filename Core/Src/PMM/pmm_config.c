#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "TCA9539.h"
#include "INA231.h"
#include "pmm_struct.h"
#include "pmm_config.h"

_PMM_table PMM__Table(uint8_t number_pwr_channel){

	_PMM_table pmm_table = {0};

	pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
	pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;

	switch(number_pwr_channel){

		case PMM_PWR_Ch_CANmain:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P17;
			break;

		case PMM_PWR_Ch_CANbackup:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P15;
			break;

		case PMM_PWR_Ch_VBAT1_eF1:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P00;
			break;

		case PMM_PWR_Ch_VBAT1_eF2:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P01;
			break;

		case PMM_PWR_Ch_VBAT2_eF1:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P02;
			break;

		case PMM_PWR_Ch_VBAT2_eF2:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P03;
			break;

		case PMM_PWR_Ch_PBMs_Logic:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P04;
			break;

		case PMM_PWR_Ch_Deploy_Logic:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P10;
			break;

		case PMM_PWR_Ch_Deploy_Power:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P11;
			break;

		case PMM_PWR_Ch_5V_Bus:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P05;
			break;

		case PMM_PWR_Ch_3_3V_Bus:
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P06;
			break;

		default:
			break;
	}

	return pmm_table;
}
