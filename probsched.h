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
    int deadline;
    int period;
} Process;

// Funções de processos
Process *create_process(int id, int arrival, int burst, int priority);
void print_process(Process *p);
void reset_processes(Process **processes, int n);

typedef struct
{
    double avg_waiting_time;
    double avg_turnaround_time;
    double cpu_utilization;
    double throughput;
    int missed_deadlines;
} SchedulingStats;

SchedulingStats calculate_stats(Process **processes, int n, int total_time);
void print_stats(const SchedulingStats *stats);

// Algoritmos de escalonamento
void fcfs(Process **processes, int n);
void sjf(Process **processes, int n);
void priority_np(Process **processes, int n);
void priority_p(Process **processes, int n);
void round_robin(Process **processes, int n, int quantum);
void rate_monotonic(Process **processes, int n);
void edf(Process **processes, int n);


// Geração de números aleatórios
int poisson_random(double lambda);
int exponential_random(double lambda);
int normal_random(double mean, double stddev);
int uniform_random(int min, int max);
void seed_rng(unsigned int seed);

//estrutura de configuracao
typedef struct 
{
    char algorithm[20];
    int n;
    int quantum;
    double lambda_arrival;
    double lambda_burst;
    char mode[10]; //random ou static
    char input_file[50]; //ficheiro com processos
}Config;

//Leitura de ficheiro com lista estatica de processos
int load_static_processes(const char *filename, Process **processes);


#endif
