#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include "adjlist.h"
#include "str_reduc.h"
#include "heap_graph.h"
#include "mst.h"
#include "dequef.h"

// TIMER structure
typedef struct {
    struct timeval start;
    const char* name;
} ScopedTimer;

// TIMER start
void timer_start(ScopedTimer* t, const char* name) {
    t->name = name ? name : "Timer";
    gettimeofday(&t->start, NULL);
}

// TIMER stop
void timer_stop(ScopedTimer* t) {
    struct timeval end;
    gettimeofday(&end, NULL);

    long seconds  = end.tv_sec  - t->start.tv_sec;
    long useconds = end.tv_usec - t->start.tv_usec;

    long total_micro = seconds * 1000000 + useconds;

    printf("[%s] : %ld µs\n", t->name, total_micro);
}

// Macros to facilitate
#define TIMER(name) ScopedTimer __timer; timer_start(&__timer, name)
#define TIMER_STOP() timer_stop(&__timer)
#define TIMER_AUTO() ScopedTimer __timer; timer_start(&__timer, __func__)


int main(void) {

    /* Testing data*/

    int num_rows, num_cols;
    int** matrix = load_matrix_binary(&num_rows, &num_cols, "data/in/original_matrix.bin");

    int num_nodes = num_rows;
    int* nodes = malloc(num_nodes*sizeof(int));
    for (int i = 0; i < num_nodes; i++) { nodes[i] = i; }
    int num_edges = (int) num_nodes*(num_nodes-1)/2;
    int** edges = edges_constructor(matrix, num_rows, num_cols);
    adj_list* list = adj_list_alloc(nodes, num_nodes, edges, num_edges, 0);


    /* MST */

    adj_list* mst_list;

    // MST search
    {   
        TIMER("MST construction");
        int** mst_edges = malloc((num_nodes - 1)*sizeof(int*));
        int mst_cost = prim(list, mst_edges);
        int* mst_nodes = malloc(num_nodes*sizeof(int));
        for (int i = 0; i < num_nodes; i++) { mst_nodes[i] = i; }

        mst_list = adj_list_alloc(mst_nodes, num_nodes, mst_edges, num_nodes - 1, 0);
        TIMER_STOP();
    }
    

    /* MST analysis/manipulation */

    int centroid;
    int** new_edges = malloc((num_nodes - 1)*sizeof(int*));
    int* pred;

    // MST centroid search
    {
        TIMER("Centroid search");
        centroid = centroid_search(mst_list);
        TIMER_STOP();
    }

    // Directing-out the MST
    {
        TIMER("Directing out");
        pred = direct_out_tree(mst_list, centroid, new_edges);
        TIMER_STOP();
    }

    adj_list* opt_list;
    int* new_nodes = malloc(num_nodes*sizeof(int));
    for (int i = 0; i < num_nodes; i++) { new_nodes[i] = i; }

    // MST adjacency list construction
    {
        TIMER("Optimal tree construction");
        opt_list = adj_list_alloc(new_nodes, num_nodes, new_edges, num_nodes - 1, 1);
        TIMER_STOP();   
    }
    
    // Optimal matrix construction
    opt_str* opt;
    {   
        TIMER("Optimal matrix construction");
        opt = opt_matrix_constructor(matrix, num_rows, num_cols, pred, centroid);
        TIMER_STOP();
    }
    
    // Data reconstruction
    int flag = 1;
    int num_threads = omp_get_max_threads();
    {
        TIMER("Data reconstruction");

        int** rec_matrix = reconstruct_matrix(opt);
        for (int i = 0; i < num_rows; i++) {
            if (memcmp(rec_matrix[i], matrix[i], num_cols*sizeof(int)) != 0) {
                flag *= 0;
            }
        }
        free_matrix(rec_matrix, num_rows);

        TIMER_STOP();
    }

    // Check if the reconstruction was successful
    printf("\nMatrix succesfully reconstructed: %s\n", flag ? "true" : "false");
    
    // Data storage
    if (flag) {
        str_effic(opt, matrix);
        save_opt_str(opt, "data/out/opt_str.bin");
    }

    // Freeing memory
    adj_list_free(list);
    adj_list_free(mst_list);
    adj_list_free(opt_list);
    free(pred);
    free_matrix(matrix, num_rows);
    opt_str_free(opt);

    return 0;
}