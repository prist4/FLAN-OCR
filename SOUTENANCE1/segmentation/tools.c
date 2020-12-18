#include <err.h>
#include "pixel_fonction.h"


void Draw_HorizontalLine(SDL_Surface* img, int y0 , int y1 , int x , int R , int G , int B)
{
    Uint32 pixel;
    Uint8 r,g,b ;
    int y ;

    for (int i = 0 ; i < y1 - y0 ; ++i)
    {
        y = i + y0;
        pixel = get_pixel(img,y,x);
        SDL_GetRGB(pixel , img -> format , &r , &g , &b);
        r = R ;
        g = G ;
        b = B ;
        pixel = SDL_MapRGB(img -> format, r, g, b);
        put_pixel(img , y , x , pixel);
    }
}


void Draw_VerticalLine(SDL_Surface* img, int x0 , int x1 , int y , int R , int G , int B)
{
    Uint32 pixel;
    Uint8 r,g,b ;
    int x ;

    for (int i = 0 ; i < x1 - x0 ; ++i)
    {
        x = i + x0;
        pixel = get_pixel(img,y,x);
        SDL_GetRGB(pixel , img -> format , &r , &g , &b);
        r = R ;
        g = G ;
        b = B ;
        pixel = SDL_MapRGB(img -> format, r, g, b);
        put_pixel(img , y , x , pixel);
    }
}

