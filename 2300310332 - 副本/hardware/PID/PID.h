#ifndef __PID_H
#define __PID_H




typedef struct
{
	 float SetPoint; /* 目标值 */ 
	 float ActualValue; /* 期望输出值 */ 
	 float SumError; /* 偏差累计 */ 
	 float Proportion; /* 比例系数 P */ 
	 float Integral; /* 积分系数 I */ 
	 float Derivative; /* 微分系数 D */ 
	 float Error; /* Error[1],第 k 次偏差 */ 
	 float LastError; /* Error[-1],第 k-1 次偏差 */ 
	 float PrevError; /* Error[-2],第 k-2 次偏差 */ 

}PID_TypeDef;

void PID__init(PID_TypeDef *pid);
float PID__ralize(PID_TypeDef *PID,float Feedback_value);
#endif
