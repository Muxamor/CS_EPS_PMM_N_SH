
#include "main.h"
#include  "Error_Handler.h"
#include "SetupPeriph.h"
#include "i2c_comm.h"
#include "tim_pwm.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "TMP1075.h"
#include "FRAM.h"
#include "CAND/CAN.h"
#include "CAND/CAN_cmd.h"
#include "ADS1015.h"
#include "TIM_delay.h"

#include "pmm_deploy.h"
#include "pmm_config.h"
#include "pmm_init.h"

#include  <stdio.h>

/****************************TODO*************************
1. Need to think about delay 30 minuts. 
2. Need change constatn mode EN/Dis after teste with Doroshkin in CAN_cmd.c
3. Need see use analog filter in I2C


**********************************************************/

extern uint32_t CAN_cmd_mask_status;

//LL_mDelay(1);
//LL_RCC_ClocksTypeDef check_RCC_Clocks,  *CHECK_RCC_CLOCKS=&check_RCC_Clocks; // Only for check setup clock. Not need use in release

#define TCA9539_I2C_ADDR						0x74
#define I2C_ADS1015_ADDR						0x48
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

#define ADS1015_LSB_SIZE_FSR		ADS1015_LSB_SIZE_FSR_256mV
//#define I2C_FRAM1_addr 			0b01010000
//#define I2C_FRAM2_addr			0b01010001

//#pragma pack(push, 1)
//
// typedef struct {
//	  uint8_t var1;
//	  uint32_t var2;
//	  uint16_t var3;
//	  uint8_t  var4;
// } data_struct_1;
//
// typedef struct {
//	  uint32_t var1;
//	  uint16_t var2;
//	  uint16_t var3;
//	  uint8_t  var4;
//	  uint32_t var5;
//	  uint8_t  var6;
// } data_struct_2;
//
// typedef struct {
//	  uint32_t var1;
//	  uint32_t var2;
//	  uint16_t var3;
//	  uint8_t  var4;
//	  uint32_t var5;
// } data_struct_3;
//
// typedef struct {
//	  uint8_t  var1;
//	  uint32_t var2;
//	  uint16_t  var3;
// } data_struct_4;
//
// typedef struct {
//	 uint8_t  var1; //data_struct_1.var1
//	 uint32_t var2; //data_struct_1.var2
//	 uint16_t var3; //data_struct_2.var3
//	 uint8_t  var4; //data_struct_2.var4
//	 uint32_t var5; //data_struct_3.var5
//	 uint8_t  var6; //data_struct_3.var4
//	 uint16_t var7; //data_struct_4.var3
//	 uint8_t  var8; //data_struct_4.var1
// } FRAM_STRUCT; //size = 16
//
// #pragma pack(pop)
  /* USER CODE END 2 */


int main(void){

 	CAN_cmd_mask_status = 0;

	/** Initialization Periph STM32L496*/
	LL_Init();
	SystemClock_Config();
	//LL_RCC_GetSystemClocksFreq(CHECK_RCC_CLOCKS); // Only for check setup clock Not need use in release
	I2C3_Init();
	I2C4_Init();
	LPUART1_Init();
	USART3_Init();
	UART5_Init();
	GPIO_Init();
//	MX_TIM7_Init();
	PWM_init(100000, 50, 0); //F=100kHz, Duty = 50%, tim devider=0

  	LL_GPIO_SetOutputPin(GPIOF, LL_GPIO_PIN_0);
  	LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_12);
  	LL_GPIO_SetOutputPin(GPIOG, LL_GPIO_PIN_13);
	//IWDG_Init();

	uint16_t TCA9539_CONFIG_register, TCA9539_OUTPUT_register, TCA9539_POLARITY_register, TCA9539_INPUT_register = 0;
	int8_t status = 0;
	status += TCA9539_conf_IO_dir_input(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);
