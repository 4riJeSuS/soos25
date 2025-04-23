#include <stdlib.h>
#include <stdio.h>
#include "probsched.h"

Process *create_process(int id, int arrival, int burst, int priority)
{
    Process *p = (Process *)malloc(sizeof(Process));
    p->id = id;
    p->arrival_t = arrival;
    p->burst_t = burst;
    p->prio = priority;
    p->remaining_t = burst;
    p->start_t = -1;
    p->end_t = -1;
    return p;
}

void print_process(Process *p)
{
    printf("P%d [arrival=%d, burst=%d, priority=%d]\n", p->id, p->arrival_t, p->burst_t, p->prio);
}