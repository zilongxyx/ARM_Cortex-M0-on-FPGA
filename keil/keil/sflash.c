/******************************** SPI FLASH��Ϣ ********************************
SPI FLASH����ܶ�, ���������������

1��W25X16��Ϣ��
ҳ(Page) ------ 256Byte
����(Sector) -- 4KByte ------ (����16ҳ)
��(Block) ----- 64KByte ----- (����16����)

W25X16оƬ������2MB (16Mbit)
          ҳ����16*16*32 (2M/256)
        ��������16*32
          ������32

2����д������
�� ------------ һ������һҳ(256B)
д ------------ ҳ
���� ---------- �������顢����оƬ

3�����ƺ�״̬�Ĵ�������(Ĭ��:0x00)
BITλ  7   6   5   4   3   2   1   0
      SPR  RV  TB  BP2 BP1 BP0 WEL BUSY
SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
TB,BP2,BP1,BP0:FLASH����д��������
WEL:дʹ������
BUSY:æ���λ(1,æ;0,����)
********************************************************************************/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "sflash.h"
#include "code_def.h"


/************************************************
�������� �� SFLASH_WriteEnable
��    �� �� SPI_FLASHдʹ�ܣ���WEL��λ
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SFLASH_WriteEnable(void)
{
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_WRITE_ENABLE);            //��дʹ�ܡ�ָ��
  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_WriteDisable
��    �� �� SPI_FLASHд��ֹ,��WEL����
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
static void SFLASH_WriteDisable(void)
{
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_WRITE_DISABLE);           //��дʧ�ܡ�ָ��
  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_ReadSR
��    �� �� ��ȡSFLASH״̬�Ĵ���
��    �� �� ��
�� �� ֵ �� Byte --- ��ȡ�ֽ�
*************************************************/
uint8_t SFLASH_ReadSR(void)
{
  uint8_t data_tmp;
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_READ_STATUS_REG);         //����״̬�Ĵ�����ָ��
  data_tmp = SPI_ReadByte();                     //��ȡһ���ֽ�
  W25Q16->SPI_CS = 1;                            //ʧ������
  return data_tmp;
}

/************************************************
�������� �� SFLASH_WriteSR
��    �� �� дSFLASH״̬�Ĵ���
��    �� �� SR --- д״̬�Ĵ�������
�� �� ֵ �� ��
*************************************************/
void SFLASH_WriteSR(uint8_t SR)
{
  SFLASH_WriteEnable();                          //дʹ��
  SPI_WriteByte(SFLASH_WRITE_STATUS_REG);        //��д״̬�Ĵ�����ָ��
  SPI_WriteByte(SR);                             //д��һ���ֽ�
  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_ReadNByte
��    �� �� ��ReadAddr��ַ��ʼ������ȡSFLASH��nByte
��    �� �� pBuffer ---- ���ݴ洢���׵�ַ
            ReadAddr --- Ҫ��ȡSFLASH Flash���׵�ַ��ַ
            nByte ------ Ҫ��ȡ���ֽ���(���65535B = 64K ��)
�� �� ֵ �� ��
*************************************************/
void SFLASH_ReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_READ_DATA);               //�������ݡ�ָ��
  SPI_WriteByte((uint8_t)((ReadAddr)>>16));      //����24bit��ַ
  SPI_WriteByte((uint8_t)((ReadAddr)>>8));
  SPI_WriteByte((uint8_t)ReadAddr);

  while(nByte--)                                 //ѭ������
  {
    *pBuffer = SPI_ReadByte();
    pBuffer++;
  }

  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_FastReadNByte
��    �� �� ��ReadAddr��ַ��ʼ�������ٶ�ȡSFLASH��nByte
��    �� �� pBuffer ---- ���ݴ洢���׵�ַ
            ReadAddr --- Ҫ��ȡSFLASH Flash���׵�ַ��ַ
            nByte ------ Ҫ��ȡ���ֽ���(���65535B = 64K ��)
�� �� ֵ �� ��
*************************************************/
void SFLASH_FastReadNByte(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t nByte)
{
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_FAST_READ);               //��������ݡ�ָ��
  SPI_WriteByte((uint8_t)((ReadAddr)>>16));      //����24bit��ַ
  SPI_WriteByte((uint8_t)((ReadAddr)>>8));
  SPI_WriteByte((uint8_t)ReadAddr);
  SPI_WriteByte(0xFF);                           //�ȴ�8��ʱ��

  while(nByte--)                                 //ѭ������
  {
    *pBuffer = SPI_ReadByte();
    pBuffer++;
  }

  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_WritePage
��    �� �� ��SFLASH��д������1ҳ(256���ֽ�)������
��    �� �� pBuffer ----- д���������׵�ַ
            WriteAddr --- Ҫд��Flash�ĵ�ַ
            nByte ------- Ҫд����ֽ���(���1ҳ)
