#include "stm32l4xx.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "INA231.h"
#include "PMM/pmm_config.h"

_PMM_table PMM__Table(uint8_t number_pwr_channel){

	_PMM_table pmm_table = {0};

	//Config Power monitor INA231, same for all power channels
	pmm_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
	pmm_table.PWR_Mon_Max_Current_float = 3.0f; 			//Max current in A
	pmm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm 
	pmm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

	pmm_table.PWR_Mon_aver_mode = INA231_AVERAGING_NUM_16; 		 // Average 16
	pmm_table.PWR_Mon_bus_conv_time = INA231_CONVERSION_TIME_1100us; // Conversion time 1.1ms
	pmm_table.PWR_Mon_shunt_conv_time = INA231_CONVERSION_TIME_1100us; // Conversion time 1.1ms
	pmm_table.PWR_Mon_work_mode = INA231_SHUNT_AND_BUS_CONTINUOUS; // Conversion time 1.1ms

	switch(number_pwr_channel){

		case PMM_PWR_Ch_CANmain:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P17;
			break;

		case PMM_PWR_Ch_CANbackup:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P15;
			break;

		case PMM_PWR_Ch_VBAT1_eF1:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P00;
			pmm_table.I2Cx_PWR_Mon = PMM_I2Cx_VBAT1_eF1_INA231;
			pmm_table.I2C_addr_PWR_Mon = PMM_I2CADDR_VBAT1_eF1_INA231;
			break;

		case PMM_PWR_Ch_VBAT1_eF2:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P01;
			pmm_table.I2Cx_PWR_Mon = PMM_I2Cx_VBAT1_eF2_INA231;
			pmm_table.I2C_addr_PWR_Mon = PMM_I2CADDR_VBAT1_eF2_INA231;
			break;

		case PMM_PWR_Ch_VBAT2_eF1:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P02;
			pmm_table.I2Cx_PWR_Mon = PMM_I2Cx_VBAT2_eF1_INA231;
			pmm_table.I2C_addr_PWR_Mon = PMM_I2CADDR_VBAT2_eF1_INA231;
			break;

		case PMM_PWR_Ch_VBAT2_eF2:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P03;
			pmm_table.I2Cx_PWR_Mon = PMM_I2Cx_VBAT2_eF2_INA231;
			pmm_table.I2C_addr_PWR_Mon = PMM_I2CADDR_VBAT2_eF2_INA231;
			break;

		case PMM_PWR_Ch_PBMs_Logic:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P04;
			break;

		case PMM_PWR_Ch_Deploy_Logic:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P10;
			break;

		case PMM_PWR_Ch_Deploy_Power:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P11;
			break;

		case PMM_PWR_Ch_5V_Bus:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P05;
			break;

		case PMM_PWR_Ch_3_3V_Bus:
            pmm_table.I2Cx_GPIO_Ext = PMM_I2Cx_GPIOExt1;
            pmm_table.I2C_addr_GPIO_Ext = PMM_I2CADDR_GPIOExt1;
			pmm_table.pin_GPIO_Ext = TCA9539_IO_P06;
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
