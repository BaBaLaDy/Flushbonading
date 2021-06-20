#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"

//  ���Ŷ���
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY1_GPIO_PORT    GPIOC			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_8

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOC
#define    KEY2_GPIO_PORT    GPIOC		   
#define    KEY2_GPIO_PIN		  GPIO_Pin_9

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY3_GPIO_PORT    GPIOA		   
#define    KEY3_GPIO_PIN		  GPIO_Pin_0

 /** �������±��ú�
	*  ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	*  ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/
#define KEY_ON	0
#define KEY_OFF	1
extern int state;
void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void Key(void);
int key_back(void);
#endif /* __KEY_H */

