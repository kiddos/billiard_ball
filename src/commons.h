#ifndef COMMONS_H
#define COMMONS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

/* math constants */
extern const double PI;

/* window rendering constants */
extern const double DEFAULT_WINDOW_WIDTH;
extern const double DEFAULT_WINDOW_HEIGHT;
extern const double FPS;
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// table width and height and position
#define TABLE_WIDTH 800
#define TABLE_HEIGHT 400
#define TABLE_Y_POSITION 200
#define TABLE_BORDER_WIDTH 26

// bag size
#define BAG_RADIUS 27

// table border locations
// horizontal borders
#define HORIZONTAL_BORDER_START_X1 (TABLE_BORDER_WIDTH + BAG_RADIUS)
#define HORIZONTAL_BORDER_START_X2 (TABLE_WIDTH / 2 + BAG_RADIUS)
#define HORIZONTAL_BORDER_END_X1 (TABLE_WIDTH / 2 - BAG_RADIUS)
#define HORIZONTAL_BORDER_END_X2 (TABLE_WIDTH - TABLE_BORDER_WIDTH - BAG_RADIUS)
#define HORIZONTAL_BORDER_START_Y (TABLE_Y_POSITION + TABLE_BORDER_WIDTH)
#define HORIZONTAL_BORDER_END_Y (TABLE_Y_POSITION + \
		TABLE_HEIGHT - TABLE_BORDER_WIDTH)
// vertical borders
#define VERTICAL_BORDER_START_X (TABLE_BORDER_WIDTH)
#define VERTICAL_BORDER_END_X (TABLE_WIDTH - TABLE_BORDER_WIDTH)
#define VERTICAL_BORDER_START_Y (TABLE_Y_POSITION + \
		TABLE_BORDER_WIDTH + BAG_RADIUS)
#define VERTICAL_BORDER_END_Y (TABLE_Y_POSITION + \
		TABLE_HEIGHT - TABLE_BORDER_WIDTH - BAG_RADIUS)
// color definition
#define COLOR_BLACK (al_map_rgb(0, 0, 0))
#define COLOR_WHITE (al_map_rgb(255, 255, 255))
#define COLOR_GREY (al_map_rgb(166, 166, 166))

// redefine
#define BALL_RADIUS BILLIARD_BALL_RADIUS
#define BALL_FRICTION BILLIARD_BALL_FRICTION
#define BORDER_WIDTH TABLE_BORDER_WIDTH
#define Y_POS TABLE_Y_POSITION
#define HB_SX1 HORIZONTAL_BORDER_START_X1
#define HB_SX2 HORIZONTAL_BORDER_START_X2
#define HB_EX1 HORIZONTAL_BORDER_END_X1
#define HB_EX2 HORIZONTAL_BORDER_END_X2
#define HB_SY HORIZONTAL_BORDER_START_Y
#define HB_EY HORIZONTAL_BORDER_END_Y
#define VB_SX VERTICAL_BORDER_START_X
#define VB_EX VERTICAL_BORDER_END_X
#define VB_SY VERTICAL_BORDER_START_Y
#define VB_EY VERTICAL_BORDER_END_Y

#endif /* end of include guard: COMMONS_H */
