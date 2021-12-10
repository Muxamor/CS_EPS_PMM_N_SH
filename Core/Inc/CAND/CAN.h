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
#define CAN_Constant_mode_offset            4

// Offset Command of PMM
#define CAN_EPS_Mode_offset                 16
#define CAN_Switch_active_CPU_offset        17
#define CAN_Reset_to_default_offset         18
#define CAN_Perform_Deploy_offset           19
#define CAN_PMM_PWR_OFF_Passive_CPU_offset  20
#define CAN_PMM_Reboot_Passive_CPU_offset   21
#define CAN_PMM_PWR_CAN_main_offset         22
#define CAN_PMM_PWR_CAN_backup_offset       23
#define CAN_PWR_VBAT1_key1_offset		    24
#define CAN_PWR_VBAT1_key2_offset		    25
#define CAN_PWR_VBAT2_key1_offset		    26
#define CAN_PWR_VBAT2_key2_offset		    27
#define CAN_PMM_PWR_PBM_Logic_offset        28

// Offset Command of PDM
#define CAN_PWR_CH1_offset			        100
#define CAN_PWR_CH2_offset			        101
#define CAN_PWR_CH3_offset                  102
#define CAN_PWR_CH4_offset			        103
#define CAN_PWR_CH5_offset			        104
#define CAN_PWR_CH6_offset			        105


//Offset Command of PAM
#define CAN_PAM_PWR_DC_DC_offset            200
#define CAN_PAM_PWR_LDO_offset              201
#define CAN_PAM_PWR_TM_SP_CH1_offset        202
#define CAN_PAM_PWR_TM_SP_CH2_offset        203
#define CAN_PAM_PWR_TM_SP_CH3_offset        204
#define CAN_PAM_PWR_TM_SP_CH4_offset        205
#define CAN_PAM_PWR_TM_SP_CH5_offset        206
#define CAN_PAM_PWR_TM_SP_CH6_offset        207

//Offset Command of PBMs

#define CAN_AB1_Charge_key_1_offset         300
#define CAN_AB1_Discharge_key_1_offset      301
#define CAN_AB1_Charge_key_2_offset         302
#define CAN_AB1_Discharge_key_2_offset      303
#define CAN_AB2_Charge_key_1_offset         304
#define CAN_AB2_Discharge_key_1_offset      305
#define CAN_AB2_Charge_key_2_offset	        306
#define CAN_AB2_Discharge_key_2_offset      307
#define CAN_AB3_Charge_key_1_offset         308
#define CAN_AB3_Discharge_key_1_offset      309
#define CAN_AB3_Charge_key_2_offset	        310
#define CAN_AB3_Discharge_key_2_offset      311
#define CAN_AB4_Charge_key_1_offset         312
#define CAN_AB4_Discharge_key_1_offset      313
#define CAN_AB4_Charge_key_2_offset	        314
#define CAN_AB4_Discharge_key_2_offset      315

#define CAN_AB1_Heat_Branch1_offset         332
#define CAN_AB1_Heat_Branch2_offset         333
#define CAN_AB2_Heat_Branch1_offset         334
#define CAN_AB2_Heat_Branch2_offset         335
#define CAN_AB3_Heat_Branch1_offset         336
#define CAN_AB3_Heat_Branch2_offset         337
#define CAN_AB4_Heat_Branch1_offset         338
#define CAN_AB4_Heat_Branch2_offset         339

#define CAN_PBM1_Emerg_Charge_Key_Branch1_offset 348
#define CAN_PBM1_Emerg_Charge_Key_Branch2_offset 349
#define CAN_PBM2_Emerg_Charge_Key_Branch1_offset 350
#define CAN_PBM2_Emerg_Charge_Key_Branch2_offset 351
#define CAN_PBM3_Emerg_Charge_Key_Branch1_offset 352

#define CAN_PBM3_Emerg_Charge_Key_Branch2_offset 353
#define CAN_PBM4_Emerg_Charge_Key_Branch1_offset 354
#define CAN_PBM4_Emerg_Charge_Key_Branch2_offset 355

#define CAN_PBM1_Corr_Charge_Level_Branch1_offset 364
#define CAN_PBM1_Corr_Charge_Level_Branch2_offset 365
#define CAN_PBM2_Corr_Charge_Level_Branch1_offset 366
#define CAN_PBM2_Corr_Charge_Level_Branch2_offset 367
#define CAN_PBM3_Corr_Charge_Level_Branch1_offset 368
#define CAN_PBM3_Corr_Charge_Level_Branch2_offset 369
#define CAN_PBM4_Corr_Charge_Level_Branch1_offset 370
#define CAN_PBM4_Corr_Charge_Level_Branch2_offset 371

#define CAN_PBM1_Reset_Branch1_offset 380
#define CAN_PBM1_Reset_Branch2_offset 381
#define CAN_PBM2_Reset_Branch1_offset 382
#define CAN_PBM2_Reset_Branch2_offset 383
#define CAN_PBM3_Reset_Branch1_offset 384
#define CAN_PBM3_Reset_Branch2_offset 385
#define CAN_PBM4_Reset_Branch1_offset 386
#define CAN_PBM4_Reset_Branch2_offset 387



extern struct CAN_IVar5 CAN_IVar5_telemetry;
extern struct CAN_IVar4  CAN_IVar4_RegCmd;


struct CAN_IN_Buffer{
    uint16_t length;
    uint16_t CAN_IN_BUFF[64];
};

extern struct CAN_IN_Buffer CAN_cmd_Buff;

