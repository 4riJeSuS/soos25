#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "probsched.h"

// Carregar configurações do ficheiro
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
        else if (strcmp(key, "mode") == 0)
            strcpy(config->mode, value);
        else if (strcmp(key, "input_file") == 0)
            strcpy(config->input_file, value);
    }

    fclose(file);
}

// Executar um algoritmo com os processos dados
void run_algorithm(const char *name, Process **processes, int n, int quantum)
{
    printf("\n--- Algoritmo: %s ---\n", name);

    if (strcmp(name, "FCFS") == 0)
        fcfs(processes, n);
    else if (strcmp(name, "SJF") == 0)
        sjf(processes, n);
    else if (strcmp(name, "PRIORITY_NP") == 0)
        priority_np(processes, n);
    else if (strcmp(name, "PRIORITY_P") == 0)
        priority_p(processes, n);
    else if (strcmp(name, "RR") == 0)
        round_robin(processes, n, quantum);
    else if (strcmp(name, "RM") == 0)
        rate_monotonic(processes, n);
    else if (strcmp(name, "EDF") == 0)
        edf(processes, n);
    else
        printf("Algoritmo desconhecido: %s\n", name);
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

    Process *processes[100];
    int n = 0;

    // Gerar ou carregar processos
    if (strcmp(config.mode, "static") == 0)
    {
        n = load_static_processes(config.input_file, processes);
    }
    else // random
    {
        n = config.n;
        for (int i = 0; i < n; i++)
        {
            int arrival = poisson_random(config.lambda_arrival);
            int burst = exponential_random(config.lambda_burst) + 1;
            int priority = uniform_random(1, 5);
            processes[i] = create_process(i, arrival + i, burst, priority);
            processes[i]->deadline = arrival + burst + uniform_random(5, 15);
            processes[i]->period = uniform_random(10, 20);
        }
    }

    // Imprimir os processos iniciais
    for (int i = 0; i < n; i++)
    {
        print_process(processes[i]);
    }

    // Modo comparação
    if (strcmp(config.algorithm, "ALL") == 0)
    {
        const char *algorithms[] = {
            "FCFS", "SJF", "PRIORITY_NP", "PRIORITY_P", "RR", "RM", "EDF"};
        int num_algorithms = sizeof(algorithms) / sizeof(algorithms[0]);

        // Guardar cópia original
        Process *original[100];
        for (int i = 0; i < n; i++)
        {
            original[i] = create_process(
                processes[i]->id,
                processes[i]->arrival_t,
                processes[i]->burst_t,
                processes[i]->prio);
            original[i]->deadline = processes[i]->deadline;
            original[i]->period = processes[i]->period;
        }

        for (int i = 0; i < num_algorithms; i++)
        {
            // Restaurar os processos
            for (int j = 0; j < n; j++)
            {
                processes[j]->arrival_t = original[j]->arrival_t;
                processes[j]->burst_t = original[j]->burst_t;
                processes[j]->prio = original[j]->prio;
                processes[j]->deadline = original[j]->deadline;
                processes[j]->period = original[j]->period;
                processes[j]->remaining_t = original[j]->burst_t;
            }

            reset_processes(processes, n);
            run_algorithm(algorithms[i], processes, n, config.quantum);
        }

        for (int i = 0; i < n; i++)
            free(original[i]);
    }
    else
    {
        reset_processes(processes, n);
        run_algorithm(config.algorithm, processes, n, config.quantum);
    }

    for (int i = 0; i < n; i++)
    {
        free(processes[i]);
    }

    return 0;
}
