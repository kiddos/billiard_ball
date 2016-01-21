CC=gcc
LIB=-lallegro -lallegro_audio -lallegro_acodec					\
	-lallegro_font -lallegro_ttf -lallegro_image				\
	-lallegro_dialog -lallegro_primitives  -lm
CCFLAG=-g -Wall -O3
DEBUGFLAG=-DDEBUG
DEPEN=main.o background.o ball.o menu.o score_board.o 


UNAME := $(shell uname)
ifeq (($UNAME), Linux)
	
endif

all: main

main: clear clean $(DEPEN)
	$(CC) $(CCFLAG) $(DEPEN) $(LIB) -o ../$@

debug: clear clean $(DEPEN)
	$(CC) $(CCFLAG) $(DEPEN) $(LIB) $(DEBUGFLAG) -o ../$@

main.o: main.c
	$(CC) $(CCFLAG) -c main.c

background.o: background.c
	$(CC) $(CCFLAG) -c background.c

ball.o: ball.c
	$(CC) $(CCFLAG) -c ball.c

menu.o: menu.c
	$(CC) $(CCFLAG) -c menu.c

score_board.o: score_board.c
	$(CC) $(CCFLAG) -c score_board.c

clear:
	clear

clean:
	rm -f *.o

.PHONY:
	debug main clean clear all

.INTERMEDIATE:
	$(DEPEN)
