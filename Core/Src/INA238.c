#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "INA238.h"



/** @brief	Read registers iNA238. 16bit register
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
    @param  INA238_addr_reg address registers of INA238:
        INA238_CONFIG_REG_ADDR
        INA238_ADC_CONFIG_REG_ADDR
        INA238_SHUNT_CAL_REG_ADDR
        INA238_VSHUNT_REG_ADDR
        INA238_VBUS_REG_ADDR
        INA238_DIETEMP_REG_ADDR
        INA238_CURRENT_REG_ADDR
        INA238_POWER_REG_ADDR
        INA238_DIAG_ALRT_REG_ADDR
        INA238_SOVL_REG_ADDR
        INA238_SUVL_REG_ADDR
        INA238_BOVL_REG_ADDR
        INA238_BUVL_REG_ADDR
        INA238_TEMP_LIMIT_REG_ADDR
        INA238_PWR_LIMIT_REG_ADDR
        INA238_MANUFACTURER_ID_REG_ADDR
        INA238_DEVICE_ID_REG_ADDR
	@param 	*read_data - pointer to store data from registers.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_read_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t INA238_addr_reg, uint16_t *read_data){

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_addr_reg, read_data) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Write registers INA238.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
    @param  INA238_addr_reg address registers of INA238:
        INA238_CONFIG_REG_ADDR
        INA238_ADC_CONFIG_REG_ADDR
        INA238_SHUNT_CAL_REG_ADDR
        INA238_VSHUNT_REG_ADDR
        INA238_VBUS_REG_ADDR
        INA238_DIETEMP_REG_ADDR
        INA238_CURRENT_REG_ADDR
        INA238_POWER_REG_ADDR
        INA238_DIAG_ALRT_REG_ADDR
        INA238_SOVL_REG_ADDR
        INA238_SUVL_REG_ADDR
        INA238_BOVL_REG_ADDR
        INA238_BUVL_REG_ADDR
        INA238_TEMP_LIMIT_REG_ADDR
        INA238_PWR_LIMIT_REG_ADDR
        INA238_MANUFACTURER_ID_REG_ADDR
        INA238_DEVICE_ID_REG_ADDR
	@param 	write_data - variable that will be write.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_write_reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t INA238_addr_reg, uint16_t write_data){

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_addr_reg, write_data) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}


//************** Configuration (CONFIG) Register (Address = 0h) [reset = 0h]


/** @brief	Hard reset of INA238. Setting this bit to 1 generates a system reset that is the same
  			as a power-on reset; all registers are reset to default values. This bit self-clears.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Hard_reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, 0x8000) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Setup CONVDLY. Sets the Delay for initial ADC conversion in steps of 2 ms.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	conv_delay - variable that will conversion in steps of 2 ms.
        0h = 0 s
        1h = 2 ms
        .
        .
        FFh = 510 ms
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_setup_CONVDLY(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t conv_delay){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    //setup valu CONVDLY
    write_reg = (uint16_t)( (0xC03F & read_reg) | ( ( (uint16_t)conv_delay ) << 6) ); //(0xC03F & read_reg) - reset CONVDLY value

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Get actual value CONVDLY. Delay for initial ADC conversion
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*conv_delay - pointer to store actual averaging mode.
	    Possible values:
		    0h = 0 s
            1h = 2 ms
            .
            .
            FFh = 510 ms
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_get_CONVDLY(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *conv_delay){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *conv_delay = (uint8_t)(read_reg >> 6); //&0xFF

    return SUCCESS;
}


/** @brief	Setup ADCRANGE. Shunt full scale range selection across IN+ and IN–
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	adc_range - variable that will setup full scale range.
        INA238_ADC_RANGE_163mv = ±163.84 mV
        INA238_ADC_RANGE_40mv = ± 40.96 mV
 	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_setup_ADCRANGE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t adc_range){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    //setup valu ADCRANGE
    write_reg = (uint16_t)( (0xFFEF & read_reg) | (adc_range << 4) ); //(0xFFEF & read_reg) - reset ADCRANGE value

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Get actual value ADCRANGE. Shunt full scale range selection across IN+ and IN–
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*adc_range - pointer to store actual full ADC scale range
	    Possible values:
		    0 = INA238_ADC_RANGE_163mv = ±163.84 mV
            1 = INA238_ADC_RANGE_40mv = ± 40.96 mV
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_get_ADCRANGE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *adc_range){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *adc_range = ((uint8_t)(read_reg >> 4)) & 0x01;

    return SUCCESS;
}




//************** ADC Configuration (ADC_CONFIG) Register (Address = 1h) [reset = FB68h]


/** @brief	Setup MODE. The user can set the MODE bits for continuous or triggered mode on
            bus voltage, shunt voltage or temperature measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	mode - variable that will setup mode.
                INA238_BUS_VOLTAGE_TRIGGERED	    	 0x01
                INA238_SHUNT_VOLTAGE_TRIGGERED			 0x02
                INA238_SHUNT_AND_BUS_TRIGGERED			 0x03
                INA238_TEMPERATURE_TRIGGERED			 0x04
                INA238_TEMPERATURE_AND_BUS_TRIGGERED	 0x05
                INA238_TEMPERATURE_AND_SHUNT_TRIGGERED	 0x06
                INA238_BUS_SHUNT_TEMPERATURE_TRIGGERED	 0x07
                INA238_SHUTDOWN						     0x08
                INA238_BUS_VOLTAGE_CONTINUOUS			 0x09
                INA238_SHUNT_VOLTAGE_CONTINUOUS		     0x0A
                INA238_SHUNT_AND_BUS_CONTINUOUS			 0x0B
                INA238_TEMPERATURE_CONTINUOUS			 0x0C
                INA238_BUS_AND_TEMPERATURE_CONTINUOUS	 0x0D
                INA238_TEMPERATURE_AND_SHUNT_CONTINUOUS	 0x0E
                INA238_BUS_SHUNT_TEMPERATURE_CONTINUOUS	 0x0F
 	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_setup_MODE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t mode){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    //setup valu ADCRANGE
    write_reg = (uint16_t)( (0x4095 & read_reg) | (( (uint16_t)mode ) << 12) ); //(0x4095 & read_reg) - reset MODE value

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get actual value MODE. Continuous or triggered mode on
            bus voltage, shunt voltage or temperature measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*mode - pointer to store actual full ADC scale range
	    Possible values:
		    INA238_BUS_VOLTAGE_TRIGGERED	    	 0x01
            INA238_SHUNT_VOLTAGE_TRIGGERED			 0x02
            INA238_SHUNT_AND_BUS_TRIGGERED			 0x03
            INA238_TEMPERATURE_TRIGGERED			 0x04
            INA238_TEMPERATURE_AND_BUS_TRIGGERED	 0x05
            INA238_TEMPERATURE_AND_SHUNT_TRIGGERED	 0x06
            INA238_BUS_SHUNT_TEMPERATURE_TRIGGERED	 0x07
            INA238_SHUTDOWN						     0x08
            INA238_BUS_VOLTAGE_CONTINUOUS			 0x09
            INA238_SHUNT_VOLTAGE_CONTINUOUS		     0x0A
            INA238_SHUNT_AND_BUS_CONTINUOUS			 0x0B
            INA238_TEMPERATURE_CONTINUOUS			 0x0C
            INA238_BUS_AND_TEMPERATURE_CONTINUOUS	 0x0D
            INA238_TEMPERATURE_AND_SHUNT_CONTINUOUS	 0x0E
            INA238_BUS_SHUNT_TEMPERATURE_CONTINUOUS	 0x0F
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_get_MODE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *mode){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *mode = (uint8_t)(read_reg >> 12);

    return SUCCESS;
}