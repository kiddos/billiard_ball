#ifndef GAME_H
#define GAME_H

#include <allegro5/allegro_native_dialog.h>

#include "commons.h"
#include "message.h"
#include "background.h"
#include "ball.h"
#include "menu.h"
#include "score_board.h"

/* constants */
extern const char* const GAME_BACKGROUND_MUSIC_PATH;
extern const char* const GAME_FONT_FILE_PATH;
extern const double GAME_DEFAULT_MUSIC_GAIN;
extern const uint32_t GAME_DEFAULT_MOUSE_FONT_SIZE;
extern const uint32_t GAME_DEFAULT_LOADING_FONT_SIZE;

typedef enum {
  MENU_MODE,
  GAME_MODE
} mode;

typedef struct game_t {
  // allegro essential elements
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
  ALLEGRO_FONT *loading_font;
  ALLEGRO_FONT *mouse_font;
  ALLEGRO_EVENT_QUEUE *event_queue;
  ALLEGRO_SAMPLE *bg_music;
  ALLEGRO_SAMPLE_INSTANCE *bg_music_instance;
  // modules
  background *bg;
  billiard_ball *balls;
  menu *m;
  score_board *board;
  // current window width/height
  uint32_t window_width, window_height;
  // font size
  uint32_t loading_font_size, mouse_font_size;
  // music gain
  double music_gain;
  // game running
  bool running;
  // redendering
  bool redraw;
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

game *game_init_object();
void game_release_object(game *obj);
void game_prepare(game *g);
void game_main_loop(game *g);

#endif /* end of include guard: GAME_H */

