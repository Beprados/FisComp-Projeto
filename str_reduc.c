#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "adjlist.h"
#include "str_reduc.h"
#include "heap_graph.h"
#include "mst.h"
#include "dequef.h"

#include "str_reduc.h"


void opt_str_free(opt_str* opt) {

    for (int i = 0; i < opt->num_rows; i++) {
        free(opt->matrix[i]);
    }
    free(opt->matrix);
    free(opt->size);
    free(opt);
}

void opt_str_display(opt_str* opt) {
    
    printf("\n*** Opt. Storage ***\n\n");
    
    printf("Opt. matrix: \n\n");

    int* vec;
    for (int i = 0; i < opt->num_rows; i++) {
        vec = opt->matrix[i];
        int n = opt->size[i];
        for (int j = 0; j < n; j ++) {
            if (j == 0) { printf("    %d ", vec[j]); }
            else { printf("%d ", vec[j]); }
        }

        printf("\n");
    }
    printf("\n");
    printf("Number of rows: %d\n", opt->num_rows);
    printf("Centroid: %d\n", opt->centroid);
    return;
}

void save_opt_str(opt_str* opt, const char* filename) {
    
    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return;
    }

    // salva os inteiros simples
    fwrite(&(opt->centroid), sizeof(int), 1, f);
    fwrite(&(opt->num_rows), sizeof(int), 1, f);

    // salva o vetor size (com num_rows elementos)
    fwrite(opt->size, sizeof(int), opt->num_rows, f);

    // agora salva as linhas da matrix
    for (int i = 0; i < opt->num_rows; i++) {
        int row_size = opt->size[i];
        fwrite(opt->matrix[i], sizeof(int), row_size, f);
    }

    fclose(f);
}

opt_str* load_opt_str(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return NULL;
    }

    opt_str* opt = malloc(sizeof(opt_str));

    fread(&(opt->centroid), sizeof(int), 1, f);
    fread(&(opt->num_rows), sizeof(int), 1, f);

    // aloca e lê o vetor size
    opt->size = malloc(opt->num_rows * sizeof(int));
    fread(opt->size, sizeof(int), opt->num_rows, f);

    // aloca a matrix (vetor de ponteiros)
    opt->matrix = malloc(opt->num_rows * sizeof(int*));
    for (int i = 0; i < opt->num_rows; i++) {
        int row_size = opt->size[i];
        opt->matrix[i] = malloc(row_size * sizeof(int));
        fread(opt->matrix[i], sizeof(int), row_size, f);
    }

    fclose(f);
    return opt;
}

void save_vec_opt_str(opt_str** opt_vec, int num_elem, const char* filename) {

    FILE* f = fopen(filename, "wb");
    if (!f) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fwrite(&num_elem, sizeof(int), 1, f);

    for (int cont=0; cont<num_elem; cont++) {

        // salva os inteiros simples
        fwrite(&(opt_vec[cont]->centroid), sizeof(int), 1, f);
        fwrite(&(opt_vec[cont]->num_rows), sizeof(int), 1, f);

        // salva o vetor size (com num_rows elementos)
        fwrite(opt_vec[cont]->size, sizeof(int), opt_vec[cont]->num_rows, f);

        // agora salva as linhas da matrix
        for (int i = 0; i < opt_vec[cont]->num_rows; i++) {
            int row_size = opt_vec[cont]->size[i];
            fwrite(opt_vec[cont]->matrix[i], sizeof(int), row_size, f);
        }
    }

    fclose(f);

    printf("\n[Storage reduction was successful]\n\n");

}

opt_str** load_vec_opt_str(const char* filename, int* num_elem_out) {
    FILE* f = fopen(filename, "rb");
    if (!f) { perror("Erro ao abrir arquivo"); return NULL; }

    int num_elem;
    fread(&num_elem, sizeof(int), 1, f);
    opt_str** opt_vec = malloc(num_elem * sizeof(opt_str*));

    for (int k = 0; k < num_elem; k++) {
        opt_vec[k] = malloc(sizeof(opt_str));

        fread(&(opt_vec[k]->centroid), sizeof(int), 1, f);
        fread(&(opt_vec[k]->num_rows), sizeof(int), 1, f);

        opt_vec[k]->size = malloc(opt_vec[k]->num_rows * sizeof(int));
        fread(opt_vec[k]->size, sizeof(int), opt_vec[k]->num_rows, f);

        opt_vec[k]->matrix = malloc(opt_vec[k]->num_rows * sizeof(int*));
        for (int i = 0; i < opt_vec[k]->num_rows; i++) {
            int row_size = opt_vec[k]->size[i];
            opt_vec[k]->matrix[i] = malloc(row_size * sizeof(int));
            fread(opt_vec[k]->matrix[i], sizeof(int), row_size, f);
        }
    }

    fclose(f);
    if (num_elem_out) *num_elem_out = num_elem;
    return opt_vec;
}


