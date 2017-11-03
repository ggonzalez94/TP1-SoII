/** @file clienteHelpers.c
 *  @brief Codigo fuente con las funciones auxiliares del cliente
 *
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include "cliente.h"

/**
* @brief Devuelve true si la cadena str comienza con la cadena pre. False caso contrario
*
* @param pre Prefijo
* @param str Cadena dentro de la cual buscar
*/
bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

/**
* @brief Envoltorio para read. Funcion segura para asegurarse de leer todos los datos
*
* @param socket Socket descriptor
* @param buffer Buffer en cual almacenar la cadena leida
* @param buffer_size Tamano del buffer
*/
int read_all(int socket, char* buffer, int buffer_size)
{
    int bytesRead = 0;
    int result;
    while (!endsWith(buffer,"%3"))
    {
        result = read(socket, buffer + bytesRead, buffer_size);
        if (result < 1 )
        {
            perror("Lectura: ");
        }

        bytesRead += result;
    }
    buffer[strlen(buffer)-1] = 0;
    buffer[strlen(buffer)-1] = 0;
    return bytesRead;
}


/**
* @brief Comprueba si una cadena (str) termina con otra (suffix)
*
* @param str Cadena dentro de la cual buscar
* @param suffix Sufijo
*/
bool endsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}



int start_udp_socket(int sock){
    struct sockaddr_in serv_addr;
    int puerto;

    sock = socket( AF_INET, SOCK_DGRAM, 0 );
    memset( &serv_addr, 0, sizeof(serv_addr) );
    puerto = 6020;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons( puerto );
    memset( &(serv_addr.sin_zero), '\0', 8 );
    if( bind( sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr) ) < 0 ) {
        perror( "ERROR en binding" );
        exit( 1 );
    }

    printf( "Socket udp disponible: %d\n", ntohs(serv_addr.sin_port) );
    
    return sock;
}

void prompt_socket(int socket_udp,struct sockaddr_in serv_addr){
    FILE *fp;
    char buffer[TAM];
    int n;
    fp = fopen("../../datos_estacion.txt","w");
    int tamano_direccion = sizeof( struct sockaddr );
    bool recibir = true;
    
    while (recibir)
    {
        memset( buffer, 0, TAM );
        n = read_all_udp(socket_udp,buffer,TAM,serv_addr,tamano_direccion);
        //n = recvfrom( socket_udp, buffer, TAM-1, 0, (struct sockaddr *)&serv_addr, &tamano_direccion );
        if ( n < 0 ) {
            perror( "socket read" );
            exit( 1 );
        }
        //printf("Recibí: %s\n",buffer );
        n = fwrite(buffer,sizeof(buffer[0]),strlen(buffer),fp);
        if (n<0){
            perror("File write");
        }
        if (startsWith("exit",buffer)){ 
            recibir = false;
        }
    }

    if (startsWith("fin",buffer)){
        return;
    }
    fclose(fp);
    return;
}

int read_all_udp(int socket_udp, char* buffer, int buffer_size,struct sockaddr_in serv_addr,int tamano_direccion)
{
    int bytesRead = 0;
    int result;
    //printf("%s\n","Reading UDP" );
    fflush(stdout);
    while (!endsWith(buffer,"%3"))
    {
        result = recvfrom( socket_udp, buffer+bytesRead, buffer_size, MSG_WAITALL, (struct sockaddr *)&serv_addr, &tamano_direccion );
        if (result < 1 )
        {
            perror("Lectura: ");
        }

        bytesRead += result;
    }
    buffer[strlen(buffer)-1] = 0;
    buffer[strlen(buffer)-1] = 0;
    return bytesRead;
}
