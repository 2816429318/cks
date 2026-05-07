#ifndef __GAME_API_H
#define __GAME_API_H

#include "lvgl.h"

typedef struct {
	int x,y;
	int size; // 0 for small pig, 1 for big pig
	lv_obj_t *img_pig;
}pig_t;

extern pig_t pig[10];

void ui_game_start(void);
void ui_game_screen(lv_event_t *e);
void pig_grow_anim(int pig_idx);
void pig_small_anim(int pig_idx);
void pig_feed_anim(int pig_idx,int fruit_idx);
void fruit_cb(lv_event_t *e);

#endif