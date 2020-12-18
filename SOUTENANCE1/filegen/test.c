#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include "display.h"
#include "tools.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void Binarisation(SDL_Surface image_surface)
{
    int width = image_surface->w;
    int height = image_surface->h;

 	// BINARISATION
    double aveR = 0, aveG = 0, aveB = 0;
    int black = 255;
    int white = 0;
    //determine average
	// METHODE OTSU
    //Otsu method that determine optimized rgb value
    int seuil = toBin(image_surface);
    if (seuil < 127)
    {
        black = 0;
        white = 255;
    }
    Uint8 r = 0, g = 0, b = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int32 pixel = get_pixel(image_surface, i, j);
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            if ((r + g + b) / 3 > seuil)
            {
                pixel = SDL_MapRGB(image_surface->format, black, black, black);
            }
            else
            {
                pixel = SDL_MapRGB(image_surface->format, white, white, white);
            }
            put_pixel(image_surface, i, j, pixel);
        }
    }
}