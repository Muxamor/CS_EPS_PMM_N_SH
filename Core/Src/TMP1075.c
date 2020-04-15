
#include "TMP1075.h"

/** @brief	Reading 2 byte register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	reg_id - register's address.
	@param 	SlaveAddr - 7-bit device address.
	@retval 0-OK, -1-Error
*/
int8_t I2C_Read_word(I2C_TypeDef *I2Cx,  uint8_t SlaveAddr, uint8_t reg_id, uint16_t *data){

	uint8_t big, little;
    int8_t error_status = 0;

    SlaveAddr = SlaveAddr << 1;

    while(LL_I2C_IsActiveFlag_BUSY(I2Cx) == SET);

    LL_I2C_HandleTransfer(I2Cx, SlaveAddr,LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    while(LL_I2C_IsActiveFlag_TXE(I2Cx) == RESET);

    LL_I2C_TransmitData8(I2Cx, reg_id);
    while(LL_I2C_IsActiveFlag_TXE(I2Cx) == RESET);
    while(LL_I2C_IsActiveFlag_TC(I2Cx) == RESET);

    LL_I2C_HandleTransfer(I2Cx, SlaveAddr,LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_RESTART_7BIT_READ);

    while(LL_I2C_IsActiveFlag_RXNE(I2Cx) == RESET);
    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    big = LL_I2C_ReceiveData8(I2Cx);

    while(LL_I2C_IsActiveFlag_RXNE(I2Cx) == RESET);
    LL_I2C_AcknowledgeNextData(I2Cx, LL_I2C_ACK);
    little = LL_I2C_ReceiveData8(I2Cx);

    LL_I2C_GenerateStopCondition(I2Cx);
    while(LL_I2C_IsActiveFlag_STOP(I2Cx) == RESET);

    LL_I2C_ClearFlag_STOP(I2Cx);

    *data = (((uint16_t)big) << 8) | ((uint16_t)little);

    return error_status;
}


/** @brief	Writing 2 byte register.
	@param *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param reg_id - register address.
	@param SlaveAddr - 7-bit device address.
	@param data - 2-byte data to write
	@retval	0-OK, -1-Error
*/
int8_t I2C_Write_word(I2C_TypeDef *I2Cx, uint8_t SlaveAddr, uint8_t reg_id, uint16_t data){

	uint8_t data_low = (data & 0xFF);
    uint8_t data_high = data >> 8;
    int8_t error_status = 0;

    SlaveAddr = SlaveAddr << 1;

    while(LL_I2C_IsActiveFlag_BUSY(I2Cx) == SET);
    LL_I2C_HandleTransfer(I2Cx, SlaveAddr,LL_I2C_ADDRSLAVE_7BIT, 3, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    while(LL_I2C_IsActiveFlag_TXE(I2Cx) == RESET);

    LL_I2C_TransmitData8(I2Cx, reg_id);
    while(LL_I2C_IsActiveFlag_TXE(I2Cx) == RESET);

    LL_I2C_TransmitData8(I2Cx, data_high);
    while(LL_I2C_IsActiveFlag_TXE(I2Cx) == RESET);

    LL_I2C_TransmitData8(I2Cx, data_low);
    while(LL_I2C_IsActiveFlag_TXE(I2Cx) == RESET);
    while(LL_I2C_IsActiveFlag_TC(I2Cx) == RESET);

    LL_I2C_GenerateStopCondition(I2Cx);
    while(LL_I2C_IsActiveFlag_STOP(I2Cx) == RESET);

    LL_I2C_ClearFlag_STOP(I2Cx);

    return error_status;
}


/**	@brief	Reading device id.
    @param *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@retval	0-OK, -1-Error
*/
int8_t TMP1075_read_id(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data){

	int8_t err_status = I2C_Read_word(I2Cx, tmp1075_addr, 0x0F, read_data);

	return err_status;
}


/**	@brief	Reading raw data from temperature register.
    @param *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@retval	0-OK, -1-Error
*/
int8_t TMP1075_read_raw_temperature(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data){

	int8_t err_status = I2C_Read_word(I2Cx, tmp1075_addr, 0x00, read_data);

	return err_status;
}


/**	@brief	Converter from raw data (temperature register) to float.
	@param ADC_CODE - raw data from tmp1075 sensor.
	@retval	temperature value in float format
*/
float TMP1075_raw_to_float(uint16_t ADC_CODE){

    int16_t ADC_data_int = 0;
    float ret_val =0;

    ADC_data_int = (int16_t)ADC_CODE;

    ADC_data_int = ADC_data_int>>4;

    ret_val = ((float)ADC_data_int)*0.0625;

    return ret_val;
}


/** @brief  Converter from raw data (temperature register)  to int8_t.
    @param ADC_CODE - raw data from tmp1075 sensor.
    @retval temperature value in int8_t format
*/
int8_t TMP1075_raw_to_int(uint16_t ADC_CODE){

	float temperature = 0;

	temperature = TMP1075_raw_to_float(ADC_CODE);

	return ((int8_t)temperature);
}


/** @brief  Converter from float to raw data temperature register..
    @param 	val_temp - temperature value in float format.
    @retval raw data in tmp1075 sensor format
*/
uint16_t TMP1075_float_to_binary(float val_temp){

	 int16_t ADC_data_int = 0;

	 ADC_data_int = (int16_t)(val_temp/0.0625);

	 ADC_data_int = ADC_data_int<<4;

	return ((uint16_t)ADC_data_int);
}


/** @brief	Reading configuration register tmp1075 sensor.
    @param *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - 7-bit device address.
	@retval	0-OK, -1-Error
*/
int8_t TMP1075_read_config(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data){

	int8_t err_status = I2C_Read_word(I2Cx, tmp1075_addr, 0x01, read_data);

	return err_status;
}


/** @brief  Sets the device in SHUTDOWN_MODE to conserve power or in CONTINUOS_CONVERSION.
    @param *I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@param mode - SHUTDOWN_MODE or CONTINUOS_CONVERSION mode of TMP1075 sensor.
	@retval 0-OK, -1-Error
*/
int8_t TMP1075_set_mode(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode){

	uint16_t last_state;
	uint16_t current_state;

	TMP1075_read_config(I2Cx, tmp1075_addr, &last_state);
    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x01, (last_state & (~(1 << 8))) | (mode << 8) ) == -1 ){
        return -1;
    }

    TMP1075_read_config(I2Cx, tmp1075_addr, &current_state);
    if((current_state & (~(1 << 8)))  | (mode << 8)){

    	return 0;

    }else{

        return -1;
    }

}