//	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR,  TCA9539_IO_P17 | TCA9539_IO_P15); // Turn on only CANbus
	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR,  TCA9539_IO_P10 | TCA9539_IO_P11); // Turn on deploy only

	status += TCA9539_read_IO_dir_reg(I2C3, TCA9539_I2C_ADDR, &TCA9539_CONFIG_register);
	status += TCA9539_read_input_reg(I2C3, TCA9539_I2C_ADDR, &TCA9539_INPUT_register);
	status += TCA9539_read_output_reg(I2C3, TCA9539_I2C_ADDR, &TCA9539_OUTPUT_register);
	status += TCA9539_read_IO_pol_reg(I2C3, TCA9539_I2C_ADDR, &TCA9539_POLARITY_register);

	printf("initial TCA9539_CONFIG_register = 0x%02X\n", TCA9539_CONFIG_register);
	printf("initial TCA9539_OUTPUT_register = 0x%02X\n", TCA9539_OUTPUT_register);
	printf("initial TCA9539_POLARITY_register = 0x%02X\n", TCA9539_POLARITY_register);
	printf("initial TCA9539_INPUT_register = 0x%02X\n\n", TCA9539_INPUT_register);

	status += CAN_Init(CAN1);
	status += CAN_Init(CAN2);
	CAN_RegisterAllVars();
	SetupInterrupt();

//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_start_channel(TIM3, LL_TIM_CHANNEL_CH4);

//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH3);
//	PWM_stop_channel(TIM3, LL_TIM_CHANNEL_CH4);

#ifdef DEBUGprintf

		printf("test  \n");
		//	Error_Handler();

#endif

//while(1){

//}

