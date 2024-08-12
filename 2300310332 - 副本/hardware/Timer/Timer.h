#ifndef __TIMER_H__
#define __TIMER_H__

void Timer2_Init(void);
void Timer3_Init(void);
void TIM2_IRQHandler(void);
void EncoderA_Init(void);
void EncoderB_Init(void);
void EncoderC_Init(void);
void EncoderD_Init(void);
int16_t EncoderA_Get(void);
int16_t EncoderB_Get(void);
int16_t EncoderC_Get(void);
int16_t EncoderD_Get(void);
void Timer7_Init(void);
#endif
