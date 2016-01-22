#include "score_board.h"

score_board *create_score_board()
{
	int i;
	score_board *sboard = malloc(sizeof(score_board));
	ALLEGRO_CONFIG *config = al_load_config_file(SB_CONFIG_FPATH);

	if(!sboard)
	{
		perror("fail to malloc for score_board");
		return NULL;
	}

	if(!config)
	{
		perror("fail to load score board.config");
		free(sboard);
		return NULL;
	}

	sboard->turn = SB_P1_TURN;
	sboard->nround = 0;
	for(i = 0 ; i < MAX_GAME_NUM ; i ++)
	{
		sboard->scores[i][0] = 0;
		sboard->scores[i][1] = 0;
	}

	sboard->p1.score = 0;
	strcpy(sboard->p1.name, 
			al_get_config_value(config, "player_names", "player1"));
	sboard->p2.score = 0;	
	strcpy(sboard->p2.name, 
			al_get_config_value(config, "player_names", "player2"));

	al_destroy_config(config);
	
	// load the font
	sboard->number_font = al_load_font(SB_FONT_FPATH, 36, ALLEGRO_TTF_NO_KERNING);
	if(!sboard->number_font)
	{
		perror("fail to load score board font");
		free(sboard);
		return NULL;
	}
	sboard->name_text_font = al_load_font(SB_FONT_FPATH, 16, ALLEGRO_TTF_NO_KERNING);
	if(!sboard->name_text_font)
	{
		perror("fail to load score board font");
		al_destroy_font(sboard->number_font);
		free(sboard);
		return NULL;
	}
	sboard->winner_board_text_font = 
			al_load_font(SB_FONT_FPATH, 20, ALLEGRO_TTF_NO_KERNING);
	if(!sboard->winner_board_text_font)
	{	
		perror("fail to load score board font");
		al_destroy_font(sboard->number_font);
		al_destroy_font(sboard->name_text_font);
		free(sboard);
		return NULL;
	}

	// load the bitmap
	sboard->score_board_bitmap = al_load_bitmap(SB_BITMAP_FPATH);
	if(!sboard->score_board_bitmap)
	{
		perror("fail to load score board bitmap");
		al_destroy_font(sboard->number_font);
		al_destroy_font(sboard->name_text_font);
		al_destroy_font(sboard->winner_board_text_font);
		free(sboard);
		return NULL;
	}
	sboard->winner_board_bitmap = al_load_bitmap(SB_WB_BITMAP_FPATH);
	if(!sboard->winner_board_bitmap)
	{	
		perror("fail to load score board bitmap");
		al_destroy_font(sboard->number_font);
		al_destroy_font(sboard->name_text_font);
		al_destroy_font(sboard->winner_board_text_font);
		al_destroy_bitmap(sboard->score_board_bitmap);
		free(sboard);
		return NULL;
	}

	return sboard;
}

void destroy_score_board(score_board *board)
{
	if(board != NULL)
	{
		if(board->number_font != NULL)
			al_destroy_font(board->number_font);
		if(board->name_text_font != NULL)
			al_destroy_font(board->name_text_font);
		if(board->winner_board_text_font != NULL)
			al_destroy_font(board->winner_board_text_font);
		if(board->score_board_bitmap != NULL) 
			al_destroy_bitmap(board->score_board_bitmap);
		if(board->winner_board_bitmap != NULL)
			al_destroy_bitmap(board->winner_board_bitmap);
		free(board);
	}
}

void reset_score_board(score_board *board)
{	
	int i;

	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return;
	}
#endif

	board->turn = SB_P1_TURN;
	board->nround = 0;
	for(i = 0 ; i < 3 ; i ++)
	{
		board->scores[i][0] = 0;
		board->scores[i][1] = 0;
	}

	board->p1.score = 0;
	board->p2.score = 0;
}

void add_score_to_score_board(score_board *board, const int score)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return;
	}
#endif

	if(board->turn == SB_P1_TURN)
		board->p1.score += score;
	else if(board->turn == SB_P2_TURN)
		board->p2.score += score;
}

void change_turn(score_board *board)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return;
	}
#endif

	if(board->turn == SB_P1_TURN)
		board->turn = SB_P2_TURN;
	else board->turn = SB_P1_TURN;
}

int get_turn(score_board *board)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return 0;
	}
#endif

	return board->turn;
}

int get_player_score(score_board *board, const int player)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return 0;
	}
#endif

	if(player == SB_P1_TURN)
		return board->p1.score;
	else if(player == SB_P2_TURN)
		return board->p2.score;
	else
	{
		perror("invalid player");
		return -999;
	}
}

