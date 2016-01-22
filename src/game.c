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
}

void game_main_loop(game *g) {

}

