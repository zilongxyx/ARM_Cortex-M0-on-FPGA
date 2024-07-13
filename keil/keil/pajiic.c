#include "pajiic.h"
#include "paj7620u2_cfg.h"
char test1_string[10];
//MPU IIC 延时函数
void PAJ_IIC_Delay(void)
{
	delay_us(1);
}

//产生IIC起始信号
void PAJ_IIC_Start(void)
{
	PAJ->PAJ_ACK=0;			 //sda线输出
	PAJ->PAJ_IIC_SDA=1;	  	  
	PAJ->PAJ_IIC_SCL=1;
	PAJ_IIC_Delay();
 	PAJ->PAJ_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	PAJ_IIC_Delay();
	PAJ->PAJ_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void PAJ_IIC_Stop(void)
{
	PAJ->PAJ_ACK=0;			 //sda线输出
	PAJ->PAJ_IIC_SCL=0;
	PAJ->PAJ_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	PAJ_IIC_Delay();
	PAJ->PAJ_IIC_SCL=1;  
	PAJ->PAJ_IIC_SDA=1;//发送I2C总线结束信号
	PAJ_IIC_Delay();							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t PAJ_IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	PAJ->PAJ_ACK=1;			 PAJ_IIC_Delay();		//释放SDA总线				
	PAJ->PAJ_IIC_SCL=1;PAJ_IIC_Delay();	 
	while(PAJ->PAJ_IIC_SDA == 1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			PAJ_IIC_Stop();
			return 1;
		}
	}
	PAJ->PAJ_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void PAJ_IIC_Ack(void)
{
	PAJ->PAJ_IIC_SCL=0;
	PAJ->PAJ_ACK=0;
	PAJ->PAJ_IIC_SDA=0;
	PAJ_IIC_Delay();
	PAJ->PAJ_IIC_SCL=1;
	PAJ_IIC_Delay();
	PAJ->PAJ_IIC_SCL=0;
}
//不产生ACK应答		    
void PAJ_IIC_NAck(void)
{
	PAJ->PAJ_IIC_SCL=0;
	PAJ->PAJ_ACK=0;
	PAJ->PAJ_IIC_SDA=1;
	PAJ_IIC_Delay();
	PAJ->PAJ_IIC_SCL=1;
	PAJ_IIC_Delay();
	PAJ->PAJ_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void PAJ_IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;
		PAJ->PAJ_ACK=0;			 //回收SDA使用权
    PAJ->PAJ_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
      PAJ->PAJ_IIC_SDA=(txd&0x80)>>7;
      txd<<=1; 	  
			PAJ->PAJ_IIC_SCL=1;
			PAJ_IIC_Delay(); 
			PAJ->PAJ_IIC_SCL=0;	
			PAJ_IIC_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t PAJ_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	PAJ->PAJ_ACK=1;
    for(i=0;i<8;i++)
	  {
      PAJ->PAJ_IIC_SCL=0; 
      PAJ_IIC_Delay();
		  PAJ->PAJ_IIC_SCL=1;
      receive<<=1;
      if(PAJ->PAJ_IIC_SDA)receive++;   
		  PAJ_IIC_Delay(); 
    }					 
    if (!ack)
        PAJ_IIC_NAck();//发送nACK
    else
        PAJ_IIC_Ack(); //发送ACK   
    return receive;
}
uint8_t paj_read_byte(uint8_t reg_addr)
{
	uint8_t reg_data;
	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE6);
	if(PAJ_IIC_Wait_Ack())
	{
		PAJ_IIC_Stop();
		return 0;
	}
	PAJ_IIC_Send_Byte(reg_addr);
	PAJ_IIC_Wait_Ack();
	
	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE7);
	PAJ_IIC_Wait_Ack();
	reg_data = PAJ_IIC_Read_Byte(0);
	PAJ_IIC_Stop();
	
	return reg_data;
}
uint8_t paj_read_nbyte(uint8_t reg_addr,uint8_t len,uint8_t *buf)
{
	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE6);//·￠D′?üá?
	if(PAJ_IIC_Wait_Ack())
	{
		PAJ_IIC_Stop();
/*		while(1)
		{
			UART1String("grm\r\n");
		}
*/
		return 1;
	}
	PAJ_IIC_Send_Byte(reg_addr);
	PAJ_IIC_Wait_Ack();

	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE7);//·￠?á?üá?
	PAJ_IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf = PAJ_IIC_Read_Byte(0);
		}
		else
		{
			*buf = PAJ_IIC_Read_Byte(1);
		}
		buf++;
		len--;
	}
	PAJ_IIC_Stop();//êí·?×ü??

	return 0;	
}
uint8_t paj_write_byte(uint8_t reg_addr,uint8_t data)
{
	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE6);
	if(PAJ_IIC_Wait_Ack())
	{
		PAJ_IIC_Stop();
		return 1;
	}
	PAJ_IIC_Send_Byte(reg_addr);
	PAJ_IIC_Wait_Ack();
	PAJ_IIC_Send_Byte(data);
	PAJ_IIC_Wait_Ack();
	PAJ_IIC_Stop();
	
	return 0;
}
uint8_t paj_wakeup()
{
	uint8_t data=0x0a;
	//wake up
	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE6);
	PAJ_IIC_Stop();
	delay_ms(5);//
	
	PAJ_IIC_Start();
	PAJ_IIC_Send_Byte(0XE6);
	PAJ_IIC_Stop();
	delay_ms(5);
	
	paj_write_byte(0XEF,0X00);//bank0
