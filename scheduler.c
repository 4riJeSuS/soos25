#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "probsched.h"

// Função para calcular o Máximo Divisor Comum (GCD)
int gcd(int a, int b)
{
    while (b != 0)
    {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Função para calcular o Mínimo Múltiplo Comum (LCM)
int lcm(int a, int b)
{
    return (a * b) / gcd(a, b);
}

void fcfs(Process **processes, int n)
{
    int current_time = 0;
    for (int i = 0; i < n; i++)
    {
        if (current_time < processes[i]->arrival_t)
            current_time = processes[i]->arrival_t;
        processes[i]->start_t = current_time;
        current_time += processes[i]->burst_t;
        processes[i]->end_t = current_time;
        printf("Processo P%d executado de %d a %d\n",
               processes[i]->id, processes[i]->start_t, processes[i]->end_t);
    }
    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}

void sjf(Process **processes, int n)
{
    int current_time = 0;
    int completed = 0;
    int *is_completed = (int *)calloc(n, sizeof(int));

    while (completed != n)
    {
        int shortest_job = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (!is_completed[i] && processes[i]->arrival_t <= current_time)
            {
                if (processes[i]->burst_t < min_burst)
                {
                    min_burst = processes[i]->burst_t;
                    shortest_job = i;
                }
            }
        }

        if (shortest_job == -1)
        {
            current_time++;
            continue;
        }

        Process *p = processes[shortest_job];
        p->start_t = current_time;
        p->end_t = current_time + p->burst_t;
        current_time = p->end_t;
        is_completed[shortest_job] = 1;
        completed++;

        printf("Processo P%d executado de %d a %d\n",
               p->id, p->start_t, p->end_t);
    }
    free(is_completed);

    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}

void priority_np(Process **processes, int n)
{
    int current_time = 0;
    int completed = 0;
    int *is_completed = (int *)calloc(n, sizeof(int));

    while (completed != n)
    {
        int highest_prio = -1;
        int max_prio = INT_MIN;

        for (int i = 0; i < n; i++)
        {
            if (!is_completed[i] && processes[i]->arrival_t <= current_time)
            {
                if (processes[i]->prio > max_prio)
                {
                    max_prio = processes[i]->prio;
                    highest_prio = i;
                }
            }
        }

        if (highest_prio == -1)
        {
            current_time++;
            continue;
        }

        Process *p = processes[highest_prio];
        p->start_t = current_time;
        p->end_t = current_time + p->burst_t;
        current_time = p->end_t;
        is_completed[highest_prio] = 1;
        completed++;

        printf("Processo P%d executado de %d a %d\n",
               p->id, p->start_t, p->end_t);
    }
    free(is_completed);

    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}

void round_robin(Process **processes, int n, int quantum)
{
    int current_time = 0;
    int completed = 0;
    int *remaining = (int *)malloc(n * sizeof(int));

    // Initialize remaining times
    for (int i = 0; i < n; i++)
    {
        remaining[i] = processes[i]->burst_t;
    }

    // Continue until all processes are completed
    while (completed < n)
    {
        int process_executed = 0;

        // Check each process in round robin fashion
        for (int i = 0; i < n; i++)
        {
            // Skip if process has no remaining time or hasn't arrived yet
            if (remaining[i] <= 0 || processes[i]->arrival_t > current_time)
            {
                continue;
            }

            // Calculate execution time for this quantum
            int exec_time = (remaining[i] < quantum) ? remaining[i] : quantum;

            // Set start time if first execution
            if (processes[i]->start_t == -1)
            {
                processes[i]->start_t = current_time;
            }

            // Execute process
            remaining[i] -= exec_time;
            current_time += exec_time;
            process_executed = 1;

            printf("Processo P%d executado de %d a %d (restante=%d)\n",
                   processes[i]->id,
                   current_time - exec_time,
                   current_time,
                   remaining[i]);

            // Check if process completed
            if (remaining[i] <= 0)
            {
                completed++;
                processes[i]->end_t = current_time;
            }
        }

        // If no process was executed, advance time
        if (!process_executed)
        {
            current_time++;
        }
    }

    free(remaining);

    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}

void rate_monotonic(Process **processes, int n)
{
    int current_time = 0;
    int total_lcm = processes[0]->period;

    // Ordena os processos pelo período (menor período = maior prioridade)
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j]->period > processes[j + 1]->period)
            {
                Process *temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Calcular o LCM de todos os períodos
    for (int i = 1; i < n; i++)
    {
        total_lcm = lcm(total_lcm, processes[i]->period);
    }

    // Simula a execução dos processos até o 2º LCM
    printf("Iniciando a execução do algoritmo Rate Monotonic...\n");
    while (current_time < total_lcm * 2)
    {
        int process_executed = 0;

        // Executa os processos no momento certo de acordo com o período
        for (int i = 0; i < n; i++)
        {
            if (current_time % processes[i]->period == 0) // Verifica se é o tempo correto para o processo
            {
                if (processes[i]->start_t == -1) // Se o processo ainda não foi iniciado
                {
                    processes[i]->start_t = current_time;
                }

                processes[i]->end_t = current_time + processes[i]->burst_t;
                printf("Processo P%d executado de %d a %d\n", processes[i]->id, current_time, processes[i]->end_t);
                current_time = processes[i]->end_t; // Avança o tempo para o final do processo
                process_executed = 1;
                break; // Executa apenas um processo por vez
            }
        }

        // Se não houver nenhum processo a ser executado no momento (teoricamente não deveria acontecer no RM)
        if (!process_executed)
        {
            current_time++; // Apenas avança o tempo
        }
    }

    // Exibe as estatísticas após a execução do algoritmo
    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}

