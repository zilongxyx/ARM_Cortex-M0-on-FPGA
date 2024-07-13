/*      
        ALIENTEK 2.4' TFTLCD IN CORTEXM0_SOC
        MAIN SIGNALS: LCD_CS, LCD_RS, LCD_WR, LCD_RD, LCD_RST, LCD_BL_CTR, LCD_DATA[16]
        2019.12.18
*/

#include "code_def.h"
#include <stdint.h>
#include <stdio.h>
#include "lcdfont.h"

uint16_t POINT_COLOR = 0X0000;
uint16_t BACK_COLOR  = 0xFFFF;

_lcd_dev lcddev;

void MakeData( uint16_t data ) {
    LCD->LCD_DATA[0] = ( data >> 0 ) & 1 ;
    LCD->LCD_DATA[1] = ( data >> 1 ) & 1 ;
    LCD->LCD_DATA[2] = ( data >> 2 ) & 1 ;
    LCD->LCD_DATA[3] = ( data >> 3 ) & 1 ;
    LCD->LCD_DATA[4] = ( data >> 4 ) & 1 ;
    LCD->LCD_DATA[5] = ( data >> 5 ) & 1 ;
    LCD->LCD_DATA[6] = ( data >> 6 ) & 1 ;
    LCD->LCD_DATA[7] = ( data >> 7 ) & 1 ;
    LCD->LCD_DATA[8] = ( data >> 8 ) & 1 ;
    LCD->LCD_DATA[9] = ( data >> 9 ) & 1 ;
    LCD->LCD_DATA[10] = ( data >> 10 ) & 1;
    LCD->LCD_DATA[11] = ( data >> 11 ) & 1;
    LCD->LCD_DATA[12] = ( data >> 12 ) & 1;
    LCD->LCD_DATA[13] = ( data >> 13 ) & 1;
    LCD->LCD_DATA[14] = ( data >> 14 ) & 1;
    LCD->LCD_DATA[15] = ( data >> 15 ) & 1;
}

uint16_t ReadData() {
	uint16_t ans = 0;
	int i;
	for ( i = 0; i < 16; i++) {
		if (LCD->LCD_DATA[i] == 1)
			ans += (1 << i);
	}
	return ans;
}

//  WRITE REG FUNCTION
void LCD_WR_REG( uint16_t data ) {
  	LCD_RS_CLR; 
 	LCD_CS_CLR; 
	//Delay(50);
	MakeData( data );
	LCD_WR_CLR;
	//Delay(50);
	LCD_WR_SET; 
 	LCD_CS_SET;         
}

//  WRITE DATA
void LCD_WR_DATA( uint16_t data ) {
  	LCD_RS_SET;
	LCD_CS_CLR;
	//Delay(50);
	MakeData( data );
	LCD_WR_CLR;
	//Delay(50);
	LCD_WR_SET;
	LCD_CS_SET;
}

uint16_t LCD_RD_DATA(void)
{										   
	uint16_t t;

	LCD_RS_SET;
	LCD_CS_CLR;
	LCD_RD_CLR;			   
	t=ReadData();
	//Delay(50);  
	LCD_RD_SET;
	LCD_CS_SET; 

	return t;  
}

//  WRITE REG VALUE
//  LCD_Reg : NUMBER OF REG
//  LCD_RegValue : VALUE NEEDED TO BE WRITTEN
void LCD_WriteReg( uint16_t LCD_Reg, uint16_t LCD_RegValue ) {
	LCD_WR_REG( LCD_Reg );  
	LCD_WR_DATA( LCD_RegValue );	    		 
} 

uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);   
	//Delay(50);
	return LCD_RD_DATA(); 
}

//  BEGIN TO WRITE GRAM
void LCD_WriteRAM_Prepare( void ) {
    LCD_WR_REG( 0x2c );
}

//  LCD WRITE GRAM
void LCD_WriteRAM( uint16_t RGB_Code ) {
    LCD_WR_DATA( RGB_Code );
}

void opt_Delay( uint8_t t ) {
    while ( t-- );
}

//  LCD START DISPLAY
void LCD_DisplayOn( void ) {
    LCD_WR_REG( 0x29 );
}

//  LCD END DISPLAY
void LCD_DisplayOff( void ) {
    LCD_WR_REG( 0x28 );
}

//  LCD SET CURSOR POSTION
void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos ) {
    LCD_WR_REG( 0X2a ); 
	LCD_WR_DATA( Xpos >> 8 );
    LCD_WR_DATA( Xpos & 0XFF ); 			 
	LCD_WR_REG( 0x2b ); 
	LCD_WR_DATA( Ypos >> 8 );
    LCD_WR_DATA( Ypos & 0XFF ); 
}

