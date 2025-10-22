#ifndef STR_REDUC_H
#define STR_REDUC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adjlist.h"
#include "str_reduc.h"
#include "heap_graph.h"
#include "mst.h"
#include "dequef.h"


//////////////////////////////////
// Estrutura para armazenamento //
//////////////////////////////////

struct opt_str{
    int centroid;
    int num_rows;
    int* size;
    int** matrix;
};
typedef struct opt_str opt_str;


/////////////////////////////////////////////////////
// Funções associadas à estrutura de armazenamento //
/////////////////////////////////////////////////////

void opt_str_free(opt_str* opt);

void opt_str_display(opt_str* opt);

void save_opt_str(opt_str* opt, const char* filename);

opt_str* load_opt_str(const char* filename);

void save_vec_opt_str(opt_str** opt_vec, int num_elem, const char* filename);

opt_str** load_vec_opt_str(const char* filename, int* num_elem_out);


/////////////////////////////////////////////
// Funções para cálculo de memória ocupada //
/////////////////////////////////////////////

int str_reduction(opt_str* opt);

void str_effic(opt_str* opt, int** orig_matrix);

void vec_str_effic(opt_str** opt_vec, int num_elem, int** orig_matrix);


/////////////////////////////////////////////////////////
// Funções auxiliares para otimização do armazenamento //
/////////////////////////////////////////////////////////

int* diff(int* vec0, int* vec1, int size);

int sum_int(int* vec, int size);

int** edges_constructor(int** matrix, int num_rows, int num_cols);

int* find_diff(int num_node, int*vec0, int* vec1, int size, int* out_size);

opt_str* opt_matrix_constructor(int** matrix, int num_rows, int num_cols, int* pred, int centroid);

dequef* find_fix(opt_str* opt, int node);

int* reconstruct_vector(opt_str* opt, int node);

int** reconstruct_matrix(opt_str* opt);


///////////////////////////////////
// Funções associadas a matrizes //
///////////////////////////////////

int largest_p_divisor(int n);

int num_slices(int num_cols);

int*** matrix_slice(int** matrix, int num_slices, int num_rows, int num_cols);

int** matrix_join(int*** slices, int num_slices, int num_rows, int num_cols);

void display_matrix(int** matrix, int num_rows, int num_cols);

void save_matrix_binary(int** matrix, int num_rows, int num_cols, const char* filename);

int** load_matrix_binary(const char* filename, int* out_rows, int* out_cols);

void free_matrix(int** matrix, int num_rows);


//////////////////////////////////
// Funções associadas aos dados //
//////////////////////////////////

void store_data(int** matrix, int num_rows, int num_cols, const char* filename);

int** load_data(const char* filename, int* p_num_rows, int* p_num_cols);

#endif