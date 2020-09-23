#ifndef INC_FRAM_H_
#define INC_FRAM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define fram_i2c_attempt_conn 3
#define fram_i2c_delay_att_conn	 3 	 //ms Delay between attempts connection through I2C in millisecond

#define FRAM_Addr_segment_1		0x0000
#define FRAM_Addr_segment_2		0x5555
#define FRAM_Addr_segment_3		0xAAAA
#define FRAM_SIZE_64KB  0x2000


#define FRAM_WRITE_PROTECTION_ENABLE	1
#define FRAM_WRITE_PROTECTION_DISABLE	0

/*
typedef enum {
  ERROR_N = -1,	
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

ErrorStatus FRAM_erase(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint32_t fram_size);
int8_t FRAM_Detect_Empty( I2C_TypeDef *I2Cx_fram, uint8_t I2C_addr_fram, uint32_t fram_size );

ErrorStatus FRAM_set_write_access(uint8_t access_flag);

ErrorStatus FRAM_triple_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size);
ErrorStatus FRAM_triple_verif_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size);

ErrorStatus FRAM_majority_read_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size );
ErrorStatus FRAM_majority_read_data_two_fram(I2C_TypeDef *I2Cx, uint8_t I2C_addr_fram_main, uint8_t I2C_addr_fram_backup, uint8_t *ptr_data, uint32_t data_size );

ErrorStatus FRAM_majority_read_byte(I2C_TypeDef *I2Cx, uint8_t fram_addr, uint32_t offset, uint8_t *read_byte);

#ifdef __cplusplus
}
#endif

#endif /* INC_FRAM_H_ */
