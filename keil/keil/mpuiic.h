#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "code_def.h"


//IIC所有操作函数
void    MPU_IIC_Delay(void);				//MPU IIC延时函数				 
void    MPU_IIC_Start(void);				//发送IIC开始信号
void    MPU_IIC_Stop(void);	  			//发送IIC停止信号
void    MPU_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint32_t MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void    MPU_IIC_Ack(void);					//IIC发送ACK信号
void    MPU_IIC_NAck(void);				//IIC不发送ACK信号
uint32_t ADC_value(uint8_t data);       //
void channel_select();
#endif
















