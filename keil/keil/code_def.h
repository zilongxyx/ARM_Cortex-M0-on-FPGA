#ifndef __code_def_H__
#define __code_def_H__

#include <stdint.h>
#include "sflash.h"

//INTERRUPT DEF
#define NVIC_CTRL_ADDR (*(volatile unsigned *)0xe000e100)

//SysTick DEF
typedef struct{
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VALUE;
    volatile uint32_t CALIB;
}SysTickType;

#define SysTick_BASE 0xe000e010
#define SysTick ((SysTickType *)SysTick_BASE)

 
//LED DEF
typedef struct{
    volatile uint32_t LED_MODE;
}LEDType;

#define LED_BASE 0x40000000
#define LED ((LEDType *)LED_BASE)

//IO DEF
typedef struct{  
	volatile uint8_t IO_MODE[8]; 	
}IOType;

#define IO_BASE 0x40090000
#define IO ((IOType *)IO_BASE)

//IO_READ DEF
typedef struct{
    volatile uint32_t IO_MODE_READ;  	
}IO_READType;

#define IO_READ_BASE 0x400A0000
#define IO_READ ((IO_READType *)IO_READ_BASE)
//UART DEF
typedef struct{
    volatile uint32_t UARTRX_DATA;
    volatile uint32_t UARTTX_STATE;
    volatile uint32_t UARTTX_DATA;
}UARTType;

#define UART_BASE 0x40010000
#define UART1 ((UARTType *)UART_BASE)


//Matrix_Key DEF
typedef struct{
	volatile uint32_t ROW;  //0x40020000
	volatile uint32_t COL;  //0x40020004
}MKEYType;

#define MKEY_BASE 0x40020000
#define MKEY ((MKEYType *)MKEY_BASE)


//SEG DEF
typedef struct{
    volatile uint32_t DATA;
}SEGType;

#define SEG_BASE 0x40030000
#define SEG ((SEGType *)SEG_BASE)


//TIME DEF
typedef struct{
    volatile uint32_t LOAD;
    volatile uint32_t ENABLE;
    volatile uint32_t VALUE;
}TIMERType;

#define TIMER_BASE 0x40040000
#define TIMER ((TIMERType *)TIMER_BASE)


//MSi001 DEF
typedef struct{
    volatile uint32_t MSi_SPI_CS;
    volatile uint32_t MSi_SPI_CLK;
    volatile uint32_t MSi_SPI_DATA;
}MSi001Type;

#define MSi001_BASE 0x40050000
#define MSi001 ((MSi001Type *)MSi001_BASE)


//QN8027 DEF
typedef struct{
    volatile uint32_t QN_IIC_SCL;   //40060000 0000
    volatile uint32_t QN_IIC_SDA; //40060004   0100
    volatile uint32_t ACK; //40060008   1000
}QN8027Type;

#define QN8027_BASE 0x40060000
#define QN8027 ((QN8027Type *)QN8027_BASE)

#define MPU_ADDR		0X2C

typedef struct{
    volatile uint32_t PAJ_IIC_SCL; //40050000
    volatile uint32_t PAJ_IIC_SDA;//40050004
    volatile uint32_t PAJ_ACK;//40050008
}PAJType;

#define PAJ_BASE 0x40050000
#define PAJ ((PAJType *)PAJ_BASE)

//LCD

typedef struct {
    volatile uint32_t LCD_CS; // 0x40002000   
    volatile uint32_t LCD_RS; // 0x40002004
    volatile uint32_t LCD_WR; // 0x40002008
    volatile uint32_t LCD_RD; // 0x4000200C
    volatile uint32_t LCD_RST;// 0x40002010
    volatile uint32_t LCD_BL_CTR;// 0x40002014
    volatile uint32_t LCD_DATA[16];// 0x40002018-0x40002054
    volatile uint32_t LCD_MODE;// 0x40002058
    volatile uint32_t LCD_INI_en;// 0x40002s05C
}LCDType;

#define LCD_BASE 0x40080000
#define LCD ((LCDType *)LCD_BASE)

typedef struct  
{										    
	uint16_t width;			
	uint16_t height;			
	uint16_t id;				
	uint8_t  dir;			
	uint16_t wramcmd;		
	uint16_t setxcmd;		
	uint16_t setycmd;		
}_lcd_dev; 

extern _lcd_dev lcddev;

extern uint16_t  POINT_COLOR;  
extern uint16_t  BACK_COLOR; 

//      BASIC SIGNAL SET AND CLEAR
#define LCD_CS_SET         (LCD->LCD_CS        = 1) 	 
#define LCD_RS_SET         (LCD->LCD_RS        = 1) 
#define LCD_WR_SET         (LCD->LCD_WR        = 1) 
#define LCD_RD_SET         (LCD->LCD_RD        = 1) 
#define LCD_RST_SET        (LCD->LCD_RST       = 1)
#define LCD_BL_CTR_SET     (LCD->LCD_BL_CTR    = 1)
     
