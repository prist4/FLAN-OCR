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

void DrawRectangle(SDL_Surface *image, Uint32 pixel, int a, int b, int c, int d)
{
    for (int i = a; i < b; i++)
    {
        for (int j = c; j < d; j++)
        {
            put_pixel(image,i,j,pixel);
        }
    }
}

void MatrixGenChar(SDL_Surface *image, int num, int i, int j)
{
    char n[10];
    char path[] = "char/";
    sprintf(n, "%d", num);
    strcat(path,n);
    FILE *file;
    file = fopen (path, "w");

    int large,high;
    Uint32 pixel = get_pixel(image,i,j);
    for (int k = 1; ; k++)
    {
        pixel = get_pixel(image,i+k,j);
        if (isColored(pixel,image,255,100,255))
        {
            large = k;
            break;
        }
    }
    for (int k = 0; ; k++)
    {
        pixel = get_pixel(image,i,k+j);
        if (isColored(pixel,image,100,150,220))
        {
            high = k;
            break;
        }
    }
    
    for (int y = 1; y < high; y++)
    {
        for (int x = 1; x < large; x++)
        {
            pixel = get_pixel(image,x+i,y+j);
            if (isColored(pixel, image, 0, 0, 0))
            {
                putc('1', file);
            }
            else
            {
                putc('0', file);
            }
        }
        putc('\n', file);
    }
    fclose(file);
}

int LineCharGenMatrix(SDL_Surface *image_surface, int start, int number)
{
    int width = image_surface->w;
    int height = image_surface->h;
    int startLine = 0;
    int countLine = 0;
    for (int i = 0; i < height; i++)
    {
        Uint32 pixel = get_pixel(image_surface, 0, i);
        if (isColored(pixel,image_surface,255,100,255))
        {
            countLine += 1;
            if (countLine == start)
            {
                startLine = i;
                break;
            }
        }
    }
    int num = 0;
    for (int j = 0; j < width; j++)
    {
        Uint32 pixel = get_pixel(image_surface,j,startLine+1);
        if (isColored(pixel,image_surface,255,100,255))
        {
            num +=1;
            MatrixGenChar(image_surface,number+num,j,startLine+1);
        }
    }
    return num;
}

int CountLines(SDL_Surface *image_surface)
{
    int height = image_surface->h;
    int count = 0;
    Uint32 pixel = get_pixel(image_surface,0,0);
    for (int i = 0; (i < height); i++)
    {
        pixel = get_pixel(image_surface,0,i);
        if (isColored(pixel,image_surface,255,100,255))
        {
            count += 1;
        }
    }
    return count;
}

void test(SDL_Surface *image_surface)
{
    int maxCount = CountLines(image_surface);
    int number = 0;
    for (int i = 0; i < maxCount; i++)
    {
        number += LineCharGenMatrix(image_surface,i,number);
    }
}

int main(int ac, char **nom)
{
    SDL_Surface *image_surface;
    init_sdl();
    char *str = nom[ac - 1];
    image_surface = load_image(str);

    test(image_surface);

    SDL_SaveBMP(image_surface, "out.bmp");
    SDL_FreeSurface(image_surface);
    return 0;
}
