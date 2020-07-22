#ifndef _CAN_H_
#define _CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32l4xx.h"
#include "Error_Handler.h"

#define CAN_ID_DEV            			0x03

#define CAN_ID_VAR_ROM					0x00
#define CAN_ROM_ADDR					0x08000000  // end = 0x0801FFFF	size = 1024 KB

#define CAN_ID_VAR_RAM					0x01
#define CAN_RAM_ADDR					0x20000000  // end = 0x2003FFFF	size = 256 KB

#define CAN_ID_IVAR_REGCMD      		0x04
#define CAN_ID_VAR_TELEMETRY 			0x05

#define ERR_INVALID_PARAMS        	    -1
#define ERR_CAN_INIT_MODE         	    -2
#define ERR_CAN_NORMAL_MODE       	    -3
#define ERR_CAN_NO_FREE_FILTER    	    -4
#define ERR_CAN_NO_TXMAILBOXES    	    -5
#define ERR_CAN_ACCESS_RANGE      	    -7
#define ERR_CAN_DCL_INVALID       	    -8

//VarID4 Offset
#define CAN_Constant_mode_offset        4

#define CAN_AB1_Charge_key_1_offset     16
#define CAN_AB1_Discharge_key_1_offset  17
#define CAN_AB1_Charge_key_2_offset     18
#define CAN_AB1_Discharge_key_2_offset  19
#define CAN_AB2_Charge_key_1_offset     20
#define CAN_AB2_Discharge_key_1_offset  21
#define CAN_AB2_Charge_key_2_offset	    22
#define CAN_AB2_Discharge_key_2_offset  23
#define CAN_AB3_Charge_key_1_offset     24
#define CAN_AB3_Discharge_key_1_offset  25
#define CAN_AB3_Charge_key_2_offset	    26
#define CAN_AB3_Discharge_key_2_offset  27

#define CAN_PWR_CH1_offset			    28
#define CAN_PWR_CH2_offset			    29
#define CAN_PWR_CH3_offset              30
#define CAN_PWR_CH4_offset			    31
#define CAN_PWR_CH5_offset			    32
#define CAN_PWR_CH6_offset			    33
#define CAN_PWR_VBAT1_offset		    34
#define CAN_PWR_VBAT2_offset		    35

#define CAN_Switch_active_CPU_offset    37


extern struct CAN_IVar5 CAN_IVar5_telemetry;
extern struct CAN_IVar4  CAN_IVar4_RegCmd;

extern uint64_t CAN_cmd_mask_status;
extern uint8_t CAN1_exchange_timeout_flag;
extern uint8_t CAN2_exchange_timeout_flag;

typedef union {
  struct {
    uint32_t res1 : 1;
    uint32_t RTR : 1;
    uint32_t IDE : 1;
    uint32_t EXID : 29;
    }std;
  struct {
    uint32_t res1 : 1;
    uint32_t RTR : 1;
    uint32_t res2 : 1;
    uint32_t Offset : 21;
    uint32_t VarId : 4;
    uint32_t DevId : 4;
    }uf;
}CAN_typeIdxMask;


struct CAN_IVar4 {

	uint8_t CAN_time_byte0;					//+0
	uint8_t CAN_time_byte1;					//+1
	uint8_t CAN_time_byte2;					//+2
	uint8_t CAN_time_byte3;					//+3
    uint8_t CAN_Constant_mode;				//+4			1 - Перейти в режим констант; 0 - штатный режим
    uint8_t CAN_Common_cmd0;				//+5
    uint8_t CAN_Common_cmd1;				//+6
    uint8_t CAN_Common_cmd2;				//+7
    uint8_t CAN_Common_cmd3;				//+8
    uint8_t CAN_Common_cmd4;				//+9
    uint8_t CAN_Common_cmd5;				//+10
    uint8_t CAN_Common_cmd6;				//+11
    uint8_t CAN_Common_cmd7;				//+12
    uint8_t CAN_Common_cmd8;				//+13
    uint8_t CAN_Common_cmd9;				//+14
    uint8_t CAN_Common_cmd10;				//+15

