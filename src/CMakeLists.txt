set(ALLEGRO_LIBRARY
    -lallegro -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf
    -lallegro_image -lallegro_dialog -lallegro_primitives -lm)

set(PROJECT_SRC background.c ball.c main.c menu.c score_board.c)

add_executable(billiard_ball ${PROJECT_SRC})
target_link_libraries(billiard_ball ${ALLEGRO_LIBRARY})
