#ifndef __SERIAL3_H
#define __SERIAL3_H

#include <stdio.h>

//extern uint8_t Serial_TxPacket1[];
//extern uint8_t ATS[6];
//extern uint8_t STA[8];
extern uint8_t RxData3;

void Serial_Init3(void);
void Serial_SendByte3(uint8_t Byte);
void Serial_SendArray3(uint8_t *Array, uint16_t Length);
void Serial_SendString3(char *String);

//void Serial_Printf(char *format, ...);

void Serial_SendPacket3(void);
uint8_t Serial_GetRxFlag3(void);

#endif
