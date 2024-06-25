#include "stm32l4xx.h"
#include "TCA9539.h"
#include "TCA9548.h"
#include "INA238.h"
#include "PDM/pdm_config.h"

_PDM_table PDM__Table(uint8_t number_pwr_channel){

	_PDM_table pdm_table = {0};

	pdm_table.I2Cx_PORT = PDM_I2Cx_PORT;
	//pdm_table.I2Cx_I2C_MUX = PDM_I2Cx_PORT;
	//pdm_table.I2Cx_PWR_Mon = PDM_I2Cx_PORT;

	switch(number_pwr_channel){

		case PDM_PWR_Channel_1:

			//Config I2C GPIO Expander TCA9539 for power channel 1
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX1;
			//Config I2C MUX for power channel 1
			pdm_table.I2C_MUX_Ch = TCA9548_CH0;

			//Config I2C GPIO Expander TCA9539 for power channel
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch1 (U25)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P01;			//PG_eFusee_in_ch1 (U25)

			//Config Power monitor INA238 for power channel 1
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch1;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 4000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 4.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_2:
			//Config I2C GPIO Expander TCA9539 for power channel 2
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX2;
			//Config I2C MUX for power channel 1
			pdm_table.I2C_MUX_Ch = TCA9548_CH0;

			//Config I2C GPIO Expander TCA9539 for power channel 2
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch2 (U20)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P01;			//PG_eFusee_in_ch2 (U20)

			//Config Power monitor INA238 for power channel 2
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch2;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 4000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 4.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_3:
			//Config I2C GPIO Expander TCA9539 for power channel 3
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX1;
			//Config I2C MUX for power channel 1
			pdm_table.I2C_MUX_Ch = TCA9548_CH1;

			//Config I2C GPIO Expander TCA9539 for power channel 3
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P02;			//EN_eFusee_in_ch1 (U25)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P03;			//PG_eFusee_in_ch1 (U25)

			//Config Power monitor INA238 for power channel 3
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch3;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 4000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 4.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_4:
			//Config I2C GPIO Expander TCA9539 for power channel 4
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX2;
			//Config I2C MUX for power channel 1
			pdm_table.I2C_MUX_Ch = TCA9548_CH1;

			//Config I2C GPIO Expander TCA9539 for power channel 2
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P02;			//EN_eFusee_in_ch2 (U20)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P03;			//PG_eFusee_in_ch2 (U20)

			//Config Power monitor INA231 for power channel 2
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch4;

			//Config Power monitor INA231, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 4000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 4.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_5:
			//Config I2C GPIO Expander TCA9539 for power channel 5
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX1;
			//Config I2C MUX for power channel 5
			pdm_table.I2C_MUX_Ch = TCA9548_CH2;

			//Config I2C GPIO Expander TCA9539 for power channel 5
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P04;			//EN_eFusee_in_ch1 (U25)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P05;			//PG_eFusee_in_ch1 (U25)

			//Config Power monitor INA238 for power channel 5
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch5;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 4000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 4.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_6:
			//Config I2C GPIO Expander TCA9539 for power channel 6
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX2;
			//Config I2C MUX for power channel 6
			pdm_table.I2C_MUX_Ch = TCA9548_CH2;

			//Config I2C GPIO Expander TCA9539 for power channel 6
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P04;			//EN_eFusee_in_ch6 (U20)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P05;			//PG_eFusee_in_ch6 (U20)

			//Config Power monitor INA238 for power channel 2
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch6;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 4000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 4.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_7:
			//Config I2C GPIO Expander TCA9539 for power channel 7
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX1;
			//Config I2C MUX for power channel 7
			pdm_table.I2C_MUX_Ch = TCA9548_CH3;

			//Config I2C GPIO Expander TCA9539 for power channel 7
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P06;			//EN_eFusee_in_ch7 (U25)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P07;			//PG_eFusee_in_ch7 (U25)

			//Config Power monitor INA238 for power channel 7
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch7;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 3.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_8:
			//Config I2C GPIO Expander TCA9539 for power channel 8 wired channel
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX2;
			//Config I2C MUX for power channel 8
			pdm_table.I2C_MUX_Ch = TCA9548_CH3;

			//Config I2C GPIO Expander TCA9539 for power channel 8
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P06;			//EN_eFusee_in_ch8 (U20)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P07;			//PG_eFusee_in_ch8 (U20)

			//Config Power monitor INA238 for power channel 2
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch8;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 3.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 20;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.020f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_40mv;//INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_9:
			//Config I2C GPIO Expander TCA9539 for power channel 9 wired channel
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX1;
			//Config I2C MUX for power channel 9
			pdm_table.I2C_MUX_Ch = TCA9548_CH4;

			//Config I2C GPIO Expander TCA9539 for power channel 9
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P10;			//EN_eFusee_in_ch9 (U25)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P11;			//PG_eFusee_in_ch9 (U25)

			//Config Power monitor INA238 for power channel 9
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch9;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 3.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 20;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.020f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_40mv;//INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_10:
			//Config I2C GPIO Expander TCA9539 for power channel 10 wired channel
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX2;
			//Config I2C MUX for power channel 10
			pdm_table.I2C_MUX_Ch = TCA9548_CH4;

			//Config I2C GPIO Expander TCA9539 for power channel 10
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P10;			//EN_eFusee_in_ch10 (U20)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P11;			//PG_eFusee_in_ch10 (U20)

			//Config Power monitor INA238 for power channel 10
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch10;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 3.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 20;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.020f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_40mv;//INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_11:
			//Config I2C GPIO Expander TCA9539 for power channel 11 wired channel
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX1;
			//Config I2C MUX for power channel 11
			pdm_table.I2C_MUX_Ch = TCA9548_CH5;

			//Config I2C GPIO Expander TCA9539 for power channel 11
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch11 (U25)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P13;			//PG_eFusee_in_ch11 (U25)

			//Config Power monitor INA238 for power channel 11
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch11;

			//Config Power monitor INA238, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 3.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 20;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.020f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_40mv;//INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;

		case PDM_PWR_Channel_12:
			//Config I2C GPIO Expander TCA9539 for power channel 12 High current channel
			pdm_table.I2C_addr_I2C_MUX = PDM_I2CADDR_I2C_MUX2;
			//Config I2C MUX for power channel 12
			pdm_table.I2C_MUX_Ch = TCA9548_CH5;

			//Config I2C GPIO Expander TCA9539 for power channel 12
			pdm_table.I2C_addr_GPIO_Ext = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch12 (U20)
			pdm_table.pin_PG_eFuse = TCA9539_IO_P13;			//PG_eFusee_in_ch12 (U20)

			//Config Power monitor INA231 for power channel 12
			pdm_table.I2C_addr_PWR_Mon = PDM_I2CADDR_PWR_Mon_Ch12;

			//Config Power monitor INA231, same for all power channels
			pdm_table.PWR_Mon_Max_Current_int16 = 20000; 		//Max current in mA
			pdm_table.PWR_Mon_Max_Current_float = 20.0f; 			//Max current in A
			pdm_table.PWR_Mon_Rshunt_int16 = 3;				//Rshunt value in mOhm
			pdm_table.PWR_Mon_Rshunt_float = 0.003f;				//Rshunt value in Ohm

			pdm_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
			pdm_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
			pdm_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;
			pdm_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;
			break;
		default:
			break;
	}

	return pdm_table;
}
	
