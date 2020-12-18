# include "image_operations.h"

int toBin(SDL_Surface *image)
{
    Uint8 r = 0, g = 0, b = 0, a = 0, average = 0;
    long double h[256];

    for (int i = 0; i < 256; i++)
    {
        h[i] = 0;
    }

    for (int i = 0; i < image->h; i++)
        for (int j = 0; j < image->w; j++)
        {
            Uint32 ip = getpixel(image, j, i);
            SDL_GetRGBA(ip, image->format, &r, &g, &b, &a);
            average = (r + g + b) / 3;
            h[average]++;
        }
    long double bin_edges[256];
    bin_edges[0] = 0.0;
    long double increment = 0.99609375;
    for (int i = 1; i < 256; i++)
        bin_edges[i] = bin_edges[i - 1] + increment;

    long double bin_mids[256];
    for (int i = 0; i < 256; i++)
        bin_mids[i] = (bin_edges[i] + bin_edges[i + 1]) / 2;

    long double weight1[256];
    weight1[0] = h[0];
    for (int i = 1; i < 256; i++)
        weight1[i] = h[i] + weight1[i - 1];

    int total_sum = 0;
    for (int i = 0; i < 256; i++)
        total_sum = total_sum + h[i];

    long double weight2[256];
    weight2[0] = total_sum;
    for (int i = 1; i < 256; i++)
        weight2[i] = weight2[i - 1] - h[i - 1];

    long double h_bin_mids[256];
    for (int i = 0; i < 256; i++)
        h_bin_mids[i] = h[i] * bin_mids[i];

    long double cumsum_mean1[256];
    cumsum_mean1[0] = h_bin_mids[0];
    for (int i = 1; i < 256; i++)
        cumsum_mean1[i] = cumsum_mean1[i - 1] + h_bin_mids[i];

    long double cumsum_mean2[256];
    cumsum_mean2[0] = h_bin_mids[255];
    for (int i = 1, j = 254; i < 256 && j >= 0; i++, j--)
        cumsum_mean2[i] = cumsum_mean2[i - 1] + h_bin_mids[j];

    long double mean1[256];
    for (int i = 0; i < 256; i++)
        mean1[i] = cumsum_mean1[i] / weight1[i];

    long double mean2[256];
    for (int i = 0, j = 255; i < 256 && j >= 0; i++, j--)
        mean2[j] = cumsum_mean2[i] / weight2[j];

    long double Inter_class_variance[255];
    long double dnum = 10000000000;
    for (int i = 0; i < 255; i++)
        Inter_class_variance[i] = ((weight1[i] * weight2[i] * (mean1[i] - mean2[i + 1])) / dnum) * (mean1[i] - mean2[i + 1]);

    long double maxi = 0;
    int getmax = 0;
    for (int i = 0; i < 255; i++)
    {
        if (maxi < Inter_class_variance[i])
        {
            maxi = Inter_class_variance[i];
            getmax = i;
        }
    }
    return bin_mids[getmax];
}


/* Transform image in Binary Image */
void binarization(SDL_Surface *img)
{
    /* Variables */
    Uint32 pixel;
    Uint8 r,g,b;
    int w = img -> w;
    int h = img -> h;
    int otsu = toBin(img);

    // Iterate on each pixels and blacknwhite 
    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < h; j++)
        {
            pixel = getpixel(img,i,j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if(r >= otsu && g >= otsu && b >= otsu)
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
