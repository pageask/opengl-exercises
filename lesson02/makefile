CC = gcc
LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

all : lesson02

lesson02 : lesson02.o
	$(CC) -o lesson02 lesson02.o $(LIBRARIES)

lesson02.o : lesson02.c
	$(CC) -c lesson02.c

clean :
	rm lesson02 lesson02.o
