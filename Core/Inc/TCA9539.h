#ifndef INC_TCA9539_H_
#define INC_TCA9539_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
typedef enum {
  ERROR_N = -1,
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */


#define TCA9539_Input_Port0					0x00
#define TCA9539_Input_Port1					0x01
#define TCA9539_Output_Port0				0x02
#define TCA9539_Output_Port1				0x03
#define TCA9539_Polarity_Inversion_Port0	0x04
#define TCA9539_Polarity_Inversion_Port1	0x05
#define TCA9539_Configuration_Port0			0x06
#define TCA9539_Configuration_Port1			0x07

#define TCA9539_IO_ALL						0xFFFF

#define TCA9539_IO_P00						0x0001
#define TCA9539_IO_P01						0x0002
#define TCA9539_IO_P02						0x0004
#define TCA9539_IO_P03						0x0008
#define TCA9539_IO_P04						0x0010
#define TCA9539_IO_P05						0x0020
#define TCA9539_IO_P06						0x0040
#define TCA9539_IO_P07						0x0080

#define TCA9539_IO_P10						0x0100
#define TCA9539_IO_P11						0x0200
#define TCA9539_IO_P12						0x0400
#define TCA9539_IO_P13						0x0800
#define TCA9539_IO_P14						0x1000
#define TCA9539_IO_P15						0x2000
#define TCA9539_IO_P16						0x4000
#define TCA9539_IO_P17						0x8000



ErrorStatus TCA9539_read_IO_dir_reg(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t *read_data);
ErrorStatus TCA9539_read_output_reg(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t *read_data);
ErrorStatus TCA9539_read_input_reg(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t *read_data);
ErrorStatus TCA9539_read_IO_pol_reg(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t *read_data);

ErrorStatus TCA9539_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin, uint8_t *read_data);
ErrorStatus TCA9539_read_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin, uint8_t *read_data);
ErrorStatus TCA9539_read_input_pin(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin, uint8_t *read_data);
ErrorStatus TCA9539_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin, uint8_t *read_data);

ErrorStatus TCA9539_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin);
ErrorStatus TCA9539_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin);

ErrorStatus TCA9539_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin);
ErrorStatus TCA9539_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin);

ErrorStatus TCA9539_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin);
ErrorStatus TCA9539_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_tca9539_addr, uint16_t pin);

#ifdef __cplusplus
}
#endif

#endif /* INC_TCA9539_H_ */
