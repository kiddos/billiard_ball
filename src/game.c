#include "game.h"

/* constants */
const char* const GAME_LOADING_TEXT = "LOADING ...";
const char* const GAME_NEW_GAME_TEXT = "NEW GAME";
const char* const GAME_BACK_TEXT = "BACK";
const char* const BUTTON_PRESSED_BITMAP = "./res/pics/button_pressed.png";
const char* const BUTTON_RELEASED_BITMAP = "./res/pics/button_released.png";
const char* const GAME_FONT_FILE_PATH = "./res/font/Junicode.ttf";
const char* const GAME_BACKGROUND_MUSIC_PATH =
      "./res/sound/background_music.ogg";
const double GAME_DEFAULT_MUSIC_GAIN = 1.6;
const uint32_t GAME_DEFAULT_MOUSE_FONT_SIZE = 12;
const uint32_t GAME_DEFAULT_LOADING_FONT_SIZE = 36;

game * game_init() {
  game *g = (game *) malloc(sizeof(game));

  // default pointers null
  g->core.display = NULL;
  g->core.timer = NULL;
  g->core.event_queue = NULL;
  g->loading_font = NULL;
  g->mouse_font = NULL;
  g->bg_music = NULL;
  g->bg_music_instance = NULL;

  g->module.bg = NULL;
  g->module.balls = NULL;
  g->module.m = NULL;
  g->module.board = NULL;
  g->module.new_game_button = NULL;
  g->module.back_return_button = NULL;
  g->module.new_game_button = NULL;
  g->module.back_return_button = NULL;

  g->ball_original_status = NULL;
  g->ball_new_status = NULL;

  // set default value
  g->window_width = DEFAULT_WINDOW_WIDTH;
  g->window_height = DEFAULT_WINDOW_HEIGHT;
  g->loading_font_size = GAME_DEFAULT_LOADING_FONT_SIZE;
  g->mouse_font_size = GAME_DEFAULT_MOUSE_FONT_SIZE;
  g->redraw = false;
  g->mx = 0;
  g->my = 0;
  g->is_ball_hitted = false;
  g->is_ready_to_hit = false;

  // load font
  g->loading_font = al_load_font(GAME_FONT_FILE_PATH,
                                g->loading_font_size,
                                ALLEGRO_TTF_NO_KERNING);
  if (!g->loading_font) {
    error_message("fail to load loading font");
    free(g);
    return NULL;
  }

  // mouse font
  g->mouse_font = al_load_font(GAME_FONT_FILE_PATH,
                               g->mouse_font_size,
                               ALLEGRO_TTF_NO_KERNING);
  if (!g->mouse_font) {
    error_message("fail to load mouse font");
    al_destroy_font(g->loading_font);
    free(g);
    return NULL;
  }

  // display flags
  // - allow resizing
  // - enable opengl
  al_set_new_display_flags(ALLEGRO_WINDOWED |
                           ALLEGRO_RESIZABLE |
                           ALLEGRO_OPENGL);

  // game display
  g->core.display = al_create_display(g->window_width, g->window_height);
  if (!g->core.display) {
    error_message("fail to create allegro display");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    free(g);
    return NULL;
  }
  al_acknowledge_resize(g->core.display);

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
  g->core.timer = al_create_timer(1.0 / FPS);
  if (!g->core.timer) {
    error_message("fail to create allegro timer");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    free(g);
    return NULL;
  }

  // event queue
  g->core.event_queue = al_create_event_queue();
  if (!g->core.event_queue) {
    error_message("fail to create allegro event queue");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    free(g);
    return NULL;
  }

  // background music
  g->bg_music = al_load_sample(GAME_BACKGROUND_MUSIC_PATH);
  if (!g->bg_music) {
    error_message("fail to load background music");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    free(g);
    return NULL;
  }
  g->bg_music_instance = al_create_sample_instance(g->bg_music);
  if (!g->bg_music_instance) {
    error_message("fail to load background music instance");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    free(g);
    return NULL;
  }
  if (!al_set_sample_instance_playmode(g->bg_music_instance,
                                      ALLEGRO_PLAYMODE_LOOP)) {
    error_message("fail to set background music playmode to loop");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    free(g);
    return NULL;
  }
  if (!al_set_sample_instance_gain(g->bg_music_instance,
                                  GAME_DEFAULT_MUSIC_GAIN)) {
    error_message("fail to set background music volume");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    free(g);
    return NULL;
  }
  // attach the music sample instance to the default mixer
  if (!al_attach_sample_instance_to_mixer(g->bg_music_instance,
                                         al_get_default_mixer())) {
    error_message("fail to attach background music to mixer");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    free(g);
    return NULL;
  }

  /* modules */
  // background module
  g->module.bg = background_init(
      size_init(g->window_width, g->window_height, 0));
  if (!g->module.bg) {
    error_message("fail to create background");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    free(g);
    return NULL;
  }

  // billiard balls module
  g->module.balls = create_billiard_balls();
  if (!g->module.balls) {
    perror("fail to create billiard balls");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    background_destroy(g->module.bg);
    free(g);
    return NULL;
  }

  // menu module
  g->module.m = create_menu();
  if (!g->module.m) {
    error_message("fail to create menu");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    background_destroy(g->module.bg);
    destroy_billiard_balls(g->module.balls);
    free(g);
    return NULL;
  }

  // score board module
  g->module.board = create_score_board();
  if (!g->module.board) {
    error_message("fail to create menu");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    background_destroy(g->module.bg);
    destroy_billiard_balls(g->module.balls);
    destroy_menu(g->module.m);
    free(g);
    return NULL;
  }

  rect r = rect_init(g->window_width - g->window_width / 8,
                     10, 0, g->window_width / 8, g->window_height / 24, 0);
  g->module.new_game_button = button_init(r, GAME_NEW_GAME_TEXT,
                                          GAME_FONT_FILE_PATH,
                                          color_white(),
                                          BUTTON_PRESSED_BITMAP,
                                          BUTTON_RELEASED_BITMAP,
                                          BUTTON_RELEASED_BITMAP,
                                          NULL);
  if (!g->module.new_game_button) {
    error_message("fail to create new game button");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    background_destroy(g->module.bg);
    destroy_billiard_balls(g->module.balls);
    destroy_menu(g->module.m);
    destroy_score_board(g->module.board);
    free(g);
    return NULL;
  }

  r = rect_init(0,
                     0, 0, g->window_width / 12, g->window_height / 24, 0);
  g->module.back_return_button = button_init(r, GAME_BACK_TEXT,
                                             GAME_FONT_FILE_PATH,
                                             color_white(),
                                             BUTTON_PRESSED_BITMAP,
                                             BUTTON_RELEASED_BITMAP,
                                             BUTTON_RELEASED_BITMAP,
                                             NULL);
  if (!g->module.back_return_button) {
    error_message("fail to create back return button");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    al_destroy_display(g->core.display);
    al_destroy_timer(g->core.timer);
    al_destroy_event_queue(g->core.event_queue);
    al_destroy_sample(g->bg_music);
    al_destroy_sample_instance(g->bg_music_instance);
    background_destroy(g->module.bg);
    destroy_billiard_balls(g->module.balls);
    destroy_menu(g->module.m);
    destroy_score_board(g->module.board);
    button_destroy(g->module.new_game_button);
    free(g);
    return NULL;
  }
  return g;
}

