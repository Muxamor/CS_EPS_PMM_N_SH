#ifndef INC_PAM_PAM_STRUCT_H_
#define INC_PAM_PAM_STRUCT_H_

#ifdef __cplusplus
extern "C" {
#endif


//#pragma pack(push, 1)

typedef struct{
	
	uint8_t Some_byte1;
	uint8_t Some_byte2;

}_PAM_SP;


typedef struct{
	
	uint8_t Some_byte1;
	uint8_t Some_byte2;

}_PAM_PWR_CH;

typedef struct{

	uint8_t Some_byte1;
	
	uint8_t :7;
	uint8_t PAM_save_conf_flag :1;

	uint16_t reserved1;
	uint16_t reserved2;

}_PAM;

//#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* INC_PDM_PDM_STRUCT_H_ */
