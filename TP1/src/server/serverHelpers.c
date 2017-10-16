/** @file serverHelpers.c
 *  @brief Funciones auxiliares necesarias para el funcionamiento del server
 *
 * Incluye funciones para la autenticacion,parseo del archivo csv y funciones internas
 * del server para ejecutar los comandos(listar,promedio,etc)
 */

#define _GNU_SOURCE

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include "usuarios.h"
#include "estructuraDeDatos.h"
#include "estructuraPromedios.h"
#include "serverHelpers.h"


/**
* @brief Funcion para autenticar un usuario dentro de la base de Datos.
*
* @param usuario Nombre del usuario
* @param password Contrasena del usuario
* @param usuarios Puntero al array de estructuras de usuarios validos(base de datos)
* @param array_length Cantidad de usuarios
*/
bool autenticacion(char *usuario, char *password, struct Usuario *usuarios,int array_length){
	bool usuario_valido = false;
	for(int i=0;i<array_length;i++){
		if (!strcmp(usuarios->nombre_usuario,usuario) && !strcmp(usuarios->password,password)){
			usuario_valido = true;
		}
		usuarios++;
	}
	return usuario_valido;
}

/**
* @brief Devuelve un puntero a un array de estructuras con los datos del archivo CSV
*
*/
struct Datos *csv_parser(){
	char buffer[BUFFER_SIZE];
	FILE *fp;
	static struct Datos datos[TAM_FILE];
	int indice = 0; //Indice para ir llenando los datos
	fp = fopen("../../datos_meteorologicos_linux.csv","r");
	if (fp == NULL){
		perror("Lectura de archivo:");
		exit(1);
	}
	//Ignoro la primera linea que tiene solo los nombres de los campos
	fgets(buffer,BUFFER_SIZE,fp);
	
	char numero[20];
	char nombre_estacion[50];
	char id_localidad[20];
	char fecha[50];
	char temperatura[20];
	char humedad[20];
	char punto_rocio[20];
	char precipitacion[20];
	char velocidad_viento[20];
	char direccion_viento[50];
	char rafaga_maxima[20];
	char presion[20];
	char radiacion_solar[20];
	char temperatura_suelo_1[20];
	char temperatura_suelo_2[20];
	char temperatura_suelo_3[20];
	char humedad_suelo_1[20];
	char humedad_suelo_2[20];
	char humedad_suelo_3[20];
	char humedad_hoja[20];
	//Leo el archivo linea por linea
	while(fgets(buffer,BUFFER_SIZE,fp) != NULL){
		//Almaceno en strings intermedias los valores correspondientes a cada campo
		int leidos = sscanf(buffer,"%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%[^','],%s",numero,nombre_estacion,id_localidad,fecha,temperatura,humedad,punto_rocio,precipitacion,velocidad_viento,direccion_viento,rafaga_maxima,presion,radiacion_solar,temperatura_suelo_1,temperatura_suelo_2,temperatura_suelo_3,humedad_suelo_1,humedad_suelo_2,humedad_suelo_3,humedad_hoja);
		//Convierto al tipo adecuado y guardo en el array de Datos
		datos[indice].numero = atoi(numero);
		strcpy(datos[indice].nombre_estacion,nombre_estacion);
		datos[indice].ID_localidad = atoi(id_localidad);
		strptime(fecha, "%d/%m/%Y %H:%M",&(datos[indice].fecha));
		datos[indice].temperatura = atof(temperatura);
		datos[indice].humedad = atof(humedad);
		datos[indice].punto_rocio = atof(punto_rocio);
		datos[indice].precipitacion = atof(precipitacion);
		datos[indice].velocidad_viento = atof(velocidad_viento);
		strcpy(datos[indice].direccion_viento,direccion_viento);
		datos[indice].rafaga_maxima = atof(rafaga_maxima);
		datos[indice].presion = atof(presion);
		datos[indice].radiacion_solar  =atof(radiacion_solar);
		if (0 == strcmp(radiacion_solar,"--")){
			datos[indice].radiacion_solar = -9999;
		}
		datos[indice].temperatura_suelo_1 = atof(temperatura_suelo_1);
		if (0 == strcmp(temperatura_suelo_1,"--")){
			datos[indice].temperatura_suelo_1 = -9999;
		}
		datos[indice].temperatura_suelo_2 = atof(temperatura_suelo_2);
		if (0 == strcmp(temperatura_suelo_2,"--")){
			datos[indice].temperatura_suelo_2 = -9999;
		}
		datos[indice].temperatura_suelo_3 = atof(temperatura_suelo_3);
		if (0 == strcmp(temperatura_suelo_3,"--")){
		datos[indice].temperatura_suelo_3 = -9999;
		}
		datos[indice].humedad_suelo_1 = atof(humedad_suelo_1);
		if (0 == strcmp(humedad_suelo_1,"--")){
			datos[indice].humedad_suelo_1 = -9999;
		}
		datos[indice].humedad_suelo_2 = atof(humedad_suelo_2);
		if (0 == strcmp(humedad_suelo_2,"--")){
			datos[indice].humedad_suelo_2 = -9999;
		}
		datos[indice].humedad_suelo_3 = atof(humedad_suelo_3);
		if (0 == strcmp(humedad_suelo_3,"--")){
			datos[indice].humedad_suelo_3 = -9999;
		}
		datos[indice].humedad_hoja = atof(humedad_hoja);
		if (0 == strcmp(humedad_hoja,"--")){
			datos[indice].humedad_hoja = -9999;
		}
		//printf("Numero: %i Nombre: %s ID: %i\n",datos[indice].numero,datos[indice].nombre_estacion,datos[indice].ID_localidad);
		//printf("Fecha: %i \n",datos[indice].fecha.tm_min);
		indice++;

	}
	fclose(fp);
	return datos;
}


