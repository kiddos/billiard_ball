#include "game.h"

/* constants */
#define GAME_LOADING_TEXT "LOADING ..."
const char* const GAME_FONT_FILE_PATH = "./res/font/Junicode.ttf";
const char* const GAME_BACKGROUND_MUSIC_PATH =
      "./res/sound/background_music.ogg";
const double GAME_DEFAULT_MUSIC_GAIN = 1.6;
const uint32_t GAME_DEFAULT_MOUSE_FONT_SIZE = 12;
const uint32_t GAME_DEFAULT_LOADING_FONT_SIZE = 36;

game *game_init_object() {
  game *g = malloc(sizeof(game));

  // default pointers null
  g->display = NULL;
  g->timer = NULL;
  g->loading_font = NULL;
  g->mouse_font = NULL;
  g->event_queue = NULL;
  g->bg_music = NULL;
  g->bg_music_instance = NULL;
  g->bg = NULL;
  g->balls = NULL;
  g->m = NULL;
  g->board = NULL;
  g->ball_original_status = NULL;
  g->ball_new_status = NULL;

  // set default value
  g->window_width = DEFAULT_WINDOW_WIDTH;
  g->window_height = DEFAULT_WINDOW_HEIGHT;
  g->loading_font_size = GAME_DEFAULT_LOADING_FONT_SIZE;
  g->mouse_font_size = GAME_DEFAULT_MOUSE_FONT_SIZE;

  // load font
  g->loading_font = al_load_font(GAME_FONT_FILE_PATH,
                                g->loading_font_size,
                                ALLEGRO_TTF_NO_KERNING);
  if (!g->loading_font) {
    error_message("fail to load loading font");
    return NULL;
  }

  // mouse font
  g->mouse_font = al_load_font(GAME_FONT_FILE_PATH,
                               g->mouse_font_size,
                               ALLEGRO_TTF_NO_KERNING);
  if (!g->mouse_font) {
    error_message("fail to load mouse font");
    al_destroy_font(g->loading_font);
    return NULL;
  }

  // display flags
  // - allow resizing
  // - enable opengl
  al_set_new_display_flags(ALLEGRO_WINDOWED |
                           ALLEGRO_RESIZABLE |
                           ALLEGRO_OPENGL);

  // game display
  g->display = al_create_display(g->window_width, g->window_height);
  if (!g->display) {
    error_message("fail to create allegro display");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    return NULL;
  }
  al_acknowledge_resize(g->display);

  // after game display is prepare
  // draw the loading text to screen
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_draw_text(g->loading_font,
               al_map_rgb(255, 255, 255),
               g->window_width / 2,
               g->window_height / 2 - g->loading_font_size,
               ALLEGRO_ALIGN_CENTER, GAME_LOADING_TEXT);
  al_flip_display();
  // and load the rest of the objects

  // timer object
  g->timer = al_create_timer(1.0 / FPS);
  if (!g->timer) {
    error_message("fail to create allegro timer");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    return NULL;
  }

  // event queue
  g->event_queue = al_create_event_queue();
  if (!g->event_queue) {
    error_message("fail to create allegro event queue");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    return NULL;
  }

  // background music
  g->bg_music = al_load_sample(GAME_BACKGROUND_MUSIC_PATH);
  if (!g->bg_music) {
    error_message("fail to load background music");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    return NULL;
  }
  g->bg_music_instance = al_create_sample_instance(g->bg_music);
  if (!g->bg_music_instance) {
    error_message("fail to load background music instance");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    return NULL;
  }
  if (!al_set_sample_instance_playmode(g->bg_music_instance,
                                      ALLEGRO_PLAYMODE_LOOP)) {
    error_message("fail to set background music playmode to loop");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    return NULL;
  }
  if (!al_set_sample_instance_gain(g->bg_music_instance,
                                  GAME_DEFAULT_MUSIC_GAIN)) {
    error_message("fail to set background music volume");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    return NULL;
  }
  // attach the music sample instance to the default mixer
  if (!al_attach_sample_instance_to_mixer(g->bg_music_instance,
                                         al_get_default_mixer())) {
    error_message("fail to attach background music to mixer");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    return NULL;
  }

  /* modules */
  // background module
  g->bg = create_background();
  if (!g->bg) {
    error_message("fail to create background");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    return NULL;
  }

  // billiard balls module
  g->balls = create_billiard_balls();
  if (!g->balls) {
    perror("fail to create billiard balls");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    destroy_background(g->bg);
    return NULL;
  }

  // menu module
  g->m = create_menu();
  if (!g->m) {
    error_message("fail to create menu");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    destroy_background(g->bg);
    destroy_billiard_balls(g->balls);
    return NULL;
  }

  // score board module
  g->board = create_score_board();
  if (!g->board) {
    error_message("fail to create menu");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->display);
    al_destroy_timer(g->timer);
    al_destroy_event_queue(g->event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    destroy_background(g->bg);
    destroy_billiard_balls(g->balls);
    destroy_menu(g->m);
    return NULL;
  }
  return g;
}