extern uint8_t CAN1_exchange_data_flag;  // 0 - no exchange   1 - was exchange
extern uint8_t CAN2_exchange_data_flag;// 0 - no exchange  1 - was exchange

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

    // Command of PMM
    uint8_t CAN_EPS_Mode;                   //+16			Установить режим СЭС
    uint8_t CAN_Switch_active_CPU;          //+17		    Переключатель основной/резервный
    uint8_t CAN_Reset_to_default;           //+18           Сброс параметров в состояние "по умолчанию"
    uint8_t CAN_Perform_Deploy;             //+19           Выполнить раскрытие
    uint8_t CAN_PMM_PWR_OFF_Passive_CPU;    //+20           Вкл/Выкл посивный CPU
    uint8_t CAN_PMM_Reboot_Passive_CPU;     //+21           Перезагрузить посивный CPU
    uint8_t CAN_PMM_PWR_CAN_main;           //+22           Вкл/Выкл питания основного CAN
    uint8_t CAN_PMM_PWR_CAN_backup;         //+23           Вкл/Выкл питания резервного CAN
    uint8_t CAN_PMM_PWR_VBAT1_key1;         //+24           Командный регистр линии VBAT1 (МС)
    uint8_t CAN_PMM_PWR_VBAT1_key2;         //+25           Командный регистр линии VBAT1 (МС)
    uint8_t CAN_PMM_PWR_VBAT2_key1;         //+26           Командный регистр линии VBAT2 (МС)
    uint8_t CAN_PMM_PWR_VBAT2_key2;         //+27           Командный регистр линии VBAT2 (МС)
    uint8_t CAN_PMM_PWR_PBM_Logic;          //+28           Вкл/Выкл питания логики PBM
    uint8_t CAN_Comd_Reserved_29;           //+29           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_30;           //+30           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_31;           //+31           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_32;           //+30           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_33;           //+31           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_34;           //+32           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_35;           //+33           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_36;           //+34           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_37;           //+35           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_38;           //+36           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_39;           //+37           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_40;           //+38           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_41;           //+39           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_42;           //+40           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_43;           //+41           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_44;           //+42           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_45;           //+43           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_46;           //+44           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_47;           //+45           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_48;           //+46           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_49;           //+47           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_50;           //+48           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_51;           //+49           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_52;           //+50           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_53;           //+51           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_54;           //+52           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_55;           //+53           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_56;           //+54           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_57;           //+55           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_58;           //+56           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_59;           //+57           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_60;           //+58           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_61;           //+59           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_62;           //+60           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_63;           //+61           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_64;           //+62           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_65;           //+63           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_66;           //+64           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_67;           //+65           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_68;           //+66           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_69;           //+67           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_70;           //+68           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_71;           //+69           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_72;           //+70           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_73;           //+71           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_74;           //+72           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_75;           //+73           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_76;           //+74           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_77;           //+75           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_78;           //+76           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_79;           //+77           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_80;           //+78           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_81;           //+79           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_82;           //+80           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_83;           //+81           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_84;           //+82           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_85;           //+83           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_86;           //+84           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_87;           //+85           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_88;           //+86           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_89;           //+87           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_90;           //+88           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_91;           //+89           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_92;           //+90           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_93;           //+91           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_94;           //+92           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_95;           //+93           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_96;           //+94           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_97;           //+95           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_98;           //+96           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_99;           //+97           Командный регистр зарезервирован

    // Command of PDM
    uint8_t CAN_PDM_PWR_CH1;                //+100          Командный регистр канал питаня 1
    uint8_t CAN_PDM_PWR_CH2;                //+101          Командный регистр канал питаня 2
    uint8_t CAN_PDM_PWR_CH3;                //+102          Командный регистр канал питаня 3
    uint8_t CAN_PDM_PWR_CH4;                //+103          Командный регистр канал питаня 4
    uint8_t CAN_PDM_PWR_CH5;                //+104          Командный регистр канал питаня 5
    uint8_t CAN_PDM_PWR_CH6;                //+105          Командный регистр канал питаня 6
    uint8_t CAN_Comd_Reserved_106;          //+106          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_107;          //+107          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_108;          //+108          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_109;          //+109          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_110;          //+110          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_111;          //+111          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_112;          //+112          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_113;          //+113          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_114;          //+114          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_115;          //+115          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_116;          //+116          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_117;          //+117          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_118;          //+118          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_119;          //+119          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_120;          //+120          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_121;          //+121          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_122;          //+122          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_123;          //+123          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_124;          //+124          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_125;          //+125          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_126;          //+126          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_127;          //+127          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_128;          //+128          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_129;          //+129          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_130;          //+130          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_131;          //+131          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_132;          //+132          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_133;          //+133          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_134;          //+134          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_135;          //+135          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_136;          //+136          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_137;          //+137          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_138;          //+138          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_139;          //+139          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_140;          //+140          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_141;          //+141          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_142;          //+142          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_143;          //+143          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_144;          //+144          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_145;          //+145          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_146;          //+146          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_147;          //+147          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_148;          //+148          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_149;          //+149          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_150;          //+150          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_151;          //+151          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_152;          //+152          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_153;          //+153          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_154;          //+154          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_155;          //+155          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_156;          //+156          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_157;          //+157          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_158;          //+158          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_159;          //+159          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_160;          //+160          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_161;          //+161          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_162;          //+162          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_163;          //+163          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_164;          //+164          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_165;          //+165          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_166;          //+166          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_167;          //+167          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_168;          //+168          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_169;          //+169          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_170;          //+170          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_171;          //+171          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_172;          //+172          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_173;          //+173          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_174;          //+174          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_175;          //+175          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_176;          //+176          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_177;          //+177          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_178;          //+178          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_179;          //+179          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_180;          //+180          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_181;          //+181          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_182;          //+182          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_183;          //+183          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_184;          //+184          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_185;          //+185          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_186;          //+186          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_187;          //+187          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_188;          //+188          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_189;          //+189          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_190;          //+190          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_191;          //+191          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_192;          //+192          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_193;          //+193          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_194;          //+194          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_195;          //+195          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_196;          //+196          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_197;          //+197          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_198;          //+198          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_199;          //+199          Командный регистр зарезервирован

    // Command of PAM
    uint8_t CAN_PAM_PWR_DC_DC;              //+200          Вкл/Выкл DC_DC источник питания PAM
    uint8_t CAN_PAM_PWR_LDO;           	    //+201          Вкл/Выкл LDO источник питания PAM
    uint8_t CAN_PAM_PWR_TM_SP_CH1;          //+202          Вкл/Выкл питания телеметрии солнечных панелей канал №1
    uint8_t CAN_PAM_PWR_TM_SP_CH2;          //+203          Вкл/Выкл питания телеметрии солнечных панелей канал №2
    uint8_t CAN_PAM_PWR_TM_SP_CH3;          //+204          Вкл/Выкл питания телеметрии солнечных панелей канал №3
    uint8_t CAN_PAM_PWR_TM_SP_CH4;          //+205          Вкл/Выкл питания телеметрии солнечных панелей канал №4
    uint8_t CAN_PAM_PWR_TM_SP_CH5;          //+206          Вкл/Выкл питания телеметрии солнечных панелей канал №5
    uint8_t CAN_PAM_PWR_TM_SP_CH6;          //+207          Вкл/Выкл питания телеметрии солнечных панелей канал №6
    uint8_t CAN_Comd_Reserved_208;          //+208          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_209;          //+209          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_210;          //+210          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_211;          //+211          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_212;          //+212          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_213;          //+213          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_214;          //+214          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_215;          //+215          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_216;          //+216          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_217;          //+217          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_218;          //+218          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_219;          //+219          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_220;          //+220          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_221;          //+221          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_222;          //+222          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_223;          //+223          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_224;          //+224          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_225;          //+225          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_226;          //+226          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_227;          //+227          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_228;          //+228          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_229;          //+229          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_230;          //+230          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_231;          //+231          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_232;          //+232          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_233;          //+233          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_234;          //+234          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_235;          //+235          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_236;          //+236          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_237;          //+237          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_238;          //+238          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_239;          //+239          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_240;          //+240          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_241;          //+241          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_242;          //+242          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_243;          //+243          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_244;          //+244          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_245;          //+245          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_246;          //+246          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_247;          //+247          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_248;          //+248          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_249;          //+249          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_250;          //+250          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_251;          //+251          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_252;          //+252          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_253;          //+253          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_254;          //+254          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_255;          //+255          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_256;          //+256          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_257;          //+257          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_258;          //+258          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_259;          //+259          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_260;          //+260          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_261;          //+261          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_262;          //+262          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_263;          //+263          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_264;          //+264          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_265;          //+265          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_266;          //+266          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_267;          //+267          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_268;          //+268          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_269;          //+269          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_270;          //+270          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_271;          //+271          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_272;          //+272          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_273;          //+273          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_274;          //+274          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_275;          //+275          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_276;          //+276          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_277;          //+277          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_278;          //+278          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_279;          //+279          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_280;          //+280          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_281;          //+281          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_282;          //+282          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_283;          //+283          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_284;          //+284          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_285;          //+285          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_286;          //+286          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_287;          //+287          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_288;          //+288          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_289;          //+289          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_290;          //+290          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_291;          //+291          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_292;          //+292          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_293;          //+293          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_294;          //+294          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_295;          //+295          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_296;          //+296          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_297;          //+297          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_298;          //+298          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_299;          //+299          Командный регистр зарезервирован


    // Command of PBMs
    uint8_t CAN_PBM1_Charge_key_branch_1;           //+300          Командный регистр ключа заряда ветви 1 АБ1
    uint8_t CAN_PBM1_Discharge_key_branch_1;        //+301          Командный регистр ключа разряда ветви 1 АБ1
    uint8_t CAN_PBM1_Charge_key_branch_2;  		    //+302          Командный регистр ключа заряда ветви 2 АБ1
    uint8_t CAN_PBM1_Discharge_key_branch_2;        //+303          Командный регистр ключа разряда ветви 2 АБ1
    uint8_t CAN_PBM2_Charge_key_branch_1;   	    //+304          Командный регистр ключа заряда ветви 1 АБ2
    uint8_t CAN_PBM2_Discharge_key_branch_1;   	    //+305          Командный регистр ключа разряда ветви 1 АБ2
    uint8_t CAN_PBM2_Charge_key_branch_2;        	//+306          Командный регистр ключа заряда ветви 2 АБ2
    uint8_t CAN_PBM2_Discharge_key_branch_2;   	    //+307          Командный регистр ключа разряда ветви 2 АБ2
    uint8_t CAN_PBM3_Charge_key_branch_1;       	//+308          Командный регистр ключа заряда ветви 1 АБ3
    uint8_t CAN_PBM3_Discharge_key_branch_1;   	    //+309          Командный регистр ключа разряда ветви 1 АБ3
    uint8_t CAN_PBM3_Charge_key_branch_2;   	    //+310          Командный регистр ключа заряда ветви 2 АБ3
    uint8_t CAN_PBM3_Discharge_key_branch_2;   	    //+311          Командный регистр ключа разряда ветви 2 АБ3
    uint8_t CAN_PBM4_Charge_key_branch_1;   	    //+312          Командный регистр ключа заряда ветви 1 АБ4
    uint8_t CAN_PBM4_Discharge_key_branch_1;   	    //+313          Командный регистр ключа разряда ветви 1 АБ4
    uint8_t CAN_PBM4_Charge_key_branch_2;   	    //+314          Командный регистр ключа заряда ветви 2 АБ4
    uint8_t CAN_PBM4_Discharge_key_branch_2;   	    //+315          Командный регистр ключа разряда ветви 2 АБ4
    uint8_t CAN_Comd_Reserved_316;                  //+316          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_317;                  //+317          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_318;                  //+318          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_319;                  //+319          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_320;                  //+320          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_321;                  //+321          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_322;                  //+322          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_323;                  //+323          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_324;                  //+324          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_325;                  //+325          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_326;                  //+326          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_327;                  //+327          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_328;                  //+328          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_329;                  //+329          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_330;                  //+330          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_331;                  //+331          Командный регистр зарезервирован

    uint8_t CAN_PBM1_Heat_Branch1;                  //+332          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 1
    uint8_t CAN_PBM1_Heat_Branch2;                  //+333          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 1
    uint8_t CAN_PBM2_Heat_Branch1;                  //+334          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 2
    uint8_t CAN_PBM2_Heat_Branch2;                  //+335          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 2
    uint8_t CAN_PBM3_Heat_Branch1;                  //+336          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 3
    uint8_t CAN_PBM3_Heat_Branch2;                  //+337          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 3
    uint8_t CAN_PBM4_Heat_Branch1;                  //+338          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 4
    uint8_t CAN_PBM4_Heat_Branch2;                  //+339          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 4
    uint8_t CAN_Comd_Reserved_340;                  //+340          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_341;                  //+341          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_342;                  //+342          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_343;                  //+343          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_344;                  //+344          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_345;                  //+345          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_346;                  //+346          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_347;                  //+347          Командный регистр зарезервирован

    uint8_t CAN_PBM1_Emerg_Charge_Key_Branch1;      //+348          Вкл/Выкл аварийную зарядки ветви 1 батарейного модуля 1
    uint8_t CAN_PBM1_Emerg_Charge_Key_Branch2;      //+349          Вкл/Выкл аварийную зарядки ветви 2 батарейного модуля 1
    uint8_t CAN_PBM2_Emerg_Charge_Key_Branch1;      //+350          Вкл/Выкл аварийную зарядки ветви 1 батарейного модуля 2
    uint8_t CAN_PBM2_Emerg_Charge_Key_Branch2;      //+351          Вкл/Выкл аварийную зарядки ветви 2 батарейного модуля 2
    uint8_t CAN_PBM3_Emerg_Charge_Key_Branch1;      //+352          Вкл/Выкл аварийную зарядки ветви 1 батарейного модуля 3
    uint8_t CAN_PBM3_Emerg_Charge_Key_Branch2;      //+353          Вкл/Выкл аварийную зарядки ветви 2 батарейного модуля 3
    uint8_t CAN_PBM4_Emerg_Charge_Key_Branch1;      //+354          Вкл/Выкл аварийную зарядки ветви 1 батарейного модуля 4
    uint8_t CAN_PBM4_Emerg_Charge_Key_Branch2;      //+355          Вкл/Выкл аварийную зарядки ветви 2 батарейного модуля 4
    uint8_t CAN_Comd_Reserved_356;                  //+356          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_357;                  //+357          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_358;                  //+358          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_359;                  //+359          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_360;                  //+360          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_361;                  //+361          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_362;                  //+362          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_363;                  //+363          Командный регистр зарезервирован

    uint8_t CAN_PBM1_Corr_Charge_Level_Branch1;         //+364
    uint8_t CAN_PBM1_Corr_Charge_Level_Branch2;         //+365
    uint8_t CAN_PBM2_Corr_Charge_Level_Branch1;         //+366
    uint8_t CAN_PBM2_Corr_Charge_Level_Branch2;         //+367
    uint8_t CAN_PBM3_Corr_Charge_Level_Branch1;         //+368
    uint8_t CAN_PBM3_Corr_Charge_Level_Branch2;         //+369
    uint8_t CAN_PBM4_Corr_Charge_Level_Branch1;         //+370
    uint8_t CAN_PBM4_Corr_Charge_Level_Branch2;         //+371
    uint8_t CAN_Comd_Reserved_372;                      //+372          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_373;                      //+373          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_374;                      //+374          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_375;                      //+375          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_376;                      //+376          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_377;                      //+377          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_378;                      //+378          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_379;                      //+379          Командный регистр зарезервирован

    uint8_t CAN_PBM1_Reset_Branch1;                     //+380
    uint8_t CAN_PBM1_Reset_Branch2;                     //+381
    uint8_t CAN_PBM2_Reset_Branch1;                     //+382
    uint8_t CAN_PBM2_Reset_Branch2;                     //+383
    uint8_t CAN_PBM3_Reset_Branch1;                     //+384
    uint8_t CAN_PBM3_Reset_Branch2;                     //+385
    uint8_t CAN_PBM4_Reset_Branch1;                     //+386
    uint8_t CAN_PBM4_Reset_Branch2;                     //+387
    uint8_t CAN_Comd_Reserved_388;                      //+388          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_389;                      //+389          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_390;                      //+390          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_391;                      //+391          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_392;                      //+392          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_393;                      //+393          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_394;                      //+394          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_395;                      //+395          Командный регистр зарезервирован

};



