/**
 * @file main
 *
 */

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#define SDL_MAIN_HANDLED /*To fix SDL's "undefined reference to WinMain" issue*/
#include <SDL2/SDL.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/sdl/sdl.h"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void hal_init(void);


using namespace std;

int main(int argc, char *argv[])
{
	char sz[] = "LVGL Example 1";	//Hover mouse over "sz" while debugging to see its contents
	cout << sz << endl;	//<================= Put a breakpoint here
	
	(void)argc; /*Unused*/
	(void)argv; /*Unused*/

	/*Initialize LVGL*/
	lv_init();
	
	/*Initialize the HAL (display, input devices, tick) for LVGL*/
	hal_init();	
	
//	lv_demo_widgets();  /// LAS TESTED
//	lv_example_btn_1(); /// LAS TESTED
//  lv_example_switch_1();
//  lv_example_calendar_1();
//  lv_example_btnmatrix_2();
//  lv_example_checkbox_1();
//  lv_example_colorwheel_1();
//  lv_example_chart_6();
//  lv_example_table_2();
//  lv_example_scroll_2();
//	lv_example_textarea_1(); /// LAS TESTED
//  lv_example_msgbox_1();
//  lv_example_dropdown_2();
//  lv_example_scroll_1();
//  lv_example_tabview_1();
//  lv_example_tabview_1();
//  lv_example_flex_3();
//  lv_example_label_1();
		
	lv_example_label_CAR();
	lv_example_arc_2();
	
//	lv_example_win_1();
//	lv_example_tileview_1();
	
	set_lv_example_label_CAR();
	
	static int count = 0;
	
	while (1)
	{
		/* Periodically call the lv_task handler.
		 * It could be done in a timer interrupt or an OS task too.*/		
		lv_task_handler();
		usleep(5 * 1000);
		if (count > 200)
		{
			
			cout << "count: " << count << endl;
			count = 0;
			set_lv_example_label_CAR();
		}
		count++;
	}	
	
	return 0;
}

extern void callback_set_lv_example_label_CAR(void)
{
	cout << "callback_set_lv_example_label_CAR" << endl;			
}


/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static void hal_init(void)
{
	/* Use the 'monitor' driver which creates window on PC's monitor to simulate a display*/
	sdl_init();

	/*Create a display buffer*/
	static lv_disp_draw_buf_t disp_buf1;
	static lv_color_t buf1_1[SDL_HOR_RES * 100];
	lv_disp_draw_buf_init(&disp_buf1, buf1_1, NULL, SDL_HOR_RES * 100);

	/*Create a display*/
	static lv_disp_drv_t disp_drv;
	lv_disp_drv_init(&disp_drv); /*Basic initialization*/
	disp_drv.draw_buf = &disp_buf1;
	disp_drv.flush_cb = sdl_display_flush;
	disp_drv.hor_res = SDL_HOR_RES;
	disp_drv.ver_res = SDL_VER_RES;
	disp_drv.antialiasing = 1;

	lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

	lv_theme_t * th = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK, LV_FONT_DEFAULT);
	lv_disp_set_theme(disp, th);

	lv_group_t * g = lv_group_create();
	lv_group_set_default(g);

	/* Add the mouse as input device
	 * Use the 'mouse' driver which reads the PC's mouse*/
	static lv_indev_drv_t indev_drv_1;
	lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
	indev_drv_1.type = LV_INDEV_TYPE_POINTER;

	/*This function will be called periodically (by the library) to get the mouse position and state*/
	indev_drv_1.read_cb = sdl_mouse_read;
	lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

	static lv_indev_drv_t indev_drv_2;
	lv_indev_drv_init(&indev_drv_2); /*Basic initialization*/
	indev_drv_2.type = LV_INDEV_TYPE_KEYPAD;
	indev_drv_2.read_cb = sdl_keyboard_read;
	lv_indev_t *kb_indev = lv_indev_drv_register(&indev_drv_2);
	lv_indev_set_group(kb_indev, g);

	static lv_indev_drv_t indev_drv_3;
	lv_indev_drv_init(&indev_drv_3); /*Basic initialization*/
	indev_drv_3.type = LV_INDEV_TYPE_ENCODER;
	indev_drv_3.read_cb = sdl_mousewheel_read;
	lv_indev_t * enc_indev = lv_indev_drv_register(&indev_drv_3);
	lv_indev_set_group(enc_indev, g);

	/*Set a cursor for the mouse*/
	LV_IMG_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
	lv_obj_t * cursor_obj = lv_img_create(lv_scr_act()); /*Create an image object for the cursor */
	lv_img_set_src(cursor_obj, &mouse_cursor_icon); /*Set the image source*/
	lv_indev_set_cursor(mouse_indev, cursor_obj); /*Connect the image  object to the driver*/
}
