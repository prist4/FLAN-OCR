# include "image_operations.h"


/* Transform image in Binary Image */
void binarization(SDL_Surface *img)
{
    /* Variables */
    Uint32 pixel;
    Uint8 r,g,b;
    int w;
    int h;
    w = img -> w;
    h = img -> h;

    // Iterate on each pixels and blacknwhite 
    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            pixel = getpixel(img,i,j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if(r >= 127 && g >= 127 && b >= 127)
            {
                r = 255;
                g = 255;
                b = 255;
            }
            else
            {
                r = 0;
                g = 0;
                b = 0;
            }
            pixel = SDL_MapRGB(img->format, r, g, b);
            putpixel(img, i, j, pixel);
        }
    }
}


SDL_Surface* increaseChar(SDL_Surface *img)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,28,28,img->format->BitsPerPixel,0,0,0,0);
    SDL_SoftStretch(img, NULL, dest, NULL);
    return dest;
}
SDL_Surface* Resize(SDL_Surface *img)
{
    SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,576,460,img->format->BitsPerPixel,0,0,0,0);
    SDL_SoftStretch(img, NULL, dest, NULL);
    return dest;
}