#pragma pack(push, 1)

struct CAN_IVar5{
    //------------------------------------------------  Offset -------- Constant ---------------------- Description ------------------------//
    // -------------------  Beacon  ------------------ //
    uint8_t Protaction_GAP_Beacon[64];
    uint8_t  CAN_Beacon_PMM_Switch_Active_CPU;				        //+64         |  const = 0x                 |  Переключатель основной/резервный МАЯК
    uint16_t CAN_Beacon_EPS_Main_Bus_Voltage;				        //+        |  const = 0x                 |  Напряжение внутренней силовой шины СЭС
    int16_t CAN_Beacon_EPS_Total_PBM_Char_Dischar_Power;            //+        |  const = 0x                 |  Полная мощность зарядки/разрядки АБ
    uint16_t CAN_Beacon_EPS_Total_PAM_Generation_Power;             //+        |  const = 0x                 |  Полная вырабатываемая мощность СБ
    uint16_t CAN_Beacon_EPS_CubeSat_Total_Cons_Power;               //+        |  const = 0x                 |  Полная потребляемая мощность
    uint16_t CAN_Beacon_EPS_Total_Battery_Level_mAh;			    //+        |  const = 0x                 |  Полный уровень заряда АБ в mAh
    int8_t  CAN_Beacon_PMM_Temp_Sensor;                             //+        |  const = 0x                 |  PMM датчик 1
    int8_t  CAN_Beacon_SP_TM_Ch1_Median_Temp;                       //+        |  const = 0x                 |  Медианная температура панели +X
    int8_t  CAN_Beacon_SP_TM_Ch2_Median_Temp;                       //+        |  const = 0x                 |  Медианная температура панели -X
    int8_t  CAN_Beacon_SP_TM_Ch3_Median_Temp;                       //+        |  const = 0x                 |  Медианная температура панели +Y
    int8_t  CAN_Beacon_SP_TM_Ch4_Median_Temp;                       //+        |  const = 0x                 |  Медианная температура панели -Y
    int8_t  CAN_Beacon_SP_TM_Ch5_Median_Temp;                       //+        |  const = 0x                 |  Медианная температура откидной панели +Y
    int8_t  CAN_Beacon_SP_TM_Ch6_Median_Temp;                       //+        |  const = 0x                 |  Медианная температура откидной панели -Y
    uint8_t  CAN_Beacon_PAM_In_PwrCh_ID_State_BitsMask;             //+        |  const = 0x                 |  Ideal Diode
    uint16_t  CAN_Beacon_SP_State_BitsMask_BEACON;                  //+        |  const = 0x                 |  Статус солнечных панелей, битовая маска (в т.ч. ФЭП) (МАЯК)
    uint16_t CAN_Beacon_PBM_Chrg_Dichrg_Key_Status_BitMask;         //+        |  const = 0x                 | Статус ключей заряда/разряда
    uint8_t  CAN_Beacon_PBM_Heating_Element_State_BitMask;          //+        |  const = 0x                 | Текущее состояние Термостата
    uint8_t  CAN_Beacon_PBM_Error_Status_BitsMask_BEACON[3];        //+        |  const = 0x                 | Статусы отказов (ошибок)АБ, битовая маска (МАЯК)
    uint8_t  CAN_Beacon_EPS_Error_Status_BitsMask_BEACON[2];        //+        |  const = 0x                 | Статус отказов элементов СЭС, битовая маска (МАЯК)
                                                                    //Total 29 byte

