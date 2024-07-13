#ifndef __GAME_H
#define __GAME_H
#include "stdint.h"
#include "code_def.h"
#define ENEMY_NUM 4
#define BULLET_NUM 3
// 存储每个敌机的位置信息
struct enemy_t {
    uint16_t x;   // 敌机横坐标
    int16_t y;   // 敌机纵坐标
    uint16_t speed;   // 敌机移动速度
};
// 定义结构体用于表示敌机位置
struct position_t {
    uint16_t x;
    int16_t y;
};
// 存储每个子弹的位置信息
struct bullet_t {
    uint16_t x;   // 敌机横坐标
    int16_t y;   // 敌机纵坐标
    uint16_t speed;   // 敌机移动速度
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