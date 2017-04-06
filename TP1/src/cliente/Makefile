

CC=gcc
CFLAGS=-I.
DEPS = cliente.h colores.h usuarios.h
OBJ = cliente.o clienteHelpers.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cliente: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o cliente
