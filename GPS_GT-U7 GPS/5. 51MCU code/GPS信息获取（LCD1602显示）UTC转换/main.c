#include "main.h"
#include "LCD1602.h"
#include "GPS.h"

//�������
unsigned char KEY_NUM = 0;
bit Page = 0;
unsigned char xdata Display_GPGGA_Buffer[68];
unsigned char xdata Display_GPRMC_Buffer[68];
bit Flag_OV = 0;
bit Flag_Calc_GPGGA_OK = 0;
bit Flag_Calc_GPRMC_OK = 0;

//****************************************************
//������
//****************************************************
void main()
{
	unsigned char i = 0;
	Init_LCD1602();
	LCD1602_write_com(0x80);						//ָ������
	LCD1602_write_word("Welcome to use!");

	Delay_ms(1000);


	Uart_Init();

	while(1)
	{
		Scan_Key();
		if(Flag_GPS_OK == 1 && RX_Buffer[4] == 'G' && RX_Buffer[6] == ',' && RX_Buffer[13] == '.')			//ȷ���Ƿ��յ�"GPGGA"��һ֡����
		{
			for( i = 0; i < 68 ; i++)
			{
				Display_GPGGA_Buffer[i] = RX_Buffer[i];	
			}
			Hour = (Display_GPGGA_Buffer[7]-0x30)*10+(Display_GPGGA_Buffer[8]-0x30)+8;			//UTCʱ��ת��������ʱ��		UTC+8
																								//0x30ΪASCIIת��Ϊ����
			if( Hour >= 24)				//���
			{
				Hour %= 24;				//��ȡ��ǰHour
				Flag_OV = 1;			//���ڽ�λ
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
			LCD1602_write_com(0x80);			//����ָ��
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
	
			LCD1602_write_com(0x80+0x40);			//����ָ��
			
			LCD1602_write_data(Display_GPGGA_Buffer[28]);			//N ���� S

			LCD1602_write_data(Display_GPGGA_Buffer[17]);			//γ��
			LCD1602_write_data(Display_GPGGA_Buffer[18]);			//γ��
			LCD1602_write_data(0xdf);								//��
			LCD1602_write_data(Display_GPGGA_Buffer[19]);			//γ��
			LCD1602_write_data(Display_GPGGA_Buffer[20]);			//γ��
			LCD1602_write_word("'");								//��
			
			LCD1602_write_data(Display_GPGGA_Buffer[42]);			//E ���� W

			LCD1602_write_data(Display_GPGGA_Buffer[30]);			//����
			LCD1602_write_data(Display_GPGGA_Buffer[31]);	
			LCD1602_write_data(Display_GPGGA_Buffer[32]);	
			LCD1602_write_data(0xdf);								
			LCD1602_write_data(Display_GPGGA_Buffer[33]);			
			LCD1602_write_data(Display_GPGGA_Buffer[34]);		
			LCD1602_write_word("'");

						
		}
		
		if(Flag_GPS_OK == 1 && RX_Buffer[4] == 'M' && RX_Buffer[52] == ',' && RX_Buffer[59] == ',')			//ȷ���Ƿ��յ�"GPRMC"��һ֡����
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


			if(Flag_OV == 1)			//�н�λ
			{
				UTCDate2LocalDate();			//UTC����ת��Ϊ����ʱ��		
			}

			Flag_Calc_GPRMC_OK = 1;
		}

		if(Page == 1 && Flag_Calc_GPRMC_OK == 1)
		{
			LED1 = ~LED1;
			Flag_Calc_GPRMC_OK = 0;
			LCD1602_write_com(0x80);			//����ָ��
			LCD1602_write_word("20");
			LCD1602_write_data(Year_High);
			LCD1602_write_data(Year_Low);
			LCD1602_write_data('-');
	
			LCD1602_write_data(Month_High);
			LCD1602_write_data(Month_Low);
			LCD1602_write_data('-');
	
			LCD1602_write_data(Day_High);
			LCD1602_write_data(Day_Low);
			
			
			LCD1602_write_com(0x80+0x40);			//����ָ��
			LCD1602_write_word("Speed:");				//��ʾ����

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
//UTC�����뵱������ת��
//****************************************************
void UTCDate2LocalDate(void)
{
	Day = (Day_High - 0x30) * 10 + (Day_Low-0x30) + 1;		//��  ��һ
	Month = (Month_High - 0x30) * 10 + (Month_Low - 0x30);
	Year = 2000 + (Year_High - 0x30) * 10 + (Year_Low - 0x30);
	
	MaxDay = GetMaxDay(Month,Year);				//��ȡ���� ���� ���ֵ
	if(Day > MaxDay)		//���
	{
		Day = 1;
		Month += 1;
		if(Month > 12)
		{
			Year+=1;
		}
	}

	Day_High = Day/10 + 0x30;				//ת������ֵΪASCII
	Day_Low = Day%10 + 0x30;

	Month_High = Month/10 + 0x30;			//ת���·�ֵΪASCII
	Month_Low = Month%10 + 0x30;

	Year_High = Year%100/10 + 0x30;			//ת�����ֵΪASCII
	Year_Low = Year%10 + 0x30;			
}

//****************************************************
//��ȡ�����������ֵ
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
				//2�·ݱȽ����⣬��Ҫ�����ǲ����������жϵ�����28�컹29��
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
//������
//****************************************************
bit IsLeapYear(unsigned int uiYear)
{
	return (((uiYear%4)==0)&&((uiYear%100)!=0))||((uiYear%400)==0);
}


//****************************************************
//����ɨ�����
//****************************************************
void Scan_Key()
{
	if( KEY4 == 0 )	 					//����1ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY4 == 0 )
		{
			while(KEY4 == 0);			//�ȴ�����
			KEY_NUM = 3;
			Page = ~Page;
			LCD1602_write_com(0X01);	//����
		}
	}
}
//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}