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
uint32_t msCnt;
int32_t KeyCnt = 0;

int fact_speed_A;
int fact_speed_B;
int fact_speed_C;
int fact_speed_D;



int car_status;
int count;
PID_TypeDef PID_A;
int32_t speed_A;
int32_t newspeed_A;
int32_t pwmA;
u8 string_A[15];


PID_TypeDef PID_B;
int32_t speed_B;
int32_t newspeed_B;
int32_t pwmB;


PID_TypeDef PID_C;
int32_t speed_C;
int32_t newspeed_C;
int32_t pwmC;


PID_TypeDef PID_D;
int32_t speed_D;
int32_t newspeed_D;
int32_t pwmD;

int A_status;
int B_status;
int C_status;
int D_status;

extern uint8_t RxData;
extern uint8_t RxData2;
extern uint8_t RxData3;
extern uint8_t recive2[20];
void C_f()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_RESET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);//AIN2
	C_status=1;//前进
}
void C_b()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_SET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_RESET);//AIN2
	C_status=3;//后退
}
void C_s()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_SET);//AIN1左前
	GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);//AIN2
	C_status=2;//停止
}



void A_f()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_RESET);//AIN2
	A_status=1;//前进
}
void A_b()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_RESET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_SET);//AIN2
	A_status=3;//后退
}
void A_s()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_SET);//AIN2
	A_status=2;//停止
}



void B_f()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET);//AIN2
	B_status=1;//前进
}
void B_b()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_RESET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_SET);//AIN2
	B_status=3;//后退
}
void B_s()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_SET);//AIN2
	B_status=2;//停止
}



void D_f()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_SET);//AIN1右后
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_RESET);//AIN2
	D_status=1;//前进
}
void D_b()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_RESET);//AIN1右后
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_SET);//AIN2
	D_status=3;//后退
}
void D_s()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_SET);//AIN1右后
	GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_SET);//AIN2
	D_status=2;//停止
}




void stop()
{
	TIM_Cmd(TIM7,DISABLE);
	A_s();
	B_s();
	C_s();
	D_s();
	
	car_status=2;//停止   车辆行进状态表示，用来解决前后行驶的冲突（详见中断函数）
	reflog=1;
}

void front(void){
	TIM_Cmd(TIM7,ENABLE);
	reflog=1;//每次行驶状态改变，计数器只清零，不运算，详见中断函数
	A_f();
	B_f();
	C_f();
	D_f();
}

void right(void)
{
	TIM_Cmd(TIM7,ENABLE);
	car_status=2;//右转
	reflog=1;
	A_b();
	B_f();
	C_f();
	D_b();
}
void left(void)
{
	TIM_Cmd(TIM7,ENABLE);
	car_status=3;//左转
	reflog=1;
	A_f();
	B_b();
	C_b();
	D_f();
}

void ground(void)
{
	TIM_Cmd(TIM7,ENABLE);
	car_status=5;//旋转
	reflog=1;
	A_f();
	B_b();
	C_f();
	D_b();
}

//void f_r(void)
//{
//	
//	A_s();
//	B_f();
//	C_s();
//	D_f();
//	
//}

void back()
{ 
	TIM_Cmd(TIM7,ENABLE);
	car_status=4;//后退	
	reflog=1;
	A_b();
	B_b();
	C_b();
	D_b();                                       
}

void SysTick_Handler(void)
{
  msCnt++;
}
int k=1;
int16_t Temp;
int main(void)
{   
	double yaw;
	double WZ;
	int angle;
	Serial_Init1();
	PID__init(&PID_A,200);
	PID__init(&PID_B,200);
	PID__init(&PID_C,200);
	PID__init(&PID_D,200);
	Timer3_Init();
	GPIO_init();               
	OLED_Init();
	OLED_Clear();
	EncoderA_Init();
	EncoderB_Init();
	EncoderC_Init();
	EncoderD_Init();
	Timer7_Init();
	
	Serial_Init1();
	Serial_Init2();
	Serial_Init3();

	int pid_flog_A;
     while(1)
		{	
			front() ;
			Delay_ms(5000);
			stop();
			Delay_ms(50000);
//			
//			Delay_ms(50000);
//			int Wz_16t=(recive2[5] << 8 | recive2[4]);//角速度
//			if (Wz_16t &0x8000)
//			{
//				Wz_16t-=65535;
//			}
//			WZ=(Wz_16t/32768.0)*2000.0;
//			
//			
//			int yaw_16t=(recive2[16] << 8 | recive2[15]);//航偏角
//			yaw=(yaw_16t/32768.0)*180.0;
//			if(yaw>180)
//			{
//				angle = (int)(yaw-180);
//			}
//			else
//			{
//				angle = (int)(180-yaw);
//			}
//			if(Serial_GetRxFlag2()==1)
//			{
//				printf("%f\n",WZ);
//			}
//			
//			printf("%d\n",speed_D);
//			OLED_ShowString(1,1,string_A,16);
//			OLED_ShowNumber(1,3,count,5,16);

		}
}


void TIM7_IRQHandler()//20ms定时器
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
		
		
		speed_A = EncoderB_Get();
		speed_B = EncoderC_Get();
		speed_C = EncoderA_Get();
		speed_D = EncoderD_Get();
		
		count++;
		
		
		pwmA=7*speed_A;
		pwmB=7*speed_B;
		pwmC=7*speed_C;
		pwmD=7*speed_D;
		
		fact_speed_A=(pwmA/1000)*115.2766203;
		
		
		if(reflog==0){
			if(pwmA>0&&A_status==1 ){
			GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_RESET);
			}
			else if(pwmA<0&&(A_status==3 || A_status==1)){
			GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_RESET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_SET);
			pwmA=-pwmA;
			}
			
			if(pwmB>0&&B_status==1){
			GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET);
			}
			else if(pwmB<0&&B_status==3){
			GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_RESET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_SET);
			pwmB=-pwmB;
			}
			
			if(pwmC>0&&C_status==1){
			GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_RESET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_SET);
			}
			else if(pwmC<0&&C_status==3){
			GPIO_WriteBit(GPIOF,GPIO_Pin_1,Bit_SET);
			GPIO_WriteBit(GPIOF,GPIO_Pin_2,Bit_RESET);
			pwmC=-pwmC;
			}
			
			if(pwmD>0&&D_status==1){
			GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_SET);//AIN1右后
			GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_RESET);//AIN2
			}
			else if(pwmD<0&&D_status==3){
			GPIO_WriteBit(GPIOF,GPIO_Pin_7,Bit_RESET);//AIN1右后
			GPIO_WriteBit(GPIOF,GPIO_Pin_8,Bit_SET);//AIN2
			pwmD=-pwmD;
			}
			
			newspeed_A=(int)PID__ralize(&PID_A,(float)pwmA);
			newspeed_B=(int)PID__ralize(&PID_B,(float)pwmB);
			newspeed_C=(int)PID__ralize(&PID_C,(float)pwmC);
			newspeed_D=(int)PID__ralize(&PID_D,(float)pwmD);
			
			TIM_SetCompare2(TIM3,newspeed_A);
			TIM_SetCompare3(TIM3,newspeed_B);
			TIM_SetCompare1(TIM3,newspeed_C);
			TIM_SetCompare4(TIM3,newspeed_D);
			
		}
		reflog=0;
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
