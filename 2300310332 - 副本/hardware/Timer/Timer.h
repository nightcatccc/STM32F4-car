#ifndef __TIMER_H__
#define __TIMER_H__

void Timer2_Init(void);
void Timer3_Init(void);
void TIM2_IRQHandler(void);
void Encoder1_Init(void);
void Encoder2_Init(void);
int16_t EncoderA_Get(void);
void Timer7_Init(void);
#endif
