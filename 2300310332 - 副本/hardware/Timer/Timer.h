#ifndef __TIMER_H__
#define __TIMER_H__

void Timer2_Init(void);
void Timer3_Init(void);
void TIM2_IRQHandler(void);
void Timer_setcompare1(uint16_t CRR);
void Timer_setcompare2(uint16_t CRR);
void Encoder1_Init(void);
void Encoder2_Init(void);
int16_t Encoder_1Get(void);
int16_t Encoder_2Get(void);
void Timer7_Init(void);
#endif
