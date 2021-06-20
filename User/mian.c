#include "stm32f10x.h"
#include "stdio.h"
#include "bsp_usart.h"
#include "bsp_systick.h"
#include "oled.h"
#include "DS18B20.h"
#include "PID.h"
#include "Tim.h"
#include "JDQ.h"
#include "valuepack.h"
#include "key.h"
extern float Water;
void PID_Init()            
{
    //pid.choose_model = MODEL_PID;
    
    pid.T=500;                //�������ڣ���ʱ��ʹ��1ms������Сִ��PID������Ϊ330ms
    
		pid.Sv=40;                //�û��趨ֵ
    pid.Kp=29;                //����ϵ��
		pid.Ti=4500000;            //����ʱ��
    pid.Td=530;                    //΢��ʱ��
    pid.OUT0=1;	//һ��ά�ֵ����
    pid.OUT00 = 0;
    pid.pwmcycle = 200;    //PWM������
}
  
void hardwareinit(void)
{
	JDQ_Init();
  //USART_Config();
	initValuePack(115200);
	TIM3_Int_Init(10-1,7200-1); //1MS����һ��PID������
	TIM4_Int_Init(7500-1,7200-1); //0.75s����һ���¶ȵĶ�ȡ
	DS18B20_Init();
	Key_GPIO_Config();
	PID_Init();
	I2C_Configuration();
}

int main()
{
	char Temp[10];
	int flag=1;
//	char Temp1[10];
//	char pidout[10];
	hardwareinit();
	OLED_Init();
	OLED_CLS();
	OLED_ShowStr(0, 0, "Temp:", 2);

	while(1)	
	{
		PID_Calc(); //pid����
		sprintf(Temp,"%.3f",pid.Pv);

		OLED_ShowStr(50,0, Temp, 2);
		
    OLED_ShowStr(0,2,"SetTemp:",2);
		OLED_ShowNum(65,2,pid.Sv,3,16);
		
//		//�¶��ж�
//		if(pid.Pv<pid.Sv-8)
//		{
//			TIM_Cmd(TIM3, DISABLE);  //ʧ��TIMx����
//			JDQ=1;
//		}
//		else if((pid.Pv>pid.Sv-8) && flag)
//		{
//			JDQ=0;
//			OLED_ShowStr(80,2, "Stop", 2);
//			
//			for(int i=0;i<40000000;i++);
//			pid.Pv=DS18B20_Get_Temp();
//			PID_Calc(); //pid����
//			OLED_ShowStr(80,2, "Star", 2);
//			TIM_Cmd(TIM3, ENABLE);
//			flag=0;
//		}
		
		
		if(readValuePack(&rxpack))
		{
			pid.Sv=rxpack.integers[0];
		  if(pid.Sv-pid.Pv>8)
			{
				flag=1;
			}

		}
		//��������Ŀ���¶�
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			
			pid.Sv+=5;
			if(pid.Sv-pid.Pv>8)
			{
				flag=1;
			}
		} 
		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			pid.Sv-=5;

		}
	}
}