    // Массив командных регистров ключей разряда АБ
    uint8_t CAN_AB1_Charge_key_branch_1;  		//+16            Командный регистр ключа заряда ветви 1 АБ1
    uint8_t CAN_AB1_Discharge_key_branch_1;     //+17            Командный регистр ключа разряда ветви 1 АБ1
    uint8_t CAN_AB1_Charge_key_branch_2;  		//+18            Командный регистр ключа заряда ветви 2 АБ1
    uint8_t CAN_AB1_Discharge_key_branch_2;     //+19            Командный регистр ключа разряда ветви 2 АБ1
    uint8_t CAN_AB2_Charge_key_branch_1;   		//+20            Командный регистр ключа заряда ветви 1 АБ2
    uint8_t CAN_AB2_Discharge_key_branch_1;   	//+21            Командный регистр ключа разряда ветви 1 АБ2
    uint8_t CAN_AB2_Charge_key_branch_2;   		//+22            Командный регистр ключа заряда ветви 2 АБ2
    uint8_t CAN_AB2_Discharge_key_branch_2;   	//+23            Командный регистр ключа разряда ветви 2 АБ2
    uint8_t CAN_AB3_Charge_key_branch_1;   		//+24            Командный регистр ключа заряда ветви 1 АБ3
    uint8_t CAN_AB3_Discharge_key_branch_1;   	//+25            Командный регистр ключа разряда ветви 1 АБ3
    uint8_t CAN_AB3_Charge_key_branch_2;   		//+26            Командный регистр ключа заряда ветви 2 АБ3
    uint8_t CAN_AB3_Discharge_key_branch_2;   	//+27            Командный регистр ключа разряда ветви 2 АБ3

    // Массив командных регистров линий питания
    uint8_t CAN_PWR_CH1;                    //+28           Командный регистр канал питаня 1 (СОП1)
    uint8_t CAN_PWR_CH2;                    //+29           Командный регистр канал питаня 2 (СОП2)
    uint8_t CAN_PWR_CH3;                    //+30           Командный регистр канал питаня 3 (БРК1)
    uint8_t CAN_PWR_CH4;                    //+31           Командный регистр канал питаня 4 (БРК2)
    uint8_t CAN_PWR_CH5;                    //+32           Командный регистр канал питаня 5 ( - )
    uint8_t CAN_PWR_CH6;                    //+33           Командный регистр канал питаня 6 ( - )
    uint8_t CAN_PWR_VBAT1;                  //+34           Командный регистр линии VBAT1 (МС)
    uint8_t CAN_PWR_VBAT2;                  //+35           Командный регистр линии VBAT2 (МС)
    uint8_t CAN_Turn_SES_mode;              //+36			Установить режим СЭС
    uint8_t CAN_Set_active_CPU;             //+37		    Переключатель основной/резервный
    uint8_t CAN_Reset_to_default;           //+38           Сброс параметров в состояние "по умолчанию"
    uint8_t CAN_Perform_disclosure;         //+39           Выполнить раскрытие

};



#pragma pack(push, 1)

struct CAN_IVar5{
    //------------------------------------------------  Offset -------- Constant ---------------------- Description ------------------------//
    // -------------------  ТМИ 0  ------------------ //

    uint8_t  CAN_Beacon_panel_median_temperature_pX;				//+0      |  const = 0x74                 |   Медианная температура панели +X
    uint8_t  CAN_Beacon_panel_median_temperature_nX;				//+1      |  const = 0x75                 |   Медианная температура панели -X
    uint8_t  CAN_Beacon_solar_panel_status[5];						//+2      |  const = 0x7A7B7C7D7E         |   Статус солнечных панелей, битовая маска (в т.ч. ФЭП)
    uint16_t CAN_Beacon_full_capacity_charge_level;					//+7      |  const = 0x8C8D               |   Полный уровень заряда АБ в mAh
    uint8_t  CAN_Beacon_AB_status[3];								//+9      |  const = 0x979899             |   Статус АБ, битовая маска
    uint16_t CAN_Beacon_charge_discharge_AB_key_status;             //+12     |  const = 0x9A9B               |   Статус ключей заряда и разряда АБ, битовая маска
    uint8_t  CAN_Beacon_subsystem_power_line_status;				//+14     |  const = 0xB0                 |   Статус линии пит. подсистем, битовая маска
    uint16_t CAN_Beacon_full_charge_discharge_power;				//+15     |  const = 0xB5B6               |   Полная мощность зарядки/разрядки АБ
    uint16_t CAN_Beacon_total_power_SB;                             //+17     |  const = 0xB7B8               |   Полная вырабатываемая мощность СБ
    uint16_t CAN_Beacon_spacecraft_total_power;                     //+19     |  const = 0xB9BA               |   Полная мощность потребления КА
    uint8_t  CAN_Beacon_median_PMM_temp;							//+21     |  const = 0xBB                 |   Медианная температура PMM (один датчик)
    uint8_t  CAN_Beacon_median_PAM_temp;							//+22     |  const = 0xBC                 |   Медианная температура PAM
    uint8_t  CAN_Beacon_median_PDM_temp;							//+23     |  const = 0xBD                 |   Медианная температура PDM
    uint8_t  CAN_Beacon_SES_module_system_elements_status[5];		//+24     |  const = 0xBEBFC0C1C2         |   Статус элементов системы модуля СЭС, битовая маска

