# ifndef NEURONAL_TOOLS_H_
# define NEURONAL_TOOLS_H_

# include "neuronal_network.h"
# include <stdlib.h>
# include <stdio.h>
# include <err.h>

//A random that returns a double in [-1; 1]
double Random();

//The classical sigmoid function
double Sigmoid(double x);

//The classical derivate of the sigmoid function
double Derivate_Sigmoid(double x);

//Return the position of the output with the greatest sigmoid result
int Pos_output(struct Neuronal_Network *net);

//Calculates the error
void Error(struct Neuronal_Network *net);

//Retrive the needed position of the char in the goal tab
int Pos_needed(double *goal);

//Retrive char from value val
char No_Char(int val);

//Save data of the NN in 4 files:
void Save_Bias_Weight(struct Neuronal_Network *net);

//Extract data previously saved in 4 files:
struct Neuronal_Network* Extract_Bias_Weight ();

# endif
