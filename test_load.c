#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_reduc.h"

int main(void) {

    int num_rows, num_cols;
    
    int** matrix = load_data("data/out/opt_vec.bin", &num_rows, &num_cols);

    free_matrix(matrix, num_rows);

    return 0;
}