void game_destroy(game *g) {
  if (g) {
    if (g->loading_font) {
      regular_message("release game loading font");
      al_destroy_font(g->loading_font);
    }
    if (g->mouse_font) {
      regular_message("release game mouse font");
      al_destroy_font(g->mouse_font);
    }
    if (g->core.display) {
      regular_message("release game display");
      al_destroy_display(g->core.display);
    }
    if (g->core.timer) {
      regular_message("release game timer");
      al_destroy_timer(g->core.timer);
    }
    if (g->core.event_queue) {
      regular_message("release game event queue");
      al_destroy_event_queue(g->core.event_queue);
    }
    if (g->bg_music) {
      regular_message("release game background music");
      al_destroy_sample(g->bg_music);
    }
    if (g->module.bg) {
      regular_message("release game background module");
      background_destroy(g->module.bg);
    }
    if (g->module.balls) {
      regular_message("release game ball module");
      destroy_billiard_balls(g->module.balls);
    }
    if (g->module.m) {
      regular_message("release game menu");
      destroy_menu(g->module.m);
    }
    if (g->module.board) {
      regular_message("release game score board");
      destroy_score_board(g->module.board);
    }
    if (g->module.new_game_button) {
      regular_message("release new game button");
      button_destroy(g->module.new_game_button);
    }
    if (g->module.back_return_button) {
      regular_message("release back button");
      button_destroy(g->module.back_return_button);
    }
    if (g->ball_original_status) {
      regular_message("release ball original status array");
      free(g->ball_original_status);
    }
    if (g->ball_new_status) {
      regular_message("release ball new status array");
      free(g->ball_new_status);
    }
  }
  free(g);
}

