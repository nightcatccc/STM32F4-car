#ifndef __PID_H
#define __PID_H


typedef struct pid{     //???
    /* data */
    float p;
    float i;
    float d;

    int16_t Aim;  //???
    int16_t Now;  //???
    int16_t Out;
    int16_t Error;//????
    int16_t Error_1;//????
	int16_t Error_2;//?????
}PID;


typedef struct
{
	 float SetPoint;
	 float ActualValue;
	 float SumError; 
	 float Proportion;
	 float Integral; 
	 float Derivative; 
	 float Error;
	 float Error_1; 
	 float Error_2; 

}PID_TypeDef;

void PID__init(PID_TypeDef *pid , float point,float P,float I,float D);
int32_t PID_calc_V(int encoder,PID *c);
float PID__ralize(PID_TypeDef *PID,float Feedback_value);
void PID_setchg(PID_TypeDef *pid,float Point);
void PID_change(PID_TypeDef *pid , float P,float I,float D);
#endif
