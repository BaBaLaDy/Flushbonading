#ifndef __JDQ_H
#define __JDQ_H

#include "stm32f10x.h"

#include "WeiDai.h"
 
#define JDQ PAout(11)	// PA8

extern u16 pw;
void JDQ_Init(void);//��ʼ��

void PID_out(void); //���PID������������		

#endif