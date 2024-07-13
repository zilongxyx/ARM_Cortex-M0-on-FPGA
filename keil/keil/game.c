#include "game.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
unsigned int image_width = 28;
unsigned int image_height = 25;
//  unsigned char gImage_enemy1[2880];
//  unsigned char gImage_enemy2[2880];
//  unsigned char gImage_enemy3[2880];
//  unsigned char gImage_enemy4[2880];
extern unsigned char gImage_enemy_small1[1400];
extern unsigned char gImage_enemy_small2[1400];
extern unsigned char gImage_enemy_small3[1400];
extern unsigned char gImage_enemy_small4[1400];
extern   unsigned char gImage_bullet1[420];
extern   unsigned char gImage_hero1[2880];
extern   unsigned char gImage_bomb[720];
uint32_t colorTab[16] = {BLACK,BLUE,BRED,GRED,GBLUE,RED,MAGENTA,GREEN,CYAN,YELLOW,BROWN,BRRED,GRAY,BRED,GRED,GBLUE};
char posx_string[10];
char posy_string[10];
char test2_string[10];
char test3_string[10];
char test4_string[10];
char test5_string[10];
char test6_string[10];
char test7_string[10];
uint8_t is_hit = 0;
int pos_x=106,pos_y=295;  //320 -36
int bullet_x[3]={0},bullet_y[3]={0};
int pos_x_old=0,pos_y_old=0;
unsigned char bullet_num = 3; // ������ӵ�
unsigned char bullet_num_show = 3; //��ʾʣ����ӵ�
unsigned char bullet_en[3]={0};
uint8_t score = 0;
uint16_t hit_x=0,hit_y=0;
// �л�����
int num_enemies = 0;
char tune = 0;

struct bullet_t bullets[3];

struct enemy_t enemies[4];
// �洢�Ѿ����ɵĵл�λ��
struct position_t positions[10] = {{0, 0}};
// ��ʼ����ֵ
uint32_t seed = 0xACE1u;
// ������ʽ�����ݲ�ͬ�Ķ���ʽ���Բ�����ͬ���������
uint32_t polynomial = 0xB4BCD35C;

// LFSR ��������ɺ���
uint32_t lfsr_rand(void) 
{
    // �������λ�Ͷ���ʽ�����
    uint32_t new_bit = (seed ^ (seed >> 1) ^ (seed >> 3) ^ (seed >> 12)) & 1;
    uint32_t xor_result = ((seed >> 1) & 1u) ^ ((seed >> 9) & 1u) ^ ((seed >> 19) & 1u) ^ ((seed >> 24) & 1u);
  
    // ��������ֵ�������µı���λ�Ͷ���ʽ�������䵽���λ
    seed = (seed >> 1) | (new_bit << 31);

    // �������ʽ�����Ϊ 1�������λȡ��
    if (xor_result == 1) {
        seed ^= polynomial;
    }

    // ���������
    return seed;
}

// �������λ��
/*void generate_random_position(uint16_t *x, int16_t *y) {
    // ������Ļ��С�͵л���С����Ϸ������λ��
	*x = rand() % (240 - 40);   // �л���������0~200��Χ��
    *y = 0;   // �л�����������Ļ�⣬ȷ���Ӷ�����ʼ�ƶ�
}*/

void generate_random_position(uint16_t *x, int16_t *y,unsigned char i) 
{
	int overlap = 0;
	int j = 0;
    do {
        *x = rand() % (240 - 40);   // �л���������0~200��Χ��
		switch(i)
		{
			case 0: *y = -36; break;
			case 1: *y = -40;break;
			case 2: *y = -50; break;
			case 3: *y = -60; break;
		}
        //*y = -36;   // �л�����������Ļ�⣬ȷ���Ӷ�����ʼ�ƶ�

        // ����Ƿ������ел��ص�
        for (j = 0; j < num_enemies; j++) 
		{
            if ((*x >= positions[j].x - 28 && *x <= positions[j].x + 28)&&( (*y >= positions[j].y - 25 && *y <= positions[j].y + 25))) 
			{
                overlap = 1;
                break;
            }
			else 
			{
				overlap = 0;
			}
        }
		/*sprintf(test5_string,"enemiex0:%d:%d\r\n",enemies[0].x,enemies[0].y);
		UART1String(test5_string);
		sprintf(test6_string,"enemiex1:%d:%d\r\n",enemies[1].x,enemies[1].y);
		UART1String(test6_string);
		sprintf(test7_string,"enemiex2:%d:%d\r\n",enemies[2].x,enemies[2].y);
		UART1String(test7_string);
		sprintf(test4_string,"num_enemies:%d\r\n",num_enemies);
		UART1String(test4_string);
		sprintf(test3_string,"positions[0]:%d:%d\r\n",positions[0].x,positions[0].y);
		UART1String(test3_string);
		sprintf(test2_string,"positions[1]:%d:%d\r\n",positions[1].x,positions[1].y);
		UART1String(test2_string);*/
    } while (overlap);
    // ����ǰ�л�λ����ӵ�������
    positions[num_enemies].x = *x;
    positions[num_enemies].y = *y;
    num_enemies++;
}
// ���Ƶл�
void draw_enemy(struct enemy_t *enemy,unsigned char i) 
{
    // ʹ��LCD��Ļ���ƺ������л����Ƶ���Ļ��
	unsigned char j = 0;
	uint16_t x = enemy->x;
	uint16_t y = enemy->y;
	//i = rand() % 3;  // 64*32
	if((x<64)&&(y<32))
	{
		j++;
	}
	else 
	{
		switch(i)
		{
			case 0: LCD_DrawPicture(x,y,x+image_width,y+image_height,gImage_enemy_small1);break;
			case 1: LCD_DrawPicture(x,y,x+image_width,y+image_height,gImage_enemy_small2); break;
			case 2: LCD_DrawPicture(x,y,x+image_width,y+image_height,gImage_enemy_small3);break;
			case 3: LCD_DrawPicture(x,y,x+image_width,y+image_height,gImage_enemy_small4);break;
		}		
	}
}

