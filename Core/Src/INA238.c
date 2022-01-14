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
        INA238_SOVL_Reg_ADDR
        INA238_SUVL_Reg_ADDR
        INA238_BOVL_Reg_ADDR
        INA238_BUVL_Reg_ADDR
        INA238_TEMP_LIMIT_Reg_ADDR
        INA238_PWR_LIMIT_Reg_ADDR
        INA238_MANUFACTURER_ID_Reg_ADDR
        INA238_DEVICE_ID_Reg_ADDR
	@param 	*Read_data - pointer to store data from Registers.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Read_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t INA238_addr_reg, uint16_t *read_data){

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_addr_reg, read_data) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Write Registers INA238.
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
ErrorStatus INA238_Write_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t INA238_addr_reg, uint16_t write_data){

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_addr_reg, write_data) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}


//************** Configuration (CONFIG) Register (Address = 0h) [reset = 0h]


/** @brief	Hard reset of INA238. Setting this bit to 1 generates a system reset that is the same
  			as a power-on reset; all Registers are reset to default values. This bit self-clears.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Hard_Reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

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
ErrorStatus INA238_Setup_CONVDLY(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t conv_delay){

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
ErrorStatus INA238_Get_CONVDLY(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *conv_delay){

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
ErrorStatus INA238_Setup_ADCRANGE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t adc_range){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if( adc_range == INA238_ADC_RANGE_163mv || adc_range == INA238_ADC_RANGE_40mv ){

        if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
            return ERROR_N;
        }

        //setup valu ADCRANGE
        write_reg = (uint16_t)( (0xFFEF & read_reg) | (adc_range << 4) ); //(0xFFEF & read_reg) - reset ADCRANGE value

        if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_CONFIG_REG_ADDR, write_reg) != SUCCESS){
            return ERROR_N;
        }

    }else{
        return ERROR_N;
    };

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
ErrorStatus INA238_Get_ADCRANGE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *adc_range){

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
ErrorStatus INA238_Setup_MODE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t mode){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if( mode <= 0x0F ){
        if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
            return ERROR_N;
        }

        //setup valu ADCRANGE
        write_reg = (uint16_t)( (0x0FFF & read_reg) | (( (uint16_t)mode ) << 12) ); //(0x0FFF & read_reg) - reset MODE value

        if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, write_reg) != SUCCESS){
            return ERROR_N;
        }
    }else{
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
	        INA238_SHUTDOWN						     0x00
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
ErrorStatus INA238_Get_MODE(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *mode){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *mode = ((uint8_t)(read_reg >> 12)) & 0x0F;

    return SUCCESS;
}


/** @brief	Setup VBUSCT. Sets the conversion time of the Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	vbusct - variable that will setup VBUSCT.
        INA238_CONVERSION_TIME_50us		0x00
        INA238_CONVERSION_TIME_84us		0x01
        INA238_CONVERSION_TIME_150us	0x02
        INA238_CONVERSION_TIME_280us	0x03
        INA238_CONVERSION_TIME_540us	0x04
        INA238_CONVERSION_TIME_1052us	0x05
        INA238_CONVERSION_TIME_2074us	0x06
        INA238_CONVERSION_TIME_4120us	0x07
 	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_VBUSCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t vbusct){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if( vbusct <= 0x07 ){
        if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
            return ERROR_N;
        }

        //setup valu ADCRANGE
        write_reg = (uint16_t)( (0xF1FF & read_reg) | (( (uint16_t)vbusct ) << 9) ); //(0xF1FF & read_reg) - reset VBUSCT value

        if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, write_reg) != SUCCESS){
            return ERROR_N;
        }
    }else{
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get actual value VBUSCT. Conversion time of the bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*vbusct - pointer to store actual conversion time of the bus voltage measurement:
	    Possible values:
	        INA238_CONVERSION_TIME_50us		0x00
            INA238_CONVERSION_TIME_84us		0x01
            INA238_CONVERSION_TIME_150us	0x02
            INA238_CONVERSION_TIME_280us	0x03
            INA238_CONVERSION_TIME_540us	0x04
            INA238_CONVERSION_TIME_1052us	0x05
            INA238_CONVERSION_TIME_2074us	0x06
            INA238_CONVERSION_TIME_4120us	0x07
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_VBUSCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *vbusct){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *vbusct = ((uint8_t)(read_reg >> 9)) & 0x07;

    return SUCCESS;
}


/** @brief	Setup VSHCT. Sets the conversion time of the shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	vshct - variable that will setup VSHCT.
        INA238_CONVERSION_TIME_50us		0x00
        INA238_CONVERSION_TIME_84us		0x01
        INA238_CONVERSION_TIME_150us	0x02
        INA238_CONVERSION_TIME_280us	0x03
        INA238_CONVERSION_TIME_540us	0x04
        INA238_CONVERSION_TIME_1052us	0x05
        INA238_CONVERSION_TIME_2074us	0x06
        INA238_CONVERSION_TIME_4120us	0x07
 	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_VSHCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t vshct){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if( vshct <= 0x07 ){
        if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
            return ERROR_N;
        }

        //setup valu ADCRANGE
        write_reg = (uint16_t)( (0xFE3F & read_reg) | (( (uint16_t)vshct ) << 6) ); //(0xFE3F & read_reg) - reset VSHCT value

        if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, write_reg) != SUCCESS){
            return ERROR_N;
        }
    }else{
        return ERROR_N;
    };

    return SUCCESS;
}

/** @brief	Get actual value VSHCT. Conversion time of the shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*vshct - pointer to store actual conversion time of the shunt voltage measurement:
	    Possible values:
	        INA238_CONVERSION_TIME_50us		0x00
            INA238_CONVERSION_TIME_84us		0x01
            INA238_CONVERSION_TIME_150us	0x02
            INA238_CONVERSION_TIME_280us	0x03
            INA238_CONVERSION_TIME_540us	0x04
            INA238_CONVERSION_TIME_1052us	0x05
            INA238_CONVERSION_TIME_2074us	0x06
            INA238_CONVERSION_TIME_4120us	0x07
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_VSHCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *vshct){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *vshct = ((uint8_t)(read_reg >> 6)) & 0x07;

    return SUCCESS;
}


/** @brief	Setup VTCT. Sets the conversion time of the temperature measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	vtct - variable that will setup VTCT.
        INA238_CONVERSION_TIME_50us		0x00
        INA238_CONVERSION_TIME_84us		0x01
        INA238_CONVERSION_TIME_150us	0x02
        INA238_CONVERSION_TIME_280us	0x03
        INA238_CONVERSION_TIME_540us	0x04
        INA238_CONVERSION_TIME_1052us	0x05
        INA238_CONVERSION_TIME_2074us	0x06
        INA238_CONVERSION_TIME_4120us	0x07
 	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_VTCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t vtct){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if( vtct <= 0x07 ){
        if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
            return ERROR_N;
        }

        //setup valu ADCRANGE
        write_reg = (uint16_t)( (0xFFC7 & read_reg) | (( (uint16_t)vtct ) << 3) ); //(0xFFC7 & read_reg) - reset VTCT value

        if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, write_reg) != SUCCESS){
            return ERROR_N;
        }
    }else{
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get actual value VTCT. Conversion time of the  temperature measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*vtct - pointer to store actual conversion time of the temperature measurement:
	    Possible values:
	        INA238_CONVERSION_TIME_50us		0x00
            INA238_CONVERSION_TIME_84us		0x01
            INA238_CONVERSION_TIME_150us	0x02
            INA238_CONVERSION_TIME_280us	0x03
            INA238_CONVERSION_TIME_540us	0x04
            INA238_CONVERSION_TIME_1052us	0x05
            INA238_CONVERSION_TIME_2074us	0x06
            INA238_CONVERSION_TIME_4120us	0x07
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_VTCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *vtct){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *vtct = ((uint8_t)(read_reg >> 3)) & 0x07;

    return SUCCESS;
}


/** @brief	Setup AVG. Selects ADC sample averaging count. The averaging setting applies
            to all active inputs.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	avg - variable that will setup avg.
        INA238_AVERAGING_COUNT_1		0x00
        INA238_AVERAGING_COUNT_4		0x01
        INA238_AVERAGING_COUNT_16		0x02
        INA238_AVERAGING_COUNT_64		0x03
        INA238_AVERAGING_COUNT_128		0x04
        INA238_AVERAGING_COUNT_256		0x05
        INA238_AVERAGING_COUNT_512		0x06
        INA238_AVERAGING_COUNT_1024		0x07
 	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t avg){

    uint16_t read_reg = 0;
    uint16_t write_reg = 0;

    if( avg <= 0x07 ){
        if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
            return ERROR_N;
        }

        //setup valu ADCRANGE
        write_reg = (uint16_t)( (0xFFF8 & read_reg) | ((uint16_t)avg) ); //(0xFFF8 & read_reg) - reset avg value

        if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, write_reg) != SUCCESS){
            return ERROR_N;
        }
    }else{
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get actual value AVG. ADC sample averaging count
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	*avg - pointer to store actual ADC sample averaging count
	    Possible values:
	        INA238_CONVERSION_TIME_50us		0x00
            INA238_CONVERSION_TIME_84us		0x01
            INA238_CONVERSION_TIME_150us	0x02
            INA238_CONVERSION_TIME_280us	0x03
            INA238_CONVERSION_TIME_540us	0x04
            INA238_CONVERSION_TIME_1052us	0x05
            INA238_CONVERSION_TIME_2074us	0x06
            INA238_CONVERSION_TIME_4120us	0x07
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint8_t *avg){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_ADC_CONFIG_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *avg = ((uint8_t)read_reg ) & 0x07;

    return SUCCESS;
}



//************** Shunt Calibration (SHUNT_CAL) Register (Address = 2h) [reset = 1000h]

/** @brief	Initialization of INA238. Setting SHUNT_CAL calibration register for starting measuring in Ampere and Ohm
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - I2C INA238 7-bit address.
	@param 	max_exp_current_A - maximum expected current of load (Ampere float).
	@param 	Rshunt - value of shunt resistor (Ohm float).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_Calibration_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float Rshunt_Ohm){

    uint16_t calibration = 0;

    calibration = (uint16_t)( (819.2*1000000*max_exp_current_A*Rshunt_Ohm)/32768.0f );

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_SHUNT_CAL_REG_ADDR, calibration) != SUCCESS ){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Initialization of INA238. Setting SHUNT_CAL calibration register for starting measuring in miliAmpere and miliOhm
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA281_addr - I2C INA238 7-bit address.
	@param 	max_exp_current_mA - maximum expected current of load (miliAmpere uint16_t).
	@param 	Rshunt_mOhm - value of shunt resistor (miliOhm uint16_t).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_Calibration_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t max_exp_current_mA, uint16_t Rshunt_mOhm){

    uint16_t calibration = 0;

    calibration = (uint16_t)( ((uint32_t)(8192*max_exp_current_mA*Rshunt_mOhm))/((uint32_t)327680) );

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_SHUNT_CAL_REG_ADDR, calibration) != SUCCESS ){
        return ERROR_N;
    }

    return SUCCESS;
}


//************** Shunt Voltage Measurement (VSHUNT) Register (Address = 4h) [reset = 0h]

/** @brief	Get Shunt voltage measurement. in Volt
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Shunt_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *shunt_voltage){

    uint8_t adc_range = 0;
    uint16_t shunt_volt_reg = 0;
    int16_t sign_shunt_volt_reg = 0;
    float voltage_lsb = 0;

    if(INA238_Get_ADCRANGE( I2Cx, I2C_INA238_addr, &adc_range) != SUCCESS){
         return ERROR_N;
     }

    if(adc_range == INA238_ADC_RANGE_163mv ){
        voltage_lsb = 0.000005f; //ADC LSB value 5uV
    }else if( adc_range == INA238_ADC_RANGE_40mv ){ // INA238_ADC_RANGE_40mv
        voltage_lsb = 0.00000125f; //ADC LSB value 1.25uV
    }else{
        return ERROR_N;
    }


    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_VSHUNT_REG_ADDR, &shunt_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

    *shunt_voltage = (float)( voltage_lsb * ( (float)sign_shunt_volt_reg) );

    return SUCCESS;
}

/** @brief	Get Shunt voltage measurement. in miliVolt
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in miliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Shunt_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, int16_t *shunt_voltage){

    uint8_t adc_range = 0;
    uint16_t shunt_volt_reg = 0;
    int16_t sign_shunt_volt_reg = 0;
    float voltage_lsb = 0;

    if(INA238_Get_ADCRANGE( I2Cx, I2C_INA238_addr, &adc_range) != SUCCESS){
        return ERROR_N;
    }

    if(adc_range == INA238_ADC_RANGE_163mv ){
        voltage_lsb = 0.005f; //ADC LSB value 5mV
    }else if( adc_range == INA238_ADC_RANGE_40mv ){ // INA238_ADC_RANGE_40mv
        voltage_lsb = 0.00125f; //ADC LSB value 1.25mV
    }else{
        return ERROR_N;
    }

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_VSHUNT_REG_ADDR, &shunt_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

    *shunt_voltage = (int16_t)( voltage_lsb * ( (float)sign_shunt_volt_reg) );

    return SUCCESS;
}

//**************  Bus Voltage Measurement (VBUS) Register (Address = 5h) [reset = 0h]

/** @brief	Get Bus voltage measurement. in Volt
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Bus_V_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *bus_voltage){

    uint16_t bus_volt_reg = 0;
    int16_t sign_bus_volt_reg = 0;
    float voltage_lsb = 0;

    voltage_lsb = 0.003125f; //ADC LSB value 3.125mV

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_VBUS_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_bus_volt_reg = (int16_t)bus_volt_reg;

    *bus_voltage = (float)( voltage_lsb * ( (float)sign_bus_volt_reg) );

    return SUCCESS;
}


/** @brief	Get Bus voltage measurement. in miliVolt
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in miliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Bus_V_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t *bus_voltage){

    uint16_t bus_volt_reg = 0;
    int16_t sign_bus_volt_reg = 0;
    float voltage_lsb = 0;

    voltage_lsb = 3.125f; //ADC LSB value 3.125mV

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_VSHUNT_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_bus_volt_reg = (int16_t)bus_volt_reg;

    *bus_voltage = (uint16_t)( voltage_lsb * ( (float)sign_bus_volt_reg) );

    return SUCCESS;
}

//************** Temperature Measurement (DIETEMP) Register (Address = 6h) [reset = 0h]

/** @brief	Get Temperature voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured Temperature value.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Temperature_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *temperature){

    uint16_t temp_reg = 0;
    int16_t sign_temp_reg = 0;
    float temp_lsb = 0;

    temp_lsb = 0.125f; //ADC LSB value 125 m°C

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_DIETEMP_REG_ADDR, &temp_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_temp_reg = ((int16_t)temp_reg)>>4;

    *temperature = (float)( temp_lsb * ( (float)sign_temp_reg) );

    return SUCCESS;
}

//************** Current Result (CURRENT) Register (Address = 7h) [reset = 0h]
/** @brief	Get Current measurement. in Amperes
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	max_exp_current_A - maximum expected current of load (Amperea float).
	@param 	*current - pointer to store measured shunt current value in Amperes
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *current){

    uint16_t current_reg = 0;
    int16_t sign_current_reg = 0;
    float current_lsb = 0;

    current_lsb = max_exp_current_A/32768.0f;

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_current_reg = (int16_t)current_reg;

    *current = (float)( current_lsb * ( (float)sign_current_reg) );

    return SUCCESS;
}


/** @brief	Get Current measurement. in miliAmperes
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	max_exp_current_mA - maximum expected current of load (miliAmpere int16).
	@param 	*current - pointer to store measured shunt current value in miliAmperes
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Current_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, int16_t max_exp_current_mA, int16_t *current){

    uint16_t current_reg = 0;
    int16_t sign_current_reg = 0;
    float current_lsb = 0;

    current_lsb = ((float)max_exp_current_mA)/(32768.0f);

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_current_reg = (int16_t)current_reg;

    *current = (int16_t)( current_lsb * ( (float)sign_current_reg) );

    return SUCCESS;
}



//************** Power Result (POWER) Register (Address = 8h) [reset = 0h]
/** @brief	Get Power measurement. in Wats
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	max_exp_current_A - maximum expected current of load (Amperea float).
	@param 	*power - pointer to store measured power value in Wats
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *power){

    uint32_t power_reg = 0;
    float current_lsb = 0;

    current_lsb = max_exp_current_A/32768.0f;

    if(I2C_Read_word_u24_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_POWER_REG_ADDR, &power_reg) != SUCCESS){
        return ERROR_N;
    }

    *power = (float)( 0.2 * current_lsb * ( (float)power_reg) );

    return SUCCESS;
}

/** @brief	Get POwer measurement. in miliWats
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	max_exp_current_mA - maximum expected current of load (miliAmpere int16).
	@param 	*power - pointer to store measured power in miliWats
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Power_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, int16_t max_exp_current_mA, int16_t *power){

    uint32_t power_reg = 0;
    float current_lsb = 0;

    current_lsb = ((float)max_exp_current_mA)/32768.0f;

    if(I2C_Read_word_u24_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_POWER_REG_ADDR, &power_reg) != SUCCESS){
        return ERROR_N;
    }

    *power = (float)( 0.2 * current_lsb * ( (float)power_reg) );

    return SUCCESS;
}

//************** Diagnostic Flags and Alert (DIAG_ALRT) Register (Address = Bh) [reset = 0001h

/** @brief	Set ALATCH  "Alert Latch" bit in DIAG_ALRT regist.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Set_ALATCH(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0x8000 | read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}


/** @brief	Reset ALATCH  "Alert Latch" bit in DIAG_ALRT regist.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Reset_ALATCH(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0x7FFF & read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set CNVR  Setting this bit high configures the Alert pin to be asserted when the
            Conversion Ready Flag (bit 1) is asserted, indicating that a
            conversion cycle has completed. DIAG_ALRT registr
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Set_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0x4000 | read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}


/** @brief	Set CNVR  Setting this bit high configures the Alert pin to be asserted when the
            Conversion Ready Flag (bit 1) is asserted, indicating that a
            conversion cycle has completed. DIAG_ALRT registr
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Reset_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0xBFFF & read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set  SLOWALERT ALERT function is asserted on the completed averaged value.
            This gives the flexibility to delay the ALERT after the averaged value.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Set_SLOWALERT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0x2000 | read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}

/** @brief	Reset SLOWALERT ALERT function is asserted on the completed averaged value.
            This gives the flexibility to delay the ALERT after the averaged value.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Reset_SLOWALERT(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0xDFFF & read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set APOL Alert Polarity bit sets the Alert pin polarity.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Set_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0x1000 | read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}

/** @brief	Reset APOL Alert Polarity bit sets the Alert pin polarity.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Reset_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)(0xEFFF & read_reg);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Get alert flags (DIAG_ALRT)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	alert_flags - contain value of alert flags:
        bit0 - MEMSTAT
        bit1 - CNVRF
        bit2 - POL
        bit3 - BUSUL
        bit4 - BUSOL
        bit5 - SHNTUL
        bit6 - SHNTOL
        bit7 - TMPOL
        bit8 - RESERVED
        bit9 - MATHOF
        bit10,11 - RESERVED
        bit12 - APOL
        bit13 - SLOWALERT
        bit14 - CNVR
        bit15 - ALATCH

	@retval 0-OK, -1-ERROR_N
*/

