#ifndef INC_FN_CRC16_H_
#define INC_FN_CRC16_H_

#ifdef __cplusplus
extern "C" {
#endif

unsigned short Crc16(unsigned char * pcBlock, unsigned short len);
uint16_t Crc16_1021(uint8_t * pcBlock, uint16_t len);

uint16_t norby_crc16_calc(uint8_t *buffer, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* INC_FN_CRC16_H_ */
