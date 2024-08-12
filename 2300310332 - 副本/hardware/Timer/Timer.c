#include "stm32f4xx.h"                  // Device header

#include "Serial.h"



void Timer3_Init(void)//pwm输出
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA ,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB ,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;           //GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PF9
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource0,GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource1,GPIO_AF_TIM3);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;           //GPIOA9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PF9

	TIM_TimeBaseStructure.TIM_Prescaler=72-1;  //定时器分频psc
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000-1;   //自动重装载值arr
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);//初始化定时器3

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OCInitStructure.TIM_Pulse=200;//pwm占空比200
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR1上的预装载寄存器
	
    TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	
}

//测速
void EncoderA_Init(void) //1, 2, 3, 4, 5, 8, 9 or 12 to select the TIM 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;          //GPIOA6?GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //??	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //??????
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //???PA6?PA7

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);           //GPIOA6??????3??1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);           //GPIOA7??????3??2
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65535-1; 	                      
	TIM_TimeBaseStructure.TIM_Prescaler=1-1;                        
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);                  
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising );

	TIM_Cmd(TIM4,ENABLE);                                           //?????3

}
int16_t EncoderA_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp;
}




void Timer7_Init(void)//定时
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);    
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 200-1; 	                      
	TIM_TimeBaseStructure.TIM_Prescaler=7200-1;                        
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStructure); 
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM7_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM7,DISABLE);

	
}

void EncoderB_Init(void) //1, 2, 3, 4, 5, 8, 9 or 12 to select the TIM 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;          //GPIOA6?GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //??	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //???PA6?PA7

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);           //GPIOA6??????3??1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);           //GPIOA7??????3??2
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65535-1; 	                      
	TIM_TimeBaseStructure.TIM_Prescaler=1-1;                        
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);                  
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising );

	TIM_Cmd(TIM5,ENABLE);                                           //?????3

}
int16_t EncoderB_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM5);
	TIM_SetCounter(TIM5, 0);
	return Temp;
}

void EncoderC_Init(void) //1, 2, 3, 4, 5, 8, 9 or 12 to select the TIM 
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;          //GPIOA6?GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //??	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //??????
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //???PA6?PA7
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ;          //GPIOA6?GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //??	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //??????
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //???PA6?PA7

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);           //GPIOA6??????3??1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);           //GPIOA7??????3??2
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65535-1; 	                      
	TIM_TimeBaseStructure.TIM_Prescaler=1-1;                        
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);                  
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising );

	TIM_Cmd(TIM2,ENABLE);                                           //?????3

}
int16_t EncoderC_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM2);
	TIM_SetCounter(TIM2, 0);
	return Temp;
}

void EncoderD_Init(void) //1, 2, 3, 4, 5, 8, 9 or 12 to select the TIM 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;          //GPIOA6?GPIOA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                    //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;              //??100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                //??	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                  //??????
	GPIO_Init(GPIOC, &GPIO_InitStructure);                          //???PA6?PA7

    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);           //GPIOA6??????3??1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);           //GPIOA7??????3??2
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 65535-1; 	                      
	TIM_TimeBaseStructure.TIM_Prescaler=1-1;                        
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;       
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;           
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);                  
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM8, &TIM_ICInitStructure);
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising );

	TIM_Cmd(TIM8,ENABLE);                                           //?????3
}



int16_t EncoderD_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM8);
	TIM_SetCounter(TIM8, 0);
	return Temp;
}
