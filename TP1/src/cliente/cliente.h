/** @file cliente.h
 *  @brief Header con las funciones auxiliares del cliente
 * 	@see clienteHelpers.c
 *
 */

#include <stdbool.h>
#define TAM 1000

bool startsWith(const char *pre, const char *str);
bool endsWith(const char *str, const char *suffix);
int read_all(int socket, char* buffer,int buffer_size);
int start_udp_socket(int socket);
void prompt_socket(int socket,struct sockaddr_in serv_addr);