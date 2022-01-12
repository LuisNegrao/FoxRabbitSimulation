OBJS= main.o Entity.o Enviroment.o Engine.o
PROGRAM=./main
CFLAGS=-g

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	gcc -o $(PROGRAM) -fopenmp $(OBJS)

clean:
	rm -f $(PROGRAM) $(OBJS) $(INTERM)
