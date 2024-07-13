#ifndef __PAJIIC_H
#define __PAJIIC_H

#include "code_def.h"
#include <stdio.h>
//IIC���в�������
void    PAJ_IIC_Delay(void);				//MPU IIC��ʱ����				 
void    PAJ_IIC_Start(void);				//����IIC��ʼ�ź�
void    PAJ_IIC_Stop(void);	  			//����IICֹͣ�ź�
void    PAJ_IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t PAJ_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t PAJ_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void    PAJ_IIC_Ack(void);					//IIC����ACK�ź�
void    PAJ_IIC_NAck(void);				//IIC������ACK�ź�
uint8_t paj_read_byte(uint8_t reg_addr);
uint8_t paj_read_nbyte(uint8_t reg_addr,uint8_t len,uint8_t *buf);
uint8_t paj_write_byte(uint8_t reg_addr,uint8_t data);
uint8_t paj_wakeup();
uint8_t paj_init();
uint8_t Gesture_test(void);
#endif