�� �� ֵ �� ��
*************************************************/
void SFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
  SFLASH_WriteEnable();                          //дʹ��

  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_WRITE_PAGE);              //��ҳ��̡�ָ��
  SPI_WriteByte((uint8_t)((WriteAddr)>>16));     //����24bit��ַ
  SPI_WriteByte((uint8_t)((WriteAddr)>>8));
  SPI_WriteByte((uint8_t)WriteAddr);

  while (nByte--)
  {
    SPI_WriteByte(*pBuffer);
    pBuffer++;
  }

  W25Q16->SPI_CS = 1;
  SFLASH_WaitForNoBusy();                        //�ȴ����У��ȴ�д�������
}

/************************************************
�������� �� SFLASH_WriteNoCheck
��    �� �� �޼���дSFLASH
            ����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0xFF,�����ڷ�0xFF��д������ݽ�ʧ��!
            �����Զ���ҳ����
            ��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
��    �� �� pBuffer ----- д���������׵�ַ
            WriteAddr --- Ҫд��Flash�ĵ�ַ
            nByte ------- Ҫд����ֽ���
�� �� ֵ �� ��
*************************************************/
void SFLASH_WriteNoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
  uint16_t PageRemain = 256 - WriteAddr%256;     //��ҳʣ���д���ֽ���

  if(nByte <= PageRemain)
    PageRemain = nByte;                          //������256���ֽ�

  while(1)
  {
    SFLASH_WritePage(pBuffer, WriteAddr, PageRemain);
    if(nByte == PageRemain)                      //д�����
      break;
    else                                         //д��δ����
    {
      pBuffer += PageRemain;                     //��һҳд������
      WriteAddr += PageRemain;                   //��һҳд�����ݵ�ַ
      nByte -= PageRemain;                       //��д���ֽ����ݼ�

      if(nByte > 256)
        PageRemain = 256;                        //��д��1ҳ(256�ֽ�)������
      else
        PageRemain = nByte;                      //��д������1ҳ(256�ֽ�)������
    }
  }
}


/************************************************
�������� �� SFLASH_WriteNByte
��    �� �� ��WriteAddr��ַ��ʼ����д��nByte��SFLASH��
��    �� �� pBuffer ----- д���������׵�ַ
            WriteAddr --- Ҫд��Flash�ĵ�ַ
            nByte ------- Ҫд����ֽ���(���65535B = 64K ��)
�� �� ֵ �� ��
*************************************************/
void SFLASH_WriteNByte(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t nByte)
{
  static uint8_t SectorBuf[4096];                //����buf
  uint32_t SecPos;                               //����λ��
  uint16_t SecOff;                               //����ƫ��
  uint16_t SecRemain;                            //ʣ������
  uint16_t i;

  SecPos = WriteAddr/4096;                       //��ַ��������(0~511)
  SecOff = WriteAddr%4096;                       //��ַ����������ƫ��
  SecRemain = 4096-SecOff;                       //��ַ��������ʣ���ֽ���(������С4096B=4KB)

  if(nByte <= SecRemain)
    SecRemain = nByte;                           //д�����ݴ�С < ʣ��ռ��С (��ʣ��ռ乻������Щ����)

  while(1)
  {
    /* ��1����У�� */
    SFLASH_ReadNByte(SectorBuf, SecPos*4096, 4096);        //������������������
    for(i=0; i<SecRemain; i++)                             //У������,�Ƿ���Ҫ����
    {
      if(SectorBuf[SecOff + i] != 0xFF)                    //�洢���ݲ�Ϊ0xFF ����Ҫ����
        break;
    }
    if(i < SecRemain)                                      //��Ҫ����
    {
      SFLASH_EraseSector(SecPos);                          //����������
      for(i=0; i<SecRemain; i++)                           //����д�������(��1��ʱ����д������������ʣ��Ŀռ�)
      {
        SectorBuf[SecOff + i] = pBuffer[i];
      }
      SFLASH_WriteNoCheck(SectorBuf, SecPos*4096, 4096);   //д���������������� = ������ + ��д�����ݣ�
    }
    else
      SFLASH_WriteNoCheck(pBuffer, WriteAddr, SecRemain);  //����Ҫ����,ֱ��д������ʣ��ռ�

    if(nByte == SecRemain)                       //д�����
    {
      SFLASH_WriteDisable();                     //дʧ��, �˳�д
      break;
    }
    else                                         //д��δ����
    {
      SecPos++;                                  //������ַ��1
      SecOff = 0;                                //ƫ��λ�ù���
      pBuffer += SecRemain;                      //ָ��ƫ��
      WriteAddr += SecRemain;                    //д��ַƫ��
      nByte -= SecRemain;                        //��д���ֽ����ݼ�
      if(nByte > 4096)
        SecRemain = 4096;                        //��д��1����(4096�ֽ�)������
      else
        SecRemain = nByte;                       //��д������1����(4096�ֽ�)������
    }
  }
}

