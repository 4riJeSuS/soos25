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

void reset_processes(Process **processes, int n)
{
    for (int i = 0; i < n; i++)
    {
        processes[i]->start_t = -1;
        processes[i]->end_t = -1;
        processes[i]->remaining_t = processes[i]->burst_t;
    }
}

int load_static_processes(const char *filename, Process **processes)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir lista estática de processos");
        exit(1);
    }

    int id, arrival, burst, prio;
    int i = 0;

    // Ler cada processo do arquivo
    while (fscanf(file, "%d %d %d %d", &id, &arrival, &burst, &prio) == 4)
    {
        processes[i] = create_process(id, arrival, burst, prio);
        
        // Calcular deadline e period
        processes[i]->deadline = arrival + burst + uniform_random(5, 15);
        processes[i]->period = uniform_random(10, 20);
        
        i++;  // Incrementar o índice do processo
    }

    fclose(file);
    
    return i;  // Retorna o número de processos lidos
}
