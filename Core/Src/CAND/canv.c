#include <stm32l496xx.h>
#define _OWN_HEADER_

#include <stdio.h>
#include "canv.h"

#define __dsb(v)  asm(" dsb")
#define __isb(v)  asm(" isb")


extern const typeRegistrationRec RegistrationRec[];


int CAN_FilterAssign(uint8_t filter_num, typeIdxMask id, typeIdxMask mask, uint8_t enable);


int CAN_Init(CAN_TypeDef *can_ref, uint32_t BTR_reg) {
  int tmout;
  if(can_ref == CAN1) {
    RCC->APB1RSTR1 |= RCC_APB1RSTR1_CAN1RST;
    RCC->APB1ENR1 |= RCC_APB1ENR1_CAN1EN;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_CAN1RST;
    }
  else if(can_ref == CAN2) {
    RCC->APB1RSTR1 |= RCC_APB1RSTR1_CAN2RST;
    RCC->APB1ENR1 |= RCC_APB1ENR1_CAN2EN;
    RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_CAN2RST;
    }
  else
    return ERR_INVALID_PARAMS;
  can_ref->MCR = CAN_MCR_INRQ;  //to init mode
  for(tmout=10000; tmout>0; tmout--)
    if(can_ref->MSR & CAN_MSR_INAK) break;
  if(tmout == 0)
    return ERR_CAN_INIT_MODE;
  /*
    config:
  - Receive FIFO locked mode;
  - Priority driven by the request order
  - No re-transmission
  */
  can_ref->MCR |= CAN_MCR_RFLM | CAN_MCR_TXFP | CAN_MCR_NART;
  /***   bit time   ***/
  can_ref->BTR = BTR_reg;
  /* filters: 32-bit, Identifer-Mask */
  if(can_ref == CAN1) {
    can_ref->FMR |= 1;  //init mode
    can_ref->FMR &= ~(0xFF<<8);
    can_ref->FMR |= (14<<8);  // CAN1's bank=0..13, CAN2's bank=14..27
    can_ref->FS1R = 0x0FFFFFFF;
    can_ref->FFA1R = 0;  //CAN1,2 assign to FIFO0
    can_ref->FMR &= ~1;  //active mode
    }
  /*interrupt*/
  can_ref->IER |= CAN_IER_FMPIE0;  //rx enable interrupt
  if(can_ref == CAN1)
    NVIC_EnableIRQ(CAN1_RX0_IRQn);
  else
    NVIC_EnableIRQ(CAN2_RX0_IRQn);
  /****/
  can_ref->MCR &= ~CAN_MCR_INRQ;  //to normal mode
  for(tmout=10000; tmout>0; tmout--)
    if((can_ref->MSR & CAN_MSR_INAK) == 0) break;
  if(tmout == 0)
    return ERR_CAN_NORMAL_MODE;
  return 0;
}


int CAN_RegisterVar(int filtr_num, uint8_t dev_id) {
  typeIdxMask id, mask;
  uint8_t access_flgs, dis;
  *((uint32_t*)&mask) = 0;
  *((uint32_t*)&id) = 0;
  id.uf.DevId = dev_id;
  id.uf.VarId = RegistrationRec[filtr_num].ivar_id;
  access_flgs = RegistrationRec[filtr_num].access_flgs;
  if(filtr_num >= 14)
    return ERR_CAN_NO_FREE_FILTER;
  if((access_flgs & CAN_AFLG_MASTER) == 0) {  //� �������� �� DevId � VarId
    if(access_flgs & CAN_AFLG_READONLY) {
      id.std.RTR = mask.std.RTR = 1;
      }
    else {
      id.std.RTR = mask.std.RTR = 0;
      }
    mask.uf.DevId = (int32_t)-1;  mask.uf.VarId = (int32_t)-1;
    dis = 0;  // ���������� �������
    }
  else {  //����������� ������
    id.u32 = 0;   mask.u32 = (uint32_t)-1;
    dis = 1;  // ���������� (����) �������
    }
  CAN_FilterAssign(filtr_num, id, mask, dis);
  return 0;
}


int CAN_Tx(CAN_TypeDef *can_ref, typeIdxMask id, void *p_data, uint16_t leng) {
  uint16_t n, i;
  uint8_t tme;
  uint64_t buff = 0;
  if(((can_ref != CAN1)&&(can_ref != CAN2))||(leng > 8))
    return ERR_INVALID_PARAMS;
  /*����� ���������� txmailbox-a*/
  tme = can_ref->TSR >> 26;
  for(n=0; n<3; n++) {
    if(tme & 1) break;
    tme = tme >> 1;
    }
  if(n >= 3) {
    /*clear bus-off state*/
    can_ref->MCR |= CAN_MCR_INRQ;
    can_ref->MCR &= ~CAN_MCR_INRQ;
    n = 0;
    }
  can_ref->sTxMailBox[n].TDTR = leng;
  if(id.std.RTR == 0) {
    if(leng) {
      for(i=0; i<leng; i++)
        ((uint8_t*)&buff)[i] = ((uint8_t*)p_data)[i];
      can_ref->sTxMailBox[n].TDLR = *((uint32_t*)&buff);
      can_ref->sTxMailBox[n].TDHR = *((uint32_t*)&buff+1);
      }
    }
  can_ref->sTxMailBox[n].TIR = *((uint32_t*)&id) | CAN_TI0R_IDE | 1;
  return 0;
}