/************************************************
�������� �� SFLASH_WaitForNoBusy
��    �� �� �ȴ���æ
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void SFLASH_WaitForNoBusy(void)
{
  while((SFLASH_ReadSR()&0x01)==0x01);           //�ȴ�BUSYλ���
}

/************************************************
�������� �� SFLASH_EraseBlock
��    �� �� ������
            ��������Ҫһ��ʱ��
��    �� �� BlockAddr --- ���ַ 0~31
�� �� ֵ �� ��
*************************************************/
void SFLASH_EraseBlock(uint32_t BlockAddr)
{
  BlockAddr *= 65536;                            //���׵�ַ
  SFLASH_WriteEnable();                          //дʹ��
  SFLASH_WaitForNoBusy();
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_ERASE_BLOCK);             //�������顷ָ��
  SPI_WriteByte((uint8_t)((BlockAddr)>>16));     //������ַ
  SPI_WriteByte((uint8_t)((BlockAddr)>>8));
  SPI_WriteByte((uint8_t)BlockAddr);
  W25Q16->SPI_CS = 1;

  SFLASH_WaitForNoBusy();                        //�ȴ��������
}

/************************************************
�������� �� SFLASH_EraseSector
��    �� �� ��������
��    �� �� SectorAddr --- ������ַ 0~511
�� �� ֵ �� ��
*************************************************/
void SFLASH_EraseSector(uint32_t SectorAddr)
{
  SectorAddr *= 4096;                            //�����׵�ַ
  SFLASH_WriteEnable();                          //дʹ��
  SFLASH_WaitForNoBusy();
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_ERASE_SECTOR);            //������������ָ��
  SPI_WriteByte((uint8_t)((SectorAddr)>>16));    //������ַ
  SPI_WriteByte((uint8_t)((SectorAddr)>>8));
  SPI_WriteByte((uint8_t)SectorAddr);
  W25Q16->SPI_CS = 1;

  SFLASH_WaitForNoBusy();                        //�ȴ��������
}

/************************************************
�������� �� SFLASH_EraseChip
��    �� �� ��������оƬ(��Ƭ����ʱ��ϳ�)
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void SFLASH_EraseChip(void)
{
  SFLASH_WriteEnable();                          //дʹ��
  SFLASH_WaitForNoBusy();
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_ERASE_CHIP);              //������оƬ��ָ��
  W25Q16->SPI_CS = 1;

  SFLASH_WaitForNoBusy();                        //�ȴ�оƬ��������
}

/************************************************
�������� �� SFLASH_PowerDown
��    �� �� �������ģʽ
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void SFLASH_PowerDown(void)
{
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_POWER_DOWN);              //�����硷ָ��
  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_WAKEUP
��    �� �� ���绽��
��    �� �� ��
�� �� ֵ �� ��
*************************************************/
void SFLASH_WAKEUP(void)
{
  W25Q16->SPI_CS = 0;                            //ʹ������
  SPI_WriteByte(SFLASH_RELEASE_POWER_DOWN);      //�����绽�ѡ�ָ��
  W25Q16->SPI_CS = 1;                            //ʧ������
}

/************************************************
�������� �� SFLASH_ReadID
��    �� �� ��ȡоƬID SFLASH��ID(W25X16: EF14)
��    �� �� ��
�� �� ֵ �� ID --- 16λID��
*************************************************/
uint16_t SFLASH_ReadID(void)
{
  uint16_t ID = 0;
  W25Q16->SPI_CS = 0;                            //ʹ������

  SPI_WriteByte(SFLASH_DEVICE_ID);               //���豸ID��ָ��
  SPI_WriteByte(0x00);
  SPI_WriteByte(0x00);
  SPI_WriteByte(0x00);

  ID |= SPI_ReadByte()<<8;                       //��ȡID
  ID |= SPI_ReadByte();
  W25Q16->SPI_CS = 1;                            //ʧ������
  return ID;
}

/************************************************
�������� �� SFLASH_ReadJEDEC_ID
��    �� �� ��ȡоƬJEDEC_ID
��    �� �� ��
�� �� ֵ �� ID --- 24λID��
*************************************************/
uint32_t SFLASH_ReadJEDEC_ID(void)
{
  uint32_t ID = 0;
  W25Q16->SPI_CS = 0;                            //ʹ������

  SPI_WriteByte(SFLASH_JEDEC_ID);                //��JEDEC_ID��ָ��

  ID |= SPI_ReadByte()<<16;                      //��ȡID
  ID |= SPI_ReadByte()<<8;
  ID |= SPI_ReadByte();
  W25Q16->SPI_CS = 1;                            //ʧ������
  return ID;
}