#define LCD_CS_CLR         (LCD->LCD_CS        = 0) 	 
#define LCD_RS_CLR         (LCD->LCD_RS        = 0) 
#define LCD_WR_CLR         (LCD->LCD_WR        = 0) 
#define LCD_RD_CLR         (LCD->LCD_RD        = 0) 
#define LCD_RST_CLR        (LCD->LCD_RST       = 0)
#define LCD_BL_CTR_CLR     (LCD->LCD_BL_CTR    = 0)
     
//      DATA     
#define LCD_DATA0_SET( x )   (LCD->LCD_DATA[0]   = (x))
#define LCD_DATA1_SET( x )   (LCD->LCD_DATA[1]   = (x)) 	  	 
#define LCD_DATA2_SET( x )   (LCD->LCD_DATA[2]   = (x)) 	 
#define LCD_DATA3_SET( x )   (LCD->LCD_DATA[3]   = (x)) 	 
#define LCD_DATA4_SET( x )   (LCD->LCD_DATA[4]   = (x)) 	 
#define LCD_DATA5_SET( x )   (LCD->LCD_DATA[5]   = (x)) 	 
#define LCD_DATA6_SET( x )   (LCD->LCD_DATA[6]   = (x)) 	 
#define LCD_DATA7_SET( x )   (LCD->LCD_DATA[7]   = (x)) 	 
#define LCD_DATA8_SET( x )   (LCD->LCD_DATA[8]   = (x)) 	 
#define LCD_DATA9_SET( x )   (LCD->LCD_DATA[9]   = (x)) 	 
#define LCD_DATA10_SET( x )  (LCD->LCD_DATA[10]  = (x)) 	 
#define LCD_DATA11_SET( x )  (LCD->LCD_DATA[11]  = (x)) 	 
#define LCD_DATA12_SET( x )  (LCD->LCD_DATA[12]  = (x)) 	 
#define LCD_DATA13_SET( x )  (LCD->LCD_DATA[13]  = (x)) 	 
#define LCD_DATA14_SET( x )  (LCD->LCD_DATA[14]  = (x)) 	 
#define LCD_DATA15_SET( x )  (LCD->LCD_DATA[15]  = (x)) 

//      SCANNING DIRECTION
#define L2R_U2D  0 // LEFT TO RIGHT, UP TO DOWN
#define L2R_D2U  1 // LEFT TO RIGHT, DOWN TO UP
#define R2L_U2D  2 // RIGHT TO LEFT, UP TO DOWN
#define R2L_D2U  3 // RIGHT TO LEFT, DOWN TO UP

#define U2D_L2R  4 // UP TO DOWN, LEFT TO RIGHT
#define U2D_R2L  5 // UP TO DOWN, RIGHT TO LEFT
#define D2U_L2R  6 // DOWN TO UP, LEFT TO RIGHT
#define D2U_R2L  7 // DOWN TO UP, RIGHT TO LEFT

#define DFT_SCAN_DIR    L2R_U2D // DEFAULT

//  PEN COLOR
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 

//  GUI COLOR ( COLOR OF PANEL )
#define DARKBLUE      	 0X01CF	
#define LIGHTBLUE      	 0X7D7C	 
#define GRAYBLUE       	 0X5458 


#define LIGHTGREEN     	 0X841F 
#define LGRAY 			 0XC618 // BACKGROUND COLOR OF WINDOW

#define LGRAYBLUE        0XA651 // MIDDLE LAYER COLOR
#define LBBLUE           0X2B12 // COLOR OF SWITCHED

void      LCD_Init(void);									
void      LCD_DisplayOn(void);													
void      LCD_DisplayOff(void);													
void      LCD_Clear(uint16_t Color);	 											
void      LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);							
void      LCD_DrawPoint(uint16_t x,uint16_t y);									
void      LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);							
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); 										
void      LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);						 		
void      LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);					
void      LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   		
void      LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);		   	
void      LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color);	
void      LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);			
void      LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  					
void      LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				
void      LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		
void LCD_Clear_back(const unsigned char *pic);
void      LCD_WriteReg(uint16_t LCD_Reg, uint16_t LCD_RegValue);
uint16_t  LCD_ReadReg(uint16_t LCD_Reg);
void      LCD_WriteRAM_Prepare(void);
void      LCD_WriteRAM(uint16_t RGB_Code);
void      LCD_SSD_BackLightSet(uint8_t pwm);							
void      LCD_Scan_Dir(uint8_t dir);									
void      LCD_Display_Dir(uint8_t dir);
void      LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height);
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t Xend,uint16_t Yend, unsigned char * pic);
void LCD_Init(void);
void Delay(uint32_t time);
//AUTO_Research DEF
typedef struct{
    volatile uint32_t enable;
    volatile uint32_t result;
}AUTOType;

