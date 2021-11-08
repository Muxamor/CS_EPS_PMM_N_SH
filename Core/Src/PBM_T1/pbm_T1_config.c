#include "stm32l4xx.h"
#include "PCA9534.h"
#include "TCA9548.h"
#include "PBM_T1/pbm_T1_config.h"


/** @brief	Fills the table of data about the selected module PBM
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
    @retval 	PBM table structure
 */
_PBM_T1_table PBM_T1_Table(uint8_t PBM_number) {

	_PBM_T1_table pbm_T1_table = { 0 };

	pbm_T1_table.GPIO_INPUT_PIN = PCA9534_IO_P01 | PCA9534_IO_P04 | PCA9534_IO_P06 | PCA9534_IO_P07;

	pbm_T1_table.INA_1_Addr = PBM_T1_INA_ADDR_1;
	pbm_T1_table.INA_2_Addr = PBM_T1_INA_ADDR_2;

	pbm_T1_table.TEMP_SENSOR_1_Addr = PBM_T1_TMP1075_ADDR_1;
	pbm_T1_table.TEMP_SENSOR_2_Addr = PBM_T1_TMP1075_ADDR_2;
	pbm_T1_table.TEMP_SENSOR_3_Addr = PBM_T1_TMP1075_ADDR_3;
	pbm_T1_table.TEMP_SENSOR_4_Addr = PBM_T1_TMP1075_ADDR_4;

	pbm_T1_table.GPIO_Addr = PBM_T1_PCA9534_ADDR;

	switch (PBM_number) {

	case PBM_T1_1:
		pbm_T1_table.I2C_MUX_Addr = PBM_T1_1_TCA9548A_ADDR;
		break;

	case PBM_T1_2:
		pbm_T1_table.I2C_MUX_Addr = PBM_T1_2_TCA9548A_ADDR;
		break;

	case PBM_T1_3:
		pbm_T1_table.I2C_MUX_Addr = PBM_T1_3_TCA9548A_ADDR;
		break;

	case PBM_T1_4:
		pbm_T1_table.I2C_MUX_Addr = PBM_T1_4_TCA9548A_ADDR;
		break;

	default:
		break;
	}

	return pbm_T1_table;
}
