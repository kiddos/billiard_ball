#include "ball.h"

billiard_ball *create_billiard_balls()
{
	int i;
	const int key_size = 256;
	char key[key_size];
	billiard_ball *balls = malloc(sizeof(billiard_ball) * BALL_COUNT);
	ALLEGRO_CONFIG *config = al_load_config_file(BCONFIG_FPATH);
	
	if(!balls)
	{
		perror("fail to malloc for billiard balls");
		return NULL;
	}

	if(!config)
	{
		perror("fail to load billiard_ball.config");
		free(balls);
		return NULL;
	}

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{
		balls[i].number = i;
		balls[i].is_on_table = 1;
		balls[i].dx = 0;
		balls[i].dy = 0;

		// prepare the key for reading config file
		memset(key, '\0', key_size);
		sprintf(key, "ball%d", i);

		// read the score for each ball
		balls[i].score = atoi(al_get_config_value(config, key, "score"));

		// read the bitmap
		balls[i].bitmap = al_load_bitmap(
				al_get_config_value(config, "pictures", key));
		if(!balls[i].bitmap)
		{
			perror("fail to load billiard ball bitmap");
			free(balls);
			return NULL;
		}
		// load in bitmap width and height
		balls[i].bitmap_sw = al_get_bitmap_width(balls[i].bitmap);
		balls[i].bitmap_sh = al_get_bitmap_height(balls[i].bitmap);

		// read the initial position
		balls[i].cx = atof(al_get_config_value(config, key, "start_x"));
		balls[i].cy = atof(al_get_config_value(config, key, "start_y"));

		// read the collision sound file
		balls[i].collision_sound = al_load_sample(
				al_get_config_value(config, key, "collision_sound"));
		if(!balls[i].collision_sound)
		{
			perror("fail to load billiard ball collision sound");
			al_destroy_bitmap(balls[i].bitmap);
			free(balls);
			return NULL;
		}
	}

	/* DEBUG
	// ball colors
	balls[0].color = al_map_rgb(255, 255, 255);
	balls[1].color = al_map_rgb(255, 191, 0);
	balls[2].color = al_map_rgb(0, 0, 255);
	balls[3].color = al_map_rgb(255, 0, 0);
	balls[4].color = al_map_rgb(191, 0, 255);
	balls[5].color = al_map_rgb(255, 128, 0);
	balls[6].color = al_map_rgb(1, 225, 1);
	balls[7].color = al_map_rgb(138, 8, 8);
	balls[8].color = al_map_rgb(0, 0, 0);
	*/

	/* DEBUG
	balls[0].cx = 600;
	balls[0].cy = TABLE_Y_POSITION + TABLE_HEIGHT / 2;

	balls[1].cx = 260;
	balls[1].cy = TABLE_Y_POSITION + TABLE_HEIGHT / 2;

	balls[2].cx = balls[1].cx - 2 * BILLIARD_BALL_RADIUS * cos(PI / 6);
	balls[2].cy = balls[1].cy - 2 * BILLIARD_BALL_RADIUS * sin(PI / 6);

	balls[3].cx = balls[1].cx - 2 * BILLIARD_BALL_RADIUS * cos(PI / 6);
	balls[3].cy = balls[1].cy + 2 * BILLIARD_BALL_RADIUS * sin(PI / 6);
	
	for(i = 4 ; i <= 6 ; i ++)
		balls[i].cx = balls[1].cx - 4 * BILLIARD_BALL_RADIUS * cos(PI / 6);
	balls[4].cy = balls[1].cy - 2 * BILLIARD_BALL_RADIUS;
	balls[5].cy = balls[1].cy;
	balls[6].cy = balls[1].cy + 2 * BILLIARD_BALL_RADIUS;

	for(i = 7 ; i <= 8 ; i ++)
	{
		balls[i].cx = balls[i - 5].cx - 4 * BILLIARD_BALL_RADIUS * cos(PI / 6);
		balls[i].cy = balls[i - 5].cy;
	}
	
	*/
	// destroy the config file handler
	al_destroy_config(config);
	return balls;
}

void destroy_billiard_balls(billiard_ball *balls)
{
	int i;
	if(balls != NULL)
	{
		for(i = 0 ; i < BALL_COUNT ; i ++)
		{
			if(balls[i].bitmap != NULL)
				al_destroy_bitmap(balls[i].bitmap);
			if(balls[i].collision_sound != NULL)
				al_destroy_sample(balls[i].collision_sound);
		}
		free(balls);
	}
}

