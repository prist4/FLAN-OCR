#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "pixel_operations.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int isColored(Uint32 pixel, SDL_Surface *image, int r1, int g1, int b1)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, image->format, &r, &g, &b);
    if (r == r1 && g == g1 && b == b1)
    {
        return 1;
    }
    return 0;
}

void ColorLine(SDL_Surface *image, int x, int width, int r, int g, int b)
{
    for (int i = 0; i < width; i++)
    {
        Uint32 pixel = get_pixel(image, i, x);
        pixel = SDL_MapRGB(image->format, r, g, b);
        put_pixel(image, i, x, pixel);
    }
}

void ColorSemiColumn(SDL_Surface *image, int y, int h1, int h2, int r, int g, int b)
{
    for (int i = h1; i < h2; i++)
    {
        Uint32 pixel = get_pixel(image, y, i);
        pixel = SDL_MapRGB(image->format, r, g, b);
        put_pixel(image, y, i, pixel);
    }
}


int toBin(SDL_Surface *image)
{
    return 127;
    Uint8 r = 0, g = 0, b = 0, a = 0, average = 0;
    long double h[256];

    for (int i = 0; i < 256; i++)
    {
        h[i] = 0;
    }

    for (int i = 0; i < image->h; i++)
        for (int j = 0; j < image->w; j++)
        {
            Uint32 ip = get_pixel(image, j, i);
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

