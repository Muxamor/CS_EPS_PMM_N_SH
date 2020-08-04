#include "stm32l4xx.h"
#include "TCA9539.h"
#include "TCA9548.h"
#include "INA231.h"
#include "PDM/pdm_config.h"

_PDM_table PDM__Table(uint8_t number_pwr_channel){

	_PDM_table pdm_table = {0};

	//Config I2C MUX, same for all power channels
	pdm_table.I2Cx_I2C_MUX = PDM_I2Cx_I2C_MUX;
	pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX;

	//Config Power monitor INA231, same for all power channels
	pdm_table.PWR_Mon_Max_Current_int16 = 2000; 		//Max current in mA 
	pdm_table.PWR_Mon_Max_Current_float = 2.0f; 			//Max current in A
	pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm 
	pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

	pdm_table.PWR_Mon_aver_mode = INA231_AVERAGING_NUM_64; 		 // Average 64
	pdm_table.PWR_Mon_bus_conv_time = INA231_CONVERSION_TIME_1100us; // Conversion time 1.1ms
	pdm_table.PWR_Mon_shunt_conv_time = INA231_CONVERSION_TIME_1100us; // Conversion time 1.1ms
	pdm_table.PWR_Mon_work_mode = INA231_SHUNT_AND_BUS_CONTINUOUS; // Conversion time 1.1ms



	switch(number_pwr_channel){

		case PDM_PWR_Channel_1:
			//Config I2C GPIO Expander TCA9539 for power channel 1
			pdm_table.I2Cx_GPIO_Ext = PDM_I2Cx_GPIOExt1;
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch1 (U19)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P02; 		//EN_eFusee_out_ch1 (U19)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P01;			//PG_eFusee_in_ch1 (U19)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P03; 		//PG_eFusee_out_ch1 (U19)

			//Config Power monitor INA231 for power channel 1
			pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PWR_Mon_Ch1;
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch1;

			//Config I2C MUX for power channel 1
			pdm_table.I2C_MUX_Ch = TCA9548_CH5;
			break;

		case PDM_PWR_Channel_2:
			//Config I2C GPIO Expander TCA9539 for power channel 2
			pdm_table.I2Cx_GPIO_Ext = PDM_I2Cx_GPIOExt2;
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch2 (U20)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P02;		//EN_eFusee_out_ch2 (U20)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P01;			//PG_eFusee_in_ch2 (U20)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P03; 		//PG_eFusee_out_ch2 (U20)

			//Config Power monitor INA231 for power channel 2
			pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PWR_Mon_Ch2;
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch2;

			//Config I2C MUX for power channel 2
			pdm_table.I2C_MUX_Ch = TCA9548_CH4;
			break;

		case PDM_PWR_Channel_3:
			//Config I2C GPIO Expander TCA9539 for power channel 3
			pdm_table.I2Cx_GPIO_Ext = PDM_I2Cx_GPIOExt1;
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P05;			//EN_eFusee_in_ch3 (U19)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P07; 		//EN_eFusee_out_ch3 (U19)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P06;			//PG_eFusee_in_ch3 (U19)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P10; 		//PG_eFusee_out_ch3 (U19)

			//Config Power monitor INA231 for power channel 3
			pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PWR_Mon_Ch3;
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch3;

			//Config I2C MUX for power channel 3
			pdm_table.I2C_MUX_Ch = TCA9548_CH3;
			break;

		case PDM_PWR_Channel_4:
			//Config I2C GPIO Expander TCA9539 for power channel 4
			pdm_table.I2Cx_GPIO_Ext = PDM_I2Cx_GPIOExt2;
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P05;			//EN_eFusee_in_ch4 (U20)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P07;		//EN_eFusee_out_ch4 (U20)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P06;			//PG_eFusee_in_ch4 (U20)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P10; 		//PG_eFusee_out_ch4 (U20)

			//Config Power monitor INA231 for power channel 4
			pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PWR_Mon_Ch4;
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch4;

			//Config I2C MUX for power channel 4
			pdm_table.I2C_MUX_Ch = TCA9548_CH2;
			break;

		case PDM_PWR_Channel_5:
			//Config I2C GPIO Expander TCA9539 for power channel 5
			pdm_table.I2Cx_GPIO_Ext = PDM_I2Cx_GPIOExt1;
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch5 (U19)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P14;		//EN_eFusee_out_ch5 (U19)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P13;			//PG_eFusee_in_ch5 (U19)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P15; 		//PG_eFusee_out_ch5 (U19)

			//Config Power monitor INA231 for power channel 5
			pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PWR_Mon_Ch5;
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch5;

			//Config I2C MUX for power channel 5
			pdm_table.I2C_MUX_Ch = TCA9548_CH1;
			break;

		case PDM_PWR_Channel_6:
			//Config I2C GPIO Expander TCA9539 for power channel 6
			pdm_table.I2Cx_GPIO_Ext = PDM_I2Cx_GPIOExt2;
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch6 (U20)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P14;		//EN_eFusee_out_ch6 (U20)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P13;			//PG_eFusee_in_ch6 (U20)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P15; 		//PG_eFusee_out_ch6 (U20)

			//Config Power monitor INA231 for power channel 6
			pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PWR_Mon_Ch6;
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch6;

			//Config I2C MUX for power channel 6
			pdm_table.I2C_MUX_Ch = TCA9548_CH0;
			break;

		default:
			break;
	}

	return pdm_table;
}
	
