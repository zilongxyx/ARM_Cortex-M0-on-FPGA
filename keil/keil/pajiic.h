#ifndef __PAJIIC_H
#define __PAJIIC_H

#include "code_def.h"
#include <stdio.h>
//IIC所有操作函数
void    PAJ_IIC_Delay(void);				//MPU IIC延时函数				 
void    PAJ_IIC_Start(void);				//发送IIC开始信号
void    PAJ_IIC_Stop(void);	  			//发送IIC停止信号
void    PAJ_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t PAJ_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t PAJ_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void    PAJ_IIC_Ack(void);					//IIC发送ACK信号
void    PAJ_IIC_NAck(void);				//IIC不发送ACK信号
uint8_t paj_read_byte(uint8_t reg_addr);
uint8_t paj_read_nbyte(uint8_t reg_addr,uint8_t len,uint8_t *buf);
uint8_t paj_write_byte(uint8_t reg_addr,uint8_t data);
uint8_t paj_wakeup();
uint8_t paj_init();
uint8_t Gesture_test(void);
#endif