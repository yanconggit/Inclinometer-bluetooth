#ifndef ADXL345_H
#define ADXL345_H

#define   uchar unsigned char
#define   uint unsigned int	
	
typedef unsigned char  BYTE;
typedef unsigned short WORD;

void delay(unsigned int k);
void Init_ADXL345(void);             //��ʼ��ADXL345

void conversion(uint temp_data);

void  Single_Write_ADXL345(uchar REG_Address,uchar REG_data);   //����д������
uchar Single_Read_ADXL345(uchar REG_Address);                   //������ȡ�ڲ��Ĵ�������
void  Multiple_Read_ADXL345();                                  //�����Ķ�ȡ�ڲ��Ĵ�������
//------------------------------------
void Delay5us();
void Delay5ms();
void ADXL345_Start();
void ADXL345_Stop();
void ADXL345_SendACK(bit ack);
bit  ADXL345_RecvACK();
void ADXL345_SendByte(BYTE dat);
BYTE ADXL345_RecvByte();
void ADXL345_ReadPage();
void ADXL345_WritePage();

void display_x();

#endif


