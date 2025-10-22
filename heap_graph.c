#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "heap_graph.h"

int is_prime(int n)
{

    if (n <= 1) {
	return 0;
    } else if (n % 2 == 0) {
	return 0;
    }

    int aux = ceil(sqrt(n));
    for (int i = 3; i <= aux; i += 2) {
	if (n % i == 0) {
	    return 0;
	}
    }

    return 1;
}

int sup_prime(int n)
{

    int aux = n;
    if (aux % 2 == 0) {
	aux += 1;
    }

    while (!is_prime(aux)) {
	aux += 2;
    }

    return aux;
}

int inf_prime(int n) {

    int aux = n;

    if (aux <= 1) { return 1; }

    if (aux % 2 == 0) {
	    aux -= 1;
    }

    while (aux > 1 && !is_prime(n)) {
        aux -= 2;
    }

    return aux;

}


hash_node* hash_node_alloc(int key, int index) {

    hash_node* n = malloc(sizeof(hash_node));
    n->key = key;
    n->index = index;

    return n;
}

hash* hash_alloc(int size) {

    if (!is_prime(size)) {
	    size = sup_prime(size);
    }

    hash *h = malloc(sizeof(hash));
    h->size = size;
    h->occup = 0;
    h->tab = calloc(size, sizeof(hash_node*));

    return h;
}

int hash_func1(int k, int n) {
    return k % n;
}

int hash_func2(int k, int n) {
    return 1 + (k % (n - 2));
}

void hash_free(hash * h) {

    hash_node** tab = h->tab;
    int n = h->size;
    hash_node *curr_node;
    hash_node *aux;
    for (int cont = 0; cont < n; cont++) {	// Entradas da tabela são lidas sequencialmente
        curr_node = tab[cont];
        if (curr_node) {	// Encontra-se posição ocupada por um ou mais nós
            aux = curr_node;
            free(aux);
	    }
    }

    free(tab);
    free(h);

    return;
}

hash_node* hash_search( hash* h, int key) {

    hash_node** tab = h->tab;
    int n = h->size;

    int id1 = hash_func1(key, n);
    int id2 = hash_func2(key, n);
    int id = id1 % n;

    int cont = 1;
    while (tab[id] && cont < n) {
        if (tab[id]->key == key) {
            return tab[id];
        }
        id = (id1 + (cont * id2)) % n;
        cont++;
    }

    return NULL;
}

void hash_insert(hash* h, int key, int index) {

    if (hash_search(h, key)) { // A chave já está na tabela hash
        return;
    }

    hash_node** tab = h->tab;
    int n = h->size;

    int id1 = hash_func1(key, n);
    int id2 = hash_func2(key, n);
    int id;

    for (int cont = 0; cont < n; cont++) {	// Averigua-se caso a chave já esteja na tabela
        
        id = (id1 + (cont * id2)) % n;

        if (!tab[id] || tab[id]->key == -1) {

            hash_node *n = hash_node_alloc(key, index);
            tab[id] = n;
            h->occup += 1;

            return;
        }
    }

    return;
}

void hash_remove(hash* h, int key)
{

    hash_node *t = hash_search(h, key);
    if (t) {
        t->key = -1;
        h->occup -= 1;
    }

    return;
}


heap_node* heap_node_alloc(int key, int* edge) {

    heap_node* n = malloc(sizeof(heap_node));

    n->key = key;
    n->edge = edge;
    n->val = edge[2];

    return n;
}

heap* heap_alloc(int size) {

    heap* h = malloc(sizeof(heap));

    h->size = size;
    h->occup = 0;
    h->heap_vec = calloc(size, sizeof(heap_node*));

    return h;
}

void exch(heap_node** pA, heap_node** pB) {

    heap_node* temp = *pA;
    *pA = *pB;
    *pB = temp;

    return;
}

void heap_fix_up(heap* h, int k) {

    if (h->occup == 0) {
        return;
    }

    heap_node** heap_vec = h->heap_vec;

    while (k>0 && (heap_vec[(k-1)/2]->val > heap_vec[k]->val || (heap_vec[(k-1)/2]->val == heap_vec[k]->val && heap_vec[(k-1)/2]->key > heap_vec[k]->key))) {
        exch(&heap_vec[(k-1)/2], &heap_vec[k]);
        k = (k-1)/2;
    }

    return;
}

void heap_fix_down(heap* h, int k) {

    heap_node** heap_vec = h->heap_vec;
    int n = h->occup;

    if (n == 0) {
        return;
    }

    int j;
    while (2*k + 1 <= n - 1) {
        j = 2*k + 1;
        if (j+1 < n && heap_vec[j]->val > heap_vec[j+1]->val) {
            j ++;
        }
        if (heap_vec[k]->val < heap_vec[j]->val) {
            break;
        }

        exch(&heap_vec[k], &heap_vec[j]);
        k = j;
    }

    return;
}

heap_node* heap_extract_min(heap* h) {

    heap_node** heap_vec = h->heap_vec;
    heap_node* min = heap_vec[0];
    int n = h->occup;
    
    if (n == 0) {
        return NULL;
    }

    exch(&heap_vec[0], &heap_vec[n-1]);
    h->occup -= 1;
    heap_fix_down(h, 0);

    return min;
}

void heap_insert(heap* h, heap_node* new_node) {

    heap_node** heap_vec = h->heap_vec;
    int n = h->occup;

    heap_vec[n] = new_node;
    h->occup += 1;
    
    if (h->occup > 1) {
        heap_fix_up(h, n);
    }
    
    return;
}

void heap_decrease(heap* h, int k, int new_val) {

    heap_node** heap_vec = h->heap_vec;
    int n = h->occup;

    if (n == 0) {
        return;
    }

    heap_vec[k]->val = new_val;
    (heap_vec[k]->edge)[2] = new_val;
    heap_fix_up(h, k);

    return;
}

void heap_free(heap* h) {

    heap_node** heap_vec = h->heap_vec;

    for (int pos = 0; pos < h->size; pos++) {
        if (!heap_vec[pos]) {
            continue;
        }
        free(heap_vec[pos]->edge);
        free(heap_vec[pos]);
    }

    free(h->heap_vec);
    free(h);

    return;
}

void heap_semi_free(heap* h) {
    
    heap_node** heap_vec = h->heap_vec;

    for (int pos = 0; pos < h->size; pos++) {
        if (!heap_vec[pos]) {
            continue;
        }
        free(heap_vec[pos]);
    }

    free(h->heap_vec);
    free(h);

    return;
}

void heap_display(heap* h) {

    heap_node** heap_vec = h->heap_vec;
    int* edge;
    int n = h->occup;

    printf("\n*** Min-Heap ***\n\n");

    for (int pos = 0; pos < n; pos++) {
        edge = heap_vec[pos]->edge;
        printf("Heap %d-th elem.: [%d, %d, *%d*]\n", pos, edge[0], edge[1], edge[2]);
    }
    printf("\n");

    return;
}