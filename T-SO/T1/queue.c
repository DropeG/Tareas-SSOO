#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

// Crear una cola con un quantum dado
Cola* create_queue(int quantum) {
    Cola* q = malloc(sizeof(Cola));
    if (!q) {
        perror("Error asignando memoria para la cola");
        exit(1);
    }
    q->capacity = 8;
    q->processes = malloc(q->capacity * sizeof(Process*));
    if (!q->processes) {
        perror("Error asignando memoria para los procesos de la cola");
        free(q);
        exit(1);
    }
    q->count = 0;
    q->quantum = quantum;
    return q;
}

// Agregar proceso al final de la cola
void enqueue(Cola* q, Process* p) {
    if (q->count >= q->capacity) {
        q->capacity *= 2;
        q->processes = realloc(q->processes, q->capacity * sizeof(Process*));
        if (!q->processes) {
            perror("Error reallocando memoria para los procesos de la cola");
            exit(1);
        }
    }
    q->processes[q->count++] = p;
}

Process* dequeue(Cola* q) {
    if (q->count == 0) return NULL;
    Process* p = q->processes[0];
    for (int i = 1;i< q->count; i++) {
        q->processes[i-1] = q->processes[i];
    }
    q->count--;
    return p;
}

void remove_process(Cola* q, Process* p) {
    for (int i = 0; i< q->count; i++) {
        if (q->processes[i] ==p) {
            //desplazar a la izquierda desde i+1
            for (int j = i+1;j<q->count;j++) {
                q->processes[j-1] = q->processes[j];
            }
            q->count--;
            break;
        }
    }
}

void sort_by_priority(Cola* q) {
    // Ordenar procesos por prioridad (bursts_done, deadline, start_time)
    for (int i = 0; i < q->count - 1; i++) {
        for (int j = i+1; j < q->count; j++) {
            Process* a = q->processes[i];
            Process* b = q->processes[j];
            // comparar prioridades
            if (a->priority > b->priority || (a->priority == b->priority && a->pid > b->pid)) {
                Process* tmp = q->processes[i];
                q->processes[i] = q-> processes[j];
                q->processes[j] = tmp;
            }
        }
    }
}