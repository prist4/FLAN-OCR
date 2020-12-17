# ifndef NEURONAL_NETWORK_H_
# define NEURONAL_NETWORK_H_

# include <stdlib.h>
# include <stdio.h>
# include <err.h>
# include <time.h>
# include <math.h>
# include "../Tools/tools.h"

struct Neuronal_Network
{
    int nbInput;
    int nbHidden;
    int nbOutput;

    //Arrays
    double Output_Input[28*28];
    double Goal[52];
    //Weight Arrays
    double Weight_Input_Hidden[28*28][20];
    double Weight_Hidden_Output[20][52];
    //Bias Array
    double Bias_Hidden[20];
    //Bias output
    double Bias_Output[52];
    //Output Hidden
    double Output_Hidden[20];
    //Output of output
    double Output_Output[52];

    //delta weight
    double deltaWeight_Input_Hidden[28*28][20];
    double deltaWeight_Hidden_Output[20][52];
    //delta of output
    double deltaOutput_Output[52];
    //delta of hidden
    double deltaHidden[20];

    double MaxError;
    double Error;
    double y;
    double x;

    char *str;
};

# include "neuronal_tools.h"

struct Neuronal_Network* InitializeNetwork();

void PrintState(struct Neuronal_Network *net);

void OCR(struct Neuronal_Network *net, double *input,double *goal);

# endif