    // -------------------  ТМИ 0  ------------------ //
    uint8_t Protaction_GAP_TMI0[64];                                //+        |  const = 0x
    uint16_t CAN_TMI0_version;                                  	//+        |  const = 0x                   |   Версия ТМИ

    // PMM //
    uint8_t  CAN_TMIx_PMM_EPS_Const_Mode;                           //+        |  const = 0x                   |   Режим констант
    uint8_t  CAN_TMIx_PMM_EPS__Mode;                                //+        |  const = 0x                   |   Режим работы EPS
    uint8_t  CAN_TMIx_PMM_Switch_Active_CPU;                        //+        |  const = 0x                   |   Переключатель основной/резервный
    uint8_t  CAN_TMIx_PMM_OFF_PWR_Passive_CPU;                      //+        |  const = 0x                   |   Упарвление питанием пассивного CPU
    int8_t  CAN_TMIx_PMM_Temp_Sensor;                               //+         |  const = 0x                    |   PMM датчик 1
    uint16_t CAN_TMIx_PMM_PWR_Ch_State_BitMask;                     //+     |  const = 0x                   |   Состояние ключа питаня PMM битовая маска: PWR_Ch_State_CANmain, PWR_Ch_State_CANbackup, PWR_Ch_State_Vbat1_eF1, PWR_Ch_State_Vbat1_eF2, PWR_Ch_State_Vbat2_eF1
                                                                    //                                        |   PWR_Ch_State_Vbat2_eF2, PWR_Ch_State_PBMs_Logic, PWR_Ch_State_Deploy_Logic, PWR_Ch_State_Deploy_Power,
    uint16_t CAN_TMIx_PMM_PWR_PG_BitMask;                           //+     |  const = 0x                   |   Статус PG линий питания PMM битовая маска:PWR_Supply_Main_PG, PWR_Supply_Backup_PG, PWR_Ch_PG_CANmain,PWR_Ch_PG_CANbackup,  PWR_Ch_PG_Vbat1_eF1,  PWR_Ch_PG_Vbat1_eF2
                                                                    //                                        |   PWR_Ch_PG_Vbat2_eF1, PWR_Ch_PG_Vbat2_eF2, PWR_Ch_PG_PBMs_Logic, PWR_Ch_PG_Deploy_LP
    uint32_t CAN_TMIx_PMM_Error_BitMask;                            //+     |  const = 0x               |   Статус ошибок модуля PMM, битовая маска:
    uint32_t CAN_TMIx_PMM_Reboot_Counter_Main_CPU;                  //+     |  const = 0x               |   Количество перезапусков основного CPU
    uint32_t CAN_TMIx_PMM_Reboot_Counter_Backup_CPU;                //+     |  const = 0x               |   Количество перезапусков резервного CPU
    uint16_t CAN_TMIx_PMM_VBAT1_Voltage;                            //+     |  const = 0x                   |   Напряжение VBAT1
    uint16_t CAN_TMIx_PMM_VBAT2_Voltage;                            //+     |  const = 0x                   |   Напряжение VBAT2
    uint16_t CAN_TMIx_PMM_VBAT1_Voltage_Aver_10s;                   //+     |  const = 0x               |   Напряжение VBAT1 усреднение 10s
    uint16_t CAN_TMIx_PMM_VBAT2_Voltage_Aver_10s;                   //+     |  const = 0x               |   Напряжение VBAT2 усреднение 10s
    int16_t CAN_TMIx_PMM_VBAT1_Current;                             //+     |  const = 0x                   |   Ток VBAT1
    int16_t CAN_TMIx_PMM_VBAT2_Current;                             //+     |  const = 0x                   |   Ток VBAT2
    int16_t CAN_TMIx_PMM_VBAT1_Current_Aver_10s;                    //+     |  const = 0x               |   Ток VBAT1 усреднение 10s
    int16_t CAN_TMIx_PMM_VBAT2_Current_Aver_10s;                    //+     |  const = 0x               |   Ток VBAT2 усреднение 10s
    uint16_t CAN_TMIx_PMM_Own_Current_Consumption;                  //+  |  const = 0x                   |   Ток потребления модуля PMM
    uint16_t CAN_TMIx_PMM_Voltage;                                  //+  |  const = 0x                   |   Напряжение питания модуля PMM 3.3V
    uint16_t CAN_TMIx_EPS_Main_Bus_Voltage;                         //+   |  const = 0x                   |   Напряжение внутренней шины СЭС
    uint16_t CAN_TMIx_EPS_CubeSat_Total_Cons_Power;                 //+  |  const = 0x                   |   Полная мощность потребления КА
    uint16_t CAN_TMIx_EPS_Deploy_Status;                            //+  |  const = 0x                   |  Состояние концевиков раскрыв. элементов, битовая маска
    uint16_t CAN_TMIx_PMM_Version_FW;                               //+  |  const = 0x                   |   Firmware version
                                                                    //Total 54 byte
    // PDM //
    uint16_t CAN_TMIx_PDM_PWR_Ch_State_BitsMask;                    //+        |  const = 0x       |   Командный регистр PDM линии питания 1..6 State_eF_in State_eF_out
    uint16_t CAN_TMIx_PDM_PWR_PG_BitsMask;                          //+        |  const = 0x       |   Power Good каналов питания PDM 1..6 PG_eF_in PG_eF_out
    uint32_t CAN_TMIx_PDM_Error_BitMask;                            //+        | const = 0x   |   Статус ошибок модуля PDM, битовая маска:Error_State_eF_in, Error_State_eF_out, Error_PWR_Mon, Error_I2C_GPIO_Ext1, Error_I2C_GPIO_Ext2, Error_I2C_MUX,Error_temp_sensor_1