void game_release_object(game *obj) {
  if (obj) {
    if (obj->loading_font) {
      regular_message("release game loading font");
      al_destroy_font(obj->loading_font);
    }
    if (obj->mouse_font) {
      regular_message("release game mouse font");
      al_destroy_font(obj->mouse_font);
    }
    if (obj->display) {
      regular_message("release game display");
      al_destroy_display(obj->display);
    }
    if (obj->timer) {
      regular_message("release game timer");
      al_destroy_timer(obj->timer);
    }
    if (obj->event_queue) {
      regular_message("release game event queue");
      al_destroy_event_queue(obj->event_queue);
    }
    if (obj->bg_music) {
      regular_message("release game background music");
      al_destroy_sample(obj->bg_music);
    }
    if (obj->bg) {
      regular_message("release game background module");
      destroy_background(obj->bg);
    }
    if (obj->balls) {
      regular_message("release game ball module");
      destroy_billiard_balls(obj->balls);
    }
    if (obj->m) {
      regular_message("release game menu");
      destroy_menu(obj->m);
    }
    if (obj->board) {
      regular_message("release game score board");
      destroy_score_board(obj->board);
    }
  }
}

void game_prepare(game *g) {
  // register event source
  al_register_event_source(g->event_queue,
                           al_get_display_event_source(g->display));
  al_register_event_source(g->event_queue,
                           al_get_mouse_event_source());
  al_register_event_source(g->event_queue,
                           al_get_keyboard_event_source());
  al_register_event_source(g->event_queue,
                           al_get_timer_event_source(g->timer));

  // start the background music
  al_play_sample_instance(g->bg_music_instance);
  // start timer
  al_start_timer(g->timer);

  // game running
  g->running = true;
}

/* draw mouse coordinates */
static void draw_mouse_coordinates(const int mx,
                                   const int my,
                                   const ALLEGRO_FONT *font) {
  char text[20] = {'\0'};
  sprintf(text, "[%03d,%03d]", mx, my);
  /*al_draw_text(font, al_map_rgb(255, 255, 255),*/
               /*MX, MY, ALLEGRO_ALIGN_LEFT, text);*/
}

/* draw the reference line for ball hitting */
static void draw_referencing_line(const int sx,
                           const int sy,
                           const int cx,
                           const int cy) {
  const int tborder_x1 = BORDER_WIDTH;
  const int tborder_x2 = TABLE_WIDTH - BORDER_WIDTH;
  const int tborder_y1 = TABLE_Y_POSITION + BORDER_WIDTH;
  const int tborder_y2 = TABLE_Y_POSITION + TABLE_HEIGHT - BORDER_WIDTH;
  const int dx = sx - cx;
  const int dy = sy - cy;
  int line_x = sx, line_y = sy;
  double delta;

  if (dx == 0 && dy == 0)
    return;

  while(1) {
    line_x += dx;
    line_y += dy;
    if(line_x <= tborder_x1 || line_x >= tborder_x2) {
      line_x = (line_x <= tborder_x1 ? tborder_x1 : tborder_x2);
      delta = 1.0 * (line_x - sx) / dx;
      line_y = sy + (int) (dy * delta);
      break;
    }

    if(line_y <= tborder_y1 || line_y >= tborder_y2) {
      line_y = (line_y <= tborder_y1 ? tborder_y1 : tborder_y2);
      delta = 1.0 * (line_y - sy) / dy;
      line_x = sx + (int) (dx * delta);
      break;
    }
  }

  /*al_draw_line(line_x, line_y, cx, cy, COLOR_WHITE, RL_WIDTH);*/
}

