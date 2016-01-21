#pragma once

#include "commons.h"


#define SCORE_BOARD_FONT_FILE_PATH "./res/font/Junicode.ttf"
#define SCORE_BOARD_CONFIG_FILE_PATH "./config/score_board.config"
#define SCORE_BOARD_BITMAP_FILE_PATH "./res/pics/score_board.png"
#define SCORE_BOARD_WINNER_BOARD_BITMAP_FILE_PATH "./res/pics/winner_board.png"
#define SCORE_BOARD_PLAYER_1_TURN 0
#define SCORE_BOARD_PLAYER_2_TURN 1 
#define SCORE_BOARD_START_X_POSITION 0
#define SCORE_BOARD_START_Y_POSITION 0
#define SCORE_BOARD_X_MARGIN 10
#define SCORE_BOARD_Y_MARGIN 10
#define SCORE_BOARD_WIDTH 130
#define SCORE_BOARD_HEIGHT 160
#define SCORE_BOARD_WINNER_BOARD_WIDTH 360
#define SCORE_BOARD_WINNER_BOARD_HEIGHT 350
#define SCORE_BOARD_WINNER_BOARD_TEXT_MARGIN 126
#define SCORE_BOARD_WINNER_BOARD_TEXT_X_PADDING 50
#define SCORE_BOARD_WINNER_BOARD_TEXT_Y_PADDING 20
#define SCORE_BOARD_DRAW 2
#define SCORE_BOARD_MAX_GAME_COUNT 3
#define SCORE_BOARD_MAX_ENTRY_NUMBER 6
// redefine
#define SB_FONT_FPATH SCORE_BOARD_FONT_FILE_PATH
#define SB_CONFIG_FPATH SCORE_BOARD_CONFIG_FILE_PATH
#define SB_BITMAP_FPATH SCORE_BOARD_BITMAP_FILE_PATH
#define SB_WB_BITMAP_FPATH SCORE_BOARD_WINNER_BOARD_BITMAP_FILE_PATH
#define SB_P1_TURN SCORE_BOARD_PLAYER_1_TURN
#define SB_P2_TURN SCORE_BOARD_PLAYER_2_TURN
#define SB_X_POS SCORE_BOARD_START_X_POSITION
#define SB_Y_POS SCORE_BOARD_START_Y_POSITION
#define SB_X_MARG SCORE_BOARD_X_MARGIN
#define SB_Y_MARG SCORE_BOARD_Y_MARGIN
#define SB_WIDTH SCORE_BOARD_WIDTH
#define SB_HEIGHT SCORE_BOARD_HEIGHT
#define SB_WB_WIDTH SCORE_BOARD_WINNER_BOARD_WIDTH
#define SB_WB_HEIGHT SCORE_BOARD_WINNER_BOARD_HEIGHT
#define SB_WB_MARG SCORE_BOARD_WINNER_BOARD_TEXT_MARGIN
#define SB_WB_X_PADDING SCORE_BOARD_WINNER_BOARD_TEXT_X_PADDING
#define SB_WB_Y_PADDING SCORE_BOARD_WINNER_BOARD_TEXT_Y_PADDING 
#define SB_DRAW SCORE_BOARD_DRAW
#define MAX_GAME_NUM SCORE_BOARD_MAX_GAME_COUNT
#define MAX_ENTRY_NUM SCORE_BOARD_MAX_ENTRY_NUMBER


typedef struct player_t
{
	int score;
	char name[256];
	char real_name[256];
} player;

typedef struct score_board_t
{
	int turn;
	int nround;
	int scores[3][2];
	player p1, p2;
	ALLEGRO_FONT *number_font;
	ALLEGRO_FONT *name_text_font;
	ALLEGRO_FONT *winner_board_text_font;
	ALLEGRO_BITMAP *score_board_bitmap;
	ALLEGRO_BITMAP *winner_board_bitmap;
} score_board;


score_board *create_score_board();
void destroy_score_board(score_board *);
void reset_score_board(score_board *);
void add_score_to_score_board(score_board *, const int);
void change_turn(score_board *);
int get_turn(score_board *);
int get_player_score(score_board *, const int);
int get_who_win(score_board *);
int get_round(score_board *);
int get_highest_player_score_of_3(score_board *);
int get_who_score_higher_of_3(score_board *);
void record_score(score_board *);
void save_score_to_config(score_board *, const int, const int);
void draw_score_board(score_board *);

// end of score_board definition

