
#include "stm32l4xx.h"
#include "PCA9534.h"
#include "pbm_config.h"


/** @brief	Fills the table of data about the selected module PBM
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
    @retval 	PBM table structure
 */
_PBM_table PBM_Table(uint8_t PBM_number) {

	_PBM_table pbm_table = { 0 };

	pbm_table.GPIO_INPUT_PIN = PCA9534_IO_P01 | PCA9534_IO_P02 | PCA9534_IO_P03 | PCA9534_IO_P04 | PCA9534_IO_P06 | PCA9534_IO_P07;

	switch (PBM_number) {

	case PBM_1:
		pbm_table.BRANCH_1_Addr = PBM_1_DS2777_ADDR_1;
		pbm_table.BRANCH_2_Addr = PBM_1_DS2777_ADDR_2;
		pbm_table.GPIO_Addr = PBM_1_PCA9534_ADDR;
		pbm_table.TEMP_SENSOR_1_Addr = PBM_1_TMP1075_ADDR_1;
		pbm_table.TEMP_SENSOR_2_Addr = PBM_1_TMP1075_ADDR_2;
		pbm_table.TEMP_SENSOR_3_Addr = PBM_1_TMP1075_ADDR_3;
		pbm_table.TEMP_SENSOR_4_Addr = PBM_1_TMP1075_ADDR_4;
		break;

	case PBM_2:
		pbm_table.BRANCH_1_Addr = PBM_2_DS2777_ADDR_1;
		pbm_table.BRANCH_2_Addr = PBM_2_DS2777_ADDR_2;
		pbm_table.GPIO_Addr = PBM_2_PCA9534_ADDR;
		pbm_table.TEMP_SENSOR_1_Addr = PBM_2_TMP1075_ADDR_1;
		pbm_table.TEMP_SENSOR_2_Addr = PBM_2_TMP1075_ADDR_2;
		pbm_table.TEMP_SENSOR_3_Addr = PBM_2_TMP1075_ADDR_3;
		pbm_table.TEMP_SENSOR_4_Addr = PBM_2_TMP1075_ADDR_4;
		break;

	case PBM_3:
		pbm_table.BRANCH_1_Addr = PBM_3_DS2777_ADDR_1;
		pbm_table.BRANCH_2_Addr = PBM_3_DS2777_ADDR_2;
		pbm_table.GPIO_Addr = PBM_3_PCA9534_ADDR;
		pbm_table.TEMP_SENSOR_1_Addr = PBM_3_TMP1075_ADDR_1;
		pbm_table.TEMP_SENSOR_2_Addr = PBM_3_TMP1075_ADDR_2;
		pbm_table.TEMP_SENSOR_3_Addr = PBM_3_TMP1075_ADDR_3;
		pbm_table.TEMP_SENSOR_4_Addr = PBM_3_TMP1075_ADDR_4;
		break;

	default:
		break;
	}

	return pbm_table;
}
