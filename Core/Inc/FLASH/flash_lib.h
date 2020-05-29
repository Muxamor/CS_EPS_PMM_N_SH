#ifndef _FLASH_LIB_
#define _FLASH_LIB_


#ifdef __cplusplus
extern "C" {
#endif
void CallbackCAN_Flash(CAN_TypeDef *can_ref, typeIdxMask id, uint16_t leng, int state);
uint8_t HaveRunFlashBlockNum(void);  // return 0 - 1-й блок, 1 - 2-й блок


#ifdef __cplusplus
}/*extern "C"*/
#endif
extern uint8_t VarCAN_FlashFragment[];


#endif