//	data = paj_read_byte(0x00);
	paj_read_nbyte(0x00,1,&data);
	if(data!=0x20) return 0;
//	while(data==0x20)
//	{
//		UART1String("113\r\n"); 	
//	}
	return 1;	
}
uint8_t paj_init()
{
	uint8_t i;
	uint8_t status;
	
	status = paj_wakeup();
	if(!status)
		return 0;
	paj_write_byte(0XEF,0X00);//bank0
	
	for(i=0;i<INIT_SIZE;i++)
	{
		paj_write_byte(init_Array[i][0],init_Array[i][1]);
	}
//	paj_write_byte(0xEF,0x00);
//	paj_write_byte(0x37,0x07);
//	paj_write_byte(0x38,0x17);
//	paj_write_byte(0x39,0x06);
//	paj_write_byte(0x41,0x00);
//	paj_write_byte(0x42,0x00);
//	paj_write_byte(0x46,0x2D);
//	paj_write_byte(0x47,0x0F);
//	paj_write_byte(0x48,0x3C);
//	paj_write_byte(0x49,0x00);
//	paj_write_byte(0x4A,0x1E);
//	paj_write_byte(0x4C,0x20);
//	paj_write_byte(0x51,0x10);
//	paj_write_byte(0x5E,0x10);
//	paj_write_byte(0x60,0x27);
//	paj_write_byte(0x80,0x42);
//	paj_write_byte(0x81,0x44);
//	paj_write_byte(0x82,0x04);
//	paj_write_byte(0x8B,0x01);
//	paj_write_byte(0x90,0x06);
//	paj_write_byte(0x95,0x0A);
//	paj_write_byte(0x96,0x0C);
//	paj_write_byte(0x97,0x05);
//	paj_write_byte(0x9A,0x14);
//	paj_write_byte(0x9C,0x3F);
//	paj_write_byte(0xA5,0x19);
//	paj_write_byte(0xCC,0x19);
//	paj_write_byte(0xCD,0x0B);
//	paj_write_byte(0xCE,0x13);
//	paj_write_byte(0xCF,0x64);
//	paj_write_byte(0xD0,0x21);
//	paj_write_byte(0xEF,0x01);
//	paj_write_byte(0x02,0x0F);
//	paj_write_byte(0x03,0x10);
//	paj_write_byte(0x04,0x02);
//	paj_write_byte(0x25,0x01);
//	paj_write_byte(0x27,0x39);
//	paj_write_byte(0x28,0x7F);
//	paj_write_byte(0x29,0x08);
//	paj_write_byte(0x3E,0xFF);
//	paj_write_byte(0x5E,0x3D);
//	paj_write_byte(0x65,0x96);
//	paj_write_byte(0x67,0x97);
//	paj_write_byte(0x69,0xCD);
//	paj_write_byte(0x6A,0x01);
//	paj_write_byte(0x6D,0x2C);
//	paj_write_byte(0x6E,0x01);
//	paj_write_byte(0x72,0x01);
//	paj_write_byte(0x73,0x35);
//	paj_write_byte(0x74,0x00);
//	paj_write_byte(0x77,0x01);
	//
    paj_write_byte(0XEF,0X00);//bank0
	
	return 1;	
}
uint8_t Gesture_test(void)
{
	uint8_t i;
    uint8_t status;
	uint8_t key = 0;
	uint8_t data[2]={0x00};
	uint16_t gesture_data;
	uint8_t ledflash=0;
	
	paj_write_byte(0XEF,0X00);//bank0
	for(i=0;i<GESTURE_SIZE;i++)
	{
		paj_write_byte(gesture_arry[i][0],gesture_arry[i][1]);//ê?ê?ê?±e?￡ê?3?ê??ˉ
	}
	paj_write_byte(0XEF,0X00);//bank0
	i=0;

	//paj_write_byte(0x41,0x00);		
	status = paj_read_nbyte(0x43,2,&data[0]);//?áè?ê?ê?×′ì?			
	if(!status)
	{   
		gesture_data =(uint16_t)data[1]<<8 | data[0];
		//sprintf(test1_string,"%d \r\n",data[0]);
		//UART1String(test1_string);
		if(gesture_data) 
		{
			switch(gesture_data)
			{
				case GES_UP:              key = 1;UART1String("UP\r\n");break; 
				case GES_DOWM:            key = 2;UART1String("DOWN\r\n");break; 
				case GES_LEFT:     		  key = 3;UART1String("LEFT\r\n");break; 
				case GES_RIGHT:    		  key = 4;UART1String("RIGHT\r\n");break; 
				case GES_FORWARD:  	      key = 5;UART1String("FORWARD\r\n");break; 
				case GES_BACKWARD: 		  key = 6;UART1String("BACKWORD\r\n");break; 
				case GES_CLOCKWISE:       key = 7;UART1String("Clockwise\r\n");break;      
				case GES_COUNT_CLOCKWISE: key = 8;UART1String("AntiClockwise\r\n");break;   
				case GES_WAVE:     		  key = 9;UART1String("Wave\r\n");        break; 				
			}	
					
		}
		else
		{
			//UART1String("11\r\n");
		}
				   
	}	
	return key;
}

