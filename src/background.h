#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "commons.h"

typedef struct background_t {
  ALLEGRO_BITMAP *table_bg;
  ALLEGRO_BITMAP *vertical_border;
  ALLEGRO_BITMAP *horizontal_border;
  ALLEGRO_SAMPLE *background_music;
  uint32_t bg_width, bg_height;
  uint64_t last_update_time;
} background, background_t;

background *background_init(size window_size);
void background_destroy(background *bg);
void background_update(background *bg, const uint64_t time_stamp);
void background_resize(background *bg, const size new_size);
void background_draw(background *bg);

#endif /* end of include guard: BACKGROUND_H */

