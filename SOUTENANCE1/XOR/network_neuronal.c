#include <stdio.h>
#include <stdlib.h> // for malloc
#include <math.h> // for exp
#include <time.h>

typedef double mlp_float;


typedef struct { 
     mlp_float *weight_synaptic; 
     mlp_float *value_neuron;
     mlp_float *value_error_neuron; 
     mlp_float *enter_neuron;
     mlp_float *exit_neuron;
     mlp_float *value_error_exit;
     int *index_layer; 
     int *size_layer; 
     int *index_synapse; 
     int number_layer; 
     int number__neuron; 
     int number__synapse;
     int size_layer_enter; 
     int size_layer_exit; 
 } mlp; 


static mlp_float MAGICAL_WEIGHT_NUMBER = 1.0f;
static mlp_float MAGICAL_LEARNING_NUMBER = 0.09f;



mlp *create_mlp(int number_layer, int *size_layer) // create a neural network with x layers, with as parameter an array with the number of neurons per layer
{

    mlp *network = malloc(sizeof * network);

    network->number_layer = number_layer;
    network->size_layer = malloc(sizeof * network->size_layer * network->number_layer);
    network->index_layer = malloc(sizeof * network->index_layer * network->number_layer);
    // we count the total number of neurons in the network (necessary to use malloc)
    int i;
    network->number__neuron = 0;
    for (i = 0; i < number_layer; i++) 
    {
        network->size_layer[i] = size_layer[i];
        network->index_layer[i] = network->number__neuron;
        network->number__neuron += size_layer[i];
    }

    network->value_neuron = malloc(sizeof * network->value_neuron * network->number__neuron);
    network->value_error_neuron = malloc(sizeof * network->value_error_neuron * network->number__neuron);

    network->size_layer_enter = size_layer[0];
    network->size_layer_exit = size_layer[number_layer-1];
    network->enter_neuron = network->value_neuron;
    network->exit_neuron = &network->value_neuron[network->index_layer[number_layer-1]];
    network->value_error_exit = &network->value_error_neuron[network->index_layer[number_layer-1]];

    // synapse index
    network->index_synapse = malloc(sizeof * network->index_synapse * (network->number_layer-1));
    network->number__synapse = 0;
    for (i = 0; i < number_layer - 1; i++) {
        network->index_synapse[i] = network->number__synapse;
        network->number__synapse += (network->size_layer[i]+1) * network->size_layer[i+1];
    }
    // we count the number of synapses to be able to use malloc
    network->weight_synaptic = malloc(sizeof * network->weight_synaptic * network->number__synapse);

    // we give a random value between - MAGICAL_WEIGHT_NUMBER and + MAGICAL_WEIGHT_NUMBER at each synapse
    for (i = 0; i < network->number__synapse; i++) {
        network->weight_synaptic[i] = MAGICAL_WEIGHT_NUMBER * (mlp_float)rand() / RAND_MAX - MAGICAL_WEIGHT_NUMBER/2;

    }
    return network;
}

void free_mlp (mlp *network) { // frees memory
    free(network->size_layer);
    free(network->index_layer);
    free(network->value_neuron);
    free(network->value_error_neuron);
    free(network->index_synapse);
    free(network->weight_synaptic);
    free(network);
}


void set_mlp (mlp * network, mlp_float *vector) { // puts the values ​​as inputs and calculates the output of the network
    // if the array passed as an argument is a null pointer, we keep the old input values
    if (vector != NULL) {
        int i;
        for (i = 0; i < network->size_layer_enter; i++) {
            network->enter_neuron[i] = vector[i];
        }
    }
    // calculates the values ​​of all neurons by advancing layer by layer
    int i;
    int index_synapse;
    index_synapse = 0;
    for (i = 1; i < network->number_layer; i++) {
        int j;
        for (j = network->index_layer[i]; j < network->index_layer[i] + network->size_layer[i]; j++) {
            mlp_float weighted_sum = 0.0;
            int k;
            for (k = network->index_layer[i-1]; k < network->index_layer[i-1] + network->size_layer[i-1]; k++) {
                weighted_sum += network->value_neuron[k] * network->weight_synaptic[index_synapse];
                index_synapse++;
            }
            weighted_sum += network->weight_synaptic[index_synapse];
            index_synapse++;
            network->value_neuron[j] = weighted_sum;
            if (i != network->number_layer - 1) network->value_neuron[j] = 1.0 / (exp(-weighted_sum) + 1.0);

        }
    }
}

