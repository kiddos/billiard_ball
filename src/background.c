#include "background.h"

background *create_background()
{
	background *bg = malloc(sizeof(background));
	bg->table_bg = NULL;
	bg->vertical_border = NULL;
	bg->horizontal_border = NULL;

	ALLEGRO_CONFIG *config = al_load_config_file(BG_CONFIG_FPATH);
	if(!config)
	{
		perror("fail to load 'background.config'");
		return NULL;
	}

	// load table background bitmap
	bg->table_bg = al_load_bitmap(
			al_get_config_value(config, "pictures", "table_background"));
	if(!bg->table_bg)
	{
		perror("fail to load table.jpg");
		free(bg);
		al_destroy_config(config);
		return NULL;
	}

	// load table border bitmap
	bg->vertical_border = al_load_bitmap(
			al_get_config_value(config, "pictures", "table_vertical_border"));
	if(!bg->vertical_border)
	{	
		perror("fail to load vertical border bitmap");
		al_destroy_bitmap(bg->table_bg);
		al_destroy_config(config);
		free(bg);
		return NULL;
	}
	bg->horizontal_border = al_load_bitmap(
			al_get_config_value(config, "pictures", "table_horizontal_border"));
	if(!bg->horizontal_border)
	{	
		perror("fail to load vertical border bitmap");
		al_destroy_bitmap(bg->table_bg);
		al_destroy_bitmap(bg->vertical_border);
		al_destroy_config(config);
		free(bg);
		return NULL;
	}

	al_destroy_config(config);
	return bg;
}

void destroy_background(background *bg)
{
	if(bg != NULL)
	{
		if(bg->table_bg != NULL) 
			al_destroy_bitmap(bg->table_bg);
		if(bg->vertical_border != NULL) 
			al_destroy_bitmap(bg->vertical_border);
		if(bg->horizontal_border != NULL) 
			al_destroy_bitmap(bg->horizontal_border);
		free(bg);
	}
}

static void draw_horizontal_border(const background *bg)
{
	const int hborder_sx1 = HB_SX1;
	const int hborder_ex1 = HB_EX1;
	const int hborder_sx2 = HB_SX2;
	const int hborder_ex2 = HB_EX2;
	const int hborder_sy1 = HB_SY - BORDER_WIDTH;
	const int hborder_ey1 = HB_SY;
	const int hborder_sy2 = HB_EY;
	const int hborder_ey2 = HB_EY + BORDER_WIDTH;
	const int width1 = hborder_ex1 - hborder_sx1;
	const int width2 = hborder_ex2 - hborder_sx2;
	const int height1 = hborder_ey1 - hborder_sy1;
	const int height2 = hborder_ey2 - hborder_sy2;
	const int sx = 0;
	const int sy = 0;
	const int sw = al_get_bitmap_width(bg->horizontal_border);
	const int sh = al_get_bitmap_height(bg->horizontal_border);
	const int dx1 = hborder_sx1;
	const int dx2 = hborder_sx2;
	const int dy1 = hborder_sy1;
	const int dy2 = hborder_sy2;

	/* DEBUG */
#ifdef DEBUG
	if(bg == NULL)
	{
		perror("background null pointer");
		return;
	}
#endif

	/*
	al_draw_filled_rectangle(hborder_sx1, hborder_sy1, 
			hborder_ex1, hborder_ey1, al_map_rgb(255, 255, 255));

	al_draw_filled_rectangle(hborder_sx1, hborder_sy2,
			hborder_ex1, hborder_ey2, al_map_rgb(255, 255, 255));

	al_draw_filled_rectangle(hborder_sx2, hborder_sy1,
			hborder_ex2, hborder_ey1, al_map_rgb(255, 255, 255));

	al_draw_filled_rectangle(hborder_sx2, hborder_sy2,
			hborder_ex2, hborder_ey2, al_map_rgb(255, 255, 255));
	*/
	al_draw_scaled_bitmap(bg->horizontal_border,
		sx, sy, sw, sh,
		dx1, dy1, width1, height1, 0);
	al_draw_scaled_bitmap(bg->horizontal_border,
		sx, sy, sw, sh,
		dx2, dy1, width2, height1, 0);
	al_draw_scaled_bitmap(bg->horizontal_border,
		sx, sy, sw, sh,
		dx1, dy2, width1, height2, 0);
	al_draw_scaled_bitmap(bg->horizontal_border,
		sx, sy, sw, sh,
		dx2, dy2, width2, height2, 0);
}

static void draw_vertical_border(const background *bg)
{
	const int vborder_sy = VB_SY;
	const int vborder_ey = VB_EY;
	const int vborder_sx1 = 0;
	const int vborder_ex1 = VB_SX;
	const int vborder_sx2 = VB_EX;
	const int vborder_ex2 = TABLE_WIDTH;
	const int width1 = vborder_ex1 - vborder_sx1;
	const int width2 = vborder_ex2 - vborder_sx2;
	const int height = vborder_ey - vborder_sy;	
	const int sx = 0;
	const int sy = 0;
	const int sw = al_get_bitmap_width(bg->vertical_border);
	const int sh = al_get_bitmap_height(bg->vertical_border);
	const int dx1 = vborder_sx1;
	const int dx2 = vborder_sx2;
	const int dy = vborder_sy;
	
	/* DEBUG */
#ifdef DEBUG
	if(bg == NULL)
	{
		perror("background null pointer");
		return;
	}
#endif

	/*
	al_draw_filled_rectangle(vborder_sx1, vborder_sy,
			vborder_ex1, vborder_ey, al_map_rgb(255, 255, 255));

	al_draw_filled_rectangle(vborder_sx2, vborder_sy,
			vborder_ex2, vborder_ey, al_map_rgb(255, 255, 255));
	*/
	al_draw_scaled_bitmap(bg->vertical_border,
			sx, sy, sw, sh,
			dx1, dy, width1, height, 0);
	al_draw_scaled_bitmap(bg->vertical_border,
			sx, sy, sw, sh,
			dx2, dy, width2, height, 0);
}

void draw_background(background *bg)
{
	const double sx = 0;
	const double sy = 0;
	const double sw = al_get_bitmap_width(bg->table_bg);
	const double sh = al_get_bitmap_height(bg->table_bg);
	const double dx = VB_SX;
	const double dy = HB_SY;
	const double dw = VB_EX - VB_SX;
	const double dh = HB_EY - HB_SY;

	/* DEBUG */
#ifdef DEBUG
	if(bg == NULL)
	{
		perror("background null pointer");
		return;
	}
#endif

	al_draw_scaled_bitmap(bg->table_bg, 
			sx, sy, sw, sh, 
			dx, dy, dw, dh, 0);
	
	/* DEBUG */
	//al_draw_filled_rectangle(0, 200, 800, 600, al_map_rgb(8, 138, 8));


	draw_horizontal_border(bg);
	draw_vertical_border(bg);
}

