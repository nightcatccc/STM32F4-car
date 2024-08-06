#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_TxPacket1[];
extern uint8_t Serial_RxPacket1[];
extern uint8_t ATS[6];
extern uint8_t STA[8];

void Serial_Init1(void);
void Serial_SendByte1(uint8_t Byte);
void Serial_SendArray1(uint8_t *Array, uint16_t Length);
void Serial_SendString1(char *String);

void Serial_Printf(char *format, ...);

void Serial_SendPacket1(void);
uint8_t Serial_GetRxFlag1(void);

#endif