    // -------------------  ТМИ 4  ------------------ //

    //Массив CAN_токов солнечных панелей                                     |                               |
    uint16_t CAN_Panal_current_pX;                                  //+29    |  const = 0x5C5D               |   Ток панели +X
    uint16_t CAN_Panal_current_nX;                                  //+31    |  const = 0x5E5F               |   Ток панели -X
    uint16_t CAN_Panal_current_pY;                                  //+33    |  const = 0x6061               |   Ток панели +Y
    uint16_t CAN_Panal_current_nY;                                  //+35    |  const = 0x6263               |   Ток панели -Y
    uint16_t CAN_Current_hinged_panels_satellite_1_plane;           //+37    |  const = 0x6465               |   Ток откидных панелей плоскость спутника 1  (LT8490)
    uint16_t CAN_Current_hinged_panels_satellite_2_plane;           //+39    |  const = 0x6667               |   Ток откидных панелей плоскость спутника 2  (LT8490)

    //Массив напряжений солнечных панелей                                |                               |
    uint16_t CAN_Panel_voltage_pX;                                  //+41    |  const = 0x6869               |   Напряжение панели +X
    uint16_t CAN_Panel_voltage_nX;                                  //+43    |  const = 0x6A6B               |   Напряжение панели -X
    uint16_t CAN_Panel_voltage_pY;                                  //+45    |  const = 0x6C6D               |   Напряжение панели +Y
    uint16_t CAN_Panel_voltage_nY;                                  //+47    |  const = 0x6E6F               |   Напряжение панели -Y
    uint16_t CAN_Voltage_hinged_panels_satellite_1_plane;           //+49    |  const = 0x7071               |   Напряжение откидных панелей плоскость спутника 1
    uint16_t CAN_Voltage_hinged_panels_satellite_2_plane;           //+51    |  const = 0x7273               |   Напряжение откидных панелей плосуость спутника 2

    //Массив медианных температур солнечных панелей                      |                               |
    uint8_t  CAN_Panel_median_temperature_pX;                       //+53    |  const = 0x74                 |   Медианная температура панели +X
    uint8_t  CAN_Panel_median_temperature_nX;                       //+54    |  const = 0x75                 |   Медианная температура панели -X
    uint8_t  CAN_Panel_median_temperature_pY;                       //+55    |  const = 0x76                 |   Медианная температура панели +Y
    uint8_t  CAN_Panel_median_temperature_nY;                       //+56    |  const = 0x77                 |   Медианная температура панели -Y
    uint8_t  CAN_Hinged_panel_median_temperature_pY;                //+57    |  const = 0x78                 |   Медианная температура откидной панели +Y
    uint8_t  CAN_Hinged_panel_median_temperature_nY;                //+58    |  const = 0x79                 |   Медианная температура откидной панели -Y
    uint8_t  CAN_Solar_panel_status[5];                             //+59    |  const = 0x7A7B7C7D7E         |   Статус солнечных панелей, битовая маска (в т.ч. ФЭП)

    //Массив уровня заряда АБ в %                                        |                               |
    uint8_t  CAN_Full_percent_charge_level;              			//+64    |  const = 0x7F                 |   Полный уровень заряда АБ в %

    //Массив уровня заряда АБ в mAh                                      |                               |
    uint16_t CAN_Capacity_charge_level_AB1_line1;                   //+65    |  const = 0x8081               |   Уровень заряда АБ1 ветвь 1
    uint16_t CAN_Capacity_charge_level_AB1_line2;                   //+67    |  const = 0x8283               |   Уровень заряда АБ1 ветвь 2
    uint16_t CAN_Capacity_charge_level_AB2_line1;                   //+69    |  const = 0x8485               |   Уровень заряда АБ2 ветвь 1
    uint16_t CAN_Capacity_charge_level_AB2_line2;                   //+71    |  const = 0x8687               |   Уровень заряда АБ2 ветвь 2
    uint16_t CAN_Capacity_charge_level_AB3_line1;                   //+73    |  const = 0x8889               |   Уровень заряда АБ3 ветвь 1
    uint16_t CAN_Capacity_charge_level_AB3_line2;                   //+75    |  const = 0x8A8B               |   Уровень заряда АБ3 ветвь 2
    uint16_t CAN_Full_capacity_charge_level;			            //+77    |  const = 0x8C8D               |   Полный уровень заряда АБ в mAh

