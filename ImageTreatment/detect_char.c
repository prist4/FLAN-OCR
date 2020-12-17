#include "detect_char.h"


static void ForwardPropagation(struct Neuronal_Network *net)
{
    double sum, weight, output, bias;

    //Calculate Output for Hidden neurons
    for (int h = 0; h < net -> nbHidden; h++)
    {
        sum = 0.0;
        for (int i = 0; i < net -> nbInput; i++)
        {
            weight = net -> Weight_Input_Hidden[i][h];
            output = net -> Output_Input[i];
            sum += weight * output;
        }
        bias = net -> Bias_Hidden[h];
        net -> Output_Hidden[h] = Sigmoid(sum + bias);
    }

  //Calculate Output for Output neurons
    for (int o = 0; o < net -> nbOutput; o++)
    {
        sum = 0.0;
        for (int h = 0; h < net -> nbHidden; h++)
        {
            weight = net -> Weight_Hidden_Output[h][o];
            output = net -> Output_Hidden[h];
            sum += weight * output;
        }
        bias = net -> Bias_Output[o];
        net -> Output_Output[o] = Sigmoid(sum + bias);
    }
}

char DetectText(struct Neuronal_Network *net, double* letter)
{
    for(int i = 0; i < net -> nbInput; i++)
    {
        net -> Output_Input[i] = letter[i];
    }

    ForwardPropagation(net);

    //Retrieve the character detected
    int pos = Pos_output(net);
    char c = No_Char(pos);
    return c;
}