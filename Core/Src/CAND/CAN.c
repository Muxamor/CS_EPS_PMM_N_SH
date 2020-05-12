#include "CAN.h"
#include <stdio.h>

//__attribute__((weak)) typeRegistrationRec RegistrationRec[14] = {0};

const CAN_typeRegistrationRec RegistrationRec[] = {
/*{указатель на IVar, размер IVar, указатель на Callback, id_IVar, доступ}*/
	{&CAN_IVar4_RegCmd, sizeof(CAN_IVar4_RegCmd), CAN_ProcCallbackCmds, CAN_ID_IVAR_REGCMD, 0},
	{&CAN_IVar5_telemetry, sizeof(CAN_IVar5_telemetry), CAN_ProcCallbackTelemetry, CAN_ID_VAR_TELEMETRY, 1},  //256k size, read-only, regarless offset
	{&(*((uint32_t*) CAN_ROM_ADDR)), 4, CAN_ProcCallbackTelemetry, CAN_ID_VAR_ROM, 1},
	{&(*((uint32_t*) CAN_RAM_ADDR)), 4, CAN_ProcCallbackTelemetry, CAN_ID_VAR_RAM, 1}
};


void CAN_ProcCallbackCmds(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state) {
  if(state == 0)

    return;  //первый вызов тут не нужен

  if(state == 2)
	  CAN_cmd_mask_status |= (1 << id.uf.Offset);
}


void CAN_ProcCallbackTelemetry(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state) {
	return;
}


int8_t CAN_RegisterVar(int n, uint8_t dev_id) {
	CAN_typeIdxMask id, mask;
	uint8_t access_flgs;
	id.uf.DevId = dev_id;
	id.uf.VarId = RegistrationRec[n].ivar_id;
	access_flgs = RegistrationRec[n].access_flgs;
	if(n >= 14){
		Error_Handler();
		return ERR_CAN_NO_FREE_FILTER;
	}
	id.uf.Offset = 0;
	*((uint32_t*)&mask) = 0;
	if((access_flgs & 4) == 0) {  //with filter
		if(access_flgs & 1) {
			id.std.RTR = mask.std.RTR = 1;
		}
		else {
			id.std.RTR = mask.std.RTR = 0;
		}
		//    mask.uf.DevId = (uint32_t)-1;
		//    mask.uf.VarId = (uint32_t)-1;
		mask.uf.DevId = 0x0F;
		mask.uf.VarId = 0x0F;
	}
	CAN_FilterAssign(n, id, mask);

	return n;
}


int8_t CAN_DeregisterVar(int filter_num) {
	if((uint32_t)filter_num >= 14){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
		return ERR_INVALID_PARAMS;
	}
	CAN_FilterDeassign(filter_num);

	return 0;
}


int8_t CAN_Tx(CAN_TypeDef *can_ref, CAN_typeIdxMask id, void *p_data, uint16_t leng) {
	uint16_t n, i;
	uint8_t tme;
	uint64_t buff = 0;
	if(((can_ref != CAN1)&&(can_ref != CAN2))||(leng > 8)){
		Error_Handler();
		return ERR_INVALID_PARAMS;
	}
	/*поиск свободного txmailbox-a*/
	tme = can_ref->TSR >> 26;
	for(n=0; n<3; n++) {
		if(tme & 1){
			break;
		}
		tme = tme >> 1;
	}
	if(n >= 3){
		Error_Handler();
		return ERR_CAN_NO_TXMAILBOXES;
	}
	can_ref->sTxMailBox[n].TDTR = leng;
	if(id.std.RTR == 0) {
		for(i=0; i<leng; i++){
			((uint8_t*)&buff)[i] = ((uint8_t*)p_data)[i];
		}
		can_ref->sTxMailBox[n].TDLR = *((uint32_t*)&buff);
		can_ref->sTxMailBox[n].TDHR = *((uint32_t*)&buff+1);
	}
	can_ref->sTxMailBox[n].TIR = *((uint32_t*)&id) | CAN_TI0R_IDE | 1;
	can_ref->sTxMailBox[n].TIR |= 1;

	return 0;
}


