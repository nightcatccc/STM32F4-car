#include "stm32f10x.h"                  // Device header
#include "PID.h"
/**************************************************************************
函数功能：位置式PID控制器
入口参数：编码器测量位置信息，目标位置
返回  值：电机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出
**************************************************************************/
void PID__init(PID_TypeDef *pid)
{
	pid-> SetPoint=150; 
	pid-> ActualValue=0;
	pid-> SumError=0; 
	pid-> Proportion=1;
	pid-> Integral=1; 
	pid-> Derivative=0.5;
	pid-> Error=0; 
	pid-> LastError=0; 
	pid-> PrevError=0; 
}

float PID__ralize(PID_TypeDef *PID,float Feedback_value)//计算pid，入口参数是实际值
{
	 PID->Error = (float)(PID->SetPoint - Feedback_value); /* 计算偏差 */ 
     PID->SumError += PID->Error; /* 累计偏差 */ 
     PID->ActualValue = (PID->Proportion * PID->Error) /* 比例环节 */ 
     + (PID->Integral * PID->SumError) /* 积分环节 */ 
     + (PID->Derivative * (PID->Error - PID->LastError)); /* 微分环节 */ 
     PID->LastError = PID->Error; /* 存储偏差，用于下次计算 */

     return ((int32_t)(PID->ActualValue)); /* 返回计算后输出的数值 */

}
