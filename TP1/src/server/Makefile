

CC=gcc
CFLAGS=-I.
DEPS = serverHelpers.h usuarios.h estructuraDeDatos.h
OBJ = server.o serverHelpers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o server
