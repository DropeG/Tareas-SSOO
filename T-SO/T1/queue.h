#ifndef QUEUE_H
#define QUEUE_H

#include "process.h"

typedef struct {
    Process **processes; // arreglo dinamico de punteros a procesos
    int count;          //cuantos procesos hay
    int capacity;      //capacidad del arreglo
    int quantum;       //quantum de estsa cola
} Cola;

Cola* create_queue(int quantum);
void enqueue(Cola* q, Process* p);
Process* dequeue(Cola* q);
void remove_process(Cola* q, Process* p);
void sort_by_priority(Cola* q);
int is_in_queue(Cola* q, Process* p);
void print_queue_names(Cola* q);

#endif