/****************************************************************/
/** @file:      cDebugUart
 *  @author:	DO HOANG DUY KHIEM
 *  @date:		Nov 29, 2015
 *  @version:	1.0
 *  @brief:     UARTDebug Interface, it includes.
 * 		_UartDebugConfigX
 * 		_UartPrintf
 * 		_ReadDebugUart
 *  @note:       
 *		this driver dont use DMA for rx. DMA comes only handy when the communication has some certain protocol like canbux
 *		send maximum msg sending speed
 *			TX_BUFFER_SIZE=512 and burst=1msg
 *			TX_BUFFER_SIZE=16  and burst=32msg
 *
 ****************************************************************/
#ifndef _cDebugUart_h
#define _cDebugUart_h
#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"

#define DEBUGUART_ID 	6

#if DEBUGUART_ID == 6
	#define DEBUGUART_ADDR	USART6
#elif DEBUGUART_ID == 1
	#define DEBUGUART_ADDR	USART1
#elif DEBUGUART_ID == 3
	#define DEBUGUART_ADDR	USART3
#elif DEBUGUART_ID == 2
	#define DEBUGUART_ADDR	USART2
#endif

void UartDebugConfig0(void);
UART_HandleTypeDef* GetUartDebugPtr(void);
int32_t UDebugPrintf(char * data, ...);
int32_t UDebugSendRaw(int8_t * data, int32_t lengthOfMsg);
int32_t UDebugSimplePrintf(int8_t * data, int32_t lengthOfMsg);
int8_t ReadDebugUart(void);
void HAL_UARTDebug_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTDebug_TxCpltCallback(UART_HandleTypeDef *huart);
void UARTDebug_ControllerLoop(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif
#endif
