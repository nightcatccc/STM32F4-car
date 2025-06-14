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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int change_flog;

int reflog;
uint32_t msCnt;
int32_t KeyCnt = 0;

int fact_speed_A;
int fact_speed_B;
int fact_speed_C;
int fact_speed_D;

int car_odomtery_dif;
int car_odomtery_last;
int car_odomtery_now;



int car_status;
int count;
PID_TypeDef PID_A;
int32_t speed_A;
int32_t newspeed_A;
float pwmA;
u8 string_A[15];


PID_TypeDef PID_B;
int32_t speed_B;
int32_t newspeed_B;
float pwmB;


PID_TypeDef PID_C;
int32_t speed_C;
int32_t newspeed_C;
float pwmC;


PID_TypeDef PID_D;
int32_t speed_D;
int32_t newspeed_D;
float pwmD;

int A_status;
int B_status;
int C_status;
int D_status;

int k=1;
int16_t Temp;
int flag = 0;
int flag_2 = 0;
int flag_3 = 0;
int flag_4 = 0;

int timer_flog;

extern uint8_t RxData;
extern uint8_t RxData2;
extern uint8_t recive2[20];
extern uint8_t send1[11];

int state_yet;

void send_arr_init(void)
{
	send1[0]=0x88;
	send1[1]=0x66;
	send1[2]=0x00;//脉冲数
	send1[3]=0x00;
	send1[4]=0x00;//行驶方向
	send1[5]=0x00;//yaw
	send1[6]=0x00;
	send1[7]=0x00;//angluar.z
	send1[8]=0x00;
	send1[9]=0x75;
	send1[10]=0x79;
}

void send_arr_update(void)
{
	if(count>=20)
			{

				car_odomtery_dif=car_odomtery_now;
				if(car_status==4){
					car_odomtery_dif=-car_odomtery_dif;
				}
				send1[2]=car_odomtery_dif/256;
				send1[3]=car_odomtery_dif%256;
				send1[4]=car_status;
				send1[5]=recive2[15];
				send1[6]=recive2[16];
				send1[7]=recive2[4];
				send1[8]=recive2[5];
				car_odomtery_now=0;
				count=0;
				Serial_SendArray1(send1,11);
			}
}

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
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_RESET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_SET);//AIN2
	A_status=1;//前进
}
void A_b()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);//AIN1右前
	GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_RESET);//AIN2
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
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_RESET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_SET);//AIN2
	B_status=1;//前进
}
void B_b()
{
	GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);//AIN1左后
	GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET);//AIN2
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
	timer_flog=1;
	car_status=0;//停止   车辆行进状态表示
	if (state_yet!=car_status){
	reflog=1;
	}
	A_s();
	Delay_ms(500);	
	B_s();
	C_s();
	D_s();
	
	
	state_yet=car_status;
}

void stop_2()
{
	TIM_Cmd(TIM7,DISABLE);
	timer_flog=1;
	car_status=0;//停止   车辆行进状态表示
	if (state_yet!=car_status){
	reflog=1;
	}
	A_s();
	B_s();
	C_s();
	D_s();
	
	
	state_yet=car_status;
}

void front(void){
	TIM_SetCompare1(TIM3,150);
	TIM_SetCompare2(TIM3,150);
	TIM_SetCompare3(TIM3,150);
	TIM_SetCompare4(TIM3,150);
	Delay_ms(80);
	PID__init(&PID_A,150,1.4,0.1,1.2);
	PID__init(&PID_B,150,1.4,0.1,1.2);
	PID__init(&PID_C,150,1.4,0.1,1.2);
	PID__init(&PID_D,150,1.4,0.1,1.2);
	
	timer_flog=0;
	
	car_status=1;//前进
	if (state_yet!=car_status){
	reflog=1;
	}
	A_f();
	B_f();
	C_f();
	D_f();
	TIM_Cmd(TIM7,ENABLE);
}

