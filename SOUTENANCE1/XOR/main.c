#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "network_neuronal.h"

int main() {
    srand(time(NULL));
    int size_layer[] = {2, 4, 1};
    mlp * network = create_mlp (3, size_layer); // we create a pointer to our 3-layer network
    mlp_float enter[4][2] = { // table of input values
        {0,0},
        {0,1},
        {1,0},
        {1,1}
    };
    mlp_float exit[4] = { // values ​​of the xor function corresponding to the inputs
        0,
        1,
        1,
        0
    };
    
    mlp_float global_error = 1;// to enter the while
        int i = 0;


        while (global_error > 0.00000000000001) { // as long as the error of our network is greater than the maximum error
            printf("error: %0.20f\n",global_error);// show the error of our network

            for (int iter=0; iter < 100; iter++) { // we give 100 examples to our network and ask it each time to correct its errors
                int num = 4.0*rand()/(RAND_MAX+1.0);
                set_mlp(network,& enter[num][0]); //the second parameter is a pointer to an mlp_float array. The set_mlp function uses the number of entries given plus to know which boxes to read in this table.
                learn_mlp(network,&exit[num]); // the number of outputs that we gave above allows learn_mlp to know how many boxes to read in the table passed as a parameter
            }
            // we corrected our network 100 times with our 4 examples but in a different order
            global_error = 0; // the global error is calculated: the standard deviation between the 4 given and desired outputs.
            for (int k=0; k < 4; k++) {
                set_mlp(network,& enter[k][0]); 
                mlp_float exit_network;
                get_mlp(network,&exit_network); // the get_mlp function uses the number of outputs given above to find out how many cells to write in this array
                mlp_float error=exit_network - exit[k];
                global_error += error * error;
            }

            global_error *= 0.25; 
            global_error = sqrt(global_error);
            // if the error is too big we redo 100 learning
            i++;
        }
        // we display the 4 results that the network gives us
        for (int k = 0; k < 4; ++k)
        {
            set_mlp(network,&enter[k][0]);
            mlp_float exit_network;
            get_mlp(network,&exit_network);
            printf("entries: %f et %f, outpout given by the network: %f desired exit: %f\n",
            enter[k][0],enter[k][1],exit_network,exit[k]);
        }
        printf("---> result found in %d iterations\n", i*100);
}
