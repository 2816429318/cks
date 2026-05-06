#include "lvgl.h"
#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "game_api.h"

lv_img_dsc_t image_struct,image_pig_small,image_pig_big;
lv_img_dsc_t image_fruit[3];

// create image of big pig and small pig, and store them in image_pig_big and image_pig_small
void create_pig_images(void)
{
	uint8_t* image_buffer = sdram_malloc( 100 * 100 * 3 + 4 );
	read_file_to_array("0:/ui_pig_small.bin", image_buffer,  100 * 100 * 3 + 4 );
	image_pig_small.header.always_zero = 0;
	image_pig_small.header.cf = LV_IMG_CF_TRUE_COLOR;
	image_pig_small.header.w = 100;
	image_pig_small.header.h = 100;
	image_pig_small.header.reserved = 0;
	image_pig_small.data_size = 100 * 100 * 3;
	image_pig_small.data = image_buffer + 4; 

	image_buffer = sdram_malloc( 100 * 100 * 3 + 4 );
	read_file_to_array("0:/ui_pig_big.bin", image_buffer,  100 * 100 * 3 + 4 );
	image_pig_big.header.always_zero = 0;
	image_pig_big.header.cf = LV_IMG_CF_TRUE_COLOR;
	image_pig_big.header.w = 100;
	image_pig_big.header.h = 100;
	image_pig_big.header.reserved = 0;
	image_pig_big.data_size = 100 * 100 * 3;
	image_pig_big.data = image_buffer + 4; 
}

// create images of fruits and store them in image_fruit array
void create_fruit_images(void)
{
	uint8_t* image_buffer = sdram_malloc( 60 * 71 * 3 + 4 );
	read_file_to_array("0:/fruit1.bin", image_buffer,  60 * 71 * 3 + 4 );
	image_fruit[0].header.always_zero = 0;
	image_fruit[0].header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	image_fruit[0].header.w = 60;
	image_fruit[0].header.h = 71;
	image_fruit[0].header.reserved = 0;
	image_fruit[0].data_size = 60 * 71 * 3;
	image_fruit[0].data = image_buffer + 4;

	image_buffer = sdram_malloc( 66 * 63 * 3 + 4 );
	read_file_to_array("0:/fruit2.bin", image_buffer,  66 * 63 * 3 + 4 );
	image_fruit[1].header.always_zero = 0;
	image_fruit[1].header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	image_fruit[1].header.w = 66;
	image_fruit[1].header.h = 63;
	image_fruit[1].header.reserved = 0;
	image_fruit[1].data_size = 66 * 63 * 3;
	image_fruit[1].data = image_buffer + 4;

	image_buffer = sdram_malloc( 68 * 73 * 3 + 4 );
	read_file_to_array("0:/fruit3.bin", image_buffer,  68 * 73 * 3 + 4 );
	image_fruit[2].header.always_zero = 0;
	image_fruit[2].header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	image_fruit[2].header.w = 68;
	image_fruit[2].header.h = 73;
	image_fruit[2].header.reserved = 0;
	image_fruit[2].data_size = 68 * 73 * 3;
	image_fruit[2].data = image_buffer + 4;
}

//	pig shack animation
static void pig_shake_anim_callback(void* var, int32_t v)
{
    int idx = (int)(uintptr_t)var;
    lv_obj_set_x(pig[idx].img_pig, pig[idx].x + v);
}

// 抖动函数（现在只需要猪的索引）
void pig_shack_anim(int pig_idx)
{
    lv_obj_t *pig_img = pig[pig_idx].img_pig;

    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, (void*)(uintptr_t)pig_idx);
    lv_anim_set_exec_cb(&anim, pig_shake_anim_callback);

    lv_anim_set_values(&anim, -2, 2);
    lv_anim_set_time(&anim, 400);
    lv_anim_set_playback_time(&anim, 400);
    lv_anim_set_repeat_count(&anim, LV_ANIM_REPEAT_INFINITE);

    lv_anim_start(&anim);
}

void ui_game_screen(lv_event_t *e)
{
	lv_obj_t *game_screen = lv_obj_create(NULL);
	lv_obj_clear_flag(game_screen, LV_OBJ_FLAG_SCROLLABLE);
	// Create game screen
	uint8_t* image_buffer = sdram_malloc( 1024 * 600 * 3 + 4 );
	read_file_to_array("0:/main.bin", image_buffer,  1024 * 600 * 3 + 4 );
	image_struct.header.always_zero = 0;
	image_struct.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
	image_struct.header.w = 1024;
	image_struct.header.h = 600;
	image_struct.header.reserved = 0;
	image_struct.data_size = 1024 * 600 * 3;
	image_struct.data = image_buffer + 4; 
	lv_obj_t *img_main = lv_img_create(game_screen);
	lv_img_set_src(img_main, &image_struct);

    //pig
	create_pig_images();
	for(int i=0;i<10;i++){
		pig[i].x = 170+i%5*120 + 50;
		pig[i].y = (i>4)?350:200;
		pig[i].img_pig = lv_img_create(game_screen);
		pig[i].size = 0; // start with small pig
		if(pig[i].size == 0){
			lv_img_set_src(pig[i].img_pig, &image_pig_small);
			lv_obj_set_pos(pig[i].img_pig, pig[i].x, pig[i].y);
		}else{
			lv_img_set_src(pig[i].img_pig, &image_pig_big);
			lv_obj_set_pos(pig[i].img_pig, pig[i].x, pig[i].y);
		}
		pig_shack_anim(i);
	}

	//create fruit images
	create_fruit_images();

	//测试代码，后续会删除
	lv_obj_t *btn_back1 = lv_btn_create(game_screen);
	lv_obj_set_size(btn_back1, 80, 40);
	lv_obj_align(btn_back1, LV_ALIGN_TOP_LEFT, 10, 10);
	lv_obj_add_event_cb(btn_back1,pig_grow_anim, LV_EVENT_RELEASED, NULL);

	lv_obj_t *btn_back2 = lv_btn_create(game_screen);
	lv_obj_set_size(btn_back2, 80, 40);
	lv_obj_align(btn_back2, LV_ALIGN_TOP_LEFT, 100, 10);
	lv_obj_add_event_cb(btn_back2,pig_small_anim, LV_EVENT_RELEASED, NULL);

	lv_obj_t *btn_back3 = lv_btn_create(game_screen);
	lv_obj_set_size(btn_back3, 80, 40);
	lv_obj_align(btn_back3, LV_ALIGN_TOP_LEFT, 190, 10);
	lv_obj_add_event_cb(btn_back3,pig_feed_anim, LV_EVENT_RELEASED, NULL);

    lv_scr_load_anim(
        game_screen,
        LV_SCR_LOAD_ANIM_FADE_ON,
        300,
        0,
        true
    );
}