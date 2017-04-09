#include <stdbool.h>

#define BUFFER_SIZE 10000
#define TAM_FILE 8

bool autenticacion(char *usuario,char *password,struct Usuario *usuarios,int array_length);
struct Datos *csv_parser();
void listar_estaciones( struct Datos * const datos, char *estaciones[],int size_datos); //ptr constante
void mensual_precipitacion(int numero_estacion,float array_precipitacion[],struct Datos * const datos,int size_datos);
void diario_precipitacion(int numero_estacion,float array_precipitacion[],struct Datos * const datos,int size_datos);
