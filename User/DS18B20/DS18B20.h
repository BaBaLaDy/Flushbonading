#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f10x.h"
#include "WeiDai.h"

#include "bsp_systick.h"



//IO�ڷ���
#define DS18B20_IO_IN()  {GPIOE->CRH&=0XFFFF0FFF;GPIOE->CRH|=8<<12;}
#define DS18B20_IO_OUT() {GPIOE->CRH&=0XFFFF0FFF;GPIOE->CRH|=3<<12;}
#define DS18B20_DQ_OUT    PEout(6) //PE6
#define DS18B20_DQ_IN     PEin(6)  //PE6 

#define DS18B20_CLK_GPIO  RCC_APB2Periph_GPIOE
#define DS18B20_GPIO 			GPIOE
#define DS18B20_Pin  			GPIO_Pin_6;

extern float T;

u8 DS18B20_Init(void);//��ʼ��DS18B20
float DS18B20_Get_Temp(void);//��ȡ�¶�
short DS18B20_Get_Temp2(void);
float DS18B20_Get_Temp3(void);
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    


#endif