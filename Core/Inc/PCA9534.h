
#ifndef INC_PCA9534_H_
#define INC_PCA9534_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
typedef enum {
  ERROR_N = -1,
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

#define PCA9534_Input_Port					0x00
#define PCA9534_Output_Port					0x01
#define PCA9534_Polarity_Inversion_Port		0x02
#define PCA9534_Configuration_Port			0x03

#define PCA9534_IO_ALL						0xFF
#define PCA9534_IO_P00						0x01
#define PCA9534_IO_P01						0x02
#define PCA9534_IO_P02						0x04
#define PCA9534_IO_P03						0x08
#define PCA9534_IO_P04						0x10
#define PCA9534_IO_P05						0x20
#define PCA9534_IO_P06						0x40
#define PCA9534_IO_P07						0x80



ErrorStatus PCA9534_read_IO_dir_reg(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t *read_data);
ErrorStatus PCA9534_read_output_reg(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t *read_data);
ErrorStatus PCA9534_read_input_reg(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t *read_data);
ErrorStatus PCA9534_read_IO_pol_reg(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t *read_data);

ErrorStatus PCA9534_read_IO_dir_pin(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin, uint8_t *read_data);
ErrorStatus PCA9534_read_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin, uint8_t *read_data);
ErrorStatus PCA9534_read_input_pin(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin, uint8_t *read_data);
ErrorStatus PCA9534_read_IO_pol_pin(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin, uint8_t *read_data);

ErrorStatus PCA9534_conf_IO_dir_output(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin);
ErrorStatus PCA9534_conf_IO_dir_input(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin);

ErrorStatus PCA9534_conf_IO_pol_normal(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin);
ErrorStatus PCA9534_conf_IO_pol_inversion(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin);

ErrorStatus PCA9534_Set_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin);
ErrorStatus PCA9534_Reset_output_pin(I2C_TypeDef *I2Cx, uint8_t I2C_PCA9534_addr, uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif /* INC_PCA9534_H_ */
