#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_reduc.h"

int main(void) {

    printf("\n************************************************\n");
    printf("*** Storage time test with parallel approach ***");
    printf("\n************************************************\n\n");

    int num_rows, num_cols;

    int** matrix = load_matrix_binary("data/in/original_matrix.bin", &num_rows, &num_cols);
    
    store_data(matrix, num_rows, num_cols, "data/out/opt_vec.bin");

    free_matrix(matrix, num_rows);

    return 0;
}