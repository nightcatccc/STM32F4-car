#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "GPIO.h"
#include "OLED.h"
#include "Serial.h"
#include "Timer.h"

uint32_t msCnt;
int32_t KeyCnt = 0;
int count;
void SysTick_Handler(void)
{
  msCnt++;
}
int k=1;
int16_t Temp;
int main(void)
{   
	Timer3_Init();
	GPIO_init();               
	OLED_Init();
	OLED_Clear();
	Encoder2_Init();
	Timer7_Init();
     while(1)
		{	Temp = TIM_GetCounter(TIM3);
			OLED_ShowNumber(1,4,0,5,16);
			OLED_ShowNumber(1,2,count,5,16);
			GPIO_WriteBit(GPIOG,GPIO_Pin_8,Bit_SET);
		}
}


void TIM7_IRQHandler()
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		count++;
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
