#include "JDQ.h"
#include "pid.h"
u16 pw=0;
//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void JDQ_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��PA,PD�˿�ʱ��
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;						 //LED0-->PA.8 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 			 //�����趨������ʼ��GPIOA.8
 GPIO_SetBits(GPIOA,GPIO_Pin_11);						 					 //PA.8 �����
}

void PID_out()  //���PID������������---ÿ1ms������1��
{
	 pw++;
	 if(pw>=pid.pwmcycle)  
	 {
	   pw=0;
	 } 
	 if(pw<pid.OUT)
	 {
		 JDQ=1;//����
	 }
	 else
	 {
	   JDQ=0;//ֹͣ����
	 }	 
}
