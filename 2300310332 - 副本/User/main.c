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

void stop()
{
	TIM_Cmd(TIM7,DISABLE);
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_SET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);//AIN2
	car_status=2;//停止   车辆行进状态表示，用来解决前后行驶的冲突（详见中断函数）
	reflog=1;
}

void front(void){
	TIM_Cmd(TIM7,ENABLE);
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_RESET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);//AIN2
	car_status=1;//前进
	reflog=1;//每次行驶状态改变，计数器只清零，不运算，详见中断函数
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_RESET);//AIN2
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET);//AIN2
	
	GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_SET);//AIN1右后
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_RESET);//AIN2
}

void back()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_SET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_RESET);//AIN2
	car_status=3;//后退	
	reflog=1;
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
	front();
	Serial_Init1();
	Serial_Init2();
	Serial_Init3();
	Serial_SendByte1(0x01);
     while(1)
		{	
//			printf("%d\n",pwmA);
			if(Serial_GetRxFlag2()==1)
			{
				Serial_SendByte2(RxData2);
			}
			
			OLED_ShowString(1,1,string_A,16);
			
			OLED_ShowNumber(1,3,count,5,16);

//			Delay_ms(2000);
//			stop();
//			Delay_ms(2000);
			
//			front();
		}
}


void TIM7_IRQHandler()
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		speed_A = EncoderA_Get();
		count++;
		pwmA=7*speed_A;
		if(reflog==0){
			if(pwmA>0&&car_status==1){
			GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_RESET);//AIN1左前
			GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);//AIN2
			}
			else if(pwmA<0&&car_status==3){
			GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_SET);//AIN1左前
			GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_RESET);//AIN2
			pwmA=-pwmA;
			}
			newspeed_A=(int)PID__ralize(&PID_A,(float)pwmA);
			
			TIM_SetCompare1(TIM3,newspeed_A);
		}
		reflog=0;
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