    int8_t  CAN_TMIx_PDM_Temp_Sensor1;                              //+       |  const = 0x               |   PDM датчик 1
    int8_t  CAN_TMIx_PDM_Temp_Sensor2;                              //+       |  const = 0x               |   PDM датчик 2
    int8_t  CAN_TMIx_PDM_Temp_Sensor3;                              //+       |  const = 0x               |   PDM датчик 3
    int8_t  CAN_TMIx_PDM_Temp_Sensor4;                              //+       |  const = 0x               |   PDM датчик 4
    int8_t  CAN_TMIx_PDM_Median_Temp;                               //+       |  const = 0x                 |   Медианная температура PDM

    uint16_t CAN_TMIx_PDM_PWR_Ch1_Voltage;                          //+     |  const = 0x               |  PDM Напряжение канала 1
    uint16_t CAN_TMIx_PDM_PWR_Ch2_Voltage;                          //+     |  const = 0x               |  PDM Напряжение канала 2
    uint16_t CAN_TMIx_PDM_PWR_Ch3_Voltage;                          //+     |  const = 0x               |  PDM Напряжение канала 3
    uint16_t CAN_TMIx_PDM_PWR_Ch4_Voltage;                          //+     |  const = 0x               |  PDM Напряжение канала 4
    uint16_t CAN_TMIx_PDM_PWR_Ch5_Voltage;                          //+     |  const = 0x               |  PDM Напряжение канала 5
    uint16_t CAN_TMIx_PDM_PWR_Ch6_Voltage;                          //+     |  const = 0x               |  PDM Напряжение канала 6
    uint16_t CAN_TMIx_PDM_PWR_Ch1_Voltage_Aver_10s;                 //+     |  const = 0x               |  Напряжение канала 1 СОП1 усреднение 10s
    uint16_t CAN_TMIx_PDM_PWR_Ch2_Voltage_Aver_10s;                 //+     |  const = 0x               |  Напряжение канала 2 СОП2 усреднение 10s
    uint16_t CAN_TMIx_PDM_PWR_Ch3_Voltage_Aver_10s;                 //+     |  const = 0x               |  Напряжение канала 3 БРК1 усреднение 10s
    uint16_t CAN_TMIx_PDM_PWR_Ch4_Voltage_Aver_10s;                 //+     |  const = 0x               |  Напряжение канала 4 БРК2 усреднение 10s
    uint16_t CAN_TMIx_PDM_PWR_Ch5_Voltage_Aver_10s;                 //+     |  const = 0x               |  Напряжение канала 4 БРК2 усреднение 10s
    uint16_t CAN_TMIx_PDM_PWR_Ch6_Voltage_Aver_10s;                 //+     |  const = 0x               |  Напряжение канала 4 БРК2 усреднение 10s

