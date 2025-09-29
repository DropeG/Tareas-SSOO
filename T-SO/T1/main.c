#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "process.h"
#include "event.h"
#include "queue.h"
#include "scheduler.h"

int main(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    int q, K, N;
    Process* processes;
    Event* events;

    parse_input(argv[1], &q, &K, &N, &processes, &events);


    // Aquí después llamaremos a run_scheduler(processes, K, events, N, q, argv[2]);
    printf("q=%d, K=%d, N=%d\n", q, K, N);

    for (int i = 0; i < K; i++) {
        Process *p = &processes[i];
        printf("Proceso %d: %s pid=%d start=%d burst=%d nbursts=%d io=%d deadline=%d\n",
               i, p->name, p->pid, p->start_time, p->burst_time,
               p->num_bursts, p->io_wait, p->deadline);
    }

    for (int j = 0; j < N; j++) {
        printf("Evento %d: pid=%d t_event=%d\n", j, events[j].pid, events[j].t_event);
    }

    run_scheduler(processes, K, events, N, q, "output.csv");

    // Liberar memoria al final:
    free(processes);
    free(events);
    return 0;
}