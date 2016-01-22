#include "menu.h"

menu *create_menu()
{
	char c = 0;
	int n = 0;
	FILE *rule_file = NULL;

	menu *m = malloc(sizeof(menu));
	if(!m)
	{
		perror("fail to malloc for menu");
		return NULL;
	}
	// initialize values
	m->selection = PLAY_MENU_SELECTION;
	m->mode = DEFAULT_MENU_MODE;
	m->title_text_font = NULL;
	m->menu_font = NULL;
	m->menu_bg = NULL;
	for(n = 0 ; n < ST_NUM ; n ++)
		memset(m->scores[n], '\0', ST_MLENGTH);

	// load the rule text in
	rule_file = fopen(RT_FPATH, "r");
	if(!rule_file)
	{
		perror("fail to open rule file");
		free(m);
		return NULL;
	}
	n = 0;
	m->nrules = 1;
	m->rules = malloc(sizeof(char*) * m->nrules);
	m->rules[m->nrules - 1] = malloc(sizeof(char) * 1);
	while((c = fgetc(rule_file)) != EOF)
	{
		if(c == '\n')
		{
			m->rules[m->nrules-1] = realloc(m->rules[m->nrules-1],
					sizeof(char) * (n + 1));
			m->rules[m->nrules-1][n] = '\0';
			m->nrules ++;
			m->rules = realloc(m->rules, sizeof(char *) * m->nrules);
			n = 0;
			m->rules[m->nrules-1] = malloc(sizeof(char) * 1);
		}
		else
		{
			m->rules[m->nrules-1] = realloc(m->rules[m->nrules-1],
					sizeof(char) * (n + 1));
			m->rules[m->nrules-1][n] = c;
			n ++;
		}
	}
	m->rules[m->nrules-1] = realloc(m->rules[m->nrules-1],
			sizeof(char) * (n + 1));
	m->rules[m->nrules-1][n] = '\0';
	fclose(rule_file);

	regular_message("finished loading rule text");

	// read the score entries
	regular_message("loading score entries...");

	load_menu_score_entries(m);

	regular_message("finished loading score entries");

	// load menu font
	m->title_text_font = al_load_font(MENU_FONT_FPATH, 36, ALLEGRO_TTF_NO_KERNING);
	if(!m->title_text_font)
	{
		perror("fail to load menu font");
		free(m);
		return NULL;
	}
	m->menu_font = al_load_font(MENU_FONT_FPATH, 24, ALLEGRO_TTF_NO_KERNING);
	if(!m->menu_font)
	{
		perror("fail to load menu font");
		al_destroy_font(m->title_text_font);
		free(m);
		return NULL;
	}

	// load menu background bitmap
	m->menu_bg = al_load_bitmap(MENU_BG_FPATH);
	if(!m->menu_bg)
	{
		perror("fail to load menu background bitmap");
		al_destroy_font(m->title_text_font);
		al_destroy_font(m->menu_font);
		free(m);
		return NULL;
	}

	return m;
}

void destroy_menu(menu *m)
{
	int i;

	if(m != NULL)
	{
		if(m->rules != NULL)
		{
			for(i = 0 ; i < m->nrules ; i ++)
				free(m->rules[i]);
			free(m->rules);
		}

		if(m->title_text_font != NULL) al_destroy_font(m->title_text_font);
		if(m->menu_font != NULL) al_destroy_font(m->menu_font);
		if(m->menu_bg != NULL) al_destroy_bitmap(m->menu_bg);
		free(m);
	}
}

void load_menu_score_entries(menu *m)
{
	int n = 0, l = 0;
	int score_count = 0;
	const int num_str_size = 8;
	const int key_size = 64;
	char num[num_str_size];
	char key[key_size];
	int temp;
	int scores[ST_NUM];
	ALLEGRO_CONFIG *config = NULL;

	/* DEBUG */
#ifdef DEBUG
	if(m == NULL)
	{
		perror("menu null pointer");
		return;
	}
#endif

	config = al_load_config_file(SCORE_FILE_PATH);
	if(!config)
	{
		perror("fail to load score");
		return;
	}

	score_count = atoi(al_get_config_value(config, "entries", "num"));
	for(n = 0 ; n < score_count ; n ++)
	{
		memset(key, '\0', key_size);
		sprintf(key, "entry%d", n);

		scores[n] = atoi(al_get_config_value(config, key, "score"));
	}

	// sort
	for(n = 0 ; n < score_count ; n ++)
	{
		for(l = n + 1 ; l < score_count ; l ++)
		{
			if(scores[n] < scores[l])
			{
				temp = scores[n];
				scores[n] = scores[l];
				scores[l] = temp;
			}
		}
	}
	for(n = 0 ; n < score_count ; n ++)
	{
		memset(num, '\0', num_str_size);
		sprintf(num, "%2d.", n + 1);

		strcpy(m->scores[n], num);
		// strcat(m->scores[n], al_get_config_value(config, key, "name"));
		strcat(m->scores[n], "                  ");
		sprintf(key, "%2d", scores[n]);
		strcat(m->scores[n], key);
	}
	al_destroy_config(config);
}

