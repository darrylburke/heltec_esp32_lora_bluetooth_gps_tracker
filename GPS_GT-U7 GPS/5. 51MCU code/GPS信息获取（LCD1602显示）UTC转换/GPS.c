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
	SCON = 0X50;  //UART方式1；8位UART
	REN  = 1;     //允许串行口接收数据
	PCON = 0x00;  //SMOD=0;波特率不加倍
	TMOD = 0x20;  //T1方式2，用于产生波特率
	TH1  = 0xFD;  //装初值
	TL1  = 0xFD;
	TR1  = 1;     //启动定时器1
	EA   = 1;     //打开全局中断控制
	ES   = 1;     //打开串行口中断	
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
























