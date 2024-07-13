#include "mpuiic.h"
extern uint8_t channel;
extern uint8_t adc_value[4];
//MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
	delay_us(1);
}

//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
	QN8027->ACK=0;			 //sda�����
	QN8027->QN_IIC_SDA=1;	  	  
	QN8027->QN_IIC_SCL=1;
	MPU_IIC_Delay();
 	QN8027->QN_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MPU_IIC_Delay();
	QN8027->QN_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
	QN8027->ACK=0;			 //sda�����
	QN8027->QN_IIC_SCL=0;
	QN8027->QN_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	MPU_IIC_Delay();
	QN8027->QN_IIC_SCL=1;  
	QN8027->QN_IIC_SDA=1;//����I2C���߽����ź�
	MPU_IIC_Delay();							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t MPU_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	QN8027->ACK=1;			 MPU_IIC_Delay();		//�ͷ�SDA����				
	QN8027->QN_IIC_SCL=1;MPU_IIC_Delay();	 
	while(QN8027->QN_IIC_SDA == 1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	QN8027->QN_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MPU_IIC_Ack(void)
{
	QN8027->QN_IIC_SCL=0;
	QN8027->ACK=0;
	QN8027->QN_IIC_SDA=0;
	MPU_IIC_Delay();
	QN8027->QN_IIC_SCL=1;
	MPU_IIC_Delay();
	QN8027->QN_IIC_SCL=0;
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
	QN8027->QN_IIC_SCL=0;
	QN8027->ACK=0;
	QN8027->QN_IIC_SDA=1;
	MPU_IIC_Delay();
	QN8027->QN_IIC_SCL=1;
	MPU_IIC_Delay();
	QN8027->QN_IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;
		QN8027->ACK=0;			 //����SDAʹ��Ȩ
    QN8027->QN_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
      QN8027->QN_IIC_SDA=(txd&0x80)>>7;
      txd<<=1; 	  
			QN8027->QN_IIC_SCL=1;
			MPU_IIC_Delay(); 
			QN8027->QN_IIC_SCL=0;	
			MPU_IIC_Delay();
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint32_t MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i=0;
	uint32_t receive=0;
	QN8027->ACK=1;
    for(i=0;i<8;i++)
	  {
      QN8027->QN_IIC_SCL=0; 
      MPU_IIC_Delay();
		  MPU_IIC_Delay();
		  MPU_IIC_Delay();
		  MPU_IIC_Delay();
		  MPU_IIC_Delay();
		  QN8027->QN_IIC_SCL=1;
      receive<<=1;
      if(QN8027->QN_IIC_SDA)receive++;   
		  MPU_IIC_Delay(); 
		  MPU_IIC_Delay();
		  MPU_IIC_Delay();
		  MPU_IIC_Delay();
		  MPU_IIC_Delay();
    }					 
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK   
    return receive;
}
uint32_t ADC_value(uint8_t data)
{		
	uint32_t value = 0;
	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte(0x90);
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();	
		return 1;		
	}
	MPU_IIC_Send_Byte(data);	//д�Ĵ�����ַ
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();	
		return 1;		
	}
//	MPU_IIC_Stop();	
	
	MPU_IIC_Start(); 
	MPU_IIC_Send_Byte(0x91);
	if(MPU_IIC_Wait_Ack())	//�ȴ�Ӧ��
	{
		MPU_IIC_Stop();	
		return 1;		
	}
	value = MPU_IIC_Read_Byte(1);
	MPU_IIC_Stop();
	return value;
}
void channel_select()
{
	switch(channel)
	{
		case 1:  adc_value[0]=ADC_value(0x00); break;
		//case 2: ADC_value(0x01);adc_value[1]=ADC_value(0x01);break;
		//case 3: ADC_value(0x02);adc_value[1]=ADC_value(0x02);break;
		//case 4: ADC_value(0x03);adc_value[3]=ADC_value(0x03);break;
	}
	channel++;
	if(channel>2)
		channel=0;
}
