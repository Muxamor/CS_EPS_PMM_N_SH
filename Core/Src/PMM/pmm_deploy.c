#include "pmm_deploy.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "ADS1015.h"
#include "TIM_delay.h"
#include "Error_Handler.h"

#define I2C_ADS1015_ADDR		0x48

#define PMM_BOTH_SWITCH_OFF		1276
#define PMM_BOTH_SWITCH_ON		0
#define PMM_ONE_SWITCH_ON		572 //976

#define PMM_BURN_TIME1			5000//1000  // millisec
#define PMM_BURN_TIME2			3000//2000  // millisec

#define PMM_EN_SW_DP_Ch1		PCA9534_IO_P00  // 0x01
#define PMM_EN_SW_DP_Ch2		PCA9534_IO_P01  // 0x02
#define PMM_EN_SW_DP_Ch3		PCA9534_IO_P02  // 0x04
#define PMM_EN_SW_DP_Ch4		PCA9534_IO_P03  // 0x08

#define PMM_OUT1				PCA9534_IO_P05  // 0x20
#define PMM_OUT2				PCA9534_IO_P06  // 0x40

#define PMM_30MIN_DELAY			1800000  // millisec
#define PMM_1MIN_DELAY			60000  // millisec


/** @brief	Setting default values for all registers of PCA9534.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PCA_default(){

	int8_t status = 0;

	status += PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);
	status += PCA9534_Set_output_pin(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);
	status += PCA9534_conf_IO_pol_normal(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);

	if(status != 0){
		Error_Handler();

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Setting default values for all registers of TCA9539.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus TCA_default(){

	int8_t status = 0;

	status += TCA9539_conf_IO_dir_input(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);
	status += TCA9539_conf_IO_pol_normal(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_ALL);

	if(status != 0){
		Error_Handler();

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Power supply to logic block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Enable_eF_Deploy_L(){

	int8_t status = 0;

	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10);

	if(status != 0){
		Error_Handler();

		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Power supply to power block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Enable_eF_Deploy_P(){

	int8_t status = 0;

	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P11);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P11);

	if(status != 0){
		Error_Handler();

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Power cut to logic block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Disable_eF_Deploy_L(){

	int8_t status = 0;

	status += TCA9539_Reset_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10);

	if(status != 0){
		Error_Handler();

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Power cut to power block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Disable_eF_Deploy_P(){

	int8_t status = 0;

	status += TCA9539_Reset_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P11);

	if(status != 0){
		Error_Handler();

		return ERROR_N;
	}

	return SUCCESS;
}


/** @brief	Reading state of deploy control channel.
    @param  switch_num - number of PCA9534 input pin state (EXIT_SENSOR_1(2) state)
    		0x20 : PMM_OUT1
    		0x40 : PMM_OUT2
    @param  *state - pointer to store state of input pin.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_check_switch(uint8_t switch_num, uint8_t *state){

	int8_t status = 0;

	if(status += PCA9534_read_input_pin(I2C4, PCA9534_ADDR, switch_num, state) != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;
}

/** @brief	Burning a wire for antennas and power deployment.
    @param  burn_time1 - time during which current will be supplied to the wire.
    @param  burn_time2 - if the first time is not enough, then the current will be supplied additionally for a given time.
    @param  wire_num - channel number of deploy power (PCA9534 pin number)
    		0x01 : PMM_EN_SW_DP_Ch1 (Z+ side deploy antenna power)
    		0x02 : PMM_EN_SW_DP_Ch2 (Z- side deploy antenna power)
    		0x04 : PMM_EN_SW_DP_Ch3 (Y- side deploy power)
    		0x08 : PMM_EN_SW_DP_Ch4 (Y+ side deploy power)
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_burn_the_wire(uint32_t burn_time1, uint32_t burn_time2, uint8_t wire_num){

	int8_t status = 0;
	int16_t meas_voltage = 0;
	uint8_t burn_status = 0;

	LL_mDelay(50); // нужно время для установки пинов PCA9534
	/*
	  	5.1) Пин переводится в активное состояние и находится в нем не дольше  установленного времени.
		Параллельно идет чтение АЦП по I2C шине. Как только мы поймем, что перемычка перегорела, мы переводим пин в неактивное состояние.
	 */
	status += ADS1015_setup_mux(I2C4, I2C_ADS1015_ADDR, wire_num+3); // Настраиваем канал АЦП
	status += PCA9534_conf_IO_dir_output(I2C4, PCA9534_ADDR, wire_num);  // Начинаем пережигать перемычку.
	status += PCA9534_Set_output_pin(I2C4, PCA9534_ADDR, wire_num);
	if(status != SUCCESS){
		PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, wire_num);  // В случае ошибки возвращаемся к исходному состоянию
		PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);
		return ERROR_N;
	}

	TIM2_init(burn_time1);

	while(TIM_finished != 1){
		status += ADS1015_read_mVolts_int16(I2C4, I2C_ADS1015_ADDR, &meas_voltage);  // Читаем напряжение на концевиках deploy control
		if(status != SUCCESS){
			PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, wire_num);  // В случае ошибки возвращаемся к исходному состоянию
			PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);
			return ERROR_N;
		}

		if((meas_voltage > (PMM_BOTH_SWITCH_OFF - 100)) && (meas_voltage < (PMM_BOTH_SWITCH_OFF + 100))){ // Если пережгли обе перемычки, то выходим
			burn_status = 0;
			break;
		}
		else{
			burn_status = 1; // Иначе будем жечь еще раз.
		}
	}
	/*
		5.2) Если в течение установленного времени мы так и не дождались нужного значения АЦП. То повторяется
		шаг 5.1 с другим установленным временем.
	 */
	if(burn_status == 1){

		TIM2_init(burn_time2);

		while(TIM_finished != 1){
			status += ADS1015_read_mVolts_int16(I2C4, I2C_ADS1015_ADDR, &meas_voltage); // Читаем напряжение на концевиках deploy control
			if(status != SUCCESS){
				PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, wire_num);  // В случае ошибки возвращаемся к исходному состоянию
				PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);
				return ERROR_N;
			}

			if((meas_voltage > (PMM_BOTH_SWITCH_OFF - 100)) && (meas_voltage < (PMM_BOTH_SWITCH_OFF + 100))){ // Если пережгли обе перемычки, то выходим
				burn_status = 0;
				break;
			}
			else{
				burn_status = 2;
			}
		}
		if(burn_status == 2){
			// (тут можно добавить вариант с возвращением ошибки о частичном раскрытии, когда напряжение в пределах PMM_ONE_SWITCH_ON)
			status += PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);  // Прекращаем пережигать перемычку.

			Error_Handler();

			return ERROR_N;
		}
		else{ // Иначе возвращаем ошибку.
			status += PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);  // Прекращаем пережигать перемычку.
		}
	}

	return SUCCESS;
}


