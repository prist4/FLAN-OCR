#include <err.h>
#include "pixel_fonction.h"
#include "tools.h"

static inline
int Is_Black_column(SDL_Surface* img , int line , int column, int hmax)
{
    int i = line;
    int is_black =  0;
    int count_black = 0;
    Uint32 pixel;
    Uint8 r , g , b;


    while (i < hmax && is_black == 0)
    {
        pixel = get_pixel(img,column,i);
        SDL_GetRGB(pixel , img -> format , &r , &g , &b);

        if (r < 120 && g < 120 && b < 120 )
        {
            if (count_black > 0)
                is_black = 1;
            else
                ++count_black;
        }

        ++i;
    }

    return is_black;
}



static inline
void Change_tab(SDL_Surface* img , int* Tab , int line , int hmax)
{

    for (int i = 0 ; i < img -> w ; ++i )
    {
        if (Is_Black_column(img , line , i , hmax))
            Tab[i] = 1;
    }
}


static inline
int Get_PosBlueLine(SDL_Surface* img , int line)
{
    Uint32 pixel ;
    Uint8 r , g , b ;
    int i = line;


    while (i < img -> h)
    {
        pixel = get_pixel(img , 0 , i);
        SDL_GetRGB(pixel , img -> format , &r ,&g , &b);
        if (r == 100 && g == 150 && b == 220)
            return i;
        ++i;
    }

    return i;
}

static inline
void Draw_Lines(SDL_Surface* img , int* Tab , int line ,int  hmax)
{
    int w = img -> w;
    int Last_nb;


    Last_nb = 0;

    for (int i = 1 ; i < w ; ++i)
    {
        if (Last_nb != Tab[i])
        {
            if(Tab[i] == 1)
                Draw_VerticalLine(img,line,hmax,i-1,255,100,255);
            else
                Draw_VerticalLine(img,line,hmax,i,100,150,220);
        }
        Last_nb = Tab[i];
    }

}


void Segment_CharOnLine(SDL_Surface* img, int line)
{
    //____________Init Value__________
    int* Tab_BorW;

    int w = img -> w;
    int hmax;

    //Create a tab of the size of weigth
    Tab_BorW = calloc(w,sizeof(int));

    //Get the position were a blue line is drawed
    hmax = Get_PosBlueLine(img , line);

    //Changes cells of the tab if its a texted colomn.
    Change_tab(img,Tab_BorW,line,hmax);

    //Draw Lines
    Draw_Lines(img,Tab_BorW,line,hmax);

    free(Tab_BorW);
}



void Segment_char(SDL_Surface* img)
{
    Uint32 pixel;
    Uint8 r,g,b;

    for (int i = 0 ; i < img -> h ; ++i)
    {
        pixel = get_pixel(img,0,i);
        SDL_GetRGB(pixel , img -> format , &r , &g , &b);

        if (r == 255 && g == 100 && b == 255)
            Segment_CharOnLine(img , i);
    }
}


