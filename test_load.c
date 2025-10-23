#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_reduc.h"
#include "timer.h"
#include "timer_macros.h"

int main(void) {

    printf("\n*******************************************************\n");
    printf("*** Reconstruction time test with parallel approach ***");
    printf("\n*******************************************************\n\n");

    int num_rows, num_cols;
    int** matrix;

    {   
        TIMER("Data reconstruction");
        matrix = load_data("data/out/opt_vec.bin", &num_rows, &num_cols);
        TIMER_STOP();
    }
    
    free_matrix(matrix, num_rows);

    return 0;
}