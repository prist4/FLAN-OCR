#include <err.h>
#include "pixel_fonction.h"
#include "tools.h"

static inline
int Is_line_black(SDL_Surface* img , int x)
{
    int y;
    int is_black;
    int weight;
    Uint32 pixel;
    Uint8 r , g , b;

    weight = img -> w;
    y = 0;
    is_black = 0;
    while (y < weight && is_black == 0)
    {
        pixel = get_pixel(img,y,x);
        SDL_GetRGB(pixel, img->format, &r, &g, &b);

        if (r < 1 && g < 1 && b < 1)
            is_black = 1;

        ++y;
    }

    return is_black;
}

static inline
void Changes_tab(SDL_Surface* img , int* Tab)
{
    int heigth = img -> h;

    if (Tab == NULL)
        printf("tab == null");

    for (int i = 0 ; i < heigth ; ++i)
    {
        if (Is_line_black(img,i))
            Tab[i] = 1;
    }


}



static inline
void Draw_Lines(SDL_Surface* img,int* Tab)
{
    int h = img -> w;
    int w = img -> h;
    int Last_nb;


    Last_nb = 0;

    for (int i = 1 ; i < w ; ++i)
    {
        if (Last_nb != Tab[i])
        {
            if(Tab[i] == 1)
                Draw_HorizontalLine(img,0,h,i-1,255,100,255);
            else
                Draw_HorizontalLine(img,0,h,i,100,150,220);
        }
        Last_nb = Tab[i];
    }

}


void Segment_lines(SDL_Surface* img)
{
    //____________Init Value__________
    int* tab_BorW;
    int heigth = img -> h;

    //Create a tab of the size of heigth
    tab_BorW = calloc(heigth,sizeof(int));

    //Changes the cells of the tabs if its a texted lines.
    Changes_tab(img , tab_BorW);

    //_____________Treament___________
    Draw_Lines(img,tab_BorW);


    free(tab_BorW);
}
