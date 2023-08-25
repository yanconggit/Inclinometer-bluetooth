#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include "stc8f.h"
#include "ADXL345.h"
#include "intrins.h"
#include "uart.h"

	
sbit	  SCL=P1^1;      //IICʱ�����Ŷ���
sbit 	  SDA=P1^0;      //IIC�������Ŷ���
#define	SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
                              //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A


BYTE BUF[8];                         //�������ݻ�����      	
uchar ge,shi,bai,qian,wan;           //��ʾ����
int  dis_data;                       //����

void delay(unsigned int k);
void Init_ADXL345(void);             //��ʼ��ADXL345

//*********************************************************
void conversion(uint temp_data)  
{  
    wan=temp_data/10000+0x30 ;
    temp_data=temp_data%10000;   //ȡ������
	qian=temp_data/1000+0x30 ;
    temp_data=temp_data%1000;    //ȡ������
    bai=temp_data/100+0x30   ;
    temp_data=temp_data%100;     //ȡ������
    shi=temp_data/10+0x30    ;
    temp_data=temp_data%10;      //ȡ������
    ge=temp_data+0x30; 	
}

/*******************************/
void delay(unsigned int k)	
{						
unsigned int i,j;				
	for(i=0;i<k;i++)
	{			
	for(j=0;j<121;j++)			
	{;}}						
}
					

/**************************************
��ʱ5΢��(STC90C52RC---12MHz---12T)
��ͬ�Ĺ�������,��Ҫ�����˺�����ע��ʱ�ӹ���ʱ��Ҫ�޸�
������1T��MCUʱ,���������ʱ����
**************************************/ 
void Delay5us()  //@12.000MHz STC_Y6ָ�
{
	unsigned char i;

	i = 18;
	while (--i);
}

/**************************************
��ʱ5����(STC90C52RC@12M)
��ͬ�Ĺ�������,��Ҫ�����˺���
������1T��MCUʱ,���������ʱ����
**************************************/
void Delay5ms()  //@12.000MHz STC_Y6ָ�
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 78;
	j = 233;
	do
	{
		while (--j);
	} while (--i);
}

