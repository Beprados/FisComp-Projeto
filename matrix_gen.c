#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "str_reduc.h"

// Random binary number generator
int rand_binary() {
    return rand() % 2;
}

// Random binary matrix generator
int** matrix_binary(int num_rows, int num_cols) {

    int** matrix = malloc(num_rows*sizeof(int*));
    for (int i = 0; i < num_rows; i++) { 
        matrix[i] = malloc(num_cols*sizeof(int)); 
    }

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            matrix[i][j] = rand_binary();
        }
    }

    return matrix;
}

int main(void) {

    int num_rows = 0;
    while(num_rows <= 0) {
        printf("Type number of rows (> 0 and < 10^4) : ");
        scanf("%d", &num_rows);
    }

    int num_cols = 0;
    while(num_cols <= 0) {
        printf("Type number of columns (> 0 and < 10^4): ");
        scanf("%d", &num_cols);
    }
    
    int** matrix = matrix_binary(num_rows, num_cols);

    save_matrix_binary(matrix, num_rows, num_cols, "data/in/original_matrix.bin");
    
    free_matrix(matrix, num_rows);
    
    return 0;
}