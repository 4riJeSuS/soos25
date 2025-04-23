#include <stdio.h>
#include "probsched.h"

void fcfs(Process **processes, int n)
{
    int time = 0;
    for (int i = 0; i < n; i++)
    {
        if (time < processes[i]->arrival_t)
            time = processes[i]->arrival_t;
        processes[i]->start_t = time;
        time += processes[i]->burst_t;
        processes[i]->end_t = time;
        printf("Processo P%d executado de %d a %d\n", processes[i]->id, processes[i]->start_t, processes[i]->end_t);
    }
}