    //Массив средних температур модулей АБ                               |                               |
    uint8_t  CAN_Average_temp_controller_AB1;                       //+79    |  const = 0x8E                 |   Средняя температура контроллера АБ1
    uint8_t  CAN_Average_line1_temp_AB1;                            //+80    |  const = 0x8F                 |   Средняя температура ветви 1 АБ1
    uint8_t  CAN_Average_line2_temp_AB1;                            //+81    |  const = 0x90                 |   Средняя температура ветви 2 АБ1
    uint8_t  CAN_Average_temp_controller_AB2;                       //+82    |  const = 0x91                 |   Средняя температура контроллера АБ2
    uint8_t  CAN_Average_line1_temp_AB2;                            //+83    |  const = 0x92                 |   Средняя температура ветви 1 АБ2
    uint8_t  CAN_Average_line2_temp_AB2;                            //+84    |  const = 0x93                 |   Средняя температура ветви 2 АБ2
    uint8_t  CAN_Average_temp_controller_AB3;                       //+85    |  const = 0x94                 |   Средняя температура контроллера АБ3
    uint8_t  CAN_Average_line1_temp_AB3;                            //+86    |  const = 0x95                 |   Средняя температура ветви 1 АБ3
    uint8_t  CAN_Average_line2_temp_AB3;                            //+87    |  const = 0x96                 |   Средняя температура ветви 2 АБ3
    uint8_t  CAN_AB_status[3];                                      //+88    |  const = 0x979899             |   Статус АБ, битовая маска

    //Массив командных регистров ключей разряда АБ                       |                               |
    uint16_t CAN_Charge_discharge_AB_key_status;                    //+91    |  const = 0x9A9B               |   Статус ключей заряда и разряда АБ, битовая маска

    //Массив токов питания подаваемого на подсистемы                      |                               |
    uint16_t CAN_Channel1_current;                                  //+93     |  const = 0x9C9D               |   Ток канал 1
    uint16_t CAN_Channel2_current;                                  //+95     |  const = 0x9E9F               |   Ток канал 2
    uint16_t CAN_Channel3_current;                                  //+97     |  const = 0xA0A1               |   Ток канал 3
    uint16_t CAN_Channel4_current;                                  //+99     |  const = 0xA2A3               |   Ток канал 4
    uint16_t CAN_Channel5_current;                                  //+101    |  const = 0xA4A5               |   Ток канал 5
    uint16_t CAN_Channel6_current;                                  //+103    |  const = 0xA6A7               |   Ток канал 6

    //Массив напряжения батарейной шины бортовой сети                     |                               |
    uint16_t CAN_VBAT1_voltage;                                     //+105    |  const = 0xA8A9               |   Напряжение VBAT1
    uint16_t CAN_VBAT2_voltage;                                     //+107    |  const = 0xAAAB               |   Напряжение VBAT2

    //Массив токов батарейной шины бортовой сети                          |                               |
    uint16_t CAN_VBAT1_current;                                     //+109    |  const = 0xACAD               |   Ток VBAT1
    uint16_t CAN_VBAT2_current;                                     //+111    |  const = 0xAEAF               |   Ток VBAT2

    //Массив командных регистров линий питания                            |                               |
    uint8_t  CAN_Subsystem_power_line_status;                       //+113    |  const = 0xB0                 |   Статус линии пит. подсистем, битовая маска
    uint16_t CAN_SES_current_consumption;                           //+114    |  const = 0xB1B2               |   Ток потребления модуля СЭС
    uint16_t CAN_SES_Voltage_power_supply;                          //+116    |  const = 0xB3B4               |   Напряжение питания модуля СЭС
    uint16_t CAN_Full_charge_discharge_power;                       //+118    |  const = 0xB5B6               |   Полная мощность зарядки/разрядки АБ
    uint16_t CAN_Total_power_SB;                                    //+120    |  const = 0xB7B8               |   Полная вырабатываемая мощность СБ
    uint16_t CAN_Spacecraft_total_power;                            //+122    |  const = 0xB9BA               |   Полная мощность потребления КА

