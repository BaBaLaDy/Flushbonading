#include "oled.h"
#include "bsp_systick.h"
#include "oledfont.h"
u8 OLED_GRAM[144][8];


/*PB6 -- SCL; PB7 -- SDA */


void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_OD;  //I2C���뿪©���
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	I2C_DeInit(I2C1);  //ʹ��I2C1
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;//������I2C��ַ,���д
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;//400K

	I2C_Cmd(I2C1, ENABLE);
	I2C_Init(I2C1, &I2C_InitStructure);
}

void I2C_WriteByte(uint8_t addr,uint8_t data)
{
  while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
	
	I2C_GenerateSTART(I2C1, ENABLE);//����I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	I2C_Send7bitAddress(I2C1, OLED_ADDRESS, I2C_Direction_Transmitter);//������ַ -- Ĭ��0x78
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, addr);//�Ĵ�����ַ
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, data);//��������
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C1, ENABLE);//�ر�I2C1����
}

void WriteCmd(unsigned char I2C_Command)//д����
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//д����
{
	I2C_WriteByte(0x40, I2C_Data);
}

void OLED_Init(void)
{
	Delay_ms(100); //�������ʱ����Ҫ
	
	WriteCmd(0xAE); //����ʾ
	WriteCmd(0x20);	//�����ڴ�Ѱַģʽ	
	WriteCmd(0x10);	//00��ˮƽѰַģʽ;01����ֱѰַģʽ;10��ҳ��Ѱַģʽ(����);11����Ч
	WriteCmd(0xb0);	//Ϊҳ��Ѱַģʽ����ҳ�濪ʼ��ַ��0-7
	WriteCmd(0xc8);	//����COM���ɨ�跽��
	WriteCmd(0x00); //---���õ��е�ַ
	WriteCmd(0x10); //---���ø��е�ַ
	WriteCmd(0x40); //--������ʼ�е�ַ
	WriteCmd(0x81); //--set contrast control register
	WriteCmd(0xff); //���ȵ��� 0x00~0xff
	WriteCmd(0xa1); //--���ö�����ӳ��0��127
	WriteCmd(0xa6); //--����������ʾ
	WriteCmd(0xa8); //--���ø��ñ�(1 ~ 64)
	WriteCmd(0x3F); //
	WriteCmd(0xa4); //0xa4,�����ѭRAM����;0xa5,Output����RAM����
	WriteCmd(0xd3); //-������ʾ����
	WriteCmd(0x00); //-not offset
	WriteCmd(0xd5); //--������ʾʱ�ӷ�Ƶ/����Ƶ��
	WriteCmd(0xf0); //--���÷���
	WriteCmd(0xd9); //--����pre-chargeʱ��
	WriteCmd(0x22); //
	WriteCmd(0xda); //--����com��ͷ��Ӳ������
	WriteCmd(0x12);
	WriteCmd(0xdb); //--����vcomh
	WriteCmd(0x20); //0x20,0.77xVcc
	WriteCmd(0x8d); //--����DC-DC
	WriteCmd(0x14); //
	WriteCmd(0xaf); //--��oled���
}

void OLED_SetPos(unsigned char x, unsigned char y) //������ʼ������
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//����
{
	OLED_Fill(0x00);
}

void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}

void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ� 
	WriteCmd(0XAE);  //OLED����
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>128-1){x=0;y=y+2;}		//���X�ķ�Χ����128���Զ�����
		if(Char_Size ==16)
			{
			OLED_SetPos(x,y);	
			for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
			OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
			}
			//6x8����
			else {	
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				
			}
}

void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

 /**
  * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~7);
	*					ch[] :- Ҫ��ʾ���ַ���; 
	*					TextSize : �ַ���С(1:6*8 ; 2:8*16)
	* @retval ��
  */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<6;i++)
					WriteDat(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i]);
				OLED_SetPos(x,y+1);
				for(i=0;i<8;i++)
					WriteDat(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}

 /**
  * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
  * @param  x,y: ��ʼ������(x:0~127, y:0~7); 
	*					N:������codetab.h�е�����
	* @retval ��
  */
 void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
}



 /**
  * @brief  OLED_DrawBMP����ʾBMPλͼ
  * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~7);
	*					x1,y1 : ���Խ���(������)������(x1:1~128,y1:1~8)
	* @retval ��
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat(BMP[j++]);
		}
	}
}