void edf(Process **processes, int n)
{
    int current_time = 0;
    int completed = 0;
    int *is_completed = (int *)calloc(n, sizeof(int));

    while (completed != n)
    {
        int earliest = -1;
        int min_deadline = INT_MAX;

        for (int i = 0; i < n; i++)
        {
            if (!is_completed[i] && processes[i]->arrival_t <= current_time)
            {
                if (processes[i]->deadline < min_deadline)
                {
                    min_deadline = processes[i]->deadline;
                    earliest = i;
                }
            }
        }

        if (earliest == -1)
        {
            current_time++;
            continue;
        }

        Process *p = processes[earliest];
        p->start_t = current_time;
        p->end_t = current_time + p->burst_t;

        printf("Processo P%d executado de %d a %d\n",
               p->id, p->start_t, p->end_t);

        current_time = p->end_t;
        is_completed[earliest] = 1;
        completed++;
    }
    free(is_completed);

    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}

void priority_p(Process **processes, int n)
{
    int current_time = 0;
    int completed = 0;
    int *remaining = (int *)malloc(n * sizeof(int));
    int *is_completed = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
    {
        remaining[i] = processes[i]->burst_t;
    }

    while (completed < n)
    {
        int highest_prio = -1;
        int max_prio = INT_MIN;

        for (int i = 0; i < n; i++)
        {
            if (!is_completed[i] && processes[i]->arrival_t <= current_time)
            {
                if (processes[i]->prio > max_prio)
                {
                    max_prio = processes[i]->prio;
                    highest_prio = i;
                }
            }
        }

        if (highest_prio == -1)
        {
            current_time++;
            continue;
        }

        if (processes[highest_prio]->start_t == -1)
        {
            processes[highest_prio]->start_t = current_time;
        }

        remaining[highest_prio]--;
        printf("Tempo %d: Processo P%d executa (resta %d)\n", current_time, processes[highest_prio]->id, remaining[highest_prio]);
        current_time++;

        if (remaining[highest_prio] == 0)
        {
            processes[highest_prio]->end_t = current_time;
            is_completed[highest_prio] = 1;
            completed++;
        }
    }

    free(remaining);
    free(is_completed);

    SchedulingStats stats = calculate_stats(processes, n, current_time);
    print_stats(&stats);
}
