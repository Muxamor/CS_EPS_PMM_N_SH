#include "pmm_deploy.h"
#include "TCA9539.h"
#include "PCA9534.h"
#include "ADS1015.h"
#include "TIM_delay.h"
#include "Error_Handler.h"

#define I2C_ADS1015_ADDR		0x48
#define PCA9534_ADDR			0x38
#define TCA9539_I2C_ADDR		0x74

#define PMM_BOTH_SWITCH_OFF		1276
#define PMM_BOTH_SWITCH_ON		0
#define PMM_ONE_SWITCH_ON		572 //976

#define PMM_BURN_TIME1			1000  // millisec
#define PMM_BURN_TIME2			2000  // millisec

#define PMM_EN_SW_DP_Ch1		0x01
#define PMM_EN_SW_DP_Ch2		0x02
#define PMM_EN_SW_DP_Ch3		0x04
#define PMM_EN_SW_DP_Ch4		0x08

#define PMM_OUT1				0x20
#define PMM_OUT2				0x40

#define PMM_30MIN_DELAY			1800000  // millisec
#define PMM_1MIN_DELAY			60000  // millisec

#define Check_error_status		if(status != SUCCESS){\
									Error_Handler();\
									PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);\
									TCA9539_conf_IO_dir_input(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10 | TCA9539_IO_P11);\
									return ERROR_N;\
								}


