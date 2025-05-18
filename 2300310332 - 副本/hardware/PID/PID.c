#include "stm32f4xx.h"                  // Device header
#include "PID.h"
extern int reflog;
void PID__init(PID_TypeDef *pid , float point,float P,float I,float D)
{
	pid-> SetPoint=point; 
	pid-> ActualValue=0;
	pid-> SumError=0; 
	pid-> Proportion=P;
	pid-> Integral=I; 
	pid-> Derivative=D;
	pid-> Error=0; //本次
	pid-> Error_1=0; //上次
	pid-> Error_2=0; //上上次
}

void PID_setchg(PID_TypeDef *pid,float Point)
{
	pid-> SetPoint=Point;
}

void PID_change(PID_TypeDef *pid , float P,float I,float D)
{
	pid-> Proportion=P;
	pid-> Integral=I; 
	pid-> Derivative=D;
}

float PID__ralize(PID_TypeDef *PID,float Feedback_value)
{
	 PID->Error = (float)(PID->SetPoint - Feedback_value); 
     //PID->SumError += PID->Error; 
     PID->ActualValue += (PID->Proportion * PID->Error) 
     + (PID->Integral * PID->Error) 
     + (PID->Derivative * (PID->Error - PID->Error_1)); 
	 PID->Error_2 = PID->Error_1;
	 PID->Error_1 = PID->Error;
	
	if(PID->ActualValue>=800){
		PID->ActualValue=800; 
		PID->SumError=0;
		reflog=1;
	}

     return ((int32_t)(PID->ActualValue)); 

}
