#include "main.h"

// default font to display
const char* const GAME_FONT_FILE_PATH = "./res/font/Junicode.ttf";
// background music file path
const char* const GAME_BACKGROUND_MUSIC_PATH =
      "./res/sound/background_music.ogg";
// music gain
const double GAME_DEFAULT_MUSIC_GAIN = 1.6;

#define GAME_LOADING_TEXT "LOADING ..."


bool main_init_allegro_library() {
  /* allegro main library */
  if(!al_init()) {
    error_message("fail to initialize allegro library");
    return false;
  }
  /* inputs */
  if(!al_install_mouse()) {
    error_message("fail to install mouse for allegro");
    return false;
  }
  if(!al_install_keyboard()) {
    error_message("fail to install keyboard for allegro");
    return 0;
  }
  /* outputs */
  if(!al_install_audio()) {
    error_message("fail to install audio for allegro");
    return 0;
  }

  /* addon */
  if(!al_init_acodec_addon()) {
    error_message("fail to initialize audio codec addon for allegro");
    return 0;
  }
  al_init_font_addon();
  if(!al_init_ttf_addon()) {
    error_message("fail to initialize font addon for allegro");
    return 0;
  }
  if(!al_init_image_addon()) {
    error_message("fail to initialize image addon for allegro");
    return 0;
  }
  if(!al_init_native_dialog_addon()) {
    error_message("fail to initialize dialog addon for allegro");
    return 0;
  }
  if(!al_init_primitives_addon()) {
    error_message("fail to initialize primitive addon for allegro");
    return 0;
  }

  /* reserve sample for audio output */
  if(!al_reserve_samples(20)) {
    error_message("fail to reserve sample for audio output");
    return 1;
  }
  return 1;
}

