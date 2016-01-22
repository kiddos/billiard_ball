#include "button.h"

button *button_init(rect r, const char *button_text,
                    const char *text_font_path,
                    const char *button_press_bitmap_path,
                    const char *button_release_bitmap_path,
                    void (*callback_function) (void)) {
  button *b = malloc(sizeof(button));
  // init with null;
  return b;
}

button *button_init_without_text(rect r,
                                 const char *text_font_path,
                                 const char *button_press_bitmap_path,
                                 const char *button_release_bitmap_path,
                                 void (*callback_function) (void)) {

}

void button_destroy(button *b) {

}

bool button_is_pressed(button* b) {

}

void button_update(button *b, uint64_t time_stamp) {

}

void button_draw(button *b) {

}