    int16_t CAN_TMIx_PDM_PWR_Ch1_Current;                           //+    |  const = 0x               |   Ток канал 1
    int16_t CAN_TMIx_PDM_PWR_Ch2_Current;                           //+    |  const = 0x               |   Ток канал 2
    int16_t CAN_TMIx_PDM_PWR_Ch3_Current;                           //+    |  const = 0x               |   Ток канал 3
    int16_t CAN_TMIx_PDM_PWR_Ch4_Current;                           //+    |  const = 0x               |   Ток канал 4
    int16_t CAN_TMIx_PDM_PWR_Ch5_Current;                           //+    |  const = 0x               |   Ток канал 5
    int16_t CAN_TMIx_PDM_PWR_Ch6_Current;                           //+    |  const = 0x               |   Ток канал 6

    int16_t CAN_TMIx_PDM_PWR_Ch1_Current_Aver_10s;                  //+    |  const = 0x               |   Ток канал 1 усреднение 10s
    int16_t CAN_TMIx_PDM_PWR_Ch2_Current_Aver_10s;                  //+    |  const = 0x               |   Ток канал 2 усреднение 10s
    int16_t CAN_TMIx_PDM_PWR_Ch3_Current_Aver_10s;                  //+    |  const = 0x               |   Ток канал 3 усреднение 10s
    int16_t CAN_TMIx_PDM_PWR_Ch4_Current_Aver_10s;                  //+    |  const = 0x               |   Ток канал 4 усреднение 10s
    int16_t CAN_TMIx_PDM_PWR_Ch5_Current_Aver_10s;                  //+    |  const = 0x               |   Ток канал 4 усреднение 10s
    int16_t CAN_TMIx_PDM_PWR_Ch6_Current_Aver_10s;                  //+    |  const = 0x               |   Ток канал 4 усреднение 10s
    uint8_t CAN_TMI0_Reserved[1];                                   //+    |  const = 0x

    // -------------------  ТМИ 1  ------------------ //
    uint8_t Protaction_GAP_TMI1[64];
    uint16_t CAN_TMI1_version;                                  	//+    |  const = 0x                   |   Версия ТМИ
    // PAM //
    uint16_t CAN_TMIx_PAM_PWR_Ch_State_BitMask;                     //+    |  const = 0x              | State_DC_DC State_LDO  PAM_SP_State_eF_out1...6
    uint16_t CAN_TMIx_EPS_Total_PAM_Generation_Power;               //+  |  const = 0x                   |   Полная вырабатываемая мощность PAM
    uint16_t CAN_TMIx_PAM_PWR_PG_BitMask;                           //+    |  const = 0x
    uint32_t CAN_TMIx_PAM_PWR_Error_BitMask;                        //+    |  const = 0x              | Статус ошибок модуля PAM, битовая маска: State_DC_DC State_LDO  PAM_SP_State_eF_out1...6
    int8_t  CAN_TMIx_PAM_Temp_Sensor1;                              //+    |  const = 0x               |   PAM датчик 1
    int8_t  CAN_TMIx_PAM_Temp_Sensor2;                              //+    |  const = 0x               |   PAM датчик 2
    int8_t  CAN_TMIx_PAM_Temp_Sensor3;                              //+    |  const = 0x               |   PAM датчик 3
    int8_t  CAN_TMIx_PAM_Temp_Sensor4;                              //+    |  const = 0x               |   PAM датчик 4
    int8_t  CAN_TMIx_PAM_Median_Temp;                               //+    |  const = 0x                 |   Медианная температура PAM
    uint8_t  CAN_TMIx_PAM_In_PwrCh_ID_State_BitsMask;               //+    |  const = 0x       |   Ideal Diode
    uint8_t  CAN_TMIx_PAM_In_PwrCh_Error_BitsMask;                  //+    |  const = 0x       |
    uint16_t CAN_TMIx_PAM_In_PwrCh1_Voltage;                        //+    |  const = 0x               |   Напряжение панели
    uint16_t CAN_TMIx_PAM_In_PwrCh2_Voltage;                        //+    |  const = 0x               |   Напряжение панели
    uint16_t CAN_TMIx_PAM_In_PwrCh3_Voltage;                        //+    |  const = 0x               |   Напряжение панели
    uint16_t CAN_TMIx_PAM_In_PwrCh4_Voltage;                        //+    |  const = 0x               |   Напряжение панели
    uint16_t CAN_TMIx_PAM_In_PwrLT1_Voltage;           			    //+    |  const = 0x               |   Напряжение откидных панелей
    uint16_t CAN_TMIx_PAM_In_PwrLT2_Voltage;                		//+    |  const = 0x               |   Напряжение откидных панелей
    int16_t CAN_TMIx_PAM_In_PwrCh1_Current;                         //+    |  const = 0x               |   Ток панели
    int16_t CAN_TMIx_PAM_In_PwrCh2_Current;                         //+    |  const = 0x               |   Ток панели
    int16_t CAN_TMIx_PAM_In_PwrCh3_Current;                         //+    |  const = 0x               |   Ток панели
    int16_t CAN_TMIx_PAM_In_PwrCh4_Current;                         //+    |  const = 0x               |   Ток панели
    int16_t CAN_TMIx_PAM_In_PwrLT1_Current;            				//+    |  const = 0x               |   Ток откидных панелей
    int16_t CAN_TMIx_PAM_In_PwrLT2_Current;           				//+    |  const = 0x               |   Ток откидных панелей