/**
* @brief Lista las estaciones sin repetir. Falta que liste los sensores
*
* @param datos Arreglo de datos de las estaciones
* @param estaciones Arreglo de estaciones para llenar sin repetir
* @param size_datos Cantidad de datos en el archivo CSV
* @param cantidad_estaciones Cantidad de estaciones
*/
void listar_estaciones( struct Datos * const datos, struct Promedio * estaciones,int size_datos,int cantidad_estaciones){
	bool llenar; //Bool para ver si debo llenar o no
	int indice = 0; //Indice para ver en que parte de estaciones llenar
	//Uso un ptr auxiliar para no modificar el puntero datos
	struct Datos *datos_auxiliar = datos;
	//Pongo el numero -1 en todas las estaciones para despues comparar
	for (int i=0;i<cantidad_estaciones;i++){
		estaciones[i].numero = -1;
	}
	
	//Recorro linea a linea
	for(int i=0;i<size_datos;i++){
		llenar = true; //Comienzo con la premisa de llenar
		for (int j=0;j<cantidad_estaciones;j++){
			if (datos_auxiliar->numero == estaciones[j].numero){ //Si ya existe la estacion en la lista coloco llenar en false
				llenar = false;
			}
		}
		//Lleno con el numero de estacion y el nombre
		if (llenar){ //Si no existe
			estaciones[indice].numero = datos_auxiliar->numero;
			strcpy(estaciones[indice].nombre_estacion,datos_auxiliar->nombre_estacion);
			indice++;
		}
		//Si ya llene la cantidad de estaciones salgo de la funcion
		if (indice >= cantidad_estaciones){
			return;
		}
		datos_auxiliar++;
	}
	return;
}
/**
* @brief LLena en el arreglo array_precipitacion el acumulado mensual de precipitaciones para la estacion solicitada
*
* @param numero_estacion La estacion a buscar
* @param array_precipitacion Arreglo de 12 elementos a llenar(1 por mes)
* @param datos Arreglo de datos de las estaciones
* @param size_datos Cantidad de datos en el archivo CSV
*/
void mensual_precipitacion(int numero_estacion,float array_precipitacion[],struct Datos * const datos,int size_datos){
	struct Datos *datos_auxiliar = datos;
	//Me aseguro que esten en 0 los datos cuando comienzo la funcion
	for(int i=0;i<12;i++){
		array_precipitacion[i] = 0;
	}
	for (int i=0;i<size_datos;i++){
		//printf("Precipitacion actual: %f \n",datos_auxiliar->precipitacion);
		//Si el numero de estacion coincide con el que deseo
		if (numero_estacion == datos_auxiliar->numero){
			//Aumento la cuenta de precipitacion en el mes correspondiente(lo tengo en las estructuras de datos)
			array_precipitacion[datos_auxiliar->fecha.tm_mon]= array_precipitacion[datos_auxiliar->fecha.tm_mon] + datos_auxiliar->precipitacion;
		}
		datos_auxiliar++;
	}
	return;
}
/**
* @brief LLena en el arreglo array_precipitacion el acumulado diario de precipitaciones para la estacion solicitada
*
* @param numero_estacion La estacion a buscar
* @param array_precipitacion Arreglo de 365 elementos a llenar(1 por dia)
* @param datos Arreglo de datos de las estaciones
* @param size_datos Cantidad de datos en el archivo CSV
*/
void diario_precipitacion(int numero_estacion,float array_precipitacion[],struct Datos * const datos,int size_datos){
	struct Datos *datos_auxiliar = datos;
	//Me aseguro que esten en 0 los datos cuando comienzo la funcion
	for(int i=0;i<365;i++){
		array_precipitacion[i] = 0;
	}
	for (int i=0;i<size_datos;i++){
		//printf("Precipitacion actual: %f \n",datos_auxiliar->precipitacion);
		//Si el numero de estacion coincide con el que deseo
		if (numero_estacion == datos_auxiliar->numero){
			//Aumento la cuenta de precipitacion en el dia correspondiente(lo tengo en las estructuras de datos)
			array_precipitacion[datos_auxiliar->fecha.tm_yday]=array_precipitacion[datos_auxiliar->fecha.tm_yday] + datos_auxiliar->precipitacion;
		}
		datos_auxiliar++;
	}
	return;
}

