#ifndef __GAME_H
#define __GAME_H
#include "stdint.h"
#include "code_def.h"
#define ENEMY_NUM 4
#define BULLET_NUM 3
// �洢ÿ���л���λ����Ϣ
struct enemy_t {
    uint16_t x;   // �л�������
    int16_t y;   // �л�������
    uint16_t speed;   // �л��ƶ��ٶ�
};
// ����ṹ�����ڱ�ʾ�л�λ��
struct position_t {
    uint16_t x;
    int16_t y;
};
// �洢ÿ���ӵ���λ����Ϣ
struct bullet_t {
    uint16_t x;   // �л�������
    int16_t y;   // �л�������
    uint16_t speed;   // �л��ƶ��ٶ�
	uint16_t bullet_state;
	unsigned char bullet_en;
};
uint32_t lfsr_rand(void);
void generate_random_position(uint16_t *x, int16_t *y,unsigned char i);
void draw_enemy(struct enemy_t *enemy,unsigned char i);
void move_enemies();
void init_enemies();
void init_bullets();
void update_bullet_position();
uint8_t check_collision(struct enemy_t *e, struct bullet_t *b);
uint8_t check_hit(struct enemy_t *e);
void game_init();
#endif