    //Массив медианных температур модуля СЭС                             |                               |
    uint8_t  CAN_Median_PMM_temp;                                   //+124   |  const = 0xBB                 |   Медианная температура PMM (один датчик)
    uint8_t  CAN_Median_PAM_temp;                                   //+125   |  const = 0xBC                 |   Медианная температура PAM
    uint8_t  CAN_Median_PDM_temp;                                   //+126   |  const = 0xBD                 |   Медианная температура PDM
    uint8_t  CAN_SES_module_system_elements_status[5];              //+127   |  const = 0xBEBFC0C1C2         |   Статус элементов системы модуля СЭС, битовая маска
    uint32_t CAN_Number_of_restarts_of_the_SES_module;              //+132   |  const = 0xC3C4C5C6           |   Количество перезапусков основного модуля СЭС
    uint32_t CAN_Number_of_restarts_of_the_reserve_SES_module;      //+136   |  const = 0xC7C8C9CA           |   Количество перезапусков резервного модуля СЭС
    uint32_t CAN_SES_module_data_array1;                            //+140   |  const = 0xCBCCCDCE           |   Массив 1 данных о работе СЭС
    uint8_t  CAN_Primary_standby_switch;                            //+144   |  const = 0xCF                 |   Переключатель основной/резервный

    // ------------  ТМИ 7 --------------------//

    //Массив токов заряда/разряда АБ                                       |                               |
    uint16_t CAN_Charge_discharge_current_AB1_line1;                //+145     |  const = 0xB8B9               |  Ток заряда/разряда АБ1 ветвь 1
    uint16_t CAN_Charge_discharge_current_AB1_line2;                //+147     |  const = 0xBABB               |  Ток заряда/разряда АБ1 ветвь 2
    uint16_t CAN_Charge_discharge_current_AB2_line1;                //+149     |  const = 0xBCBD               |  Ток заряда/разряда АБ2 ветвь 1
    uint16_t CAN_Charge_discharge_current_AB2_line2;                //+151     |  const = 0xBEBF               |  Ток заряда/разряда АБ2 ветвь 2
    uint16_t CAN_Charge_discharge_current_AB3_line1;                //+153     |  const = 0xC0C1               |  Ток заряда/разряда АБ3 ветвь 1
    uint16_t CAN_Charge_discharge_current_AB3_line2;                //+155     |  const = 0xC2C3               |  Ток заряда/разряда АБ3 ветвь 2

    //Массив напряжений АБ                                                 |                               |
    uint16_t CAN_Voltage_AB1_line1_battery_1;                       //+157     |  const = 0xC4C5               |  Напряжение АБ1 ветвь 1 аккумулятор 1
    uint16_t CAN_Voltage_AB1_line1_battery_2;                       //+159     |  const = 0xC6C7               |  Напряжение АБ1 ветвь 1 аккумулятор 2
    uint16_t CAN_Voltage_AB1_line2_battery_1;                       //+161     |  const = 0xC8C9               |  Напряжение АБ1 ветвь 2 аккумулятор 1
    uint16_t CAN_Voltage_AB1_line2_battery_2;                       //+163     |  const = 0xCACB               |  Напряжение АБ1 ветвь 2 аккумулятор 2
    uint16_t CAN_Voltage_AB2_line1_battery_1;                       //+165     |  const = 0xCCCD               |  Напряжение АБ2 ветвь 1 аккумулятор 1
    uint16_t CAN_Voltage_AB2_line1_battery_2;                       //+167     |  const = 0xCECF               |  Напряжение АБ2 ветвь 1 аккумулятор 2
    uint16_t CAN_Voltage_AB2_line2_battery_1;                       //+169     |  const = 0xD0D1               |  Напряжение АБ2 ветвь 2 аккумулятор 1
    uint16_t CAN_Voltage_AB2_line2_battery_2;                       //+171     |  const = 0xD2D3               |  Напряжение АБ2 ветвь 2 аккумулятор 2
    uint16_t CAN_Voltage_AB3_line1_battery_1;                       //+173     |  const = 0xD4D5               |  Напряжение АБ3 ветвь 1 аккумулятор 1
    uint16_t CAN_Voltage_AB3_line1_battery_2;                       //+175     |  const = 0xD6D7               |  Напряжение АБ3 ветвь 1 аккумулятор 2
    uint16_t CAN_Voltage_AB3_line2_battery_1;                       //+177     |  const = 0xD8D9               |  Напряжение АБ3 ветвь 2 аккумулятор 1
    uint16_t CAN_Voltage_AB3_line2_battery_2;                       //+179     |  const = 0xDADB               |  Напряжение АБ3 ветвь 2 аккумулятор 2

    //Массив уровня заряда АБ в %                                          |                               |
    uint8_t  CAN_Percent_charge_level_AB1_line1;                    //+181     |  const = 0xDC                 |  Уровень заряда АБ1 ветвь 1
    uint8_t  CAN_Percent_charge_level_AB1_line2;                    //+182     |  const = 0xDD                 |  Уровень заряда АБ1 ветвь 2
    uint8_t  CAN_Percent_charge_level_AB2_line1;                    //+183     |  const = 0xDE                 |  Уровень заряда АБ2 ветвь 1
    uint8_t  CAN_Percent_charge_level_AB2_line2;                    //+184     |  const = 0xDF                 |  Уровень заряда АБ2 ветвь 2
    uint8_t  CAN_Percent_charge_level_AB3_line1;                    //+185     |  const = 0xE0                 |  Уровень заряда АБ3 ветвь 1
    uint8_t  CAN_Percent_charge_level_AB3_line2;                    //+186     |  const = 0xE1                 |  Уровень заряда АБ3 ветвь 2