game *main_init_game_object() {
  game *g = malloc(sizeof(game));

  // set default value
  g->window_width = DEFAULT_WINDOW_WIDTH;
  g->window_height = DEFAULT_WINDOW_HEIGHT;
  g->loading_font_size = WINDOW_WIDTH / 20;
  g->mouse_font_size = MOUSE_FONT_SIZE;

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
  al_set_new_display_flags(ALLEGRO_WINDOWED);
  al_set_new_display_flags(ALLEGRO_RESIZABLE);
  al_set_new_display_flags(ALLEGRO_OPENGL);

  // game display
  g->display = al_create_display(g->window_width, g->window_height);
  if (!g->display) {
    error_message("fail to create allegro display");
    al_destroy_font(g->loading_font);
    al_destroy_font(g->mouse_font);
    return NULL;
  }

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



int main(int argc, char *argv[]) {
  // allegro essential elements
  ALLEGRO_FONT *loading_font = NULL;
  ALLEGRO_FONT *mouse_font = NULL;
  ALLEGRO_FONT *button_font = NULL;
  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_TIMER *timer = NULL;
  ALLEGRO_EVENT event;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  ALLEGRO_SAMPLE *bg_music = NULL;
  ALLEGRO_SAMPLE_INSTANCE *bg_music_instance = NULL;
  ALLEGRO_BITMAP *new_game_pressed = NULL;
  ALLEGRO_BITMAP *new_game_released = NULL;
  ALLEGRO_BITMAP *back_pressed = NULL;
  ALLEGRO_BITMAP *back_released = NULL;
  // modules
  background *bg = NULL;
  billiard_ball *balls = NULL;
  menu *m = NULL;
  score_board *board = NULL;
  // button pressed
  bool ngbutton_pressed = false;
  bool bbutton_pressed = false;
  const int button_sx = 0;
  const int button_sy = 0;
  int button_sw = 0;
  int button_sh = 0;
  // redendering
  bool redraw = false;
  // iterator
  int i;
  // mode
  int mode = MENU_MODE;
  // dialog result
  int result;
  // mouse coordinates
  int mx = 0, my = 0;
  // hitting the ball
  bool is_ready_to_hit = false;
  bool is_ball_hitted = false;
  bool should_change_turn = false;
  int *ball_original_status = NULL;
  int *ball_new_status = NULL;
  int highest_score = 0, who_score_highest = 0;
  int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
  double dx = 0, dy = 0;
  const int who_str_size = 128;
  char who_win[who_str_size];

  // initialize necessary library
  if(!init()) return 1;

  fprintf(stdout, "loading...\n");
  // reserve amount of samples to 20
  // there are 16 balls with different collision sound track
  // and 1 background music
  if(!al_reserve_samples(20))
  {
    perror("fail to reserve samples");
    return 1;
  }
  // load font
  if(!(loading_font = al_load_font(
          FONT_FPATH, LOADING_FONT_SIZE, ALLEGRO_TTF_NO_KERNING)))
  {
    perror("fail to load loading font");
    return 1;
  }
  if(!(mouse_font = al_load_font(
          FONT_FPATH, MOUSE_FONT_SIZE, ALLEGRO_TTF_NO_KERNING)))
  {
    perror("fail to load mouse font");
    al_destroy_font(loading_font);
    return 1;
  }

  if(!(button_font = al_load_font(
          FONT_FPATH, BUTTON_FONT_SIZE, ALLEGRO_TTF_NO_KERNING)))
  {
    perror("fail to load button font");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    return 1;
  }

  al_set_new_display_flags(ALLEGRO_WINDOWED);
  al_set_new_display_flags(ALLEGRO_OPENGL);
  al_set_new_display_flags(ALLEGRO_RESIZABLE);
  // initialize core gui objects
  if(!(display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT)))
  {
    perror("fail to create allegro display");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    return 1;
  }

  if(!(timer = al_create_timer(1.0 / FPS)))
  {
    perror("fail to create timer");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    return 1;
  }
  if(!(event_queue = al_create_event_queue()))
  {
    perror("fail to create event queue");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    return 1;
  }
  // draw the loading text to screen
  al_clear_to_color(COLOR_BLACK);
  al_draw_text(loading_font, COLOR_WHITE,
      WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 30, ALLEGRO_ALIGN_CENTER,
      "LOADING");
  al_flip_display();
  // and load the rest of the objects

  // background music
  if(!(bg_music = al_load_sample(BG_MUSIC_FPATH)))
  {
    perror("fail to load background music");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    return 1;
  }
  if(!(bg_music_instance = al_create_sample_instance(bg_music)))
  {
    perror("fail to create background sample instance");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    return 1;
  }
  if(!al_set_sample_instance_playmode(bg_music_instance, ALLEGRO_PLAYMODE_LOOP))
  {
    perror("fail to set background music playmode");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    return 1;
  }
  if(!al_set_sample_instance_gain(bg_music_instance, MUSIC_GAIN))
  {
    perror("fail to set background music volume");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    return 1;
  }
  // attach the music sample instance to the default mixer
  if(!al_attach_sample_instance_to_mixer(bg_music_instance,
        al_get_default_mixer()))
  {
    perror("fail to attach background music to mixer");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    return 1;
  }
  if(!(new_game_pressed = al_load_bitmap(BP_BITMAP)))
  {
    perror("fail to load button pressed bitmap");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    return 1;
  }
  if(!(new_game_released = al_load_bitmap(BR_BITMAP)))
  {
    perror("fail to load button released bitmap");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    return 1;
  }
  if(!(back_pressed = al_load_bitmap(BP_BITMAP)))
  {
    perror("fail to load button pressed bitmap");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    al_destroy_bitmap(new_game_released);
    return 1;
  }
  if(!(back_released = al_load_bitmap(BR_BITMAP)))
  {
    perror("fail to load button released bitmap");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    al_destroy_bitmap(new_game_released);
    al_destroy_bitmap(back_pressed);
    return 1;
  }
  // get the button width/height
  button_sw = al_get_bitmap_width(new_game_pressed);
  button_sh = al_get_bitmap_height(new_game_pressed);
  // background
  if(!(bg = create_background()))
  {
    perror("fail to create background");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    al_destroy_bitmap(new_game_released);
    al_destroy_bitmap(back_pressed);
    al_destroy_bitmap(back_released);
    return 1;
  }
  // billiard balls
  if(!(balls = create_billiard_balls()))
  {
    perror("fail to create billiard balls");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    al_destroy_bitmap(new_game_released);
    al_destroy_bitmap(back_pressed);
    al_destroy_bitmap(back_released);
    destroy_background(bg);
    return 1;
  }

  if(!(m = create_menu()))
  {
    perror("fail to create menu");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    al_destroy_bitmap(new_game_released);
    al_destroy_bitmap(back_pressed);
    al_destroy_bitmap(back_released);
    destroy_background(bg);
    destroy_billiard_balls(balls);
    return 1;
  }
  if(!(board = create_score_board()))
  {
    perror("fail to create menu");
    al_destroy_font(loading_font);
    al_destroy_font(mouse_font);
    al_destroy_font(button_font);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_sample(bg_music);
    al_destroy_sample_instance(bg_music_instance);
    al_destroy_bitmap(new_game_pressed);
    al_destroy_bitmap(new_game_released);
    al_destroy_bitmap(back_pressed);
    al_destroy_bitmap(back_released);
    destroy_background(bg);
    destroy_billiard_balls(balls);
    destroy_menu(m);
    return 1;
  }

  // register event source
  al_register_event_source(event_queue,
      al_get_display_event_source(display));
  al_register_event_source(event_queue,
      al_get_mouse_event_source());
  al_register_event_source(event_queue,
      al_get_keyboard_event_source());
  al_register_event_source(event_queue,
      al_get_timer_event_source(timer));
  fprintf(stdout, "done loading.\n");

  // start the background music
  al_play_sample_instance(bg_music_instance);
  // start timer
  al_start_timer(timer);
  // main loop
  while(1)
  {
    // listen to event
    al_wait_for_event(event_queue, &event);
    if(event.type == ALLEGRO_EVENT_TIMER)
    {
      redraw = true;

      switch(mode)
      {
        case MENU_MODE:
          break;
        case GAME_MODE:
          update_billiard_balls(balls);

          // if the balls are moving
          // due to user input
          if(is_ball_hitted && is_all_ball_stopped(balls) &&
              ball_original_status != NULL)
          {
            // printf("checking status\n");
            should_change_turn = true;
            ball_new_status = get_ball_status(balls);
            for(i = 0 ; i < BALL_COUNT ; i ++)
            {
              if(ball_original_status[i] > ball_new_status[i])
              {
                // if the ball this player hitted in
                // is not the white ball,
                // it should NOT change turn
                if(i == 0)
                {
                  // set the white ball to default location
                  // cx = 600, cy = 400
                  balls[0].cx = WB_X;
                  balls[0].cy = WB_Y;
                  balls[0].is_on_table = 1;
                  // printf("resetting white ball\n");
                }
                else should_change_turn = false;
                add_score_to_score_board(board, balls[i].score);
              }
            }

            // if all ball(except white ball) are all in bags
            // this game is done
            if(is_all_ball_in_bag(balls))
            {
              memset(who_win, '\0', who_str_size);
              if(get_who_win(board) == SB_P1_TURN)
              {
#ifdef DEBUG
                printf("player 1 wins\n");
#endif
                strcpy(who_win, "Player 1 Win !!!");
              }
              else if(get_who_win(board) == SB_P2_TURN)
              {
#ifdef DEBUG
                printf("player 2 wins\n");
#endif
                strcpy(who_win, "Player 2 Win !!!");
              }
              else if(get_who_win(board) == SB_DRAW)
              {
#ifdef DEBUG
                printf("draw\n");
#endif
                strcpy(who_win, "Draw !!!");
              }

              al_rest(1.00);
              record_score(board);
              reset_billiard_balls(balls);

              if(get_round(board) == MAX_GAME_NUM)
              {
                highest_score = get_highest_player_score_of_3(board);
                who_score_highest = get_who_score_higher_of_3(board);

#ifdef DEBUG
                printf("%d got %i", who_score_highest, 
                    highest_score);
#endif
                save_score_to_config(board, 
                    who_score_highest, highest_score);

                // prompt the user to continue or not
                result = al_show_native_message_box(display, 
                    "3 Game Finished", who_win, 
                    "Do you want to Play it Again?",
                    NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);

                if(result != 1)
                {
                  mode = MENU_MODE;
                  set_menu_mode(m, DEFAULT_MENU_MODE);
                }

                reset_billiard_balls(balls);
                reset_score_board(board);
              }
            }
            else
            {
              // check if its necessary to change turn
              // if the player didnt hitted in any ball
              // or the player hitted in white ball
              // change turn
              if(should_change_turn ||
                  ball_original_status[0] > ball_new_status[0])
              {
#ifdef DEBUG
                printf("change turn\n");
#endif
                change_turn(board);
              }
            }

            // reset is_ball_hitted boolean
            // so the ball can be hitted again
            is_ball_hitted = false;
            // free the used memory
            if(ball_new_status != NULL)
            {
              free(ball_new_status);
              ball_new_status = NULL;
            }
            if(ball_original_status != NULL)
            {
              free(ball_original_status);
              ball_original_status = NULL;
            }
          }
          break;
      }
    }
    else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
    {
      result = al_show_native_message_box(display,
          "Quit?", "Quit?", "Are you sure you want to Quit?",
          NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
      // if the result code == 1
      // means user click ok
      // break the main loop and exit
      if(result == 1)
        break;
    }
    else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
    {
      if(mode == MENU_MODE)
      {
        int current_selection = get_menu_selection(m);
        switch(event.keyboard.keycode)
        {
          case ALLEGRO_KEY_UP:
            if(current_selection > PLAY_MENU_SELECTION)
              set_menu_selection(m, current_selection - 1);
            break;
          case ALLEGRO_KEY_DOWN:
            if(current_selection < EXIT_MENU_SELECTION)
              set_menu_selection(m, current_selection + 1);
            break;
          case ALLEGRO_KEY_ENTER:
            if(current_selection == PLAY_MENU_SELECTION)
              mode = GAME_MODE;
            else if(current_selection == RULE_MENU_SELECTION)
            {
              if(get_menu_mode(m) != RULE_MODE)
                set_menu_mode(m, RULE_MODE);
              else if(get_menu_mode(m) == RULE_MODE)
                set_menu_mode(m, DEFAULT_MENU_MODE);
            }
            else if(current_selection == SCORE_MENU_SELECTION)
            {
              if(get_menu_mode(m) != SCORE_MODE)
                set_menu_mode(m, SCORE_MODE);
              else if(get_menu_mode(m) == SCORE_MODE)
                set_menu_mode(m, DEFAULT_MENU_MODE);
            }
            else if(current_selection == EXIT_MENU_SELECTION)
            {
              // exit procedure
              // stop playing the music
              al_stop_sample_instance(bg_music_instance);
              // free the used memory
              al_destroy_font(loading_font);
              al_destroy_font(mouse_font);
              al_destroy_font(button_font);
              al_destroy_display(display);
              al_destroy_timer(timer);
              al_destroy_event_queue(event_queue);
              al_destroy_sample(bg_music);
              al_destroy_sample_instance(bg_music_instance);
              al_destroy_bitmap(new_game_pressed);
              al_destroy_bitmap(new_game_released);
              al_destroy_bitmap(back_pressed);
              al_destroy_bitmap(back_released);
              destroy_background(bg);
              destroy_billiard_balls(balls);
              destroy_menu(m);
              destroy_score_board(board);
              // exit program with code 0 (OK)
              exit(0);
            }
            break;
        }
      }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_AXES ||
        event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
    {
      if(mode == GAME_MODE)
      {
        mx = event.mouse.x;
        my = event.mouse.y;
      }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY)
    {
      if(mode == GAME_MODE)
      {
        is_ready_to_hit = false;
      }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
      if(mode == GAME_MODE)
      {
        if(is_mouse_on_ball(balls[0], mx, my))
        {
          start_x = event.mouse.x;
          start_y = event.mouse.y;

          is_ready_to_hit = true;
        }

        // if the mouse press on the buttons
        if(event.mouse.x > NGB_X_POS && 
            event.mouse.x < NGB_X_POS + NGB_WIDTH &&
            event.mouse.y > NGB_Y_POS &&
            event.mouse.y < NGB_Y_POS + NGB_HEIGHT)
        {
          printf("new game button pressed\n");
          reset_billiard_balls(balls);
          reset_score_board(board);

          ngbutton_pressed = true;
        }
        if(event.mouse.x > BB_X_POS && 
            event.mouse.x < BB_X_POS + BB_WIDTH &&
            event.mouse.y > BB_Y_POS &&
            event.mouse.y < BB_Y_POS + BB_HEIGHT)
        {
          bbutton_pressed = true;
          printf("back button pressed\n");
          mode = MENU_MODE;
          reset_billiard_balls(balls);
          reset_score_board(board);
          set_menu_mode(m, DEFAULT_MENU_MODE);

          bbutton_pressed = false;
        }
      }
    }
    else if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
    {
      if(mode == MENU_MODE)
      {

      }
      else if(mode == GAME_MODE)
      {
        // if the mouse is not on the ball after the dragging
        // its ready to shoot the ball
        if(!is_mouse_on_ball(balls[0], event.mouse.x, event.mouse.y) &&
            is_ready_to_hit)
        {
          end_x = event.mouse.x;
          end_y = event.mouse.y;

          dx = end_x - start_x;
          dy = end_y - start_y;

          set_velocity_to_ball(balls, 0, 
              dx * DELTA_SCALE, dy * DELTA_SCALE);

          // clear the original status memory first
          // get the original status
          if(ball_original_status != NULL)
          {
            free(ball_original_status);
            ball_original_status = NULL;
          }
          ball_original_status = get_ball_status(balls);

          // after the velocity is set
          // balls are moving
          is_ball_hitted = true;
        }
        is_ready_to_hit = false;

        ngbutton_pressed = false;
        bbutton_pressed = false;
      }
    }

    if(redraw && al_is_event_queue_empty(event_queue))
    {
      // clear the screen to black
      al_clear_to_color(COLOR_BLACK);

      switch(mode)
      {
        case MENU_MODE:
          draw_menu(m);
          break;
        case GAME_MODE:
          // drawings

          // draw the background first
          draw_background(bg);
          // draw the score board
          draw_score_board(board);

          // draw 2 buttons
          if(ngbutton_pressed)
          {
            al_draw_scaled_bitmap(new_game_pressed,
                button_sx, button_sy, button_sw, button_sh,
                NGB_X_POS, NGB_Y_POS, NGB_WIDTH, NGB_HEIGHT, 0);
            al_draw_text(button_font, BP_COLOR, NGB_X_POS, NGB_Y_POS,
                ALLEGRO_ALIGN_LEFT, NEW_GAME_TEXT);
          }
          else
          {
            al_draw_scaled_bitmap(new_game_released,
                button_sx, button_sy, button_sw, button_sh,
                NGB_X_POS, NGB_Y_POS, NGB_WIDTH, NGB_HEIGHT, 0);
            al_draw_text(button_font, BR_COLOR, NGB_X_POS, NGB_Y_POS,
                ALLEGRO_ALIGN_LEFT, NEW_GAME_TEXT);
          }
          if(bbutton_pressed)
          {
            al_draw_scaled_bitmap(back_pressed,
                button_sx, button_sy, button_sw, button_sh,
                BB_X_POS, BB_Y_POS, BB_WIDTH, BB_HEIGHT, 0);
            al_draw_text(button_font, BP_COLOR, BB_X_POS, BB_Y_POS,
                ALLEGRO_ALIGN_LEFT, "Back");
          }
          else
          {
            al_draw_scaled_bitmap(back_released,
                button_sx, button_sy, button_sw, button_sh,
                BB_X_POS, BB_Y_POS, BB_WIDTH, BB_HEIGHT, 0);
            al_draw_text(button_font, BR_COLOR, BB_X_POS, BB_Y_POS,
                ALLEGRO_ALIGN_LEFT, "Back");
          }

          // draw mouse coordinates
          draw_mouse_coordinates(mx, my, mouse_font);

          if(is_ready_to_hit)
            draw_referencing_line(balls[0].cx, balls[0].cy, mx, my);

          draw_billiard_balls(balls);
          break;
      }
      // flip the display
      al_flip_display();
      redraw = false;
    }
  }

  // exit procedure
  // stop playing the music
  al_stop_sample_instance(bg_music_instance);
  // free the used memory
  al_destroy_font(loading_font);
  al_destroy_font(mouse_font);
  al_destroy_font(button_font);
  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(event_queue);
  al_destroy_sample(bg_music);
  al_destroy_sample_instance(bg_music_instance);
  al_destroy_bitmap(new_game_pressed);
  al_destroy_bitmap(new_game_released);
  al_destroy_bitmap(back_pressed);
  al_destroy_bitmap(back_released);
  destroy_background(bg);
  destroy_billiard_balls(balls);
  destroy_menu(m);
  destroy_score_board(board);
  return 0;
}

int init()
{
  if(!al_init())
  {
    perror("fail to initialize allegro library");
    return 0;
  }
  if(!al_install_mouse())
  {
    perror("fail to install mouse");
    return 0;
  }
  if(!al_install_keyboard())
  {
    perror("fail to install keyboard");
    return 0;
  }
  if(!al_install_audio())
  {
    perror("fail to install audio");
    return 0;
  }

  if(!al_init_acodec_addon())
  {
    perror("fail to initialize allegro acodec addon");
    return 0;
  }
  al_init_font_addon();
  if(!al_init_ttf_addon())
  {
    perror("fail to initialize allegro ttf addon");
    return 0;
  }
  if(!al_init_image_addon())
  {
    perror("fail to initialize allgro image addon");
    return 0;
  }
  if(!al_init_native_dialog_addon())
  {
    perror("fail to initialize allegro native dialog");
    return 0;
  }
  if(!al_init_primitives_addon())
  {
    perror("fail to initialize allegro primitives addon");
    return 0;
  }
  return 1;
}

void draw_mouse_coordinates(const int mx, const int my, const ALLEGRO_FONT *font) {
  char text[20] = {'\0'};
  sprintf(text, "[%03d,%03d]", mx, my);
  al_draw_text(font, COLOR_WHITE, MX, MY, ALLEGRO_ALIGN_LEFT, text);
}

void draw_referencing_line(const int sx, const int sy, const int cx, const int cy) {
  const int tborder_x1 = BORDER_WIDTH;
  const int tborder_x2 = TABLE_WIDTH - BORDER_WIDTH;
  const int tborder_y1 = TABLE_Y_POSITION + BORDER_WIDTH;
  const int tborder_y2 = TABLE_Y_POSITION + TABLE_HEIGHT - BORDER_WIDTH;
  const int dx = sx - cx;
  const int dy = sy - cy;
  int line_x = sx, line_y = sy;
  double delta;

  if(dx == 0 && dy == 0)
    return;

  while(1)
  {
    line_x += dx;
    line_y += dy;
    if(line_x <= tborder_x1 || line_x >= tborder_x2)
    {
      line_x = (line_x <= tborder_x1 ? tborder_x1 : tborder_x2);
      delta = 1.0 * (line_x - sx) / dx;
      line_y = sy + (int) (dy * delta);
      break;
    }
    if(line_y <= tborder_y1 || line_y >= tborder_y2)
    {
      line_y = (line_y <= tborder_y1 ? tborder_y1 : tborder_y2);
      delta = 1.0 * (line_y - sy) / dy;
      line_x = sx + (int) (dx * delta);
      break;
    }
  }

  al_draw_line(line_x, line_y, cx, cy, COLOR_WHITE, RL_WIDTH);
}


#undef GAME_LOADING_TEXT