ErrorStatus INA238_Get_ALERT_Flags(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t *alert_flags ){

    uint16_t read_reg = 0;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_DIAG_ALRT_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *alert_flags = read_reg;
    return SUCCESS;
}


//************** Shunt Overvoltage Threshold (SOVL) Register (Address = Ch) [reset = 7FFFh]
/** @brief	Set SOVL Shunt Overvoltage Threshold Level in Voltage
            Note! before setup SOVL you heve to setup ADCRANGE!
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	shunt_ovl - value Shunt Overvoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_Shunt_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float shunt_ovl){

    uint8_t adc_range = 0;
    int16_t reg_value = 0;
    float voltage_lsb = 0;

    if(INA238_Get_ADCRANGE( I2Cx, I2C_INA238_addr, &adc_range) != SUCCESS){
        return ERROR_N;
    }

    if(adc_range == INA238_ADC_RANGE_163mv ){
        voltage_lsb = 0.000005f; //ADC LSB value 5mV
    }else if( adc_range == INA238_ADC_RANGE_40mv ){ // INA238_ADC_RANGE_40mv
        voltage_lsb = 0.00000125f; //ADC LSB value 1.25mV
    }else{
        return ERROR_N;
    }

    reg_value = (int16_t)(shunt_ovl / voltage_lsb);

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_SOVL_REG_ADDR, (uint16_t)reg_value) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get SOVL Shunt Overvoltage Threshold Level in Voltage
            Note! before Read SOVL you heve to setup ADCRANGE!
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*shunt_ovl - pointer contain value Shunt Overvoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Shunt_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *shunt_ovl){

    uint8_t adc_range = 0;
    uint16_t shunt_volt_reg = 0;
    int16_t sign_shunt_volt_reg = 0;
    float voltage_lsb = 0;

    if(INA238_Get_ADCRANGE( I2Cx, I2C_INA238_addr, &adc_range) != SUCCESS){
        return ERROR_N;
    }

    if(adc_range == INA238_ADC_RANGE_163mv ){
        voltage_lsb = 0.000005f; //ADC LSB value 5uV
    }else if( adc_range == INA238_ADC_RANGE_40mv ){ // INA238_ADC_RANGE_40mv
        voltage_lsb = 0.00000125f; //ADC LSB value 1.25uV
    }else{
        return ERROR_N;
    }


    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_SOVL_REG_ADDR, &shunt_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

    *shunt_ovl = (float)( voltage_lsb * ( (float)sign_shunt_volt_reg) );

    return SUCCESS;
}

//**************  Shunt Undervoltage Threshold (SUVL) Register (Address = Dh) [reset = 8000h]
/** @brief	Set SUVL  Shunt Undervoltage Threshold Level in Voltage
            Note! before setup SUVL you heve to setup ADCRANGE!
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	shunt_uvl - value Shunt Undervoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_Shunt_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float shunt_uvl){

    uint8_t adc_range = 0;
    int16_t reg_value = 0;
    float voltage_lsb = 0;

    if(INA238_Get_ADCRANGE( I2Cx, I2C_INA238_addr, &adc_range) != SUCCESS){
        return ERROR_N;
    }

    if(adc_range == INA238_ADC_RANGE_163mv ){
        voltage_lsb = 0.000005f; //ADC LSB value 5mV
    }else if( adc_range == INA238_ADC_RANGE_40mv ){ // INA238_ADC_RANGE_40mv
        voltage_lsb = 0.00000125f; //ADC LSB value 1.25mV
    }else{
        return ERROR_N;
    }

    reg_value = (int16_t)(shunt_uvl / voltage_lsb);

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_SUVL_REG_ADDR, (uint16_t)reg_value) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get SUVL Shunt Undervoltage Threshold Level in Voltage
            Note! before Read SUVL you heve to setup ADCRANGE!
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*shunt_uvl - pointer contain value Shunt Undervoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Shunt_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *shunt_uvl){

    uint8_t adc_range = 0;
    uint16_t shunt_volt_reg = 0;
    int16_t sign_shunt_volt_reg = 0;
    float voltage_lsb = 0;

    if(INA238_Get_ADCRANGE( I2Cx, I2C_INA238_addr, &adc_range) != SUCCESS){
        return ERROR_N;
    }

    if(adc_range == INA238_ADC_RANGE_163mv ){
        voltage_lsb = 0.000005f; //ADC LSB value 5uV
    }else if( adc_range == INA238_ADC_RANGE_40mv ){ // INA238_ADC_RANGE_40mv
        voltage_lsb = 0.00000125f; //ADC LSB value 1.25uV
    }else{
        return ERROR_N;
    }


    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_SUVL_REG_ADDR, &shunt_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

    *shunt_uvl = (float)( voltage_lsb * ( (float)sign_shunt_volt_reg) );

    return SUCCESS;
}


//************** Bus Overvoltage Threshold (BOVL) Register (Address = Eh) [reset = 7FFFh]
/** @brief	Set BOVL Bus Overvoltage Threshold Level in Voltage
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	bus_ovl - value Bus Overvoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_Bus_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float bus_ovl){

    uint16_t reg_value = 0;
    float voltage_lsb = 0;

    voltage_lsb = 0.003125f; //ADC LSB value 3.125mV

    reg_value = (uint16_t)(bus_ovl / voltage_lsb);

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_BOVL_REG_ADDR, reg_value) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get BOVL Bus Overvoltage Threshold Level in Voltage
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*bus_ovl - pointer contain value BUS Undervoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Bus_OVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *bus_ovl){

    uint16_t bus_volt_reg = 0;
    float voltage_lsb = 0;

    voltage_lsb = 0.003125f; //ADC LSB value 3.125mV

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_BOVL_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    *bus_ovl = (float)( voltage_lsb * ( (float)bus_volt_reg) );

    return SUCCESS;
}


//**************  Bus Undervoltage Threshold (BUVL) Register (Address = Fh) [reset = 0h]
/** @brief	Set BUVL Bus Undervoltage Threshold Level in Voltage
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	bus_uvl - value Bus Undervoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_Bus_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float bus_uvl){

    uint16_t reg_value = 0;
    float voltage_lsb = 0;

    voltage_lsb = 0.003125f; //ADC LSB value 3.125mV

    reg_value = (uint16_t)(bus_uvl / voltage_lsb);

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_BUVL_REG_ADDR, reg_value) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get BOVL Bus Undervoltage Threshold Level in Voltage
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*bus_uvl - pointer contain value Bus Undervoltage Threshold in voltage (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_Bus_UVL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *bus_uvl){

    uint16_t bus_volt_reg = 0;
    float voltage_lsb = 0;

    voltage_lsb = 0.003125f; //ADC LSB value 3.125mV

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_BUVL_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    *bus_uvl = (float)( voltage_lsb * ( (float)bus_volt_reg) );

    return SUCCESS;
}



//************** Temperature Over-Limit Threshold (TEMP_LIMIT) Register (Address = 10h) [reset = 7FFFh]
/** @brief	Set TOL Temperature Over-Limit Threshold in °C
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	tol - value Temperature Over-Limit Threshold in °C (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_TOL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float tol){

    int16_t reg_value = 0;
    float temp_lsb = 0;

    temp_lsb = 0.125f; //ADC LSB value 125 m°C

    reg_value = (int16_t)(tol/ temp_lsb);

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_TEMP_LIMIT_REG_ADDR, (uint16_t)( reg_value<<4) ) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get TOL Temperature Over-Limit Threshold in °C
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*tol- pointer contain value Temperature Over-Limit Threshold in °C (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_TOL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float *tol){

    uint16_t temp_reg = 0;
    int16_t sign_temp_reg = 0;
    float temp_lsb = 0;

    temp_lsb = 0.125f; //ADC LSB value 125 m°C

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_TEMP_LIMIT_REG_ADDR, &temp_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_temp_reg = ((int16_t)temp_reg)>>4;

    *tol = (float)( temp_lsb * ( (float)sign_temp_reg) );

    return SUCCESS;
}



//************** Power Over-Limit Threshold (PWR_LIMIT) Register (Address = 11h) ) [reset = FFFFh]
/** @brief	Set POL Power Over-Limit Threshold (PWR_LIMIT) Register
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	max_exp_current_A - maximum expected current of load (Amperea float)
	@param 	tol - value Power Over-Limit Threshold in Wats (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Setup_POL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float pol){

    uint16_t reg_value = 0;
    float power_lsb = 0;

    power_lsb = (max_exp_current_A/32768.0f) * 0.2f; //ADC LSB value 125 m°C

    reg_value = (uint16_t)(pol / (power_lsb * 256.0f));

    if(INA238_Write_Reg(I2Cx, I2C_INA238_addr, INA238_PWR_LIMIT_REG_ADDR, reg_value) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get POL Power Over-Limit Threshold (PWR_LIMIT) Register
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
    @param 	max_exp_current_A - maximum expected current of load (Amperea float)
	@param 	pol - pointer contain value Power Over-Limit Threshold in Wats (float)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_POL_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *pol){

    uint16_t reg_value = 0;
    float power_lsb = 0;

    power_lsb = (max_exp_current_A/32768.0f) * 0.2f; //ADC LSB value 125 m°C

    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_PWR_LIMIT_REG_ADDR, &reg_value) != SUCCESS){
        return ERROR_N;
    }

    *pol =  (float)reg_value * power_lsb * 256.0f;

    return SUCCESS;
}


/** @brief	Current, voltage and power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*current - pointer to store measured current value in Amps.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@param 	*power - pointer to store measured power value in Watts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_I_V_P_float( I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, float max_exp_current_A, float *current, float *bus_voltage, float *power ){

    uint16_t current_reg = 0;
    int16_t sign_current_reg = 0;
    int16_t sign_bus_volt_reg = 0;
    uint32_t power_reg = 0;
    uint16_t bus_volt_reg = 0;
    float current_lsb = ((float)max_exp_current_A)/32768.0f;
    float power_lsb = current_lsb * 0.2f;
    float voltage_lsb = 0.003125f; //ADC LSB value 3.125mV

    //Get current
    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
        return ERROR_N;
    }
    sign_current_reg = (int16_t)current_reg;
    *current = (float)( current_lsb * ( (float)sign_current_reg) );

    //Get voltage
    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_VBUS_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }
    sign_bus_volt_reg = (int16_t)bus_volt_reg;
    *bus_voltage = (float)( voltage_lsb * ( (float)sign_bus_volt_reg) );

    //Get power
    if(I2C_Read_word_u24_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_POWER_REG_ADDR, &power_reg) != SUCCESS){
        return ERROR_N;
    }

    *power = (float)( power_lsb * ( (float)power_reg) );

    return SUCCESS;
}

/** @brief Get value Current, Voltage and Power.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA238_addr - 7-bit device address.
	@param 	*current_mA - pointer to store measured current value in milliAmps.
	@param 	*bus_voltage_mV - pointer to store measured bus voltage value in milliVolts.
	@param 	*power_mW - pointer to store measured power value in milliWatts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA238_Get_I_V_P_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA238_addr, uint16_t max_exp_current_mA, int16_t *current_mA, uint16_t *bus_voltage_mV, uint16_t *power_mW){

    uint16_t current_reg = 0;
    int16_t sign_current_reg = 0;
    int16_t sign_bus_volt_reg = 0;
    uint32_t power_reg = 0;
    uint16_t bus_volt_reg = 0;
    float current_lsb = ((float)max_exp_current_mA)/32768.0f;
    float power_lsb = current_lsb * 0.2f;
    float voltage_lsb = 3.125f; //ADC LSB value 3.125mV

    //Get current
    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
        return ERROR_N;
    }
    sign_current_reg = (int16_t)current_reg;
    *current_mA = (int16_t)( current_lsb * ( (float)sign_current_reg) );

    //Get voltage
    if(INA238_Read_Reg(I2Cx, I2C_INA238_addr, INA238_VBUS_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }
    sign_bus_volt_reg = (int16_t)bus_volt_reg;
    *bus_voltage_mV = (uint16_t)( voltage_lsb * ( (float)sign_bus_volt_reg) );

    //Get power
    if(I2C_Read_word_u24_St_ReSt(I2Cx, I2C_INA238_addr, I2C_SIZE_REG_ADDR_U8, INA238_POWER_REG_ADDR, &power_reg) != SUCCESS){
        return ERROR_N;
    }
    *power_mW = (uint16_t)( power_lsb * ( (float)power_reg) );

    return SUCCESS;
}
