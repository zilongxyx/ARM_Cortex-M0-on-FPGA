#ifndef _SFLASH_H
#define _SFLASH_H

#include <stdint.h>

/* �궨�� --------------------------------------------------------------------*/

/* ָ��� */
#define SFLASH_WRITE_ENABLE       0x06                     //дʹ��
#define SFLASH_WRITE_DISABLE      0x04                     //дʧ��
#define SFLASH_READ_STATUS_REG    0x05                     //��״̬�Ĵ���
#define SFLASH_WRITE_STATUS_REG   0x01                     //д״̬�Ĵ���

#define SFLASH_READ_DATA          0x03                     //������
#define SFLASH_FAST_READ          0x0B                     //�������
#define SFLASH_FAST_READ_DUAL     0x3B                     //�������(˫���������)
#define SFLASH_WRITE_PAGE         0x02                     //ҳ���
#define SFLASH_ERASE_BLOCK        0xD8                     //������
#define SFLASH_ERASE_SECTOR       0x20                     //��������
#define SFLASH_ERASE_CHIP         0xC7                     //����оƬ
#define SFLASH_POWER_DOWN         0xB9                     //����
#define SFLASH_RELEASE_POWER_DOWN 0xAB                     //�ͷŵ���
#define SFLASH_DEVICE_ID          0x90                     //�豸ID
#define SFLASH_JEDEC_ID           0x9F                     //Jedec ID

/* �������� ------------------------------------------------------------------*/
void SFLASH_Initializes(void);
uint8_t SPI_WriteReadByte(uint8_t TxData);

static void SFLASH_WriteEnable(void);                      //дʹ��
static void SFLASH_WriteDisable(void);                     //дʧ��
uint8_t SFLASH_ReadSR(void);                               //��״̬�Ĵ���
void SFLASH_WriteSR(uint8_t SR);                           //д״̬�Ĵ���
void SFLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte);         //��ȡn�ֽ�����
void SFLASH_FastReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte);     //���n�ֽ�����
void SFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);        //д��n(<256�ֽ�)����

void SFLASH_WriteNoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);     //д��n�ֽ�����(��У��)
void SFLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);       //д��n�ֽ�����
void SFLASH_WaitForNoBusy(void);                           //�ȴ���æ

void SFLASH_EraseBlock(uint32_t BlockAddr);                //������
void SFLASH_EraseSector(uint32_t SectorAddr);              //��������
void SFLASH_EraseChip(void);                               //��������оƬ
void SFLASH_PowerDown(void);                               //����ģʽ
void SFLASH_WAKEUP(void);                                  //��������
uint16_t SFLASH_ReadID(void);                              //��ȡSFLASH ID
uint32_t SFLASH_ReadJEDEC_ID(void);                        //��ȡJEDEC_ID


#endif /* _SFLASH_H */