/** @brief	Setting default values for all registers of PCA9534.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PCA_default(){

	int8_t status = 0;

	status += PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);
	status += PCA9534_Set_output_pin(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);
	status += PCA9534_conf_IO_pol_normal(I2C4, PCA9534_ADDR, PCA9534_IO_ALL);

	Check_error_status;

	return SUCCESS;
}


/** @brief	Setting default values for all registers of TCA9539.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus TCA_default(){

	int8_t status = 0;

	status += TCA9539_conf_IO_dir_input(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10 | TCA9539_IO_P11);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10 | TCA9539_IO_P11);
	status += TCA9539_conf_IO_pol_normal(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10 | TCA9539_IO_P11);

	Check_error_status;

	return SUCCESS;
}


/** @brief	Power supply to logic block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Enable_eF_Deploy_L(){

	int8_t status = 0;

	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10);

	Check_error_status;

	LL_mDelay(50); // нужно время для установки питания

	return SUCCESS;
}

/** @brief	Power supply to power block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Enable_eF_Deploy_P(){

	int8_t status = 0;

	status += TCA9539_conf_IO_dir_output(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P11);
	status += TCA9539_Set_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P11);

	Check_error_status;

	LL_mDelay(50); // нужно время для установки питания

	return SUCCESS;
}


/** @brief	Power cut to logic block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Disable_eF_Deploy_L(){

	int8_t status = 0;

	status += TCA9539_Reset_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P10);

	Check_error_status;

	return SUCCESS;
}


/** @brief	Power cut to power block of PMM.
	@retval 0-OK, -1-ERROR_N
*/
ErrorStatus PMM_Disable_eF_Deploy_P(){

	int8_t status = 0;

	status += TCA9539_Reset_output_pin(I2C3, TCA9539_I2C_ADDR, TCA9539_IO_P11);

	Check_error_status;

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

	status += PCA9534_read_input_pin(I2C4, PCA9534_ADDR, switch_num, state);

	Check_error_status;

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

	/*
	  	5.1) The pin is put into an active state and is in it no longer than the set time. In parallel,
	  	the ADC is reading on the I2C bus. As soon as we understand that the jumper is burnt out, we put the pin in an inactive state.
	*/
	status += ADS1015_setup_mux(I2C4, I2C_ADS1015_ADDR, wire_num+3); // Configure the ADC channel
	status += PCA9534_conf_IO_dir_output(I2C4, PCA9534_ADDR, wire_num);  // Start to burn the jumper.
	status += PCA9534_Set_output_pin(I2C4, PCA9534_ADDR, wire_num);

	if(status != SUCCESS){
		PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, wire_num);
		PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);
		return ERROR_N;
	}

	TIM2_init(burn_time1);

	while(TIM_finished != 1){
		status += ADS1015_read_mVolts_int16(I2C4, I2C_ADS1015_ADDR, &meas_voltage);  // read the voltage on the limit switches of "deploy control"
		if(status != SUCCESS){
			PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, wire_num);  //  In case of an error, we return to the initial state
			PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);
			return ERROR_N;
		}

		if((meas_voltage > (PMM_BOTH_SWITCH_OFF - 100)) && (meas_voltage < (PMM_BOTH_SWITCH_OFF + 100))){ // Если пережгли обе перемычки, то выходим
			burn_status = 0;
			break;
		}
		else{
			burn_status = 1; // Otherwise, we will burn again.
		}
	}
	/*
		5.2) If during the set time we still have not reached the desired ADC value. Then step 5.1 is repeated with a different set time.
	 */
	if(burn_status == 1){

		TIM2_init(burn_time2);

		while(TIM_finished != 1){
			status += ADS1015_read_mVolts_int16(I2C4, I2C_ADS1015_ADDR, &meas_voltage); // read the voltage on the limit switches of "deploy control"
			if(status != SUCCESS){
				PCA9534_conf_IO_dir_input(I2C4, PCA9534_ADDR, wire_num);  // In case of an error, we return to the initial state
				PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);
				return ERROR_N;
			}

			// If both jumpers are burned, then exit
			if((meas_voltage > (PMM_BOTH_SWITCH_OFF - 100)) && (meas_voltage < (PMM_BOTH_SWITCH_OFF + 100))){
				burn_status = 0;
				break;
			}
			else{
				burn_status = 2;
			}
		}
		if(burn_status == 2){
			// (here we can add an option with the return of a partial disclosure error when the voltage is within PMM_ONE_SWITCH_ON)
			Error_Handler();

			PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);  // Stop cutting the jumper.

			return ERROR_N;
		}
		else{ //  Otherwise, return an error.
			Error_Handler();

			PCA9534_Reset_output_pin(I2C4, PCA9534_ADDR, wire_num);  // Stop cutting the jumper.

			return ERROR_N;
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

	status += TCA_default();

	//1) activate the pin EN_eF_Deploy_L, thereby powering up PCA9534 and ADS1015
	status += PMM_Enable_eF_Deploy_L();
	status += PCA_default();
	Check_error_status;

	while(inside_status == 1){
		//2) After applying power, read the status of the end switches OUT 1 and OUT2.
		status += PMM_check_switch(PMM_OUT1, &OUT1_pin_status);
		status += PMM_check_switch(PMM_OUT2, &OUT2_pin_status);
		Check_error_status;

		if((OUT1_pin_status == 1) || (OUT2_pin_status == 1)){ // Outside the container?
			inside_status = 0;
		}
		else{
			TIM2_init(PMM_1MIN_DELAY);	// Read once a minute.
			status += PMM_Disable_eF_Deploy_L();  // While waiting, turn off the power to the logic.
			Check_error_status;
			while(TIM_finished != 1){
				//WAITING 1 MINUTE
			}
			status += PMM_Enable_eF_Deploy_L();  // Return logic power
			Check_error_status;
		}
	}

	//	As soon as we read the state that we have left the container, we begin the countdown of 30 minutes.
	TIM2_init(PMM_30MIN_DELAY);

	status += PMM_Disable_eF_Deploy_L(); // While waiting, turn off the power of the logic
	Check_error_status;
	while(TIM_finished != 1){
		// WAITING 30 MINUTES
	}
	status += PMM_Enable_eF_Deploy_L();  // Return logic power

	//3) After 30 minutes, need to supply power to the TPS25924A power switch by activating the EN_eF_Deploy_P pin.
	status += PMM_Enable_eF_Deploy_P();
	Check_error_status;

	//4) Now that the entire deployment unit has been powered up, we can begin the process of deployment antennas and solar panels.
	PMM_EN_SW_DP_Ch1_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch1);
	PMM_EN_SW_DP_Ch2_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch2);
	PMM_EN_SW_DP_Ch3_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch3);
	PMM_EN_SW_DP_Ch4_status = PMM_burn_the_wire(PMM_BURN_TIME1, PMM_BURN_TIME2, PMM_EN_SW_DP_Ch4);

	/*
	 * Here you can fill in the structure of the state of the system, entering the state of the channels there.
	 */

	//6) When all antennas and solar panels are deployed, it remains only to remove the power of
	//the deployment block to a low state by setting the pins EN_eF_Deploy_L (P) to a low state.
	status += PMM_Disable_eF_Deploy_P();
	status += PMM_Disable_eF_Deploy_L();
	Check_error_status;

	return SUCCESS;

}
