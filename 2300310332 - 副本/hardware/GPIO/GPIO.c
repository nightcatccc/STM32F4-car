#include "GPIO.h"
#include "stm32f4xx.h"
void GPIO_init()
{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
			
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOG,&GPIO_InitStructure);
	
}
