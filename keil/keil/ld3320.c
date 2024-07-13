#include "ld3320.h"
/************************************************************************************
//	nAsrStatus 用来在main主程序中表示程序运行的状态，不是LD3320芯片内部的状态寄存器
//	LD_ASR_NONE:			表示没有在作ASR识别
//	LD_ASR_RUNING：		表示LD3320正在作ASR识别中
//	LD_ASR_FOUNDOK:		表示一次识别流程结束后，有一个识别结果
//	LD_ASR_FOUNDZERO:	表示一次识别流程结束后，没有识别结果
//	LD_ASR_ERROR:			表示一次识别流程中LD3320芯片内部出现不正确的状态
*********************************************************************************/
static uint8 nAsrStatus = LD_ASR_NONE;	
uint8 nLD_Mode = LD_MODE_IDLE;//用来记录当前是在进行ASR识别还是在播放MP3
uint8 ucRegVal;
u8 LD3320_Mode=LD3320_MODE_KL;
u8 LD3320_KL_Flag=0;
u8 temp[10]="0";
char test_string[10];
char bf_string[10];
static void LD3320_delay(unsigned long uldata)
{
	unsigned int i  =  0;
	unsigned int j  =  0;
	unsigned int k  =  0;
	for (i=0;i<5;i++)
	{
		for (j=0;j<uldata;j++)
		{
			k = 200;
			while(k--);
		}
	}
}
void LD_reset(void) 
{
	
	
	LD_RST_H();
	//delay_ms(10);
	LD_RST_L();
	//delay_ms(10);
	LD_RST_H();
	//delay_ms(10);
	LD_CS_L();
	//delay_ms(10);
	LD_CS_H();
	//delay_ms(10);
	SPI->SPI_CS 	 = 0;
	SPI->SPI_CLK  = 1;
	LD_SPIS_L();
	//SPI->SPI_MOSI  = 1;
}
///寄存器操作 end
uint8_t Ld3320_Fun(void)
{
	uint8 nAsrRes=0;
	uint8 nAsrResCount=0;
	//nAsrStatus = LD_ASR_NONE;//初始状态：没有在作ASR     nAsrStatus   上面有定义         #define LD_ASR_NONE					0x00	//表示没有在作ASR识别
	switch(nAsrStatus)
	{
		case LD_ASR_RUNING:
			LD_WriteReg(0x29,0) ;
			LD_WriteReg(0x02,0) ;

			if((LD_ReadReg(0xb2)==0x21)&&(LD_ReadReg(0xbf)==0x35))		
			{	 
				nAsrResCount = LD_ReadReg(0xc5); // 
				if(nAsrResCount>0 && nAsrResCount<=4) 
				{
					nAsrStatus=LD_ASR_FOUNDOK; 				
				}
				else
				{
					nAsrStatus=LD_ASR_FOUNDZERO;
				}				
			}
			else
			{
				nAsrStatus=LD_ASR_RUNING;//执行没有识别
			}
//			UART1String("LD_ASR_RUNING\r\n");
			break;
			//#define LD_ASR_RUNING				0x01	//表示LD3320正在作ASR识别中										
		case LD_ASR_ERROR:                 //#define LD_ASR_ERROR	 			0x31	//	表示一次识别流程中LD3320芯片内部出现不正确的状态
				UART1String("ERROR\r\n");
				break;	
		case LD_ASR_NONE:                 //#define LD_ASR_NONE					0x00	//表示没有在作ASR识别
				nAsrStatus=LD_ASR_RUNING;     //初始状态：没有在作ASR     nAsrStatus   上面有定义         #define LD_ASR_NONE					0x00	//表示没有在作ASR识别			
				if (RunASR()==0)//启动一次ASR识别流程：ASR初始化，ASR添加关键词语，启动ASR运算
				{		
					nAsrStatus = LD_ASR_ERROR;
					UART1String("RunASR ERROR\r\n");
				}
				//UART1String("LD_ASR_NONE \r\n");
				break;													
		case LD_ASR_FOUNDOK: 
				 nAsrStatus = LD_ASR_NONE;
				 nAsrRes = LD_GetResult();//一次ASR识别流程结束，去取ASR识别结果
				 sprintf(test_string,"\r\n识别码:%d",nAsrRes);
				 UART1String(test_string);
				break;
		case LD_ASR_FOUNDZERO: nAsrStatus = LD_ASR_NONE; break;
//		default:
//				nAsrStatus = LD_ASR_NONE;
				//break;
		}//switch
	return nAsrRes;
}
static uint8 RunASR(void)
{
	uint8 i=0;
	uint8 asrflag=0;
	uint8 nAsrResCount=0;
	for (i=0; i<2; i++)		//防止由于硬件原因导致LD3320芯片工作不正常，所以一共尝试5次启动ASR识别流程
	{
		LD_Init_ASR();			//初始化ASR
		//delay_ms(100);
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
		{
			LD_reset();				//LD3320芯片内部出现不正常，立即重启LD3320芯片
			//delay_ms(50);	//并从初始化开始重新ASR识别流程
			continue;
		}
		//delay_ms(10);
		if (LD_AsrRun() == 0)
		{
			LD_reset();			 //LD3320芯片内部出现不正常，立即重启LD3320芯片
			//delay_ms(50);//并从初始化开始重新ASR识别流程
			continue;
		}
		asrflag=1;
		break;						//ASR流程启动成功，退出当前for循环。开始等待LD3320送出的中断信号
	}	

//	while(LD_ReadReg(0xb2)!=0x21)
//	{
//		UART1String("whilexyx\r\n");
//	}
//	LD_WriteReg(0x29,0) ;
//	LD_WriteReg(0x02,0) ;

	return asrflag;
}
static void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	//delay_ms(5);
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	//delay_ms( 1 );
}
static void LD_Init_Common(void)
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	delay_ms(5);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	delay_ms(5);
	LD_WriteReg(0xCF, 0x43);   
	delay_ms(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	delay_ms(5);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	delay_ms(1);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}
