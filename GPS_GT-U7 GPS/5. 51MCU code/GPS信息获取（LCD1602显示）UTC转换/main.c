#include "main.h"
#include "LCD1602.h"
#include "GPS.h"

//定义变量
unsigned char KEY_NUM = 0;
bit Page = 0;
unsigned char xdata Display_GPGGA_Buffer[68];
unsigned char xdata Display_GPRMC_Buffer[68];
bit Flag_OV = 0;
bit Flag_Calc_GPGGA_OK = 0;
bit Flag_Calc_GPRMC_OK = 0;

//****************************************************
//主函数
//****************************************************
void main()
{
	unsigned char i = 0;
	Init_LCD1602();
	LCD1602_write_com(0x80);						//指针设置
	LCD1602_write_word("Welcome to use!");

	Delay_ms(1000);


	Uart_Init();

	while(1)
	{
		Scan_Key();
		if(Flag_GPS_OK == 1 && RX_Buffer[4] == 'G' && RX_Buffer[6] == ',' && RX_Buffer[13] == '.')			//确定是否收到"GPGGA"这一帧数据
		{
			for( i = 0; i < 68 ; i++)
			{
				Display_GPGGA_Buffer[i] = RX_Buffer[i];	
			}
			Hour = (Display_GPGGA_Buffer[7]-0x30)*10+(Display_GPGGA_Buffer[8]-0x30)+8;			//UTC时间转换到北京时间		UTC+8
																								//0x30为ASCII转换为数字
			if( Hour >= 24)				//溢出
			{
				Hour %= 24;				//获取当前Hour
				Flag_OV = 1;			//日期进位
			}
			else
			{
				Flag_OV = 0;
			}

			Min_High = Display_GPGGA_Buffer[9];
			Min_Low = Display_GPGGA_Buffer[10];
	
			Sec_High = Display_GPGGA_Buffer[11];
			Sec_Low = Display_GPGGA_Buffer[12];

			Flag_Calc_GPGGA_OK = 1;
		}

		if(Page == 0 && Flag_Calc_GPGGA_OK == 1)
		{
			LED1 = ~LED1;
			Flag_Calc_GPGGA_OK = 0;
			LCD1602_write_com(0x80);			//设置指针
			LCD1602_write_data(Hour/10+0x30);
			LCD1602_write_data(Hour%10+0x30);

			LCD1602_write_data(':');

			LCD1602_write_data(Min_High);
			LCD1602_write_data(Min_Low);

			LCD1602_write_data(':');
	
			LCD1602_write_data(Sec_High);
			LCD1602_write_data(Sec_Low);

			LCD1602_write_word("  ");
			
			LCD1602_write_data(Display_GPGGA_Buffer[54]);	
			LCD1602_write_data(Display_GPGGA_Buffer[55]);	
			LCD1602_write_data(Display_GPGGA_Buffer[56]);	
			LCD1602_write_data(Display_GPGGA_Buffer[57]);
			LCD1602_write_word("m");
	
			LCD1602_write_com(0x80+0x40);			//设置指针
			
			LCD1602_write_data(Display_GPGGA_Buffer[28]);			//N 或者 S

			LCD1602_write_data(Display_GPGGA_Buffer[17]);			//纬度
			LCD1602_write_data(Display_GPGGA_Buffer[18]);			//纬度
			LCD1602_write_data(0xdf);								//度
			LCD1602_write_data(Display_GPGGA_Buffer[19]);			//纬度
			LCD1602_write_data(Display_GPGGA_Buffer[20]);			//纬度
			LCD1602_write_word("'");								//秒
			
			LCD1602_write_data(Display_GPGGA_Buffer[42]);			//E 或者 W

			LCD1602_write_data(Display_GPGGA_Buffer[30]);			//经度
			LCD1602_write_data(Display_GPGGA_Buffer[31]);	
			LCD1602_write_data(Display_GPGGA_Buffer[32]);	
			LCD1602_write_data(0xdf);								
			LCD1602_write_data(Display_GPGGA_Buffer[33]);			
			LCD1602_write_data(Display_GPGGA_Buffer[34]);		
			LCD1602_write_word("'");

						
		}
		
		if(Flag_GPS_OK == 1 && RX_Buffer[4] == 'M' && RX_Buffer[52] == ',' && RX_Buffer[59] == ',')			//确定是否收到"GPRMC"这一帧数据
		{
			for( i = 0; i < 68 ; i++)
			{
				Display_GPRMC_Buffer[i] = RX_Buffer[i];	
			}

			Year_High = Display_GPRMC_Buffer[57];
			Year_Low = Display_GPRMC_Buffer[58];

			Month_High = Display_GPRMC_Buffer[55];
			Month_Low = Display_GPRMC_Buffer[56];

			Day_High = Display_GPRMC_Buffer[53];
			Day_Low = Display_GPRMC_Buffer[54];


			if(Flag_OV == 1)			//有进位
			{
				UTCDate2LocalDate();			//UTC日期转换为北京时间		
			}

			Flag_Calc_GPRMC_OK = 1;
		}

		if(Page == 1 && Flag_Calc_GPRMC_OK == 1)
		{
			LED1 = ~LED1;
			Flag_Calc_GPRMC_OK = 0;
			LCD1602_write_com(0x80);			//设置指针
			LCD1602_write_word("20");
			LCD1602_write_data(Year_High);
			LCD1602_write_data(Year_Low);
			LCD1602_write_data('-');
	
			LCD1602_write_data(Month_High);
			LCD1602_write_data(Month_Low);
			LCD1602_write_data('-');
	
			LCD1602_write_data(Day_High);
			LCD1602_write_data(Day_Low);
			
			
			LCD1602_write_com(0x80+0x40);			//设置指针
			LCD1602_write_word("Speed:");				//显示内容

			LCD1602_write_data(Display_GPRMC_Buffer[46]);		
			LCD1602_write_data(Display_GPRMC_Buffer[47]);		
			LCD1602_write_data(Display_GPRMC_Buffer[48]);			
			LCD1602_write_data(Display_GPRMC_Buffer[49]);	
			LCD1602_write_data(Display_GPRMC_Buffer[50]);
			
			LCD1602_write_word("m/s");				
		}		
	}
}
//****************************************************
//UTC日期与当地日期转换
//****************************************************
void UTCDate2LocalDate(void)
{
	Day = (Day_High - 0x30) * 10 + (Day_Low-0x30) + 1;		//日  加一
	Month = (Month_High - 0x30) * 10 + (Month_Low - 0x30);
	Year = 2000 + (Year_High - 0x30) * 10 + (Year_Low - 0x30);
	
	MaxDay = GetMaxDay(Month,Year);				//获取当月 天数 最大值
	if(Day > MaxDay)		//溢出
	{
		Day = 1;
		Month += 1;
		if(Month > 12)
		{
			Year+=1;
		}
	}

	Day_High = Day/10 + 0x30;				//转换日期值为ASCII
	Day_Low = Day%10 + 0x30;

	Month_High = Month/10 + 0x30;			//转换月份值为ASCII
	Month_Low = Month%10 + 0x30;

	Year_High = Year%100/10 + 0x30;			//转换年份值为ASCII
	Year_Low = Year%10 + 0x30;			
}

//****************************************************
//获取当月日期最大值
//****************************************************
unsigned char GetMaxDay(unsigned char Month_Value,unsigned int Year_Value)
{
	unsigned char iDays;
	switch(Month_Value)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			{
				iDays = 31;
			}
			break;
		case 2:
			{
				//2月份比较特殊，需要根据是不是闰年来判断当月是28天还29天
				iDays = IsLeapYear(Year_Value)?29:28;
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			{
				iDays = 30;
			}
			break;
		default : break;
	}
	return(iDays);						
}

//****************************************************
//闰年检测
//****************************************************
bit IsLeapYear(unsigned int uiYear)
{
	return (((uiYear%4)==0)&&((uiYear%100)!=0))||((uiYear%400)==0);
}


//****************************************************
//按键扫描程序
//****************************************************
void Scan_Key()
{
	if( KEY4 == 0 )	 					//按键1扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY4 == 0 )
		{
			while(KEY4 == 0);			//等待松手
			KEY_NUM = 3;
			Page = ~Page;
			LCD1602_write_com(0X01);	//清屏
		}
	}
}
//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}