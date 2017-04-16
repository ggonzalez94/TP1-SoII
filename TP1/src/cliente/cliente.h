/** @file cliente.h
 *  @brief Header con las funciones auxiliares del cliente
 * 	@see clienteHelpers.c
 *
 */

#include <stdbool.h>
bool startsWith(const char *pre, const char *str);
bool endsWith(const char *str, const char *suffix);
int read_all(int socket, void* buffer,int buffer_size);
