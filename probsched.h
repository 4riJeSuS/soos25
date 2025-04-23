#ifndef PROBSCHED_H
#define PROBSCHED_H

// Estrutura dos processos
typedef struct
{
    int id;
    int arrival_t;
    int burst_t;
    int prio;
    int remaining_t;
    int start_t;
    int end_t;
} Process;

// Funções de processos
Process *create_process(int id, int arrival, int burst, int priority);
void print_process(Process *p);

// Algoritmos de escalonamento
void fcfs(Process **processes, int n);

// Geração de números aleatórios
int poisson_random(double lambda);
int exponential_random(double lambda);
int normal_random(double mean, double stddev);
int uniform_random(int min, int max);
void seed_rng(unsigned int seed);

#endif
