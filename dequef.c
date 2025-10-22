#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "dequef.h"


/**
   Create an empty deque of ints.

   capacity is both the initial and minimum capacity.
   factor is the resizing factor, larger than 1.0.

   On success it returns the address of a new dequef.
   On failure it returns NULL.
**/
dequef *df_alloc(int capacity, double factor)
{

    dequef *deque;

    deque = malloc(sizeof(dequef));

    if (deque) {
	deque->cap = capacity;
	deque->mincap = capacity;
	deque->factor = factor;
	deque->size = 0;
	deque->data = malloc(capacity * sizeof(int));
    }

    return deque;
}


/**
  The index of the last element of a dequef.
**/
int last_index(dequef * D)
{

    int l_index;

    if (D->size != 0) {

	l_index = (D->first + D->size - 1) % (D->cap);
    }

    else {
	l_index = -1;
    }

    return l_index;
}

/** 
  Copies elements from a vector of ints A into another vector B, from index i to j (i<=j).
**/
void copy_elem(int *B, int *A, int start_index_A, int final_index_A,
	       int start_index_B)
{

    for (int k = 0; k <= final_index_A - start_index_A; k++) {
	B[start_index_B + k] = A[start_index_A + k];
    }

    return;
}


/** 
  Linearly rearranges the data array from a deque into a new array.
**/
void rearr(int *new_arr, dequef * D, int D_last_index)
{

    if (D_last_index < D->first) {

	copy_elem(new_arr, D->data, D->first, D->cap - 1, 0);
	copy_elem(new_arr, D->data, 0, D_last_index, D->cap - D->first);

    }

    else {

	copy_elem(new_arr, D->data, D->first, D_last_index, 0);

    }

    return;
}


/**
  Release a dequef and its data.
**/
void df_free(dequef * D)
{

    free(D->data);
    free(D);

    return;
}


/**
   The size of the deque.
**/
int df_size(dequef * D)
{

    return D->size;
}



/**
   Add x to the end of D.

   If the array is full, it first tries to increase the array size to
   capacity*factor.

   On success it returns 1.
   If attempting to resize the array fails then it returns 0 and D remains unchanged.
**/
int df_push(dequef * D, int x)
{

    int l_index = last_index(D);

    if (D->size == D->cap) {

	int new_cap = (D->cap) * (D->factor);
	int *new_arr = malloc(new_cap * sizeof(int));

	if (!new_arr) {
	    return 0;
	}
	// printf("\nExpansion time\n");
	rearr(new_arr, D, l_index);

	free(D->data);

	D->first = 0;
	D->data = new_arr;
	(D->data)[D->size] = x;
	D->cap = new_cap;

    }

    else {

	if (D->size == 0) {
	    (D->data)[0] = x;
	    D->first = 0;
	}

	else if (l_index == D->cap - 1) {
	    (D->data)[0] = x;
	}

	else {
	    (D->data)[l_index + 1] = x;
	}

    }

    D->size += 1;

    return 1;
}



/**
   Remove a int from the end of D.

   If the deque has capacity/(factor^2) it tries to reduce the array size to
   capacity/factor.  If capacity/factor is smaller than the minimum capacity,
   the minimum capacity is used instead.  If it is not possible to resize, then
   the array size remains unchanged.

   This function returns the int removed from D.
   If D was empty prior to invocation, the returned int is meaningless.
**/
int df_pop(dequef * D)
{

    if (D->size == 0) {
	return 0;
    }

    int l_index = last_index(D);
    int l_elem = (D->data)[l_index];

    if (D->size == ceil((D->cap) / ((D->factor) * (D->factor)))) {

	int new_cap = ceil((D->cap) / (D->factor));
	if (new_cap < D->mincap) {
	    new_cap = D->mincap;
	}

	int *new_arr = malloc(new_cap * sizeof(int));
	if (!new_arr) {
	    return l_elem;
	}

	rearr(new_arr, D, l_index);
	// printf("\nContraction time\n");
	free(D->data);

	D->first = 0;
	D->data = new_arr;
	D->cap = new_cap;

    }

    D->size -= 1;

    return l_elem;
}



