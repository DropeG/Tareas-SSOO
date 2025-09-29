#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"

int hay_eventos_pendientes(Event* events, int num_events, int tick_actual) {
    for (int i = 0; i < num_events; i++) {
        if (events[i].t_event >= tick_actual) {
            return 1;
        }
    }
    return 0;
}

void run_scheduler(Process* processes, int num_proc, 
                   Event* events, int num_events, 
                   int q, const char* output_file) {
    // Crear colas High y Low
    Cola* high = create_queue(2 * q);
    Cola* low = create_queue(q);

    int tick = 0;
    int terminated_processes = 0;
    Process* running_process = NULL;

    printf("\n--- Comienza simulación ---\n");

    while (terminated_processes <num_proc  && tick < 10000) {
        printf("\nTick %d\n", tick);

        // ✅ 1. Actualizar los procesos que hayan terminado su tiempo de espera de I/O de WAITING a READY.
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

        // 2. Actualizar los procesos en las colas que hayan cumplido su deadline pero no completaron sus bursts a estado DEAD.
        for (int i = 0; i< num_proc; i++) {
            Process* p = &processes[i];
            if (p->state != FINISHED && p->state != DEAD && (tick >= (p->start_time+p->deadline))) {

                // Mini verificacion ¿El proceso terminaría su burst en este tick?
                if (p->state ==RUNNING && p->burst_remaining == 1 && p->bursts_done + 1 == p->num_bursts) {
                    // El proceso terminaría completamente en este tick
                    printf("  %s termina antes de deadline - no se marca DEAD\n", p->name);
                    continue; // No marcar como DEAD
                }
                p->state = DEAD;
                p->finish_time = tick;
                printf("  %s perdió su deadline en tick %d (marcado DEAD)\n",
                p->name, tick);
                //Sacarlo de las colas
                remove_process(high,p);
                remove_process(low,p);
                terminated_processes++;
            }
        }
        // 3. Actualizar proceso RUNNING si existe
        if (running_process) {
            // 3.1) Verificar deadline
            if (running_process->state == DEAD) {
                running_process = NULL;
            } 

            // 3.2) Verificar si termino CPU burst
            else if (running_process->burst_remaining == 0) {
                running_process->bursts_done++;
                running_process->quantum_used = 0;

                if (running_process->bursts_done == running_process->num_bursts) {
                    running_process->state = FINISHED;
                    running_process->finish_time = tick + 1;
                    printf("  %s terminó en tick %d\n", running_process->name, tick + 1);

                    remove_process(high, running_process);
                    remove_process(low, running_process);
                    terminated_processes++;
                } else {
                    running_process->state = WAITING;
                    running_process->io_remaining = running_process->io_wait;
                    running_process->last_cpu_exit = tick + 1;

                    remove_process(high, running_process);
                    remove_process(low, running_process);
                    printf("  %s va a WAITING por %d ticks\n", running_process->name, running_process->io_wait);
                }
                running_process = NULL;
            }

            // 3.3) Verificar si se acabo quantum
            else{
                int current_queue_quantum = 0;
                if(is_in_queue(high,  running_process)) {
                    current_queue_quantum = high->quantum;
                } else if (is_in_queue(low, running_process)) {
                    current_queue_quantum = low->quantum;
                }

                if (running_process->quantum_used >= current_queue_quantum) {
                    running_process->state = READY;
                    running_process->quantum_used = 0;
                    running_process->interruptions++;

                    //Si estaba en High, baja a Low
                    if (is_in_queue(high, running_process)) {
                        remove_process(high, running_process);
                        enqueue(low, running_process);
                        printf("  %s bajó a Low\n", running_process->name);
                }
                    running_process = NULL;
                }
            }
        }

        printf("Cola High:");
        print_queue_names(high);

        // 4. Ingresar los procesos a las colas segun corresponda

        // 4.1) Proceos que salen de cpu (ya manejado arriba con running_process)

        // 4.2) Procesos nuevos por tiempo de inicio
        for (int i = 0; i<num_proc; i++) {
            Process* p = &processes[i];
            if (p->state == NEW && p->start_time == tick) {
                p->state = READY;
                enqueue(high, p);
                printf("  %s llegó en tick %d\n", p->name, tick);
            }
        }

        // 4.3) Procesos que vuelven de WAITING (ya manejado arriba)
        for (int i = 0; i < low->count; i++) {
            Process* p = low->processes[i];
    
            // Condición: 2 * deadline < tick - last_cpu_exit
            if (2 * p->deadline < tick - p->last_cpu_exit) {
                remove_process(low, p);
                enqueue(high, p);
                printf("  %s subió de Low a High (2*%d < %d-%d)\n", 
                       p->name, p->deadline, tick, p->last_cpu_exit);
                i--; // Ajustar índice porque removimos un elemento
            }
        }

        // 5. Actualizar las prioridades de todos los procesos segun la fórmula dada. 
        for (int i = 0; i< num_proc; i++ ){
            Process*p = &processes[i];
            if(p->state == READY){
                p->priority = 1.0 / (double)(p->deadline - tick) + (p->num_bursts - p->bursts_done);
            }
        }

        printf("  Prioridades: ");
        for (int i = 0; i < num_proc; i++) {
            Process* p = &processes[i];
            if (p->state == READY) {
                printf("%s(%.6f) ", p->name, p->priority);
            }
        }
        printf("\n");

        sort_by_priority(high);
        sort_by_priority(low);

        // Verificar eventos ANTES de procesar nuevos procesos
        Process* event_process = NULL;
        for (int e = 0; e < num_events; e++) {
            if (events[e].t_event == tick) {
                int pid_evento = events[e].pid;
                printf("  Evento: PID=%d debe entrar forzosamente\n", pid_evento);
                
                for (int i = 0; i < num_proc; i++) {
                    if (processes[i].pid == pid_evento) {
                        event_process = &processes[i];
                        break;
                    }
                }
                
                // En la sección de eventos, verificar si el proceso puede reactivarse
                if (event_process) {
                    // Solo procesar evento si el proceso NO está terminado
                    if (event_process->state != FINISHED && event_process->state != DEAD) {
                        // Si hay proceso corriendo, interrumpirlo
                        if (running_process && running_process != event_process) {
                            running_process->state = READY;
                            running_process->interruptions++;
                            running_process = NULL;
                        }

                        remove_process(low, event_process);
                        remove_process(high, event_process);
                        enqueue(high, event_process);
                        event_process->state = READY;
                    } else {
                        printf("  Evento ignorado: %s ya terminó\n", event_process->name);
                    }
                }
                break; // Solo un evento por tick
            }
        }

        //6. Seleccionar proceso a CPU
        if (!running_process) {
            // 6.1) Si se cumplio el tiempo de un evento
            if(event_process && event_process->state == READY) {
                running_process = event_process;
            }

            // 6.2 Primer procesos de High
            else if (high->count > 0) {
                printf("  Seleccionando de High %s\n", high->processes[0]->name);

                running_process = high->processes[0];
            }
            // 6.3 Primer proceso de Low
            else if (low->count > 0) {
                running_process = low->processes[0];
            }
        }

        //Ejecutar tick en proceso seleccionado
        if (running_process) {
            running_process->state = RUNNING;

            if (running_process->first_response == -1) {
                running_process->first_response = tick +1;
            }

            running_process->burst_remaining--;
            printf("Burst remaining de %s: %d\n", running_process->name, running_process->burst_remaining);
            running_process->quantum_used++;

            printf("  Ejecutando %s (pid=%d) burst_remaining=%d quantum_used=%d\n",
                   running_process->name, running_process->pid,
                   running_process->burst_remaining, running_process->quantum_used);
        } else {
            printf("  CPU idle\n");
        }

        for (int i = 0; i<num_proc; i++) {
            if (processes[i].state == READY) {
                processes[i].waiting_time++;
            }
        }

        tick++;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Error abriendo output");
        exit(1);
    }

    fprintf(out, "nombre,pid,estado,interrupciones,turnaround,response,waiting\n");

    for (int i = 0; i<num_proc; i++) {
        Process* p = &processes[i];
        const char* state_str = (p->state == FINISHED) ? "FINISHED" :
                                (p->state == DEAD) ? "DEAD" : "FINISHED";
        int turnaround = p->finish_time - p->start_time;
        int response = p->first_response - p->start_time;
        fprintf(out, "%s,%d,%s,%d,%d,%d,%d\n",
                p->name,
                p->pid,
                state_str,
                p->interruptions,
                turnaround,
                response,
                p->waiting_time);
    }

    fclose(out);
    printf("\n--- Simulación terminada ---\n");
}