int str_reduction(opt_str* opt) {

    int opt_size = 0;

    opt_size += 1*sizeof(int); // Centroid storage
    opt_size += 1*sizeof(int); // Num rows storage
    
    opt_size += 1*sizeof(int*); // Size vector pointer
    opt_size += opt->num_rows*sizeof(int); // Size vector elements
    
    opt_size += 1*sizeof(int**); // Opt. matrix pointer
    opt_size += opt->num_rows*sizeof(int*); // Opt. matrix rows pointers
    opt_size += sum_int(opt->size, opt->num_rows)*sizeof(int); // Opt. matrix elements 

    return opt_size;
}

void str_effic(opt_str* opt, int** orig_matrix) {

    int opt_size = str_reduction(opt);

    int orig_size = 0;
    orig_size += 1*sizeof(int**); // Original matrix pointer
    orig_size += opt->num_rows; // Original matrix rows pointers
    orig_size += opt->num_rows * opt->size[opt->centroid] * sizeof(int); // Original matrix elements

    float abs_reduc = orig_size-opt_size;
    float rel_reduc = 1.0 - (float) opt_size/orig_size;

    printf("\n\n*** Storage Reduction Status ***\n\n");

    printf("Original matrix size: %.2e bytes\n", (double) orig_size);
    printf("Optimal struct size: %.2e bytes\n", (double) opt_size);
    printf("Net reduction: %.2e bytes\n", (double) abs_reduc);
    printf("Relative reduction: %.2f%%\n\n", 100*rel_reduc);

}

void vec_str_effic(opt_str** opt_vec, int num_elem, int** orig_matrix) {

    int opt_size = sizeof(int);
    for (int cont=0; cont<num_elem; cont++) { opt_size += str_reduction(opt_vec[cont]); }

    int orig_size = 0;
    orig_size += 1*sizeof(int**); // Original matrix pointer
    orig_size += opt_vec[0]->num_rows; // Original matrix rows pointers
    orig_size += opt_vec[0]->num_rows * opt_vec[0]->size[opt_vec[0]->centroid] * num_elem * sizeof(int); // Original matrix elements

    float abs_reduc = orig_size-opt_size;
    float rel_reduc = 1.0 - (float) opt_size/orig_size;

    printf("\n\n*** Storage Reduction Status ***\n\n");

    printf("Original matrix size: %.2e bytes\n", (double) orig_size);
    printf("Optimal struct size: %.2e bytes\n", (double) opt_size);
    printf("Net reduction: %.2e bytes\n", (double) abs_reduc);
    printf("Relative reduction: %.2f%%\n\n", 100*rel_reduc);

}


int* diff(int* vec0, int* vec1, int size) {

    int * diff_vec = malloc(size*sizeof(int));
    for (int i = 0; i < size; i++) {
        diff_vec[i] = ( vec0[i] != vec1[i] );
    }
    
    return diff_vec;
}

int sum_int(int* vec, int size) {

    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += vec[i];
    }
    return sum;
}

int** edges_constructor(int** matrix, int num_rows, int num_cols) {

    int num_edges = num_rows*(num_rows-1)/2;
    int** edges = malloc(num_edges*sizeof(int*));
    int diff_sum;
    int cont_edges = 0;

    for (int i=0; i < num_rows; i++) {
        for (int j=i+1; j < num_rows; j++) {

            int * diff_vec = diff(matrix[i], matrix[j], num_cols);
            diff_sum = sum_int(diff_vec, num_cols);
            free(diff_vec);
            
            edges[cont_edges] = malloc(3*sizeof(int));
            edges[cont_edges][0] = i;
            edges[cont_edges][1] = j;
            edges[cont_edges][2] = diff_sum;
            
            cont_edges ++;
            
        }
    }

    return edges;
}