int CAN_SetFilterId(int filtr_num, typeIdxMask id, uint8_t disable) {
  if(filtr_num >= 14) 
    return ERR_INVALID_PARAMS;
  CAN1->FA1R &= ~( (1<<(filtr_num+14)) | (1<<filtr_num) );  //filter off
  if(disable == 0) {
    id.std.IDE = 1;  //to extendet mode
    CAN1->sFilterRegister[filtr_num].FR1 = CAN1->sFilterRegister[filtr_num+14].FR1  =  *((uint32_t*)&id);
    CAN1->FA1R |=  ( (1<<(filtr_num+14)) | (1<<filtr_num) );  //filter on
    }
  return 0;
}


/*----------------------------------------------------------------------------*/


int CAN_FilterAssign(uint8_t filter_num, typeIdxMask id, typeIdxMask mask, uint8_t disable) {
  if(filter_num >= 14)
    return ERR_INVALID_PARAMS;
  CAN1->FA1R &=  ~( (1<<(filter_num+14)) | (1<<filter_num) );  //filter off
  id.std.IDE = mask.std.IDE = 1;  //to extendet mode
  CAN1->sFilterRegister[filter_num].FR1 = CAN1->sFilterRegister[filter_num+14].FR1  =  *((uint32_t*)&id);
  CAN1->sFilterRegister[filter_num].FR2 = CAN1->sFilterRegister[filter_num+14].FR2  =  *((uint32_t*)&mask);
  if(disable == 0)
    CAN1->FA1R |= (1<<(filter_num+14)) | (1<<filter_num);  //filter on
  return 0;
}


void CAN_RX_Handler(CAN_TypeDef *can_ref) {
  uint16_t i;
  uint16_t pkt_leng, filtr_num, bound;
  typeIdxMask id;
  uint8_t *pvar;
  int state = 0;
  *((uint32_t*)&id) = can_ref->sFIFOMailBox[0].RIR;
  pkt_leng = can_ref->sFIFOMailBox[0].RDTR & 0x000F;
  filtr_num = (can_ref->sFIFOMailBox[0].RDTR >> 8) & 0x00FF;
  pvar = (uint8_t*)((uint32_t)RegistrationRec[filtr_num].VarPtr);
  bound = pkt_leng;
  if(pkt_leng == 0) {  // ��� ������� - ��� �������������� ����� �� ������
    CANMasterWriteAckFlg = 1;
    can_ref->RF0R = 0x20;  //release FIFO
    return;
    }
  if(pkt_leng > 8)
    state = ERR_CAN_DCL_INVALID;
  else {
    if((RegistrationRec[filtr_num].access_flgs & CAN_AFLG_NOOFFSET) == 0) { //offset is needed?
      pvar = pvar + id.uf.Offset;
      bound = bound + id.uf.Offset;
      }
    /* 1-�� Callback */
    if(RegistrationRec[filtr_num].CallBackProc) {
      RegistrationRec[filtr_num].CallBackProc(can_ref, id, pkt_leng, state);
      }
    if(id.std.RTR == 0) {                                            
      /*** RTR == 0 - ��������� �� ������ ***/
      if(bound <= RegistrationRec[filtr_num].VarLeng) {
        /*���������� �������*/
        for(i=0; i<pkt_leng; i++)
          pvar[i] = ((uint8_t*)&can_ref->sFIFOMailBox[0].RDLR)[i];
        CANMasterRxFlg = 1;
        }
      else {
        /*������������ �������*/
        state = ERR_CAN_ACCESS_RANGE;
        }
      }
    else if((RegistrationRec[filtr_num].access_flgs & CAN_AFLG_RTRNORESP) == 0) {
      /*** RTR == 1 - ��������� �� ������ ***/      
      id.std.RTR = 0;
      state = CAN_Tx(can_ref, id, pvar, pkt_leng);
      id.std.RTR = 1;  //restore RTR
      }
    }
  can_ref->RF0R = 0x20;  //release FIFO
  /* 2-�� Callback */
  if(RegistrationRec[filtr_num].CallBackProc) {
    if(state == 0) state = 1;
    RegistrationRec[filtr_num].CallBackProc(can_ref, id, pkt_leng, state);
    }
  /*������������� ������*/  
  if((id.std.RTR == 0)&&(pkt_leng != 0)) {
    CAN_Tx(can_ref, id, (void*)0, 0);
    }
}


void CAN1_RX0_IRQHandler() {
	CAN_RX_Handler(CAN1);
	__dsb(0);__isb(0);
	#ifdef DEBUGprintf
		printf("RX_CAN1\n");
	#endif
}


void CAN2_RX0_IRQHandler() {
	CAN_RX_Handler(CAN2);
	__dsb(0);__isb(0);
	#ifdef DEBUGprintf
		printf("RX_CAN2\n");
	#endif
}

