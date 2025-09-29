#ifndef EVENT_H
#define EVENT_H

typedef struct {
    int pid; //proceso afectado
    int t_event; //tick en que ocurre el evento
} Event;

#endif