    //Массив напряжений питания подаваемого на подсистемы                  |                               |
    uint16_t CAN_Channel1_voltage;                                  //+187     |  const = 0xE2E3               |  Напряжение канала 1
    uint16_t CAN_Channel2_voltage;                                  //+189     |  const = 0xE4E5               |  Напряжение канала 2
    uint16_t CAN_Channel3_voltage;                                  //+191     |  const = 0xE6E7               |  Напряжение канала 3
    uint16_t CAN_Channel4_voltage;                                  //+193     |  const = 0xE8E9               |  Напряжение канала 4
    uint16_t CAN_Channel5_voltage;                                  //+195     |  const = 0xEAEB               |  Напряжение канала 5
    uint16_t CAN_Channel6_voltage;                                  //+197     |  const = 0xECED               |  Напряжение канала 6

    //Массив медианных температур модуля СЭС                               |                               |
    uint16_t CAN_Opening_elements_state;                            //+199     |  const = 0xEEEF               |  Состояние концевиков раскрыв. элементов, битовая маска
    uint8_t  CAN_SES_module_data_array2[57];                        //+201     |  const = 0xF0F1F2F3F4F5F6F7F8 |  Массив 2 данных о работе СЭС
                                                                    //                      F9FAFBFCFDFEFF00   |
                                                                    //                      0102030405060708   |
                                                                    //                      090A0B0C0D0E0F10   |
                                                                    //                      1112131415161718   |
                                                                    //                      191A1B1C1D1E1F20   |
                                                                    //                      2122232425262728   |
    uint8_t  CAN_Reset_to_default;                                  //+258     |  const = 0x29                 |  Сброс параметров в состояние "по умолчанию"
    uint8_t  CAN_Reset_counter_to_default_state;                    //+259     |  const = 0x2A                 |  Счетчик сбросов параметров в состояние "по умолчанию"
    uint8_t  CAN_Perform_disclosure;                                //+260     |  const = 0x2B                 |  Выполнить раскрытие

    // ----------------------- ТМИ 8 -------------------        --------  	   |
    //Массив температур солнечных панелей                                      |
    uint8_t  CAN_Panal_temp_pX_sensor_1;                            //+261     |  const = 0x2C               |   Температура панели +X датчик 1
    uint8_t  CAN_Panal_temp_pX_sensor_2;                            //+262     |  const = 0x2D               |   Температура панели +X датчик 2
    uint8_t  CAN_Panal_temp_pX_sensor_3;                            //+263     |  const = 0x2E               |   Температура панели +X датчик 3
    uint8_t  CAN_Panal_temp_pX_sensor_4;                            //+264     |  const = 0x2F               |   Температура панели +X датчик 4
    uint8_t  CAN_Panal_temp_nX_sensor_1;                            //+265     |  const = 0x30               |   Температура панели -X датчик 1
    uint8_t  CAN_Panal_temp_nX_sensor_2;                            //+266     |  const = 0x31               |   Температура панели -X датчик 2
    uint8_t  CAN_Panal_temp_nX_sensor_3;                            //+267     |  const = 0x32               |   Температура панели -X датчик 3
    uint8_t  CAN_Panal_temp_nX_sensor_4;                            //+268     |  const = 0x33               |   Температура панели -X датчик 4
    uint8_t  CAN_Panal_temp_pY_sensor_1;                            //+269     |  const = 0x34               |   Температура панели +Y датчик 1
    uint8_t  CAN_Panal_temp_pY_sensor_2;                            //+270     |  const = 0x35               |   Температура панели +Y датчик 2
    uint8_t  CAN_Panal_temp_nY_sensor_1;                            //+271     |  const = 0x36               |   Температура панели -Y датчик 1
    uint8_t  CAN_Panal_temp_nY_sensor_2;                            //+272     |  const = 0x37               |   Температура панели -Y датчик 2
    uint8_t  CAN_Hinged_panel_temp_pY_sensor_1;                     //+273     |  const = 0x38               |   Температура откидной панели +Y датчик 1
    uint8_t  CAN_Hinged_panel_temp_pY_sensor_2;                     //+274     |  const = 0x39               |   Температура откидной панели +Y датчик 2
    uint8_t  CAN_Hinged_panel_temp_pY_sensor_3;                     //+275     |  const = 0x3A               |   Температура откидной панели +Y датчик 3
    uint8_t  CAN_Hinged_panel_temp_pY_sensor_4;                     //+276     |  const = 0x3B               |   Температура откидной панели +Y датчик 4
    uint8_t  CAN_Hinged_panel_temp_nY_sensor_1;                     //+277     |  const = 0x3C               |   Температура откидной панели -Y датчик 1
    uint8_t  CAN_Hinged_panel_temp_nY_sensor_2;                     //+278     |  const = 0x3D               |   Температура откидной панели -Y датчик 2
    uint8_t  CAN_Hinged_panel_temp_nY_sensor_3;                     //+279     |  const = 0x3E               |   Температура откидной панели -Y датчик 3
    uint8_t  CAN_Hinged_panel_temp_nY_sensor_4;                     //+280     |  const = 0x3F               |   Температура откидной панели -Y датчик 4

