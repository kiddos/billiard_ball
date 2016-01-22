#ifndef MENU_H
#define MENU_H

#include "commons.h"

// PATH
#define MENU_FONT_FILE_PATH "./res/font/Junicode.ttf"
#define MENU_BACKGROUND_FILE_PATH "./res/pics/menu_bg.png"
#define RULE_TEXT_FILE_PATH "./res/text/rules"
#define SCORE_FILE_PATH "./config/score_board.config"
// selection choices
#define PLAY_MENU_SELECTION 0
#define RULE_MENU_SELECTION 1
#define SCORE_MENU_SELECTION 2
#define EXIT_MENU_SELECTION 3
#define DEFAULT_MENU_MODE 0
#define RULE_MODE 1
#define SCORE_MODE 2
// text positioning
#define TITLE_TEXT_X_POSITION (WINDOW_WIDTH / 2)
#define TITLE_TEXT_Y_POSITION 60
#define TEXT_CENTER_X_POSITION (WINDOW_WIDTH / 2)
#define TEXT_START_Y_POSITION 420
#define TEXT_Y_MARGIN 46
// rule text max length
#define RULE_TEXT_START_X_POSITION 100
#define RULE_TEXT_START_Y_POSITION 120
#define RULE_TEXT_MARGIN 46
// score text
#define SCORE_TEXT_MAX_LENGTH 128
#define SCORE_TEXT_NUMBER 6
#define SCORE_TEXT_START_X_POSITION 100
#define SCORE_TEXT_START_Y_POSITION 120
#define SCORE_TEXT_Y_MARGIN 30

// redefine
#define MENU_FONT_FPATH MENU_FONT_FILE_PATH
#define MENU_BG_FPATH MENU_BACKGROUND_FILE_PATH
#define RT_FPATH RULE_TEXT_FILE_PATH
#define TTX_POS TITLE_TEXT_X_POSITION
#define TTY_POS TITLE_TEXT_Y_POSITION
#define TX_POS TEXT_CENTER_X_POSITION
#define TY_POS TEXT_START_Y_POSITION
#define TY_MARG TEXT_Y_MARGIN
#define RT_SX RULE_TEXT_START_X_POSITION
#define RT_SY RULE_TEXT_START_Y_POSITION
#define RT_MARG RULE_TEXT_MARGIN
#define ST_MLENGTH SCORE_TEXT_MAX_LENGTH
#define ST_NUM SCORE_TEXT_NUMBER
#define ST_SX SCORE_TEXT_START_X_POSITION
#define ST_SY SCORE_TEXT_START_Y_POSITION
#define ST_Y_MARG SCORE_TEXT_Y_MARGIN

typedef struct menu_t
{
	int selection;
	int mode;
	int nrules;
	char **rules;
	char scores[ST_NUM][ST_MLENGTH];
	ALLEGRO_FONT *title_text_font;
	ALLEGRO_FONT *menu_font;
	ALLEGRO_BITMAP *menu_bg;
} menu;

menu *create_menu();
void destroy_menu(menu *);
void load_menu_score_entries(menu *);
void draw_menu(menu *);
void set_menu_selection(menu *, const int);
int get_menu_mode(menu *);
void set_menu_mode(menu *, const int);
int get_menu_selection(menu *);

#endif /* end of include guard: MENU_H */