/**
* @brief Calcula el promedio para la variable solicitada de todas las estaciones. No funciona con todas las variables
* 
* Por un tema de tiempo y no repetir if(no encontre otra forma) solo trabaja con algunas variables hardcodeadas
* 
* @param variable Variable de interes
* @param promedio_variable Arreglo con 1 elemento por estacio
* @param datos Arreglo de datos de las estaciones
* @param size_datos Cantidad de datos en el archivo CSV
* @param cantidad_estaciones Cantidad de estaciones
*/
void calcular_promedio(char *variable,struct Promedio * promedio_variable,struct Datos * const datos,int size_datos,int cantidad_estaciones){

	//Pongo los valores de promedio en 0 y reinicio la cantidad de lecturas
	for (int i=0;i<cantidad_estaciones;i++){
		promedio_variable[i].variable = 0;
		promedio_variable[i].cantidad_lecturas = 0;
	}
	//Recorro todo el archivo(las estructuras)
	for (int i=0;i<size_datos;i++){
		if (0 == strcmp(variable,"temperatura")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].temperatura != -9999){
					promedio_variable[j].variable = promedio_variable[j].variable + datos[i].temperatura;
					promedio_variable[j].cantidad_lecturas =promedio_variable[j].cantidad_lecturas + 1;
				}
			}
		}
		
		else if (0 == strcmp(variable,"humedad")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].humedad != -9999){
					promedio_variable[j].variable = promedio_variable[j].variable + datos[i].humedad;
					promedio_variable[j].cantidad_lecturas ++;
				}
			}
		}
		
		else if (0 == strcmp(variable,"punto_rocio")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].punto_rocio != -9999){
					promedio_variable[j].variable = promedio_variable[j].variable + datos[i].punto_rocio;
					promedio_variable[j].cantidad_lecturas ++;
				}
			}
		}
		
		else if (0 == strcmp(variable,"precipitacion")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].precipitacion != -9999){
					promedio_variable[j].variable += datos[i].precipitacion;
					promedio_variable[j].cantidad_lecturas += 1;
				}
			}
		}
		
		else if (0 == strcmp(variable,"velocidad_viento")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].velocidad_viento != -9999){
					promedio_variable[j].variable += datos[i].velocidad_viento;
					promedio_variable[j].cantidad_lecturas += 1;
				}
			}
		}
		
		else if (0 == strcmp(variable,"rafaga_maxima")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].rafaga_maxima != -9999){
					promedio_variable[j].variable += datos[i].rafaga_maxima;
					promedio_variable[j].cantidad_lecturas += 1;
				}
			}
		}
		else if (0 == strcmp(variable,"presion")){
			for (int j=0;j<cantidad_estaciones;j++){
				//Valido el numero de estacion y que existan datos del sensor
				if (promedio_variable[j].numero == datos[i].numero && datos[i].presion != -9999){
					promedio_variable[j].variable += datos[i].presion;
					promedio_variable[j].cantidad_lecturas += 1;
				}
			}
		}
		
		//Si no reconozco el comando lleno con -9999(No Data)
		else{
			for (int i=0;i<cantidad_estaciones;i++){
				promedio_variable[i].variable = -9999;
			}		
		}
	}
	
	//Teniendo el acumulado calculo el promedio
	for (int i=0;i<cantidad_estaciones;i++){
		promedio_variable[i].variable = promedio_variable[i].variable/promedio_variable[i].cantidad_lecturas;
	}
}


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
            exit(1);
        }

        bytesRead += result;
    }
    buffer[strlen(buffer)-1] = 0;
    buffer[strlen(buffer)-1] = 0;
    return bytesRead;
}

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

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

