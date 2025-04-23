#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "probsched.h"

#define MAX_PROCESSES 5

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "probsched.h"

#define MAX_PROCESSES 5

typedef struct
{
    char algorithm[20];
    int n;
    int quantum;
    double lambda_arrival;
    double lambda_burst;
} Config;

void load_config(const char *filename, Config *config)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Erro ao abrir ficheiro de configuração");
        exit(1);
    }

    char key[50], value[50];
    while (fscanf(file, "%49[^=]=%49s\n", key, value) == 2)
    {
        if (strcmp(key, "algorithm") == 0)
            strcpy(config->algorithm, value);
        else if (strcmp(key, "n") == 0)
            config->n = atoi(value);
        else if (strcmp(key, "quantum") == 0)
            config->quantum = atoi(value);
        else if (strcmp(key, "lambda_arrival") == 0)
            config->lambda_arrival = atof(value);
        else if (strcmp(key, "lambda_burst") == 0)
            config->lambda_burst = atof(value);
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso: %s config.txt\n", argv[0]);
        return 1;
    }

    Config config;
    load_config(argv[1], &config);

    seed_rng(time(NULL));

    Process *processes[config.n];

    for (int i = 0; i < config.n; i++)
    {
        int arrival = poisson_random(config.lambda_arrival);
        int burst = exponential_random(config.lambda_burst) + 1;
        int priority = uniform_random(1, 5);
        processes[i] = create_process(i, arrival + i, burst, priority);
        processes[i]->deadline = arrival + burst + uniform_random(5, 15);
        processes[i]->period = uniform_random(10, 20);
        print_process(processes[i]);
    }

    printf("\n--- Algoritmo Selecionado: %s ---\n", config.algorithm);

    if (strcmp(config.algorithm, "FCFS") == 0)
        fcfs(processes, config.n);
    else if (strcmp(config.algorithm, "SJF") == 0)
        sjf(processes, config.n);
    else if (strcmp(config.algorithm, "PRIORITY_NP") == 0)
        priority_np(processes, config.n);
    else if (strcmp(config.algorithm, "PRIORITY_P") == 0)
        priority_p(processes, config.n);
    else if (strcmp(config.algorithm, "RR") == 0)
        round_robin(processes, config.n, config.quantum);
    else if (strcmp(config.algorithm, "RM") == 0)
        rate_monotonic(processes, config.n);
    else if (strcmp(config.algorithm, "EDF") == 0)
        edf(processes, config.n);
    else
        printf("Algoritmo desconhecido.\n");

    for (int i = 0; i < config.n; i++)
    {
        free(processes[i]);
    }

    return 0;
}