void game_prepare(game *g) {
  // register event source
  al_register_event_source(g->core.event_queue,
                           al_get_display_event_source(g->core.display));
  al_register_event_source(g->core.event_queue,
                           al_get_mouse_event_source());
  al_register_event_source(g->core.event_queue,
                           al_get_keyboard_event_source());
  al_register_event_source(g->core.event_queue,
                           al_get_timer_event_source(g->core.timer));

  // start the background music
  al_play_sample_instance(g->bg_music_instance);
  // start timer
  al_start_timer(g->core.timer);

  // game running
  g->running = true;
}

/* draw mouse coordinates */
static void draw_mouse_coordinates(const point mouse, const size window_size,
                                   const ALLEGRO_FONT *font) {
  double text_width;
  char text[32] = {'\0'};

  sprintf(text, "[%03.0lf,%03.0lf]", mouse.x, mouse.y);
  text_width = al_get_text_width(font, text);
  al_draw_text(font, color_white(),
               window_size.width - text_width, 10, ALLEGRO_ALIGN_LEFT, text);
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
    al_wait_for_event(g->core.event_queue, &event);
    if (event.type == ALLEGRO_EVENT_TIMER) {
      g->redraw = true;

      switch (g->module.m->mode) {
        case MENU_MODE:
          break;
        case GAME_MODE:
          update_billiard_balls(g->module.balls);

          // if the balls are moving
          // due to user input
          if (g->is_ball_hitted && is_all_ball_stopped(g->module.balls) &&
              g->ball_original_status != NULL) {
            // printf("checking status\n");
            g->should_change_turn = true;
            g->ball_new_status = get_ball_status(g->module.balls);
            for (i = 0 ; i < BALL_COUNT ; i ++) {
              if (g->ball_original_status[i] > g->ball_new_status[i]) {
                // if the ball this player hitted in
                // is not the white ball,
                // it should NOT change turn
                if (i == 0) {
                  // set the white ball to default location
                  // cx = 600, cy = 400
                  g->module.balls[0].cx = WB_X;
                  g->module.balls[0].cy = WB_Y;
                  g->module.balls[0].is_on_table = 1;
                  // printf("resetting white ball\n");
                } else {
                  g->should_change_turn = false;
                }
                add_score_to_score_board(g->module.board,
                                         g->module.balls[i].score);
              }
            }

            // if all ball(except white ball) are all in bags
            // this game is done
            if (is_all_ball_in_bag(g->module.balls)) {
              if (get_who_win(g->module.board) == SB_P1_TURN) {
                regular_message("player 1 wins");
              } else if (get_who_win(g->module.board) == SB_P2_TURN) {
                regular_message("player 2 wins");
              } else if (get_who_win(g->module.board) == SB_DRAW) {
                regular_message("draw");
              }

              record_score(g->module.board);
              reset_billiard_balls(g->module.balls);

              if (get_round(g->module.board) == MAX_GAME_NUM) {
                g->highest_score = get_highest_player_score_of_3(g->module.board);
                g->who_score_highest = get_who_score_higher_of_3(g->module.board);

                printf("%d got %i", g->who_score_highest,
                    g->highest_score);
                save_score_to_config(g->module.board,
                    g->who_score_highest, g->highest_score);

                // TODO
                // prompt the user to continue or not

                if (result != 1) {
                  g->module.m->mode = MENU_MODE;
                  set_menu_mode(g->module.m, DEFAULT_MENU_MODE);
                }

                reset_billiard_balls(g->module.balls);
                reset_score_board(g->module.board);
              }
            } else {
              // check if its necessary to change turn
              // if the player didnt hitted in any ball
              // or the player hitted in white ball
              // change turn
              if (g->should_change_turn ||
                  g->ball_original_status[0] > g->ball_new_status[0]) {
                regular_message("change turn");
                change_turn(g->module.board);
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
      result = al_show_native_message_box(g->core.display,
          "Quit?", "Quit?", "Are you sure you want to Quit?",
          NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
      // if the result code == 1
      // means user click ok
      // break the main loop and exit
      if (result == 1) {
        break;
      }
    } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
      if (g->module.m->mode == MENU_MODE) {
        int current_selection = get_menu_selection(g->module.m);
        switch (event.keyboard.keycode) {
          case ALLEGRO_KEY_UP:
            if (current_selection > PLAY_MENU_SELECTION) {
              set_menu_selection(g->module.m, current_selection - 1);
            }
            break;
          case ALLEGRO_KEY_DOWN:
            if (current_selection < EXIT_MENU_SELECTION) {
              set_menu_selection(g->module.m, current_selection + 1);
            }
            break;
          case ALLEGRO_KEY_ENTER:
            if (current_selection == PLAY_MENU_SELECTION) {
              g->module.m->mode = GAME_MODE;
            } else if (current_selection == RULE_MENU_SELECTION) {
              if (get_menu_mode(g->module.m) != RULE_MODE) {
                set_menu_mode(g->module.m, RULE_MODE);
              } else if (get_menu_mode(g->module.m) == RULE_MODE) {
                set_menu_mode(g->module.m, DEFAULT_MENU_MODE);
              }
            } else if (current_selection == SCORE_MENU_SELECTION) {
              if (get_menu_mode(g->module.m) != SCORE_MODE) {
                set_menu_mode(g->module.m, SCORE_MODE);
              } else if (get_menu_mode(g->module.m) == SCORE_MODE) {
                set_menu_mode(g->module.m, DEFAULT_MENU_MODE);
              }
            } else if (current_selection == EXIT_MENU_SELECTION) {
              g->running = false;
            }
            break;
        }
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES ||
               event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
      if (g->module.m->mode == GAME_MODE) {
        g->mx = event.mouse.x;
        g->my = event.mouse.y;
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY) {
      if (g->module.m->mode == GAME_MODE) {
        g->is_ready_to_hit = false;
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
      switch (g->module.m->mode) {
        case GAME_MODE:
          g->mx = event.mouse.x;
          g->my = event.mouse.y;

          // update button
          button_update(g->module.new_game_button,
                        point_init(g->mx, g->my, 0), true);
          button_update(g->module.back_return_button,
                        point_init(g->mx, g->my, 0), true);

          // update balls
          if (is_mouse_on_ball(g->module.balls[0], g->mx, g->my)) {
            g->start_x = event.mouse.x;
            g->start_y = event.mouse.y;

            g->is_ready_to_hit = true;
          }
          break;
        case MENU_MODE:
          break;
      }
    } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
      if (g->module.m->mode == MENU_MODE) {

      } else if (g->module.m->mode == GAME_MODE) {
        const int mx = event.mouse.x;
        const int my = event.mouse.y;

        // update button
        button_update(g->module.new_game_button, point_init(mx, my, 0), false);
        button_update(g->module.back_return_button, point_init(mx, my, 0), false);

        // if the mouse is not on the ball after the dragging
        // its ready to shoot the ball
        if (!is_mouse_on_ball(g->module.balls[0], mx, my) &&
            g->is_ready_to_hit) {
          g->end_x = mx;
          g->end_y = my;

          g->dx = g->end_x - g->start_x;
          g->dy = g->end_y - g->start_y;

          set_velocity_to_ball(g->module.balls, 0,
                               g->dx * BALL_DELTA_SCALE,
                               g->dy * BALL_DELTA_SCALE);

          // clear the original status memory first
          // get the original status
          if (g->ball_original_status != NULL) {
            free(g->ball_original_status);
            g->ball_original_status = NULL;
          }

          g->ball_original_status = get_ball_status(g->module.balls);

          // after the velocity is set
          // balls are moving
          g->is_ball_hitted = true;
        }
        g->is_ready_to_hit = false;
      }
    } else if (event.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {
      al_acknowledge_resize(g->core.display);
      background_resize(g->module.bg,
                        size_init(event.display.width, event.display.height, 0));
      g->window_width = event.display.width;
      g->window_height = event.display.height;
    }

    if (g->redraw && al_is_event_queue_empty(g->core.event_queue)) {
      // clear the screen to black
      al_clear_to_color(al_map_rgb(0, 0, 0));

      switch (g->module.m->mode) {
        case MENU_MODE:
          draw_menu(g->module.m);
          break;
        case GAME_MODE:
          // drawings

          // draw the background first
          background_draw(g->module.bg);
          // draw the score board
          /*draw_score_board(g->module.board);*/

          // TODO
          // draw 2 buttons
          button_draw(g->module.new_game_button);
          button_draw(g->module.back_return_button);

          // draw mouse coordinates
          draw_mouse_coordinates(point_init(g->mx, g->my, 0),
                                 size_init(g->window_width, g->window_height, 0),
                                 g->mouse_font);

          if (g->is_ready_to_hit)
            draw_referencing_line(g->module.balls[0].cx,
                                  g->module.balls[0].cy,
                                  g->mx, g->my);

          /*draw_billiard_balls(g->module.balls);*/
          break;
      }

      // flip the display
      al_flip_display();
      g->redraw = false;
    }
  }
}

