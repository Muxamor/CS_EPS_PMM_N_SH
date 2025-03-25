#ifndef INC_TCA6424A_H_
#define INC_TCA6424A_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TCA6424A_INPUT_PORT0_ADDR			0x00
#define TCA6424A_INPUT_PORT1_ADDR			0x01
#define TCA6424A_INPUT_PORT2_ADDR			0x02
#define TCA6424A_OUTPUT_PORT0_ADDR			0x04
#define TCA6424A_OUTPUT_PORT1_ADDR			0x05
#define TCA6424A_OUTPUT_PORT2_ADDR			0x06
#define TCA6424A_POL_INV_PORT0_ADDR			0x08
#define TCA6424A_POL_INV_PORT1_ADDR			0x09
#define TCA6424A_POL_INV_PORT2_ADDR			0x0A
#define TCA6424A_CONFIG_PORT0_ADDR			0x0C
#define TCA6424A_CONFIG_PORT1_ADDR			0x0D
#define TCA6424A_CONFIG_PORT2_ADDR			0x0E

//#define TCA6424A_AUTOINC_ADDR				0x80

#define TCA6424A_IO_P00						0x000001
#define TCA6424A_IO_P01						0x000002
#define TCA6424A_IO_P02						0x000004
#define TCA6424A_IO_P03						0x000008
#define TCA6424A_IO_P04						0x000010
#define TCA6424A_IO_P05						0x000020
#define TCA6424A_IO_P06						0x000040
#define TCA6424A_IO_P07						0x000080

#define TCA6424A_IO_P10						0x000100
#define TCA6424A_IO_P11						0x000200
#define TCA6424A_IO_P12						0x000400
#define TCA6424A_IO_P13						0x000800
#define TCA6424A_IO_P14						0x001000
#define TCA6424A_IO_P15						0x002000
#define TCA6424A_IO_P16						0x004000
#define TCA6424A_IO_P17						0x008000

#define TCA6424A_IO_P20						0x010000
#define TCA6424A_IO_P21						0x020000
#define TCA6424A_IO_P22						0x040000
#define TCA6424A_IO_P23						0x080000
#define TCA6424A_IO_P24						0x100000
#define TCA6424A_IO_P25						0x200000
#define TCA6424A_IO_P26						0x400000
#define TCA6424A_IO_P27						0x800000

#define TCA6424A_IO_ALL						0xFFFFFF

#define TCA6424A_ADDR 						0x22

ErrorStatus TCA6424A_Write_three_reg(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint32_t port_addr, uint8_t state);
ErrorStatus TCA6424A_Read_three_reg(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data, uint32_t port_addr);

ErrorStatus TCA6424A_read_IO_dir_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data);
ErrorStatus TCA6424A_read_output_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data);
ErrorStatus TCA6424A_read_input_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data);
ErrorStatus TCA6424A_read_IO_pol_regs(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t *read_data);

ErrorStatus TCA6424A_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state);
ErrorStatus TCA6424A_read_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state);
ErrorStatus TCA6424A_read_input_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state);
ErrorStatus TCA6424A_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin, uint8_t *pin_state);


ErrorStatus TCA6424A_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin);
ErrorStatus TCA6424A_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin);

ErrorStatus TCA6424A_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin);
ErrorStatus TCA6424A_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin);

ErrorStatus TCA6424A_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin);
ErrorStatus TCA6424A_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_TCA6424A_addr, uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* INC_TCA6424A_H_ */
