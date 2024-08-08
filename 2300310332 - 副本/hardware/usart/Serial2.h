#ifndef __SERIAL2_H
#define __SERIAL2_H

#include <stdio.h>

//extern uint8_t Serial_TxPacket1[];
//extern uint8_t Serial_RxPacket1[];
//extern uint8_t ATS[6];
//extern uint8_t STA[8];

void Serial_Init2(void);
void Serial_SendByte2(uint8_t Byte);
void Serial_SendArray2(uint8_t *Array, uint16_t Length);
void Serial_SendString2(char *String);

//void Serial_Printf(char *format, ...);

void Serial_SendPacket2(void);
uint8_t Serial_GetRxFlag2(void);

#endif
