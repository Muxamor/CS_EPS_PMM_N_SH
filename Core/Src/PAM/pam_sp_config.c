#include "stm32l4xx.h"
#include "PCA9534.h"
#include "TCA9548.h"
#include "PAM/pam_config.h"
#include "PAM/pam_sp_config.h"

_PAM_SP_table PAM_SP_Table( uint8_t number_SP){

    _PAM_SP_table pam_sp_table = { 0 };

    pam_sp_table.PAM_I2Cx_PORT = PAM_I2C_PORT;
    pam_sp_table.PAM_I2C_addr_I2C_MUX = PAM_I2CADDR_I2C_MUX_2;

    switch( number_SP ){

        case PAM_SP1:
            pam_sp_table.PAM_I2C_MUX_Ch = TCA9548_CH1;

            pam_sp_table.ID_quantity = PAM_SP1_ID_quantity;
            //Norbi 1
            //pam_sp_table.SP_I2C_addr_GPIO_Ext=  PAM_SP1_I2CADDR_GPIO_Ext_1; No for Norbi
            //pam_sp_table.Num_pin_GPIO_Ext_ID[] =  No for Norbi

            //Norby 2
            //pam_sp_table.Num_pin_GPIO_Ext_ID[] =  No for Norby2

            pam_sp_table.PWR_number_ch_TM_SP = PAM_PWR_TM_SP_Ch1;

            pam_sp_table.temp_sensor_quantity = PAM_SP1_temp_sens_quantity;
            pam_sp_table.SP_I2C_addr_temp_senor[0] = PAM_SP1_I2CADDR_TEMP_SENS_1;
            pam_sp_table.SP_I2C_addr_temp_senor[1] = PAM_SP1_I2CADDR_TEMP_SENS_2;
            pam_sp_table.SP_I2C_addr_temp_senor[2] = PAM_SP1_I2CADDR_TEMP_SENS_3;
            pam_sp_table.SP_I2C_addr_temp_senor[3] = PAM_SP1_I2CADDR_TEMP_SENS_4;
            break;

        case PAM_SP2:
            pam_sp_table.PAM_I2C_MUX_Ch = TCA9548_CH0;

            pam_sp_table.ID_quantity = PAM_SP2_ID_quantity;
            //Norbi 1
           /* pam_sp_table.SP_I2C_addr_GPIO_Ext = PAM_SP2_I2CADDR_GPIO_Ext_1;
            pam_sp_table.Num_pin_GPIO_Ext_ID[0] = PCA9534_IO_P07;
            pam_sp_table.Num_pin_GPIO_Ext_ID[1] = PCA9534_IO_P06;
            pam_sp_table.Num_pin_GPIO_Ext_ID[2] = PCA9534_IO_P05;
            pam_sp_table.Num_pin_GPIO_Ext_ID[3] = PCA9534_IO_P04;
            pam_sp_table.Num_pin_GPIO_Ext_ID[4] = PCA9534_IO_P03;
            pam_sp_table.Num_pin_GPIO_Ext_ID[5] = PCA9534_IO_P02;*/

           //Norby 2
           //pam_sp_table.Num_pin_GPIO_Ext_ID[] =  No for Norby2

            pam_sp_table.PWR_number_ch_TM_SP = PAM_PWR_TM_SP_Ch2;

            pam_sp_table.temp_sensor_quantity = PAM_SP2_temp_sens_quantity;
            pam_sp_table.SP_I2C_addr_temp_senor[0] = PAM_SP2_I2CADDR_TEMP_SENS_1;
            pam_sp_table.SP_I2C_addr_temp_senor[1] = PAM_SP2_I2CADDR_TEMP_SENS_2;
            pam_sp_table.SP_I2C_addr_temp_senor[2] = PAM_SP2_I2CADDR_TEMP_SENS_3;
            pam_sp_table.SP_I2C_addr_temp_senor[3] = PAM_SP2_I2CADDR_TEMP_SENS_4;
            break;

        case PAM_SP3:
            pam_sp_table.PAM_I2C_MUX_Ch = TCA9548_CH4;

            pam_sp_table.ID_quantity = PAM_SP3_ID_quantity;
            //pam_sp_table.SP_I2C_addr_GPIO_Ext= PAM_SP3_I2CADDR_GPIO_Ext_1; No for Norbi
            //pam_sp_table.Num_pin_GPIO_Ext_ID[] =  No for Norbi, No for Norby2

            pam_sp_table.PWR_number_ch_TM_SP = PAM_PWR_TM_SP_Ch3;

            pam_sp_table.temp_sensor_quantity = PAM_SP3_temp_sens_quantity;
            pam_sp_table.SP_I2C_addr_temp_senor[0] = PAM_SP3_I2CADDR_TEMP_SENS_1;
            pam_sp_table.SP_I2C_addr_temp_senor[1] = PAM_SP3_I2CADDR_TEMP_SENS_2;
            pam_sp_table.SP_I2C_addr_temp_senor[2] = PAM_SP3_I2CADDR_TEMP_SENS_3;
            pam_sp_table.SP_I2C_addr_temp_senor[3] = PAM_SP3_I2CADDR_TEMP_SENS_4;
            break;

        case PAM_SP4:
            pam_sp_table.PAM_I2C_MUX_Ch = TCA9548_CH3;

            pam_sp_table.ID_quantity = PAM_SP4_ID_quantity;
            //Norby 1
            /*pam_sp_table.SP_I2C_addr_GPIO_Ext = PAM_SP2_I2CADDR_GPIO_Ext_1;
            pam_sp_table.Num_pin_GPIO_Ext_ID[0] = PCA9534_IO_P07;
            pam_sp_table.Num_pin_GPIO_Ext_ID[1] = PCA9534_IO_P06;
            pam_sp_table.Num_pin_GPIO_Ext_ID[2] = PCA9534_IO_P05;
            pam_sp_table.Num_pin_GPIO_Ext_ID[3] = PCA9534_IO_P04;
            pam_sp_table.Num_pin_GPIO_Ext_ID[4] = PCA9534_IO_P03;
            pam_sp_table.Num_pin_GPIO_Ext_ID[5] = PCA9534_IO_P02;*/

            //Norby2
            //pam_sp_table.Num_pin_GPIO_Ext_ID[] = No for Norby2

            pam_sp_table.PWR_number_ch_TM_SP = PAM_PWR_TM_SP_Ch4;

            pam_sp_table.temp_sensor_quantity = PAM_SP4_temp_sens_quantity;
            pam_sp_table.SP_I2C_addr_temp_senor[0] = PAM_SP4_I2CADDR_TEMP_SENS_1;
            pam_sp_table.SP_I2C_addr_temp_senor[1] = PAM_SP4_I2CADDR_TEMP_SENS_2;
            pam_sp_table.SP_I2C_addr_temp_senor[2] = PAM_SP4_I2CADDR_TEMP_SENS_3;
            pam_sp_table.SP_I2C_addr_temp_senor[3] = PAM_SP4_I2CADDR_TEMP_SENS_4;
            break;

        case PAM_SP5:
            pam_sp_table.PAM_I2C_MUX_Ch = TCA9548_CH2;

            pam_sp_table.ID_quantity = PAM_SP5_ID_quantity;
            pam_sp_table.SP_I2C_addr_GPIO_Ext = PAM_SP5_I2CADDR_GPIO_Ext_1;
            //Norbi 1
            /*pam_sp_table.Num_pin_GPIO_Ext_ID[0] = PCA9534_IO_P07;
            pam_sp_table.Num_pin_GPIO_Ext_ID[1] = PCA9534_IO_P05;*/

            //Norby 2
            pam_sp_table.Num_pin_GPIO_Ext_ID[0] = PCA9534_IO_P00;
            pam_sp_table.Num_pin_GPIO_Ext_ID[1] = PCA9534_IO_P01;

            pam_sp_table.PWR_number_ch_TM_SP = PAM_PWR_TM_SP_Ch5;

            pam_sp_table.temp_sensor_quantity = PAM_SP5_temp_sens_quantity;
            pam_sp_table.SP_I2C_addr_temp_senor[0] = PAM_SP5_I2CADDR_TEMP_SENS_1;
            pam_sp_table.SP_I2C_addr_temp_senor[1] = PAM_SP5_I2CADDR_TEMP_SENS_2;
            break;

        case PAM_SP6:
            pam_sp_table.PAM_I2C_MUX_Ch = TCA9548_CH5;

            pam_sp_table.ID_quantity = PAM_SP6_ID_quantity;
            pam_sp_table.SP_I2C_addr_GPIO_Ext= PAM_SP6_I2CADDR_GPIO_Ext_1;
            //Norby1
            /*pam_sp_table.Num_pin_GPIO_Ext_ID[0] = PCA9534_IO_P07;
            pam_sp_table.Num_pin_GPIO_Ext_ID[1] = PCA9534_IO_P06;
            pam_sp_table.Num_pin_GPIO_Ext_ID[2] = PCA9534_IO_P05;*/

            //Norby2
            pam_sp_table.Num_pin_GPIO_Ext_ID[0] = PCA9534_IO_P00;
            pam_sp_table.Num_pin_GPIO_Ext_ID[1] = PCA9534_IO_P01;

            pam_sp_table.PWR_number_ch_TM_SP = PAM_PWR_TM_SP_Ch6;

            pam_sp_table.temp_sensor_quantity = PAM_SP6_temp_sens_quantity;
            pam_sp_table.SP_I2C_addr_temp_senor[0] = PAM_SP6_I2CADDR_TEMP_SENS_1;
            pam_sp_table.SP_I2C_addr_temp_senor[1] = PAM_SP6_I2CADDR_TEMP_SENS_2;
            break;

        default:
            break;
    }

    return pam_sp_table;
}

