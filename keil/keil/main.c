#include "code_def.h"
#include <stdint.h>
#include "stdlib.h"
#include <stdio.h>
#include "string.h"
#include "mpuiic.h"
#include "ld3320.h"
#include "pajiic.h"
#include "game.h"
#define LCD_WIDTH 240
#define LCD_HEIGHT 320

float Fre = 100.0;        //选台频率   不用管
int	    Gain_Reduction = 10;//增益衰减  不用管

char adc_string[10];
char spi_string[10];
extern char posx_string[10];
extern char posy_string[10];
extern char test2_string[10];
extern char test3_string[10];
extern char test4_string[10];
extern char test5_string[10];
extern char test6_string[10];
extern char test7_string[10];

unsigned char bullet_state[3] = {0};
uint32_t adc_value[4] = {0};
uint8_t channel = 0;
uint8_t voice_state=1;
uint8_t voice_state_old=1;
uint8_t voice_state_reg=1;
uint8_t game_mode  = 0;
char show_char[9]="Star Trek";
char over_char[9]="Game Over";
char submit_char[50];
char score_char[10]="Score:";
char score_over_char[15]="Your Score:";
char life_char[8];
char start_char[18]="Press k3 to start!";
char bullet_char[8];
int show_blink = 0;
uint8_t hit = 0;
uint8_t is_plane_hit = 0;
int i, j,point_x, point_y;
uint8_t key_value = 0;
char volume = 15;
extern int pos_x,pos_y;  //320 -36
extern int bullet_x[3],bullet_y[3];
extern int pos_x_old,pos_y_old;
extern unsigned char bullet_num; // 打出的子弹
extern unsigned char bullet_num_show; //显示剩余的子弹
extern unsigned char bullet_en[3];
extern uint8_t score;
extern uint16_t hit_x,hit_y;
extern uint8_t is_hit;
char show_num = 0;
extern unsigned int image_width;
extern unsigned int image_height;
extern struct position_t positions[10];
extern struct bullet_t bullets[3];
extern struct enemy_t enemies[4];
extern uint32_t colorTab[16];
extern uint32_t LCD_ini_finish;

extern unsigned char gImage_enemy_small1[1400];
extern unsigned char gImage_enemy_small2[1400];
extern unsigned char gImage_enemy_small3[1400];
extern unsigned char gImage_enemy_small4[1400];
//extern unsigned char gImage_enemy_small5[1400];
//unsigned char gImage_enemy_small10[1400];
extern unsigned char gImage_hero_small[1400];
//unsigned char gImage_enemy4[1];
extern   unsigned char gImage_bullet1[420];
//extern   unsigned char gImage_hero1[2880];
extern   unsigned char gImage_bomb[720];
extern int num_enemies;

void begin_show();
void over_show();
void gaming_show();
void gamepad_mode();
void motion_mode();
void key_mode();
void intelligence_mode();
void collision_check();
void level_improve();
void show_init();
void esp8266_init();
void esp8266_delay();
void volumn_change();
int main()
{ 
	uint16_t x, y;
	unsigned char k = 0;
	uint8_t life = 3;
	uint16_t time=0;
	uint8_t dx, dy;
	NVIC_CTRL_ADDR	=	0x7;
	LCD->LCD_MODE = 1;
	LCD_RST_CLR;
	LCD_RST_SET;
	LCD_BL_CTR_SET;
	x  = y  = 110;
	dx = dy = 20;
	PCF8591_Init();  //PCF8591 init
	LCD_Init();
	while(!LCD_ini_finish); 
	while(paj_init()==0);
	LCD_ini_finish = 0;
	LCD -> LCD_MODE = 1;
	LCD_CS_SET;
	LCD_RS_SET;
	LCD_WR_SET;
	LCD_RD_SET;
	LCD_Display_Dir(1);
	LCD_Clear(BLACK);
  
	show_init();
	init_enemies();
	init_bullets();
	esp8266_init();
	Uart_SendCmd(0X06,0,volume);  //设置音量
	Uart_SendCmd(0X03,0,0X06);  //播放bgm
	//delay_ms(6000);
	while(1)
	{
        
        //调试
//  	game_mode=1; IO->IO_MODE[7]=1;IO->IO_MODE[6]=1; IO->IO_MODE[5]=1;
//      begin_show();
        
        // 键盘 智能 手柄  体感
		// 按下最右边手柄开始游戏

		key_value = Key_Scan();
		volumn_change();  //PCB上增加AI32C作为触摸按键来控制音量的+-
		if(key_value==4)
		{
				LCD_Fill(60,120, 204,210, BLACK); //隐藏开始界面
				if(show_num==2)
				{
					game_init();
				}
				show_num = 1;			
		}



		if(show_num==0) //初始化显示界面
		{
			begin_show();
		}
		else if(show_num==1) //游戏进行画面
		{
			switch(game_mode)
			{
				case 0: key_mode(); break;
				case 1: intelligence_mode(); break;
				case 2: gamepad_mode(); break;
				case 3: motion_mode(); break;
			}
			gaming_show();
			collision_check();
			level_improve();		
		}		 
		else if(show_num==2) //游戏结束显示画面
		{
			over_show();
		}
 
	}
}
void esp8266_delay()		
{
	unsigned char i, j, k,x;

	i = 109;
	j = 200;
	k = 199;
	for(x=0;x<2;x++)
	{
		do
		{
			do
			{
				while (--k);
			} while (--j);
		} while (--i);		
	}

}

