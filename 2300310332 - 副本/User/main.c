#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "GPIO.h"
#include "OLED.h"
#include "Serial.h"
#include "Timer.h"
#include "PID.h"

uint32_t msCnt;
int32_t KeyCnt = 0;
int count;
PID_TypeDef PID_A;
int32_t speed_A;
int32_t newspeed_A;
u8 string_A[15];
int32_t pwmA;



void car_init(void){
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_RESET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);//AIN2
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_RESET);//AIN2
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET);//AIN2
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_SET);//AIN1右后
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_RESET);//AIN2
}

void SysTick_Handler(void)
{
  msCnt++;
}
int k=1;
int16_t Temp;
int main(void)
{   
	Serial_Init1();
	PID__init(&PID_A,200);
	Timer3_Init();
	GPIO_init();               
	OLED_Init();
	OLED_Clear();
	Encoder2_Init();
	Timer7_Init();
	car_init();
	
     while(1)
		{	
			if(count>=50){
			printf("%d\n",speed_A);
				count=0;
			}
			OLED_ShowString(1,1,string_A,16);
			
			OLED_ShowNumber(1,3,count,5,16);
		}
}


void TIM7_IRQHandler()
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		speed_A = EncoderA_Get();
		count++;
		newspeed_A=(int)PID__ralize(&PID_A,(float)speed_A);
		TIM_SetCompare1(TIM3, newspeed_A);
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