/**************************************
��ʼ�ź�
**************************************/
void ADXL345_Start()
{
    SDA = 1;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 0;                    //�����½���
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void ADXL345_Stop()
{
    SDA = 0;                    //����������
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SDA = 1;                    //����������
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(bit ack)
{
    SDA = ack;                  //дӦ���ź�
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
bit ADXL345_RecvACK()
{
    SCL = 1;                    //����ʱ����
    Delay5us();                 //��ʱ
    CY = SDA;                   //��Ӧ���ź�
    SCL = 0;                    //����ʱ����
    Delay5us();                 //��ʱ

    return CY;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void ADXL345_SendByte(BYTE dat)
{
    BYTE i;

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;              //�Ƴ����ݵ����λ
        SDA = CY;               //�����ݿ�
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    ADXL345_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
BYTE ADXL345_RecvByte()
{
    BYTE i;
    BYTE dat = 0;

    SDA = 1;                    //ʹ���ڲ�����,׼����ȡ����,
    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        SCL = 1;                //����ʱ����
        Delay5us();             //��ʱ
        dat |= SDA;             //������               
        SCL = 0;                //����ʱ����
        Delay5us();             //��ʱ
    }
    return dat;
}

//******���ֽ�д��*******************************************

void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    ADXL345_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    ADXL345_Stop();                   //����ֹͣ�ź�
}

//********���ֽڶ�ȡ*****************************************
uchar Single_Read_ADXL345(uchar REG_Address)
{  uchar REG_data;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=ADXL345_RecvByte();              //�����Ĵ�������
	ADXL345_SendACK(1);   
	ADXL345_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
//*********************************************************
//
//��������ADXL345�ڲ����ٶ����ݣ���ַ��Χ0x32~0x37
//
//*********************************************************
void Multiple_read_ADXL345(void)
{   uchar i;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(0x32);                   //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	 for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = ADXL345_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           ADXL345_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //��ӦACK
       }
   }
    ADXL345_Stop();                          //ֹͣ�ź�
    Delay5ms();
}


//*****************************************************************

//��ʼ��ADXL345��������Ҫ��ο�pdf�����޸�************************
void Init_ADXL345()
{
   Single_Write_ADXL345(0x31,0x0B);   //������Χ,����16g��13λģʽ
   Single_Write_ADXL345(0x2C,0x08);   //�����趨Ϊ12.5 �ο�pdf13ҳ
   Single_Write_ADXL345(0x2D,0x08);   //ѡ���Դģʽ   �ο�pdf24ҳ
   Single_Write_ADXL345(0x2E,0x80);   //ʹ�� DATA_READY �ж�
   Single_Write_ADXL345(0x1E,0x00);   //X ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   Single_Write_ADXL345(0x1F,0x00);   //Y ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
   Single_Write_ADXL345(0x20,0x05);   //Z ƫ���� ���ݲ��Դ�������״̬д��pdf29ҳ
}
//***********************************************************************
//��ʾx��
void display_x()
{   float temp;
		char buff[10];
    dis_data=(BUF[1]<<8)+BUF[0];  //�ϳ�����   
	if(dis_data<0){
	dis_data=-dis_data;
    UartSendStr("-");      //��ʾ��������λ
	}
	else  UartSendStr(" "); //��ʾ�ո�

    temp=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
    conversion(temp);          //ת������ʾ��Ҫ������

	buff[0] = qian;
	buff[1] = '.';
	buff[2] = bai;
	buff[3] = shi;
	buff[4] = ' ';
	
	UartSendStr("X:");
	UartSendStr(buff);
	UartSendStr("\r\n");
}

////***********************************************************************
////��ʾy��
//void display_y()
//{     float temp;
//    dis_data=(BUF[3]<<8)+BUF[2];  //�ϳ�����   
//	if(dis_data<0){
//	dis_data=-dis_data;
//    DisplayOneChar(2,1,'-');      //��ʾ��������λ
//	}
//	else DisplayOneChar(2,1,' '); //��ʾ�ո�

//    temp=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
//    conversion(temp);          //ת������ʾ��Ҫ������
//		DisplayOneChar(0,1,'Y');   //��1�У���0�� ��ʾy
//    DisplayOneChar(1,1,':'); 
//    DisplayOneChar(3,1,qian); 
//		DisplayOneChar(4,1,'.'); 
//    DisplayOneChar(5,1,bai); 
//    DisplayOneChar(6,1,shi);  
//		DisplayOneChar(7,1,' ');  
//}

////***********************************************************************
////��ʾz��
//void display_z()
//{
//    float temp;
//    dis_data=(BUF[5]<<8)+BUF[4];    //�ϳ�����   
//	if(dis_data<0){
//	dis_data=-dis_data;
//    DisplayOneChar(10,1,'-');       //��ʾ������λ
//	}
//	else DisplayOneChar(10,1,' ');  //��ʾ�ո�

//    temp=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
//    conversion(temp);          //ת������ʾ��Ҫ������
//		DisplayOneChar(8,1,'Z');  //��0�У���10�� ��ʾZ
//    DisplayOneChar(9,1,':'); 
//    DisplayOneChar(11,1,qian); 
//		DisplayOneChar(12,1,'.'); 
//    DisplayOneChar(13,1,bai); 
//    DisplayOneChar(14,1,shi); 
//		DisplayOneChar(15,1,' ');  
//}


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
//	while(1)                         	//ѭ��
//	{ 
//		
//		Init_ADXL345();                 	//��ʼ��ADXL345
//		devid=Single_Read_ADXL345(0X00);	//����������Ϊ0XE5,��ʾ��ȷ
//		if(devid!=0XE5)
//		{			
//			DisplayOneChar(8,0,'N');
//			DisplayOneChar(9,0,'o');		
//			DisplayOneChar(10,0,' ');		
//			DisplayOneChar(11,0,'F');
//			DisplayOneChar(12,0,'i');
//			DisplayOneChar(13,0,'n');
//			DisplayOneChar(14,0,'d');		
//			DisplayOneChar(15,0,'!');		
//			//
//			DisplayOneChar(0,1,' ');   //��1�У���0�� ��ʾy
//			DisplayOneChar(1,1,' '); 
//			DisplayOneChar(2,1,' '); 	
//			DisplayOneChar(3,1,' '); 
//			DisplayOneChar(4,1,' '); 
//			DisplayOneChar(5,1,' '); 
//			DisplayOneChar(6,1,' ');  
//			DisplayOneChar(7,1,' ');  
//			DisplayOneChar(8,1,' ');  //��0�У���10�� ��ʾZ
//			DisplayOneChar(9,1,' '); 
//			DisplayOneChar(11,1,' '); 
//			DisplayOneChar(12,1,' '); 
//			DisplayOneChar(13,1,' '); 
//			DisplayOneChar(14,1,' '); 
//			DisplayOneChar(15,1,' ');  			
//			
//		}
//		else
//		{			
//			Multiple_Read_ADXL345();       	//�����������ݣ��洢��BUF��
//			display_x();                   	//---------��ʾX��
//			display_y();                   	//---------��ʾY��
//			display_z();                   	//---------��ʾZ��			    
//		}      
//		delay(350);                    	//��ʱ  
//	}
//} 