#include <err.h>
#include "pixel_fonction.h"
#include "tools.h"

int Is_char_zone(SDL_Surface* img, int x, int y)
// Check if in (x,y) we are on the beginning of a char zone.
{
    Uint8 r , g , b;
	Uint32 pixel = get_pixel(img, y, ++x);

	SDL_GetRGB(pixel, img -> format, &r, &g, &b); 

	if (r == 255 && g == 100 && b == 255)
		return 1;

	return 0;
}

static inline
int Is_line_black(SDL_Surface* img , int x, int y, int width)
{
    int is_black = 0;
    Uint32 pixel;
    int i = y;
    Uint8 r , g , b;

    while (i < y+width && is_black == 0)
    {
        pixel = get_pixel(img,i,x);
        SDL_GetRGB(pixel, img->format, &r, &g, &b);

        if (r != 255 && g != 255 && b != 255)
            is_black = 1;

        ++i;
    }


    return is_black;
}


int Get_width_char(SDL_Surface* img, int x, int y)
//Get the width of a char zone. Return the width.
{
	Uint32 pixel;
    Uint8 r , g , b;
    int i = y;
    int res = 0;

    while (i < img -> w && !res)
    {
    	pixel = get_pixel(img,i,x);
		SDL_GetRGB(pixel, img -> format, &r, &g, &b); 
		if (r == 100 && g == 150 && b == 220)
			res = 1;
		else
    		++i;
    }
    return i - y;
}

int Get_heigth_char(SDL_Surface* img, int x, int y)
//Get the heigth of a char zone. Return the heigth.
{
	Uint32 pixel;
    Uint8 r , g , b;
    int i = x;
    int res = 0;

    while (i < img -> h && !res)
    {
    	pixel = get_pixel(img,y,i);
		SDL_GetRGB(pixel, img -> format, &r, &g, &b); 
		if (r == 100 && g == 150 && b == 220)
			res = 1;
		else
    		++i;
    }
    return i - x;
}

static inline
void Change_tab(SDL_Surface* img, int* Tab, int x, int y, int h, int w)
//Put 0 and 1 in case case of the tab depending if its a char line.
{
	for (int i = 0; i < h; ++i)
	{
		if (Is_line_black(img, x + i, y, w))
		{
			Tab[i] = 1;
		}

	}
}

void Draw_Lines(SDL_Surface* img,int* Tab, int x, int y, int width, int height)
{
    int Last_nb = 0;
	
    for (int i = x ; i < height + x ; ++i)
    {
        if (Last_nb != Tab[i - x])
        	Draw_HorizontalLine(img, y, y + width ,i-1,0,255,0);
        
        Last_nb = Tab[i-x];
    }
}

void Segment_redim_char(SDL_Surface* img, int x, int y, int height)
//re-draw the horizontal line that define a char zone.
{
    //Init values
	int* Tab_BorW;
	int width = Get_width_char(img,x,y);
	Tab_BorW = calloc(height,sizeof(int));

    //Change value of Tab_BorW
	Change_tab(img, Tab_BorW, x, y, height, width);

    //Update Image
	Draw_Lines(img, Tab_BorW, x, y, width, height);

	free(Tab_BorW);
}


void Segment_hori_char(SDL_Surface* img) 
// Search charactere zone and modifie 
//the zone to be the perfect height and witdth of a char.
{
	int width  = img -> w;
	int height = img -> h;
	Uint32 pixel;
    Uint8 r , g , b;
    int h;


	for (int x = 0; x < height - 1 ; ++x)		
	{
		pixel = get_pixel(img,0,x);
		SDL_GetRGB(pixel, img -> format, &r, &g, &b); 	

		if (r == 255 && g == 100 && b == 255)
		{
			//In char line zone
			h = Get_heigth_char(img,x,0);
			for (int y = 0; y < width ; ++y)
			{
				if (Is_char_zone(img, x, y))
					Segment_redim_char(img,x,y,h);
			}	
		}		
	}
}
