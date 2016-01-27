#include "button.h"

#define TEXT_SIZE_SCALE 0.6

button *button_init(rect r, const char *button_text,
                    const char *text_font_path,
                    ALLEGRO_COLOR text_color,
                    const char *button_press_bitmap_path,
                    const char *button_release_bitmap_path,
                    const char *button_hover_bitmap_path,
                    void (*callback_function) (void)) {
  const uint32_t text_size = r.height * TEXT_SIZE_SCALE;
  button *b = malloc(sizeof(button));
  // init with null;
  b->pressed_bitmap = NULL;
  b->release_bitmap = NULL;
  b->text_font = NULL;
  b->text = NULL;
  b->text_color = text_color;

  b->with_text = true;
  b->is_pressed = false;
  b->action_performed = false;

  b->sx = r.x;
  b->sy = r.y;
  b->width = r.width;
  b->height = r.height;

  b->callback = callback_function;

  b->text = (char *) malloc(sizeof(char) * (strlen(button_text) + 1));
  memset(b->text, '\0', sizeof(char) * (strlen(button_text) + 1));
  strcpy(b->text, button_text);

  // load bitmap and font
  b->pressed_bitmap = al_load_bitmap(button_press_bitmap_path);
  if (!b->pressed_bitmap) {
    return NULL;
  }

  b->release_bitmap = al_load_bitmap(button_release_bitmap_path);
  if (!b->release_bitmap) {
    al_destroy_bitmap(b->pressed_bitmap);
    return NULL;
  }

  b->hover_bitmap = al_load_bitmap(button_hover_bitmap_path);
  if (!b->hover_bitmap) {
    al_destroy_bitmap(b->pressed_bitmap);
    al_destroy_bitmap(b->release_bitmap);
    return NULL;
  }

  b->text_font = al_load_font(text_font_path,
                              text_size,
                              ALLEGRO_TTF_NO_KERNING);
  if (!b->text_font) {
    al_destroy_bitmap(b->pressed_bitmap);
    al_destroy_bitmap(b->release_bitmap);
    al_destroy_bitmap(b->hover_bitmap);
    return NULL;
  }

  return b;
}

button *button_init_without_text(rect r,
                                 const char *text_font_path,
                                 const char *button_press_bitmap_path,
                                 const char *button_release_bitmap_path,
                                 const char *button_hover_bitmap_path,
                                 void (*callback_function) (void)) {
  button *b = malloc(sizeof(button));
  // init with null;
  b->pressed_bitmap = NULL;
  b->release_bitmap = NULL;
  b->text_font = NULL;
  b->text = NULL;

  b->with_text = false;
  b->is_pressed = false;
  b->action_performed = false;

  b->sx = r.x;
  b->sy = r.y;
  b->width = r.width;
  b->height = r.height;

  b->callback = callback_function;

  // load bitmap and font
  b->pressed_bitmap = al_load_bitmap(button_press_bitmap_path);
  if (!b->pressed_bitmap) {
    return NULL;
  }

  b->release_bitmap = al_load_bitmap(button_release_bitmap_path);
  if (!b->release_bitmap) {
    al_destroy_bitmap(b->pressed_bitmap);
    return NULL;
  }

  b->hover_bitmap = al_load_bitmap(button_hover_bitmap_path);
  if (!b->hover_bitmap) {
    al_destroy_bitmap(b->pressed_bitmap);
    al_destroy_bitmap(b->release_bitmap);
    return NULL;
  }

  return b;
}

void button_destroy(button *b) {
  if (b != NULL) {
    if (b->pressed_bitmap) {
      al_destroy_bitmap(b->pressed_bitmap);
    }
    if (b->release_bitmap) {
      al_destroy_bitmap(b->release_bitmap);
    }
    if (b->hover_bitmap) {
      al_destroy_bitmap(b->hover_bitmap);
    }
    if (b->with_text && b->text) {
      free(b->text);
    }
  }
  free(b);
}

bool button_is_pressed(button* b) {
  return b->is_pressed;
}

void button_update(button *b, point mouse, bool mouse_pressed) {
  const double mx = mouse.x;
  const double my = mouse.y;

  if (mx >= b->sx && mx <= b->sx + b->width &&
      my >= b->sy && my <= b->sy + b->height) {
    b->is_pressed = mouse_pressed;
    b->is_hovered = true;
  } else {
    b->is_pressed = false;
    b->is_hovered = false;
  }
}

void button_resize(button *b, const double scale) {
  b->width *= scale;
  b->height *= scale;
  b->sx -= b->width * (scale - 1) / 2;
  b->sy -= b->height * (scale - 1) / 2;
}

void button_draw(button *b) {
  int bitmap_width, bitmap_height, text_width;
  double sx, sy;

  // draw the button bitmap
  if (b->is_pressed) {
    bitmap_width = al_get_bitmap_width(b->pressed_bitmap);
    bitmap_height = al_get_bitmap_height(b->pressed_bitmap);
    al_draw_scaled_bitmap(b->pressed_bitmap, 0, 0,
                          bitmap_width, bitmap_height,
                          b->sx, b->sy, b->width, b->height, 0);
  } else if (b->is_hovered) {
    bitmap_width = al_get_bitmap_width(b->hover_bitmap);
    bitmap_height = al_get_bitmap_height(b->hover_bitmap);
    al_draw_scaled_bitmap(b->hover_bitmap, 0, 0,
                          bitmap_width, bitmap_height,
                          b->sx, b->sy, b->width, b->height, 0);
  } else {
    bitmap_width = al_get_bitmap_width(b->release_bitmap);
    bitmap_height = al_get_bitmap_height(b->release_bitmap);
    al_draw_scaled_bitmap(b->release_bitmap, 0, 0,
                          bitmap_width, bitmap_height,
                          b->sx, b->sy, b->width, b->height, 0);
  }

  if (b->with_text) {
    text_width = al_get_text_width(b->text_font, b->text);
    sx = (b->width - text_width) / 2.0 + b->sx;
    sy = (b->height * (1 - TEXT_SIZE_SCALE)) / 2.0 + b->sy;
    al_draw_text(b->text_font, b->text_color, sx, sy, 0, b->text);
  }
}