// �ƶ��л�
void move_enemies() 
{
	uint16_t point_x,point_y;
	uint16_t point_i;
	int i = 0;
	int j = 0;
	for ( i = 0; i < ENEMY_NUM; i++) 
	{
		// ���µл���λ�ò����»���
		if(enemies[i].y<320)
		{
			enemies[i].y += enemies[i].speed;
			draw_enemy(&enemies[i],i);	
			for(j = 0; j < num_enemies; j++)
			{
				positions[j].x = 0;
				positions[j].y = 0;
			}
			num_enemies = 0;			
		}
		else 
		{
			for (point_i = 0; point_i < 20; point_i++) 
			{
				point_x = rand() % 200 + 20;
				point_y = rand() % 284+36;
				LCD_DrawPoint(point_x,point_y);
			}
			switch(i)
			{
				case 0: 
						positions[1].x = enemies[1].x;
						positions[1].y = enemies[1].y;
						positions[2].x = enemies[2].x;
						positions[2].y = enemies[2].y;
						positions[3].x = enemies[3].x;
						positions[3].y = enemies[3].y;
				break;
				case 1: 
						positions[0].x = enemies[0].x;
						positions[0].y = enemies[0].y;
						positions[2].x = enemies[2].x;
						positions[2].y = enemies[2].y;
						positions[3].x = enemies[3].x;
						positions[3].y = enemies[3].y;
				break;
				case 2: 
						positions[0].x = enemies[0].x;
						positions[0].y = enemies[0].y;
						positions[1].x = enemies[1].x;
						positions[1].y = enemies[1].y;
						positions[3].x = enemies[3].x;
						positions[3].y = enemies[3].y;
				break;
				case 3: 
						positions[0].x = enemies[0].x;
						positions[0].y = enemies[0].y;
						positions[1].x = enemies[1].x;
						positions[1].y = enemies[1].y;
						positions[2].x = enemies[2].x;
						positions[2].y = enemies[2].y;
				break;
			}
			num_enemies = 3;
			generate_random_position(&enemies[i].x, &enemies[i].y,i);
		}

	}

}

// ��ʼ���л�
void init_enemies() 
{
	unsigned char i = 0;
    for (i = 0; i < ENEMY_NUM; i++) 
	{
        // �������λ��
        generate_random_position(&enemies[i].x, &enemies[i].y,i);
        
        // ���õл��ٶȵ���Ϣ
        enemies[i].speed = 6;
        // ���Ƶл�
		draw_enemy(&enemies[i],i);
    }
}
// ��ʼ���ӵ�
void init_bullets() 
{
	unsigned char i = 0;
    for (i = 0; i < BULLET_NUM; i++) 
	{
		bullets[i].x = 0;
		bullets[i].y = 0;
		// ���õл��ٶȵ���Ϣ
		bullets[i].speed = 10;
		bullets[i].bullet_en = 0;
		bullets[i].bullet_state = 0;

    }
}


