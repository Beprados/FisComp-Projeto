#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_reduc.h"
#include "timer.h"
#include "timer_macros.h"

int main(void) {

    printf("\n************************************************\n");
    printf("*** Storage time test with parallel approach ***");
    printf("\n************************************************\n\n");

    int num_rows, num_cols;

    int** matrix = load_matrix_binary("data/in/original_matrix.bin", &num_rows, &num_cols);

    {   
        TIMER("Storage reduction");
        store_data(matrix, num_rows, num_cols, "data/out/opt_vec.bin");
        TIMER_STOP();
    }

    free_matrix(matrix, num_rows);

    return 0;
}