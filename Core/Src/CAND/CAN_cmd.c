#include "stm32l4xx.h"

#include "pdm_struct.h"
#include "pdm_config.h"
#include "pdm.h"


#include "pmm_struct.h"
#include "pmm_config.h"
#include "pmm_ctrl.h"

#include "uart_comm.h"

#include "CAN.h"
#include "CAN_cmd.h"

#include  <stdio.h>

extern struct CAN_IVar4 CAN_IVar4_RegCmd;
extern struct CAN_IVar5 CAN_IVar5_telemetry;


void CAN_Var4_cmd_parser(uint32_t *cmd_status, _PDM *pdm_ptr ){

	uint8_t number_cmd_reg = 0;
	uint32_t cmd_bit_flag = 0;

	NVIC_DisableIRQ(CAN1_RX0_IRQn);
	NVIC_DisableIRQ(CAN2_RX0_IRQn);

	for( number_cmd_reg = 0; number_cmd_reg < 32; number_cmd_reg++ ){

		cmd_bit_flag = (*cmd_status) >> number_cmd_reg;

		if( cmd_bit_flag & 0x00000001 ){

			switch (number_cmd_reg) {

                case 12: //Enable/Disable power channel 1 (SOP1) 

                    if( CAN_IVar4_RegCmd.CAN_PWR_CH1 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 12 -> ENABLE power channel 1\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_1, ENABLE );

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 12 -> DISABLE power channel 1\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_1, DISABLE );
                    }
                    break;

                case 13: //Enable/Disable power channel 2 (SOP2) 

                    if( CAN_IVar4_RegCmd.CAN_PWR_CH2 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 13 -> ENABLE power channel 2\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_2, ENABLE );

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 13 -> DISABLE power channel 2\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_2, DISABLE );
                    }
                    break;

                case 14: //Enable/Disable power channel 3 (BRK1) 

                    if( CAN_IVar4_RegCmd.CAN_PWR_CH3 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 14 -> ENABLE power channel 3\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, ENABLE );

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 14 -> DISABLE power channel 3\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_3, DISABLE );
                    }
                    break;

                case 15: //Enable/Disable power channel 4 (BRK2)  uint8_t CAN_PWR_CH4;

                    if( CAN_IVar4_RegCmd.CAN_PWR_CH4 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 15 -> ENABLE power channel 4\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, ENABLE );

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 15 -> DISABLE power channel 4\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_4, DISABLE );
                    }
                    break;

                case 16: //Enable/Disable power channel 5 

                    if( CAN_IVar4_RegCmd.CAN_PWR_CH5 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 16 -> ENABLE power channel 5\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_5, ENABLE );

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 16 -> DISABLE power channel 5\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_5, DISABLE );
                    }
                    break;

                case 17: //Enable/Disable power channel 6 

                    if( CAN_IVar4_RegCmd.CAN_PWR_CH6 == 0x01 ){
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 17 -> ENABLE power channel 6\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_6, ENABLE );

                    }else{
                        #ifdef DEBUGprintf
                            printf("Get comm. reg. 17 -> DISABLE power channel 6\n");
                        #endif
                        PDM_Set_state_PWR_CH( pdm_ptr, PDM_PWR_Channel_6, DISABLE );
                    }
                    break;

                
				case 21://Constant mode command

					if( CAN_IVar4_RegCmd.CAN_Constant_mode == 0x01 ){

						/* This only for test with Dorohkin, need delet after test*/
							#ifdef DEBUGprintf// Only for test with Dorohkin !!!!!!!!!!!!!!!!!! 
								printf("fill telemetry struct by constants\n");// Only for test with Dorohkin !!!!!!!!!!!!!!!!!! 
								printf("size of tel-ry %d\n", sizeof(CAN_IVar5_telemetry));// Only for test with Dorohkin !!!!!!!!!!!!!!!!!! 
							#endif// Only for test with Dorohkin !!!!!!!!!!!!!!!!!! 
						/***********************************************************/

						#ifdef DEBUGprintf
							printf("Get comm. reg. 21 -> Enable constant mode\n");
						#endif
						CAN_Var5_fill_telemetry_const();
						//Need write config to PMM struct
					}else{
						/* This only for test with Dorohkin, need delet after test*/
							printf("fill telemetry struct by zero\n");
							for(uint16_t j = 0; j < sizeof(CAN_IVar5_telemetry); j++){
								*((uint8_t *)(&CAN_IVar5_telemetry) + j) = 0;
							}
						/***********************************************************/

						#ifdef DEBUGprintf
							printf("Get comm. reg. 21 -> Disable constant mode\n");
						#endif
						//Need vrite config to PMM struct
					}
					break;

				case 22: // Switch active CPU (CPUmain active or CPUbackup )

					if( CAN_IVar4_RegCmd.CAN_Switch_active_CPU == 0x00 ){

						#ifdef DEBUGprintf
							printf("Get comm. reg. 22 -> Set active CPUmain\n");
						#endif

						PMM_Set_MUX_CAN_CPUm_CPUb( CPUmain );

					}else if( CAN_IVar4_RegCmd.CAN_Switch_active_CPU == 0x01 ){

						#ifdef DEBUGprintf
							printf("Get comm. reg. 22 -> Set active CPUbackup\n");
						#endif

						LPUART_send_byte( LPUART1, 0xAA );

						//PMM_Set_MUX_CAN_CPUm_CPUb( CPUbackup );
					}
					break;

				default:
					break;
			}

			if( ( cmd_bit_flag & 0xFFFFFFFE ) == 0 ){
				break;
			}
		}
	}

	*cmd_status = 0;
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	NVIC_EnableIRQ(CAN2_RX0_IRQn);
}

