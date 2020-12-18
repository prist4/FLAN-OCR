#ifndef MLP_H
#define MLP_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


#define mlp_float double

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
    int number_neuron;
    int number_synapse;
    int size_layer_enter;
    int size_layer_exit;
} mlp;




extern mlp_float MAGICAL_WEIGHT_NUMBER;
extern mlp_float MAGICAL_LEARNING_NUMBER;

mlp *create_mlp(int number_layer, int *size_layer);
void free_mlp (mlp *network);
void set_mlp (mlp * network, mlp_float *vector);
void get_mlp (mlp *network, mlp_float *vector);
void learn_mlp (mlp *network, mlp_float *desired_out);

#endif 
