#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "heap_graph.h"
#include "adjlist.h"

#include "mst.h"

int prim(adj_list* list, int** mst_edges) {

    int mst_cost = 0;
    int edges_cont = 0;

    hash* visited = hash_alloc(list->num_nodes);
    hash_insert(visited, 0, 0);

    hash* unvisited = hash_alloc(list->num_nodes);
    for (int i = 1; i < list->num_nodes; i++) {
        hash_insert(unvisited, i, i);
    }

    int size = list->num_edges;
    int edge_id = 0;
    heap* fringe = heap_alloc(size);

    node* temp = list->adj[0]->head;
    while(temp != NULL) {
        int* edge = malloc(3*sizeof(int));
        edge[0] = 0;
        edge[1] = temp->out;
        edge[2] = temp->cost;
        heap_insert(fringe, heap_node_alloc(edge_id++, edge));
        temp = temp->next;
    }
    
    int start_node, end_node, edge_cost;

    while (unvisited->occup > 0) { 

        while (fringe->occup > 0 && 
               hash_search(visited, fringe->heap_vec[0]->edge[1]) != NULL) {

                heap_node* discarded = heap_extract_min(fringe);
                if (discarded) {
                    free(discarded->edge);  // Libera o heap_node
                }
        }

        heap_node* min = heap_extract_min(fringe);
        start_node = min->edge[0];
        end_node = min->edge[1];
        edge_cost = min->edge[2];

        mst_cost += edge_cost;
        mst_edges[edges_cont] = min->edge;
        edges_cont += 1;

        hash_remove(unvisited, end_node);
        hash_insert(visited, end_node, 0);

        node* temp = list->adj[end_node]->head;
        while (temp != NULL) {
            if (hash_search(unvisited, temp->out) != NULL) {
                int* edge = malloc(3*sizeof(int));
                edge[0] = end_node;
                edge[1] = temp->out;
                edge[2] = temp->cost;
                heap_insert(fringe, heap_node_alloc(edge_id++, edge));
            }
            temp = temp->next;
        }

    }

    while (fringe->occup > 0) {
        heap_node* remaining = heap_extract_min(fringe);
        if (remaining) {
            free(remaining->edge);  // Libera o heap_node
        }
    }

    hash_free(visited);
    hash_free(unvisited);
    heap_semi_free(fringe); 
    
    return mst_cost;
}

void descend_search(adj_list* list, int* pred, int curr_node, int* descend, int* p_centroid) {

    node* edge = list->adj[curr_node]->head;
    int next_node;

    while(edge != NULL) {

        next_node = edge->out;
        if (next_node == pred[curr_node]) {
            if (list->adj[curr_node]->size > 1) {
                edge = edge->next;
                continue;
            }
            else {
                descend[curr_node] += 1;
                return;
            }
        }

        pred[next_node] = curr_node;
        descend_search(list, pred, next_node, descend, p_centroid);
        descend[curr_node] += descend[next_node];

        edge = edge->next;
    }

    descend[curr_node] += 1;
    if (descend[curr_node] >= list->num_nodes/2 && *p_centroid == -1) {
        *p_centroid = curr_node;
    }
    
}

int centroid_search(adj_list* list) {

    int* descend = malloc(list->num_nodes*sizeof(int));
    int* pred = malloc(list->num_nodes*sizeof(int));

    for (int i = 0; i < list->num_nodes; i++) {
        descend[i] = 0;
        pred[i] = -1;
    }

    int centroid = -1;
    descend_search(list, pred, 0, descend, &centroid);

    free(descend);
    free(pred);

    return centroid;
}

int* direct_out_tree(adj_list* list, int centroid,  int** new_edges) {

    int* pred = malloc(list->num_nodes*sizeof(int));
    for (int i = 0; i < list->num_nodes; i++) {
        pred[i] = -1;
    }
    pred[centroid] = centroid;

    int* aux_vec = malloc(list->num_nodes*sizeof(int));
    aux_vec[0] = centroid;
    int beg = 0;
    int end = 1;
    int size = 1;

    int cont_edges = 0;

    while (size > 0) {
        int curr_node = aux_vec[beg++];
        size --;
        node* edge = list->adj[curr_node]->head;
        while (edge != NULL) {
            int next_node = edge->out;
            if (pred[next_node] == -1) {
                
                pred[next_node] = curr_node;
                aux_vec[end++] = next_node;
                size ++;
                
                new_edges[cont_edges] = malloc(3*sizeof(int));
                new_edges[cont_edges][0] = curr_node;
                new_edges[cont_edges][1] = next_node;
                new_edges[cont_edges][2] = edge->cost;
                cont_edges++;
            }
            edge = edge->next;
        }
    }

    free(aux_vec);

    return pred;
}   

