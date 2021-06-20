#include "pid.h"
#include "stdio.h"
#include "oled.h"
#include "JDQ.h"
PID pid; //���PID�㷨����Ҫ������
//void PID_Init()            
//{
//    //pid.choose_model = MODEL_PID;
//    
//    pid.T=500;                //�������ڣ���ʱ��ʹ��1ms������Сִ��PID������Ϊ330ms
//    
//  pid.Sv=60;                //�û��趨ֵ
//    pid.Kp=0.5;                //����ϵ��
//  pid.Ti=180;            //����ʱ��
//    pid.Td=1;                    //΢��ʱ��
//    pid.OUT0=0;                //һ��ά�ֵ����
//    
//    pid.pwmcycle = 200;    //PWM������
//}
    
void PID_Calc()  //pid����
{
    float DelEk;            //���κ��ϴ�ƫ��������ƫ��֮��
    float ti,ki;
    float td;
    float kd;
    float out;

    if(pid.Tdata < (pid.T))  //��С��������δ��
     {
            return ;
     }
    pid.Tdata = 0;
    pid.Ek=pid.Sv-pid.Pv;               //�õ���ǰ��ƫ��ֵ
    pid.Pout=pid.Kp*pid.Ek;          //�������
 
    pid.SEk+=pid.Ek;                    //��ʷƫ���ܺ�
 
    DelEk=pid.Ek-pid.Ek_1;              //�������ƫ��֮��
 
    ti=pid.T/pid.Ti;
    ki=ti*pid.Kp;
 
    pid.Iout=ki*pid.SEk;  //�������
    /*ע�⣺��������ж��˸�pid.Kp,ԭ�������У����Զ�ɾ������ȷ��Ӧ����pid.Iout=ki*pid.SEK */


    td=pid.Td/pid.T;
 
    kd=pid.Kp*td;
 
  pid.Dout=kd*DelEk;                //΢�����
 
    
//     switch(pid.choose_model)
//     {
//         case MODEL_P:     out= pid.Pout;                                                printf("ʹ��P����\r\n") ;
//             break;
//         
//         case MODEL_PI:  out= pid.Pout+ pid.Iout;                            printf("ʹ��PI����\r\n") ;
//             break;
//                 
//         case MODEL_PID: out= pid.Pout+ pid.Iout+ pid.Dout;        printf("ʹ��PID����\r\n") ;
//             break;
//     }
 
 //////////////////////////////////////////////////////////
		 out= pid.Pout+ pid.Iout+ pid.Dout;
		 

        /*�ж���������Ƿ���Ͽ���Ҫ��*/
     if(out>pid.pwmcycle)        //���ܱ�PWM���ڴ�������ȫ����
     {
        pid.OUT=pid.pwmcycle;
     }
//		 else if(out<pid.pwmcycle&&out>0)
//		 {
//				if(pid.Ek<0.12)
//				{
//					pid.OUT=10;
//				}
//		 }
     else if(out<0)             //ֵ����Ϊ����
     {
        pid.OUT=10; 
			  if(pid.Ek<-0.1)
				{
						pid.OUT = pid.OUT00;	
				}
     }
     else 
     {
        pid.OUT=out;
			 if(pid.Ek<0.35)
			 {
				 pid.OUT=13;
				 if(pid.Ek<-0.1)
				 {
						pid.OUT = pid.OUT00;	
				 }
			 }
     }
     OLED_ShowNum(0,6, pid.OUT,3, 16);
     pid.Ek_1=pid.Ek;  //����ƫ��

     
}