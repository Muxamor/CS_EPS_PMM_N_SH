/*
 * DWT.c
 *
 *  Created on: May 2, 2020
 *      Author: ftor
 */

#include "DWT.h"

#define DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC

#define DWT_CLK_MILLI_DEVIDER		79999
#define VAR1
void DWT_start(){
	SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT
	DWT_CONTROL |= DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик
	DWT_CYCCNT = 0;// обнуляем счётчик
}

void DWT_stop(){
	SCB_DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT
	DWT_CONTROL &= ~DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик
}

uint32_t DWT_get_clk_tick(){
	uint32_t count_tick = DWT_CYCCNT;
	return count_tick;
}

uint32_t DWT_get_milli_tick(){
	uint32_t count_tick = DWT_CYCCNT;
	return count_tick / DWT_CLK_MILLI_DEVIDER;
}

void DWT_reset_timer(){
	DWT_CYCCNT = 0; // обнуляем счётчик
}

void DWT_delay(uint32_t delay){

	DWT_start();
	uint32_t clk_tick_counter = 0;

#ifdef VAR1
	uint32_t millisec_counter = 0;

	while(millisec_counter < delay){
		clk_tick_counter = DWT_get_clk_tick();
		if(clk_tick_counter >= DWT_CLK_MILLI_DEVIDER){
			millisec_counter++;
			DWT_reset_timer();
		}
	}
#else
	while(clk_tick_counter < delay * DWT_CLK_MILLI_DEVIDER){
		clk_tick_counter = DWT_get_clk_tick();
	}
	DWT_reset_timer();
#endif

//	DWT_stop();
}