void reset_billiard_balls(billiard_ball *balls)
{
	int i;
	const int key_size = 256;
	char key[key_size];
	ALLEGRO_CONFIG *config = al_load_config_file(BCONFIG_FPATH);
	
	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return;
	}
#endif	
	
	if(!config)
	{
		perror("fail to load billiard_ball.config");
		return;
	}

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{
		balls[i].number = i;
		balls[i].is_on_table = 1;
		balls[i].dx = 0;
		balls[i].dy = 0;

		// prepare the key for reading config file
		memset(key, '\0', key_size);
		sprintf(key, "ball%d", i);

		// read the initial position
		balls[i].cx = atof(al_get_config_value(config, key, "start_x"));
		balls[i].cy = atof(al_get_config_value(config, key, "start_y"));	
	}
	al_destroy_config(config);
}

int is_ball_collided(billiard_ball ball1, billiard_ball ball2)
{
	double delta_x, delta_y, distance;
	// if the any of the ball is not on table,
	// theres no collision either
	if(!ball1.is_on_table || !ball2.is_on_table)
		return 0;

	delta_x = ball1.cx + ball1.dx - ball2.cx - ball2.dx;
	delta_y = ball1.cy + ball1.dy - ball2.cy - ball2.dy;
	distance = sqrt(delta_x * delta_x + delta_y * delta_y);
	if(distance <= 2.0 * BALL_RADIUS)
		return 1;
	else return 0;
}

int is_ball_in_bag(billiard_ball ball)
{
	const int tborder_x1 = BORDER_WIDTH;
	const int tborder_x2 = TABLE_WIDTH - BORDER_WIDTH;
	const int tborder_y1 = Y_POS + BORDER_WIDTH;
	const int tborder_y2 = Y_POS + TABLE_HEIGHT - BORDER_WIDTH;
	const int new_x = ball.cx + ball.dx;
	const int new_y = ball.cy + ball.dy;

	if(new_x <= tborder_x1 || new_x >= tborder_x2 ||
		new_y <= tborder_y1 || new_y >= tborder_y2)
		return 1;
	return 0;
}

int is_mouse_on_ball(billiard_ball ball, const int mx, const int my)
{
	double distance, dx, dy;
	
	dx = mx - ball.cx;
	dy = my - ball.cy;
	distance = sqrt(dx * dx + dy * dy);

	if(distance <= BALL_RADIUS)
		return 1;
	else return 0;
}

int is_all_ball_stopped(billiard_ball *balls)
{
	int i;
	
	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return 0;
	}
#endif

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{
		if(balls[i].dx != 0 || balls[i].dy != 0)
			return 0;
	}
	return 1;
}

int is_all_ball_in_bag(billiard_ball *balls)
{
	int i;

	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return;
	}
#endif

	for(i = 1 ; i < BALL_COUNT ; i ++)
	{
		if(balls[i].is_on_table)
			return 0;
	}
	return 1;
}

int is_ball_on_table_horizontal_border(billiard_ball ball)
{
	const int hborder_sx1 = HB_SX1;
	const int hborder_ex1 = HB_EX1;
	const int hborder_sx2 = HB_SX2;
	const int hborder_ex2 = HB_EX2;
	const int hborder_sy = HB_SY;
	const int hborder_ey = HB_EY;
	const double new_x = ball.cx + ball.dx;
	const double new_y = ball.cy + ball.dy;
	
	if((new_y - BALL_RADIUS <= hborder_sy) ||
		(new_y + BALL_RADIUS >= hborder_ey))	
	{
		if((new_x >= hborder_sx1 && new_x <= hborder_ex1) ||
			(new_x >= hborder_sx2 && new_x <= hborder_ex2))
			return 1;
	}
	return 0;
}

int is_ball_on_table_vertical_border(billiard_ball ball)
{
	const int vborder_sx = VB_SX;
	const int vborder_ex = VB_EX;
	const int vborder_sy = VB_SY;
	const int vborder_ey = VB_EY;
	const double new_x = ball.cx + ball.dx;
	const double new_y = ball.cy + ball.dy;
	
	if((new_x - BALL_RADIUS <= vborder_sx) ||
		(new_x + BALL_RADIUS >= vborder_ex))
	{
		if(new_y >= vborder_sy && new_y <= vborder_ey)
			return 1;
	}
	return 0;
}

