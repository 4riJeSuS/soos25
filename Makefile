CC = cc
FLAGS = -c -Wall
LIBS = -lm
OBS = main.o scheduler.o process.o distributions.o stats.o

probsched: $(OBS)
    $(CC) $(OBS) -o probsched $(LIBS)

main.o: main.c
    $(CC) $(FLAGS) main.c

scheduler.o: scheduler.c scheduler.h
    $(CC) $(FLAGS) scheduler.c

process.o: process.c process.h
    $(CC) $(FLAGS) process.c

distributions.o: distributions.c distributions.h
    $(CC) $(FLAGS) distributions.c

stats.o: stats.c stats.h
    $(CC) $(FLAGS) stats.c

clean:
    rm -f *.o probsched