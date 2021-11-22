#include "stm32l4xx.h"
#include "PCA9534.h"
#include "TCA9548.h"
#include "PBM_T1/pbm_T1_config.h"


/** @brief	Fills the table of data about the selected module PBM
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
    @param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2).
    @param 	Heat - select number heat (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval 	PBM table structure
 */
_PBM_T1_table PBM_T1_Table(uint8_t PBM_number, uint8_t Branch, uint8_t Heat) {

	_PBM_T1_table pbm_T1_table = { 0 };

	pbm_T1_table.GPIO_INPUT_PIN = PCA9534_IO_P01 | PCA9534_IO_P04 | PCA9534_IO_P06 | PCA9534_IO_P07;
	pbm_T1_table.GPIO_OUTPUT_PIN = PCA9534_IO_P00|PCA9534_IO_P02|PCA9534_IO_P03|PCA9534_IO_P05;

	pbm_T1_table.INA238_1_Addr = PBM_T1_INA238_ADDR_1;
	pbm_T1_table.INA238_2_Addr = PBM_T1_INA238_ADDR_2;

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

	switch (Branch) {

	case PBM_T1_BRANCH_1:
		pbm_T1_table.GPIO_Pin_EmergChrg = PBM_T1_GPIO_CH_EmergChrg_1;
		pbm_T1_table.GPIO_Pin_Shift_EmergChrg = PBM_T1_GPIO_CH_Shift_EmergChrg_1;
		break;

	case PBM_T1_BRANCH_2:
		pbm_T1_table.GPIO_Pin_EmergChrg = PBM_T1_GPIO_CH_EmergChrg_2;
		pbm_T1_table.GPIO_Pin_Shift_EmergChrg = PBM_T1_GPIO_CH_Shift_EmergChrg_2;
		break;

	default:
			break;
	}

	switch (Heat) {

	case PBM_T1_HEAT_1:
		pbm_T1_table.GPIO_Pin_Heat_CMD = PBM_T1_GPIO_CH_Heat_CMD_1;
		pbm_T1_table.GPIO_Pin_Shift_Heat_CMD = PBM_T1_GPIO_CH_Shift_Heat_CMD_1;
		pbm_T1_table.GPIO_Pin_Heat_State = PBM_T1_GPIO_CH_Heat_State_1;
		pbm_T1_table.GPIO_Pin_Shift_Heat_State = PBM_T1_GPIO_CH_Shift_Heat_State_1;
		pbm_T1_table.TEMP_SENSOR_1_Addr = PBM_T1_TMP1075_ADDR_1;
		pbm_T1_table.TEMP_SENSOR_2_Addr = PBM_T1_TMP1075_ADDR_2;
		break;

	case PBM_T1_HEAT_2:
		pbm_T1_table.GPIO_Pin_Heat_CMD = PBM_T1_GPIO_CH_Heat_CMD_2;
		pbm_T1_table.GPIO_Pin_Shift_Heat_CMD = PBM_T1_GPIO_CH_Shift_Heat_CMD_2;
		pbm_T1_table.GPIO_Pin_Heat_State = PBM_T1_GPIO_CH_Heat_State_2;
		pbm_T1_table.GPIO_Pin_Shift_Heat_State = PBM_T1_GPIO_CH_Shift_Heat_State_2;
		pbm_T1_table.TEMP_SENSOR_1_Addr = PBM_T1_TMP1075_ADDR_3;
		pbm_T1_table.TEMP_SENSOR_2_Addr = PBM_T1_TMP1075_ADDR_4;
		break;

	default:
			break;
	}

	return pbm_T1_table;
}
