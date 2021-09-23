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
#define CAN_PWR_VBAT1_offset		        24
#define CAN_PWR_VBAT2_offset		        25
#define CAN_PMM_PWR_PBM_Logic_offset        26

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

#define CAN_AB1_Heat_Branch1_offset         350
#define CAN_AB1_Heat_Branch2_offset         351
#define CAN_AB2_Heat_Branch1_offset         352
#define CAN_AB2_Heat_Branch2_offset         353
#define CAN_AB3_Heat_Branch1_offset         354
#define CAN_AB3_Heat_Branch2_offset         355
#define CAN_AB4_Heat_Branch1_offset         356
#define CAN_AB4_Heat_Branch2_offset         357



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
    uint8_t CAN_PMM_PWR_VBAT1;              //+24           Командный регистр линии VBAT1 (МС)
    uint8_t CAN_PMM_PWR_VBAT2;              //+25           Командный регистр линии VBAT2 (МС)
    uint8_t CAN_PMM_PWR_PBM_Logic;          //+26           Вкл/Выкл питания логики PBM
    uint8_t CAN_Comd_Reserved_27;           //+27           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_28;           //+28           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_29;           //+29           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_30;           //+30           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_31;           //+31           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_32;           //+32           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_33;           //+33           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_34;           //+34           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_35;           //+35           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_36;           //+36           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_37;           //+37           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_38;           //+38           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_39;           //+39           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_40;           //+40           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_41;           //+41           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_42;           //+42           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_43;           //+43           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_44;           //+44           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_45;           //+45           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_46;           //+46           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_47;           //+47           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_48;           //+48           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_49;           //+49           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_50;           //+50           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_51;           //+51           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_52;           //+52           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_53;           //+53           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_54;           //+54           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_55;           //+55           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_56;           //+56           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_57;           //+57           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_58;           //+58           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_59;           //+59           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_60;           //+60           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_61;           //+61           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_62;           //+62           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_63;           //+63           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_64;           //+64           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_65;           //+65           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_66;           //+66           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_67;           //+67           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_68;           //+68           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_69;           //+69           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_70;           //+70           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_71;           //+71           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_72;           //+72           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_73;           //+73           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_74;           //+74           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_75;           //+75           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_76;           //+76           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_77;           //+77           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_78;           //+78           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_79;           //+79           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_80;           //+80           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_81;           //+81           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_82;           //+82           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_83;           //+83           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_84;           //+84           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_85;           //+85           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_86;           //+86           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_87;           //+87           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_88;           //+88           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_89;           //+89           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_90;           //+90           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_91;           //+91           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_92;           //+92           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_93;           //+93           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_94;           //+94           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_95;           //+95           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_96;           //+96           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_97;           //+97           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_98;           //+98           Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_99;           //+99           Командный регистр зарезервирован


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
    uint8_t CAN_AB1_Charge_key_branch_1;  		//+300            Командный регистр ключа заряда ветви 1 АБ1
    uint8_t CAN_AB1_Discharge_key_branch_1;     //+301            Командный регистр ключа разряда ветви 1 АБ1
    uint8_t CAN_AB1_Charge_key_branch_2;  		//+302            Командный регистр ключа заряда ветви 2 АБ1
    uint8_t CAN_AB1_Discharge_key_branch_2;     //+303            Командный регистр ключа разряда ветви 2 АБ1
    uint8_t CAN_AB2_Charge_key_branch_1;   		//+304            Командный регистр ключа заряда ветви 1 АБ2
    uint8_t CAN_AB2_Discharge_key_branch_1;   	//+305            Командный регистр ключа разряда ветви 1 АБ2
    uint8_t CAN_AB2_Charge_key_branch_2;   		//+306            Командный регистр ключа заряда ветви 2 АБ2
    uint8_t CAN_AB2_Discharge_key_branch_2;   	//+307            Командный регистр ключа разряда ветви 2 АБ2
    uint8_t CAN_AB3_Charge_key_branch_1;   		//+308            Командный регистр ключа заряда ветви 1 АБ3
    uint8_t CAN_AB3_Discharge_key_branch_1;   	//+309            Командный регистр ключа разряда ветви 1 АБ3
    uint8_t CAN_AB3_Charge_key_branch_2;   		//+310            Командный регистр ключа заряда ветви 2 АБ3
    uint8_t CAN_AB3_Discharge_key_branch_2;   	//+311            Командный регистр ключа разряда ветви 2 АБ3
    uint8_t CAN_AB4_Charge_key_branch_1;   		//+312            Командный регистр ключа заряда ветви 1 АБ4
    uint8_t CAN_AB4_Discharge_key_branch_1;   	//+313            Командный регистр ключа разряда ветви 1 АБ4
    uint8_t CAN_AB4_Charge_key_branch_2;   		//+314            Командный регистр ключа заряда ветви 2 АБ4
    uint8_t CAN_AB4_Discharge_key_branch_2;   	//+315            Командный регистр ключа разряда ветви 2 АБ4
    uint8_t CAN_Comd_Reserved_316;          //+316          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_317;          //+317          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_318;          //+318          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_319;          //+319          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_320;          //+320          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_321;          //+321          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_322;          //+322          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_323;          //+323          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_324;          //+324          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_325;          //+325          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_326;          //+326          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_327;          //+327          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_328;          //+328          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_329;          //+329          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_330;          //+330          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_331;          //+331          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_332;          //+332          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_333;          //+333          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_334;          //+334          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_335;          //+335          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_336;          //+336          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_337;          //+337          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_338;          //+338          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_339;          //+339          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_340;          //+340          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_341;          //+341          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_342;          //+342          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_343;          //+343          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_344;          //+344          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_345;          //+345          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_346;          //+346          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_347;          //+347          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_348;          //+348          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_349;          //+349          Командный регистр зарезервирован

    uint8_t CAN_AB1_Heat_Branch1;           //+350         Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 1
    uint8_t CAN_AB1_Heat_Branch2;           //+351          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 1
    uint8_t CAN_AB2_Heat_Branch1;           //+352          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 2
    uint8_t CAN_AB2_Heat_Branch2;           //+353          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 2
    uint8_t CAN_AB3_Heat_Branch1;           //+354          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 3
    uint8_t CAN_AB3_Heat_Branch2;           //+355          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 3
    uint8_t CAN_AB4_Heat_Branch1;           //+356          Вкл/Выкл автонагрев(термостат) ветви 1 батарейного модуля 4
    uint8_t CAN_AB4_Heat_Branch2;           //+357          Вкл/Выкл автонагрев(термостат) ветви 2 батарейного модуля 4
    uint8_t CAN_Comd_Reserved_358;          //+358          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_359;          //+369          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_360;          //+360          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_361;          //+361          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_362;          //+362          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_363;          //+363          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_364;          //+364          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_365;          //+365          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_366;          //+366          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_367;          //+367          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_368;          //+368          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_369;          //+369          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_370;          //+370          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_371;          //+371          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_372;          //+372          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_373;          //+373          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_374;          //+374          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_375;          //+375          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_376;          //+376          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_377;          //+377          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_378;          //+378          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_379;          //+379          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_380;          //+380          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_381;          //+381          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_382;          //+382          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_383;          //+383          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_384;          //+384          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_385;          //+385          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_386;          //+386          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_387;          //+387          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_388;          //+388          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_389;          //+389          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_390;          //+390          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_391;          //+391          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_392;          //+392          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_393;          //+393          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_394;          //+394          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_395;          //+395          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_396;          //+396          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_397;          //+397          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_398;          //+398          Командный регистр зарезервирован
    uint8_t CAN_Comd_Reserved_399;          //+399          Командный регистр зарезервирован
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
    uint16_t CAN_Beacon_Total_IN_Power_SP;                      	//+17     |  const = 0xB7B8               |   Полная вырабатываемая мощность СБ
    uint16_t CAN_Beacon_spacecraft_total_power;                     //+19     |  const = 0xB9BA               |   Полная мощность потребления КА
    uint8_t  CAN_Beacon_median_PMM_temp;							//+21     |  const = 0xBB                 |   Медианная температура PMM (один датчик)
    uint8_t  CAN_Beacon_median_PAM_temp;							//+22     |  const = 0xBC                 |   Медианная температура PAM
    uint8_t  CAN_Beacon_median_PDM_temp;							//+23     |  const = 0xBD                 |   Медианная температура PDM
    uint8_t  CAN_Beacon_SES_module_system_elements_status[5];		//+24     |  const = 0xBEBFC0C1C2         |   Статус элементов системы модуля СЭС, битовая маска

    // -------------------  ТМИ 4  ------------------ //

    //Массив CAN_токов солнечных панелей                                     |                               |
    uint16_t CAN_SP_current_pX;                                  	//+29    |  const = 0x5C5D               |   Ток панели +X
    uint16_t CAN_SP_current_nX;                                  	//+31    |  const = 0x5E5F               |   Ток панели -X
    uint16_t CAN_SP_current_pY;                                 	//+33    |  const = 0x6061               |   Ток панели +Y
    uint16_t CAN_SP_current_nY;                                  	//+35    |  const = 0x6263               |   Ток панели -Y
    uint16_t CAN_SPF_current_1;           							//+37    |  const = 0x6465               |   Ток откидных панелей плоскость спутника 1  (LT8490)
    uint16_t CAN_SPF_current_2;           							//+39    |  const = 0x6667               |   Ток откидных панелей плоскость спутника 2  (LT8490)

    //Массив напряжений солнечных панелей                                |                               |
    uint16_t CAN_SP_voltage_pX;                                  	//+41    |  const = 0x6869               |   Напряжение панели +X
    uint16_t CAN_SP_voltage_nX;                                  	//+43    |  const = 0x6A6B               |   Напряжение панели -X
    uint16_t CAN_SP_voltage_pY;                                  	//+45    |  const = 0x6C6D               |   Напряжение панели +Y
    uint16_t CAN_SP_voltage_nY;                                  	//+47    |  const = 0x6E6F               |   Напряжение панели -Y
    uint16_t CAN_SPF_voltage_1;           							//+49    |  const = 0x7071               |   Напряжение откидных панелей плоскость спутника 1
    uint16_t CAN_SPF_voltage_2;           							//+51    |  const = 0x7273               |   Напряжение откидных панелей плосуость спутника 2

    //Массив медианных температур солнечных панелей                      |                               |
    uint8_t  CAN_Panel_median_temperature_pX;                       //+53    |  const = 0x74                 |   Медианная температура панели +X
    uint8_t  CAN_Panel_median_temperature_nX;                       //+54    |  const = 0x75                 |   Медианная температура панели -X
    uint8_t  CAN_Panel_median_temperature_pY;                       //+55    |  const = 0x76                 |   Медианная температура панели +Y
    uint8_t  CAN_Panel_median_temperature_nY;                       //+56    |  const = 0x77                 |   Медианная температура панели -Y
    uint8_t  CAN_Hinged_panel_median_temperature_pY;                //+57    |  const = 0x78                 |   Медианная температура откидной панели +Y
    uint8_t  CAN_Hinged_panel_median_temperature_nY;                //+58    |  const = 0x79                 |   Медианная температура откидной панели -Y
    uint8_t  CAN_Solar_panel_status[5];                             //+59    |  const = 0x7A7B7C7D7E         |   Статус солнечных панелей, битовая маска (в т.ч. ФЭП)

    //Массив уровня заряда АБ в %                                        |                               |
    uint8_t  CAN_Full_ABs_Energy_level_percent;              			//+64    |  const = 0x7F                 |   Полный уровень заряда АБ в %

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
    uint16_t CAN_Total_Generate_Power_SP;                                 //+120    |  const = 0xB7B8               |   Полная вырабатываемая мощность СБ
    uint16_t CAN_Spacecraft_total_power;                            //+122    |  const = 0xB9BA               |   Полная мощность потребления КА

    //Массив медианных температур модуля СЭС                             |                               |
    uint8_t  CAN_Median_PMM_temp;                                   //+124   |  const = 0xBB                 |   Медианная температура PMM (один датчик)
    uint8_t  CAN_Median_PAM_temp;                                   //+125   |  const = 0xBC                 |   Медианная температура PAM
    uint8_t  CAN_Median_PDM_temp;                                   //+126   |  const = 0xBD                 |   Медианная температура PDM
    uint8_t  CAN_SES_module_system_elements_status[5];              //+127   |  const = 0xBEBFC0C1C2         |   Статус элементов системы модуля СЭС, битовая маска
    uint32_t CAN_Number_of_restarts_of_the_SES_module;              //+132   |  const = 0xC3C4C5C6           |   Количество перезапусков основного модуля СЭС
    uint32_t CAN_Number_of_restarts_of_the_reserve_SES_module;      //+136   |  const = 0xC7C8C9CA           |   Количество перезапусков резервного модуля СЭС
    uint8_t  CAN_State_PWR_CANmain;                                 //+140   |  const = 0xCB                 |   Состояние ключа питаня CAN main
    uint8_t  CAN_State_PWR_CANbackup;                               //+141   |  const = 0xCC                 |   Состояние ключа питаня CAN backup
    uint8_t  CAN_OFF_PWR_Passive_CPU;                               //+142   |  const = 0xCD                 |   Упарвление питанием пассивного CPU
    uint8_t  CAN_EPS_Const_Mode;                                    //+142   |  const = 0xCE                 |   Режим констант
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
    uint8_t  CAN_SES_module_data_array2[60];                        //+201     |  const = 0xF0F1F2F3F4F5F6F7F8 |  Массив 2 данных о работе СЭС
                                                                    //                      F9FAFBFCFDFEFF00   |
                                                                    //                      0102030405060708   |
                                                                    //                      090A0B0C0D0E0F10   |
                                                                    //                      1112131415161718   |
                                                                    //                      191A1B1C1D1E1F20   |
                                                                    //                      2122232425262728   |
                                                                    //                      292A2B             |


    // ----------------------- ТМИ 8 -------------------        --------  	   |
    //Массив температур солнечных панелей                                      |
    uint8_t  CAN_SPanel_temp_pX_sensor_1;                            //+261     |  const = 0x2C               |   Температура панели +X датчик 1
    uint8_t  CAN_SPanel_temp_pX_sensor_2;                            //+262     |  const = 0x2D               |   Температура панели +X датчик 2
    uint8_t  CAN_SPanel_temp_pX_sensor_3;                            //+263     |  const = 0x2E               |   Температура панели +X датчик 3
    uint8_t  CAN_SPanel_temp_pX_sensor_4;                            //+264     |  const = 0x2F               |   Температура панели +X датчик 4
    uint8_t  CAN_SPanel_temp_nX_sensor_1;                            //+265     |  const = 0x30               |   Температура панели -X датчик 1
    uint8_t  CAN_SPanel_temp_nX_sensor_2;                            //+266     |  const = 0x31               |   Температура панели -X датчик 2
    uint8_t  CAN_SPanel_temp_nX_sensor_3;                            //+267     |  const = 0x32               |   Температура панели -X датчик 3
    uint8_t  CAN_SPanel_temp_nX_sensor_4;                            //+268     |  const = 0x33               |   Температура панели -X датчик 4
    uint8_t  CAN_SPanel_temp_pY_sensor_1;                            //+269     |  const = 0x34               |   Температура панели +Y датчик 1
    uint8_t  CAN_SPanel_temp_pY_sensor_2;                            //+270     |  const = 0x35               |   Температура панели +Y датчик 2
    uint8_t  CAN_SPanel_temp_nY_sensor_1;                            //+271     |  const = 0x36               |   Температура панели -Y датчик 1
    uint8_t  CAN_SPanel_temp_nY_sensor_2;                            //+272     |  const = 0x37               |   Температура панели -Y датчик 2
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
    uint8_t  CAN_data_array3[10];                                   //+330     |  const = 0x717273747576     |   Массив 3 данных о работе СЭС
                                                                               //           7778797A7
    uint8_t  CAN_PMM_PBM_Termostat_bit_mask;                        //+340     |  const = 0x7B                 |  Термостаты модулей PBMs битовая маска
    uint8_t  CAN_PMM_PWR_PBM_Logic;                                 //+341     |  const = 0x7C                 |   Питание логики PBMs
    uint8_t  CAN_PAM_PWR_DC_DC;                                     //+342     |  const = 0x7D                 |   Источник PAM DC-DC
    uint8_t  CAN_PAM_PWR_LDO;                                       //+343     |  const = 0x7E                 |   Источник PAM LDO
    uint8_t  CAN_PAM_SP_PWR_CH1;                                    //+344     |  const = 0x7F                 |   Питание телеметрии SP канал 1
    uint8_t  CAN_PAM_SP_PWR_CH2;                                    //+345     |  const = 0x80                 |   Питание телеметрии SP канал 2
    uint8_t  CAN_PAM_SP_PWR_CH3;                                    //+346     |  const = 0x81                 |   Питание телеметрии SP канал 3
    uint8_t  CAN_PAM_SP_PWR_CH4;                                    //+347     |  const = 0x82                 |   Питание телеметрии SP канал 4
    uint8_t  CAN_PAM_SP_PWR_CH5;                                    //+348     |  const = 0x83                 |   Питание телеметрии SP канал 5
    uint8_t  CAN_PAM_SP_PWR_CH6;                                    //+349     |  const = 0x84                 |   Питание телеметрии SP канал 6
    uint8_t  CAN_EPS__Mode;                                         //+350     |  const = 0x85                 |   Режим работы EPS
    uint16_t CAN_Version_FW;                                        //+351     |  const = 0x8687               |   Firmware version
    uint16_t CAN_Channel1_current_average_10s;                      //+353     |  const = 0x8889               |   Ток канал 1 усреднение 10s
    uint16_t CAN_Channel2_current_average_10s;                      //+355     |  const = 0x8A8B               |   Ток канал 2 усреднение 10s
    uint16_t CAN_Channel3_current_average_10s;                      //+357     |  const = 0x8C8D               |   Ток канал 3 усреднение 10s
    uint16_t CAN_Channel4_current_average_10s;                      //+359     |  const = 0x8E8F               |   Ток канал 4 усреднение 10s
    uint16_t CAN_VBAT1_current_average_10s;                         //+361     |  const = 0x9091               |   Ток VBAT1 усреднение 10s
    uint16_t CAN_VBAT2_current_average_10s;                         //+363     |  const = 0x9293               |   Ток VBAT2 усреднение 10s

    uint16_t CAN_Channel1_voltage_average_10s;                      //+365     |  const = 0x9495               |  Напряжение канала 1 СОП1 усреднение 10s
    uint16_t CAN_Channel2_voltage_average_10s;                      //+367     |  const = 0x9697               |  Напряжение канала 2 СОП2 усреднение 10s
    uint16_t CAN_Channel3_voltage_average_10s;                      //+369     |  const = 0x9899               |  Напряжение канала 3 БРК1 усреднение 10s
    uint16_t CAN_Channel4_voltage_average_10s;                      //+371     |  const = 0x9A9B               |  Напряжение канала 4 БРК2 усреднение 10s
    uint16_t CAN_VBAT1_voltage_average_10s;                         //+373     |  const = 0x9C9D               |   Напряжение VBAT1 усреднение 10s
    uint16_t CAN_VBAT2_voltage_average_10s;                         //+375     |  const = 0x9E9F               |   Напряжение VBAT2 усреднение 10s

    // size ТМИ0+ТМИ4+ТМИ7+ТМИ8 = 29 + 3 * 116 = 377

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


