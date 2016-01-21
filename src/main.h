#ifndef MAIN_H
#define MAIN_H

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>

#include "commons.h"
#include "message.h"
#include "background.h"
#include "ball.h"
#include "menu.h"
#include "score_board.h"

// default font to display
extern const char* const GAME_FONT_FILE_PATH;
#define FONT_FILE_PATH "./res/font/Junicode.ttf"
// background music file path
extern const char* const GAME_BACKGROUND_MUSIC_PATH;
#define BACKGROUND_MUSIC_FILE_PATH "./res/sound/background_music.ogg"

// button bitmaps
#define BUTTON_PRESSED_BITMAP "./res/pics/button_pressed.png"
#define BUTTON_RELEASED_BITMAP "./res/pics/button_released.png"

// TODO
// font size should be variable
// as window size change font size also changes
// font size
#define LOADING_FONT_SIZE 36
#define MOUSE_FONT_SIZE 12
#define BUTTON_FONT_SIZE 16

// TODO
// music gain should be variable
// music gain
#define MUSIC_GAIN 1.6
extern const double GAME_DEFAULT_MUSIC_GAIN;

// TODO
// menu mode should be define in menu.h
// modes
#define MENU_MODE 0
#define GAME_MODE 1

// hitting scaling constant
// negative because ball moves the opposite way the mouse pull
#define DELTA_SCALE -0.05

// mouse coordinate position
#define MOUSE_X_POSITION (WINDOW_WIDTH - 50)
#define MOUSE_Y_POSITION (TABLE_Y_POSITION - 36)
// reference line width
#define REFERENCE_LINE_WIDTH 2.6
// buttons
#define NEW_GAME_BUTTON_WIDTH 100
#define NEW_GAME_BUTTON_HEIGHT 30
#define NEW_GAME_BUTTON_X_POSITION 690
#define NEW_GAME_BUTTON_Y_POSITION 10
#define BACK_BUTTON_WIDTH 100
#define BACK_BUTTON_HEIGHT 30
#define BACK_BUTTON_X_POSITION 690
#define BACK_BUTTON_Y_POSITION 50
#define BUTTON_PRESS_TEXT_COLOR (al_map_rgb(160, 0, 0))
#define BUTTON_RELEASED_TEXT_COLOR (al_map_rgb(255, 0, 0))
#define NEW_GAME_TEXT "NEW GAME"
#define BACK_TEXT "BACK"
// redefine
#define FONT_FPATH FONT_FILE_PATH
#define BG_MUSIC_FPATH BACKGROUND_MUSIC_FILE_PATH
#define BP_BITMAP BUTTON_PRESSED_BITMAP
#define BR_BITMAP BUTTON_RELEASED_BITMAP
#define NGB_WIDTH NEW_GAME_BUTTON_WIDTH
#define NGB_HEIGHT NEW_GAME_BUTTON_HEIGHT
#define BB_WIDTH BACK_BUTTON_WIDTH
#define BB_HEIGHT BACK_BUTTON_HEIGHT
#define NGB_X_POS NEW_GAME_BUTTON_X_POSITION
#define NGB_Y_POS NEW_GAME_BUTTON_Y_POSITION
#define BB_X_POS BACK_BUTTON_X_POSITION
#define BB_Y_POS BACK_BUTTON_Y_POSITION
#define BP_COLOR BUTTON_PRESS_TEXT_COLOR
#define BR_COLOR BUTTON_RELEASED_TEXT_COLOR
#define MX MOUSE_X_POSITION
#define MY MOUSE_Y_POSITION
#define RL_WIDTH REFERENCE_LINE_WIDTH

typedef struct game_t {
  // allegro essential elements
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT event;
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

bool main_init_allegro_library();
game *main_init_game_object();
void main_release_game_object(game *obj);

// initialize all require library
int init();
// draw mouse coordinates
void draw_mouse_coordinates(const int, const int, const ALLEGRO_FONT *);
// draw the reference line for ball hitting
void draw_referencing_line(const int, const int, const int, const int);
// void *playing_music(ALLEGRO_THREAD *, void *);



#endif /* end of include guard: MAIN_H */

