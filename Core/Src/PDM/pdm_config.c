#include "stm32l4xx.h"
#include "stm32l4xx_ll_utils.h"
#include "pdm_config.h"
#include "TCA9539.h"


_PDM_table PDM__Table(uint8_t number_pwr_channel){

	_PDM_table pdm_table = {0};

	switch(number_pwr_channel){

		case PDM_PWR_Channel_1:
			
			pdm_table.I2Cx = PDM_I2Cx_GPIOExt1;
			pdm_table.I2C_tca9539_addr = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch1 (U19)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P02; 		//EN_eFusee_out_ch1 (U19)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P01;			//PG_eFusee_in_ch1 (U19)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P03; 		//PG_eFusee_out_ch1 (U19)
			break;

		case PDM_PWR_Channel_2:

			pdm_table.I2Cx = PDM_I2Cx_GPIOExt2;
			pdm_table.I2C_tca9539_addr = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P00;			//EN_eFusee_in_ch2 (U20)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P02;			//EN_eFusee_out_ch2 (U20)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P01;			//PG_eFusee_in_ch2 (U20)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P03; 		//PG_eFusee_out_ch2 (U20)
			break;

		case PDM_PWR_Channel_3:

			pdm_table.I2Cx = PDM_I2Cx_GPIOExt1;
			pdm_table.I2C_tca9539_addr = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P05;			//EN_eFusee_in_ch3 (U19)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P07; 		//EN_eFusee_out_ch3 (U19)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P06;			//PG_eFusee_in_ch3 (U19)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P10; 		//PG_eFusee_out_ch3 (U19)
			break;

		case PDM_PWR_Channel_4:

			pdm_table.I2Cx = PDM_I2Cx_GPIOExt2;
			pdm_table.I2C_tca9539_addr = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P05;			//EN_eFusee_in_ch4 (U20)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P07;			//EN_eFusee_out_ch4 (U20)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P06;			//PG_eFusee_in_ch4 (U20)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P10; 		//PG_eFusee_out_ch4 (U20)
			break;

		case PDM_PWR_Channel_5:

			pdm_table.I2Cx = PDM_I2Cx_GPIOExt1;
			pdm_table.I2C_tca9539_addr = PDM_I2CADDR_GPIOExt1;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch5 (U19)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P14;			//EN_eFusee_out_ch5 (U19)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P13;			//PG_eFusee_in_ch5 (U19)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P15; 		//PG_eFusee_out_ch5 (U19)
			break;

		case PDM_PWR_Channel_6:

			pdm_table.I2Cx = PDM_I2Cx_GPIOExt2;
			pdm_table.I2C_tca9539_addr = PDM_I2CADDR_GPIOExt2;
			pdm_table.pin_EN_in_eFuse = TCA9539_IO_P12;			//EN_eFusee_in_ch6 (U20)
			pdm_table.pin_EN_out_eFuse = TCA9539_IO_P14;			//EN_eFusee_out_ch6 (U20)
			pdm_table.pin_PG_in_eFuse = TCA9539_IO_P13;			//PG_eFusee_in_ch6 (U20)
			pdm_table.pin_PG_out_eFuse = TCA9539_IO_P15; 		//PG_eFusee_out_ch6 (U20)
			break;

		default:
			break;
	}

	return pdm_table;

}
	
