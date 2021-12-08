#ifndef INC_PBM_T1_STRUCT_H_
#define INC_PBM_T1_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PBM_T1/pbm_T1_config.h"

typedef struct  // Structure for PBM data
{
	uint16_t OVP :1; 						// Overvoltage faults.
	uint16_t UVP :1; 						// Undervoltage faults.
	uint16_t OCCP :1; 						// Overcharge current faults.
	uint16_t ODCP :1; 						// Overdischarge current faults.
	uint16_t BalCell1 :1;					// Flag balancing cell1.
	uint16_t BalCell2 :1;					// Flag balancing cell2.
	uint16_t DchgEnableBit :1; 				// Discharge Enable Bit must be set 1 to allow the ON state. // Need to save in flash.
	uint16_t ChgEnableBit :1; 				// Charge Enable Bit must be set 1 to allow the ON state. // Need to save in flash.
	uint16_t DchgControlFlag :1; 			// Discharge Control Flag indicates the real logic state of the DC pin driver.
	uint16_t ChgControlFlag :1; 			// Charge Control Flag indicates the logic state of the CC pin driver.
	uint16_t PreqF :1; 						// Prequal timeout faults.
	uint16_t LDet :1; 						// Leakage Detection Fault. This bit is set when a leakage detection fault has been detected.
	uint16_t OC :1;
	uint16_t OD :1;
	uint16_t SC :1;
	uint16_t PreqChrg :1;

	uint16_t PCA9534_Emerg_Chrg :1; 		// PCA9534 port. Show state emergency charge.
	uint16_t Counter_Reset_PreqF :8; 		// Counter for reset MAX17320 if exist prequal timer error.
	uint16_t Error_Charge :1; 				// Register error charge branch.
	uint16_t Error_Discharge :1; 			// Register error discharge branch.
	uint16_t Error_Emerg_Chrg :1; 			// Register error emergency charge branch №1.
	uint16_t Error_MAX17320 :1; 			// Register error MAX17320.
	uint16_t :3; 							// Reserve.

	int16_t Voltage [PBM_T1_BRANCH_BAT_QUANTITY];// Register voltage of high battery (on 8.4V) in mV.
	uint16_t MaxVCELL; 						// MaxVCELL value. Maximum channel voltage reading (20mV resolution).
	uint16_t MinVCELL; 						// MinVCELL value. Minimum VCell register reading (20mV resolution).
	int16_t Current;						// Register charge/discharge current level over the preceding 351ms in mA.
	int16_t AverageCurrent;					// Register average charge/discharge current level over the preceding ?s in mA.
	int16_t MaxCurr;						// MaxCurr value. Maximum Current register reading (0.40mV resolution).
	int16_t MinCurr;						// MinCurr value. Minimum Current register reading (0.40mV resolution).
	int16_t Power;							// Register power level over the preceding 351ms in mW.
	int16_t AveragePower;					// Register average power level over the preceding ?s in mW.
	int8_t Temperature;						// Register temperature of MAX17320 in °C.
	uint8_t RelativeCapacity_Perc;			// Register active relative Capacity in %.
	uint16_t AbcoluteCapacity_mAh;			// Register accumulate charge of battery's in mAh.
	uint8_t AgeScalar;						// Register adjusts the cell capacity estimation in percentage value of the application’s present cell capacity.
	uint8_t CycleCounter;					// Register cycle counter is an absolute count of the discharge cycles.
	uint16_t RCell;						 	// RCell value. The register displays the calculated internal resistance of the cell or the average internal resistance.
	uint16_t LeakCurr;						// Leakage Current value. Leakage current is an unsigned 8-bit result of leakage current from self-discharge in a cell.


} _T1_Branch_Data;

typedef struct  // Structure for heat data
{
	int8_t Heat_TMP1075 [PBM_T1_HEAT_TEMPSENS_QUANTITY]; // Temperature TMP1075 №1 in °C.
	uint8_t Error_Heat_TMP1075 [PBM_T1_HEAT_TEMPSENS_QUANTITY]; // Register error TMP1075.

	uint16_t HeatVoltage; 					// Voltage on heat branch in mV.
	int16_t HeatCurrent;					// Current on heat branch in mA.
	uint16_t HeatPower; 					// Power on heat branch in mW.

	uint16_t PCA9534_ON_Heat_CMD :1; 		// PCA9534 port. State permission bit auto heat branch. // Need to save in flash.
	uint16_t PCA9534_Heat_State :1; 		// PCA9534 port. Show state auto heat branch.
	uint16_t Error_INA238 :1; 				// Register error INA238.
	uint16_t Error_Heat :1; 				// Register error heat branch.
	uint16_t :12; 							// Reserve.




} _T1_Heat_Data;

typedef struct  // Structure for PBM data
{
	_T1_Branch_Data Branch[PBM_T1_BRANCH_QUANTITY]; // All branch data struct.
	_T1_Heat_Data Heat[PBM_T1_HEAT_QUANTITY];		// All heat data struct.

	//int8_t TMP1075_Temp [PBM_T1_TEMPSENS_QUANTITY]; 		// Temperature TMP1075 №1 in °C.
	//uint8_t Error_TMP1075 [PBM_T1_TEMPSENS_QUANTITY]; 	// Register error TMP1075.

	uint16_t TotalAbcoluteCapacity_mAh;		// Register total accumulate charge of battery's in mAh.
	uint8_t TotalRelativeCapacity_Perc;		// Register total active relative capacity PBM in %.

	uint8_t Error_PCA9534 :1; 				// Register error PCA9534.
	uint8_t Error_I2C_MUX :1;				// Register error TCA9548A.
	uint8_t Low_Energy_Flag :1; 		    // Indicates warning low energy PBM. 0 - ok 1 - low Energy
	uint8_t Zero_Energy_Flag :1;		    // Indicates moment for off all load.
	uint8_t PBM_save_conf_flag:1;			// Indicates that data have been modificate.
	uint8_t :3; 							// Reserve.

} _PBM_T1;

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_T1_STRUCT_H_ */
