#include <stdio.h>
#include "probsched.h"

SchedulingStats calculate_stats(Process **processes, int n, int total_time)
{
    SchedulingStats stats = {0};
    int total_waiting = 0;
    int total_turnaround = 0;
    int total_burst = 0;
    int missed = 0;

    for (int i = 0; i < n; i++)
    {
        // Calculate waiting time (end - arrival - burst)
        int waiting = processes[i]->end_t - processes[i]->arrival_t - processes[i]->burst_t;
        total_waiting += waiting;

        // Calculate turnaround time (end - arrival)
        int turnaround = processes[i]->end_t - processes[i]->arrival_t;
        total_turnaround += turnaround;

        // Sum burst times for CPU utilization
        total_burst += processes[i]->burst_t;

        // Check for missed deadlines
        if (processes[i]->end_t > processes[i]->deadline)
        {
            missed++;
        }
    }

    // Calculate averages and metrics
    stats.avg_waiting_time = (double)total_waiting / n;
    stats.avg_turnaround_time = (double)total_turnaround / n;
    stats.cpu_utilization = ((double)total_burst / total_time) * 100.0;
    stats.throughput = (double)n / total_time;
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