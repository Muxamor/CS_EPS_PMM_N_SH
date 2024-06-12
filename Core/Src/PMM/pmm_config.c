#include "stm32l4xx.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "INA238.h"
#include "PMM/pmm_config.h"

_PMM_table PMM__Table(uint8_t number_pwr_channel){

	_PMM_table pmm_table = {0};

	//Config Power monitor INA231, same for all power channels
	pmm_table.PWR_Mon_Max_Current_int16 = 6400; 		//Max current in mA
	pmm_table.PWR_Mon_Max_Current_float = 6.4f;			//Max current in A
	pmm_table.PWR_Mon_Rshunt_int16 = 7;					//Rshunt value in mOhm
	pmm_table.PWR_Mon_Rshunt_float = 0.007f;			//Rshunt value in Ohm

	pmm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
	pmm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
	pmm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
	pmm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

	switch(number_pwr_channel){

		case PMM_PWR_Ch_CANmain:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P04;
			break;

		case PMM_PWR_Ch_CANbackup:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P14;
			break;

		case PMM_Reset_CANmain:
		    pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
		    pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P00;
			break;

		case PMM_Reset_CANbackup:
		    pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
		    pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P10;
			break;

		case PMM_PWR_Ch_VBAT1_eF:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P06;
			pmm_table.I2Cx_PWR_Mon = PMM_I2Cx_VBAT1_eF_INA238;
			pmm_table.I2C_addr_PWR_Mon = PMM_I2CADDR_VBAT1_eF_INA238;
			break;

		case PMM_PWR_Ch_VBAT2_eF:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P15;
			pmm_table.I2Cx_PWR_Mon = PMM_I2Cx_VBAT2_eF_INA238;
			pmm_table.I2C_addr_PWR_Mon = PMM_I2CADDR_VBAT2_eF_INA238;
			break;

		case PMM_PWR_Ch_PBMs_Logic:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P01;
			break;

		case PMM_PWR_Ch_Deploy_Logic:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P12;
			break;

		case PMM_PWR_Ch_Deploy_Power:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P02;
			break;

	    case PMM_PWR_Deploy_Ch1:
            pmm_table.I2Cx_GPIO_Ext =  PMM_I2Cx_DeployGPIOExt;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_DeployGPIOExt;
            pmm_table.pin_GPIO_Ext = PCA9534_IO_P00;
//            pmm_table.I2Cx_Deploy_ADC = ;
//            pmm_table.I2C_addr_Deploy_ADC = ;
//            pmm_table.Deploy_number_ch_ADC = ;
            break;

	    case PMM_PWR_Deploy_Ch2:
            pmm_table.I2Cx_GPIO_Ext =  PMM_I2Cx_DeployGPIOExt;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_DeployGPIOExt;
            pmm_table.pin_GPIO_Ext = PCA9534_IO_P01;
            break;

        case PMM_PWR_Deploy_Ch3:
            pmm_table.I2Cx_GPIO_Ext =  PMM_I2Cx_DeployGPIOExt;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_DeployGPIOExt;
            pmm_table.pin_GPIO_Ext = PCA9534_IO_P02;
            break;

        case PMM_PWR_Deploy_Ch4:
            pmm_table.I2Cx_GPIO_Ext =  PMM_I2Cx_DeployGPIOExt;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_DeployGPIOExt;
            pmm_table.pin_GPIO_Ext = PCA9534_IO_P03;
            break;

		default:
			break;
	}

	return pmm_table;
}
