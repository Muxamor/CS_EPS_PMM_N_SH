#ifndef INC_PMM_PMM_H_
#define INC_PMM_PMM_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
typedef enum {
  ERROR_N = -1,
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

#include "PBM/pbm_config.h"

// Structure PDM
typedef struct{
    uint8_t State_eF_in  	:1;
    uint8_t State_eF_out 	:1;
    uint8_t :6;

 }FRAM_PDM_PWR_CH;


// Structure PAM
typedef struct{
    uint8_t State_eF_out    :1;
    uint8_t :7;

}FRAM_PAM_PWR_CH_TM_SP;

// change!!!!!! Morsin A.A.
// Structure PBM_T1

typedef struct{

	uint8_t DchgEnableBit :1; 				// Discharge Enable Bit must be set 1 to allow the ON state.
	uint8_t ChgEnableBit :1; 				// Charge Enable Bit must be set 1 to allow the ON state.
	uint8_t :6;

}FRAM_PBM_T1_Branch;

typedef struct{

	uint16_t PCA9534_ON_Heat_CMD :1; 		// PCA9534 port. State permission bit auto heat branch.
	uint8_t :7;

}FRAM_PBM_T1_Heat;

typedef struct{

	FRAM_PBM_T1_Branch Branch[PBM_T1_BRANCH_QUANTITY]; // Branch data to need save in fram.
	FRAM_PBM_T1_Heat Heat[PBM_T1_HEAT_QUANTITY];		// Heat data to need save in fram.
    uint8_t Branch_1_DchgEnableBit  :1;
    uint8_t Branch_1_ChgEnableBit   :1;
    uint8_t Branch_2_DchgEnableBit  :1;
    uint8_t Branch_2_ChgEnableBit   :1;
    uint8_t PCA9534_ON_Heat_1       :1;
    uint8_t PCA9534_ON_Heat_2       :1;
    uint8_t :2;

}FRAM_PBM_T1;


#pragma pack(push, 1)

 typedef struct {

     //PMM
     uint32_t FRAM_PMM_PWR_Ch_State_CANmain			:1;
     uint32_t FRAM_PMM_PWR_Ch_State_CANbackup		:1;
     uint32_t FRAM_PMM_PWR_Ch_State_Vbat1_eF1   	:1;
     uint32_t FRAM_PMM_PWR_Ch_State_Vbat1_eF2		:1;
     uint32_t FRAM_PMM_PWR_Ch_State_Vbat2_eF1     	:1;
     uint32_t FRAM_PMM_PWR_Ch_State_Vbat2_eF2		:1;
     uint32_t FRAM_PMM_PWR_Ch_State_PBMs_Logic		:1;
     uint32_t FRAM_PMM_PWR_Ch_State_Deploy_Logic 	:1;
     uint32_t FRAM_PMM_PWR_Ch_State_Deploy_Power 	:1;
     uint32_t FRAM_PMM_PWR_Ch_State_5V_Bus 			:1;
     uint32_t FRAM_PMM_PWR_Ch_State_3_3V_Bus 		:1;
     uint32_t FRAM_PMM_PWR_Ch_State_I2C_Bus 		:1;
     uint32_t FRAM_PMM_PWR_OFF_Passive_CPU          :1;
     uint32_t FRAM_PMM_Active_CPU		 		    :1;
     uint32_t FRAM_PMM_CAN_constatnt_mode		    :1;
     uint32_t FRAM_PMM_Deploy_Lim_SW_Exit_1         :1;
     uint32_t FRAM_PMM_Deploy_Lim_SW_Exit_2         :1;
     uint32_t FRAM_PMM_Deploy_Ch4_Lim_SW_1_Yp       :1;
     uint32_t FRAM_PMM_Deploy_Ch4_Lim_SW_2_Yp       :1;
     uint32_t FRAM_PMM_Deploy_Ch3_Lim_SW_1_Yn       :1;
     uint32_t FRAM_PMM_Deploy_Ch3_Lim_SW_2_Yn       :1;
     uint32_t FRAM_PMM_Deploy_Ch1_Lim_SW_1_Zp       :1;
     uint32_t FRAM_PMM_Deploy_Ch1_Lim_SW_2_Zp       :1;
     uint32_t FRAM_PMM_Deploy_Ch2_Lim_SW_1_Zn       :1;
     uint32_t FRAM_PMM_Deploy_Ch2_Lim_SW_2_Zn       :1;
     uint32_t :7;

     uint32_t Reserved1;
     uint32_t Reserved2;
     uint32_t Reserved3;
     uint32_t Reserved4;

     uint32_t FRAM_PMM_reboot_counter_CPUm;
     uint32_t FRAM_PMM_reboot_counter_CPUb;

     uint8_t FRAM_PMM_EPS_Mode;
     uint8_t FRAM_PMM_Deploy_stage;

     //PDM
     uint8_t  FRAM_PDM_PDM_ID_module;
     FRAM_PDM_PWR_CH FRAM_PDM_PWR_Ch[PDM_PWR_Ch_quantity];

     //PAM
     uint8_t FRAM_PAM_State_DC_DC 			    :1;
     uint8_t FRAM_PAM_State_LDO 				:1;
     uint8_t :6;

     uint8_t FRAM_PAM_PAM_ID_module;
     FRAM_PAM_PWR_CH_TM_SP FRAM_PAM_PWR_Ch_TM_SP[PAM_PWR_TM_SP_Ch_quantity];

     //PBM
     FRAM_PBM_T1 FRAM_PBM_T1[PBM_T1_QUANTITY];

     uint16_t FRAM_DATA_CRC;

 } _FRAM_SAVE_DATA;

 #pragma pack(pop)



ErrorStatus PMM_FRAM_write_data( I2C_TypeDef *I2Cx_fram1, I2C_TypeDef *I2Cx_fram2, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, _EPS_Param eps_p );
ErrorStatus PMM_FRAM_read_data( I2C_TypeDef *I2Cx_fram1, I2C_TypeDef *I2Cx_fram2, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, _EPS_Param eps_p );
ErrorStatus PMM_FRAM_Restore_Settings ( _EPS_Param eps_p );
ErrorStatus PMM_Sync_and_Save_Settings_A_P_CPU( _EPS_Param eps_p );
ErrorStatus FRAM_Check_Data_CRC( uint8_t *data, uint16_t data_size, uint16_t expected_crc );

#ifdef __cplusplus
}
#endif

#endif /* INC_PMM_PMM_H_ */