/** @brief	Conversion rate setting when device is in continuos conversion mode
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - 7-bit device address.
	@param  time - conversion rate:
			     TMP1075_CR_VERY_FAST        // 27.5 ms
			     TMP1075_CR_FAST             // 55 ms
			     TMP1075_CR_MEDIUM           // 110 ms
			     TMP1075_CR_SLOW             // 220 ms
	@retval 0-OK, -1-Error
*/
int8_t TMP1075_set_time_conversion(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t time){

	uint16_t last_state;
    uint16_t current_state;

    TMP1075_read_config(I2Cx, tmp1075_addr, &last_state);
    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x01, (last_state & (~(3 << 13))) | (time << 13)) == -1 ){
        return -1;
    }

	TMP1075_read_config(I2Cx, tmp1075_addr, &current_state);
    if(((last_state & (~(3 << 13))) | (time << 13)) != current_state){

    	return -1;
    }

    return 0;
}


/** @brief	One-shot conversion mode (for SHUTDOWN_MODE). Requests single measurement of temperature.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - 7-bit device address.
	@retval	0-OK, 1-Error
*/
int8_t TMP1075_one_shot_conversion_start(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

	uint16_t last_state;
	uint16_t current_state;

    TMP1075_read_config(I2Cx, tmp1075_addr, &last_state);
    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x01, (last_state & (~(1 << 15))) | (1 << 15)) == -1 ){
        return -1;
    }

	TMP1075_read_config(I2Cx, tmp1075_addr, &current_state);
    if(((last_state & (~(1 << 15))) | (1 << 15)) != current_state){

    	return -1;
    }

    return 0;
}


/** @brief  Disable ALERT pin function.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param  tmp1075_addr - 7-bit device address.
    @retval 0-OK, 1-Error
*/
int8_t TMP1075_disable_ALERT_pin(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr){

    if( TMP1075_set_low_limit( I2Cx, tmp1075_addr, -128) == -1){
        return -1;
    }

    if( TMP1075_set_high_limit(I2Cx, tmp1075_addr, 127.9375) == -1){

        return -1;
    }

    return 0;
}


