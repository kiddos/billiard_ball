#ifndef BUTTON_H
#define BUTTON_H

#include "commons.h"

typedef enum {
  BUTTON_PRESS,
  BUTTON_RELEASE
} button_state, button_state_t;

typedef struct button_t {
  ALLEGRO_BITMAP *pressed_bitmap, *release_bitmap, *hover_bitmap;
  ALLEGRO_FONT *text_font;
  ALLEGRO_COLOR text_color;
  bool with_text;
  bool is_pressed;
  bool action_performed;
  double sx, sy, width, height;
  void (*callback) (void);
} button, button_t;

button *button_init(rect r, const char *button_text,
                    const char *text_font_path,
                    const char *button_press_bitmap_path,
                    const char *button_release_bitmap_path,
                    const char *button_hover_bitmap_path,
                    void (*callback_function) (void));
button *button_init_without_text(rect r,
                                 const char *text_font_path,
                                 const char *button_press_bitmap_path,
                                 const char *button_release_bitmap_path,
                                 const char *button_hover_bitmap_path,
                                 void (*callback_function) (void));
void button_destroy(button *b);
bool button_is_pressed(button* b);
void button_update(button *b, point mouse, uint64_t time_stamp);
void button_draw(button *b);

#endif /* end of include guard: BUTTON_H */

