#include <string.h>

#include "cliente.h"

//Funcion para determinar si una cadena(pre) comienza con otra(str)
bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