    //Массив температур модулей АБ
    uint8_t  CAN_AB1_controller_1_sensor;                           //+281     |  const = 0x40               |   АБ1 датчик контроллера 1
    uint8_t  CAN_AB1_controller_2_sensor;                           //+282     |  const = 0x41               |   АБ1 датчик контроллера 2
    uint8_t  CAN_AB1_sensor_on_board1;                              //+283     |  const = 0x42               |   АБ1 датчик на плате 1
    uint8_t  CAN_AB1_sensor_on_board2;                              //+284     |  const = 0x43               |   АБ1 датчик на плате 2
    uint8_t  CAN_AB1_sensor_on_board3;                              //+285     |  const = 0x44               |   АБ1 датчик на плате 3
    uint8_t  CAN_AB1_sensor_on_board4;                              //+286     |  const = 0x45               |   АБ1 датчик на плате 4
    uint8_t  CAN_AB2_controller_1_sensor;                           //+287     |  const = 0x46               |   АБ2 датчик контроллера 1
    uint8_t  CAN_AB2_controller_2_sensor;                           //+288     |  const = 0x47               |   АБ2 датчик контроллера 2
    uint8_t  CAN_AB2_sensor_on_board1;                              //+289     |  const = 0x48               |   АБ2 датчик на плате 1
    uint8_t  CAN_AB2_sensor_on_board2;                              //+290     |  const = 0x49               |   АБ2 датчик на плате 2
    uint8_t  CAN_AB2_sensor_on_board3;                              //+291     |  const = 0x4A               |   АБ2 датчик на плате 3
    uint8_t  CAN_AB2_sensor_on_board4;                              //+292     |  const = 0x4B               |   АБ2 датчик на плате 4
    uint8_t  CAN_AB3_controller_1_sensor;                           //+293     |  const = 0x4C               |   АБ3 датчик контроллера 1
    uint8_t  CAN_AB3_controller_2_sensor;                           //+294     |  const = 0x4D               |   АБ3 датчик контроллера 2
    uint8_t  CAN_AB3_sensor_on_board1;                              //+295     |  const = 0x4E               |   АБ3 датчик на плате 1
    uint8_t  CAN_AB3_sensor_on_board2;                              //+296     |  const = 0x4F               |   АБ3 датчик на плате 2
    uint8_t  CAN_AB3_sensor_on_board3;                              //+297     |  const = 0x50               |   АБ3 датчик на плате 3
    uint8_t  CAN_AB3_sensor_on_board4;                              //+298     |  const = 0x51               |   АБ3 датчик на плате 4

    //Массив командных регистров ключей разряда АБ
    uint8_t  CAN_AB1_Charge_key_branch_1;                           //+299     |  const = 0x52                 |   Командный регистр ключа заряда ветви 1 АБ1
    uint8_t  CAN_AB1_Discharge_key_branch_1;                        //+300     |  const = 0x53                 |   Командный регистр ключа разряда ветви 1 АБ1
    uint8_t  CAN_AB1_Charge_key_branch_2;                           //+301     |  const = 0x54                 |   Командный регистр ключа заряда ветви 2 АБ1
    uint8_t  CAN_AB1_Discharge_key_branch_2;                        //+302     |  const = 0x55                 |   Командный регистр ключа разряда ветви 2 АБ1
    uint8_t  CAN_AB2_Charge_key_branch_1;                           //+303     |  const = 0x56                 |   Командный регистр ключа заряда ветви 1 АБ2
    uint8_t  CAN_AB2_Discharge_key_branch_1;                        //+304     |  const = 0x57                 |   Командный регистр ключа разряда ветви 1 АБ2
    uint8_t  CAN_AB2_Charge_key_branch_2;                           //+305     |  const = 0x58                 |   Командный регистр ключа заряда ветви 2 АБ2
    uint8_t  CAN_AB2_Discharge_key_branch_2;                        //+306     |  const = 0x59                 |   Командный регистр ключа разряда ветви 2 АБ2
    uint8_t  CAN_AB3_Charge_key_branch_1;                           //+307     |  const = 0x5A                 |   Командный регистр ключа заряда ветви 1 АБ3
    uint8_t  CAN_AB3_Discharge_key_branch_1;                        //+308     |  const = 0x5B                 |   Командный регистр ключа разряда ветви 1 АБ3
    uint8_t  CAN_AB3_Charge_key_branch_2;                           //+309     |  const = 0x5C                 |   Командный регистр ключа заряда ветви 2 АБ3
    uint8_t  CAN_AB3_Discharge_key_branch_2;                        //+310     |  const = 0x5D                 |   Командный регистр ключа разряда ветви 2 АБ3

