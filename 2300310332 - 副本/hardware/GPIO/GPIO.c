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
			
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
			
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOC,&GPIO_InitStructure);
			
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
			
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
			GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;//0(stby)1(AIN1)2(AIN2)
			GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
			GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
			GPIO_Init(GPIOF,&GPIO_InitStructure);
}
