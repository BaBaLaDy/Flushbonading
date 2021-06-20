#include "./HC-SR04/HC_SR04.h"
#include "./WeiDai/WeiDai.h"
#include "./systick/bsp_systick.h"

int overcount=0; //��¼��ʱ���������

//PA6�����Trig��  PA7����(Echo)

void SR04_Int_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStruct;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		
		//PA6�����PA7����		
		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//����Ϊ�������
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;
		GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOA,&GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
		GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
		GPIO_Init(GPIOA,&GPIO_InitStruct);
		//��ʱ��3��ʼ��
		TIM_TimeBaseStructure.TIM_Period = 999; //�����Զ���ת��ֵΪ999
		TIM_TimeBaseStructure.TIM_Prescaler =71; //����Ԥ��Ƶϵ��
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���ģʽ
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ���ƶ�TIM3�жϣ���������ж�
		//�ж�����
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		TIM_Cmd(TIM3, DISABLE);
}


//��ຯ��
float Senor_Using(void) //��������������
{
		float length=0,sum=0;
		u16 tim;
		unsigned int i=0;
		//��������ݼ���һ��ƽ��ֵ
		while(i!=5)
		{
			PAout(6)=1;   //�����źţ���Ϊ�����ź�
			Delay_us(20); //�ߵ�ƽ�źų���10us
			PAout(6)=0;   //�ȴ������ź�
			while(PAin(7)==RESET);//�����źŵ�����������ʱ������	
			TIM_Cmd(TIM3,ENABLE);
			i++; //ÿ�յ�һ�λ����ź�+1,�յ�5�ξͼ����ֵ
			while(PAin(7)==SET);//�����ź���ʧ
			TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��
			tim=TIM_GetCounter(TIM3); //��ȡ��TIM3���Ĵ����еļ���ֵ
			length=(tim+overcount*1000)/58.0; //ͨ�������źż������
			sum=length+sum;
			TIM3->CNT=0; //��TIM3�����Ĵ����ļ���ֵ����
			overcount=0; //�ж������������
			Delay_ms(100);
		}
			length=sum/5;
			return length; //������Ϊ��������ֵ
}


