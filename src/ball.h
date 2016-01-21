#pragma once

#include "commons.h"


// config file path
#define BILLIARD_BALL_CONFIG_FILE "./config/billiard_ball.config"
// radius of the billard balls
#define BILLIARD_BALL_RADIUS 11
#define BILLIARD_BALL_COUNT 18
// physics constants
#define BILLIARD_BALL_FRICTION -0.09
#define COLLISION_FRICTION 0.96
#define TABLE_COLLISION_FRICTION 0.80
#define COLLISION_PREVENTION 0.02
#define BALL_COLLISOION_SOUND_GAIN 0.6
// white ball default location
#define WHITE_BALL_DEFAULT_X 600
#define WHITE_BALL_DEFAULT_Y 400
// redefine
#define BCONFIG_FPATH BILLIARD_BALL_CONFIG_FILE
#define BALL_COUNT BILLIARD_BALL_COUNT
#define CP COLLISION_PREVENTION
#define WB_X WHITE_BALL_DEFAULT_X
#define WB_Y WHITE_BALL_DEFAULT_Y
#define BC_SOUND_GAIN BALL_COLLISOION_SOUND_GAIN


typedef struct billiard_ball_t
{
	int number, score, is_on_table;
	double cx, cy, dx, dy;	
	double bitmap_sw, bitmap_sh;
	ALLEGRO_BITMAP *bitmap;
	ALLEGRO_SAMPLE *collision_sound;
	ALLEGRO_COLOR color;
} billiard_ball;


billiard_ball *create_billiard_balls();
void destroy_billiard_balls(billiard_ball *);
void reset_billiard_balls(billiard_ball *);
int is_ball_collided(billiard_ball, billiard_ball);
int is_ball_in_bag(billiard_ball);
int is_mouse_on_ball(billiard_ball, const int, const int);
int is_all_ball_stopped(billiard_ball *);
int is_all_ball_in_bag(billiard_ball *);
int is_ball_on_table_vertical_border(billiard_ball);
int is_ball_on_table_horizontal_border(billiard_ball);
void set_velocity_to_ball(billiard_ball *, const int, const double ,const double);
void set_white_ball_to_original_place(billiard_ball *);
int *get_ball_status(billiard_ball *);
void update_billiard_balls(billiard_ball *);
void draw_billiard_balls(billiard_ball *);

// end of billiard ball definition

