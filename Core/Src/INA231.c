#include "stm32l4xx.h"
#include "i2c_comm.h"
#include "INA231.h"


/** @brief	Read registers INA231. 16bit register
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - I2C INA281 7-bit address.
    @param  INA231_addr_reg address registers of INA231:
        INA231_CONFIGURATION_REG_ADDR			0x00
        INA231_SHUNT_VOLTAGE_REG_ADDR			0x01
        INA231_BUS_VOLTAGE_REG_ADDR				0x02
        INA231_POWER_REG_ADDR					0x03
        INA231_CURRENT_REG_ADDR					0x04
        INA231_CALIBRATION_REG_ADDR				0x05
        INA231_MASK_REG_ADDR					0x06
        INA231_ALERT_LIMIT_REG_ADDR				0x07
	@param 	*Read_data - pointer to store data from Registers.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Read_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t INA231_addr_reg, uint16_t *read_data){

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_addr_reg, read_data) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Write Registers INA231.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - I2C INA231 7-bit address.
    @param  INA231_addr_reg address registers of INA231:
        INA231_CONFIGURATION_REG_ADDR			0x00
        INA231_SHUNT_VOLTAGE_REG_ADDR			0x01
        INA231_BUS_VOLTAGE_REG_ADDR				0x02
        INA231_POWER_REG_ADDR					0x03
        INA231_CURRENT_REG_ADDR					0x04
        INA231_CALIBRATION_REG_ADDR				0x05
        INA231_MASK_REG_ADDR					0x06
        INA231_ALERT_LIMIT_REG_ADDR				0x07
	@param 	write_data - variable that will be write.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Write_Reg(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t INA231_addr_reg, uint16_t write_data){

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_addr_reg, write_data) != SUCCESS){
        return ERROR_N;
    }
    return SUCCESS;
}



/** @brief	Power reset of INA231. Setting this bit to 1 generates a system reset that is the same
  			as a power-on reset; all registers are reset to default values. This bit self-clears.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Power_Reset(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, 0x8000) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Setup averaging mode. These bits set the number of samples that are collected and averaged together
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	avg_mode - variable that will set averaging mode.
			0x00 - INA231_AVERAGING_NUM_1
			0x01 - INA231_AVERAGING_NUM_4
			0x02 - INA231_AVERAGING_NUM_16
			0x03 - INA231_AVERAGING_NUM_64
			0x04 - INA231_AVERAGING_NUM_128
			0x05 - INA231_AVERAGING_NUM_256
			0x06 - INA231_AVERAGING_NUM_512
			0x07 - INA231_AVERAGING_NUM_1024
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Setup_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t avg_mode){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    //setup valu AVR mode
    write_reg = (uint16_t)( (0xF1FF & read_reg) | (avg_mode << 9) ); //(0xF1FF & read_reg) - reset Average moide value

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Read averaging mode. These bits set the number of samples that are collected and averaged together
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*avg_mode - pointer to store actual averaging mode.
			Possible values:
				0x00 - INA231_AVERAGING_NUM_1
				0x01 - INA231_AVERAGING_NUM_4
				0x02 - INA231_AVERAGING_NUM_16
				0x03 - INA231_AVERAGING_NUM_64
				0x04 - INA231_AVERAGING_NUM_128
				0x05 - INA231_AVERAGING_NUM_256
				0x06 - INA231_AVERAGING_NUM_512
				0x07 - INA231_AVERAGING_NUM_1024
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_AVG(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *avg_mode){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    *avg_mode = (uint8_t)((read_reg >> 9) & 0x07);

    return SUCCESS;
}


/** @brief	Write bus voltage conversion time. These bits set the conversion time for the bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	conv_time - variable that will set bus voltage conversion time.
			0x00 - INA231_CONVERSION_TIME_140us
			0x01 - INA231_CONVERSION_TIME_204us
			0x02 - INA231_CONVERSION_TIME_332us
			0x03 - INA231_CONVERSION_TIME_588us
			0x04 - INA231_CONVERSION_TIME_1100us
			0x05 - INA231_CONVERSION_TIME_2116us
			0x06 - INA231_CONVERSION_TIME_4156us
			0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Setup_VbusCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t conv_time){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    write_reg = (uint16_t)( (0xFE3F & read_reg) | (conv_time << 6) );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get bus voltage conversion time. These bits set the conversion time for the bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*conv_time - pointer to store actual conversion time.
			Possible values:
				0x00 - INA231_CONVERSION_TIME_140us
				0x01 - INA231_CONVERSION_TIME_204us
				0x02 - INA231_CONVERSION_TIME_332us
				0x03 - INA231_CONVERSION_TIME_588us
				0x04 - INA231_CONVERSION_TIME_1100us
				0x05 - INA231_CONVERSION_TIME_2116us
				0x06 - INA231_CONVERSION_TIME_4156us
				0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_VbusCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *conv_time){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    *conv_time = (uint8_t)((read_reg >> 6) & 0x07);

    return SUCCESS;
}


/** @brief	Write shunt voltage conversion time. These bits set the conversion time for the shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	conv_time - variable that will set shunt voltage conversion time.
			0x00 - INA231_CONVERSION_TIME_140us
			0x01 - INA231_CONVERSION_TIME_204us
			0x02 - INA231_CONVERSION_TIME_332us
			0x03 - INA231_CONVERSION_TIME_588us
			0x04 - INA231_CONVERSION_TIME_1100us
			0x05 - INA231_CONVERSION_TIME_2116us
			0x06 - INA231_CONVERSION_TIME_4156us
			0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Setup_VshCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t conv_time){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)( (0xFFC7 & read_reg) | (conv_time << 3) );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Get shunt voltage conversion time. These bits set the conversion time for the shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual conversion time.
				Possible values:
				0x00 - INA231_CONVERSION_TIME_140us
				0x01 - INA231_CONVERSION_TIME_204us
				0x02 - INA231_CONVERSION_TIME_332us
				0x03 - INA231_CONVERSION_TIME_588us
				0x04 - INA231_CONVERSION_TIME_1100us
				0x05 - INA231_CONVERSION_TIME_2116us
				0x06 - INA231_CONVERSION_TIME_4156us
				0x07 - INA231_CONVERSION_TIME_8244us
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_VshCT(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *conv_time){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    *conv_time = (uint8_t)((read_reg >> 3) & 0x07);

    return SUCCESS;
}


/** @brief	Write operation mode. These bits select continuous, triggered, or power-down mode of operation.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	write_data - variable that will set operation mode.
			0x00 - INA231_POWER_DOWN
			0x01 - INA231_SHUNT_VOLTAGE_TRIGGERED
			0x02 - INA231_BUS_VOLTAGE_TRIGGERED
			0x03 - INA231_SHUNT_AND_BUS_TRIGGERED
			0x04 - INA231_POWER_DOWN
			0x05 - INA231_SHUNT_VOLTAGE_CONTINUOUS
			0x06 - INA231_BUS_VOLTAGE_CONTINUOUS
			0x07 - INA231_SHUNT_AND_BUS_CONTINUOUS
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Setup_Mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t mode){

    uint16_t read_reg;
    uint16_t write_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    write_reg = (uint16_t)( (0xFFF8 & read_reg) | mode );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, write_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Get operation mode. These bits select continuous, triggered, or power-down mode of operation.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*mode - pointer to store actual operation mode.
			Possible values:
				0x00 - INA231_POWER_DOWN(not used)
				0x01 - INA231_SHUNT_VOLTAGE_TRIGGERED
				0x02 - INA231_BUS_VOLTAGE_TRIGGERED
				0x03 - INA231_SHUNT_AND_BUS_TRIGGERED
				0x04 - INA231_POWER_DOWN
				0x05 - INA231_SHUNT_VOLTAGE_CONTINUOUS
				0x06 - INA231_BUS_VOLTAGE_CONTINUOUS
				0x07 - INA231_SHUNT_AND_BUS_CONTINUOUS
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Mode(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint8_t *mode){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CONFIGURATION_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }
    *mode = (uint8_t)(read_reg & 0x07);

    return SUCCESS;
}


/** @brief	Set "Shunt Voltage Overvoltage" bit (Mask/Enable Register - SOL). Setting this bit high configures the ALERT pin to
  			be asserted when the shunt voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_SOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0x7FFF & read_reg) | 0x8000);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Shunt Voltage Overvoltage" bit (Mask/Enable Register - SOL). Setting this bit high configures the ALERT pin to
  			be asserted when the shunt voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_SOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0x7FFF & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set "Shunt Voltage Undervoltage" bit (Mask/Enable Register - SUL). Setting this bit high configures the ALERT pin to
  	  	  	be asserted when the shunt voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_SUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xBFFF & read_reg) | 0x4000);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Shunt Voltage Undervoltage" bit (Mask/Enable Register - SUL). Setting this bit high configures the ALERT pin to
  	  	  	be asserted when the shunt voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_SUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xBFFF & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set "Bus Voltage Overvoltage" bit (Mask/Enable Register - BOL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_BOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xDFFF & read_reg) | 0x2000);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Bus Voltage Overvoltage" bit (Mask/Enable Register - BOL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_BOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xDFFF & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set "Bus Voltage Undervoltage" bit (Mask/Enable Register - BUL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_BUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xEFFF & read_reg) | 0x1000);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Bus Voltage Undervoltage" bit (Mask/Enable Register - BUL). Setting this bit high configures the ALERT pin to
  			be asserted when the bus voltage conversion drops below the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_BUL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xEFFF & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}



/** @brief	Set "Power Overlimit" bit (Mask/Enable Register - POL). Setting this bit high configures the ALERT pin to
  			be asserted when the power calculation exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_POL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xF7FF & read_reg) | 0x0800);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Power Overlimit" bit (Mask/Enable Register - POL). Setting this bit high configures the ALERT pin to
  			be asserted when the power calculation exceeds the value in the Alert Limit register.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_POL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xF7FF & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Set "Conversion Ready" bit (Mask/Enable Register - CNVR). Setting this bit high configures the ALERT pin to
  			be asserted when the Conversion Ready Flag bit (CVRF, bit 3) is asserted, indicating
  			that the device is ready for the next conversion.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xFBFF & read_reg) | 0x0400);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Conversion Ready" bit (Mask/Enable Register - CNVR). Setting this bit high configures the ALERT pin to
  			be asserted when the Conversion Ready Flag bit (CVRF, bit 3) is asserted, indicating
  			that the device is ready for the next conversion.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_CNVR(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xFBFF & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}



/** @brief	Get Alerts
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*read_data - pointer to store actual bit state.
        bit0 - LEN: Alert Latch Enable
        bit1 - APOL: Alert Polarity
        bit2 - OVF: Math Overflow Flag
        bit3 - CVRF: Conversion Ready Flag
        bit4 - AFF: Alert Function Flag

        bit10 - CNVR: Conversion Ready
        bit11 - POL: Power Overlimit
        bit12 - BUL: Bus Voltage Undervoltage
        bit13 - BOL: Bus Voltage Overvoltage
        bit14 - SUL: Shunt Voltage Undervoltage
        bit15 - SOL: Shunt Voltage Overvoltage1
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Alert_Config(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *read_data){

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, read_data) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Set "Alert Polarity" bit (Mask/Enable Register - APOL). Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xFFFD & read_reg) | 0x0002);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset  "Alert Polarity" bit (Mask/Enable Register - APOL). Configures the latching feature of the ALERT pin and the flag bits.
			1 = Inverted (active-high open collector)
			0 = Normal (active-low open collector) (default)
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_APOL(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xFFFD & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}



/** @brief	Set "Alert Latch Enable" bit (Mask/Enable Register - LEN). Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_LEN(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( (0xFFFE & read_reg) | 0x0001);

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}

/** @brief	Reset "Alert Latch Enable" bit (Mask/Enable Register - LEN). Configures the latching feature of the ALERT pin and flag bits.
			1 = Latch enabled
			0 = Transparent (default)
			When the Alert Latch Enable bit is set to Transparent mode, the ALERT pin and flag bits reset to their idle states
			when the fault has been cleared. When the Alert Latch Enable bit is set to Latch mode, the ALERT pin and flag bits
			remain active following a fault until the Mask/Enable register has been read.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Reset_LEN(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr){

    uint16_t read_reg;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR, &read_reg) != SUCCESS){
        return ERROR_N;
    }

    read_reg = (uint16_t)( 0xFFFE & read_reg );

    if(I2C_Write_word_u16_St(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_MASK_REG_ADDR,  read_reg) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Initialization of INA231. Setting calibration register for starting measuring.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	Rshunt - value of shunt resistor (Ohm float).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_Calibration_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float Rshunt){

    uint16_t calibration = (uint16_t)( 167.77216f / (max_current * Rshunt) );

    if( INA231_Write_Reg( I2Cx, I2C_INA231_addr, INA231_CALIBRATION_REG_ADDR, calibration) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Initialization of INA231. Setting calibration register for starting measuring.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	Rshunt - value of shunt resistor (mOhm uint16_t).
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Set_Calibration_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t Rshunt){

    uint16_t calibration = 0;

    calibration = (uint16_t)( 167772160 / (max_current * Rshunt) );

    if( INA231_Write_Reg( I2Cx, I2C_INA231_addr, INA231_CALIBRATION_REG_ADDR, calibration) != SUCCESS){
        return ERROR_N;
    }

    return SUCCESS;
}


/** @brief	Current measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	*current - pointer to store measured current value in Amps.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Current_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current){

    uint16_t current_reg = 0;
    int16_t sign_current_reg = 0;
    float current_lsb = max_current / 32768.0f;

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_current_reg = (int16_t)current_reg;

    *current = (float)( current_lsb * ((float)sign_current_reg) );

    return SUCCESS;
}


/** @brief	Current measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	*current - pointer to store measured current value in milliAmps.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Current_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current){

    uint16_t current_reg = 0;
    int16_t sign_current_reg = 0;

    float current_lsb = ((float)max_current) / 32768.0f; //Current in mA

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_current_reg = (int16_t)current_reg;

    *current = (int16_t)( current_lsb * ((float)sign_current_reg));

    return SUCCESS;
}


/** @brief	Power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (Ampere float).
	@param 	*power - pointer to store measured power value in Watts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Power_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *power){

    uint16_t power_reg = 0;

    float power_lsb = (max_current / 32768.0f) * 25.0f; // 25 times see Datasheet

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_POWER_REG_ADDR, &power_reg) != SUCCESS){
        return ERROR_N;
    }

    *power = (float)( power_lsb * ((float)power_reg) );

    return SUCCESS;
}

/** @brief	Power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	max_current - maximum expected current of load (mAmpere uint16_t).
	@param 	*power - pointer to store measured power value in milliWatts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_Power_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, uint16_t *power){

    uint16_t power_reg = 0;

    float power_lsb = ( ((float)max_current) / 32768.0f ) * 25.0f; // 25 times see Datasheet, Current in mA

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_POWER_REG_ADDR, &power_reg) != SUCCESS){
        return ERROR_N;
    }

    *power = (uint16_t)( power_lsb * ((float)power_reg));

    return SUCCESS;
}



/** @brief	Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_BusV_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *bus_voltage){

    uint16_t bus_volt_reg = 0;
    float voltage_lsb = 0.00125f; //Fixed value 1.25mV

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_BUS_VOLTAGE_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    *bus_voltage = (float)( voltage_lsb * ((float)bus_volt_reg) );

    return SUCCESS;
}

/** @brief	Bus voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*bus_voltage - pointer to store measured bus voltage value in milliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_BusV_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t *bus_voltage){

    uint16_t bus_volt_reg = 0;
    float voltage_lsb = 1.25f; //Fixed value 1.25mV

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_BUS_VOLTAGE_REG_ADDR, &bus_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    *bus_voltage = (uint16_t)( voltage_lsb * ((float)bus_volt_reg)); //*1000 convert to mVolts

    return SUCCESS;
}


/** @brief	Shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in Volts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_ShuntV_float(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float *shunt_voltage){

    uint16_t shunt_volt_reg = 0;
    int16_t sign_shunt_volt_reg = 0;

    float voltage_lsb = 0.0000025f; //Fixed value 2.5uV

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_SHUNT_VOLTAGE_REG_ADDR, &shunt_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

    *shunt_voltage = (float)( voltage_lsb * ((float)sign_shunt_volt_reg) );

    return SUCCESS;
}

/** @brief	Shunt voltage measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*shunt_voltage - pointer to store measured shunt voltage value in milliVolts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_ShuntV_int16( I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, int16_t *shunt_voltage ){

    uint16_t shunt_volt_reg = 0;
    int16_t sign_shunt_volt_reg = 0;
    float voltage_lsb = 0.0025f; //Fixed value 2.5uV

    if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_SHUNT_VOLTAGE_REG_ADDR, &shunt_volt_reg) != SUCCESS){
        return ERROR_N;
    }

    sign_shunt_volt_reg = (int16_t)shunt_volt_reg;

    *shunt_voltage = (int16_t)( voltage_lsb * ((float)sign_shunt_volt_reg));

    return SUCCESS;
}



/** @brief	Current, voltage and power measurement.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*current - pointer to store measured current value in Amps.
	@param 	*bus_voltage - pointer to store measured bus voltage value in Volts.
	@param 	*power - pointer to store measured power value in Watts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_I_V_P_float( I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, float max_current, float *current, float *bus_voltage, float *power ){

	uint16_t current_reg = 0;
	int16_t sign_current_reg = 0;
	uint16_t power_reg = 0;
	uint16_t bus_volt_reg = 0;
	float current_lsb = max_current / 32768.0f;
	float power_lsb = current_lsb * 25.0f;

	//Get current value
	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
	    return ERROR_N;
	}

	sign_current_reg = (int16_t)current_reg;

	*current = (float)( current_lsb * ((float)sign_current_reg) );

	//Get voltage value
	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_BUS_VOLTAGE_REG_ADDR, &bus_volt_reg) != SUCCESS){
	    return ERROR_N;
	}

	*bus_voltage = (float)(0.00125f * ((float)bus_volt_reg) );

	//Get power value
	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_POWER_REG_ADDR, &power_reg) != SUCCESS){
	    return ERROR_N;
	}

	*power = (float)( power_lsb * ((float)power_reg) );

	return SUCCESS;
}

/** @brief Get value Current, Voltage and Power.
	@param 	*I2Cx - pointer to I2C controller, where x is a number (e.x., I2C1, I2C2 etc.).
	@param 	I2C_INA231_addr - 7-bit device address.
	@param 	*current - pointer to store measured current value in milliAmps.
	@param 	*bus_voltage - pointer to store measured bus voltage value in milliVolts.
	@param 	*power - pointer to store measured power value in milliWatts.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus INA231_Get_I_V_P_int16(I2C_TypeDef *I2Cx, uint8_t I2C_INA231_addr, uint16_t max_current, int16_t *current, uint16_t *bus_voltage, uint16_t *power){

	uint16_t current_reg = 0;
	int16_t sign_current_reg = 0;
	uint16_t power_reg = 0;
	uint16_t bus_volt_reg = 0;
	float current_lsb = ((float)max_current) / 32768.0f;
	float power_lsb = current_lsb * 25.0f;

	//Get current value
	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_CURRENT_REG_ADDR, &current_reg) != SUCCESS){
	    return ERROR_N;
	}

	sign_current_reg = (int16_t)current_reg;

	*current = (int16_t)( current_lsb * ((float)sign_current_reg) );

	//Get voltage value
	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_BUS_VOLTAGE_REG_ADDR, &bus_volt_reg) != SUCCESS){
	    return ERROR_N;
	}

	*bus_voltage = (uint16_t)( 1.25f * ((float)bus_volt_reg) );

	//Get power value
	if(I2C_Read_word_u16_St_ReSt(I2Cx, I2C_INA231_addr, I2C_SIZE_REG_ADDR_U8, INA231_POWER_REG_ADDR, &power_reg) != SUCCESS){
	    return ERROR_N;
	}

	*power = (uint16_t)( power_lsb * ((float)power_reg) );

	return SUCCESS;
}