void LD_WriteReg(uint8 data1,uint8 data2)
{
	LD_CS_L();
	LD_SPIS_L();
	SPI_WriteByte(0x04);
	SPI_WriteByte(data1);
	SPI_WriteByte(data2);
	LD_CS_H();
}
static uint8 LD_AsrAddFixed(void)
{
	uint8 k, flag;
	uint8 nAsrAddLength;
	#define DATE_A 7    //数组二维数值
	#define DATE_B 20		//数组一维数值
	//添加关键词，用户修改
	uint8  sRecog[DATE_A][DATE_B] = {
	 			"xiao ying",\
				"jian pan",\
				"zhi neng",\
				"shou bing",\
				"ti gan",\
				"zhuang tai",\
		    "ce shi"
		
																	};	
	uint8  pCode[DATE_A] = {
																CODE_KL,	\
	 															CODE_LSD,	\
																CODE_SS,	\
																CODE_AJCF,\
																CODE_QM,	\
																CODE_JT,		\
		                            CODE_START_SHOW\
															};	//添加识别码，用户修改
	flag = 1;
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}

		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0);
		LD_WriteReg(0x08, 0x04);
		//delay_ms(1);
		LD_WriteReg(0x08, 0x00); 
		//delay_ms(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
		}
		LD_WriteReg(0xb9, nAsrAddLength);   //添加识别语
		LD_WriteReg(0xb2, 0xff);  //设置ASR为忙状态
		LD_WriteReg(0x37, 0x04);  //通知DSP要添加一项识别语
	}

//	sprintf(bf_string,"0xbf:%d\r\n",LD_ReadReg(0xbf));
//	UART1String(bf_string);	
	return flag;
}
static uint8 LD_GetResult(void)
{
	return LD_ReadReg(0xc5); //读取识别结果
}

static uint8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);  //设置麦克风音量
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);       //清除 FIFO_DATA
	//delay_ms( 5 );
	LD_WriteReg(0x08, 0x00);
	//delay_ms( 5);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff); //设置ASR忙	
	LD_WriteReg(0x37, 0x06);  //开始识别语音
	LD_WriteReg(0x37, 0x06);
//	sprintf(bf_string,"0xbf2:%d\r\n",LD_ReadReg(0xbf));
//	UART1String(bf_string);
	//delay_ms(5);
	LD_WriteReg(0x1C, 0x0b); //麦克风输入 ADC 通道可用
	LD_WriteReg(0x29, 0x00); //FIFO中断允许
	LD_WriteReg(0xBD, 0x00);   //启动ASR
	return 1;
}
uint8 LD_Check_ASRBusyFlag_b2(void)
{
	uint8 j;
	uint8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		delay_ms(10);		
	}
	return flag;
}
uint8 LD_ReadReg(uint8 reg_add)
{
	uint8 i;
	LD_CS_L();
	LD_SPIS_L();
	SPI_WriteByte(0x05);
	SPI_WriteByte(reg_add);
	i=SPI_ReadByte();
	//i=spi_send_byte(0xff);
//	printf("%d test\r\n",i);
	LD_CS_H();
	return(i);
}
///寄存器操作
uint8 spi_send_byte(uint8 byte)
{
	uint8_t i;
	
	MSi001->MSi_SPI_CS  = 0;
	MSi001->MSi_SPI_CLK = 0;
	for(i = 0; i < 8; i++)
	{
		if(byte & 0x80)
			MSi001->MSi_SPI_DATA = 1;
		else
			MSi001->MSi_SPI_DATA = 0;
		
		delay_us(1);
		MSi001->MSi_SPI_CLK = 0;
		
		delay_us(1);
		MSi001->MSi_SPI_CLK = 1;
		
		delay_us(1);
		byte <<= 1;
	}
	
	MSi001->MSi_SPI_CS  = 1;
	MSi001->MSi_SPI_CLK = 0;
	delay_us(1);
}
void ProcessInt(void)
{
	uint8 nAsrResCount=0;

	ucRegVal = LD_ReadReg(0x2B);

// 语音识别产生的中断
//（有声音输入，不论识别成功或失败都有中断）
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;

	if((ucRegVal & 0x10) && LD_ReadReg(0xb2)==0x21 && LD_ReadReg(0xbf)==0x35)		
	{	 
			nAsrResCount = LD_ReadReg(0xba);

			if(nAsrResCount>0 && nAsrResCount<=4) 
			{
				nAsrStatus=LD_ASR_FOUNDOK; 				
			}
			else
			{
				nAsrStatus=LD_ASR_FOUNDZERO;
			}	
	}
	else
	{
		nAsrStatus=LD_ASR_FOUNDZERO;//执行没有识别
	}

	LD_WriteReg(0x2b,0);
	LD_WriteReg(0x1C,0);//写0:ADC不可用
	LD_WriteReg(0x29,0);
	LD_WriteReg(0x02,0);
	LD_WriteReg(0x2B,0);
	LD_WriteReg(0xBA,0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);//清除FIFO_DATA
	LD_WriteReg(0x08,0);//清除FIFO_DATA后 再次写0
}