int* find_diff(int num_node, int*vec0, int* vec1, int size, int* out_size) {

    int* opt_vec = malloc((2*size + 1)*sizeof(int));
    opt_vec[0] = num_node;
    int real_size = 1;

    for (int i = 0; i < size; i++) {
        if (vec1[i] != vec0[i]) {
            opt_vec[real_size] = i;
            opt_vec[real_size + 1] = vec1[i];
            real_size += 2;
        }
    }
        
    if (real_size < 2*size + 1) {
        opt_vec = realloc(opt_vec, real_size*sizeof(int));
    }

    *out_size = real_size;
    return opt_vec;
}

dequef* find_fix(opt_str* opt, int node) {

    int* vec = opt->matrix[node];
    int size = opt->size[node];
    int pred_node = vec[0];
    int centroid = opt->centroid;

    dequef* fix = df_alloc(5, 1.75);

    while (pred_node != centroid) {
        
        if (size > 1) {
            for (int i = 1; i < size; i += 2) {
                df_push(fix, vec[i+1]);
                df_push(fix, vec[i]);
            }
        }
        vec = opt->matrix[pred_node];
        size = opt->size[pred_node];
        pred_node = vec[0];
    }

    for (int i = 1; i < size; i += 2) {
        df_push(fix, vec[i+1]);
        df_push(fix, vec[i]);
    }
    // df_print(fix);
    return fix;
}

int* reconstruct_vector(opt_str* opt, int node) {

    int centroid = opt->centroid;
    int vec_size = opt->size[centroid];
    int* vec = malloc(vec_size*sizeof(int));
    for (int i=0; i<vec_size; i++) {
        vec[i] = opt->matrix[centroid][i];
    }

    if (node != centroid) {
        dequef* fix = find_fix(opt, node);

        int* fix_vec = fix->data;
        int aux_index;
        int aux_fix;
        if (fix->size > 0) {
            for (int i = fix->size - 1; i > 0; i -= 2) {
                aux_index = fix_vec[i];
                aux_fix = fix_vec[i-1];
                vec[aux_index] = aux_fix;
            }
        }
        df_free(fix);
    }

    return vec;
}

int** reconstruct_matrix(opt_str* opt) {

    int num_threads = omp_get_max_threads();

    int num_rows = opt->num_rows;
    int num_cols = opt->size[opt->centroid];
    int** result = malloc(opt->num_rows * sizeof(int*));

    // OpenMP parallel constructor
    #pragma omp parallel for num_threads(num_threads)
        for (int i = 0; i < num_rows; i++) {
            result[i] = reconstruct_vector(opt, i);
        }
    
    return result;
}

opt_str* opt_matrix_constructor(int** matrix, int num_rows, int num_cols, int* pred, int centroid) {

    opt_str* opt = malloc(sizeof(opt_str));
    int** opt_matrix = malloc(num_rows*sizeof(int*));
    opt_matrix[centroid] = malloc(num_cols*sizeof(int));
    for (int j = 0; j < num_cols; j++) {
        opt_matrix[centroid][j] = matrix[centroid][j];
    }

    int* opt_size = malloc(num_rows*sizeof(int));
    opt_size[centroid] = num_cols;

    int node_pred;
    int* pred_vec;
    int* son_vec;
    for (int i = 0; i<centroid; i++) {
        node_pred = pred[i];
        pred_vec = matrix[node_pred];
        son_vec = matrix[i];
        opt_matrix[i] = find_diff(node_pred, pred_vec, son_vec, num_cols, &opt_size[i]);
    }
    for (int i = centroid+1; i<num_rows; i++) {
        node_pred = pred[i];
        pred_vec = matrix[node_pred];
        son_vec = matrix[i];
        opt_matrix[i] = find_diff(node_pred, pred_vec, son_vec, num_cols, &opt_size[i]);
    }

    opt->centroid = centroid;
    opt->num_rows = num_rows;
    opt->size = opt_size;
    opt->matrix = opt_matrix;

    return opt;
}


int largest_p_divisor(int n) {

    // if (n % 2 == 0) {return n/2; }
    if( is_prime(n) ) { return 1; }

    int median;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n%i == 0) {median = i;}
    }
    return n/median;

}

int num_slices(int num_cols) {
    return largest_p_divisor(num_cols);
}

