#ifndef __ld3320_H
#define __ld3320_H
#include "stdio.h"
#include "stdint.h"
#include "code_def.h"
#define u8 uint8_t
#define uint8 uint8_t
#define u16 uint16_t
extern u8 temp[10];
///ʶ���루�ͻ��޸Ĵ���
#define CODE_KL   0x8  //����
#define CODE_LSD	1	 //��ˮ��
#define CODE_SS	  2	 //��˸
#define CODE_AJCF	3	 //��������
#define CODE_QM	  4	 //ȫ��
#define CODE_JT		5  //״̬
#define CODE_START_SHOW 6
///��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 		0x40

///�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE					0x00	//��ʾû������ASRʶ��
#define LD_ASR_RUNING				0x01	//��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 			0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬


#define LD3320_MODE_KL 0	//����
#define LD3320_MODE_ZC 1	//�ǿ���

//	��������״̬����������¼������������ASRʶ����������MP3����
#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 		0x40


//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_NONE					0x00	//	��ʾû������ASRʶ��
#define LD_ASR_RUNING				0x01	//	��ʾLD3320������ASRʶ����
#define LD_ASR_FOUNDOK			0x10	//	��ʾһ��ʶ�����̽�������һ��ʶ����
#define LD_ASR_FOUNDZERO 		0x11	//	��ʾһ��ʶ�����̽�����û��ʶ����
#define LD_ASR_ERROR	 			0x31	//	��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬


#define CLK_IN   					24/* user need modify this value according to clock in */
#define LD_PLL_11					(uint8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19			0x0f
#define LD_PLL_MP3_1B			0x18
#define LD_PLL_MP3_1D   	(uint8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f
// LD chip fixed values.
#define        RESUM_OF_MUSIC               0x01
#define        CAUSE_MP3_SONG_END           0x20

#define        MASK_INT_SYNC								0x10
#define        MASK_INT_FIFO								0x04
#define    	   MASK_AFIFO_INT								0x01
#define        MASK_FIFO_STATUS_AFULL				0x08
#define MIC_VOL 0x43

///LD3320������ض���
#define LD_RST_H() 						IO->IO_MODE[0] = 1
#define LD_RST_L() 						IO->IO_MODE[0] = 0


#define LD_CS_H()							SPI->SPI_CS  = 1
#define LD_CS_L()							SPI->SPI_CS  = 0


#define LD_SPIS_H()  					IO->IO_MODE[1] = 1
#define LD_SPIS_L()  					IO->IO_MODE[1] = 0

extern u8 LD3320_Mode;
extern u8 LD3320_KL_Flag;
///�û��޸ĺ���
static uint8 LD_AsrAddFixed(void);
static void Board_text(uint8 Code_Val);
//extern void Delayms(uint16 i);
static void LD3320_OUT_GPIO_Cfg(void);   

///�м��
static uint8 RunASR(void);
void LD_reset(void);  //��ʼ��
static void LD_AsrStart(void);
static uint8 LD_Check_ASRBusyFlag_b2(void);
static void LD3320_delay(unsigned long uldata);

///�Ĵ�������
uint8 spi_send_byte(uint8 byte);
void LD_WriteReg(uint8 data1,uint8 data2);
uint8 LD_ReadReg(uint8 reg_add);
static uint8 LD_GetResult(void);
static uint8 LD_AsrRun(void);
void ProcessInt(void);
static void LD_Init_Common(void);
static void LD_Init_ASR(void);
uint8_t Ld3320_Fun(void);
#endif
