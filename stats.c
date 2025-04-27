#include <stdio.h>
#include "probsched.h"

SchedulingStats calculate_stats(Process **processes, int n, int total_time)
{
    SchedulingStats stats = {0};
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_burst = 0;
    int missed = 0;
    int completed = 0;

    for (int i = 0; i < n; i++)
    {
        Process *p = processes[i];

        // Ignorar processos que não foram executados
        if (p->end_t == -1 || p->start_t == -1)
            continue;

        // Calcular turnaround: fim - chegada
        int turnaround = p->end_t - p->arrival_t;
        total_turnaround += turnaround;

        // Calcular tempo de espera: início - chegada (nunca negativo)
        int waiting = p->start_t - p->arrival_t;
        if (waiting < 0)
            waiting = 0;
        total_waiting += waiting;

        // Somar burst para calcular utilização da CPU
        total_burst += p->burst_t;

        // Verificar deadlines
        if (p->end_t > p->deadline)
            missed++;

        completed++;
    }

    // Calcular métricas finais se houver processos válidos
    if (completed > 0)
    {
        stats.avg_waiting_time = (double)total_waiting / completed;
        stats.avg_turnaround_time = (double)total_turnaround / completed;
        stats.cpu_utilization = ((double)total_burst / total_time) * 100.0;
        stats.throughput = (double)completed / total_time;
    }

    stats.missed_deadlines = missed;
    return stats;
}

void print_stats(const SchedulingStats *stats)
{
    printf("\nEstatísticas:\n");
    printf("Tempo médio de espera: %.2f\n", stats->avg_waiting_time);
    printf("Tempo médio de retorno: %.2f\n", stats->avg_turnaround_time);
    printf("Utilização da CPU: %.2f%%\n", stats->cpu_utilization);
    printf("Throughput: %.2f processos/unidade de tempo\n", stats->throughput);
    printf("Deadlines perdidas: %d\n", stats->missed_deadlines);
    printf("----------------------------------------\n");
}
