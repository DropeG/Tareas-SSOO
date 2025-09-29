CC = gcc
CCFLAGS = -Wall -Wextra -g

OBJS = main.o parser.o queue.o scheduler.o

all: DCCambios

DCCambios: $(OBJS)
	$(CC) $(CCFLAGS) -o DCCambios $(OBJS)

main.o: main.c parser.H process.h event.h
parser.o: parser.c parser.h process.h event.h
queue.o: queue.c queue.h process.h
scheduler.o: scheduler.c scheduler.h process.h event.h queue.h

clean:
	rm -f *.o DCCambios