int get_who_win(score_board *board)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return SB_DRAW;
	}
#endif

	if(board->p1.score > board->p2.score)
		return SB_P1_TURN;
	else if(board->p2.score > board->p1.score)
		return SB_P2_TURN;
	else return SB_DRAW;
}

int get_round(score_board *board)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return 0;
	}
#endif
	
	return board->nround;
}

int get_highest_player_score_of_3(score_board *board)
{
	int i;
	int num;
	int max = board->scores[0][0] >= board->scores[0][1] ?
			board->scores[0][0] : board->scores[0][1];

	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return 0;
	}
#endif
	
	for(i = 1 ; i < MAX_GAME_NUM ; i ++)
	{
		num = board->scores[i][0] >= board->scores[i][1] ? 
				board->scores[i][0] : board->scores[i][1];
		if(num > max) max = num;
	}
	return max;
}

int get_who_score_higher_of_3(score_board *board)
{
	// compute who ever got the highest first
	int i;
	int highest_score = get_highest_player_score_of_3(board);

	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return 0;
	}
#endif

	for(i = 0 ; i < MAX_GAME_NUM ; i ++)
	{
		if(board->scores[i][0] == highest_score &&
			board->scores[i][1] == highest_score)
			return SB_DRAW;
		else if(board->scores[i][0] == highest_score)
			return SB_P1_TURN;
		else if(board->scores[i][1] == highest_score)
			return SB_P2_TURN;
	}
	return SB_DRAW;
}

void record_score(score_board *board)
{
	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return;
	}
#endif

#ifdef DEBUG
	printf("recording score...\n");
#endif

	if(board->nround >= MAX_GAME_NUM)
	{
		perror("3 matches are all done");
		return;
	}

	board->scores[board->nround][0] = board->p1.score;
	board->scores[board->nround][1] = board->p2.score;
	board->p1.score = 0;
	board->p2.score = 0;

	board->nround ++; 
}

static int get_lowest_score_entry(score_board *board)
{
	int i;
	int entry = 0, entry_num;
	int scores[MAX_ENTRY_NUM] = {0};
	const int key_size = 64;
	char key[key_size];
	ALLEGRO_CONFIG *config = NULL;

	config = al_load_config_file(SB_CONFIG_FPATH);
	if(!config)
	{
		perror("fail to load score board config file");
		return 0;
	}

	entry_num = atoi(al_get_config_value(config, "entries", "num"));
	for(i = 0 ; i < entry_num ; i ++)
	{
		memset(key, '\0', key_size);
		sprintf(key, "entry%d", i);
		scores[i] = atoi(al_get_config_value(config, key, "score"));
	}
	al_destroy_config(config);
	
	if(entry_num == 0)
		return entry_num;

	entry = 0;
	for(i = 1 ; i < entry_num ; i ++)
		if(scores[entry] > scores[i]) entry = i;

	return entry;
}

void save_score_to_config(score_board *board, const int who, const int score)
{
	int entry_num;
	const int num_str_size = 8;
	const int section_str_size = 256;
	const int max_entry_num = MAX_ENTRY_NUM;
	char num_str[num_str_size];
	char section_name[section_str_size];
	ALLEGRO_CONFIG *config = al_load_config_file(SB_CONFIG_FPATH);

	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return;
	}
#endif

	if(!config)
	{
		perror("fail to load score board configuration file");
		return;
	}

#ifdef DEBUG
	printf("saving score to config file...\n");
#endif

	// get the number of entries

	entry_num = atoi(al_get_config_value(config, "entries", "num"));
	
	// if the entry num is greater than the max entry
	// replace the entry with the lowest score
	// else increment entry num by 1
	if(entry_num >= max_entry_num)
		entry_num = get_lowest_score_entry(board);
	else
	{
		entry_num ++;
		// write the new entry number
		memset(num_str, '\0', num_str_size);
		sprintf(num_str, "%d", entry_num);
		al_set_config_value(config, "entries", "num", num_str);
	}
	// compute section number
	memset(section_name, '\0', section_str_size);
	sprintf(section_name, "entry%d", entry_num);
	// add config section if it does not exist
	al_add_config_section(config, section_name);
	
	// write the highest score of all 3 games into config file
	if(who == SB_P1_TURN || who == SB_DRAW)
	{
		al_set_config_value(config, section_name, "name", board->p1.name);
		memset(num_str, '\0', num_str_size);
		sprintf(num_str, "%d", score);
		al_set_config_value(config, section_name, "score", num_str);
	}
	if(who == SB_P2_TURN || who == SB_DRAW)
	{
		al_set_config_value(config, section_name, "name", board->p2.name);
		memset(num_str, '\0', num_str_size);
		sprintf(num_str, "%d", score);
		al_set_config_value(config, section_name, "score", num_str);
	}

	// save the config file
	if(!al_save_config_file(SB_CONFIG_FPATH, config))
	{
		perror("fail to save score to score config file");
		al_destroy_config(config);
		return;
	}

