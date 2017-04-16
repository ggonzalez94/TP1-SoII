/** @file clienteHelpers.c
 *  @brief Codigo fuente con las funciones auxiliares del cliente
 *
 */

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
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
int read_all(int socket, void* buffer, int buffer_size)
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
	buffer = strtok(buffer,"%3");
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
