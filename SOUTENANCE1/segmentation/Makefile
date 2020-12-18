# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -pedantic-errors -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: segmentation

segmentation : segmentation.o segment_lines.o pixel_fonction.o segment_char.o segment_hori_char.o tools.o 


segmentation.o : segment_lines.h pixel_fonction.h segment_char.h segment_hori_char.h tools.h 

clean:
	${RM} *.o
	${RM} *.d
	#${RM} segmentation
	${RM} *~
	${RM} '*'


# END
