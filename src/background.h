#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "commons.h"

extern const char* const BACKGROUND_CONFIG_FILE_PATH;

typedef struct background_t
{
	ALLEGRO_BITMAP *table_bg;
	ALLEGRO_BITMAP *vertical_border;
	ALLEGRO_BITMAP *horizontal_border;
} background;


background *create_background();
void destroy_background(background *);
void draw_background(background *);

#endif /* end of include guard: BACKGROUND_H */

