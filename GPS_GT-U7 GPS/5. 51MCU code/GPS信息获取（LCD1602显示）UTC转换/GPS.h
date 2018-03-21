#ifndef __GPS_H__
#define __GPS_H__

#include <reg52.h>
#include "main.h"



//函数或者变量声明
extern void Uart_Init();



extern unsigned char RX_Buffer[68];
extern unsigned char RX_Count;
extern unsigned char Hour,Min_High,Min_Low,Sec_High,Sec_Low;
extern unsigned char Month,Day,Month_High, Month_Low,Day_Low ,Day_High, Year_High,Year_Low;
extern unsigned int Year;
extern unsigned char MaxDay;

extern bit Flag_GPS_OK;



#endif