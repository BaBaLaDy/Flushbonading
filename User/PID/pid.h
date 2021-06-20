#ifndef _pid_H
#define _pid_H
#include "stm32f10x.h"
//#define     MODEL_P       1
//#define     MODEL_PI      2
//#define     MODEL_PID     3

typedef struct
{
    u8 choose_model;        //ʹ���ĸ�ģʽ����
    
    float Sv;     //�û��趨ֵ
    float Pv;     //��ǰֵ��ʵ��ֵ
 
    float Kp;        //����ϵ��
    float T;      //PID��������--��������
    u16   Tdata;    //�ж�PID���ڵ�û��
    float Ti;        //����ʱ�䳣��
    float Td;       //΢��ϵ��
    
    
    
    float Ek;          //����ƫ��
    float Ek_1;        //�ϴ�ƫ��
    float SEk;         //��ʷƫ��֮��
    
    float Iout;        //�������
    float Pout;        //�������    
    float Dout;        //΢�����
    
    float OUT0;        //һ��ά�ֵ��������ֹʧ��
    float OUT00;
    int OUT;        //�������յõ���ֵ
    
    
    u16 pwmcycle;//pwm����
    
    
}PID;


extern PID pid; //���PID�㷨����Ҫ������
void PID_Calc(void); //pid����
void PID_Init(void);        //PID��ʼ�� 
#endif