void set_velocity_to_ball(billiard_ball *balls, const int index, 
		const double dx, const double dy)
{
	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return;
	}
#endif

	balls[index].dx = dx;
	balls[index].dy = dy;
}

void set_white_ball_to_original_place(billiard_ball *balls)
{
	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return;
	}
#endif
	
	balls[0].cx = WB_X;
	balls[0].cy = WB_Y;
	balls[0].is_on_table = 1;
}

int *get_ball_status(billiard_ball *balls)
{
	int i;
	int *status = malloc(sizeof(int) * BALL_COUNT);
	
	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return NULL;
	}
#endif

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{
		if(balls[i].is_on_table)
			status[i] = 1;
		else 
			status[i] = 0;
	}
	return status;
}

// function to compute direction
static double compute_direction(const double dx, const double dy)
{
	double theta = 0;;
	if(dx == 0 && dy == 0)
		return 0;
	else if(dx == 0 && dy < 0)
		return 3 * PI / 2;
	else if(dx == 0 && dy > 0)
		return PI / 2;
	else theta = atan(dy / dx);

	if(dy < 0 && dx > 0) theta += 2 * PI;
	else if(dy < 0 && dx < 0) theta += PI;
	else if(dy > 0 && dx < 0) theta += PI;
	
	return theta;
}

void update_billiard_balls(billiard_ball *balls)
{
	int i, j;
	double dx, dy, speed, direction;
	// variables for collisions
	// vector(x1 - x2)
	double delta_x, delta_y;
	// vector(v1 - v2)
	double dvx, dvy;
	// dot product, delta_length, an
	double dot_product, delta_length_square, an;
	// new velocities
	double new_v1x, new_v1y, new_v2x, new_v2y;
	// to prevent ball colliding each other the next time
	// we need to move the ball away if they will collide on each other again
	double slope, dcx, dcy;

	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return;
	}
