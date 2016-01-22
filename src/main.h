#ifndef MAIN_H
#define MAIN_H

#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>

#include "commons.h"
#include "message.h"
#include "game.h"

// default font to display
#define FONT_FILE_PATH "./res/font/Junicode.ttf"
// background music file path
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

/* init allegro library */
bool main_init_allegro_library();
/* close allegro library */
void main_close_allegro_library();
void draw_mouse_coordinates(const int mx,
                            const int my,
                            const ALLEGRO_FONT *font);
void draw_referencing_line(const int sx,
                           const int sy,
                           const int cx,
                           const int cy);

#endif /* end of include guard: MAIN_H */