void right(void)
{
	
	TIM_SetCompare2(TIM3,81);
	TIM_SetCompare3(TIM3,80);
	TIM_SetCompare1(TIM3,79);
	TIM_SetCompare4(TIM3,80);
	Delay_ms(80);
	PID__init(&PID_A,81,1.4,0.1,1.2);
	PID__init(&PID_B,80,1.4,0.1,1.2);
	PID__init(&PID_C,79,1.4,0.1,1.2);
	PID__init(&PID_D,80,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=2;//右转
	if (state_yet!=car_status){
	reflog=1;
	}
		
	B_f();
	A_b();
	C_f();
	D_b();
	state_yet=car_status;
}

void left(void)
{
	TIM_SetCompare1(TIM3,81);
	TIM_SetCompare2(TIM3,80);
	TIM_SetCompare3(TIM3,79);
	TIM_SetCompare4(TIM3,80);
	Delay_ms(80);
	PID__init(&PID_A,81,1.4,0.1,1.2);
	PID__init(&PID_B,80,1.4,0.1,1.2);
	PID__init(&PID_C,79,1.4,0.1,1.2);
	PID__init(&PID_D,80,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	
	timer_flog=0;
	car_status=3;//左转
	if (state_yet!=car_status){
	reflog=1;
	}
	
	B_b();
	A_f();
	C_b();
	D_f();
	state_yet=car_status;
}

void ground_left(void)
{
	
	TIM_SetCompare2(TIM3,80);
	TIM_SetCompare3(TIM3,80);
	TIM_SetCompare3(TIM3,79);
	TIM_SetCompare4(TIM3,80);
	Delay_ms(80);
	PID__init(&PID_A,80,1.4,0.1,1.2);
	PID__init(&PID_B,80,1.4,0.1,1.2);
	PID__init(&PID_C,80,1.4,0.1,1.2);
	PID__init(&PID_D,80,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=5;//旋转
	if (state_yet!=car_status){
	reflog=1;
	}
	A_f();
	B_b();
	C_f();
	D_b();
	state_yet=car_status;
}




void ground_left_weitiao(void)//微调时使用，未测试过
{
	
	TIM_SetCompare2(TIM3,30);
	TIM_SetCompare3(TIM3,30);
	TIM_SetCompare3(TIM3,30);
	TIM_SetCompare4(TIM3,30);
	Delay_ms(20);
	PID__init(&PID_A,30,1.4,0.1,1.2);
	PID__init(&PID_B,30,1.4,0.1,1.2);
	PID__init(&PID_C,30,1.4,0.1,1.2);
	PID__init(&PID_D,30,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=5;//旋转
	if (state_yet!=car_status){
	reflog=1;
	}
	A_f();
	B_b();
	C_f();
	D_b();
	state_yet=car_status;
}




void ground_right(void)
{
	
	TIM_SetCompare2(TIM3,80);
	TIM_SetCompare3(TIM3,80);
	TIM_SetCompare3(TIM3,79);
	TIM_SetCompare4(TIM3,80);
	Delay_ms(80);
	PID__init(&PID_A,80,1.4,0.1,1.2);
	PID__init(&PID_B,80,1.4,0.1,1.2);
	PID__init(&PID_C,80,1.4,0.1,1.2);
	PID__init(&PID_D,80,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=5;//旋转
	if (state_yet!=car_status){
	reflog=1;
	}
	A_b();
	B_f();
	C_b();
	D_f();
	state_yet=car_status;
}

void ground_right_weitiao(void)//微调时使用，未测试过
{
	
	TIM_SetCompare2(TIM3,30);
	TIM_SetCompare3(TIM3,30);
	TIM_SetCompare3(TIM3,30);
	TIM_SetCompare4(TIM3,30);
	Delay_ms(20);
	PID__init(&PID_A,30,1.4,0.1,1.2);
	PID__init(&PID_B,30,1.4,0.1,1.2);
	PID__init(&PID_C,30,1.4,0.1,1.2);
	PID__init(&PID_D,30,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=5;//旋转
	if (state_yet!=car_status){
	reflog=1;
	}
	A_b();
	B_f();
	C_b();
	D_f();
	state_yet=car_status;
}

void back()
{ 
	
	TIM_SetCompare2(TIM3,150);
	TIM_SetCompare3(TIM3,149);
	TIM_SetCompare1(TIM3,150);
	TIM_SetCompare4(TIM3,149);
	Delay_ms(80);
	PID__init(&PID_A,150,1.4,0.1,1.2);
	PID__init(&PID_B,149,1.4,0.1,1.2);
	PID__init(&PID_C,150,1.4,0.1,1.2);
	PID__init(&PID_D,149,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=4;//后退	
	if (state_yet!=car_status){
		reflog=1;
	}
	A_b();
	B_b();
	C_b();
	D_b();  
	state_yet=car_status;	
}

int state_chg;
int angle;
int get_yaw(void)
{
	int angle_t;
	double yaw;
	int yaw_16t=(recive2[16] << 8 | recive2[15]);//航偏角
			yaw=(yaw_16t/32768.0)*180.0;
			if(yaw>180)
			{
				angle = (int)(yaw-180)-180;
			}
			else
			{
				angle = 180-(int)(180-yaw);
			}
		angle_t=angle;
	return angle_t;
}

double get_wz(void)
{
	double WZ;
	int Wz_16t=(recive2[5] << 8 | recive2[4]);//角速度
			if (Wz_16t &0x8000)
			{
				Wz_16t-=65535;
			}
	WZ=(Wz_16t/32768.0)*2000.0;
	return WZ;	
}



void ground_left_to_0(void)
{
		TIM_SetCompare2(TIM3,30);
		TIM_SetCompare3(TIM3,30);
		TIM_SetCompare3(TIM3,30);
		TIM_SetCompare4(TIM3,30);
		Delay_ms(80);
		PID__init(&PID_A,30,1.4,0.1,1.2);
		PID__init(&PID_B,30,1.4,0.1,1.2);
		PID__init(&PID_C,30,1.4,0.1,1.2);
		PID__init(&PID_D,30,1.4,0.1,1.2);
		TIM_Cmd(TIM7,ENABLE);
		timer_flog=0;
		car_status=5;//旋转
		if (state_yet!=car_status){
		reflog=1;
		}
		A_f();
		B_b();
		C_f();
		D_b();
		state_yet=car_status;
		while(1){
				if(Serial_GetRxFlag2()==1)
				{
					//printf("%d\n",angle);
					if(-angle<3&&angle<0)break;
					//if(angle+180<2&&angle<0)break;
				}
			}
		stop_2();
}
void ground_right_to_0(void)
{
	
	TIM_SetCompare2(TIM3,30);
	TIM_SetCompare3(TIM3,30);
	TIM_SetCompare3(TIM3,30);
	TIM_SetCompare4(TIM3,30);
	Delay_ms(80);
	PID__init(&PID_A,30,1.4,0.1,1.2);
	PID__init(&PID_B,30,1.4,0.1,1.2);
	PID__init(&PID_C,30,1.4,0.1,1.2);
	PID__init(&PID_D,30,1.4,0.1,1.2);
	TIM_Cmd(TIM7,ENABLE);
	timer_flog=0;
	car_status=5;//旋转
	if (state_yet!=car_status){
	reflog=1;
	}
	A_b();
	B_f();
	C_b();
	D_f();
	state_yet=car_status;
	while(1){
				if(Serial_GetRxFlag2()==1)
				{
					//printf("%d\n",angle);
					if(angle-0<3&&angle>0)break;
					//if(angle+180<2&&angle<0)break;
				}
			}
	stop_2();
}

void to_0(void)
{
	if(Serial_GetRxFlag2()==1)
				{
					//printf("%d\n",angle);
					if(angle>1){
						ground_right_to_0();
					}
					else if(angle<-1){
						ground_left_to_0();
					}
					//if(angle+180<2&&angle<0)break;
				}
}














void ground_left_to_180(void)
{
		TIM_SetCompare2(TIM3,30);
		TIM_SetCompare3(TIM3,30);
		TIM_SetCompare3(TIM3,30);
		TIM_SetCompare4(TIM3,30);
		Delay_ms(80);
		PID__init(&PID_A,30,1.4,0.1,1.2);
		PID__init(&PID_B,30,1.4,0.1,1.2);
		PID__init(&PID_C,30,1.4,0.1,1.2);
		PID__init(&PID_D,30,1.4,0.1,1.2);
		TIM_Cmd(TIM7,ENABLE);
		timer_flog=0;
		car_status=5;//旋转
		if (state_yet!=car_status){
		reflog=1;
		}
		A_f();
		B_b();
		C_f();
		D_b();
		state_yet=car_status;
		while(1){
				if(Serial_GetRxFlag2()==1)
				{
					printf("%d\n",angle);
					if(180-angle<4&&angle>0)break;
					//if(angle+180<2&&angle<0)break;
				}
				Delay_ms(100);
			}
			stop_2();
}


void SysTick_Handler(void)
{
  msCnt++;
}

int main(void)
{   
	int yaw;
	uint8_t yaw_h;
	uint8_t yaw_l;
	Serial_Init1();
	Timer3_Init();
	GPIO_init();               
	OLED_Init();
	OLED_Clear();
	EncoderA_Init();
	EncoderB_Init();
	EncoderC_Init();
	EncoderD_Init();
	Timer7_Init();
	stop();
	Serial_Init1();
	Serial_Init2();
	Serial_Init3();
	send_arr_init();
	Delay_ms(500);
//	Serial_SendString3("ok");
//	Delay_ms(20000);
	front();

     while(1)
		{	
			
			if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7) == 1 && flag == 0)
			{
				stop_2();
				Delay_ms(1000);
				left ();
				Delay_ms(3950);
				stop_2();
				Delay_ms(500);
				Serial_SendString3("ok");
				while(1)
				{
					if(flag_2 == 0 )
					{
						//printf("to_0 is ERROR\n");
						to_0();
						//printf("to_0 is current\n");
						Delay_ms(500);
						Serial_SendString3("or");
						Delay_ms(15000);
						Delay_ms(10000);
						flag_2 = 1;
//						if(RxData3 == '1')
//						{
//							ground_left();
//							Delay_ms(400);
//							stop_2();
//							flag_2 = 1;
//							Serial_SendString3("or");
//							Delay_ms(10000);
//							Delay_ms(10000);
//							Delay_ms(10000);
//							Delay_ms(10000);
//							Delay_ms(10000);
//						}
//						if(RxData3 == '2')
//						{
//							ground_right();
//							Delay_ms(400);
//							stop_2();
//							flag_2 = 1;
//							Serial_SendString3("or");
//							Delay_ms(10000);
//							Delay_ms(10000);
//							Delay_ms(10000);
//							Delay_ms(10000);
//							Delay_ms(10000);
//						}
					}
					if(flag_2 == 1)
					{
						ground_left_to_180();
						Delay_ms(500);
						front();
						while(1)
						{
							if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7) == 1 && flag_3 == 0)
							{
								stop_2();
								flag_3 = 1;
								Delay_ms(500);
								Serial_SendString3("oc");
								
								while(1)
								{
									if(flag_4 == 0 && Serial_GetRxFlag3()==1)
									{
										if(RxData3 == '0')
										{
											flag_4 = 1;
											Serial_SendString3("or");
											Delay_ms(10000);
										}
										if(RxData3 == '1')
										{
											right();
											Delay_ms(300);
											stop_2();
											flag_4 = 1;
											Serial_SendString3("or");
											Delay_ms(10000);
										}
										if(RxData3 == '2')
										{
											right();
											Delay_ms(600);
											stop_2();
											flag_4 = 1;
											Serial_SendString3("or");
											Delay_ms(10000);
										}
										
										if(RxData3 == '3')
										{
											left();
											Delay_ms(300);
											stop_2();
											flag_4 = 1;
											Serial_SendString3("or");
											Delay_ms(10000);
										}
										if(RxData3 == '4')
										{
											left();
											Delay_ms(600);
											stop_2();
											flag_4 = 1;
											Serial_SendString3("or");
											Delay_ms(10000);
										}
									}
								}
							}
						}
					}
				}
			}
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
//				ground_left();
//				Delay_ms(5850);
//				stop_2();
//				Delay_ms(500);
//				front();
//				
//				flag = 1;
//				Serial_SendByte3(0x11);
		}
}


void TIM7_IRQHandler()//20ms定时器
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET){
			//printf("%d,%d,%d,%d\n",speed_A,speed_B,speed_C,speed_D);
			count++;
			speed_A = EncoderB_Get();
			speed_B = EncoderC_Get();
			speed_C = EncoderA_Get();
			speed_D = EncoderD_Get();
			
			car_odomtery_last=(speed_A+speed_B+speed_C+speed_D)/4;
			car_odomtery_now+=car_odomtery_last;
			
			pwmA=(-speed_A)*2.73;
			pwmB=(speed_B)*2.73;
			pwmC=(speed_C)*2.73;
			pwmD=(-speed_D)*2.73;
			
			
			if(reflog==0){
				if(pwmA>0&&A_status==1 ){
				GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_RESET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_SET);
					
				}
				else if(pwmA<0&&A_status==3){
				GPIO_WriteBit(GPIOF,GPIO_Pin_3,Bit_SET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_4,Bit_RESET);
				pwmA=-pwmA;
				}
				
				if(pwmB>0&&B_status==1){
				GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_RESET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_SET);
				}
				else if(pwmB<0&&B_status==3){
				GPIO_WriteBit(GPIOF,GPIO_Pin_5,Bit_SET);
				GPIO_WriteBit(GPIOF,GPIO_Pin_6,Bit_RESET);
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
//				
				newspeed_A=(int)PID__ralize(&PID_A,(float)pwmA);
				newspeed_B=(int)PID__ralize(&PID_B,(float)pwmB);
				newspeed_C=(int)PID__ralize(&PID_C,(float)pwmC);
				newspeed_D=(int)PID__ralize(&PID_D,(float)pwmD);
				
				TIM_SetCompare2(TIM3,newspeed_A);
				TIM_SetCompare3(TIM3,newspeed_B);
				TIM_SetCompare1(TIM3,newspeed_C);
				TIM_SetCompare4(TIM3,newspeed_D);
//				
			}
			reflog=0;
			
		
		
		
		
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
