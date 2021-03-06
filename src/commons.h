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
#include <allegro5/allegro_opengl.h>

#include "message.h"

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
ALLEGRO_COLOR color_black();
ALLEGRO_COLOR color_white();
ALLEGRO_COLOR color_gray();
ALLEGRO_COLOR color_red();
ALLEGRO_COLOR color_orange();
ALLEGRO_COLOR color_yellow();
ALLEGRO_COLOR color_green();
ALLEGRO_COLOR color_blue();
ALLEGRO_COLOR color_purple();

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

typedef struct point_t {
  double x, y, z;
} point, point_t;

typedef struct rect_t {
  double x, y, z;
  double width, height, depth;
} rect, rect_t;

typedef struct size {
  double width, height, depth;
} size;

point point_init(double x, double y, double z);
double point_get_x(point p);
double point_get_y(point p);
double point_get_z(point p);

rect rect_init(double x, double y, double z,
               double w, double h, double d);
double rect_get_x(rect r);
double rect_get_y(rect r);
double rect_get_z(rect r);
double rect_get_w(rect r);
double rect_get_h(rect r);
double rect_get_d(rect r);

size size_init(double width, double height, double depth);
double size_get_width(size s);
double size_get_height(size s);
double size_get_depth(size s);

#endif /* end of include guard: COMMONS_H */
