# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -g
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: main

main: pixel_operations.o display.o tools.o main.o

main.o: pixel_operations.h display.h tools.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
