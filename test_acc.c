#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str_reduc.h"

int main(void) {

    printf("\n************************************\n");
    printf("*** Reconstruction accuracy test ***");
    printf("\n************************************\n\n");

    int num_rows, num_cols;
    
    int** rec_matrix = load_data("data/out/opt_vec.bin", &num_rows, &num_cols);

    int** orig_matrix = load_matrix_binary("data/in/original_matrix.bin", &num_rows, &num_cols);

    int flag = 1;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (orig_matrix[i][j] != rec_matrix[i][j]) { flag *= 0; break; }
        }   
    }
    
    printf("*** Matrix was succesfully reconstructed: %s\n\n", flag ? "true ***" : "false ***");
    
    free_matrix(rec_matrix, num_rows);
    free_matrix(orig_matrix, num_rows);

    return 0;
}