// copy the output values ​​network-> exit_neuron [i] in the array passed as argument
void get_mlp (mlp *network, mlp_float *vector) { // recovers network output 
    int i;
    for (i = 0; i < network->size_layer_exit; i++) {
        vector[i] = network->exit_neuron[i];
    }
}

// function that does a learning
void learn_mlp (mlp *network, mlp_float *desired_out) { // fixes the network by giving it the exit it should have found


    int i;
    // we position ourselves in the weight_synaptic array at the index_synapse [network-> number_layer-2]
    int index_synapse = network->index_synapse[network->number_layer-2];
    // for all output neurons S
    for (i = 0; i < network-> size_layer_exit; i++) {
        // calculates the difference between the calculated output and the desired output
        network->value_error_exit[i] = network->exit_neuron[i] - desired_out[i];
        int j;
        // for all neurons of the middle layer N
        for (j = network->index_layer[network->number_layer-2]; j < network->index_layer[network->number_layer-2] + network->size_layer[network->number_layer-2]; j++) {
            // weight_synaptic =weightChange
            mlp_float weightChange;
            weightChange = MAGICAL_LEARNING_NUMBER * network->value_error_exit[i] * network->value_neuron[j];
            network->weight_synaptic[index_synapse] -= weightChange;
            // this synaptic weight must not be too low or too large
            if (network->weight_synaptic[index_synapse] > 5) network->weight_synaptic[index_synapse] = 5;
            if (network->weight_synaptic[index_synapse] < -5) network->weight_synaptic[index_synapse] = -5;

            index_synapse++;
        }
        // here it is the threshold that we modify
        mlp_float weightChange;
        weightChange = MAGICAL_LEARNING_NUMBER * network->value_error_exit[i];
        network->weight_synaptic[index_synapse] -= weightChange;

        if (network->weight_synaptic[index_synapse] > 5) network->weight_synaptic[index_synapse] = 5;
        if (network->weight_synaptic[index_synapse] < -5) network->weight_synaptic[index_synapse] = -5;

        index_synapse++;
    }

    // for all C layers except exit layer and in descending direction
    for (i = network->number_layer - 2; i > 0; i--) {
        int j;
        int jj= 0;
        // we position ourselves on the synapses from the C-1 layer to the C layer
        int index_synapse = network->index_synapse[i-1];
        // for all N neurons of this layer
        for (j = network->index_layer[i]; j < network->index_layer[i] + network->size_layer[i]; j++,jj++) {
            int k;
            // we position ourselves on the synapse which connects N to a neuron of the C + 1 layer
            int index_synapse2 = network->index_synapse[i] + jj;
            network->value_error_neuron[j] = 0;
            //we sum the error values ​​of the neurons of the layer C + 1
            // weighted by synaptic weights from N to C + 1 layer neurons
            for (k = network->index_layer[i+1]; k < network->index_layer[i+1] + network->size_layer[i+1]; k++) {
                network->value_error_neuron[j] += network->weight_synaptic[index_synapse2] * network->value_error_neuron[k];
                // we go to the next neuron of layer C + 1 by incrementing k, and to the corresponding synapse by adding
                // network-> size_layer [C] +1 to index_synapse2 (the +1 for the threshold)
                index_synapse2+=network->size_layer[i]+1;
            }
            //printf(" %d = %f \n", j, reseau->value_error_neuron[j]);

            // for all X neurons of the C-1 layer
            for (k = network->index_layer[i-1]; k < network->index_layer[i-1] + network->size_layer[i-1]; k++) {

                // weight_synaptic = weightChange
                mlp_float weightChange;

                weightChange = 1.0 - network->value_neuron[j] * network->value_neuron[j];

                weightChange *= network->value_error_neuron[j] * MAGICAL_LEARNING_NUMBER;
                //printf(" %d = %f  bis\n", j, reseau->weight_synaptic[12+jj] * reseau->value_error_exit[0]);
                weightChange *= network->value_neuron[k];

                network->weight_synaptic[index_synapse] -= weightChange;
                index_synapse++;
            }
            // synaptic weight threshold =weightChange
            mlp_float weightChange;

            weightChange = 1.0 - network->value_neuron[j] * network->value_neuron[j];
            weightChange *= network->value_error_neuron[j] * MAGICAL_LEARNING_NUMBER;
            network->weight_synaptic[index_synapse] -= weightChange;
            index_synapse++;
        }
    }
}
