#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"                  // Device header

void UART3_Init(void);
void UART3_SendData(uint8_t *pt,uint16_t cnt);

#endif
