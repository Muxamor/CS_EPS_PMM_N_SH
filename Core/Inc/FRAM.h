#ifndef INC_FRAM_H_
#define INC_FRAM_H_

//#define I2C_FRAM1_addr 			0b01010000
//#define I2C_FRAM2_addr			0b01010001

#define FRAM_Addr_segment_1		0x0000
#define FRAM_Addr_segment_2		0x5555
#define FRAM_Addr_segment_3		0xAAAA


#define FRAM_WRITE_PROTECTION_ENABLE	1
#define FRAM_WRITE_PROTECTION_DISABLE	0


/*
typedef enum {
  ERROR_N = -1,	
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */

//#pragma pack(1)
#pragma pack(push, 1)

 typedef struct {
	  uint8_t var1;
	  uint32_t var2;
	  uint16_t var3;
	  uint8_t  var4;
 } data_struct_1;

 typedef struct {
	  uint32_t var1;
	  uint16_t var2;
	  uint16_t var3;
	  uint8_t  var4;
	  uint32_t var5;
	  uint8_t  var6;
 } data_struct_2;

 typedef struct {
	  uint32_t var1;
	  uint32_t var2;
	  uint16_t var3;
	  uint8_t  var4;
	  uint32_t var5;
 } data_struct_3;

 typedef struct {
	  uint8_t  var1;
	  uint32_t var2;
	  uint16_t  var3;
 } data_struct_4;

 typedef struct {
	 uint8_t  var1; //data_struct_1.var1
	 uint32_t var2; //data_struct_1.var2
	 uint16_t var3; //data_struct_2.var3
	 uint8_t  var4; //data_struct_2.var4
	 uint32_t var5; //data_struct_3.var5
	 uint8_t  var6; //data_struct_3.var4
	 uint16_t var7; //data_struct_4.var3
	 uint8_t  var8; //data_struct_4.var1
 } FRAM_STRUCT; //size = 16

 #pragma pack(pop)


ErrorStatus FRAM_set_write_access(uint8_t access_flag);

ErrorStatus FRAM_triple_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size);
ErrorStatus FRAM_triple_verif_write_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size);

ErrorStatus FRAM_majority_read_data(I2C_TypeDef *I2Cx, uint8_t I2C_fram_addr, uint8_t *ptr_data, uint32_t data_size );
ErrorStatus FRAM_majority_read_data_two_fram(I2C_TypeDef *I2Cx, uint8_t I2C_addr_fram_main, uint8_t I2C_addr_fram_backup, uint8_t *ptr_data, uint32_t data_size );

ErrorStatus FRAM_majority_read_byte(I2C_TypeDef *I2Cx, uint8_t fram_addr, uint32_t offset, uint8_t *read_byte);

ErrorStatus FRAM_save_data(I2C_TypeDef *I2Cx, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, data_struct_1 *struct_1, data_struct_2 *struct_2, data_struct_3 *struct_3, data_struct_4 *struct_4);
ErrorStatus FRAM_read_data(I2C_TypeDef *I2Cx, uint8_t i2c_addr_fram1, uint8_t i2c_addr_fram2, data_struct_1 *struct_1, data_struct_2 *struct_2, data_struct_3 *struct_3, data_struct_4 *struct_4);

#endif /* INC_FRAM_H_ */
