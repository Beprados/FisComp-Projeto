#ifndef HEAP_GRAPH_H
#define HEAP_GRAPH_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Função que determina se um número natural n é primo, ou não
int is_prime(int n);
// Função que encontra o primeiro número primo maior que um número n
int sup_prime(int n);
// Função que encontra o primeiro número primo menor que um número n
int inf_prime(int n);

//////////////////////////////
// Estruturas para min-heap //
//////////////////////////////

struct heap_node {
    int key;
    int val;
    int* edge;
};
typedef struct heap_node heap_node;

struct heap {

    int size;
    int occup;
    heap_node** heap_vec;

};
typedef struct heap heap;

//////////////////////////
// Estruturas para hash //
//////////////////////////

struct hash_node {
    int key;
    int index;
};
typedef struct hash_node hash_node;

struct hash {
    int size;
    int occup;
    hash_node** tab;
};
typedef struct hash hash;

///////////////////////
// Funções para hash //
///////////////////////

hash_node* hash_node_alloc(int key, int index);

hash* hash_alloc(int size);

int hash_func1(int k, int n);

int hash_func2(int k, int n);

void hash_free(hash * h);

hash_node* hash_search( hash* h, int key);

void hash_insert(hash* h, int key, int index);

void hash_remove(hash* h, int key);

///////////////////////////
// Funções para min-heap //
///////////////////////////

heap_node* heap_node_alloc(int key, int* edge);

heap* heap_alloc(int size);

void exch(heap_node** pA, heap_node** pB);

void heap_fix_up(heap* h, int k);            

void heap_fix_down(heap* h, int k);

heap_node* heap_extract_min(heap* h);

void heap_insert(heap* h, heap_node* new_node);

void heap_decrease(heap* h, int k, int new_val);

void heap_free(heap* h);    

void heap_semi_free(heap* h); 

void heap_display(heap* h);

#endif