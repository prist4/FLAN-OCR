# include "neuronal_network.h"

struct Neuronal_Network* InitializeNetwork()
{
    struct Neuronal_Network *net = NULL;
    net = malloc(sizeof(struct Neuronal_Network));
    net -> nbInput = 28*28;
    net -> nbHidden = 20;
    net -> nbOutput = 52;
    net -> Error = 0.0;
    net -> MaxError = 0.0;
    net -> y = 0.5;
    net -> x = 0.9;
    net -> str = "";

    //Set Values
    for (int i = 0; i < net -> nbInput; i++)
    {
        for (int h = 0; h < net -> nbHidden; h++)
        {
            net -> Weight_Input_Hidden[i][h] = Random();
            net -> deltaWeight_Input_Hidden[i][h] = 0.0;
        }
    }

    for (int h = 0; h < net -> nbHidden; h++)
    {
        for(int o = 0; o < net -> nbOutput; o++)
        {
            net -> Weight_Hidden_Output[h][o] = Random();
            net -> deltaWeight_Hidden_Output[h][o] = 0.0;
        }
        net -> Bias_Hidden[h] = Random();
    }

    for (int o = 0; o < net -> nbOutput; o++)
    {
        net -> Bias_Output[o] = Random();
        net -> deltaOutput_Output[o] = 0.0;
    }
    return net;
}

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

static void BackPropagation(struct Neuronal_Network *net)
{
    //Calulate Cost function for output neurons
    double output, derivate, goal, error;
    for (int o = 0; o < net -> nbOutput; o++)
    {
        output = net -> Output_Output[o];
        derivate = Derivate_Sigmoid(output);
        goal = net -> Goal[o];
        error = (goal - output) * derivate;
        net -> deltaOutput_Output[o] = error;
    }

    ///Calculate Cost function for hidden neurons
    double sum, weight, delta;
    for (int h = 0; h < net -> nbHidden; h++)
    {
        sum = 0.0;
        for (int o = 0; o < net -> nbOutput; o++)
        {
            weight = net -> Weight_Hidden_Output[h][o];
            delta = net -> deltaOutput_Output[o];
            sum += weight * delta;
        }
        output = net -> Output_Hidden[h];
        derivate = Derivate_Sigmoid(output);
        net -> deltaHidden[h] = sum * derivate;
    }
}

static void UpdateWeights(struct Neuronal_Network *net)
{
    double y, x, error, output, deltaWeight;
    y = net -> y;
    x = net -> x;

    //Weights between Input and Hidden layers
    for (int h = 0; h < net -> nbHidden; h++)
    {
        for(int i = 0; i < net -> nbInput; i++)
        {
            output = net -> Output_Input[i];
            error = net -> deltaHidden[h];
            deltaWeight = net -> deltaWeight_Input_Hidden[i][h];
            net -> Weight_Input_Hidden[i][h] += y * error * output + x * deltaWeight;
            net -> deltaWeight_Input_Hidden[i][h] = y * error * output;
        }
    }

    //Weights between Hidden and Ouput layers
    for (int o = 0; o < net -> nbOutput; o++)
    {
        for (int h = 0; h < net -> nbHidden; h++)
        {
            output = net -> Output_Hidden[h];
            error = net -> deltaOutput_Output[o];
            deltaWeight = net -> deltaWeight_Hidden_Output[h][o];
            net -> Weight_Hidden_Output[h][o] += y * error * output + x * deltaWeight;
            net -> deltaWeight_Hidden_Output[h][o] = y * error * output;
        }
    }
}

static void UpdateBiases(struct Neuronal_Network *net)
{
    double y, delta, deltaBias;
    y = net -> y;
    //Update BiasH
    for (int h = 0; h < net -> nbHidden; h++)
    {
        delta = net -> deltaHidden[h];
        deltaBias = y * delta;
        net -> Bias_Hidden[h] += deltaBias;
    }

    //Update BiasO
    for (int o = 0; o < net -> nbOutput; o++)
    {
        delta = net -> deltaOutput_Output[o];
        deltaBias = y * delta;
        net -> Bias_Output[o] += deltaBias;
    }
}


void PrintState(struct Neuronal_Network *net)
{
    //error function
    Error(net);
    int output = Pos_output(net);

    //Retrive the chars : wanted & found
    char goalChar = No_Char(Pos_needed(net -> Goal));
    char recognizedChar = No_Char(output);

    if(net -> Error > net -> MaxError)
        net -> MaxError = net -> Error;

    //Print the progress
    if(output == Pos_needed(net -> Goal))
        printf("Position Found = %d Expected %d OK \n",output, Pos_needed(net -> Goal));
    else
        printf("Position Found = %d Expected %d KO \n",output, Pos_needed(net -> Goal));
    
    printf("Char entered: %c | Char recoginized: %c | Error: %f\n",goalChar,recognizedChar,net -> Error);
}

void OCR(struct Neuronal_Network *net, double *input, double *goal)
{
    //Initialise Goals & InputValues for this char
    for (int g = 0; g < 52; g++)
    {
        net -> Goal[g] = goal[g];
    }
    for(int i = 0; i < net -> nbInput; i++)
    {
        net -> Output_Input[i] = input[i];
    }

    //Run the learning
    net -> Error = 0;
    ForwardPropagation(net);
    BackPropagation(net);
    UpdateWeights(net);
    UpdateBiases(net);
}
