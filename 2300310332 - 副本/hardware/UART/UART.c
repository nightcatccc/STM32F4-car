#include "UART.h"
#include "debug.h"

void UART3_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

	GPIO_InitTypeDef USART1GPIO;
	USART1GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
	USART1GPIO.GPIO_Pin = GPIO_Pin_10;
	USART1GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&USART1GPIO);
	
	USART1GPIO.GPIO_Mode = GPIO_Mode_IPU;
	USART1GPIO.GPIO_Pin = GPIO_Pin_11;
	USART1GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&USART1GPIO);
	
	USART_InitTypeDef USART1Init;
	USART1Init.USART_BaudRate = 115200;
	USART1Init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1Init.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART1Init.USART_Parity = USART_Parity_No;
	USART1Init.USART_StopBits = USART_StopBits_1;
	USART1Init.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART1Init);
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVICInitStructure;
	NVICInitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVICInitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVICInitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVICInitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVICInitStructure);
	
	USART_Cmd(USART3,ENABLE);
}

void UART3_SendData(uint8_t *pt,uint16_t cnt)
{
	int16_t i;
	for(i = 0;i < cnt;i++)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
		USART_SendData(USART3,pt[i]);
	}
}


void USART3_IRQHandler(void)
{
	uint8_t a;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		a=USART_ReceiveData(USART3);
		debug_handle(&a);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
