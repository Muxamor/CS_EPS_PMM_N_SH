#include "stm32l4xx.h"
#include "INA231.h"
#include "TCA6424A.h"
#include "TCA9548.h"
#include "pam_config.h"

_PAM_table PAM__Table(uint8_t number_pwr_channel) {

	_PAM_table pam_table = { 0 };

	//Config I2C MUX, same for all power channels
	pam_table.I2Cx_PORT = PAM_I2C_PORT;
	pam_table.I2C_addr_I2C_MUX_PWR = PAM_I2CADDR_I2C_MUX_1;
	pam_table.I2C_addr_I2C_MUX_SP = PAM_I2CADDR_I2C_MUX_2;
	pam_table.I2C_addr_GPIO_Ext = PAM_I2CADDR_GPIO_Ext;

	//Config Power monitor INA231, same for all power channels
	pam_table.PWR_Mon_Max_Current_int16 = 3000; 		//Max current in mA
	pam_table.PWR_Mon_Max_Current_float = 3.0; 			//Max current in A
	pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
	pam_table.PWR_Mon_Rshunt_float = 0.015;				//Rshunt value in Ohm

	pam_table.PWR_Mon_aver_mode = INA231_AVERAGING_NUM_64; 		 		// Average 64
	pam_table.PWR_Mon_bus_conv_time = INA231_CONVERSION_TIME_1100us; 	// Conversion time 1.1ms
	pam_table.PWR_Mon_shunt_conv_time = INA231_CONVERSION_TIME_1100us; 	// Conversion time 1.1ms
	pam_table.PWR_Mon_work_mode = INA231_SHUNT_AND_BUS_CONTINUOUS; 		// Conversion time 1.1ms

	pam_table.pin_State_ID = 0;
	pam_table.pin_Enable_eF = 0;
	pam_table.I2C_addr_PWR_Mon = 0;
	pam_table.I2C_MUX_Ch = 0;

	switch (number_pwr_channel) {

	case PAM_PWR_IN_Channel_1:
		//Config I2C GPIO Expander TCA6424A for power channel 1
		pam_table.pin_State_ID = TCA6424A_IO_P10;			//Show State_ID_in_ch1 (U15)

		//Config Power monitor INA231 for power channel 1
		pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_Ch1;

		//Config I2C MUX for power channel 1
		pam_table.I2C_MUX_Ch = TCA9548_CH2;
		break;

	case PAM_PWR_IN_Channel_2:
		//Config I2C GPIO Expander TCA6424A for power channel 2
		pam_table.pin_State_ID = TCA6424A_IO_P11;			//Show State_ID_in_ch2 (U15)

		//Config Power monitor INA231 for power channel 2
		pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_Ch2;

		//Config I2C MUX for power channel 2
		pam_table.I2C_MUX_Ch = TCA9548_CH1;
		break;

	case PAM_PWR_IN_Channel_3:
		//Config I2C GPIO Expander TCA6424A for power channel 3
		pam_table.pin_State_ID = TCA6424A_IO_P12;			//Show State_ID_in_ch3 (U15)

		//Config Power monitor INA231 for power channel 3
		pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_Ch3;

		//Config I2C MUX for power channel 13
		pam_table.I2C_MUX_Ch = TCA9548_CH0;
		break;

	case PAM_PWR_IN_Channel_4:
		//Config I2C GPIO Expander TCA6424A for power channel 4
		pam_table.pin_State_ID = TCA6424A_IO_P13;			//Show State_ID_in_ch4 (U15)

		//Config Power monitor INA231 for power channel 4
		pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_Ch4;

		//Config I2C MUX for power channel 4
		pam_table.I2C_MUX_Ch = TCA9548_CH5;
		break;

	case PAM_PWR_IN_Channel_5:
		//Config I2C GPIO Expander TCA6424A for power channel 5 / LT_1
		pam_table.pin_State_ID = TCA6424A_IO_P14;			//Show State_ID_in_ch5 (U15)

		//Config Power monitor INA231 for power channel 5 / LT_1
		pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_Ch5;

		//Config I2C MUX for power channel 5 / LT_1
		pam_table.I2C_MUX_Ch = TCA9548_CH3;
		break;

	case PAM_PWR_IN_Channel_6:
		//Config I2C GPIO Expander TCA6424A for power channel 6 / LT_2
		pam_table.pin_State_ID = TCA6424A_IO_P15;			//Show State_ID_in_ch6 (U15)

		//Config Power monitor INA231 for power channel 6 / LT_2
		pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_Ch6;

		//Config I2C MUX for power channel 6 / LT_2
		pam_table.I2C_MUX_Ch = TCA9548_CH4;
		break;

	case PAM_PWR_DC_DC:
		//Config I2C GPIO Expander TCA6424A for power DC_DC
		pam_table.pin_State_ID = TCA6424A_IO_P20;			//Show State DC_DC (U15)
		pam_table.pin_Enable_eF = TCA6424A_IO_P00;			//Set State DC_DC (U15)

		break;

	case PAM_PWR_LDO:
		//Config I2C GPIO Expander TCA6424A for power LDO
		pam_table.pin_State_ID = TCA6424A_IO_P21;			//Show State LDO (U15)
		pam_table.pin_Enable_eF = TCA6424A_IO_P01;			//Set State LDO (U15)

		break;

	default:
		break;
	}

	return pam_table;
}


