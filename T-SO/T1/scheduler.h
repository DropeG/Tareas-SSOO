#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "event.h"
#include "queue.h"

void run_scheduler(Process* processes, int num_proc, 
                   Event* events, int num_events, 
                   int q, const char* output_file);

#endif 