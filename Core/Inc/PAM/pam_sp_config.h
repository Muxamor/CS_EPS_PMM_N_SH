
#ifndef CS_EPS_PMM_N_SH_V_PAM_SP_CONFIG_H
#define CS_EPS_PMM_N_SH_V_PAM_SP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define PAM_SP_quantity  			    0x06 // Quantity solar panel on the satellite
#define PAM_SP_temp_sens_max_quantity   0x04 // Max quantity Temp. sensor per solar panel.
#define PAM_SP_ID_max_quantity	        0x02 // Max quantity Ideal Diode per solar panel.

#define PAM_SP1           			    0x00 //Solar panel namber 1 is Folding Y+ - Norbi1, is Folding X- - Norby2
#define PAM_SP2          			    0x01 //Solar panel namber 2 is X- - Norbi1,          is Folding X- - Norby2
#define PAM_SP3         			    0x02 //Solar panel namber 3 is Folding Y- - Norbi1,  is Folding X+ - Norby2
#define PAM_SP4         			    0x03 //Solar panel namber 4 is X+ - Norbi1,          is X+ - Norby2
#define PAM_SP5          			    0x04 //Solar panel namber 5 is Y+ (SRD) - Norbi1,    is Y+ - Norby2
#define PAM_SP6         			    0x05 //Solar panel namber 6 is Y- - Norbi1,          is Y- - Norby2

#define PAM_SP1_temp_sens_quantity      0x04  // Quantity temp senos on the solar panel namber 1 is Folding Y+ - Norbi1,    is Folding X- - Norby2
#define PAM_SP2_temp_sens_quantity      0x04  // Quantity temp senos on the solar panel namber 2 is X- - Norbi1,            is X - Norby2
#define PAM_SP3_temp_sens_quantity      0x04  // Quantity temp senos on the solar panel namber 3 is Folding Y- - Norbi1,    is Folding X+ - Norby2
#define PAM_SP4_temp_sens_quantity      0x04  // Quantity temp senos on the solar panel namber 4 is X+ - Norbi1,            is X+ - Norby2
#define PAM_SP5_temp_sens_quantity      0x02  // Quantity temp senos on the solar panel namber 5 is Y+ (SRD) - Norbi1,      is Y+ - Norby2
#define PAM_SP6_temp_sens_quantity      0x02  // Quantity temp senos on the solar panel namber 6 is Y- - Norbi1,            is Y- - Norby2

#define PAM_SP1_ID_quantity             0x00  // Quantity Ideal Diode on the solar panel namber 1 is 0x00 Folding Y+ - Norbi,    is Folding X- - Norby2
#define PAM_SP2_ID_quantity             0x00  // Quantity Ideal Diode on the solar panel namber 2 is 0x06 X- for Norbi,          is X - Norby2
#define PAM_SP3_ID_quantity             0x00  // Quantity Ideal Diode on the solar panel namber 3 is 0x00 Folding Y- for Norbi,  is Folding X+ - Norby2
#define PAM_SP4_ID_quantity             0x00  // Quantity Ideal Diode on the solar panel namber 4 is 0x06 X+ for Norbi,          is X+ - Norby2
#define PAM_SP5_ID_quantity             0x02  // Quantity Ideal Diode on the solar panel namber 5 is 0x02 Y+ (SRD) for Norbi,    is Y+ - Norby2
#define PAM_SP6_ID_quantity             0x02  // Quantity Ideal Diode on the solar panel namber 6 is 0x03 Y- for Norbi,          is Y- - Norby2


//I2C address temp senos on the solar panel namber 1 is Folding Y+ - Norbi,     is Folding X- - Norby2
#define PAM_SP1_I2CADDR_TEMP_SENS_1     0x48
#define PAM_SP1_I2CADDR_TEMP_SENS_2 	0x49
#define PAM_SP1_I2CADDR_TEMP_SENS_3 	0x4A
#define PAM_SP1_I2CADDR_TEMP_SENS_4 	0x4B
//I2C address GPIO I2C extander on the solar panel namber 1 is Folding Y+ for Norbi
//#define PAM_SP1_I2CADDR_GPIO_EXT_1     0x