void update_bullet_position()
{
	int i, point_x, point_y;
	if(bullets[0].bullet_state==1)  // �ӵ�0����
	{

			if(bullets[0].y>bullets[0].speed)     //����λ��
			{
				//UART1String("666\r\n");
				bullets[0].y = bullets[0].y-bullets[0].speed;

				LCD_Fill(bullets[0].x, bullets[0].y+21,bullets[0].x+10,bullets[0].y+21+bullets[0].speed, BLACK);
				LCD_DrawPicture(bullets[0].x,bullets[0].y,bullets[0].x+10,bullets[0].y+21,gImage_bullet1);
			}
			else
			{
				if(is_hit==0)
					bullet_num_show++;
				bullets[0].bullet_en=0;
				bullets[0].bullet_state = 0;
				LCD_Fill(bullets[0].x, bullets[0].y,bullets[0].x+10,bullets[0].y+21, BLACK);
			}
	}
	else 
	{
		bullets[0].x = pos_x+14-5;
		bullets[0].y = pos_y;	
		//bullets[0].y = pos_y-18-10;		
	}	
	
	if(bullets[1].bullet_state==1)
	{
			if(bullets[1].y>bullets[1].speed)     //����λ��
			{
				bullets[1].y = bullets[1].y-bullets[1].speed;
				LCD_Fill(bullets[1].x, bullets[1].y+21,bullets[1].x+10,bullets[1].y+21+bullets[1].speed, BLACK);
				LCD_DrawPicture(bullets[1].x,bullets[1].y,bullets[1].x+10,bullets[1].y+21,gImage_bullet1);
			}
			else
			{
				if(is_hit==0)
					bullet_num_show++;
				bullets[1].bullet_en=0;
				bullets[1].bullet_state = 0;
				LCD_Fill(bullets[1].x, bullets[1].y,bullets[1].x+10,bullets[1].y+21, BLACK);
			}
	}
	else 
	{
		bullets[1].x = pos_x+14-5;
		bullets[1].y = pos_y;	
		//bullets[1].y = pos_y-18-10;		
	}	
	
	
	if(bullets[2].bullet_state==1)
	{
			if(bullets[2].y>bullets[2].speed)     //����λ��
			{
				bullets[2].y = bullets[2].y-bullets[2].speed;
				LCD_Fill(bullets[2].x, bullets[2].y+21,bullets[2].x+10,bullets[2].y+21+bullets[2].speed, BLACK);
				LCD_DrawPicture(bullets[2].x,bullets[2].y,bullets[2].x+10,bullets[2].y+21,gImage_bullet1);
			}
			else
			{
				bullets[2].bullet_en=0;
				bullets[2].bullet_state = 0;
				if(is_hit==0)
					bullet_num_show++;
				LCD_Fill(bullets[2].x, bullets[2].y,bullets[2].x+10,bullets[2].y+21, BLACK);
			}
	}
	else 
	{
		bullets[2].y = pos_y;
		bullets[2].x = pos_x+14-5;
		//bullets[2].y = pos_y-18-10;		
	}
	
	

}
uint8_t check_collision(struct enemy_t *e, struct bullet_t *b) 
{
    // ����л����ӵ�����������
	uint8_t bullet_hit = 0;
	int enemy_y,enemy_x;
	int bullet_center_x,bullet_y;
	
	enemy_x = e->x;
	enemy_y	= e->y+25;
	bullet_center_x = b->x+5;
	bullet_y = b->y;

	if(((enemy_x-5)<=bullet_center_x)&&((enemy_x+28+5)>=bullet_center_x))
	{
		if((bullet_y>=(enemy_y-5))&&(bullet_y<=(enemy_y+10)))
		{
			if((enemy_y+10)<(pos_y))//��ֹ��⵽û������ӵ�
			{
				score = score+1;
				LCD_DrawPicture(e->x,e->y,e->x+20,e->y+18,gImage_bomb);
				LCD_Fill(e->x+20, e->y, e->x+40, e->y+18, BLACK);
				LCD_Fill(e->x, e->y+18, e->x+40, e->y+36, BLACK);
				LCD_Fill(b->x, b->y, b->x+10, b->y+21, BLACK);
				hit_x = e->x;
				hit_y = e->y;
				//bullets[0].bullet_state=0;
				bullet_hit = 1;
				//b->x = pos_x+20-5;b->y= pos_y-18-10;	
				//LCD_Fill(e->x,e->y,e->x+20,e->y+18,BLACK);
				e->y  = 365;
			}
		}
			
			
	}	
	return bullet_hit;
}
uint8_t check_hit(struct enemy_t *e) 
{
	uint8_t plane_hit = 0;
    // ����л�����
	int enemy_y,enemy_x;
	enemy_x = e->x;
	enemy_y	= e->y+25+10;
	if((enemy_x<=(pos_x+28))&&(enemy_x>=(pos_x-28)))
	{
		//UART1String("666\r\n");
		if(enemy_y>pos_y)
		{
			//UART1String("777\r\n");
			plane_hit = 1;
		}
	}
	return plane_hit;
	
}
//��Ϸ��ʼ��
void game_init()
{
	score = 0;
	positions[0].x = 0;
	positions[0].y = 0;
	positions[1].x = 0;
	positions[1].y = 0;
	positions[2].x = 0;
	positions[2].y = 0;
	positions[3].x = 0;
	positions[3].y = 0;
	num_enemies = 0;
	init_enemies();
	init_bullets();	
	bullet_num_show=3;		
}