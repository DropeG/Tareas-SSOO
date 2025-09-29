#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "event.h"

void parse_input(const char* filename, int* q, int * K, int* N, 
                 Process** processes_out, Event** events_out ){
    FILE* f = fopen(filename, "r");
    if (!f) {
        perror("Error al abrir input");
        exit(1);
    }

    // leer q, K, N
    if (fscanf(f, "%d", q) != 1) { fprintf(stderr, "Error leyendo q\n"); exit(1); }
    if (fscanf(f, "%d", K) != 1) { fprintf(stderr, "Error leyendo K\n"); exit(1); }
    if (fscanf(f, "%d", N) != 1) { fprintf(stderr, "Error leyendo N\n"); exit(1); }

    // reservar memoria
    Process* processes = calloc(*K, sizeof(Process));
    Event* events = calloc(*N, sizeof(Event));

    // leer K procesos
    for (int i = 0; i < *K; i++){
        Process p;
        if (fscanf(f, "%s %d %d %d %d %d %d",
                   p.name, &p.pid, &p.start_time, &p.burst_time,
                   &p.num_bursts, &p.io_wait, &p.deadline) != 7) {
            fprintf(stderr, "Error leyendo proceso %d\n", i);
            exit(1);
        }
        // inicializar otros campos
        p.state = READY;
        p.bursts_done = 0;
        p.last_cpu_exit = -1;
        p.priority = 0.0;
        p.interruptions = 0;
        p.first_response = -1;
        p.finish_time = -1;
        p.waiting_time = 0;
        p.quantum_used = 0;

        p.burst_remaining = p.burst_time;
        p.io_remaining = 0;

        processes[i] = p;
    }

    //leer N eventos
    for (int j = 0; j< *N; j++){
        int pid, t_event;
        if (fscanf(f, "%d %d", &pid, &t_event) != 2) {
            fprintf(stderr, "Error leyendo evento %d\n", j);
            exit(1);
        }
        events[j].pid = pid;
        events[j].t_event = t_event;
    }

    fclose(f);

    *processes_out = processes;
    *events_out = events;
}