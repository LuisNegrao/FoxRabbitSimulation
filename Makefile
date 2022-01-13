OBJS= main.o Entity.o Enviroment.o Engine.o
PROGRAM=./main
CFLAGS=-g
OPENMP = -fopenmp

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	gcc main.c Enviroment.c Engine.c Entity.c -o main -fopenmp -lm

clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM)