int start_udp_socket(int sock,struct sockaddr_in *dest_addr){
	struct hostent *server;
	char buffer[1000];

	server = gethostbyname("localhost");
	if ( server == NULL ) {
		fprintf( stderr, "ERROR, no existe el host\n");
		exit(0);
	}

	int puerto = 6020;
	sock = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sock < 0) {
		perror( "apertura de socket" );
		exit( 1 );
	}

	dest_addr->sin_family = AF_INET;
	dest_addr->sin_port = htons(puerto);
	dest_addr->sin_addr = *( (struct in_addr *)server->h_addr );
	memset( &(dest_addr->sin_zero), '\0', 8 );
	printf("Puerto cliente %d\n",ntohs(dest_addr->sin_port));
	return sock;
}

void prompt_socket(int sock,struct sockaddr_in dest_addr){
	char buffer[TAM];
	memset( buffer, 0, 1000 );
	fgets( buffer, 1000, stdin );

	while(1){
		printf( "Ingrese el mensaje a transmitir: " );
		memset( buffer, '\0', TAM );
		fgets( buffer, TAM-1, stdin );
		int tamano_direccion = sizeof( dest_addr );
		int n = sendto( sock, (void *)buffer, 1000, 0, (struct sockaddr *)&dest_addr, tamano_direccion );
		if ( n < 0 ) {
			perror( "Escritura en socket" );
			exit( 1 );
		}
		if(startsWith("fin",buffer)){
			return;
		}
	}
	return;
}

void send_file_info(int sock,struct sockaddr_in dest_addr,int estacion,float precipitacion_mensual[12],float precipitacion_diaria[365],float temperatura,float humedad){
	char buffer[TAM];
	char cadena[40];
	char nro_estacion[40];
	memset( buffer, 0, TAM );
	strcat(buffer,"Nombre estacion: \n");
	sprintf(nro_estacion,"Numero de estacion: %i\n",estacion);
	strcat(buffer,nro_estacion);
	sprintf(cadena,"Promedio Temperatura: %f\n",temperatura);
	strcat(buffer,cadena);
	sprintf(cadena,"Promedio humedad: %f\n",humedad);
	strcat(buffer,cadena);
	for (int i = 0; i < 12.; i++)
	{
		sprintf(cadena,"Mes %i: %f \n",i,precipitacion_mensual[i]);
		strcat(buffer,cadena);
	}
	int tamano_direccion = sizeof( dest_addr );
	printf("Envio por UDP: %s\n",buffer );
	fflush(stdout);
	strcat(buffer, "%3"); //Secuencia de fin
	int n = sendto( sock, (void *)buffer, TAM, 0, (struct sockaddr *)&dest_addr, tamano_direccion );
	return;
}

