#ifndef _CANV_H_
#define _CANV_H_

#ifdef __cplusplus
}
#endif

typedef union {
  struct {
    uint32_t res1 : 1;
    uint32_t RTR : 1;
    uint32_t IDE : 1;
    uint32_t EXID : 29;
    }std;
  struct {
    uint32_t res1 : 1;
    uint32_t RTR : 1;
    uint32_t res2 : 1;
    uint32_t Offset : 21;
    uint32_t VarId : 4;
    uint32_t DevId : 4;
    }uf;
  uint32_t u32;
}typeIdxMask;


#pragma pack(push, 2)

typedef struct {
  void *VarPtr;
  uint32_t VarLeng;
  void (*CallBackProc)(CAN_TypeDef *can_ref, typeIdxMask id, uint16_t leng, int state);
  uint8_t ivar_id;
  uint8_t access_flgs;  //[0]=1 - только чтение; [1]=1 - без учета offset; [2]=1 - управляемый фильтр; [3]=1 - без ответа на RTR=1(для CANBridge)
}typeRegistrationRec;

#pragma pack(pop)


#define ERR_INVALID_PARAMS        -1
#define ERR_CAN_INIT_MODE         -2
#define ERR_CAN_NORMAL_MODE       -3
#define ERR_CAN_NO_FREE_FILTER    -4
#define ERR_CAN_NO_TXMAILBOXES    -5
#define ERR_CAN_ACCESS_RANGE      -7
#define ERR_CAN_DCL_INVALID       -8

#define CAN_AFLG_READONLY     (1<<0)
#define CAN_AFLG_NOOFFSET     (1<<1)
#define CAN_AFLG_MASTER       (1<<2)
#define CAN_AFLG_RTRNORESP    (1<<3)


int CAN_Init(CAN_TypeDef *can_ref, uint32_t BTR_reg);
int CAN_RegisterVar(int filtr_num, uint8_t dev_id);
int CAN_Tx(CAN_TypeDef *can_ref, typeIdxMask id, void *p_data, uint16_t leng);
int CAN_SetFilterId(int filtr_num, typeIdxMask id, uint8_t disable);
#ifdef _OWN_HEADER_
uint8_t CANMasterWriteAckFlg = 0;
uint8_t CANMasterRxFlg = 0;
#else
extern uint8_t CANMasterWriteAckFlg;
extern uint8_t CANMasterRxFlg;
#endif


#ifdef __cplusplus
}
#endif

#endif


