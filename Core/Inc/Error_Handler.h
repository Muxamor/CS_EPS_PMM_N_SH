/*
 * Error_Handler.h
 *
 *  Created on: Apr 16, 2020
 *      Author: Ivan
 */

#ifndef INC_ERROR_HANDLER_H_
#define INC_ERROR_HANDLER_H_

#if defined(USE_FULL_ASSERT)
#include "stm32_assert.h"
#endif /* USE_FULL_ASSERT */

//#define DEBUGprintf // Enable printf() message  to SWO

/*
typedef enum {
  ERROR_N = -1,
  SUCCESS = 0,
  ERROR = !SUCCESS,
} ErrorStatus; */
#define ERR_INVALID_PARAMS        	-1
#define ERR_CAN_INIT_MODE         	-2
#define ERR_CAN_NORMAL_MODE       	-3
#define ERR_CAN_NO_FREE_FILTER    	-4
#define ERR_CAN_NO_TXMAILBOXES    	-5
#define ERR_CAN_ACCESS_RANGE      	-7
#define ERR_CAN_DCL_INVALID       	-8

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif


#endif /* INC_ERROR_HANDLER_H_ */