#ifdef DEBUG
	printf("data saved to config file\n");
#endif

	al_destroy_config(config);
}

void draw_score_board(score_board *board)
{
	const int text_margin = 18;
	const int width = SB_WIDTH;
	const int height = SB_HEIGHT;
	const int x_margin = SB_X_MARG;
	const int y_margin = SB_Y_MARG;
	const int sx = 0;
	const int sy = 0;
	const ALLEGRO_COLOR selected = color_white();
	const ALLEGRO_COLOR nselected = color_gray();

	int i;
	int sw = al_get_bitmap_width(board->score_board_bitmap);
	int sh = al_get_bitmap_height(board->score_board_bitmap);
	int x1 = SB_X_POS + x_margin;
	int y1 = SB_Y_POS + y_margin;
	int x2 = x1 + width;
	int y2 = y1 + height;
	int cx = (x1 + x2) / 2;
	int cy = (y1 + y2) / 2 - text_margin;

	/* DEBUG */
#ifdef DEBUG
	if(board == NULL)
	{
		perror("score_board null pointer");
		return;
	}
#endif

	// player 1
	/* DEBUG
	al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(x1 + x_margin, y1 + y_margin,
			x2 - x_margin, y2 - y_margin, al_map_rgb(255, 255, 255));
	*/
	// player 1
	al_draw_scaled_bitmap(board->score_board_bitmap,
			sx, sy, sw, sh, x1, y1, width, height, 0);
	al_draw_textf(board->number_font, color_black(),
			cx, cy, ALLEGRO_ALIGN_CENTER, "%03d", board->p1.score);
	al_draw_text(board->name_text_font,
			board->turn == SB_P1_TURN ? selected : nselected,
			cx, y1 + height + y_margin, ALLEGRO_ALIGN_CENTER, board->p1.name);
	x1 = x2 + x_margin;
	x2 = x1 + width;
	cx = (x1 + x2) / 2;
	
	// player 2
	/* DEBUG
	al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(255, 0, 0));
	al_draw_filled_rectangle(x1 + x_margin, y1 + y_margin,
			x2 - x_margin, y2 - y_margin, al_map_rgb(255, 255, 255));
	*/
	// player 2
	al_draw_scaled_bitmap(board->score_board_bitmap, 
			sx, sy, sw, sh, x1, y1, width, height, 0);
	al_draw_textf(board->number_font, color_black(), 
			cx, cy, ALLEGRO_ALIGN_CENTER, "%03d", board->p2.score);
	al_draw_text(board->name_text_font,
			board->turn == SB_P2_TURN ? selected : nselected,
			cx, y1 + height + y_margin, ALLEGRO_ALIGN_CENTER, board->p2.name);
	
	// winner board
	x1 = x2 + x_margin;
	sw = al_get_bitmap_width(board->winner_board_bitmap);
	sh = al_get_bitmap_width(board->winner_board_bitmap);
	al_draw_scaled_bitmap(board->winner_board_bitmap,
			sx, sy, sw, sh, x1, y1, SB_WB_WIDTH, SB_WB_HEIGHT, 0);
	
	for(i = 0 ; i < MAX_GAME_NUM ; i ++)
	{
		al_draw_text(board->winner_board_text_font, color_black(),
			x1 + SB_WB_X_PADDING + i * SB_WB_MARG, 
			cy - SB_WB_Y_PADDING, ALLEGRO_ALIGN_CENTER, board->p1.name);
		al_draw_textf(board->winner_board_text_font, color_black(),
			x1 + SB_WB_X_PADDING + i * SB_WB_MARG, 
			cy, ALLEGRO_ALIGN_CENTER, 
			"%03d", board->scores[i][0]);
		al_draw_text(board->winner_board_text_font, color_black(),
			x1 + SB_WB_X_PADDING + i * SB_WB_MARG, 
			cy + SB_WB_Y_PADDING, ALLEGRO_ALIGN_CENTER, board->p2.name);
		al_draw_textf(board->winner_board_text_font, color_black(),
			x1 + SB_WB_X_PADDING + i * SB_WB_MARG, 
			cy + 2 * SB_WB_Y_PADDING, ALLEGRO_ALIGN_CENTER, 
			"%03d", board->scores[i][1]);
	}
}