/** @brief	Selects the mode of the ALERT pin.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param  tmp1075_addr - 7-bit device address.
	@param  mode - ALERT mode:
			     TMP1075_COMPARATOR_MODE
			     TMP1075_INTERRUPT_MODE
	@retval	0-OK, 1-Error
*/
int8_t TMP1075_set_mode_ALERT_pin(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode){

	uint16_t last_state;
	uint16_t current_state;

    TMP1075_read_config(I2Cx, tmp1075_addr, &last_state);
    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x01, (last_state & (~(1 << 9))) | (mode << 9)) == -1 ){
        return -1;
    }

    TMP1075_read_config(I2Cx, tmp1075_addr, &current_state);
    if(((last_state & (~(1 << 9))) | (mode << 9)) != current_state){

    	return -1;
    }

    return 0;
}


/** @brief	Polarity of the output ALERT pin.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@param mode - ALERT polarity.
			     TMP1075_ACTIVE_LOW
			     TMP1075_ACTIVE_HIGH
	@retval	0-OK, 1-Error
*/
int8_t TMP1075_ALERT_active_level(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode){

	uint16_t last_state;
	uint16_t current_state;

    TMP1075_read_config(I2Cx, tmp1075_addr, &last_state);
    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x01, (last_state & (~(1 << 10))) | (mode << 10)) == -1 ){
        return -1;
    }

    TMP1075_read_config(I2Cx, tmp1075_addr, &current_state);
    if(((last_state & (~(1 << 10))) | (mode << 10)) != current_state){

    	return -1;
    }

    return 0;
}


/** @brief	Consecutive fault measurements to trigger the alert function.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@param mode - amount of faults:
			     TMP1075_ONE_FAULT   
			     TMP1075_TWO_FAULT   
			     TMP1075_THREE_FAULT 
			     TMP1075_FOUR_FAULT  
	@retval	0-OK, 1-Error
*/
int8_t TMP1075_ALERT_sensitivity(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint8_t mode){

	uint16_t last_state;
	uint16_t current_state;

    TMP1075_read_config(I2Cx, tmp1075_addr, &last_state);
    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x01, (last_state & (~(3 << 11))) | (mode << 11)) == -1 ) {
        return -1;
    }

    TMP1075_read_config(I2Cx, tmp1075_addr, &current_state);
    if(((last_state & (~(3 << 11))) | (mode << 11)) != current_state){

    	return -1;
    }

    return 0;
   
}


/** @brief	Set low limit for comparison with temperature results.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@param low_limit -  low limit for comparison with temperature results.
	@retval	0-OK, -1-Error
*/
int8_t TMP1075_set_low_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, float low_limit){

	uint16_t low_limit_temp=0;
    uint16_t get_reg_data=0;

    low_limit_temp = TMP1075_float_to_binary(low_limit);

    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x02, low_limit_temp) == -1 ){
        return -1;
    };
    
    TMP1075_get_low_limit(I2Cx, tmp1075_addr, &get_reg_data);
    
    if( low_limit_temp != get_reg_data ){

        return -1;
    }

    return 0;
}


/** @brief  Set high limit for comparison with temperature results
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@param high_limit - high limit for comparison with temperature results.
	@retval	0-OK, -1-Error
*/
int8_t TMP1075_set_high_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, float high_limit){

    uint16_t high_limit_temp=0;
    uint16_t get_reg_data=0;

    high_limit_temp = TMP1075_float_to_binary(high_limit);

    if( I2C_Write_word(I2Cx, tmp1075_addr, 0x03, high_limit_temp) == -1){

        return -1;
    }

    TMP1075_get_high_limit(I2Cx, tmp1075_addr, &get_reg_data);
    
    if( high_limit_temp != get_reg_data ){

        return -1;
    }
    
    return 0;
}


/** @brief  Reading current value of low limit register.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
    @param tmp1075_addr - 7-bit device address.
    @retval 0-OK, -1-Error
*/
int8_t TMP1075_get_low_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data){

    int8_t err_status =  I2C_Read_word(I2Cx, tmp1075_addr,  0x02, read_data);

    return err_status;
}


/** @brief	Reading current value of high limit register.
    @param  I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param tmp1075_addr - 7-bit device address.
	@retval	0-OK, -1-Error
*/
int8_t TMP1075_get_high_limit(I2C_TypeDef *I2Cx, uint8_t tmp1075_addr, uint16_t *read_data){

	int8_t err_status = I2C_Read_word(I2Cx, tmp1075_addr, 0x03, read_data);

	return err_status;
}










