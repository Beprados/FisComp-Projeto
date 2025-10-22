#ifndef ADJLIST_H
#define ADJLIST_H

#include <stdio.h>
#include <stdlib.h>

///////////////////////
// Estrutura para nó //
///////////////////////

struct node{

    int out;
    int cost;
    struct node * prev;
    struct node * next;

};
typedef struct node node;

///////////////////////////////////////////
// Estrutura para lista duplamente ligada//
///////////////////////////////////////////

struct db_list{

    int size;
    struct node* head;
    struct node* last;

};
typedef struct db_list db_list;


///////////////////////////////////////
// Estrutura para lista de adjacência//
///////////////////////////////////////

struct adj_list{

    int* nodes;
    int num_nodes;

    int** edges;
    int num_edges;

    int directed;

    struct db_list ** adj;
};
typedef struct adj_list adj_list;

/*Aloca memória para um nó de lista duplamente ligada*/
node* node_alloc(int out, int cost);

/*Libera memória de um nó de lista duplamente ligada*/
void node_free(node* node);

/*Aloca memória para uma lista duplamente ligada*/
db_list* db_list_alloc();

/*Libera memória de uma lista duplamente ligada*/
void db_list_free(db_list* list);

/*Adiciona um nó à lista duplamente ligada*/
void db_list_push(db_list* list, node* node);

/*Aloca memória para uma lista de adjacência*/
adj_list* adj_list_alloc(int* nodes, int num_nodes, int** edges,int num_edges, int directed);

/*Libera memória de uma lista de adjacência*/
void adj_list_free(adj_list* list);

/*Adiciona uma aresta à lista de adjacência*/
void adj_list_add_edge(adj_list* list, int* edge);

/*Constroi a lista de adjacência a partir das arestas*/
void adj_list_construct(adj_list* list);

/*Exibe a lista de adjacência*/
void adj_list_display(adj_list* list);

#endif