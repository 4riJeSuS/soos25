CC = cc
FLAGS = -c -Wall
LIBS = -lm
OBS = main.o scheduler.o process.o distributions.o stats.o


all: probsched

main.o: probsched.h main.c
	$(CC) $(FLAGS) main.c

scheduler.o: probsched.h scheduler.c 
	$(CC) $(FLAGS) scheduler.c

process.o: probsched.h process.c 
	$(CC) $(FLAGS) process.c

distributions.o: probsched.h distributions.c 
	$(CC) $(FLAGS) distributions.c

stats.o: probsched.h stats.c 
	$(CC) $(FLAGS) stats.c

probsched: $(OBS)
	$(CC) -o probsched $(OBS) $(LIBS)

clean:
	rm -f probsched *.o 
	rm -f *~
	@echo "Remover: Files executáveis."
