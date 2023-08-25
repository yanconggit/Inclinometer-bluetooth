#include "stc8f.h"
#include "intrins.h"
#include "uart.h"
#include "ADXL345.h"

//#define FOSC            12000000UL


void main()
{
	uchar devid;
	
    UartInit();
    ES = 1;
    EA = 1;
    UartSendStr("Uart Test !\r\n");	

	delay(500);	                   	//上电延时		
	while(1)                         	//循环
	{ 
		
		Init_ADXL345();                 	//初始化ADXL345
		devid=Single_Read_ADXL345(0X00);	//读出的数据为0XE5,表示正确
		if(devid!=0XE5)
		{			

			UartSendStr("No Find!\r\n");					
			
		}
		else
		{			
			Multiple_Read_ADXL345();       	//连续读出数据，存储在BUF中
			display_x();                   	//---------显示X轴
//			display_y();                   	//---------显示Y轴
//			display_z();                   	//---------显示Z轴			    
		}      
		delay(350);                    	//延时  
	}
	

}


////*********************************************************
////******主程序********
////*********************************************************
//void main()
//{ 
//	uchar devid;
//	delay(500);	                   	//上电延时		

//		
//	//Init_ADXL345();                 	//初始化ADXL345
//	//devid=Single_Read_ADXL345(0X00);	//读出的数据为0XE5,表示正确
//	
//	while(1)                         	//循环
//	{ 
//		
//		Init_ADXL345();                 	//初始化ADXL345
//		devid=Single_Read_ADXL345(0X00);	//读出的数据为0XE5,表示正确
//		if(devid!=0XE5)
//		{			

//			UartSendStr("No Find!\r\n");					
//			
//		}
//		else
//		{			
//			Multiple_Read_ADXL345();       	//连续读出数据，存储在BUF中
//			display_x();                   	//---------显示X轴
////			display_y();                   	//---------显示Y轴
////			display_z();                   	//---------显示Z轴			    
//		}      
//		delay(350);                    	//延时  
//	}
//} 