    //Массив командных регистров линий питания
    uint8_t  CAN_Line1;                                             //+311     |  const = 0x5E                 |   Командный регистр линии 1 (СОП1)
    uint8_t  CAN_Line2;                                             //+312     |  const = 0x5F                 |   Командный регистр линии 2 (СОП2)
    uint8_t  CAN_Line3;                                             //+313     |  const = 0x60                 |   Командный регистр линии 3 (БРК1)
    uint8_t  CAN_Line4;                                             //+314     |  const = 0x61                 |   Командный регистр линии 4 (БРК2)
    uint8_t  CAN_Line5;                                             //+315     |  const = 0x62                 |   Командный регистр линии 5 ( - )
    uint8_t  CAN_Line6;                                             //+316     |  const = 0x63                 |   Командный регистр линии 6 ( - )
    uint8_t  CAN_Line_VBAT1;                                        //+317     |  const = 0x64                 |   Командный регистр линии VBAT1 (МС)
    uint8_t  CAN_Line_VBAT2;                                        //+318     |  const = 0x65                 |   Командный регистр линии VBAT2 (МС)

    //Массив командных регистров линий питания                             |                             |
    uint16_t CAN_SES_internal_bus_voltage;                          //+319     |  const = 0x6667             |   Напряжение внутренней шины СЭС

    //Массив температур модуля СЭС                                         |                             |
    uint8_t  CAN_PMM_sensor1;                                       //+321     |  const = 0x68               |   PMM датчик 1
    uint8_t  CAN_PAM_sensor1;                                       //+322     |  const = 0x69               |   PAM датчик 1
    uint8_t  CAN_PAM_sensor2;                                       //+323     |  const = 0x6A               |   PAM датчик 2
    uint8_t  CAN_PAM_sensor3;                                       //+324     |  const = 0x6B               |   PAM датчик 3
    uint8_t  CAN_PAM_sensor4;                                       //+325     |  const = 0x6C               |   PAM датчик 4
    uint8_t  CAN_PDM_sensor1;                                       //+326     |  const = 0x6D               |   PDM датчик 1
    uint8_t  CAN_PDM_sensor2;                                       //+327     |  const = 0x6E               |   PDM датчик 2
    uint8_t  CAN_PDM_sensor3;                                       //+328     |  const = 0x6F               |   PDM датчик 3
    uint8_t  CAN_PDM_sensor4;                                       //+329     |  const = 0x70               |   PDM датчик 4
    uint8_t  CAN_data_array3[47];                                   //+330     |  const = 0x717273747576     |   Массив 3 данных о работе СЭС
                                                                           //           7778797A7B7C
                                                                           //           7D7E7F808182
                                                                           //           838485868788
                                                                           //           898A8B8C8D8E
                                                                           //           8F9091929394
                                                                           //           95969798999A
                                                                           //           9B9C9D9E9F
                                                                //+377

    // size ТМИ0+ТМИ4+ТМИ7+ТМИ8 = 29 + 3 * 116 = 377

} ;

#pragma pack(pop)


#pragma pack(push, 2)

typedef struct {
  void *VarPtr;
  uint32_t VarLeng;
  void (*CallBackProc)(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state);
  uint8_t ivar_id;
  uint8_t access_flgs;  //[0]=1 - read only; [1]=1 - regardless offset; [2]=1 - without filter
}CAN_typeRegistrationRec;

#pragma pack(pop)

void CAN_ProcCallbackCmds(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state);
void CAN_ProcCallbackTelemetry(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state);

int8_t CAN_RegisterAllVars();

#ifdef __cplusplus
}
#endif

#endif


