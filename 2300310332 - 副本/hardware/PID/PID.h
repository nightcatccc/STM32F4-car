#ifndef __PID_H
#define __PID_H




typedef struct
{
	 float SetPoint; /* Ŀ��ֵ */ 
	 float ActualValue; /* �������ֵ */ 
	 float SumError; /* ƫ���ۼ� */ 
	 float Proportion; /* ����ϵ�� P */ 
	 float Integral; /* ����ϵ�� I */ 
	 float Derivative; /* ΢��ϵ�� D */ 
	 float Error; /* Error[1],�� k ��ƫ�� */ 
	 float LastError; /* Error[-1],�� k-1 ��ƫ�� */ 
	 float PrevError; /* Error[-2],�� k-2 ��ƫ�� */ 

}PID_TypeDef;

void PID__init(PID_TypeDef *pid);
float PID__ralize(PID_TypeDef *PID,float Feedback_value);
#endif
