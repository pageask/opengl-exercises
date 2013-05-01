INCLUDE = -I/usr/include/
LIBDIR = -L/usr/X11R6/lib

COMPILERFLAGS = -Wall
CC = gcc
CFLAGS = $(COMPILERFLAGSE) $(INCLUDE)
LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

lesson% : lesson%.o
	$(CC) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBRARIES)