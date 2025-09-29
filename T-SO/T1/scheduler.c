#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

void run_scheduler(Process* processes, int num_proc, 
                   Event* events, int num_events, 
                   int q, const char* output_file) {
    // Crear colas High y Low

    Cola* high = create_queue(2 * q);
    Cola* low = create_queue(q);

    // Ingresas procesos iniciales (por T_INICIO=0)
    for (int i =0; i < num_proc; i++) {
        if(processes[i].start_time == 0) {
            enqueue(high, &processes[i]);
            processes[i].state = READY;
        }
    }

    int tick = 0;
    int terminated_processes = 0;

    printf("\n--- Comienza simulación ---\n");

    while (terminated_processes <num_proc && tick < 50) {
        printf("\nTick %d\n", tick);

        //Actualizar procesos en WAITING
        for (int i =0; i<num_proc; i++) {
            Process* p = &processes[i];
            if (p->state == WAITING) {
                p->io_remaining--;
                if(p->io_remaining <= 0) {
                    p->state = READY;
                    p->burst_remaining = p->burst_time;
                    enqueue(high, p); //vuelve a cola alta
                    printf("  %s volvió de I/O a High\n", p->name);
                }
            }
        }

        //Recalcular prioridades de procesos en READY

        for (int i = 0; i< num_proc; i++ ){
            Process*p = &processes[i];
            if(p->state == READY){
                p->priority = 1.0 / (p->deadline - tick) +
                              (p->num_bursts - p->bursts_done);
            }
        }

        sort_by_priority(high);
        sort_by_priority(low);

        //Elegir proceso a CPU
        Process* current = NULL;
        int actualQueue = 0; // 1 = high, 2 =low
        if (high->count >0) {
            current = high->processes[0];
            actualQueue = 1;
        } else if (low->count >0) {
            current = low->processes[0];
            actualQueue = 2;
        }

        if (current) {
            current->state = RUNNING;

            if (current->first_response == -1) {
                current->first_response = tick;
            }

            //Consumir tick
            current->burst_remaining--;
            current->quantum_used++;

            printf("  Ejecutando %s (pid=%d) burst_remaining=%d quantum_used=%d\n",
                   current->name, current->pid,
                   current->burst_remaining, current->quantum_used);

            // ¿Terminó la ráfaga?
            if (current->burst_remaining == 0) {
                current->bursts_done++; 
                current->quantum_used = 0;

                if (current->bursts_done == current->num_bursts) {
                    current->state = FINISHED;
                    current->finish_time = tick+1;
                    printf("  %s terminó en tick %d\n",
                           current->name, current->finish_time);
                    remove_process(high, current);
                    remove_process(low, current);
                    terminated_processes++;
                } else {
                    current->state = WAITING;
                    current->io_remaining = current->io_wait;
                    current->last_cpu_exit = tick+1;
                    printf("  %s va a WAITING por %d ticks de I/O\n",
                           current->name, current->io_wait);
                    remove_process(high, current);
                    remove_process(low, current);
                }
            }
            // Se acabo el quantum sin terminar rafaga?
            else if (actualQueue == 1 && current->quantum_used == high->quantum) {
                current->state = READY;
                current->quantum_used = 0;
                remove_process(high, current);
                enqueue(low, current);
                printf("  %s bajó a Low\n", current->name);
            } else if (actualQueue == 2 && current->quantum_used == low->quantum) {
                current->state = READY;
                current->quantum_used = 0;
                // En Low se queda, solo resetear quantum
            }
        } else {
            printf("  CPU idle\n");
        }

        tick++;
    }

    printf("\n--- Fin simulación (esqueleto) ---\n");
}