    uint16_t CAN_TMIx_SP_TM_Ch1_Status_BitMask;                     //+    |  const = 0x               |   Y+ Folding
    uint16_t CAN_TMIx_SP_TM_Ch2_Status_BitMask;                     //+    |  const = 0x               |   X-
    uint16_t CAN_TMIx_SP_TM_Ch3_Status_BitMask;                     //+    |  const = 0x               |    Y- Folding
    uint16_t CAN_TMIx_SP_TM_Ch4_Status_BitMask;                     //+    |  const = 0x               |    X+
    uint16_t CAN_TMIx_SP_TM_Ch5_Status_BitMask;                     //+    |  const = 0x               |    Y+
    uint16_t CAN_TMIx_SP_TM_Ch6_Status_BitMask;                     //+    |  const = 0x               |    Y-

    int8_t  CAN_TMIx_SP_TM_Ch1_Temp_Sensor1;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch1_Temp_Sensor2;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch1_Temp_Sensor3;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch1_Temp_Sensor4;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch2_Temp_Sensor1;                        //+     |  const = 0x              |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch2_Temp_Sensor2;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch2_Temp_Sensor3;                        //     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch2_Temp_Sensor4;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch3_Temp_Sensor1;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch3_Temp_Sensor2;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch3_Temp_Sensor3;                        //     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch3_Temp_Sensor4;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch4_Temp_Sensor1;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch4_Temp_Sensor2;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch4_Temp_Sensor3;                        //     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch4_Temp_Sensor4;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch5_Temp_Sensor1;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch5_Temp_Sensor2;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch5_Temp_Sensor3;                        //     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch5_Temp_Sensor4;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch6_Temp_Sensor1;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch6_Temp_Sensor2;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch6_Temp_Sensor3;                        //     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch6_Temp_Sensor4;                        //+     |  const = 0x               |   Температура панели
    int8_t  CAN_TMIx_SP_TM_Ch1_Median_Temp;                         //+    |  const = 0x                 |   Медианная температура панели  Folding Y+ - Norbi,    is Folding X- - Norby2
    int8_t  CAN_TMIx_SP_TM_Ch2_Median_Temp;                         //+    |  const = 0x                 |   Медианная температура панели  X- for Norbi,          is X - Norby2
    int8_t  CAN_TMIx_SP_TM_Ch3_Median_Temp;                         //+    |  const = 0x                 |   Медианная температура панели  Folding Y- for Norbi,  is Folding X+ - Norby2
    int8_t  CAN_TMIx_SP_TM_Ch4_Median_Temp;                         //+    |  const = 0x                 |   Медианная температура панели  X+ for Norbi,          is X+ - Norby2
    int8_t  CAN_TMIx_SP_TM_Ch5_Median_Temp;                         //+    |  const = 0x                 |   Медианная температура панели  Y+ (SRD) for Norbi,    is Y+ - Norby2
    int8_t  CAN_TMIx_SP_TM_Ch6_Median_Temp;                         //+    |  const = 0x                 |   Медианная температура панели  Y- for Norbi,          is Y- - Norby2

    uint8_t  CAN_TMI1_Reserved[31];                                 //+  |  const = 0x


    // -------------------  ТМИ 2  ------------------ //
    uint8_t Protaction_GAP_TMI2[64];
    uint16_t CAN_TMI2_version;                                  	//+ |  const = 0x                   |   Версия ТМИ

    // PBM part1 //
    uint16_t CAN_TMIx_PBM_Chrg_Dichrg_Key_ComReg_BitMask;           //+ |  const = 0x                   |
    uint16_t CAN_TMIx_PBM_Chrg_Dichrg_Key_Status_BitMask;           //+ |  const = 0x                   |
    uint8_t CAN_TMIx_PBM_Thermostat_ComReg_BitMask;                 //+ |  const = 0x                   | Команда на включение Термостат
    uint8_t CAN_TMIx_PBM_Heating_Element_State_BitMask;             //+ |  const = 0x                       | Текущее состояние Термостата
    uint8_t CAN_TMIx_PBM_Wreck_Charge_Key_State_BitMask;            //+ |  const = 0x                   | Ключи аварийной зарядки
    int16_t CAN_TMIx_PBM_Total_PBM_Char_Dischar_Power;              //+  |  const = 0x                   |   Полная мощность зарядки/разрядки PBM
    uint16_t CAN_TMIx_PBM_Total_Heating_Elements_Power;			    //+    |  const = 0xD               |   Полный уровень заряда АБ в mAh
    uint16_t CAN_TMIx_PBM_Total_Battery_Level_mAh;			        //+    |  const = 0xD               |   Полный уровень заряда АБ в mAh
    uint8_t  CAN_TMIx_PBM_Total_Battery_Level_percent;              //+   |  const = 0x7F                 |   Полный уровень заряда АБ в %
    uint16_t CAN_TMIx_PBM1_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM2_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM3_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM4_Error_BitMask;                           //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM1_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM1_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM2_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM2_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM3_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM3_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM4_Controller1_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска
    uint16_t CAN_TMIx_PBM4_Controller2_Error_BitMask;               //+ |  const = 0x                   |   Статус ошибок модуля PBM1, битовая маска

