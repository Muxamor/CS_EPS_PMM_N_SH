#include "stm32l4xx.h"
#include "PCA9534.h"
#include "TCA9548.h"
#include "INA238.h"
#include "TMP1075.h"
#include "PBM_T1/pbm_T1_config.h"


/** @brief	Fills the table of data about the selected module PBM
    @param 	PBM_number - select PBM (PBM_1, PBM_2, PBM_3 or PBM_ALL).
    @param 	Branch - select Branch (PBM_T1_Branch_1, PBM_T1_Branch_2).
    @param 	Heat - select number heat (PBM_T1_HEAT_1, PBM_T1_HEAT_2).
    @retval 	PBM table structure
 */
_PBM_T1_table PBM_T1_Table(uint8_t PBM_number, uint8_t Branch, uint8_t Heat) {

	_PBM_T1_table pbm_T1_table = { 0 };

	pbm_T1_table.GPIO_Addr = PBM_T1_PCA9534_ADDR;

	pbm_T1_table.I2C_MUX_Ch_GPIO = PBM_T1_I2C_MUX_CH_GPIO;
	//pbm_T1_table.I2C_MUX_Ch_TempSens = PBM_T1_I2C_MUX_CH_TEMPSENS;

	//pbm_T1_table.TEMP_SENSOR_Addr[0] = ;

	pbm_T1_table.INA238_Convr_Time = INA238_CONVERSION_TIME_1052us;
	pbm_T1_table.INA238_Aver_Count = INA238_AVERAGING_COUNT_128;
	pbm_T1_table.INA238_ADC_Range = INA238_ADC_RANGE_163mv;
	pbm_T1_table.INA238_Mode = INA238_SHUNT_AND_BUS_CONTINUOUS;

	pbm_T1_table.TMP1075_Mode = TMP1075_CONTINUOUS_CONV;
	pbm_T1_table.TMP1075_Alert_Level = TMP1075_ACTIVE_HIGH;
	pbm_T1_table.TMP1075_Mode_Alert = TMP1075_COMPARATOR_MODE;
	pbm_T1_table.TMP1075_Convr_Time = TMP1075_CR_MEDIUM;
	pbm_T1_table.TMP1075_Alert_Sens = TMP1075_TWO_FAULT;

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

	case PBM_T1_5:
		pbm_T1_table.I2C_MUX_Addr = PBM_T1_5_TCA9548A_ADDR;
		break;

	default:
		break;
	}

	switch (Branch) {

	case PBM_T1_BRANCH_1:
		pbm_T1_table.GPIO_Pin_EmergChrg = PBM_T1_GPIO_CH_EmergChrg_1;
		pbm_T1_table.I2C_MUX_Ch_Branch = PBM_T1_I2C_MUX_CH_Br1;
		break;

	case PBM_T1_BRANCH_2:
		/*pbm_T1_table.GPIO_Pin_EmergChrg = PBM_T1_GPIO_CH_EmergChrg_2;
		pbm_T1_table.I2C_MUX_Ch_Branch = PBM_T1_I2C_MUX_CH_Br2;*/
		break;

	default:
			break;
	}

	switch (Heat) {

	case PBM_T1_HEAT_1:
		pbm_T1_table.GPIO_Pin_Heat_CMD = PBM_T1_GPIO_CH_Heat_CMD_1;
		pbm_T1_table.GPIO_Pin_Heat_State = PBM_T1_GPIO_CH_Heat_State_1;
		pbm_T1_table.TempSens_Heat_Addr[PBM_T1_HEAT_TEMPSENS_1] = PBM_T1_TMP1075_HEAT_1_ADDR_1;
		pbm_T1_table.TempSens_Heat_Addr[PBM_T1_HEAT_TEMPSENS_2] = PBM_T1_TMP1075_HEAT_1_ADDR_2;
		pbm_T1_table.I2C_MUX_Ch_Heat = PBM_T1_I2C_MUX_CH_Heat1;
		pbm_T1_table.I2C_MUX_Ch_PwrMon = PBM_T1_I2C_MUX_CH_PwrMon1;
		pbm_T1_table.PwrMon_Addr = PBM_T1_INA238_ADDR_1;
		break;

	case PBM_T1_HEAT_2:
		pbm_T1_table.GPIO_Pin_Heat_CMD = PBM_T1_GPIO_CH_Heat_CMD_2;
		pbm_T1_table.GPIO_Pin_Heat_State = PBM_T1_GPIO_CH_Heat_State_2;
		pbm_T1_table.TempSens_Heat_Addr[PBM_T1_HEAT_TEMPSENS_1] = PBM_T1_TMP1075_HEAT_2_ADDR_1;
		pbm_T1_table.TempSens_Heat_Addr[PBM_T1_HEAT_TEMPSENS_2] = PBM_T1_TMP1075_HEAT_2_ADDR_2;
		pbm_T1_table.I2C_MUX_Ch_Heat = PBM_T1_I2C_MUX_CH_Heat2;
		pbm_T1_table.I2C_MUX_Ch_PwrMon = PBM_T1_I2C_MUX_CH_PwrMon2;
		pbm_T1_table.PwrMon_Addr = PBM_T1_INA238_ADDR_2;
		break;

	default:
			break;
	}

	return pbm_T1_table;
}