/** @brief	Starting deployment procedure of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Deploy(){

	int8_t status = 0;
	uint8_t inside_status = 1;
	uint8_t OUT1_pin_status = 0xFF;
	uint8_t OUT2_pin_status = 0xFF;
	uint8_t PMM_EN_SW_DP_Ch1_status, PMM_EN_SW_DP_Ch2_status, PMM_EN_SW_DP_Ch3_status, PMM_EN_SW_DP_Ch4_status = 0; // временно заменяют поля структуры

	TCA_default();

	//1) активируем пин EN_eF_Deploy_L, тем самым запитываем PCA9534 и ADS1015
	PMM_Enable_eF_Deploy_L();
	PCA_default();

	while(inside_status == 1){
		//2) После подачи питания читаем состояния концевиков OUT1 и OUT2.
		status += PMM_check_switch(PMM_OUT1, &OUT1_pin_status);
		status += PMM_check_switch(PMM_OUT2, &OUT2_pin_status);
		if(status != SUCCESS){
			return ERROR_N;
		}

		if((OUT1_pin_status == 1) || (OUT2_pin_status == 1)){ // Вышли из контейнера?
			inside_status = 0;
		}
		else{
			TIM2_init(PMM_1MIN_DELAY);	// Читаем раз в минуту.
			PMM_Disable_eF_Deploy_L();  // Во время ожидания отключаем питание логики.
			while(TIM_finished != 1){
				//WAITING 1 MINUTE
			}
			PMM_Enable_eF_Deploy_L();  // Возвращаем питание логики
		}
	}

	//Как только прочитаем состояние о том, что мы вышли из контейнера, то начинаем отсчет 30 минут.
	TIM2_init(PMM_30MIN_DELAY);
	PMM_Disable_eF_Deploy_L(); // Во время ожидания отключаем питание логики
	while(TIM_finished != 1){
		// WAITING 30 MINUTES
	}
	PMM_Enable_eF_Deploy_L();  // Возвращаем питание логики

	//3) По прошествии 30 минут нужно подать питание на силовой ключ TPS25924A, активировав пин  EN_eF_Deploy_P.
	PMM_Enable_eF_Deploy_P();

	//4) Теперь, когда весь deployment-блок запитан можно приступать к процессу развертки антенн и солнечных панелей.

	PMM_EN_SW_DP_Ch1_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch1);
	PMM_EN_SW_DP_Ch2_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch2);
	PMM_EN_SW_DP_Ch3_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch3);
	PMM_EN_SW_DP_Ch4_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch4);

	/*
	 * Тут можно заполнить структуру состояния системы, внеся туда состояния каналов.
	 */

	//6) Когда все антенны и солнечные батареи развернуты остается только
	//   убрать питание deployment-блока в низкое состояние, установив пины EN_eF_Deploy_L(P) в низкое состояние.
	status += PMM_Disable_eF_Deploy_P();
	status += PMM_Disable_eF_Deploy_L();
	if(status != SUCCESS){
		return ERROR_N;
	}

	return SUCCESS;

}
