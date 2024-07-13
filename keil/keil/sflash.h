#ifndef _SFLASH_H
#define _SFLASH_H

#include <stdint.h>

/* 宏定义 --------------------------------------------------------------------*/

/* 指令表 */
#define SFLASH_WRITE_ENABLE       0x06                     //写使能
#define SFLASH_WRITE_DISABLE      0x04                     //写失能
#define SFLASH_READ_STATUS_REG    0x05                     //读状态寄存器
#define SFLASH_WRITE_STATUS_REG   0x01                     //写状态寄存器

#define SFLASH_READ_DATA          0x03                     //读数据
#define SFLASH_FAST_READ          0x0B                     //快读数据
#define SFLASH_FAST_READ_DUAL     0x3B                     //快读数据(双数据线输出)
#define SFLASH_WRITE_PAGE         0x02                     //页编程
#define SFLASH_ERASE_BLOCK        0xD8                     //擦除块
#define SFLASH_ERASE_SECTOR       0x20                     //擦除扇区
#define SFLASH_ERASE_CHIP         0xC7                     //擦除芯片
#define SFLASH_POWER_DOWN         0xB9                     //掉电
#define SFLASH_RELEASE_POWER_DOWN 0xAB                     //释放掉电
#define SFLASH_DEVICE_ID          0x90                     //设备ID
#define SFLASH_JEDEC_ID           0x9F                     //Jedec ID

/* 函数申明 ------------------------------------------------------------------*/
void SFLASH_Initializes(void);
uint8_t SPI_WriteReadByte(uint8_t TxData);

static void SFLASH_WriteEnable(void);                      //写使能
static void SFLASH_WriteDisable(void);                     //写失能
uint8_t SFLASH_ReadSR(void);                               //读状态寄存器
void SFLASH_WriteSR(uint8_t SR);                           //写状态寄存器
void SFLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte);         //读取n字节数据
void SFLASH_FastReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte);     //快读n字节数据
void SFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);        //写入n(<256字节)数据

void SFLASH_WriteNoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);     //写入n字节数据(无校验)
void SFLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte);       //写入n字节数据
void SFLASH_WaitForNoBusy(void);                           //等待不忙

void SFLASH_EraseBlock(uint32_t BlockAddr);                //擦除块
void SFLASH_EraseSector(uint32_t SectorAddr);              //擦除扇区
void SFLASH_EraseChip(void);                               //擦除整个芯片
void SFLASH_PowerDown(void);                               //掉电模式
void SFLASH_WAKEUP(void);                                  //唤醒器件
uint16_t SFLASH_ReadID(void);                              //读取SFLASH ID
uint32_t SFLASH_ReadJEDEC_ID(void);                        //读取JEDEC_ID


#endif /* _SFLASH_H */
