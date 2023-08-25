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

	delay(500);	                   	//�ϵ���ʱ		
	while(1)                         	//ѭ��
	{ 
		
		Init_ADXL345();                 	//��ʼ��ADXL345
		devid=Single_Read_ADXL345(0X00);	//����������Ϊ0XE5,��ʾ��ȷ
		if(devid!=0XE5)
		{			

			UartSendStr("No Find!\r\n");					
			
		}
		else
		{			
			Multiple_Read_ADXL345();       	//�����������ݣ��洢��BUF��
			display_x();                   	//---------��ʾX��
//			display_y();                   	//---------��ʾY��
//			display_z();                   	//---------��ʾZ��			    
		}      
		delay(350);                    	//��ʱ  
	}
	

}


////*********************************************************
////******������********
////*********************************************************
//void main()
//{ 
//	uchar devid;
//	delay(500);	                   	//�ϵ���ʱ		

//		
//	//Init_ADXL345();                 	//��ʼ��ADXL345
//	//devid=Single_Read_ADXL345(0X00);	//����������Ϊ0XE5,��ʾ��ȷ
//	
//	while(1)                         	//ѭ��
//	{ 
//		
//		Init_ADXL345();                 	//��ʼ��ADXL345
//		devid=Single_Read_ADXL345(0X00);	//����������Ϊ0XE5,��ʾ��ȷ
//		if(devid!=0XE5)
//		{			

//			UartSendStr("No Find!\r\n");					
//			
//		}
//		else
//		{			
//			Multiple_Read_ADXL345();       	//�����������ݣ��洢��BUF��
//			display_x();                   	//---------��ʾX��
////			display_y();                   	//---------��ʾY��
////			display_z();                   	//---------��ʾZ��			    
//		}      
//		delay(350);                    	//��ʱ  
//	}
//} 
