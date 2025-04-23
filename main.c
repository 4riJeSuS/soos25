#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "probsched.h"

#define MAX_PROCESSES 5

int main()
{
    seed_rng(time(NULL));

    Process *processes[MAX_PROCESSES];

    // Create processes
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        int arrival = poisson_random(1.0);
        int burst = exponential_random(0.5) + 1;
        int priority = uniform_random(1, 5);
        processes[i] = create_process(i, arrival + i, burst, priority);
        processes[i]->deadline = arrival + burst + uniform_random(5, 15);
        processes[i]->period = uniform_random(10, 20);
        print_process(processes[i]);
    }

    printf("\n--- FCFS Scheduling ---\n");
    fcfs(processes, MAX_PROCESSES);

    printf("\n--- SJF Scheduling ---\n");
    sjf(processes, MAX_PROCESSES);

    printf("\n--- Priority (Non-preemptive) Scheduling ---\n");
    priority_np(processes, MAX_PROCESSES);

    printf("\n--- Round Robin Scheduling (Quantum = 2) ---\n");
    round_robin(processes, MAX_PROCESSES, 2);

    printf("\n--- Rate Monotonic Scheduling ---\n");
    rate_monotonic(processes, MAX_PROCESSES);

    printf("\n--- EDF Scheduling ---\n");
    edf(processes, MAX_PROCESSES);

    // Free memory
    for (int i = 0; i < MAX_PROCESSES; i++) {
        free(processes[i]);
    }

    return 0;
}