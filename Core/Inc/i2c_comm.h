
#ifndef INC_I2C_COMM_H_
#define INC_I2C_COMM_H_


#include "stm32l4xx.h"

#define I2C_TIMEOUT 1000000

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

ErrorStatus I2C_Read_byte_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr , uint32_t reg_addr, uint8_t *data);
ErrorStatus I2C_Read_word_u16_St_ReSt(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t *data);

ErrorStatus I2C_Write_byte_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint8_t data);
ErrorStatus I2C_Write_word_u16_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr, uint32_t reg_addr, uint16_t data);

ErrorStatus I2C_Write_array_u8_St(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t size_reg_addr,uint32_t reg_addr, uint8_t *data, uint32_t data_size);

#endif /* INC_I2C_COMM_H_ */