#ifdef DEBUGprintf
uint32_t last_cmd_mask_status = 0;
#endif

	uint16_t data = 0;
	float fdata = 0;
	uint8_t reg_byte = 0;
	status += ADS1015_setup_default_values(I2C4, I2C_ADS1015_ADDR);
	LL_mDelay(100);
	status += ADS1015_read_config_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_config_reg = 0x%02X\n", data);
	status += ADS1015_read_conv_status(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_conversion_status = 0x%01X\n", reg_byte);
	status += ADS1015_read_mux(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_mult_config = 0x%01X\n", reg_byte);
	status += ADS1015_read_gain_FSR(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_gain_config = 0x%01X\n", reg_byte);
	status += ADS1015_read_conv_mode(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_operation_mode = 0x%01X\n", reg_byte);
	status += ADS1015_read_conv_data_rate(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_data_rate = 0x%01X\n", reg_byte);
	status += ADS1015_read_comp_mode(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_comparator_mode = 0x%01X\n", reg_byte);
	status += ADS1015_read_comp_pol(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_comparator_polarity = 0x%01X\n", reg_byte);
	status += ADS1015_read_latching_comp(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_latching_comparator = 0x%01X\n", reg_byte);
	status += ADS1015_read_comp_queue(I2C4, I2C_ADS1015_ADDR, &reg_byte);
	printf("ADS1015_read_comparator_queue = 0x%01X\n", reg_byte);
	status += ADS1015_read_conv_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_read_conversion_reg(before) = 0x%02X\n", data);
	status += ADS1015_start_single_conv(I2C4, I2C_ADS1015_ADDR);
	LL_mDelay(200);
	status += ADS1015_read_conv_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_read_conversion_reg(after) = 0x%02X\n", data);



//	status += ADS1015_setup_conv_data_rate(I2C4, I2C_ADS1015_ADDR, ADS1015_2400_SPS);
	status += ADS1015_setup_gain_FSR(I2C4, I2C_ADS1015_ADDR, ADS1015_FSR_4096mV);
	status += ADS1015_setup_mux(I2C4, I2C_ADS1015_ADDR, ADS1015_AINp0_AINnGND);
	status += ADS1015_setup_conv_mode(I2C4, I2C_ADS1015_ADDR, ADS1015_CONT_CONV_MODE);
//	status += ADS1015_setup_comp_pol(I2C4, I2C_ADS1015_ADDR, ADS1015_ACTIVE_HIGH_POL);
//	status += ADS1015_setup_latching_comp(I2C4, I2C_ADS1015_ADDR, ADS1015_LATCHING_COMP);
	status += ADS1015_read_config_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_config_reg = 0x%02X\n", data);

	status += ADS1015_read_conv_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_read_conversion_reg(after) = 0x%02X\n", data);

	status += ADS1015_read_lo_thresh_val(I2C4, I2C_ADS1015_ADDR, &fdata);
	printf("ADS1015_read_lo_thresh_reg(before) = %.03f\n", fdata);
	status += ADS1015_read_hi_thresh_val(I2C4, I2C_ADS1015_ADDR, &fdata);
	printf("ADS1015_read_hi_thresh_reg(before) = %.03f\n", fdata);

	status += ADS1015_read_lo_thresh_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_read_lo_thresh_reg(before) = 0x%02X\n", data);
	status += ADS1015_read_hi_thresh_reg(I2C4, I2C_ADS1015_ADDR, &data);
	printf("ADS1015_read_hi_thresh_reg(before) = 0x%02X\n", data);

	ADS1015_setup_lo_thresh_val(I2C4, I2C_ADS1015_ADDR, -0.250);
	ADS1015_setup_hi_thresh_val(I2C4, I2C_ADS1015_ADDR, 0.120);
	status += ADS1015_read_lo_thresh_val(I2C4, I2C_ADS1015_ADDR, &fdata);
	printf("ADS1015_read_lo_thresh_reg(after) = %.03f\n", data);
	status += ADS1015_read_hi_thresh_val(I2C4, I2C_ADS1015_ADDR, &fdata);
	printf("ADS1015_read_hi_thresh_reg(after) = %.03f\n", data);
	printf("error status = %d\n", status);

	int16_t voltage = 0;
	float fvoltage = 0;

	printf("voltage 3.452 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, 3.452)));
	printf("voltage -3.452 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -3.452)));
	printf("voltage -4.094 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -4.094)));
	printf("voltage -0.150 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -0.15)));
	printf("voltage -0.001 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -0.001)));
	printf("voltage -0.002 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -0.002)));
	printf("voltage -0.007 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -0.007)));
	printf("voltage 0.001 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, 0.001)));
	printf("voltage 0.007 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, 0.007)));
	printf("voltage 0.000 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, 0)));
	printf("voltage 0.100 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, 0.1)));
	printf("voltage -0.016 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -0.016)));
	printf("voltage -0.256 = %.3f\n", ADS1015_raw_to_Volts(ADS1015_LSB_SIZE_FSR, ADS1015_Volts_to_raw(ADS1015_LSB_SIZE_FSR, -0.256)));



//	data_struct_1 struct_1 ={
//	  .var1 = 0x15,
//	  .var2 = 0x12345678,
//	  .var3 = 0xABCD,
//	  .var4 = 0xEF
//	};
//	data_struct_2 struct_2 ={
//	  .var1 = 0x12345678,
//	  .var2 = 0xABCD,
//	  .var3 = 0xCDAB,
//	  .var4 = 0xEF,
//	  .var5 = 0xABCDEF11,
//	  .var6 = 0xFF
//	};
//	data_struct_3 struct_3 ={
//	  .var1 = 0x12345678,
//	  .var2 = 0xABCD,
//	  .var3 = 0xCDAB,
//	  .var4 = 0xEF,
//	  .var5 = 0x01020304
//	};
//	data_struct_4 struct_4 ={
//	  .var1 = 0x35,
//	  .var2 = 0xA0B0C0D0,
//	  .var3 = 0xFFFF
//	};
//	data_struct_1 read1;
//	data_struct_2 read2;
//	data_struct_3 read3;
//	data_struct_4 read4;
//	status += FRAM_save_data(I2C3, I2C_FRAM1_addr, I2C_FRAM2_addr, &struct_1, &struct_2, &struct_3, &struct_4);
//
//	status += FRAM_read_data(I2C3, I2C_FRAM1_addr, I2C_FRAM2_addr, &read1, &read2, &read3, &read4);
//	printf("error status = %d\n", status);
//	printf("\nstruct_1.var1 = 0x%02X\n", read1.var1);
//	printf("struct_1.var2 = 0x%02X\n", read1.var2);
//	printf("struct_2.var3 = 0x%02X\n", read2.var3);
//	printf("struct_2.var4 = 0x%02X\n", read2.var4);
//	printf("struct_3.var5 = 0x%02X\n", read3.var5);
//	printf("struct_3.var4 = 0x%02X\n", read3.var4);
//	printf("struct_4.var3 = 0x%02X\n", read4.var3);
//	printf("struct_4.var1 = 0x%02X\n\n", read4.var1);
//
//	FRAM_erase(I2C3, I2C_FRAM1_addr, FRAM_SIZE_64KB);
//	FRAM_erase(I2C3, I2C_FRAM2_addr, FRAM_SIZE_64KB);
//	status += FRAM_read_data(I2C3, I2C_FRAM1_addr, I2C_FRAM2_addr, &read1, &read2, &read3, &read4);
//	printf("error status = %d\n", status);
//	printf("\nstruct_1.var1 = 0x%02X\n", read1.var1);
//	printf("struct_1.var2 = 0x%02X\n", read1.var2);
//	printf("struct_2.var3 = 0x%02X\n", read2.var3);
//	printf("struct_2.var4 = 0x%02X\n", read2.var4);
//	printf("struct_3.var5 = 0x%02X\n", read3.var5);
//	printf("struct_3.var4 = 0x%02X\n", read3.var4);
//	printf("struct_4.var3 = 0x%02X\n", read4.var3);
//	printf("struct_4.var1 = 0x%02X\n\n", read4.var1);
	while (1){


		#ifdef DEBUGprintf
			if(last_cmd_mask_status != CAN_cmd_mask_status){
				last_cmd_mask_status = CAN_cmd_mask_status;
				printf("cmd_reg status: " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN " " BYTE_TO_BINARY_PATTERN"\n",
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 24), BYTE_TO_BINARY(CAN_cmd_mask_status >> 16),
				BYTE_TO_BINARY(CAN_cmd_mask_status >> 8), BYTE_TO_BINARY(CAN_cmd_mask_status));
			}
		#endif

		CAN_Var4_cmd_parser(&CAN_cmd_mask_status);

		status += ADS1015_start_single_conv(I2C4, I2C_ADS1015_ADDR);
		LL_mDelay(1000);
//		TIM_delay_ms(1000);
//		status += ADS1015_read_conv_reg(I2C4, I2C_ADS1015_ADDR, &data);
//		printf("ADS1015_read_conversion_reg(after) = 0x%02X\n", data);
//		ADS1015_raw_to_millivolts(ADS1015_VOLTAGE_RANGE, data, &voltage);
//		printf("voltage = %d\n", voltage);

//		ADS1015_read_mVolts_int16(I2C4, I2C_ADS1015_ADDR, &voltage);
//		printf("voltage = %d\n", voltage);
//
//		ADS1015_read_Volts_float(I2C4, I2C_ADS1015_ADDR, &fvoltage);
//		printf("voltage = %.3f\n", fvoltage);
//
//		ADS1015_read_lo_thresh_val(I2C4, I2C_ADS1015_ADDR, &fvoltage);
//		printf("lo voltage = %.3f\n", fvoltage);
//		ADS1015_read_hi_thresh_val(I2C4, I2C_ADS1015_ADDR, &fvoltage);
//		printf("hi voltage = %.3f\n", fvoltage);
//		if(TIM_finished)
//			TIM2_init(1000 * 60 * 30);
		PMM_Deploy();


	}

}

