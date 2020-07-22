
#ifndef INC_I2C_COMM_H_
#define INC_I2C_COMM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_TIMEOUT 1000 // 400kHz - 150, 100kHz - 600.

#define I2C_SIZE_REG_ADDR_U8  	1 
#define I2C_SIZE_REG_ADDR_U16  	2 
#define I2C_SIZE_REG_ADDR_U24  	3 
#define I2C_SIZE_REG_ADDR_U32  	4 

/*
typedef enum {
  ERROR_N = -1,	
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

ErrorStatus I2C_check_flag(uint32_t (*I2C_check_flag)(I2C_TypeDef *), I2C_TypeDef *I2Cx, uint8_t flag_state); 
void I2C_Clear_Error_Flags(I2C_TypeDef *I2Cx);

ErrorStatus I2C_Read_byte_directly_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t *data);
ErrorStatus I2C_Read_byte_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr , uint32_t reg_addr, uint8_t *data);
ErrorStatus I2C_Read_word_u16_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t *data);

ErrorStatus I2C_Write_byte_directly_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t data);
ErrorStatus I2C_Write_byte_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint8_t data);
ErrorStatus I2C_Write_word_u16_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t data);

ErrorStatus I2C_Write_array_u8_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr,uint32_t reg_addr, uint8_t *data, uint32_t data_size);

#ifdef __cplusplus
}
#endif

#endif /* INC_I2C_COMM_H_ */