void game_main_loop(game *g) {
  int i;
  int result;
  ALLEGRO_EVENT event;
  while (g->running) {
    // listen to event
    al_wait_for_event(g->event_queue, &event);
    if (event.type == ALLEGRO_EVENT_TIMER) {
      g->redraw = true;

      switch (g->m->mode) {
        case MENU_MODE:
          break;
        case GAME_MODE:
          update_billiard_balls(g->balls);

          // if the balls are moving
          // due to user input
          if (g->is_ball_hitted && is_all_ball_stopped(g->balls) &&
              g->ball_original_status != NULL) {
            // printf("checking status\n");
            g->should_change_turn = true;
            g->ball_new_status = get_ball_status(g->balls);
            for (i = 0 ; i < BALL_COUNT ; i ++) {
              if (g->ball_original_status[i] > g->ball_new_status[i]) {
                // if the ball this player hitted in
                // is not the white ball,
                // it should NOT change turn
                if (i == 0) {
                  // set the white ball to default location
                  // cx = 600, cy = 400
                  g->balls[0].cx = WB_X;
                  g->balls[0].cy = WB_Y;
                  g->balls[0].is_on_table = 1;
                  // printf("resetting white ball\n");
                } else {
                  g->should_change_turn = false;
                }
                add_score_to_score_board(g->board, g->balls[i].score);
              }
            }

            // if all ball(except white ball) are all in bags
            // this game is done
            if (is_all_ball_in_bag(g->balls)) {
              if (get_who_win(g->board) == SB_P1_TURN) {
                regular_message("player 1 wins");
              } else if (get_who_win(g->board) == SB_P2_TURN) {
                regular_message("player 2 wins");
              } else if (get_who_win(g->board) == SB_DRAW) {
                regular_message("draw");
              }

              record_score(g->board);
              reset_billiard_balls(g->balls);

              if (get_round(g->board) == MAX_GAME_NUM) {
                g->highest_score = get_highest_player_score_of_3(g->board);
                g->who_score_highest = get_who_score_higher_of_3(g->board);

                printf("%d got %i", g->who_score_highest,
                    g->highest_score);
                save_score_to_config(g->board,
                    g->who_score_highest, g->highest_score);

                // TODO
                // prompt the user to continue or not

                if (result != 1) {
                  g->m->mode = MENU_MODE;
                  set_menu_mode(g->m, DEFAULT_MENU_MODE);
                }

                reset_billiard_balls(g->balls);
                reset_score_board(g->board);
              }
            } else {
              // check if its necessary to change turn
              // if the player didnt hitted in any ball
              // or the player hitted in white ball
              // change turn
              if (g->should_change_turn ||
                  g->ball_original_status[0] > g->ball_new_status[0]) {
                regular_message("change turn");
                change_turn(g->board);
              }
            }

            // reset is_ball_hitted boolean
            // so the ball can be hitted again
            g->is_ball_hitted = false;
            // free the used memory
            if (g->ball_new_status != NULL) {
              free(g->ball_new_status);
              g->ball_new_status = NULL;
            }

            if (g->ball_original_status != NULL) {
              free(g->ball_original_status);
              g->ball_original_status = NULL;
            }
          }
          break;
      }
    } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      result = al_show_native_message_box(g->display,
          "Quit?", "Quit?", "Are you sure you want to Quit?",
          NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
      // if the result code == 1
      // means user click ok
      // break the main loop and exit
      if (result == 1) {
        break;
      }
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      if (g->m->mode == MENU_MODE) {
        int current_selection = get_menu_selection(g->m);
        switch (event.keyboard.keycode) {
          case ALLEGRO_KEY_UP:
            if (current_selection > PLAY_MENU_SELECTION) {
              set_menu_selection(g->m, current_selection - 1);
            }
            break;
          case ALLEGRO_KEY_DOWN:
            if (current_selection < EXIT_MENU_SELECTION) {
              set_menu_selection(g->m, current_selection + 1);
            }
            break;
          case ALLEGRO_KEY_ENTER:
            if (current_selection == PLAY_MENU_SELECTION) {
              g->m->mode = GAME_MODE;
            } else if (current_selection == RULE_MENU_SELECTION) {
              if (get_menu_mode(g->m) != RULE_MODE) {
                set_menu_mode(g->m, RULE_MODE);
              } else if (get_menu_mode(g->m) == RULE_MODE) {
                set_menu_mode(g->m, DEFAULT_MENU_MODE);
              }
            } else if (current_selection == SCORE_MENU_SELECTION) {
              if (get_menu_mode(g->m) != SCORE_MODE) {
                set_menu_mode(g->m, SCORE_MODE);
              } else if (get_menu_mode(g->m) == SCORE_MODE) {
                set_menu_mode(g->m, DEFAULT_MENU_MODE);
              }
            } else if (current_selection == EXIT_MENU_SELECTION) {
              g->running = false;
            }
            break;
        }
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES ||
               event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
      if (g->m->mode == GAME_MODE) {
        g->mx = event.mouse.x;
        g->my = event.mouse.y;
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
      if (g->m->mode == GAME_MODE) {
        g->is_ready_to_hit = false;
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
      if (g->m->mode == GAME_MODE) {
        if (is_mouse_on_ball(g->balls[0], g->mx, g->my)) {
          g->start_x = event.mouse.x;
          g->start_y = event.mouse.y;

          g->is_ready_to_hit = true;
        }

        // TODO
        // if the mouse press on the buttons
        /*if (event.mouse.x > NGB_X_POS &&*/
            /*event.mouse.x < NGB_X_POS + NGB_WIDTH &&*/
            /*event.mouse.y > NGB_Y_POS &&*/
            /*event.mouse.y < NGB_Y_POS + NGB_HEIGHT) {*/
          /*printf("new game button pressed\n");*/
          /*reset_billiard_balls(g->balls);*/
          /*reset_score_board(g->board);*/

          /*ngbutton_pressed = true;*/
        /*}*/

        /*if(event.mouse.x > BB_X_POS &&*/
            /*event.mouse.x < BB_X_POS + BB_WIDTH &&*/
            /*event.mouse.y > BB_Y_POS &&*/
            /*event.mouse.y < BB_Y_POS + BB_HEIGHT)*/
        /*{*/
          /*bbutton_pressed = true;*/
          /*printf("back button pressed\n");*/
          /*mode = MENU_MODE;*/
          /*reset_billiard_balls(balls);*/
          /*reset_score_board(board);*/
          /*set_menu_mode(m, DEFAULT_MENU_MODE);*/

          /*bbutton_pressed = false;*/
        /*}*/

      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
      if (g->m->mode == MENU_MODE) {

      } else if (g->m->mode == GAME_MODE) {
        // if the mouse is not on the ball after the dragging
        // its ready to shoot the ball
        if (!is_mouse_on_ball(g->balls[0], event.mouse.x, event.mouse.y) &&
            g->is_ready_to_hit) {
          g->end_x = event.mouse.x;
          g->end_y = event.mouse.y;

          g->dx = g->end_x - g->start_x;
          g->dy = g->end_y - g->start_y;

          set_velocity_to_ball(g->balls, 0,
                               g->dx * BALL_DELTA_SCALE,
                               g->dy * BALL_DELTA_SCALE);

          // clear the original status memory first
          // get the original status
          if (g->ball_original_status != NULL) {
            free(g->ball_original_status);
            g->ball_original_status = NULL;
          }

          g->ball_original_status = get_ball_status(g->balls);

          // after the velocity is set
          // balls are moving
          g->is_ball_hitted = true;
        }
        g->is_ready_to_hit = false;
      }
    }

    if (g->redraw && al_is_event_queue_empty(g->event_queue)) {
      // clear the screen to black
      al_clear_to_color(al_map_rgb(0, 0, 0));

      switch (g->m->mode) {
        case MENU_MODE:
          draw_menu(g->m);
          break;
        case GAME_MODE:
          // drawings

          // draw the background first
          draw_background(g->bg);
          // draw the score board
          draw_score_board(g->board);

          // TODO
          // draw 2 buttons

          // draw mouse coordinates
          draw_mouse_coordinates(g->mx, g->my, g->mouse_font);

          if (g->is_ready_to_hit)
            draw_referencing_line(g->balls[0].cx, g->balls[0].cy,
                                  g->mx, g->my);

          draw_billiard_balls(g->balls);
          break;
      }

      // flip the display
      al_flip_display();
      g->redraw = false;
    }
  }
}

