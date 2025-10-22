#ifndef MST_H
#define MST_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "heap_graph.h"
#include "adjlist.h"

int prim(adj_list* list, int** mst_edges);

void descend_search(adj_list* list, int* pred, int curr_node, int* descend, int* p_centroid);

int centroid_search(adj_list* list);

int* direct_out_tree(adj_list* list, int centroid,  int** new_edges);

#endif