#include "stm32l4xx.h"
#include "INA238.h"
#include "TCA6424A.h"
#include "TCA9548.h"
#include "PAM/pam_config.h"

_PAM_table PAM__Table( uint8_t number_pwr_channel, uint8_t type_pwr_ch ) {

	_PAM_table pam_table = { 0 };

	//Config I2C MUX, same for all power channels
	pam_table.I2Cx_PORT = PAM_I2C_PORT;
	pam_table.I2C_addr_I2C_MUX_PWR = PAM_I2CADDR_I2C_MUX_1;
	pam_table.I2C_addr_GPIO_Ext = PAM_I2CADDR_GPIO_Ext;

	pam_table.PWR_Mon_Convr_Time = INA238_CONVERSION_TIME_1052us;
	pam_table.PWR_Mon_Aver_Count = INA238_AVERAGING_COUNT_128;
	pam_table.PWR_Mon_ADC_Range = INA238_ADC_RANGE_163mv;


	pam_table.pin_FLT_eF = 0;
	pam_table.pin_Enable_eF = 0;
	pam_table.I2C_addr_PWR_Mon = 0;
	pam_table.I2C_MUX_Ch = 0;

	if( type_pwr_ch == PAM_TYPE_PWR_CH_IN || type_pwr_ch == PAM_TYPE_PWR_CH_Supply ){
        switch( number_pwr_channel ){

            case PAM_PWR_IN_SPD_Ch1:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 3500; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 3.5f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_BUS_SHUNT_TEMPERATURE_CONTINUOUS;

                //Config Power monitor INA231 for power channel 1
                pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_SPD_Ch1;

                //Config I2C MUX for power channel 1
                pam_table.I2C_MUX_Ch = TCA9548_CH2;
                break;

            case PAM_PWR_IN_SPD_Ch2:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 3500; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 3.5f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

                //Config Power monitor INA231 for power channel 2
                pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_SPD_Ch2;

                //Config I2C MUX for power channel 2
                pam_table.I2C_MUX_Ch = TCA9548_CH1;
                break;

            case PAM_PWR_IN_SPD_Ch3:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 3500; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 3.5f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

                //Config Power monitor INA231 for power channel 3
                pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_SPD_Ch3;

                //Config I2C MUX for power channel 13
                pam_table.I2C_MUX_Ch = TCA9548_CH0;
                break;

            case PAM_PWR_IN_SPD_Ch4:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 3500; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 3.5f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_BUS_SHUNT_TEMPERATURE_CONTINUOUS;

                //Config Power monitor INA231 for power channel 4
                pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_SPD_Ch4;

                //Config I2C MUX for power channel 4
                pam_table.I2C_MUX_Ch = TCA9548_CH7;
                break;

            case PAM_PWR_IN_SPD_Ch5:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 3500; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 3.5f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

                //Config Power monitor INA231 for power channel 5 / LT_1
                pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_SPD_Ch5;

                //Config I2C MUX for power channel 5 / LT_1
                pam_table.I2C_MUX_Ch = TCA9548_CH6;
                break;

            case PAM_PWR_IN_SPD_Ch6:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 3500; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 3.5f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 15;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.015f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

                //Config Power monitor INA231 for power channel 6 / LT_2
                pam_table.I2C_addr_PWR_Mon = PAM_I2CADDR_PWR_Mon_SPD_Ch6;

                //Config I2C MUX for power channel 6 / LT_2
                pam_table.I2C_MUX_Ch = TCA9548_CH5;
                break;

            case PAM_PWR_IN_SPCC_Ch1:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 6000; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 6.0f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 10;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.010f;				//Rshunt value in Ohm
                pam_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

                //Config Power monitor INA231 for power channel 6 / LT_2
                pam_table.I2C_addr_PWR_Mon =  PAM_I2CADDR_PWR_Mon_SPCC_Ch1;

                //Config I2C MUX for power channel 6 / LT_2
                pam_table.I2C_MUX_Ch = TCA9548_CH3;
                break;

            case PAM_PWR_IN_SPCC_Ch2:
            	//Config Power monitor INA238, same for all power channels
            	//pam_table.PWR_Mon_Max_Current_int16 = 6000; 		//Max current in mA
            	pam_table.PWR_Mon_Max_Current_float = 6.0f; 			//Max current in A
            	//pam_table.PWR_Mon_Rshunt_int16 = 10;				//Rshunt value in mOhm
            	pam_table.PWR_Mon_Rshunt_float = 0.010f;				//Rshunt value in Ohm
            	pam_table.PWR_Mon_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

                //Config Power monitor INA231 for power channel 6 / LT_2
                pam_table.I2C_addr_PWR_Mon =  PAM_I2CADDR_PWR_Mon_SPCC_Ch2;

                //Config I2C MUX for power channel 6 / LT_2
                pam_table.I2C_MUX_Ch = TCA9548_CH4;
                break;

            case PAM_PWR_DC_DC:
                pam_table.pin_FLT_eF = TCA6424A_IO_P20;
                pam_table.pin_Enable_eF = TCA6424A_IO_P21;
                break;

            case PAM_PWR_LDO:
                pam_table.pin_FLT_eF = TCA6424A_IO_P22;
                pam_table.pin_Enable_eF = TCA6424A_IO_P23;
                break;

            default:
                break;
        }
    }else if( type_pwr_ch == PAM_TYPE_PWR_CH_TM_SP ){

        switch( number_pwr_channel ){
            case PAM_PWR_TM_SP_Ch1:
                //Config I2C GPIO Expander TCA6424A for power SP channel 1 in PAM
                pam_table.pin_FLT_eF = TCA6424A_IO_P10;
                pam_table.pin_Enable_eF = TCA6424A_IO_P00;
                break;

            case PAM_PWR_TM_SP_Ch2:
                //Config I2C GPIO Expander TCA6424A for power SP channel 2 in PAM
                pam_table.pin_FLT_eF = TCA6424A_IO_P11;
                pam_table.pin_Enable_eF = TCA6424A_IO_P01;
                break;

            case PAM_PWR_TM_SP_Ch3:
                //Config I2C GPIO Expander TCA6424A for power SP channel 3 in PAM
                pam_table.pin_FLT_eF = TCA6424A_IO_P12;
                pam_table.pin_Enable_eF = TCA6424A_IO_P02;
                break;

            case PAM_PWR_TM_SP_Ch4:
                //Config I2C GPIO Expander TCA6424A for power SP channel 4 in PAM
                pam_table.pin_FLT_eF = TCA6424A_IO_P13;
                pam_table.pin_Enable_eF = TCA6424A_IO_P03;
                break;

            case PAM_PWR_TM_SP_Ch5:
                //Config I2C GPIO Expander TCA6424A for power SP channel 5 in PAM
                pam_table.pin_FLT_eF = TCA6424A_IO_P14;
                pam_table.pin_Enable_eF = TCA6424A_IO_P04;
                break;

            case PAM_PWR_TM_SP_Ch6:
                //Config I2C GPIO Expander TCA6424A for power SP channel 6 in PAM
                pam_table.pin_FLT_eF = TCA6424A_IO_P15;
                pam_table.pin_Enable_eF = TCA6424A_IO_P05;
                break;

            case PAM_PWR_TM_SP_Ch7:
            	//Config I2C GPIO Expander TCA6424A for power SP channel 6 in PAM
            	pam_table.pin_FLT_eF = TCA6424A_IO_P16;
            	pam_table.pin_Enable_eF = TCA6424A_IO_P06;
            	break;

            case PAM_PWR_TM_SP_Ch8:
            	//Config I2C GPIO Expander TCA6424A for power SP channel 6 in PAM
            	pam_table.pin_FLT_eF = TCA6424A_IO_P17;
            	pam_table.pin_Enable_eF = TCA6424A_IO_P07;
            	break;

            default:
                break;
        }

	}

	return pam_table;
}


