#ifndef INC_TCA9548_H_
#define INC_TCA9548_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TCA9548_CH0				0x01
#define TCA9548_CH1				0x02
#define TCA9548_CH2				0x04
#define TCA9548_CH3				0x08
#define TCA9548_CH4				0x10
#define TCA9548_CH5				0x20
#define TCA9548_CH6				0x40
#define TCA9548_CH7				0x80
#define TCA9548_ALL_CHANNELS	0xFF



/*
typedef enum {
  ERROR_N = -1,
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

ErrorStatus TCA9548_Read_reg(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t *data_reg);
ErrorStatus TCA9548_Write_reg(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t data_reg);

ErrorStatus TCA9548_Enable_I2C_ch(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t I2C_ch);
ErrorStatus TCA9548_Disable_I2C_ch(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t I2C_ch);

ErrorStatus TCA9548_Get_state_I2C_ch(I2C_TypeDef *I2Cx, uint8_t tca9548_I2C_addr, uint8_t I2C_ch, uint8_t *ch_state);

#ifdef __cplusplus
}
#endif

#endif /* INC_TCA9548_H_ */