#endif

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{
	//	printf("updating ball %i\n", i);
		dx = balls[i].dx;
		dy = balls[i].dy;

		// if the ball is not on the table,
		// which means it's in the bag
		// dont event bother updating it
		if(!balls[i].is_on_table)
			continue;

		// compute speed and direction
		speed = sqrt(dx * dx + dy * dy);
		direction = compute_direction(dx, dy);	
		// apply friction force
		if(speed + BALL_FRICTION > 0) speed += BALL_FRICTION;
		else if(speed + BALL_FRICTION / 2 > 0) speed += BALL_FRICTION / 2;
		else if(speed + BALL_FRICTION / 8 > 0) speed += BALL_FRICTION / 8;
		else speed = 0;
		balls[i].dx = speed * cos(direction);
		balls[i].dy = speed * sin(direction);

		// if(speed == 0) continue;

		// collision detection -- border of the table
		if(is_ball_on_table_vertical_border(balls[i]))
		{
			balls[i].dx = -dx;
			balls[i].dx *= COLLISION_FRICTION;
			// to prevent ball sticking on the vertical border
			// if the ball is still on the vertical border next time
			// change the cx a little so that
			// the ball no longer stays on the vertical border
			while(is_ball_on_table_vertical_border(balls[i]))
				balls[i].cx += balls[i].dx >= 0 ? CP : -CP;
		}
		if(is_ball_on_table_horizontal_border(balls[i]))	
		{
			balls[i].dy = -dy;
			balls[i].dy *= COLLISION_FRICTION;	
			// to prevent ball sticking on the horizontal border
			// if the ball is still on the horizontal border next time
			// change the cy a little so that
			// the ball no longer stays on the horizontal border
			while(is_ball_on_table_horizontal_border(balls[i]))
				balls[i].cy += balls[i].dy >= 0 ? CP : -CP;
		}
		// check if the ball is in the bag
		if(is_ball_in_bag(balls[i]))
		{
			balls[i].dx = 0;
			balls[i].dy = 0;
			balls[i].is_on_table = 0;
			/*
			if(balls[i].number == 0)
			{
				// white ball got into the bag
				// reset the white ball and change turn
				balls[i].cx = 600;
				balls[i].cy = 400;
				balls[i].is_on_table = 1;
			}
			*/
		}
		// collision detection -- ball with ball
		for(j = 0 ; j < BALL_COUNT ; j ++)
		{
			if(i == j) continue;
			if(!balls[j].is_on_table) continue;
			if(is_ball_collided(balls[i], balls[j]))
			{
				
				//printf("ball %d collide with ball %i\n", i, j);
				//printf("before collision: ");
				//printf("ball %i(dx=%lf,dy=%lf), ball %i(dx=%lf,dy=%lf)\n",
				//		i, balls[i].dx, balls[i].dy, j, balls[j].dx, balls[j].dy);
				

				/***************************************************************
				 *	using the 2d collision formula on	
				 *	http://en.wikipedia.org/wiki/Elastic_collision
				 *
				 *	<v1'> = <v1> - (<v1-v2> dot <x1-x2>) / |<x1-x2>|^2 * <x1-x2>
				 *	<v2'> = <v2> - (<v2-v1> dot <x2-x1>) / |<x2-x1>|^2 * <x2-x1>
				 ***************************************************************/
				// new velocities computation
				// vector(x1 - x2)
				delta_x = balls[i].cx - balls[j].cx;
				delta_y = balls[i].cy - balls[j].cy;
				// vector(v1 - v2)
				dvx = balls[i].dx - balls[j].dx;
				dvy = balls[i].dy - balls[j].dy;
				// dot product vector(v1 - v2) dot vector(x1 - x2)
				dot_product = dvx * delta_x + dvy * delta_y;
				// denumerator
				delta_length_square = delta_x * delta_x + delta_y * delta_y;
				an = dot_product / delta_length_square;

				new_v1x = balls[i].dx - an * delta_x;
				new_v1y = balls[i].dy - an * delta_y;
				
				new_v2x = balls[j].dx + an * delta_x;
				new_v2y = balls[j].dy + an * delta_y;

				// assign new velocities to ball i and j
				balls[i].dx = new_v1x;
				balls[i].dy = new_v1y;
				balls[j].dx = new_v2x;
				balls[j].dy = new_v2y;	

				// two ball collide with friction
				// which reduce both balls' velocity
				balls[i].dx *= COLLISION_FRICTION;
				balls[i].dy *= COLLISION_FRICTION;
				balls[j].dx *= COLLISION_FRICTION;
				balls[j].dy *= COLLISION_FRICTION;

				// play the collision audio
				al_play_sample(balls[i].collision_sound, 
						BC_SOUND_GAIN, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

				while(is_ball_collided(balls[i], balls[j]))
				{
#ifdef DEBUG
					printf("shifting...\n");
#endif
					if(delta_x != 0)
					{
						slope = delta_y / delta_x;
						dcx = balls[i].cx > balls[j].cx ? CP : -CP;
						dcy = balls[i].cy > balls[j].cy ? slope * dx : -slope * dx;

						balls[i].cx += dcx;
						balls[i].cy += dcy;
						balls[j].cx -= dcx;	
						balls[j].cy -= dcy;
					}
					else
					{
						dcy = balls[i].cy > balls[j].cy ? CP : -CP;
						balls[i].cy += dcy;
						balls[j].cy -= dcy;
					}
				}
			}
		}
	}

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{	
		// change the x, y position
		balls[i].cx += balls[i].dx;
		balls[i].cy += balls[i].dy;
	}
}

void draw_billiard_balls(billiard_ball *balls)
{
	int i;
	const double sx = 0;
	const double sy = 0;
	const double dw = 2 * BALL_RADIUS;
	const double dh = 2 * BALL_RADIUS;
	double sw, sh, dx, dy;
	
	/* DEBUG */
#ifdef DEBUG
	if(balls == NULL)
	{
		perror("billiard_ball null pointer");
		return;
	}
	for(i = 0 ; i < BALL_COUNT ; i ++)
		al_draw_filled_circle(balls[i].cx, balls[i].cy,
				BALL_RADIUS, balls[i].color);
#endif

	for(i = 0 ; i < BALL_COUNT ; i ++)
	{
		if(!balls[i].is_on_table)
			continue;
		sw = balls[i].bitmap_sw;
		sh = balls[i].bitmap_sh;
		dx = balls[i].cx - BALL_RADIUS;
		dy = balls[i].cy - BALL_RADIUS;
		al_draw_scaled_bitmap(balls[i].bitmap,
			sx, sy, sw, sh,
			dx, dy, dw, dh, 0);
	}
}

