# ifndef DETECT_CHAR_H_
# define DETECT_CHAR_H_

# include <stdlib.h>
# include <stdio.h>

#include "../NeuronalNetwork/neuronal_tools.h"

char DetectText(struct Neuronal_Network *net, double* letter);

# endif