int*** matrix_slice(int** matrix, int num_slices, int num_rows, int num_cols) {


    int*** result = malloc(num_slices*sizeof(int**));
    if (!result) {return NULL;}

    for (int i=0; i<num_slices; i++) {

        result[i] = malloc(num_rows*sizeof(int*));
        if (!result[i]) {return NULL;}

        for (int j = 0; j<num_rows; j++) {

            result[i][j] = malloc((num_cols/num_slices)*sizeof(int));
            if (!result[i][j]) {return NULL;}

        }

    }

    for (int i=0; i<num_slices; i++) {
        for (int j = 0; j<num_rows; j++) {
            for (int k=0; k<num_cols/num_slices; k++) {
                result[i][j][k] = matrix[j][k + i*(num_cols/num_slices)];
            }
        }
    }


    return result;

}

int** matrix_join(int*** slices, int num_slices, int num_rows, int num_cols) {
    
    int** result = malloc(num_rows*sizeof(int*));
    if (!result) {return NULL;}

    for (int i=0; i<num_rows; i++) {
        result[i] = malloc(num_cols*sizeof(int));
        if (!result[i]) {return NULL;}
    }

    for (int i=0; i<num_slices; i++) {
        for (int j=0; j<num_rows; j++) {
            for (int k=0; k<num_cols/num_slices; k++) {
                result[j][k + i*(num_cols/num_slices)] = slices[i][j][k];
            }
        }
    }

    return result;

}

void display_matrix(int** matrix, int num_rows, int num_cols) {

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if(j==0){
            printf("    %d ", matrix[i][j]);
            }
            else{
            printf("%d ", matrix[i][j]);
            }
        }
        printf("\n");
    }

    return;
}

void save_matrix_binary(int** matrix, int num_rows, int num_cols, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) { perror("Erro ao abrir arquivo"); return; }

    // opcional: salvar as dimensões no arquivo
    fwrite(&num_rows, sizeof(int), 1, f);
    fwrite(&num_cols, sizeof(int), 1, f);

    // salvar cada linha
    for (int i = 0; i < num_rows; i++) {
        fwrite(matrix[i], sizeof(int), num_cols, f);
    }

    fclose(f);
}

int** load_matrix_binary(const char* filename, int* out_rows, int* out_cols) {
    FILE* f = fopen(filename, "rb");
    if (!f) { perror("Erro ao abrir arquivo"); return NULL; }

    int num_rows, num_cols;
    fread(&num_rows, sizeof(int), 1, f);
    fread(&num_cols, sizeof(int), 1, f);

    int** matrix = malloc(num_rows * sizeof(int*));
    for (int i = 0; i < num_rows; i++) {
        matrix[i] = malloc(num_cols * sizeof(int));
        fread(matrix[i], sizeof(int), num_cols, f);
    }

    fclose(f);
    *out_rows = num_rows;
    *out_cols = num_cols;
    return matrix;
}

void free_matrix(int** matrix, int num_rows) {

    for (int i=0; i<num_rows; i++) {
        free(matrix[i]);
    }
    free(matrix);

}


