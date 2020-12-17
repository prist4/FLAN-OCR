# include <err.h>
# include <stdlib.h>
# include <stdio.h>
# include <SDL/SDL.h>
# include <SDL/SDL_image.h>
# include "Tools/tools.h"
# include "ImageTreatment/image_operations.h"
# include "ImageTreatment/segmentation.h"
# include "NeuronalNetwork/network_neuronal.h"
# include "NeuronalNetwork/neuronal_network.h"
# include <gtk/gtk.h>
# include <string.h>
# include "Interface/interface.h"


int main(int argc, char *argv[])
{
    printf("Welcome to OCR FLAN\n");
    create_window(argc,argv);
    printf("Made by FLAN team\n");
    char *imgp = "resizedImage.bmp";
    remove(imgp);
    return(EXIT_SUCCESS);
}
