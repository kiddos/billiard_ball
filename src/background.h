#pragma once

#include "commons.h"


#define BACKGROUND_CONFIG_FILE_PATH "./config/background.config"
#define BG_CONFIG_FPATH BACKGROUND_CONFIG_FILE_PATH


typedef struct background_t
{
	ALLEGRO_BITMAP *table_bg;
	ALLEGRO_BITMAP *vertical_border;
	ALLEGRO_BITMAP *horizontal_border;
} background;


background *create_background();
void destroy_background(background *); 
void draw_background(background *);

// end of background definition

