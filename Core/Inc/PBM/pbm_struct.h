

#ifndef INC_PBM_STRUCT_H_
#define INC_PBM_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  // Structure for PBM data
{
	uint8_t Branch_1_DchgEnableBit :1; 		// Discharge Enable Bit must be set to allow the ON state. // Need to save in flash.
	uint8_t Branch_1_ChgEnableBit :1; 		// Charge Enable Bit must be set to allow the ON state. // Need to save in flash.
	uint8_t Branch_1_DchgControlFlag :1; 	// Discharge Control Flag indicates the real logic state of the DC pin driver.
	uint8_t Branch_1_ChgControlFlag :1; 	// Charge Control Flag indicates the logic state of the CC pin driver.
	uint8_t Branch_1_LrnFlag :1; 			// LEARN Flag indicates that the current-charge cycle can use to learn batt.
	uint8_t Branch_1_StbEmptyFlag :1; 		// Standby-Empty Flag is set to indicate power battery is less than 10%.
	uint8_t Branch_1_ActEmptyFlag :1; 		// Active-Empty Flag is set when the battery is at or below the AE point.
	uint8_t Branch_1_ChgTerminateFlag :1; 	// Charge-Termination Flag is set to indicate that the battery charged.
	uint8_t Branch_1_RelativeCapacity;		// Register active relative Capacity in %.
	
	int16_t Branch_1_AbcoluteCapacity;		// Register accumulate charge of battery's in mAh.
	
	uint8_t Branch_1_AgeScalar;				// Register adjusts the cell capacity estimation in %.
	uint8_t Branch_1_CycleCounter;			// Register cycle counter is an absolute count of the discharge cycles.
		
	int16_t Branch_1_Current;				// Register current level over the preceding 3.52s in mA.
	int16_t Branch_1_AverageCurrent;		// Register average current level over the preceding 28.16s in mA.
	int16_t Branch_1_VoltageHi;				// Register voltage of high battery (on 8.4V) in mV.
	int16_t Branch_1_VoltageLo;				// Register voltage of lower battery (on GND) in mV.
	
	int8_t Branch_1_Temperature;			// Register temperature of DS2777 in °C.
	uint8_t Branch_2_DchgEnableBit :1; 		// Discharge Enable Bit must be set to allow the ON state
	uint8_t Branch_2_ChgEnableBit :1; 		// Charge Enable Bit must be set to allow the ON state.
	uint8_t Branch_2_DchgControlFlag :1; 	// Discharge Control Flag indicates the real logic state of the DC pin driver.
	uint8_t Branch_2_ChgControlFlag :1; 	// Charge Control Flag indicates the logic state of the CC pin driver.
	uint8_t Branch_2_LrnFlag :1; 			// LEARN Flag indicates that the current-charge cycle can use to learn batt.
	uint8_t Branch_2_StbEmptyFlag :1; 		// Standby-Empty Flag is set to indicate power battery is less than 10%.
	uint8_t Branch_2_ActEmptyFlag :1; 		// Active-Empty Flag is set when the battery is at or below the AE point.
	uint8_t Branch_2_ChgTerminateFlag :1; 	// Charge-Termination Flag is set to indicate that the battery charged.
	
	int16_t Branch_2_AbcoluteCapacity;		// Register accumulate charge of battery's in mAh.

	uint8_t Branch_2_RelativeCapacity;		// Register active relative Capacity in %.
	uint8_t Branch_2_AgeScalar;				// Register adjusts the cell capacity estimation in %.
	
	uint8_t Branch_2_CycleCounter;			// Register cycle counter is an absolute count of the discharge cycles.
	int8_t Branch_2_Temperature;			// Register temperature of DS2777 in °C.
	
	int16_t Branch_2_Current;				// Register current level over the preceding 3.52s in mA.
	int16_t Branch_2_AverageCurrent;		// Register average current level over the preceding 28.16s in mA.
	int16_t Branch_2_VoltageHi;				// Register voltage of high battery (on 8.4V) in mV.
	int16_t Branch_2_VoltageLo;				// Register voltage of lower battery (on GND) in mV.
	int16_t TotalAbcoluteCapacity;			// Register total accumulate charge of battery's in mAh.
	
	uint8_t TotalRelativeCapacity;			// Register total active relative capacity PBM in %.
	int8_t TMP1075_temp_1;					// Temperature TMP1075 №1 in °C.
	
	int8_t TMP1075_temp_2;					// Temperature TMP1075 №2 in °C.
	int8_t TMP1075_temp_3;					// Temperature TMP1075 №3 in °C.
	
	int8_t TMP1075_temp_4;					// Temperature TMP1075 №4 in °C.
	uint8_t PCA9534_ON_Heat_1 :1; 			// PCA9534 port №1. State permission bit auto heat branch №1. // Need to save in flash.
	uint8_t PCA9534_ON_Heat_2 :1; 			// PCA9534 port №6. State permission bit auto heat branch №2. // Need to save in flash.
	uint8_t PCA9534_TempSens_State_1 :1; 	// PCA9534 port №7. Show state auto heat branch №1.
	uint8_t PCA9534_TempSens_State_2 :1; 	// PCA9534 port №4. Show state auto heat branch №2.
	uint8_t PCA9534_PIO_1 :1; 				// PCA9534 port №5. Show state PIO DS2777 branch №1.
	uint8_t PCA9534_PIO_2 :1; 				// PCA9534 port №8. Show state PIO DS2777 branch №2.
	uint8_t PCA9534_P1 :1; 					// PCA9534 port №2.
	uint8_t PCA9534_P2 :1; 					// PCA9534 port №3.
	
	uint16_t Error_DS2777_1 :1; 			// Register error DS2777 №1.
	uint16_t Error_DS2777_2 :1; 			// Register error DS2777 №2.
	uint16_t Error_TMP1075_1 :1; 			// Register error TMP1075 №1.
	uint16_t Error_TMP1075_2 :1; 			// Register error TMP1075 №2.
	uint16_t Error_TMP1075_3 :1; 			// Register error TMP1075 №3.
	uint16_t Error_TMP1075_4 :1; 			// Register error TMP1075 №4.
	uint16_t Error_PCA9534 :1; 				// Register error PCA9534.
	uint16_t Error_Heat_1 :1; 				// Register error heat branch №1.
	uint16_t Error_Heat_2 :1; 				// Register error heat branch №2.
	uint16_t Error_Charge_1 :1; 			// Register error charge branch №1.
	uint16_t Error_Charge_2 :1; 			// Register error charge branch №2.
	uint16_t Error_Discharge_1 :1; 			// Register error discharge branch №1.
	uint16_t Error_Discharge_2 :1; 			// Register error discharge branch №2.
	uint16_t Low_Energy_Flag :1; 		    // Indicates warning low energy PBM. 0 - ok 1 - low Energy
	uint16_t Zero_Energy_Flag :1;		    // Indicates moment for off all load.
	uint16_t PBM_save_conf_flag:1;			// Indicates that data have been modificate.

	uint16_t Reserve_1;						// Reserve data space.
	uint16_t Reserve_2;						// Reserve data space.

} _PBM;

#ifdef __cplusplus
}
#endif

#endif /* INC_PBM_STRUCT_H_ */
