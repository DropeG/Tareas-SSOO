#ifndef PARSER_H
#define PARSER_H

#include "process.h"
#include "event.h"

void parse_input(const char *filename, int *q, int *K, int *N, Process** processes, Event** events);

#endif