    uint8_t  CAN_TMIx_PBM1_Branch1_Charge_Level_Percent;            //+     |  const = 0x                |  Уровень заряда АБ1 ветвь 1
    uint8_t  CAN_TMIx_PBM1_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ1 ветвь 2
    uint8_t  CAN_TMIx_PBM2_Branch1_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ2 ветвь 1
    uint8_t  CAN_TMIx_PBM2_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ2 ветвь 2
    uint8_t  CAN_TMIx_PBM3_Branch1_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ3 ветвь 1
    uint8_t  CAN_TMIx_PBM3_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ3 ветвь 2
    uint8_t  CAN_TMIx_PBM4_Branch1_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ4 ветвь 1
    uint8_t  CAN_TMIx_PBM4_Branch2_Charge_Level_Percent;            //+     |  const = 0x                 |  Уровень заряда АБ4 ветвь 2

    uint16_t CAN_TMIx_PBM1_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ1 ветвь 1
    uint16_t CAN_TMIx_PBM1_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ1 ветвь 1
    uint16_t CAN_TMIx_PBM2_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ2 ветвь 2
    uint16_t CAN_TMIx_PBM2_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ2 ветвь 2
    uint16_t CAN_TMIx_PBM3_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ3 ветвь 2
    uint16_t CAN_TMIx_PBM3_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ3 ветвь 2
    uint16_t CAN_TMIx_PBM4_Branch1_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ4 ветвь 2
    uint16_t CAN_TMIx_PBM4_Branch2_Charge_Level_mAh;                //+    |  const = 0x               |   Уровень заряда АБ4 ветвь 2

    int16_t CAN_TMIx_PBM1_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ1 ветвь 1
    int16_t CAN_TMIx_PBM1_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ1 ветвь 2
    int16_t CAN_TMIx_PBM2_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ2 ветвь 1
    int16_t CAN_TMIx_PBM2_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ2 ветвь 2
    int16_t CAN_TMIx_PBM3_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ3 ветвь 1
    int16_t CAN_TMIx_PBM3_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ3 ветвь 2
    int16_t CAN_TMIx_PBM4_Branch1_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ4 ветвь 1
    int16_t CAN_TMIx_PBM4_Branch2_Char_Discha_Current;              //+     |  const = 0x               |  Ток заряда/разряда АБ4 ветвь 2

    int8_t  CAN_PBM1_Temp_Controller1;                              //+     |  const = 0x               |   АБ1 датчик контроллера 1
    int8_t  CAN_PBM1_Temp_Controller2;                              //+     |  const = 0x               |   АБ1 датчик контроллера 2
    int8_t  CAN_PBM1_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ1 датчик на плате 1
    int8_t  CAN_PBM1_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ1 датчик на плате 2
    int8_t  CAN_PBM1_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ1 датчик на плате 3
    int8_t  CAN_PBM1_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ1 датчик на плате 4
    int8_t  CAN_PBM2_Temp_Controller1;                              //+     |  const = 0x               |   АБ2 датчик контроллера 1
    int8_t  CAN_PBM2_Temp_Controller2;                              //+     |  const = 0x               |   АБ2 датчик контроллера 2
    int8_t  CAN_PBM2_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ2 датчик на плате 1
    int8_t  CAN_PBM2_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ2 датчик на плате 2
    int8_t  CAN_PBM2_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ2 датчик на плате 3
    int8_t  CAN_PBM2_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ2 датчик на плате 4
    int8_t  CAN_PBM3_Temp_Controller1;                              //+     |  const = 0x               |   АБ3 датчик контроллера 1
    int8_t  CAN_PBM3_Temp_Controller2;                              //+     |  const = 0x               |   АБ3 датчик контроллера 2
    int8_t  CAN_PBM3_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ3 датчик на плате 1
    int8_t  CAN_PBM3_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ3 датчик на плате 2
    int8_t  CAN_PBM3_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ3 датчик на плате 3
    int8_t  CAN_PBM3_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ3 датчик на плате 4
    int8_t  CAN_PBM4_Temp_Controller1;                              //+     |  const = 0x               |   АБ4 датчик контроллера 1
    int8_t  CAN_PBM4_Temp_Controller2;                              //+     |  const = 0x               |   АБ4 датчик контроллера 2
    int8_t  CAN_PBM4_Temp_Sensor1;                                  //+     |  const = 0x               |   АБ4 датчик на плате 1
    int8_t  CAN_PBM4_Temp_Sensor2;                                  //+     |  const = 0x               |   АБ4 датчик на плате 2
    int8_t  CAN_PBM4_Temp_Sensor3;                                  //+     |  const = 0x               |   АБ4 датчик на плате 3
    int8_t  CAN_PBM4_Temp_Sensor4;                                  //+     |  const = 0x               |   АБ4 датчик на плате 4

    uint8_t  CAN_TMI2_Reserved[3];                                  //+  |  const = 0x

    // -------------------  ТМИ 3  ------------------ //
    uint8_t Protaction_GAP_TMI3[64];
    uint16_t CAN_TMI3_version;                                  	//+ |  const = 0x                   |   Версия ТМИ

    // PBM part2 //
    uint16_t CAN_TMIx_PBM1_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM1_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM1_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM1_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ1 ветвь 2 аккумулятор 2
    uint16_t CAN_TMIx_PBM2_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM2_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM2_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM2_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ2 ветвь 2 аккумулятор 2
    uint16_t CAN_TMIx_PBM3_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM3_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM3_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM3_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 2
    uint16_t CAN_TMIx_PBM4_Branch1_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 1
    uint16_t CAN_TMIx_PBM4_Branch1_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 1 аккумулятор 2
    uint16_t CAN_TMIx_PBM4_Branch2_Battery1_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 1
    uint16_t CAN_TMIx_PBM4_Branch2_Battery2_Voltage;                //+     |  const = 0x               |  Напряжение АБ3 ветвь 2 аккумулятор 2

    int16_t CAN_TMIx_PBM1_Branch1_Max_Current;                      //+     |  const = 0x               |  Максимальный ток АБ1 ветвь 1


    uint8_t  CAN_TMI3_Reserved[2];                                  //+  |  const = 0x
};

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