/*void CAN_Var5_fill_telemetry(){
}
*/


void CAN_Var5_fill_telemetry_const(void){

	uint8_t i = 0;
	uint8_t j = 0;

    // -------------------  ТМИ 0  ------------------ //
    CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_pX		    =  0x74;
    CAN_IVar5_telemetry.CAN_Beacon_panel_median_temperature_nX		    =  0x75;
    CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[0]				=  0x7A;
    CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[1]				=  0x7B;
    CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[2]				=  0xC7;
    CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[3]				=  0x7D;
    CAN_IVar5_telemetry.CAN_Beacon_solar_panel_status[4]				=  0x7E;
    CAN_IVar5_telemetry.CAN_Beacon_full_capacity_charge_level_AB3_line2 =  0x8C8D;
    CAN_IVar5_telemetry.CAN_Beacon_AB_status[0]							=  0x97;
    CAN_IVar5_telemetry.CAN_Beacon_AB_status[1]						 	=  0x98;
    CAN_IVar5_telemetry.CAN_Beacon_AB_status[2]						 	=  0x99;
    CAN_IVar5_telemetry.CAN_Beacon_charge_discharge_AB_key_status       =  0x9A9B;
    CAN_IVar5_telemetry.CAN_Beacon_subsystem_power_line_status		    =  0xB0;
    CAN_IVar5_telemetry.CAN_Beacon_full_charge_discharge_power		    =  0xB5B6;
    CAN_IVar5_telemetry.CAN_Beacon_total_power_SB                       =  0xB7B8;
    CAN_IVar5_telemetry.CAN_Beacon_spacecraft_total_power               =  0xB9BA;
    CAN_IVar5_telemetry.CAN_Beacon_median_PMM_temp					    =  0xBB;
    CAN_IVar5_telemetry.CAN_Beacon_median_PAM_temp					    =  0xBC;
    CAN_IVar5_telemetry.CAN_Beacon_median_PDM_temp					    =  0xBD;
    CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[0] =  0xBE;
    CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[1] =  0xBF;
    CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[2] =  0xC0;
    CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[3] =  0xC1;
    CAN_IVar5_telemetry.CAN_Beacon_SES_module_system_elements_status[4] =  0xC2;
    // -------------------  ТМИ 4  ------------------ //
    CAN_IVar5_telemetry.CAN_Panal_current_pX                           =	0x5C5D;
    CAN_IVar5_telemetry.CAN_Panal_current_nX                           =	0x5E5F;
    CAN_IVar5_telemetry.CAN_Panal_current_pY                           =	0x6061;
    CAN_IVar5_telemetry.CAN_Panal_current_nY                           =	0x6263;
    CAN_IVar5_telemetry.CAN_Current_hinged_panels_satellite_1_plane    =   0x6465;
    CAN_IVar5_telemetry.CAN_Current_hinged_panels_satellite_2_plane    =   0x6667;


    CAN_IVar5_telemetry.CAN_Panel_voltage_pX                           =	0x6869;
    CAN_IVar5_telemetry.CAN_Panel_voltage_nX                           =	0x6A6B;
    CAN_IVar5_telemetry.CAN_Panel_voltage_pY                           =	0x6C6D;
    CAN_IVar5_telemetry.CAN_Panel_voltage_nY                           =	0x6E6F;
    CAN_IVar5_telemetry.CAN_Voltage_hinged_panels_satellite_1_plane    =   0x7071;
    CAN_IVar5_telemetry.CAN_Voltage_hinged_panels_satellite_2_plane    =   0x7273;


    CAN_IVar5_telemetry.CAN_Panel_median_temperature_pX                 =  0x74;
    CAN_IVar5_telemetry.CAN_Panel_median_temperature_nX                 =  0x75;
    CAN_IVar5_telemetry.CAN_Panel_median_temperature_pY                 =  0x76;
    CAN_IVar5_telemetry.CAN_Panel_median_temperature_nY                 =  0x77;
    CAN_IVar5_telemetry.CAN_Hinged_panel_median_temperature_pY          =  0x78;
    CAN_IVar5_telemetry.CAN_Hinged_panel_median_temperature_nY          =  0x79;
    CAN_IVar5_telemetry.CAN_Solar_panel_status[0]                       =  0x7A;
    CAN_IVar5_telemetry.CAN_Solar_panel_status[1]                       =  0x7B;
    CAN_IVar5_telemetry.CAN_Solar_panel_status[2]                       =  0x7C;
    CAN_IVar5_telemetry.CAN_Solar_panel_status[3]                       =  0x7D;
    CAN_IVar5_telemetry.CAN_Solar_panel_status[4]                       =  0x7E;


    CAN_IVar5_telemetry.CAN_Full_percent_charge_level_AB3_line2         =  0x7F;


    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB1_line1             =  0x8081;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB1_line2             =  0x8283;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB2_line1             =  0x8485;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB2_line2             =  0x8687;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line1             =  0x8889;
    CAN_IVar5_telemetry.CAN_Capacity_charge_level_AB3_line2             =  0x8A8B;
    CAN_IVar5_telemetry.CAN_Full_capacity_charge_level_AB3_line2        =  0x8C8D;


    CAN_IVar5_telemetry.CAN_Average_temp_controller_AB1                 =  0x8E;
    CAN_IVar5_telemetry.CAN_Average_line1_temp_AB1                      =  0x8F;
    CAN_IVar5_telemetry.CAN_Average_line2_temp_AB1                      =  0x90;
    CAN_IVar5_telemetry.CAN_Average_temp_controller_AB2                 =  0x91;
    CAN_IVar5_telemetry.CAN_Average_line1_temp_AB2                      =  0x92;
    CAN_IVar5_telemetry.CAN_Average_line2_temp_AB2                      =  0x93;
    CAN_IVar5_telemetry.CAN_Average_temp_controller_AB3                 =  0x94;
    CAN_IVar5_telemetry.CAN_Average_line1_temp_AB3                      =  0x95;
    CAN_IVar5_telemetry.CAN_Average_line2_temp_AB3                      =  0x96;
    CAN_IVar5_telemetry.CAN_AB_status[0]                                =  0x97;
    CAN_IVar5_telemetry.CAN_AB_status[1]                                =  0x98;
    CAN_IVar5_telemetry.CAN_AB_status[2]                                =  0x99;

    CAN_IVar5_telemetry.CAN_Charge_discharge_AB_key_status              =  0x9A9B;

    CAN_IVar5_telemetry.CAN_Channel1_current                            =  0x9C9D;
    CAN_IVar5_telemetry.CAN_Channel2_current                            =  0x9E9F;
    CAN_IVar5_telemetry.CAN_Channel3_current                            =  0xA0A1;
    CAN_IVar5_telemetry.CAN_Channel4_current                            =  0xA2A3;
    CAN_IVar5_telemetry.CAN_Channel5_current                            =  0xA4A5;
    CAN_IVar5_telemetry.CAN_Channel6_current                            =  0xA6A7;

    CAN_IVar5_telemetry.CAN_VBAT1_voltage                               =  0xA8A9;
    CAN_IVar5_telemetry.CAN_VBAT2_voltage                               =  0xAAAB;

    CAN_IVar5_telemetry.CAN_VBAT1_current                               =  0xACAD;
    CAN_IVar5_telemetry.CAN_VBAT2_current                               =  0xAEAF;

    CAN_IVar5_telemetry.CAN_Subsystem_power_line_status                 =  0xB0;
    CAN_IVar5_telemetry.CAN_SES_current_consumption                     =  0xB1B2;
    CAN_IVar5_telemetry.CAN_SES_power_supply                            =  0xB3B4;
    CAN_IVar5_telemetry.CAN_Full_charge_discharge_power                 =  0xB5B6;
    CAN_IVar5_telemetry.CAN_Total_power_SB                              =  0xB7B8;
    CAN_IVar5_telemetry.CAN_Spacecraft_total_power                      =  0xB9BA;

    CAN_IVar5_telemetry.CAN_Median_PMM_temp                             =  0xBB;
    CAN_IVar5_telemetry.CAN_Median_PAM_temp                             =  0xBC;
    CAN_IVar5_telemetry.CAN_Median_PDM_temp                             =  0xBD;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[0]        =  0xBE;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[1]        =  0xBF;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[2]        =  0xC0;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[3]        =  0xC1;
    CAN_IVar5_telemetry.CAN_SES_module_system_elements_status[4]        =  0xC2;
    CAN_IVar5_telemetry.CAN_Number_of_restarts_of_the_SES_module        =  0xC3C4C5C6;
    CAN_IVar5_telemetry.CAN_Number_of_restarts_of_the_reserve_SES_module=  0xC7C8C9CA;
    CAN_IVar5_telemetry.CAN_SES_module_data_array1                  	 =  0xCBCCCDCE;
    CAN_IVar5_telemetry.CAN_Primary_standby_switch                      =  0xCF;
    // -------------------  ТМИ 7  ------------------ //
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB1_line1          =  0xB8B9;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB1_line2          =  0xBABB;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB2_line1          =  0xBCBD;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB2_line2          =  0xBEBF;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB3_line1          =  0xC0C1;
    CAN_IVar5_telemetry.CAN_Charge_discharge_current_AB3_line2          =  0xC2C3;

    CAN_IVar5_telemetry.CAN_Voltage_AB1_line1_battery_1                 =	0xC4C5;
    CAN_IVar5_telemetry.CAN_Voltage_AB1_line1_battery_2                 =	0xC6C7;
    CAN_IVar5_telemetry.CAN_Voltage_AB1_line2_battery_1                 =	0xC8C9;
    CAN_IVar5_telemetry.CAN_Voltage_AB1_line2_battery_2                 =	0xCACB;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line1_battery_1                 =	0xCCCD;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line1_battery_2                 =	0xCECF;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line2_battery_1                 =	0xD0D1;
    CAN_IVar5_telemetry.CAN_Voltage_AB2_line2_battery_2                 =	0xD2D3;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line1_battery_1                 =	0xD4D5;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line1_battery_2                 =	0xD6D7;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line2_battery_1                 =	0xD8D9;
    CAN_IVar5_telemetry.CAN_Voltage_AB3_line2_battery_2                 =	0xDADB;

    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB1_line1              =	0xDC;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB1_line2              =	0xDD;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB2_line1              =	0xDE;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB2_line2              =	0xDF;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB3_line1              =	0xE0;
    CAN_IVar5_telemetry.CAN_Percent_charge_level_AB3_line2              =	0xE1;

    CAN_IVar5_telemetry.CAN_Channel1_voltage                            =	0xE2E3;
    CAN_IVar5_telemetry.CAN_Channel2_voltage                            =	0xE4E5;
    CAN_IVar5_telemetry.CAN_Channel3_voltage                            =	0xE6E7;
    CAN_IVar5_telemetry.CAN_Channel4_voltage                            =	0xE8E9;
    CAN_IVar5_telemetry.CAN_Channel5_voltage                            =	0xEAEB;
    CAN_IVar5_telemetry.CAN_Channel6_voltage                            =	0xECED;

    CAN_IVar5_telemetry.CAN_Opening_elements_state                      =	0xEEEF;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[0]                   =	0xF0;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[1]                   =  0xF1;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[2]                   =  0xF2;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[3]                   =  0xF3;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[4]                   =  0xF4;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[5]                   =  0xF5;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[6]                   =  0xF6;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[7]                   =  0xF7;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[8]                   =  0xF8;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[9]                   =  0xF9;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[10]                  =  0xFA;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[11]                  =  0xFB;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[12]                  =  0xFC;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[13]                  =  0xFD;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[14]                  =  0xFE;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[15]                  =  0xFF;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[16]                  =  0x00;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[17]                  =  0x01;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[18]                  =  0x02;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[19]                  =  0x03;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[20]                  =  0x04;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[21]                  =  0x05;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[22]                  =  0x06;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[23]                  =  0x07;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[24]                  =  0x08;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[25]                  =  0x09;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[26]                  =  0x0A;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[27]                  =  0x0B;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[28]                  =  0x0C;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[29]                  =  0x0D;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[30]                  =  0x0E;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[31]                  =  0x0F;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[32]                  =  0x10;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[33]                  =  0x11;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[34]                  =  0x12;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[35]                  =  0x13;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[36]                  =  0x14;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[37]                  =  0x15;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[38]                  =  0x16;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[39]                  =  0x17;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[40]                  =  0x18;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[41]                  =  0x19;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[42]                  =  0x1A;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[43]                  =  0x1B;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[44]                  =  0x1C;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[45]                  =  0x1D;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[46]                  =  0x1E;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[47]                  =  0x1F;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[48]                  =  0x20;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[49]                  =  0x21;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[50]                  =  0x22;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[51]                  =  0x23;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[52]                  =  0x24;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[53]                  =  0x25;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[54]                  =  0x26;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[55]                  =  0x27;
    CAN_IVar5_telemetry.CAN_SES_module_data_array2[56]                  =  0x28;

    CAN_IVar5_telemetry.CAN_Reset_to_default                            =  0x29;
    CAN_IVar5_telemetry.CAN_Reset_counter_to_default_state              =  0x2A;
    CAN_IVar5_telemetry.CAN_Perform_disclosure                          =  0x2B;
    // ----------------------- ТМИ 8 -------------------
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_1                      =	0x2C;
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_2                      =	0x2D;
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_3                      =	0x2E;
    CAN_IVar5_telemetry.CAN_Panal_temp_pX_sensor_4                      =	0x2F;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_1                      =	0x30;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_2                      =	0x31;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_3                      =	0x32;
    CAN_IVar5_telemetry.CAN_Panal_temp_nX_sensor_4                      =	0x33;
    CAN_IVar5_telemetry.CAN_Panal_temp_pY_sensor_1                      =	0x34;
    CAN_IVar5_telemetry.CAN_Panal_temp_pY_sensor_2                      =	0x35;
    CAN_IVar5_telemetry.CAN_Panal_temp_nY_sensor_1                      =	0x36;
    CAN_IVar5_telemetry.CAN_Panal_temp_nY_sensor_2                      =	0x37;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_1               =  0x38;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_2               =  0x39;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_3               =  0x3A;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_pY_sensor_4               =  0x3B;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_1               =  0x3C;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_2               =  0x3D;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_3               =  0x3E;
    CAN_IVar5_telemetry.CAN_Hinged_panel_temp_nY_sensor_4               =  0x3F;

    CAN_IVar5_telemetry.CAN_AB1_controller_1_sensor                     =	0x40;
    CAN_IVar5_telemetry.CAN_AB1_controller_2_sensor                     =	0x41;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board1                        =	0x42;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board2                        =	0x43;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board3                        =	0x44;
    CAN_IVar5_telemetry.CAN_AB1_sensor_on_board4                        =	0x45;
    CAN_IVar5_telemetry.CAN_AB2_controller_1_sensor                     =	0x46;
    CAN_IVar5_telemetry.CAN_AB2_controller_2_sensor                     =	0x47;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board1                        =	0x48;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board2                        =	0x49;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board3                        =	0x4A;
    CAN_IVar5_telemetry.CAN_AB2_sensor_on_board4                        =	0x4B;
    CAN_IVar5_telemetry.CAN_AB3_controller_1_sensor                     =	0x4C;
    CAN_IVar5_telemetry.CAN_AB3_controller_2_sensor                     =	0x4D;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board1                        =	0x4E;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board2                        =	0x4F;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board3                        =	0x50;
    CAN_IVar5_telemetry.CAN_AB3_sensor_on_board4                        =	0x51;


    CAN_IVar5_telemetry.CAN_Charge_gate_1_AB1                           =  0x52;
    CAN_IVar5_telemetry.CAN_Discharge_gate_1_AB1                        =  0x53;
    CAN_IVar5_telemetry.CAN_Charge_gate_2_AB1                           =  0x54;
    CAN_IVar5_telemetry.CAN_Discharge_gate_2_AB1                        =  0x55;
    CAN_IVar5_telemetry.CAN_Charge_gate_1_AB2                           =  0x56;
    CAN_IVar5_telemetry.CAN_Discharge_gate_1_AB2                        =  0x57;
    CAN_IVar5_telemetry.CAN_Charge_gate_2_AB2                           =  0x58;
    CAN_IVar5_telemetry.CAN_Discharge_gate_2_AB2                        =  0x59;
    CAN_IVar5_telemetry.CAN_Charge_gate_1_AB3                           =  0x5A;
    CAN_IVar5_telemetry.CAN_Discharge_gate_1_AB3                        =  0x5B;
    CAN_IVar5_telemetry.CAN_Charge_gate_2_AB3                           =  0x5C;
    CAN_IVar5_telemetry.CAN_Discharge_gate_2_AB3                        =  0x5D;


    CAN_IVar5_telemetry.CAN_Line1                                       =	0x5E;
    CAN_IVar5_telemetry.CAN_Line2                                       =	0x5F;
    CAN_IVar5_telemetry.CAN_Line3                                       =	0x60;
    CAN_IVar5_telemetry.CAN_Line4                                       =	0x61;
    CAN_IVar5_telemetry.CAN_Line5                                       =	0x62;
    CAN_IVar5_telemetry.CAN_Line6                                       =	0x63;
    CAN_IVar5_telemetry.CAN_Line_VBAT1                                  =	0x64;
    CAN_IVar5_telemetry.CAN_Line_VBAT2                                  =	0x65;

    CAN_IVar5_telemetry.CAN_SES_internal_bus_voltage                    =	0x6667;


    CAN_IVar5_telemetry.CAN_PMM_sensor1                                 =	0x68;
    CAN_IVar5_telemetry.CAN_PAM_sensor1                                 =	0x69;
    CAN_IVar5_telemetry.CAN_PAM_sensor2                                 =	0x6A;
    CAN_IVar5_telemetry.CAN_PAM_sensor3                                 =	0x6B;
    CAN_IVar5_telemetry.CAN_PAM_sensor4                                 =	0x6C;
    CAN_IVar5_telemetry.CAN_PDM_sensor1                                 =	0x6D;
    CAN_IVar5_telemetry.CAN_PDM_sensor2                                 =	0x6E;
    CAN_IVar5_telemetry.CAN_PDM_sensor3                                 =	0x6F;
    CAN_IVar5_telemetry.CAN_PDM_sensor4                                 =	0x70;

    for( i = 0, j = 0x71; i < 47; i++, j++ ){
        CAN_IVar5_telemetry.CAN_data_array3[i] = j;
    }
}
