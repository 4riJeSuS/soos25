#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "probsched.h"

#define MAX_PROCESSES 5

int main()
{
    seed_rng(time(NULL));

    Process *processes[MAX_PROCESSES];

    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        int arrival = poisson_random(1.0);
        int burst = exponential_random(0.5) + 1;
        int priority = uniform_random(1, 5);
        processes[i] = create_process(i, arrival + i, burst, priority);
        print_process(processes[i]);
    }

    printf("\n--- FCFS Scheduling ---\n");
    fcfs(processes, MAX_PROCESSES);

    return 0;
}