#define AUTO_BASE 0x40070000
#define AUTO ((AUTOType *)AUTO_BASE)


//W25Q16 DEF
typedef struct{
    volatile uint32_t SPI_CS;
    volatile uint32_t SPI_CLK;
    volatile uint32_t SPI_MOSI;
    volatile uint32_t SPI_MISO;	
}SPIType;

#define SPI_BASE 0x40040000
#define SPI ((SPIType *)SPI_BASE)

//W25Q16 DEF
typedef struct{
    volatile uint32_t SPI_CS;
    volatile uint32_t SPI_CLK;
    volatile uint32_t SPI_MOSI;
    volatile uint32_t SPI_MISO;	
}W25Q16Type;
#define W25Q16_BASE 0x40080000
#define W25Q16 ((W25Q16Type *)W25Q16_BASE)
//SPEAK DEF
typedef struct{
    volatile uint32_t Mode;	
	volatile uint32_t Recording_mode;
	volatile uint32_t Recording_play_mode;
	volatile uint32_t Recording_type;
}SPEAKType;

#define SPEAK_BASE 0x40090000
#define SPEAK ((SPEAKType *)SPEAK_BASE)


//Buzzer DEF
typedef struct{
    volatile uint32_t EN;
}BuzzerType;

#define Buzzer_BASE 0x400A0000
#define Buzzer ((BuzzerType *)Buzzer_BASE)



//SD DEF
typedef struct{
    volatile uint32_t play;
    volatile uint32_t next;
    volatile uint32_t last;
    volatile uint32_t stop;
}SDType;

#define SD_BASE 0x400B0000
#define SD ((SDType *)SD_BASE)


//GAME1 DEF
typedef struct{
    volatile uint32_t Game;	
    volatile uint32_t GAME_type;
    volatile uint32_t Finger_emit;
	  volatile uint32_t Finger_recv;
    volatile uint32_t Number_emit;
    volatile uint32_t Number_recv;
}GAMEType;

#define GAME_BASE 0x400C0000
#define GAME ((GAMEType *)GAME_BASE)

//Score DEF
typedef struct{
    volatile uint32_t rhythm_mark;	
    volatile uint32_t frequency_mark;
}ScoreType;

#define Score_BASE 0x400D0000
#define Score ((ScoreType *)Score_BASE)


//Stereo DEF
typedef struct{
    volatile uint32_t turn_on;	
}StereoType;

#define Stereo_BASE 0x400E0000
#define Stereo ((StereoType *)Stereo_BASE)



//UART2 DEF
typedef struct{
    volatile uint32_t UARTRX_DATA;
    volatile uint32_t UARTTX_STATE;
    volatile uint32_t UARTTX_DATA;
}UART2Type;

#define UART2_BASE 0x400F0000
#define UART2 ((UART2Type *)UART2_BASE)


void delay_us(int us);
void delay_ms(int ms);

void KEY0Handle(void);
void KEY1Handle(void);
void KEY2Handle(void);
void KEY3Handle(void);
void KEY4Handle(void);
void KEY5Handle(void);
void KEY6Handle(void);
void KEY7Handle(void);

char ReadUART1State(void);
char ReadUART1(void);
void WriteUART1(char data);
void Uart_SendCmd(unsigned char CMD,unsigned char feedback,unsigned char dat);
void DoSum(unsigned char *Str,unsigned char len);
void SendCmd(unsigned char len);
void UART_DATA(unsigned char data);
void UART1String(char *stri);
void UART1Handle(void);
void Serial1_Receive_Data(char data);

char ReadUART2State(void);
char ReadUART2(void);
void WriteUART2(char data);
void UART2String(char *stri);
void UART2Handle(void);
void Serial2_Receive_Data(uint8_t data);

uint8_t Key_Scan();

void fre_select(void);
void gain_select(void);

void TIME_Init(void);
void TIMEHandle(void);

void MSi001_Init(void);
void MSi_SPI_Write(uint32_t Data);

void QN8027_Init(void);
//uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);//IIC连续写
//uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf); //IIC连续读 
uint8_t MPU_Write_Byte(uint8_t reg,uint8_t data);				//IIC写一个字节
//uint8_t MPU_Read_Byte(uint8_t reg);						//IIC读一个字节

void ResearchHandle(void);
void SPIHandle(void);
void SPI_Initializes(void);
void SPI_WriteByte(char TxData);
char SPI_ReadByte(void);

void tune_left(void);
void tune_right(void);

void SD_next(void);
void SD_last(void);
void SD_stop(void);

void Serial_Send_Data1(int num, int fre);
void Serial_Send_Data2(char data);
void Serial_Send_Data3(char data);
void Serial_Send_Data4(void);
void mem_tunnel(void);

void GAME1Handle(void);

void Auto_Search(void);

#endif