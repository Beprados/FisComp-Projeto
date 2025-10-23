#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>

#include "timer.h"

void timer_start(ScopedTimer* t, const char* name) {
    t->name = name ? name : "Timer";
    gettimeofday(&t->start, NULL);
}

void timer_stop(ScopedTimer* t) {
    struct timeval end;
    gettimeofday(&end, NULL);

    long seconds  = end.tv_sec  - t->start.tv_sec;
    long useconds = end.tv_usec - t->start.tv_usec;

    long total_micro = seconds * 1000000 + useconds;

    printf("[%s] : %ld µs\n\n", t->name, total_micro);
}