//  SET LCD SCANNING DIRECTION 
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))
	{			   
		switch(dir)
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/6804/5310/5510/1963
	{
		switch(dir)
		{
			case L2R_U2D:
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U:
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D:
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U:
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R:
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://´ÓÉÏµ½ÏÂ,´ÓÓÒµ½×ó
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://´ÓÏÂµ½ÉÏ,´Ó×óµ½ÓÒ
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://´ÓÏÂµ½ÉÏ,´ÓÓÒµ½×ó
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963²»ÐèÒªBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804µÄBIT6ºÍ9341µÄ·´ÁË	   
		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963²»×ö×ø±ê´¦Àí
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//½»»»X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//½»»»X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D:
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U:
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D:
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U:
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R:
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L:
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R:
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L:
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 
		dirreg=0X03;
		regval|=1<<12; 
		LCD_WriteReg(dirreg,regval);
	}
}     

void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		
	LCD_WriteRAM_Prepare();	
	LCD_WR_DATA(BACK_COLOR); 
}	 

void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	   
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF);
	LCD_RS_CLR;
 	LCD_CS_CLR; 
	MakeData(lcddev.wramcmd); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET; 
	LCD_WR_DATA(color);		
}

void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			
	{
		lcddev.dir=0;	
		lcddev.width=240;
		lcddev.height=320;
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.width=320;
			lcddev.height=480;
		}
		
	}else 				
	{	  				
		lcddev.dir=1;	
		lcddev.width=320;
		lcddev.height=240;
		lcddev.wramcmd=0X2C;
	 	lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  
	} 
	// LCD_Scan_Dir(DFT_SCAN_DIR);	
}	 

void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{    
	uint8_t hsareg,heareg,vsareg,veareg;
	uint16_t hsaval,heaval,vsaval,veaval; 
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(sx>>8); 
	LCD_WR_DATA(sx&0XFF);	 
	LCD_WR_DATA(twidth>>8); 
	LCD_WR_DATA(twidth&0XFF);  
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(sy>>8); 
	LCD_WR_DATA(sy&0XFF); 
	LCD_WR_DATA(theight>>8); 
	LCD_WR_DATA(theight&0XFF); 
}

void LCD_Init(void)
{ 
    LCD -> LCD_MODE = 0;
    LCD -> LCD_INI_en = 1;
}  		  

void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			
	if((lcddev.id==0X6804)&&(lcddev.dir==1))
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	 
	LCD_WriteRAM_Prepare();     			  	  
	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
} 
void LCD_Clear_back(const unsigned char *pic)
{
	uint16_t *bitmap = (uint16_t *)pic;		
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			
	if((lcddev.id==0X6804)&&(lcddev.dir==1))
	{						    
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_SetCursor(0x00,0x0000);		  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else LCD_SetCursor(0x00,0x0000);	 
	LCD_WriteRAM_Prepare();     			  	  
	for(index=0;index<totalpoint;index++)LCD_WriteRAM(*bitmap++); 	
} 
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	uint16_t temp;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
	 	LCD_SetCursor(sx,i);      				
		LCD_WriteRAM_Prepare();     				  
		for(j=0;j<xlen;j++)LCD_WriteRAM(color);		    
	}
}  
void LCD_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t Xend,uint16_t Yend,  unsigned char * pic)
{
	
    static   uint16_t i=0,j=0;
	uint16_t *bitmap = (uint16_t *)pic;		

	for(i=StartY;i<Yend;i++)
	{
		LCD_SetCursor(StartX,i);
		LCD_WriteRAM_Prepare();		
		for(j=StartX;j<Xend;j++)
		{
			LCD_WriteRAM(*bitmap++); 
		
		}        
	}
// LCD_Fill(0,0,100,100,BLUE);
	

}

void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			
	height=ey-sy+1;			
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	 
		LCD_WriteRAM_Prepare();     
		for(j=0;j<width;j++)LCD_WR_DATA(color[i*width+j]);
	}	  
} 

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0;
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		LCD_DrawPoint(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
								  

uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 

void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
}   
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);			
 	num=num-' ';
	for(t=0;t<csize;t++)
	{   
		//if(size==12)temp=asc2_1206[num][t]; 	 	
		if(size==16)temp=asc2_1608[num][t];	
		//else if(size==24)temp=asc2_2412[num][t];	
		else return;								
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,0XF800);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	
				break;
			}
		}  	 
	}  	    	   	 	  
}   
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{         
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))
    {       
        if(x>=width)
		{
			x=x0;
			y+=size;
		}
        if(y>=height)
			break;
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}
