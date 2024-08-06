#include "stm32f10x.h"                  // Device header
#include "PID.h"
/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������������λ����Ϣ��Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������
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

float PID__ralize(PID_TypeDef *PID,float Feedback_value)//����pid����ڲ�����ʵ��ֵ
{
	 PID->Error = (float)(PID->SetPoint - Feedback_value); /* ����ƫ�� */ 
     PID->SumError += PID->Error; /* �ۼ�ƫ�� */ 
     PID->ActualValue = (PID->Proportion * PID->Error) /* �������� */ 
     + (PID->Integral * PID->SumError) /* ���ֻ��� */ 
     + (PID->Derivative * (PID->Error - PID->LastError)); /* ΢�ֻ��� */ 
     PID->LastError = PID->Error; /* �洢ƫ������´μ��� */

     return ((int32_t)(PID->ActualValue)); /* ���ؼ�����������ֵ */

}
