#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "segment_lines.h"
#include "segment_char.h"
#include "segment_hori_char.h"
#include "tools.h"


void Init_SDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1 , "ERROR on Init SDL > %s \n", SDL_GetError());
}


char* Get_ImgPath(int argc , char ** argv)
{
    if (argc == 1)
        errx(1,"Need argument : path of image to segment.\n");
    if (argc > 2)
        errx(1,"To many argument : only need path of image to segment.\n");

    return argv[1];
}


SDL_Surface* Load_bmp(char* Path)
{
    SDL_Surface* img;

    img = SDL_LoadBMP(Path);
    if (img == NULL)
        errx(1,"ERROR : load BMP failed > %s",SDL_GetError());

    return img;
}


int main(int argc, char** argv)
{
    //_________________INIT VALUE________________
    SDL_Surface* img_surface;

    //________________GET IMG PATH_______________
    char* Path = Get_ImgPath(argc,argv);

    //__________________INIT SDL_________________
    Init_SDL();

    //__________________LOAD IMG_________________
    img_surface = Load_bmp(Path);

    if( img_surface == NULL)
        printf("ERROR : Failed to load img BMP");


    //______________Segmentation_________________
    Segment_lines(img_surface);
    Segment_char(img_surface);
    Segment_hori_char(img_surface);


    //______________Save new image________________
    SDL_SaveBMP(img_surface,"img/result/newImage.bmp");

    //_______________FREE SURFACE________________
    SDL_FreeSurface(img_surface);


    return 0;
}
