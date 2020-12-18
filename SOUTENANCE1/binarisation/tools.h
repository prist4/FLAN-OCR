#ifndef TOOLS_H_
#define TOOLS_H_

#include <stdlib.h>
#include <SDL.h>

int isColored(Uint32 pixel, SDL_Surface* image, int r1 , int g1, int b1);
void ColorLine(SDL_Surface* image, int x, int width, int r, int g, int b);
void ColorSemiColumn(SDL_Surface* image, int y, int h1, int h2, int r, int g, int b);
int toBin(SDL_Surface* image);

#endif