void store_data(int** matrix, int num_rows, int num_cols, const char* filename) {

    int num_threads = omp_get_max_threads(); // Number of available CPU's cores


    ////////////////////
    //* Data slicing *//
    ////////////////////

    int num_slices = largest_p_divisor(num_cols);
    int*** slices = matrix_slice(matrix, num_slices, num_rows, num_cols);

    int num_nodes = num_rows;
    int num_edges = (int) num_nodes*(num_nodes-1)/2;
    adj_list** list_vec = malloc(num_slices*sizeof(adj_list*));


    //////////////////////////////
    //* Dense net construction *//
    //////////////////////////////

    #pragma omp parallel for num_threads(num_threads)
        for (int cont = 0; cont<num_slices; cont++) { 
            int* nodes = malloc(num_nodes*sizeof(int));
            for (int i = 0; i < num_nodes; i++) { nodes[i] = i; }
            int** edges = edges_constructor(slices[cont], num_rows, num_cols/num_slices);
            list_vec[cont] = adj_list_alloc(nodes, num_nodes, edges, num_edges, 0);
        }

    
    /////////////////////////////////
    //* MST analysis/manipulation *//
    /////////////////////////////////

    adj_list** mst_vec = malloc(num_slices*sizeof(adj_list*));

    #pragma omp parallel for num_threads(num_threads)
        for (int cont = 0; cont<num_slices; cont++) { 
            int** mst_edges = malloc((num_nodes - 1)*sizeof(int*));
            int mst_cost = prim(list_vec[cont], mst_edges);
            int* mst_nodes = malloc(num_nodes*sizeof(int));
            for (int i = 0; i < num_nodes; i++) { mst_nodes[i] = i; }

            mst_vec[cont] = adj_list_alloc(mst_nodes, num_nodes, mst_edges, num_nodes - 1, 0);
        }

    int* centroid_vec = malloc(num_slices*sizeof(int));
    int*** new_edges_vec = malloc(num_slices*sizeof(int**));
    int** pred_vec =  malloc(num_slices*sizeof(int*));
    
    #pragma omp parallel for num_threads(num_threads)
        for (int cont = 0; cont<num_slices; cont++) {
            new_edges_vec[cont] = malloc((num_nodes - 1)*sizeof(int*));
            centroid_vec[cont] = centroid_search(mst_vec[cont]);
            pred_vec[cont] = direct_out_tree(mst_vec[cont], centroid_vec[cont], new_edges_vec[cont]);
        }

    
    /////////////////////////    
    //* Storage reduction *//
    /////////////////////////
    
    adj_list** opt_vec = malloc(num_slices*sizeof(adj_list*));

    #pragma omp parallel for num_threads(num_threads)
        for (int cont = 0; cont<num_slices; cont++) {
            int* nodes = malloc(num_nodes*sizeof(int));
            for (int i = 0; i < num_nodes; i++) { nodes[i] = i; }
            opt_vec[cont] = adj_list_alloc(nodes, num_nodes, new_edges_vec[cont], num_nodes - 1, 1);
        }

    opt_str** opt_str_vec = malloc(num_slices*sizeof(opt_str*));

    #pragma omp parallel for num_threads(num_threads)
        for (int cont=0; cont<num_slices; cont++) {
            opt_str_vec[cont] = opt_matrix_constructor(slices[cont], num_rows, num_cols/num_slices, pred_vec[cont], centroid_vec[cont]);
        }

    
    ///////////////    
    //* Storage *//
    ///////////////
    
    vec_str_effic(opt_str_vec, num_slices, matrix);
    save_vec_opt_str(opt_str_vec, num_slices, filename);


    /////////////////
    //* Free zone *//
    /////////////////

    for (int cont=0; cont<num_slices; cont++) {
        free_matrix(slices[cont], num_rows);
    }
    free(slices);

    for (int cont = 0; cont<num_slices; cont++) { 
        adj_list_free(list_vec[cont]);
        adj_list_free(mst_vec[cont]);
        adj_list_free(opt_vec[cont]);
        free(pred_vec[cont]);
        opt_str_free(opt_str_vec[cont]);
    }
    free(list_vec); free(mst_vec); free(pred_vec); free(centroid_vec); free(new_edges_vec); free(opt_vec); free(opt_str_vec);

}

int** load_data(const char* filename, int* p_num_rows, int* p_num_cols) {

    int num_threads = omp_get_max_threads(); // Number of available CPU's cores


    ////////////////////
    //* Loading data *//
    ////////////////////

    int num_slices;
    opt_str** opt_str_vec = load_vec_opt_str(filename, &num_slices);

    *p_num_rows = (opt_str_vec[0])->num_rows;
    *p_num_cols = num_slices * (opt_str_vec[0])->size[(opt_str_vec[0])->centroid];


    ///////////////////////////
    //* Data reconstruction *//
    ///////////////////////////

    int*** slices = malloc(num_slices*sizeof(int**));
    #pragma omp parallel for num_threads(num_threads)
        for (int cont = 0; cont<num_slices; cont++) {
            slices[cont] = reconstruct_matrix(opt_str_vec[cont]);
        }
    int** matrix = matrix_join(slices, num_slices, *p_num_rows, *p_num_cols);
    
    printf("\n[Matrix reconstruction was successful]\n\n");

    /////////////////
    //* Free zone *//
    /////////////////

    for (int cont=0; cont<num_slices; cont++) {
        free_matrix(slices[cont], *p_num_rows);
        opt_str_free(opt_str_vec[cont]);
    }
    free(slices); free(opt_str_vec);

    return matrix;
}