int8_t CAN_RegisterAllVars(){
	uint8_t n;
	int8_t err = 0;
	for(n = 0; n < sizeof(RegistrationRec) / sizeof(CAN_typeRegistrationRec); n++) {
		err = CAN_RegisterVar(n, CAN_ID_DEV);
		#ifdef DEBUGprintf
			printf("register[%d] filter num = %d\n", n , err);
		#endif
	}

	return err;
}


int8_t CAN_FilterAssign(uint8_t filter_num, CAN_typeIdxMask id, CAN_typeIdxMask mask) {

	if(filter_num >= 14){

		Error_Handler();
		return ERR_INVALID_PARAMS;
	}

	CAN1->FMR |= 1;  //init mode
	id.std.IDE = mask.std.IDE = 1;  //to extendet mode
	CAN1->sFilterRegister[filter_num].FR1 = CAN1->sFilterRegister[filter_num+14].FR1  =  *((uint32_t*)&id);
	CAN1->sFilterRegister[filter_num].FR2 = CAN1->sFilterRegister[filter_num+14].FR2  =  *((uint32_t*)&mask);
	CAN1->FA1R |= (1 << (filter_num + 14)) | (1 << filter_num);
	CAN1->FMR &= ~1;  //active mode

	return 0;
}


int8_t CAN_FilterDeassign(uint8_t filter_num) {

	if(filter_num > 27){
		Error_Handler();
		return ERR_INVALID_PARAMS;
	}
	CAN1->FMR |= 1;  //init mode
	CAN1->FA1R &= ~((1<<(filter_num+14)) | (1<<filter_num));
	CAN1->FMR &= ~1;  //active mode

	return 0;
}


void CAN_RX_Handler(CAN_TypeDef *can_ref) {
	uint16_t i;
	uint16_t pkt_leng, filtr_num, bound;
	CAN_typeIdxMask id;
	uint8_t *pvar;
	int state = 0;
	*((uint32_t*)&id) = can_ref->sFIFOMailBox[0].RIR;
	pkt_leng = can_ref->sFIFOMailBox[0].RDTR & 0x000F;
	filtr_num = (can_ref->sFIFOMailBox[0].RDTR >> 8) & 0x00FF;
	pvar = (uint8_t*)((uint32_t)RegistrationRec[filtr_num].VarPtr);
	bound = pkt_leng;
	if(pvar == (void*)0) {
		can_ref->RF0R = 0x20;  //release FIFO

		return;
	}
	if(pkt_leng > 8){
		
		Error_Handler();
		state = ERR_CAN_DCL_INVALID;
	}
	else {
		if((RegistrationRec[filtr_num].access_flgs & 0x02) == 0) { //offset is needed?
			pvar = pvar + id.uf.Offset;
			bound = bound + id.uf.Offset;
		}
		/* 1-ый Callback */
		if(RegistrationRec[filtr_num].CallBackProc) {
			RegistrationRec[filtr_num].CallBackProc(can_ref, id, pkt_leng, state);
		}
		if(id.std.RTR == 0) {  // обращение по записи
			if(bound <= RegistrationRec[filtr_num].VarLeng) {
				state = 2;
				/*допустимая область*/
				for(i=0; i<pkt_leng; i++){
					pvar[i] = ((uint8_t*)&can_ref->sFIFOMailBox[0].RDLR)[i];
				}
			}
			else {
				/*недопустимая область*/
				state = ERR_CAN_ACCESS_RANGE;
			}
		}
		else {  //RTR == 1 - обращение по чтению
			state = 3;
			id.std.RTR = 0;
			state = CAN_Tx(can_ref, id, pvar, pkt_leng);
			id.std.RTR = 1;  //restore RTR
		}
	}
	can_ref->RF0R = 0x20;  //release FIFO
	/* 2-ой Callback */
	if(RegistrationRec[filtr_num].CallBackProc) {
		if(state == 0){
			state = 1;
		}
		RegistrationRec[filtr_num].CallBackProc(can_ref, id, pkt_leng, state);
	}
}

