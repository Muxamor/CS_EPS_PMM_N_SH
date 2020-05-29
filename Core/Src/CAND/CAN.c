#include "CAN.h"
#include <stdio.h>
#include "canv.h"
#include "flash_lib.h"

//__attribute__((weak)) typeRegistrationRec RegistrationRec[14] = {0};

const CAN_typeRegistrationRec RegistrationRec[] = {
/*{указатель на IVar, размер IVar, указатель на Callback, id_IVar, доступ}*/
	{&CAN_IVar4_RegCmd, sizeof(CAN_IVar4_RegCmd), CAN_ProcCallbackCmds, CAN_ID_IVAR_REGCMD, 0},
	{&CAN_IVar5_telemetry, sizeof(CAN_IVar5_telemetry), CAN_ProcCallbackTelemetry, CAN_ID_VAR_TELEMETRY, 1},  //256k size, read-only, regarless offset
	{&(*((uint32_t*) CAN_ROM_ADDR)), 4, CAN_ProcCallbackTelemetry, CAN_ID_VAR_ROM, 1},
	{&(*((uint32_t*) CAN_RAM_ADDR)), 4, CAN_ProcCallbackTelemetry, CAN_ID_VAR_RAM, 1},
	{&VarCAN_FlashFragment, 0x80008, CallbackCAN_Flash, 14, CAN_AFLG_NOOFFSET}
};


void CAN_ProcCallbackCmds(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state) {
  if(state == 0)

    return;  //первый вызов тут не нужен

  if(state == 1)
	  if(id.std.RTR == 0)
//		  CAN_cmd_mask_status |= (1 << id.uf.Offset);
		  for(uint16_t i = 0; i < leng; i++){
			  CAN_cmd_mask_status = CAN_cmd_mask_status | ( ( (uint64_t)1 ) << (id.uf.Offset + i) );
		  }
}


void CAN_ProcCallbackTelemetry(CAN_TypeDef *can_ref, CAN_typeIdxMask id, uint16_t leng, int state) {
	return;
}



int8_t CAN_RegisterAllVars(){
	uint8_t n;
	int8_t err = 0;
	for(n = 0; n < sizeof(RegistrationRec) / sizeof(CAN_typeRegistrationRec); n++) {
		err = CAN_RegisterVar(n, CAN_ID_DEV);
		#ifdef DEBUGprintf
			printf("CAN_RegisterVar[%d] with err = %d\n", n , err);
		#endif
	}

	return err;
}