void esp8266_init()
{
	UART2String("AT+RST\r\n");
	esp8266_delay();
	UART2String("AT+CWMODE=1\r\n");
	esp8266_delay();
	UART2String("AT+CWJAP=\"test\",\"88888888\"\r\n");
	esp8266_delay();
	UART2String("AT+MQTTUSERCFG=0,1,\"esp8266_test\",\"\",\"\",0,0,\"\"\r\n");
	esp8266_delay();
	UART2String("AT+MQTTCONN=0,\"mqtt.eclipseprojects.io\",1883,0\r\n");
}
//游戏开始界面显示
void begin_show()
{
	show_blink++;
	LCD_ShowString(90,120,72,16,16,(uint8_t *)show_char); 
	LCD_DrawPicture(110, 150, 110+image_width, 150 + image_height, gImage_hero_small);
	if((show_blink%10)<5) //控制字符串闪烁
		LCD_ShowString(60,190,144,16,16,(uint8_t *)start_char); //显示游戏字符	//18*8
	else 
		LCD_Fill(60,190, 204,206, BLACK);
	voice_state_reg = Ld3320_Fun(); //获取语音识别结果
	if(voice_state_reg!=0) //滤除掉0，获得真实的识别结果
	{
		voice_state_old = voice_state;
		voice_state = voice_state_reg;
	}
	//不同游戏模式对应不同的led
	switch(voice_state)
	{
		
		case 1: game_mode=0; IO->IO_MODE[7]=1;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;IO->IO_MODE[4]=0;break;
		case 2: game_mode=1; IO->IO_MODE[7]=0;IO->IO_MODE[6]=1; IO->IO_MODE[5]=0;IO->IO_MODE[4]=0;break;
		case 3: game_mode=2; IO->IO_MODE[7]=0;IO->IO_MODE[6]=0; IO->IO_MODE[5]=1;IO->IO_MODE[4]=0;break;
		case 4: game_mode=3; IO->IO_MODE[7]=0;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;IO->IO_MODE[4]=1;break;
		//default: game_mode=0; IO->IO_MODE[7]=1;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;
	}
	//切换模式语音提示，保证同一模式下MP3提示语音只响一次
	if(voice_state!=voice_state_old) 
	{
			voice_state_old = voice_state;
			switch(voice_state)
			{
				
				case 1: Uart_SendCmd(0X03,0,0X02);break;
				case 2: Uart_SendCmd(0X03,0,0X01);break;
				case 3: Uart_SendCmd(0X03,0,0X03);break;
				case 4: Uart_SendCmd(0X03,0,0X04);break;
				//default: game_mode=0; IO->IO_MODE[7]=1;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;
			}
			esp8266_delay();
			Uart_SendCmd(0X03,0,0X05);
		
	}   
}
//游戏结束界面显示
void over_show()
{	
	channel_select();
	
	voice_state_reg = Ld3320_Fun();
	if(voice_state_reg!=0)
		voice_state = voice_state_reg;
	switch(voice_state)
	{
		case 1: game_mode=0; IO->IO_MODE[7]=1;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;IO->IO_MODE[4]=0;break;
		case 2: game_mode=1; IO->IO_MODE[7]=0;IO->IO_MODE[6]=1; IO->IO_MODE[5]=0;IO->IO_MODE[4]=0;break;
		case 3: game_mode=2; IO->IO_MODE[7]=0;IO->IO_MODE[6]=0; IO->IO_MODE[5]=1;IO->IO_MODE[4]=0;break;
		case 4: game_mode=3; IO->IO_MODE[7]=0;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;IO->IO_MODE[4]=1;break;
//				default: game_mode=0; IO->IO_MODE[7]=1;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;
	}
	if(voice_state!=voice_state_old) //保证同一模式下MP3提示语音只响一次
	{
			voice_state_old = voice_state;
			switch(voice_state)
			{
				
				case 1: Uart_SendCmd(0X03,0,0X02);break;
				case 2: Uart_SendCmd(0X03,0,0X01);break;
				case 3: Uart_SendCmd(0X03,0,0X03);break;
				case 4: Uart_SendCmd(0X03,0,0X04);break;
				//default: game_mode=0; IO->IO_MODE[7]=1;IO->IO_MODE[6]=0; IO->IO_MODE[5]=0;
			}
			esp8266_delay();
			Uart_SendCmd(0X03,0,0X05);
		
	}  
			
	show_blink++;
	start_char[0]='P';
	over_char[0]='G';
	score_over_char[11]=score/100+'0';
	score_over_char[12]=score%100/10+'0';
	score_over_char[13]=score%10+'0';
	//sprintf(over_char,"Game Over");
	LCD_ShowString(90,140,72,16,16,(uint8_t *)over_char); //显示游戏字符
	//sprintf(score_over_char,"Your Score:%d",score);
	LCD_ShowString(78,160,120,16,16,(uint8_t *)score_over_char); //显示游戏字符
	//sprintf(start_char,"Press k3 to start!");
	if((show_blink%10)<5)
		LCD_ShowString(60,190,144,16,16,(uint8_t *)start_char); //显示游戏字符	//18*8
	else 
		LCD_Fill(60,190, 204,206, BLACK);	
}
void gaming_show()
{
	sprintf(bullet_char,"Bullet:%d",bullet_num_show);
	sprintf(score_char,"Score:%d",score);
	LCD_ShowString(0,0,64,16,16,(uint8_t *)score_char); //显示游戏字符
	LCD_ShowString(0,16,64,16,16,(uint8_t *)bullet_char); //显示游戏字符
	update_bullet_position();
	LCD_DrawPicture(pos_x, pos_y, pos_x+image_width, pos_y + image_height, gImage_hero_small);
	move_enemies();
	//清除敌机拖影
	if((enemies[0].y>=enemies[0].speed)&&(!((enemies[0].x<64)&&(enemies[0].y<32))))
		LCD_Fill(enemies[0].x,enemies[0].y-enemies[0].speed, enemies[0].x + image_width, enemies[0].y, BLACK);
	if((enemies[1].y>=enemies[1].speed)&&(!((enemies[1].x<64)&&(enemies[1].y<32))))
		LCD_Fill(enemies[1].x,enemies[1].y-enemies[1].speed, enemies[1].x + image_width, enemies[1].y, BLACK);
	if((enemies[2].y>=enemies[2].speed)&&(!((enemies[2].x<64)&&(enemies[2].y<32))))
		LCD_Fill(enemies[2].x,enemies[2].y-enemies[2].speed, enemies[2].x + image_width, enemies[2].y, BLACK);
	if((enemies[3].y>=enemies[3].speed)&&(!((enemies[3].x<64)&&(enemies[3].y<32))))
		LCD_Fill(enemies[3].x,enemies[3].y-enemies[3].speed, enemies[3].x + image_width, enemies[3].y, BLACK);	
}
//初始化星空界面
void show_init()
{	
	int i = 0;
	for (i = 0; i < 50; i++) 
	{
        point_x = rand() % 240;
        point_y = rand() % 320;
		LCD_DrawPoint(point_x,point_y);
    }
//	//改变敌机的颜色
//	for(i=0;i<1400;i++)
//	{
//		if(gImage_enemy_small1[i]!=0X00)
//		{
//				//gImage_enemy_small2[i] = gImage_enemy_small1[i]+0XCC;
//				//gImage_enemy_small3[i] = gImage_enemy_small1[i]+0X6A;
//				//gImage_enemy_small4[i] = gImage_enemy_small1[i]+0XCC;
//		}

//	}
}
//手柄模式
void gamepad_mode()
{
	channel_select();
	if(adc_value[0]<100)
	{			
		pos_x_old = pos_x;				
		pos_x -=3;
		if(pos_x<=0)
			pos_x = 0;
		LCD_Fill(pos_x_old + image_width-3, pos_y, pos_x_old + image_width, pos_y + image_height, BLACK);									
	}
	else if(adc_value[0]>140)
	{
		pos_x_old = pos_x;				
		pos_x +=3;
		if(pos_x>(LCD_WIDTH-image_width))
			pos_x = LCD_WIDTH-image_width;
		LCD_Fill(pos_x_old, pos_y, pos_x_old + 3, pos_y + image_height, BLACK);						
			
	}
	if(((IO_READ->IO_MODE_READ)&0X04)!=0x04)  //          00000100
	{
		//delay_ms(10);
		if(((IO_READ->IO_MODE_READ)&0X04)!=0x04) 
		{
			if(bullet_num_show>0)
			{
				switch(bullet_num)
				{
					case 1: bullets[2].bullet_state =1 ; bullet_num_show--;break;
					case 2: bullets[1].bullet_state =1 ; bullet_num_show--;break;
					case 3: bullets[0].bullet_state =1 ; bullet_num_show--;break;
				}
				if(bullet_num>1)
				{
					bullet_num--;
				}
				else 
				{
					bullet_num=3;
				}

			}						
			//IO->IO_MODE[3]=0;
			while(((IO_READ->IO_MODE_READ)&0X04)!=0x04);
		}
	}	
}
//体感模式
void motion_mode()
{
	//体感模式  IO_READ[0]左边角度传感器(N1)   IO_READ[1]右(M1)
	if((((IO_READ->IO_MODE_READ)&0X01)==0x01)&&(((IO_READ->IO_MODE_READ)&0X02)!=0x02)) 
	{			
		pos_x_old = pos_x;				
		pos_x -=3;
		if(pos_x<=0)
			pos_x = 0;
		LCD_Fill(pos_x_old + image_width-3, pos_y, pos_x_old + image_width, pos_y + image_height, BLACK);									
	}
	else if((((IO_READ->IO_MODE_READ)&0X01)!=0x01)&&(((IO_READ->IO_MODE_READ)&0X02)==0x02))
	{
		pos_x_old = pos_x;				
		pos_x +=3;
		if(pos_x>(LCD_WIDTH-image_width))
			pos_x = LCD_WIDTH-image_width;
		LCD_Fill(pos_x_old, pos_y, pos_x_old + 3, pos_y + image_height, BLACK);						
			
	}

	if(((IO_READ->IO_MODE_READ)&0X04)!=0x04)  //          00000100
	{
		//delay_ms(10);
		if(((IO_READ->IO_MODE_READ)&0X04)!=0x04) 
		{
			if(bullet_num_show>0)
			{
				switch(bullet_num)
				{
					case 1: bullets[2].bullet_state =1 ; bullet_num_show--;break;
					case 2: bullets[1].bullet_state =1 ; bullet_num_show--;break;
					case 3: bullets[0].bullet_state =1 ; bullet_num_show--;break;
				}
				if(bullet_num>1)
				{
					bullet_num--;
				}
				else 
				{
					bullet_num=3;
				}

			}						
			//IO->IO_MODE[3]=0;
			while(((IO_READ->IO_MODE_READ)&0X04)!=0x04);
		}
	}	
}
//手势识别操控模式
void intelligence_mode()
{
	uint8_t gesture_value  = 0;
	gesture_value = Gesture_test();
	switch(gesture_value)
	{
		case 1: 
				pos_x_old = pos_x;				
				pos_x -=30;
				if(pos_x<=0)
					pos_x = 0;
				LCD_Fill(pos_x_old + image_width-30, pos_y, pos_x_old + image_width, pos_y + image_height, BLACK);								
		break;
		case 2: 
				pos_x_old = pos_x;				
				pos_x +=30;
				if(pos_x>(LCD_WIDTH-image_width))
					pos_x = LCD_WIDTH-image_width;
				LCD_Fill(pos_x_old, pos_y, pos_x_old + 30, pos_y + image_height, BLACK);						
			
		break;
		case 3: 
				pos_x_old = pos_x;				
				pos_x -=30;
				if(pos_x<=0)
					pos_x = 0;
				LCD_Fill(pos_x_old + image_width-30, pos_y, pos_x_old + image_width, pos_y + image_height, BLACK);							
		break;
		case 4:
				pos_x_old = pos_x;				
				pos_x +=30;
				if(pos_x>(LCD_WIDTH-image_width))
					pos_x = LCD_WIDTH-image_width;
				LCD_Fill(pos_x_old, pos_y, pos_x_old + 30, pos_y + image_height, BLACK);						
		break;
		case 5: 
				if(bullet_num_show>0)
				{
					switch(bullet_num)
					{
						case 1: bullets[2].bullet_state =1 ; bullet_num_show--;break;
						case 2: bullets[1].bullet_state =1 ; bullet_num_show--;break;
						case 3: bullets[0].bullet_state =1 ; bullet_num_show--;break;
					}
					if(bullet_num>1)
					{
						bullet_num--;
					}
					else 
					{
						bullet_num=3;
					}

				}						
		break;
		case 6: 
				if(bullet_num_show>0)
				{
					switch(bullet_num)
					{
						case 1: bullets[2].bullet_state =1 ; bullet_num_show--;break;
						case 2: bullets[1].bullet_state =1 ; bullet_num_show--;break;
						case 3: bullets[0].bullet_state =1 ; bullet_num_show--;break;
					}
					if(bullet_num>1)
					{
						bullet_num--;
					}
					else 
					{
						bullet_num=3;
					}

				}						
		break;
//			case 7: break;
//			case 8: break;
//			case 9: break;
	}					
}
//按键操控模式
void key_mode()
{
	switch(key_value)
	{			
		case 1:	
				pos_x_old = pos_x;				
				pos_x -=10;
				if(pos_x<=0)
					pos_x = 0;
				LCD_Fill(pos_x_old + image_width-10, pos_y, pos_x_old + image_width, pos_y + image_height, BLACK);				

		break;
		case 2:
				pos_x_old = pos_x;				
				pos_x +=10;
				if(pos_x>(LCD_WIDTH-image_width))
					pos_x = LCD_WIDTH-image_width;
				LCD_Fill(pos_x_old, pos_y, pos_x_old + 10, pos_y + image_height, BLACK);						
		break;
		case 3:	
				if(bullet_num_show>0)
				{
					switch(bullet_num)
					{
						case 1: bullets[2].bullet_state =1 ; bullet_num_show--;break;
						case 2: bullets[1].bullet_state =1 ; bullet_num_show--;break;
						case 3: bullets[0].bullet_state =1 ; bullet_num_show--;break;
					}
					if(bullet_num>1)
					{
						bullet_num--;
					}
					else 
					{
						bullet_num=3;
					}

				}								
		break;
	}	
}
void collision_check()
{
	if(hit>0)
	{
		hit++;

	}
	if(hit==4)
	{
		LCD_Fill(hit_x,hit_y,hit_x+20,hit_y+18,BLACK);
		hit = 0;			
	}
	for(i=0;i<ENEMY_NUM;i++)
	{
		is_plane_hit = check_hit(&enemies[i]);
		if(is_plane_hit==1)
		{
			show_num=2;
			sprintf(submit_char,"AT+MQTTPUB=0,\"esp8266_xyx\",\"%d%d%d\",0,0\r\n",score/100,score%100/10,score%10);
			UART2String(submit_char);
			LCD_Clear(BLACK);
			LCD_Fill(0,0,240,320,BLACK);
			for (i = 0; i < 50; i++) 
			{
				point_x = rand() % 240;
				point_y = rand() % 320;
				LCD_DrawPoint(point_x,point_y);
			}
			break;
		}
		for(j=0;j<BULLET_NUM;j++)
		{
			is_hit = check_collision(&enemies[i], &bullets[j]);
			if(is_hit==1)
			{
				bullets[j].bullet_state=0;
				bullet_num_show++;
				hit = 1;
				/*if(j==2)
					bullet_num_show = 3;*/
			}
		}
		
	}	
}
void level_improve()
{
	if(score>10)
	{
		for (i = 0; i < ENEMY_NUM; i++) 
		{
			// 设置敌机速度等信息
			enemies[i].speed = 6+score/10;

		}			
	}	
}
void volumn_change()
{
	if((((IO_READ->IO_MODE_READ)&0X08)!=0x08)&&(((IO_READ->IO_MODE_READ)&0X10)!=0x10))    //OUT2 ->IO_MODE_READ[4]  OUT1 ->IO_MODE_READ[3]
	{
		if((((IO_READ->IO_MODE_READ)&0X08)!=0x08)&&(((IO_READ->IO_MODE_READ)&0X10)!=0x10))
		{
			volume += 5;
			if(volume>30)
			{
				volume = 30;
			}
		
		}
	}

	if((((IO_READ->IO_MODE_READ)&0X08)!=0x08)&&(((IO_READ->IO_MODE_READ)|0XEF)!=0XEF))
	{
		if((((IO_READ->IO_MODE_READ)&0X08)!=0x08)&&(((IO_READ->IO_MODE_READ)|0XEF)!=0XEF))
		{
			volume -= 5;
			if(volume<0)
			{
				volume = 0;
			}
			
		}
	}	
	Uart_SendCmd(0X06,0,volume);		
}





