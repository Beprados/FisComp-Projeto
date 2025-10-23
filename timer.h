#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>


/* Timer structure for blocks of code */
typedef struct {
    struct timeval start;
    const char* name;
} ScopedTimer;

/* Starts timer named "name" */
void timer_start(ScopedTimer* t, const char* name);

/* Stops previously started timer */
void timer_stop(ScopedTimer* t);


#endif