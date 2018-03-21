#include "GPS.h"

unsigned char RX_Buffer[68];
unsigned char RX_Count = 0;
unsigned char Hour = 0,Min_High = 0,Min_Low = 0,Sec_High = 0,Sec_Low = 0;
unsigned char Month = 0,Day = 0,Month_High = 0, Month_Low = 0,Day_Low = 0 ,Day_High = 0, Year_High = 0,Year_Low = 0;
unsigned int Year = 0;
bit Flag_GPS_OK = 0;
unsigned char MaxDay = 0;

void Uart_Init()					  			   
{
	SCON = 0X50;  //UART��ʽ1��8λUART
	REN  = 1;     //�����пڽ�������
	PCON = 0x00;  //SMOD=0;�����ʲ��ӱ�
	TMOD = 0x20;  //T1��ʽ2�����ڲ���������
	TH1  = 0xFD;  //װ��ֵ
	TL1  = 0xFD;
	TR1  = 1;     //������ʱ��1
	EA   = 1;     //��ȫ���жϿ���
	ES   = 1;     //�򿪴��п��ж�	
}


void RECEIVE_DATA(void) interrupt 4 using 3	  		
{ 
	unsigned char temp = 0;
	ES=0;
	temp = SBUF;
	RI = 0;
//	LED1 = ~LED1;
	
	if(temp == '$')
	{
		RX_Count = 0;
		Flag_GPS_OK = 0;		
	}

	RX_Buffer[RX_Count++] = temp;

	if(RX_Count >= 59)
	{
		RX_Count = 59;
		Flag_GPS_OK = 1;		
	}

	ES=1; 
}
























