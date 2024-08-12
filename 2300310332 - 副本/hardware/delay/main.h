#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "GPIO.h"
#include "OLED.h"
#include "Serial.h"
#include "Timer.h"
#include "PID.h"
#include "Delay.h"
#include "Serial2.h"
#include "Serial3.h"

int reflog;
int car_status;
uint32_t msCnt;
int32_t KeyCnt = 0;
int count;
PID_TypeDef PID_A;
int32_t speed_A;
int32_t newspeed_A;
u8 string_A[15];
int32_t pwmA;
extern uint8_t RxData;
extern uint8_t RxData2;
extern uint8_t RxData3;
extern uint8_t recive2[20];

#endif
