#include <stdio.h>
#include <stdlib.h>
#include "io.h"

unsigned int read_in_bf(){
    char in_data[3] = {0};
    unsigned int num_val = 0;
    fscanf(stdin, "%3s", in_data);

    if(in_data[0] < 48 || in_data[0] > 57)
        return in_data[0];
    
    int i = 0;
    while(in_data[i] > 47 && in_data[i] < 58){
        num_val *= 10;
        num_val += in_data[i++] - 48;
    }

    return num_val;
}