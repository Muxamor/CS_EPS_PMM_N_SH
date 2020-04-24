#include "CAN.h"
#include <stdio.h>

//__attribute__((weak)) typeRegistrationRec RegistrationRec[14] = {0};

const CAN_typeRegistrationRec RegistrationRec[] = {
/*{указатель на IVar, размер IVar, указатель на Callback, id_IVar, доступ}*/
	{&CAN_IVar_Cmds, sizeof(CAN_IVar_Cmds), CAN_ProcCallbackCmds, CAN_ID_IVAR_CMDS, 0},
	{&CAN_telemetry, sizeof(CAN_telemetry), CAN_ProcCallbackTelemetry, CAN_ID_VAR_TELEMETRY, 1},  //256k size, read-only, regarless offset
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


void CAN_check_cmd_status(uint32_t *cmd_status){

	for(uint8_t i = 0; i < 24; i++){
		if((*cmd_status >> i) & 0x01){
			switch (i) {
				case 21:
					if(CAN_IVar_Cmds.CAN_Constant_mode == 1){
						#ifdef DEBUGprintf
							printf("fill telemetry struct by constants\n");
							printf("size of tel-ry %d\n", sizeof(CAN_telemetry));
						#endif
						CAN_fill_telemetry_by_constants(&CAN_telemetry);
						*cmd_status &= ~(1 << 21);
					}
					else if(CAN_IVar_Cmds.CAN_Constant_mode == 0){
						#ifdef DEBUGprintf
							printf("fill telemetry struct by zero\n");
						#endif
						for(uint16_t i = 0; i < sizeof(CAN_telemetry); i++){
							*((uint8_t *)(&CAN_telemetry) + i) = 0;
						}
						*cmd_status &= ~(1 << 21);
					}
					break;
				default:
					break;
			}
			if((*cmd_status >> i) == 0){
				break;
			}
		}
	}
}

int8_t CAN_RegisterVar(int n, uint8_t dev_id) {
	CAN_typeIdxMask id, mask;
	uint8_t access_flgs;
	id.uf.DevId = dev_id;
	id.uf.VarId = RegistrationRec[n].ivar_id;
	access_flgs = RegistrationRec[n].access_flgs;
	if(n >= 14){
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
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
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
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
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
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
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
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
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
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
		#ifdef DEBUGprintf
			Error_Handler();
		#endif
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




void CAN_fill_telemetry_by_constants(CAN_IVar_telemetry *telemetry_constants){
    // -------------------  ТМИ 0  ------------------ //
    telemetry_constants->CAN_Beacon_panel_median_temperature_pX		     =  0x74;
    telemetry_constants->CAN_Beacon_panel_median_temperature_nX		     =  0x75;
    telemetry_constants->CAN_Beacon_solar_panel_status[0]				 =  0x7A;
    telemetry_constants->CAN_Beacon_solar_panel_status[1]				 =  0x7B;
    telemetry_constants->CAN_Beacon_solar_panel_status[2]				 =  0xC7;
    telemetry_constants->CAN_Beacon_solar_panel_status[3]				 =  0x7D;
    telemetry_constants->CAN_Beacon_solar_panel_status[4]				 =  0x7E;
    telemetry_constants->CAN_Beacon_full_capacity_charge_level_AB3_line2 =  0x8C8D;
    telemetry_constants->CAN_Beacon_AB_status[0]						 =  0x97;
    telemetry_constants->CAN_Beacon_AB_status[1]						 =  0x98;
    telemetry_constants->CAN_Beacon_AB_status[2]						 =  0x99;
    telemetry_constants->CAN_Beacon_charge_discharge_AB_key_status       =  0x9A9B;
    telemetry_constants->CAN_Beacon_subsystem_power_line_status		     =  0xB0;
    telemetry_constants->CAN_Beacon_full_charge_discharge_power		     =  0xB5B6;
    telemetry_constants->CAN_Beacon_total_power_SB                       =  0xB7B8;
    telemetry_constants->CAN_Beacon_spacecraft_total_power               =  0xB9BA;
    telemetry_constants->CAN_Beacon_median_PMM_temp					     =  0xBB;
    telemetry_constants->CAN_Beacon_median_PAM_temp					     =  0xBC;
    telemetry_constants->CAN_Beacon_median_PDM_temp					     =  0xBD;
    telemetry_constants->CAN_Beacon_SES_module_system_elements_status[0] =  0xBE;
    telemetry_constants->CAN_Beacon_SES_module_system_elements_status[1] =  0xBF;
    telemetry_constants->CAN_Beacon_SES_module_system_elements_status[2] =  0xC0;
    telemetry_constants->CAN_Beacon_SES_module_system_elements_status[3] =  0xC1;
    telemetry_constants->CAN_Beacon_SES_module_system_elements_status[4] =  0xC2;
    // -------------------  ТМИ 4  ------------------ //
    telemetry_constants->CAN_Panal_current_pX                           =	0x5C5D;
    telemetry_constants->CAN_Panal_current_nX                           =	0x5E5F;
    telemetry_constants->CAN_Panal_current_pY                           =	0x6061;
    telemetry_constants->CAN_Panal_current_nY                           =	0x6263;
    telemetry_constants->CAN_Current_hinged_panels_satellite_1_plane    =   0x6465;
    telemetry_constants->CAN_Current_hinged_panels_satellite_2_plane    =   0x6667;


    telemetry_constants->CAN_Panel_voltage_pX                           =	0x6869;
    telemetry_constants->CAN_Panel_voltage_nX                           =	0x6A6B;
    telemetry_constants->CAN_Panel_voltage_pY                           =	0x6C6D;
    telemetry_constants->CAN_Panel_voltage_nY                           =	0x6E6F;
    telemetry_constants->CAN_Voltage_hinged_panels_satellite_1_plane    =   0x7071;
    telemetry_constants->CAN_Voltage_hinged_panels_satellite_2_plane    =   0x7273;


    telemetry_constants->CAN_Panel_median_temperature_pX                 =  0x74;
    telemetry_constants->CAN_Panel_median_temperature_nX                 =  0x75;
    telemetry_constants->CAN_Panel_median_temperature_pY                 =  0x76;
    telemetry_constants->CAN_Panel_median_temperature_nY                 =  0x77;
    telemetry_constants->CAN_Hinged_panel_median_temperature_pY          =  0x78;
    telemetry_constants->CAN_Hinged_panel_median_temperature_nY          =  0x79;
    telemetry_constants->CAN_Solar_panel_status[0]                       =  0x7A;
    telemetry_constants->CAN_Solar_panel_status[1]                       =  0x7B;
    telemetry_constants->CAN_Solar_panel_status[2]                       =  0x7C;
    telemetry_constants->CAN_Solar_panel_status[3]                       =  0x7D;
    telemetry_constants->CAN_Solar_panel_status[4]                       =  0x7E;


    telemetry_constants->CAN_Full_percent_charge_level_AB3_line2         =  0x7F;


    telemetry_constants->CAN_Capacity_charge_level_AB1_line1             =  0x8081;
    telemetry_constants->CAN_Capacity_charge_level_AB1_line2             =  0x8283;
    telemetry_constants->CAN_Capacity_charge_level_AB2_line1             =  0x8485;
    telemetry_constants->CAN_Capacity_charge_level_AB2_line2             =  0x8687;
    telemetry_constants->CAN_Capacity_charge_level_AB3_line1             =  0x8889;
    telemetry_constants->CAN_Capacity_charge_level_AB3_line2             =  0x8A8B;
    telemetry_constants->CAN_Full_capacity_charge_level_AB3_line2        =  0x8C8D;


    telemetry_constants->CAN_Average_temp_controller_AB1                 =  0x8E;
    telemetry_constants->CAN_Average_line1_temp_AB1                      =  0x8F;
    telemetry_constants->CAN_Average_line2_temp_AB1                      =  0x90;
    telemetry_constants->CAN_Average_temp_controller_AB2                 =  0x91;
    telemetry_constants->CAN_Average_line1_temp_AB2                      =  0x92;
    telemetry_constants->CAN_Average_line2_temp_AB2                      =  0x93;
    telemetry_constants->CAN_Average_temp_controller_AB3                 =  0x94;
    telemetry_constants->CAN_Average_line1_temp_AB3                      =  0x95;
    telemetry_constants->CAN_Average_line2_temp_AB3                      =  0x96;
    telemetry_constants->CAN_AB_status[0]                                =  0x97;
    telemetry_constants->CAN_AB_status[1]                                =  0x98;
    telemetry_constants->CAN_AB_status[2]                                =  0x99;

    telemetry_constants->CAN_Charge_discharge_AB_key_status              =  0x9A9B;

    telemetry_constants->CAN_Channel1_current                            =  0x9C9D;
    telemetry_constants->CAN_Channel2_current                            =  0x9E9F;
    telemetry_constants->CAN_Channel3_current                            =  0xA0A1;
    telemetry_constants->CAN_Channel4_current                            =  0xA2A3;
    telemetry_constants->CAN_Channel5_current                            =  0xA4A5;
    telemetry_constants->CAN_Channel6_current                            =  0xA6A7;

    telemetry_constants->CAN_VBAT1_voltage                               =  0xA8A9;
    telemetry_constants->CAN_VBAT2_voltage                               =  0xAAAB;

    telemetry_constants->CAN_VBAT1_current                               =  0xACAD;
    telemetry_constants->CAN_VBAT2_current                               =  0xAEAF;

    telemetry_constants->CAN_Subsystem_power_line_status                 =  0xB0;
    telemetry_constants->CAN_SES_current_consumption                     =  0xB1B2;
    telemetry_constants->CAN_SES_power_supply                            =  0xB3B4;
    telemetry_constants->CAN_Full_charge_discharge_power                 =  0xB5B6;
    telemetry_constants->CAN_Total_power_SB                              =  0xB7B8;
    telemetry_constants->CAN_Spacecraft_total_power                      =  0xB9BA;

    telemetry_constants->CAN_Median_PMM_temp                             =  0xBB;
    telemetry_constants->CAN_Median_PAM_temp                             =  0xBC;
    telemetry_constants->CAN_Median_PDM_temp                             =  0xBD;
    telemetry_constants->CAN_SES_module_system_elements_status[0]        =  0xBE;
    telemetry_constants->CAN_SES_module_system_elements_status[1]        =  0xBF;
    telemetry_constants->CAN_SES_module_system_elements_status[2]        =  0xC0;
    telemetry_constants->CAN_SES_module_system_elements_status[3]        =  0xC1;
    telemetry_constants->CAN_SES_module_system_elements_status[4]        =  0xC2;
    telemetry_constants->CAN_Number_of_restarts_of_the_SES_module        =  0xC3C4C5C6;
    telemetry_constants->CAN_Number_of_restarts_of_the_reserve_SES_module=  0xC7C8C9CA;
    telemetry_constants->CAN_SES_module_data_array1                  	 =  0xCBCCCDCE;
    telemetry_constants->CAN_Primary_standby_switch                      =  0xCF;
    // -------------------  ТМИ 7  ------------------ //
    telemetry_constants->CAN_Charge_discharge_current_AB1_line1          =  0xB8B9;
    telemetry_constants->CAN_Charge_discharge_current_AB1_line2          =  0xBABB;
    telemetry_constants->CAN_Charge_discharge_current_AB2_line1          =  0xBCBD;
    telemetry_constants->CAN_Charge_discharge_current_AB2_line2          =  0xBEBF;
    telemetry_constants->CAN_Charge_discharge_current_AB3_line1          =  0xC0C1;
    telemetry_constants->CAN_Charge_discharge_current_AB3_line2          =  0xC2C3;

    telemetry_constants->CAN_Voltage_AB1_line1_battery_1                 =	0xC4C5;
    telemetry_constants->CAN_Voltage_AB1_line1_battery_2                 =	0xC6C7;
    telemetry_constants->CAN_Voltage_AB1_line2_battery_1                 =	0xC8C9;
    telemetry_constants->CAN_Voltage_AB1_line2_battery_2                 =	0xCACB;
    telemetry_constants->CAN_Voltage_AB2_line1_battery_1                 =	0xCCCD;
    telemetry_constants->CAN_Voltage_AB2_line1_battery_2                 =	0xCECF;
    telemetry_constants->CAN_Voltage_AB2_line2_battery_1                 =	0xD0D1;
    telemetry_constants->CAN_Voltage_AB2_line2_battery_2                 =	0xD2D3;
    telemetry_constants->CAN_Voltage_AB3_line1_battery_1                 =	0xD4D5;
    telemetry_constants->CAN_Voltage_AB3_line1_battery_2                 =	0xD6D7;
    telemetry_constants->CAN_Voltage_AB3_line2_battery_1                 =	0xD8D9;
    telemetry_constants->CAN_Voltage_AB3_line2_battery_2                 =	0xDADB;

    telemetry_constants->CAN_Percent_charge_level_AB1_line1              =	0xDC;
    telemetry_constants->CAN_Percent_charge_level_AB1_line2              =	0xDD;
    telemetry_constants->CAN_Percent_charge_level_AB2_line1              =	0xDE;
    telemetry_constants->CAN_Percent_charge_level_AB2_line2              =	0xDF;
    telemetry_constants->CAN_Percent_charge_level_AB3_line1              =	0xE0;
    telemetry_constants->CAN_Percent_charge_level_AB3_line2              =	0xE1;

    telemetry_constants->CAN_Channel1_voltage                            =	0xE2E3;
    telemetry_constants->CAN_Channel2_voltage                            =	0xE4E5;
    telemetry_constants->CAN_Channel3_voltage                            =	0xE6E7;
    telemetry_constants->CAN_Channel4_voltage                            =	0xE8E9;
    telemetry_constants->CAN_Channel5_voltage                            =	0xEAEB;
    telemetry_constants->CAN_Channel6_voltage                            =	0xECED;

    telemetry_constants->CAN_Opening_elements_state                      =	0xEEEF;
    telemetry_constants->CAN_SES_module_data_array2[0]                   =	0xF0;
    telemetry_constants->CAN_SES_module_data_array2[1]                   =  0xF1;
    telemetry_constants->CAN_SES_module_data_array2[2]                   =  0xF2;
    telemetry_constants->CAN_SES_module_data_array2[3]                   =  0xF3;
    telemetry_constants->CAN_SES_module_data_array2[4]                   =  0xF4;
    telemetry_constants->CAN_SES_module_data_array2[5]                   =  0xF5;
    telemetry_constants->CAN_SES_module_data_array2[6]                   =  0xF6;
    telemetry_constants->CAN_SES_module_data_array2[7]                   =  0xF7;
    telemetry_constants->CAN_SES_module_data_array2[8]                   =  0xF8;
    telemetry_constants->CAN_SES_module_data_array2[9]                   =  0xF9;
    telemetry_constants->CAN_SES_module_data_array2[10]                  =  0xFA;
    telemetry_constants->CAN_SES_module_data_array2[11]                  =  0xFB;
    telemetry_constants->CAN_SES_module_data_array2[12]                  =  0xFC;
    telemetry_constants->CAN_SES_module_data_array2[13]                  =  0xFD;
    telemetry_constants->CAN_SES_module_data_array2[14]                  =  0xFE;
    telemetry_constants->CAN_SES_module_data_array2[15]                  =  0xFF;
    telemetry_constants->CAN_SES_module_data_array2[16]                  =  0x00;
    telemetry_constants->CAN_SES_module_data_array2[17]                  =  0x01;
    telemetry_constants->CAN_SES_module_data_array2[18]                  =  0x02;
    telemetry_constants->CAN_SES_module_data_array2[19]                  =  0x03;
    telemetry_constants->CAN_SES_module_data_array2[20]                  =  0x04;
    telemetry_constants->CAN_SES_module_data_array2[21]                  =  0x05;
    telemetry_constants->CAN_SES_module_data_array2[22]                  =  0x06;
    telemetry_constants->CAN_SES_module_data_array2[23]                  =  0x07;
    telemetry_constants->CAN_SES_module_data_array2[24]                  =  0x08;
    telemetry_constants->CAN_SES_module_data_array2[25]                  =  0x09;
    telemetry_constants->CAN_SES_module_data_array2[26]                  =  0x0A;
    telemetry_constants->CAN_SES_module_data_array2[27]                  =  0x0B;
    telemetry_constants->CAN_SES_module_data_array2[28]                  =  0x0C;
    telemetry_constants->CAN_SES_module_data_array2[29]                  =  0x0D;
    telemetry_constants->CAN_SES_module_data_array2[30]                  =  0x0E;
    telemetry_constants->CAN_SES_module_data_array2[31]                  =  0x0F;
    telemetry_constants->CAN_SES_module_data_array2[32]                  =  0x10;
    telemetry_constants->CAN_SES_module_data_array2[33]                  =  0x11;
    telemetry_constants->CAN_SES_module_data_array2[34]                  =  0x12;
    telemetry_constants->CAN_SES_module_data_array2[35]                  =  0x13;
    telemetry_constants->CAN_SES_module_data_array2[36]                  =  0x14;
    telemetry_constants->CAN_SES_module_data_array2[37]                  =  0x15;
    telemetry_constants->CAN_SES_module_data_array2[38]                  =  0x16;
    telemetry_constants->CAN_SES_module_data_array2[39]                  =  0x17;
    telemetry_constants->CAN_SES_module_data_array2[40]                  =  0x18;
    telemetry_constants->CAN_SES_module_data_array2[41]                  =  0x19;
    telemetry_constants->CAN_SES_module_data_array2[42]                  =  0x1A;
    telemetry_constants->CAN_SES_module_data_array2[43]                  =  0x1B;
    telemetry_constants->CAN_SES_module_data_array2[44]                  =  0x1C;
    telemetry_constants->CAN_SES_module_data_array2[45]                  =  0x1D;
    telemetry_constants->CAN_SES_module_data_array2[46]                  =  0x1E;
    telemetry_constants->CAN_SES_module_data_array2[47]                  =  0x1F;
    telemetry_constants->CAN_SES_module_data_array2[48]                  =  0x20;
    telemetry_constants->CAN_SES_module_data_array2[49]                  =  0x21;
    telemetry_constants->CAN_SES_module_data_array2[50]                  =  0x22;
    telemetry_constants->CAN_SES_module_data_array2[51]                  =  0x23;
    telemetry_constants->CAN_SES_module_data_array2[52]                  =  0x24;
    telemetry_constants->CAN_SES_module_data_array2[53]                  =  0x25;
    telemetry_constants->CAN_SES_module_data_array2[54]                  =  0x26;
    telemetry_constants->CAN_SES_module_data_array2[55]                  =  0x27;
    telemetry_constants->CAN_SES_module_data_array2[56]                  =  0x28;

    telemetry_constants->CAN_Reset_to_default                            =  0x29;
    telemetry_constants->CAN_Reset_counter_to_default_state              =  0x2A;
    telemetry_constants->CAN_Perform_disclosure                          =  0x2B;
    // ----------------------- ТМИ 8 -------------------
    telemetry_constants->CAN_Panal_temp_pX_sensor_1                      =	0x2C;
    telemetry_constants->CAN_Panal_temp_pX_sensor_2                      =	0x2D;
    telemetry_constants->CAN_Panal_temp_pX_sensor_3                      =	0x2E;
    telemetry_constants->CAN_Panal_temp_pX_sensor_4                      =	0x2F;
    telemetry_constants->CAN_Panal_temp_nX_sensor_1                      =	0x30;
    telemetry_constants->CAN_Panal_temp_nX_sensor_2                      =	0x31;
    telemetry_constants->CAN_Panal_temp_nX_sensor_3                      =	0x32;
    telemetry_constants->CAN_Panal_temp_nX_sensor_4                      =	0x33;
    telemetry_constants->CAN_Panal_temp_pY_sensor_1                      =	0x34;
    telemetry_constants->CAN_Panal_temp_pY_sensor_2                      =	0x35;
    telemetry_constants->CAN_Panal_temp_nY_sensor_1                      =	0x36;
    telemetry_constants->CAN_Panal_temp_nY_sensor_2                      =	0x37;
    telemetry_constants->CAN_Hinged_panel_temp_pY_sensor_1               =  0x38;
    telemetry_constants->CAN_Hinged_panel_temp_pY_sensor_2               =  0x39;
    telemetry_constants->CAN_Hinged_panel_temp_pY_sensor_3               =  0x3A;
    telemetry_constants->CAN_Hinged_panel_temp_pY_sensor_4               =  0x3B;
    telemetry_constants->CAN_Hinged_panel_temp_nY_sensor_1               =  0x3C;
    telemetry_constants->CAN_Hinged_panel_temp_nY_sensor_2               =  0x3D;
    telemetry_constants->CAN_Hinged_panel_temp_nY_sensor_3               =  0x3E;
    telemetry_constants->CAN_Hinged_panel_temp_nY_sensor_4               =  0x3F;

    telemetry_constants->CAN_AB1_controller_1_sensor                     =	0x40;
    telemetry_constants->CAN_AB1_controller_2_sensor                     =	0x41;
    telemetry_constants->CAN_AB1_sensor_on_board1                        =	0x42;
    telemetry_constants->CAN_AB1_sensor_on_board2                        =	0x43;
    telemetry_constants->CAN_AB1_sensor_on_board3                        =	0x44;
    telemetry_constants->CAN_AB1_sensor_on_board4                        =	0x45;
    telemetry_constants->CAN_AB2_controller_1_sensor                     =	0x46;
    telemetry_constants->CAN_AB2_controller_2_sensor                     =	0x47;
    telemetry_constants->CAN_AB2_sensor_on_board1                        =	0x48;
    telemetry_constants->CAN_AB2_sensor_on_board2                        =	0x49;
    telemetry_constants->CAN_AB2_sensor_on_board3                        =	0x4A;
    telemetry_constants->CAN_AB2_sensor_on_board4                        =	0x4B;
    telemetry_constants->CAN_AB3_controller_1_sensor                     =	0x4C;
    telemetry_constants->CAN_AB3_controller_2_sensor                     =	0x4D;
    telemetry_constants->CAN_AB3_sensor_on_board1                        =	0x4E;
    telemetry_constants->CAN_AB3_sensor_on_board2                        =	0x4F;
    telemetry_constants->CAN_AB3_sensor_on_board3                        =	0x50;
    telemetry_constants->CAN_AB3_sensor_on_board4                        =	0x51;


    telemetry_constants->CAN_Charge_gate_1_AB1                           =  0x52;
    telemetry_constants->CAN_Discharge_gate_1_AB1                        =  0x53;
    telemetry_constants->CAN_Charge_gate_2_AB1                           =  0x54;
    telemetry_constants->CAN_Discharge_gate_2_AB1                        =  0x55;
    telemetry_constants->CAN_Charge_gate_1_AB2                           =  0x56;
    telemetry_constants->CAN_Discharge_gate_1_AB2                        =  0x57;
    telemetry_constants->CAN_Charge_gate_2_AB2                           =  0x58;
    telemetry_constants->CAN_Discharge_gate_2_AB2                        =  0x59;
    telemetry_constants->CAN_Charge_gate_1_AB3                           =  0x5A;
    telemetry_constants->CAN_Discharge_gate_1_AB3                        =  0x5B;
    telemetry_constants->CAN_Charge_gate_2_AB3                           =  0x5C;
    telemetry_constants->CAN_Discharge_gate_2_AB3                        =  0x5D;


    telemetry_constants->CAN_Line1                                       =	0x5E;
    telemetry_constants->CAN_Line2                                       =	0x5F;
    telemetry_constants->CAN_Line3                                       =	0x60;
    telemetry_constants->CAN_Line4                                       =	0x61;
    telemetry_constants->CAN_Line5                                       =	0x62;
    telemetry_constants->CAN_Line6                                       =	0x63;
    telemetry_constants->CAN_Line_VBAT1                                  =	0x64;
    telemetry_constants->CAN_Line_VBAT2                                  =	0x65;

    telemetry_constants->CAN_SES_internal_bus_voltage                    =	0x6667;


    telemetry_constants->CAN_PMM_sensor1                                 =	0x68;
    telemetry_constants->CAN_PAM_sensor1                                 =	0x69;
    telemetry_constants->CAN_PAM_sensor2                                 =	0x6A;
    telemetry_constants->CAN_PAM_sensor3                                 =	0x6B;
    telemetry_constants->CAN_PAM_sensor4                                 =	0x6C;
    telemetry_constants->CAN_PDM_sensor1                                 =	0x6D;
    telemetry_constants->CAN_PDM_sensor2                                 =	0x6E;
    telemetry_constants->CAN_PDM_sensor3                                 =	0x6F;
    telemetry_constants->CAN_PDM_sensor4                                 =	0x70;
    for(uint8_t i = 0, j = 0x71; i < 47; i++, j++){
        telemetry_constants->CAN_data_array3[i] = j;
    }
}



