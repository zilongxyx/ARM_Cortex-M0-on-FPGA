#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "code_def.h"


//IIC���в�������
void    MPU_IIC_Delay(void);				//MPU IIC��ʱ����				 
void    MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void    MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void    MPU_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint32_t MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void    MPU_IIC_Ack(void);					//IIC����ACK�ź�
void    MPU_IIC_NAck(void);				//IIC������ACK�ź�
uint32_t ADC_value(uint8_t data);       //
void channel_select();
#endif
