void draw_menu(menu *m)
{
	// background
	const int bg_sx = 0;
	const int bg_sy = 0;
	const int bg_sw = al_get_bitmap_width(m->menu_bg);
	const int bg_sh = al_get_bitmap_height(m->menu_bg);
	const int bg_dx = 0;
	const int bg_dy = 0;
	const int bg_dw = WINDOW_WIDTH;
	const int bg_dh = WINDOW_HEIGHT;
	// text
	const int y_margin = TY_MARG;
	const int x = TX_POS;
	int i;
	int y = TY_POS;

	ALLEGRO_COLOR selected = COLOR_WHITE;
	ALLEGRO_COLOR nselected = COLOR_GREY;

	ALLEGRO_COLOR colors[] = {nselected, nselected, nselected, nselected};
	colors[m->selection] = selected;

	/* DEBUG */
#ifdef DEBUG
	if(m == NULL)
	{
		perror("menu null pointer");
		return;
	}
#endif

	// background
	al_draw_scaled_bitmap(m->menu_bg,
			bg_sx, bg_sy, bg_sw, bg_sh,
			bg_dx, bg_dy, bg_dw, bg_dh, 0);

	// title text
	al_draw_text(m->title_text_font, COLOR_WHITE,
			TTX_POS, TTY_POS, ALLEGRO_ALIGN_CENTER, "BILLIARD BALL");

	// selection text
	al_draw_text(m->menu_font, colors[PLAY_MENU_SELECTION],
			x, y, ALLEGRO_ALIGN_CENTER, "PLAY");
	y += y_margin;
	al_draw_text(m->menu_font, colors[RULE_MENU_SELECTION],
			x, y, ALLEGRO_ALIGN_CENTER, "RULE");
	y += y_margin;
	al_draw_text(m->menu_font, colors[SCORE_MENU_SELECTION],
			x, y, ALLEGRO_ALIGN_CENTER, "SCORE");
	y += y_margin;
	al_draw_text(m->menu_font, colors[EXIT_MENU_SELECTION],
			x, y, ALLEGRO_ALIGN_CENTER, "EXIT");

	if(m->mode == RULE_MODE)
	{
		for(i = 0 ; i < m->nrules ; i ++)
		{
			al_draw_text(m->menu_font, COLOR_WHITE,
					RT_SX, RT_SY + i * RT_MARG, ALLEGRO_ALIGN_LEFT, m->rules[i]);
		}
	}
	else if(m->mode == SCORE_MODE)
	{
		al_draw_text(m->menu_font, COLOR_WHITE,
				ST_SX, ST_SY, ALLEGRO_ALIGN_LEFT, "RANK          SCORE");
		for(i = 0 ; i < ST_NUM ; i ++)
		{
			al_draw_text(m->menu_font, COLOR_WHITE,
					ST_SX, ST_SY + (i + 1) * ST_Y_MARG,
					ALLEGRO_ALIGN_LEFT, m->scores[i]);
		}
	}
}

void set_menu_selection(menu *m, const int s)
{
	/* DEBUG */
#ifdef DEBUG
	if(m == NULL)
	{
		perror("menu null pointer");
		return;
	}
#endif

	m->selection = s;
}

int get_menu_mode(menu *m)
{
	/* DEBUG */
#ifdef DEBUG
	if(m == NULL)
	{
		perror("menu null pointer");
		return 0;
	}
#endif

	return m->mode;
}

void set_menu_mode(menu *m, const int mode)
{
	/* DEBUG */
#ifdef DEBUG
	if(m == NULL)
	{
		perror("menu null pointer");
		return;
	}
#endif

	m->mode = mode;

	// if the mode to change is SCORE_MODE
	// reload the score
	if(mode == SCORE_MODE)
		load_menu_score_entries(m);
}

int get_menu_selection(menu *m)
{
	/* DEBUG */
#ifdef DEBUG
	if(m == NULL)
	{
		perror("menu null pointer");
		return 0;
	}
#endif

	return m->selection;
}

