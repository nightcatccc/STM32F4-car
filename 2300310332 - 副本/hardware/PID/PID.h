#ifndef __PID_H
#define __PID_H




typedef struct
{
	 float SetPoint;
	 float ActualValue;
	 float SumError; 
	 float Proportion;
	 float Integral; 
	 float Derivative; 
	 float Error;
	 float LastError; 
	 float PrevError; 

}PID_TypeDef;

void PID__init(PID_TypeDef *pid,float point);
float PID__ralize(PID_TypeDef *PID,float Feedback_value);
#endif
