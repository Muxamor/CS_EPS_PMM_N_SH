


/*********************** TODO *********************/
/**
 * 3. Подумать над тем чтобы функцию PDM_Get_temp_all_sensor убрать в функцию PDM_Get_telemetry.
 *
 **************************************************/


/** @brief  Get Power Good power channel status
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*//*
ErrorStatus PDM_Get_PG_ALL_PWR_CH( _PDM *pdm_ptr ){

}

*/


/** @brief  Get temperature from temp. sensor on PDM .
	@param  *pdm_ptr - pointer to struct which contain all information about PDM.
	@retval 0 - SUCCESS, -1 - ERROR_N.
*/
//ErrorStatus PDM_Get_temp_all_sensor( _PDM *pdm_ptr ){
//
//	//SW_TMUX1209_I2C_main_PDM(); // Switch MUX to PDM I2C bus on PMM
//	int8_t error_status;
//
//	error_status = PDM_Get_temp_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_1, PDM_I2CADDR_TMP1075_1 );
//	error_status += PDM_Get_temp_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_2, PDM_I2CADDR_TMP1075_2 );
//	error_status += PDM_Get_temp_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_3, PDM_I2CADDR_TMP1075_3 );
//	error_status += PDM_Get_temp_TMP1075( pdm_ptr, PDM_I2Cx_TMP1075_4, PDM_I2CADDR_TMP1075_4 );
//
//	if(error_status != SUCCESS ){
//		return ERROR_N;
//	}
//
//	return SUCCESS;
//}
