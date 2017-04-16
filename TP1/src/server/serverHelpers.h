/** @file serverHelpers.h
 *  @brief Header file para las funciones definidas en serverHelpers.c
 * 	@see serverHelpers.c
 *
 * Este es el codigo encargado de correr el servidor.
 * Autentica a los usuarios e intercambia informacion con el cliente.
 */

#include <stdbool.h>
#define BUFFER_SIZE 10000
#define TAM_FILE 18304

bool autenticacion(char *usuario,char *password,struct Usuario *usuarios,int array_length);
struct Datos *csv_parser();
void listar_estaciones( struct Datos * const datos, struct Promedio * promedio_variable,int size_datos,int cantidad_estaciones); //ptr constante
void mensual_precipitacion(int numero_estacion,float array_precipitacion[],struct Datos * const datos,int size_datos);
void diario_precipitacion(int numero_estacion,float array_precipitacion[],struct Datos * const datos,int size_datos);
void calcular_promedio(char *variable,struct Promedio * estaciones,struct Datos * const datos,int size_datos,int cantidad_estaciones);
bool endsWith(const char *str, const char *suffix);
int read_all(int socket, void* buffer,int buffer_size);
bool startsWith(const char *pre, const char *str);
