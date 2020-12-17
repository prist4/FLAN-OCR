// segmentation.h

# ifndef SEGMENTATION_H_
# define SEGMENTATION_H_

# include <stdlib.h>
# include <SDL/SDL.h>
# include <err.h>
# include "../Tools/tools.h"
# include "image_operations.h"
# include "detect_char.h"


SDL_Surface* lineCut(SDL_Surface *img);

void isolateLine(SDL_Surface *img, struct Neuronal_Network *net);

void cuttedSurface(SDL_Surface *img, int firstCut,
                    int lastCut, struct Neuronal_Network *net);

void charCut(SDL_Surface *img);

void isolateChar(SDL_Surface *img, struct Neuronal_Network *net);

# endif