//I2C address temp senos on the solar panel namber 2 is X- - Norbi, is X - Norby2
#define PAM_SP2_I2CADDR_TEMP_SENS_1     0x48
#define PAM_SP2_I2CADDR_TEMP_SENS_2 	0x49
#define PAM_SP2_I2CADDR_TEMP_SENS_3 	0x4A
#define PAM_SP2_I2CADDR_TEMP_SENS_4 	0x4B
//I2C address GPIO I2C extander on the solar panel namber 2 is X- - Norbi, is X - Norby2
//#define PAM_SP2_I2CADDR_GPIO_Ext_1      0x38

//I2C address temp senos on the solar panel namber 3 is Folding Y- - Norbi, is Folding X+ - Norby2
#define PAM_SP3_I2CADDR_TEMP_SENS_1     0x48
#define PAM_SP3_I2CADDR_TEMP_SENS_2 	0x49
#define PAM_SP3_I2CADDR_TEMP_SENS_3 	0x4A
#define PAM_SP3_I2CADDR_TEMP_SENS_4 	0x4B
//I2C address GPIO I2C extander on the solar panel namber 3 is Folding Y- - Norbi, is Folding X+ - Norby2
//#define PAM_SP3_I2CADDR_GPIO_EXT_1     0x

//I2C address temp senos on the solar panel namber 4 is X+ - Norbi, is X+ - Norby2
#define PAM_SP4_I2CADDR_TEMP_SENS_1     0x48
#define PAM_SP4_I2CADDR_TEMP_SENS_2 	0x49
#define PAM_SP4_I2CADDR_TEMP_SENS_3 	0x4A
#define PAM_SP4_I2CADDR_TEMP_SENS_4 	0x4B
//I2C address GPIO I2C extander on the solar panel namber 4 is X+ - Norbi, is X+ - Norby2
//#define PAM_SP4_I2CADDR_GPIO_Ext_1      0x

//I2C address temp senos on the solar panel namber 5 is Y+ (SRD) - Norbi, is Y+ - Norby2
#define PAM_SP5_I2CADDR_TEMP_SENS_1     0x48
#define PAM_SP5_I2CADDR_TEMP_SENS_2 	0x4B
//I2C address GPIO I2C extander on the solar panel namber 5 is Y+ (SRD) - Norbi, is Y+ - Norby2
#define PAM_SP5_I2CADDR_GPIO_Ext_1      0x38

//I2C address temp senos on the solar panel namber 5 is Y- for Norbi, is Y- - Norby2
#define PAM_SP6_I2CADDR_TEMP_SENS_1     0x48
#define PAM_SP6_I2CADDR_TEMP_SENS_2 	0x4B
//I2C address temp senos on the solar panel namber 5 is Y- for Norbi, is Y- - Norby2
#define PAM_SP6_I2CADDR_GPIO_Ext_1      0x38

typedef struct{

    I2C_TypeDef *PAM_I2Cx_PORT;
    uint8_t PAM_I2C_addr_I2C_MUX;
    uint8_t PAM_I2C_MUX_Ch;

    uint8_t SP_I2C_addr_temp_senor[PAM_SP_temp_sens_max_quantity];
    uint8_t SP_I2C_addr_GPIO_Ext;

    uint8_t Num_pin_GPIO_Ext_ID[PAM_SP_ID_max_quantity];

    uint8_t temp_sensor_quantity;
    uint8_t ID_quantity;
    uint8_t PWR_number_ch_TM_SP;

}_PAM_SP_table;

_PAM_SP_table PAM_SP_Table( uint8_t number_SP);

#ifdef __cplusplus
}
#endif

#endif
