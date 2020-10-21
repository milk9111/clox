CC=gcc
CFLAGS=-I.
DEPS = common.h chunk.h memory.h debug.h value.h
OBJ =  main.o chunk.o memory.o debug.o value.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clox: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)