/**
   Add x to the beginning of D.

   If the array is full, it first tries to increase the array size to
   capacity*factor.

   On success it returns 1.
   If attempting to resize the array fails then it returns 0 and D remains unchanged.
**/
int df_inject(dequef * D, int x)
{

    if (D->size == (D->cap)) {

        int new_cap = (D->cap) * (D->factor);
        int *new_arr = malloc(new_cap * sizeof(int));
        if (!new_arr) {
            return 0;
        }

        int l_index = last_index(D);

        rearr(new_arr, D, l_index);
        // printf("\nExpansion time\n");
        free(D->data);

        D->first = new_cap - 1;
        D->data = new_arr;
        (D->data)[new_cap - 1] = x;
        D->cap = new_cap;

    }

    else {

	if (D->size == 0) {

	    (D->data)[0] = x;
	    D->first = 0;
	}

	else if (D->first == 0) {

	    (D->data)[D->cap - 1] = x;
	    D->first = D->cap - 1;
	}

	else {

	    (D->data)[D->first - 1] = x;
	    D->first -= 1;
	}

    }

    D->size += 1;

    return 1;
}


/**
   Remove a int from the beginning of D.

   If the deque has capacity/(factor^2) elements, this function tries to reduce
   the array size to capacity/factor.  If capacity/factor is smaller than the
   minimum capacity, the minimum capacity is used instead.

   If it is not possible to resize, then the array size remains unchanged.

   This function returns the int removed from D.
   If D was empty prior to invocation, the returned int is meaningless.
**/
int df_eject(dequef * D)
{

    if (D->size == 0) {
	return 0;
    }

    int f_elem = (D->data)[D->first];

    if (D->size == ceil((D->cap) / ((D->factor) * (D->factor)))) {

	int new_cap = ceil((D->cap) / (D->factor));
	if (new_cap < D->mincap) {
	    new_cap = D->mincap;
	}

	int *new_arr = malloc(new_cap * sizeof(int));
	if (!new_arr) {
	    return 0;
	}

	int l_index = last_index(D);
	// printf("\nContraction time\n");
	rearr(new_arr, D, l_index);

	free(D->data);

	D->first = 1;
	D->data = new_arr;
	D->cap = new_cap;

    }

    else {

	if (D->first == D->cap - 1) {
	    D->first = 0;
	}

	else {
	    D->first += 1;

	}

    }

    D->size -= 1;

    return f_elem;
}


/**
   Set D[i] to x.

   If i is not in [0,|D|-1]] then D remains unchanged.
**/
void df_set(dequef * D, int i, int x)
{

    if ((0 > i) || (i > (D->size) - 1)) {
	return;
    }

    int rel_index = (D->first + i) % (D->cap);
    (D->data)[rel_index] = x;

    return;
}


/**
   Return D[i].

   If i is not in [0,|D|-1]] the returned int is meaningless.
**/
int df_get(dequef * D, int i)
{

    if ((0 > i) || (i > (D->size) - 1)) {
	return 0;
    }

    int rel_index = (D->first + i) % (D->cap);

    return (D->data)[rel_index];
}


/**
   Print the elements of D in a line.
**/
void df_print(dequef * D)
{

    int l_index = last_index(D);

    if (D->size > 0) {
    printf("\n\n*** Deque ***\n\n");
	printf("Size : %d\n", D->size);
    printf("Capacity : %d\n", D->cap);
    printf("First elem. index: %d\n", D->first);
    printf("Deque data: ");

	if (l_index < D->first) {
	    for (int i = D->first; i <= D->cap - 1; i++) {
		printf("%d ", (D->data)[i]);
	    }

	    for (int i = 0; i <= l_index - 1; i++) {
		printf("%d ", (D->data)[i]);
	    }
	    printf("%d", (D->data)[l_index]);
	}

	else {
	    for (int i = D->first; i <= l_index - 1; i++) {
		printf("%d ", (D->data)[i]);
	    }
	    printf("%d", (D->data)[l_index]);
	}

    }

    else {
	printf("deque (%d):", D->size);
    }

    printf("\n");

    return;
}
