

CC=gcc
CFLAGS=-I. -std=gnu99
DEPS = serverHelpers.h usuarios.h estructuraDeDatos.h estructuraPromedios.h
OBJ = server.o serverHelpers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

server: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o server
