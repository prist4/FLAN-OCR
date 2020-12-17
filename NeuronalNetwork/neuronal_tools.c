# include "neuronal_tools.h"

//A random that returns a double in [-1; 1]
double Random()
{
    return ((double)rand()) / ((double)RAND_MAX / 2) - 1;
}

//The classical sigmoid function
double Sigmoid(double x)
{
    return(1.0 / (1.0 + exp(-x)));
}

//The classical derivate of the sigmoid function
double Derivate_Sigmoid(double x)
{
    return x * (1.0 - x);
}

//Return the position of the output with the greatest sigmoid result
int Pos_output(struct Neuronal_Network *net)
{
    double max = 0;
    int posMax = 0;

    for (int o = 0; o < net -> nbOutput; o++)
    {
        if (max < net -> Output_Output[o])
        {
            posMax = o;
            max = net -> Output_Output[o];
        }
    }
    return posMax;
}

//Calculates the error
void Error(struct Neuronal_Network *net)
{
    double max = 0;
    double sum = 0;

    for (int o = 0; o < net -> nbOutput; o++)
    {
        if (max < net -> Output_Output[o])
        {
            max = net -> Output_Output[o];
        }
        sum += (net -> Goal[o] - net -> Output_Output[o]) *(net -> Goal[o] - net -> Output_Output[o]);
    }
    net -> Error += 0.5 * sum;
}


//Retrive the needed position of the char
int Pos_needed(double *goal)
{
    int count = 0;

    while(goal[count] != 1.0)
        count++;

    return count;
}

//Retrive char from value val
char No_Char(int val)
{
    char c;
    if(val <= 25)
    {
        c = val + 65;
    }
    else if(val > 25 && val <= 51)
    {
        c = (val + 97 - 26);
    }
    else if(val > 51 && val <= 61)
    {
        c = val + 48 - 52;
    }
    else
    {
        switch(val)
        {
        case 62:
            c = ';';
            break;
        case 63:
            c = '\'';
            break;
        case 64:
            c = ':';
            break;
        case 65:
            c = '-';
            break;
        case 66:
            c = '.';
            break;
        case 67:
            c = '!';
            break;
        case 68:
            c = '?';
            break;
        case 69:
            c = '(';
            break;
        case 70:
            c = '\"';
            break;
        case 71:
            c = ')';
            break;
        default:
            exit(1);
            break;
        }
    }
    return c;
}

//Save data of the NeuronalNetwork in 4 files:
void Save_Bias_Weight(struct Neuronal_Network *net)
{
    FILE* weight_Input_Hidden = fopen("weight_Input_Hidden.txt", "w");
    for(int i = 0; i < net -> nbInput; ++i)
    {
        for(int h = 0; h < net -> nbHidden; ++h)
        {
            fprintf(weight_Input_Hidden, "%f\n", net -> Weight_Input_Hidden[i][h]);
        }
    }
    fclose(weight_Input_Hidden);

    FILE* weight_Hidden_Output = fopen("weight_Hidden_Output.txt", "w");
    for(int h = 0; h < net -> nbHidden; ++h)
    {
        for(int o = 0; o < net -> nbOutput; ++o)
        {
            fprintf(weight_Hidden_Output, "%f\n", net -> Weight_Hidden_Output[h][o]);
        }
    }
    fclose(weight_Hidden_Output);

    FILE* bias_Hidden = fopen("bias_Hidden.txt", "w");
    for(int h = 0; h < net -> nbHidden; ++h)
    {
        fprintf(bias_Hidden, "%f\n", net -> Bias_Hidden[h]);
    }
    fclose(bias_Hidden);

    FILE* bias_Output = fopen("bias_Output.txt", "w");
    for (int o = 0; o < net -> nbOutput; ++o)
    {
        fprintf(bias_Output, "%f\n", net -> Bias_Output[o]);
    }
    fclose(bias_Output);
}

//Extract data previously saved in 4 files:
struct Neuronal_Network* Extract_Bias_Weight ()
{
    //CREATE NeuronalNetwork
    struct Neuronal_Network *net = malloc(sizeof(struct Neuronal_Network));
    net -> nbInput = 28*28; //size of imgs
    net -> nbHidden = 20;
    net -> nbOutput = 52; //26*2 letters
    net -> str = malloc(sizeof(char)*1200);
    net -> str = "\0";

    int sizeMax = 15;
    char *line = calloc(15, sizeof(char));

    //WeightIH
    FILE* weight_Input_Hidden = fopen("weight_Input_Hidden.txt", "r");
    for(int i = 0; i < net -> nbInput; ++i)
    {
        for(int h = 0; h < net -> nbHidden; ++h)
        {
            fgets(line, sizeMax, weight_Input_Hidden);
            strtok(line, "\n");
            net -> Weight_Input_Hidden[i][h] = atof(line);
        }
    }
    fclose(weight_Input_Hidden);

    //Weight HO
    FILE* weight_Hidden_Output = fopen("weight_Hidden_Output.txt", "r");
    for(int h = 0; h < net -> nbHidden; ++h)
    {
        for(int o = 0; o < net -> nbOutput; ++o)
        {
            fgets(line, sizeMax, weight_Hidden_Output);
            strtok(line, "\n");
            net -> Weight_Hidden_Output[h][o] = atof(line);
        }
    }
    fclose(weight_Hidden_Output);

    //BiasH
    FILE* bias_Hidden = fopen("bias_Hidden.txt", "r");
    for(int h = 0; h < net -> nbHidden; ++h)
    {
        fgets(line, sizeMax, bias_Hidden);
        strtok(line, "\n");
        net -> Bias_Hidden[h] = atof(line);
    }
    fclose(bias_Hidden);

    //BiasO
    FILE* bias_Output = fopen("bias_Output.txt", "r");
    for (int o = 0; o < net -> nbOutput; ++o)
    {
        fgets(line, sizeMax, bias_Output);
        strtok(line, "\n");
        net -> Bias_Output[o] = atof(line);
    }
    fclose(bias_Output);

    return net;
}
