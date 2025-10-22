/**
  dequef.h
  Declarations for a deque of ints stored in a dynamic array.
**/


#ifndef DEQUEF_H
#define DEQUEF_H

typedef struct {

  int* data;  // The array of ints.

  int first;  // The index of the first element in the deque.
  int size;   // The deque size.

  int cap;     // The array size.
  int mincap;  // The minimum array size.
  double factor;  // The resizing factor.

} dequef;


dequef* df_alloc(int capacity, double factor);
void df_free(dequef* D);

int df_size(dequef* A);

int df_push(dequef* D, int x);
int df_pop(dequef* D);

int df_inject(dequef* D, int x);
int df_eject(dequef* D);

void df_set(dequef* D, int i, int x);
int df_get(dequef* D, int i);

void df_print(dequef* A);

#endif
