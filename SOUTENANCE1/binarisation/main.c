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

int main(int ac, char **nom)
{
    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;

    init_sdl();
    //test is arg error
    if (ac != 3)
        errx(1,"wrong arg number, expected : 2\n format : <image_jpg> <bin_methode>");
    char *str = nom[ac - 2];
    /*
    char *str2 = "./images/";
    strcat(str2,str);
    */
    image_surface = load_image(str);
    screen_surface = display_image(image_surface);
    wait_for_keypressed();

    int width = image_surface->w;
    int height = image_surface->h;

// BINARISATION
    double aveR = 0, aveG = 0, aveB = 0;
    int black = 255;
    int white = 0;

    //determine average
    if (strcmp(nom[ac - 1],"0"))
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Uint32 pixel = get_pixel(image_surface, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                aveR += r;
                aveG += g;
                aveB += b;
            }
        }
        aveR = aveR / (width * height);
        aveG = aveG / (width * height);
        aveB = aveB / (width * height);

// METHODE SIMPLE
    //use 127 as average rgb value
        if ((aveR+aveG+aveB)/3 < 127)
        {
            black = 0;
            white = 255;
        }
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                Uint32 pixel = get_pixel(image_surface, i, j);
                Uint8 r, g, b;
                SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
                if ((r+g+b+aveR+aveG+aveB)/6 > 127)
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
    else
    {
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
                Uint32 pixel = get_pixel(image_surface, i, j);
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
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

//SEGMENTATION HORIZONTALE
    //color full white line
    int lc;
    for (int y = 0; y < height; y++)
    {

        lc = 0;
        for (int x = 0; x < width; x++)
        {
            Uint32 pixel = get_pixel(image_surface, x, y);
            if (isColored(pixel, image_surface, 0, 0, 0))
            {
                lc = 1;
                break;
            }
        }
        if (lc == 0)
        {
            ColorLine(image_surface, y, width, 255, 0, 30);
        }
    }

//SEGMENTATION VERTICALE
    //color between character and horizontal line
    int lastRed = 0;
    int wascolored;
    int cr;
    for (int x = 0; x < width; x++)
    {
        cr = 0;
        Uint32 pixel = get_pixel(image_surface, x, 0);
        if (isColored(pixel, image_surface, 255, 0, 30))
        {
            wascolored = 0;
        }
        else
        {
            wascolored = 1;
        }
        for (int y = 0; y < height; y++)
        {

            Uint32 pixel = get_pixel(image_surface, x, y);
            if (isColored(pixel, image_surface, 0, 0, 0) || (cr == 1 && wascolored == 0))
            {
                cr = 1;
            }
            else
            {
                cr = 0;
            }

            int red = isColored(pixel, image_surface, 255, 0, 30);

            if (red == 1 && wascolored == 1)
            {
                ;
            }
            else if (red == 0 && wascolored == 1)
            {
                lastRed = y;
                wascolored = 0;
            }
            else if ((red == 1 && wascolored == 0) || y + 1 == height)
            {
                if (cr == 0)
                {
                    ColorSemiColumn(image_surface, x, lastRed, y + 1, 255, 0, 30);
                }
                wascolored = 1;
            }
            else if (red == 0 && wascolored == 0)
            {
                ;
            }
        }
    }
    //wait key to free sdl memory
    update_surface(screen_surface, image_surface);
    wait_for_keypressed();
    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}
