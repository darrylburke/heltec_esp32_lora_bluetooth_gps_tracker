#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

//LED IO设置
sbit LED1 = P2^0;
sbit LED2 = P2^1;
#define LED_ON 0
#define LED_OFF 1

//按键 IO设置
sbit KEY1 = P3^0;
sbit KEY2 = P3^1;
sbit KEY3 = P3^5;
sbit KEY4 = P3^6;


//函数或者变量声明
extern void Delay_ms(unsigned int n);
extern void Scan_Key();
extern void UTCDate2LocalDate(void);
extern bit IsLeapYear(unsigned int uiYear);	
extern unsigned char GetMaxDay(unsigned char Month_Value,unsigned int Year_Value);




#endif