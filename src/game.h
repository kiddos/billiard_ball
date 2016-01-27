#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro_native_dialog.h>

#include "commons.h"
#include "message.h"
#include "background.h"
#include "ball.h"
#include "menu.h"
#include "score_board.h"
#include "button.h"

typedef enum {
  MENU_MODE,
  GAME_MODE
} mode;

typedef struct game_t {
  // allegro essential elements
  struct {
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
  } core;
  ALLEGRO_FONT *loading_font, *mouse_font;
  ALLEGRO_SAMPLE *bg_music;
  ALLEGRO_SAMPLE_INSTANCE *bg_music_instance;
  // modules
  struct {
    background *bg;
    billiard_ball *balls;
    menu *m;
    score_board *board;
    button *new_game_button, *back_return_button;
  } module;
  // current window width/height
  uint32_t window_width, window_height;
  // font size
  uint32_t loading_font_size, mouse_font_size;
  // music gain
  double music_gain;
  // game running and drawing
  bool running, redraw;
  // mouse coordinates
  int mx, my;
  // hitting the ball
  bool is_ready_to_hit;
  bool is_ball_hitted;
  bool should_change_turn;
  int *ball_original_status;
  int *ball_new_status;
  int highest_score, who_score_highest;
  int start_x, start_y, end_x, end_y;
  double dx, dy;
} game_t, game;

game *game_init();
void game_destroy(game *g);
void game_prepare(game *g);
void game_main_loop(game *g);

#endif /* end of include guard: GAME_H */

