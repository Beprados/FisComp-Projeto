#include <stdlib.h>
#include <stdio.h>
#include "adjlist.h"

node* node_alloc(int out, int cost) {

    node* new_node = malloc(sizeof(node));
    new_node->out = out;
    new_node->cost = cost;
    new_node->prev = new_node->next = NULL;

    return new_node;
}

void node_free(node* node) {

    free(node);

}

db_list* db_list_alloc() {

    db_list* new_list = malloc(sizeof(db_list));
    new_list->size = 0;
    new_list->head = new_list->last = NULL;

    return new_list;
}

void db_list_free(db_list* list) {
    
    node* temp = list->head;
    while (temp != NULL) {
        node* next = temp->next;
        node_free(temp);
        temp = next;
    }
    free(list);
}
 
void db_list_push(db_list* list, node* node) {

    if (list->size == 0) {
        list->head = list->last = node;
    }
    else {
        struct node * temp = list->last;
        temp->next = node;
        node->prev = temp;
        list->last = node;
    }

    list->size ++;

}

adj_list* adj_list_alloc(int* nodes, int num_nodes, int** edges,int num_edges, int directed) {

    adj_list* new_adj = malloc(sizeof(adj_list));
    new_adj->num_nodes = num_nodes;
    new_adj->num_edges = num_edges;
    new_adj->nodes = nodes;
    new_adj->edges = edges;
    new_adj->directed = directed;
    new_adj->adj = malloc(num_nodes * sizeof(db_list*));
    for (int i = 0; i<num_nodes; i++) {
        new_adj->adj[i] = db_list_alloc();
    }
    adj_list_construct(new_adj);

    return new_adj;
}

void adj_list_free(adj_list* list) {
    
    for (int i = 0; i<list->num_nodes; i++) { // Libera lista de adjacência
        db_list_free(list->adj[i]);
    }
    free(list->nodes); // Libera nós do grafo
    for (int j = 0; j<list->num_edges; j++) { // Libera arestas do grafo
        free(list->edges[j]);
    }
    free(list->edges); // Libera vetor de arestas do grafo
    free(list->adj);    // Libera lista de adjacência
    free(list);        // Libera lista de adjacência
}

void adj_list_add_edge(adj_list* list, int* edge) {

    node* new_node = node_alloc(edge[1], edge[2]);
    db_list_push(list->adj[edge[0]], new_node);
    if (!(list->directed)) {
        new_node = node_alloc(edge[0], edge[2]);
        db_list_push(list->adj[edge[1]], new_node);
    }
}

void adj_list_construct(adj_list* list) {

    int** edges = list->edges;
    for (int j = 0; j<list->num_edges; j++) {
        adj_list_add_edge(list, edges[j]);
    }

}

void adj_list_display(adj_list* list) {

    printf("\n*** Adjacency List ***\n\n");

    for (int i=0; i<list->num_nodes; i++) {
        printf("%d :", i);
        node* temp = ((list->adj)[i])->head;
        while (temp != NULL) {
            printf("[%d, %d] ", temp->out, temp->cost);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\n");
}