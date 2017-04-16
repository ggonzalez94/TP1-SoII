/** @file server.c
 *  @brief Archivo principal del servidor. Maneja hasta 5 clientes conectados
 *
 * Este es el codigo encargado de correr el servidor.
 * Autentica a los usuarios e intercambia informacion con el cliente.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "usuarios.h"
#include "estructuraDeDatos.h"
#include "estructuraPromedios.h"
#include "serverHelpers.h"
#define TAM 10000
#define puerto 6020
#define TAM_FILE 18304 //Tamano de la estructura que guarda las lineas del archivo
#define ESTACIONES 3 //Cantidad de estaciones en el archivo CSV

int main( int argc, char *argv[] ) {
	//Declaracion de variables
	int sockfd, newsockfd, clilen, pid;
	char buffer[TAM];
	char promedio[80];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	struct Datos *datos;
	char *estaciones[ESTACIONES]; //Arreglo con las estaciones no repetidas
	float precipitacion_mensual[12]; //Uno para cada mes del ano
	float precipitacion_diaria[365]; //Uno para cada dia del ano
	struct Promedio promedio_estaciones[ESTACIONES]; //Arreglo para promedio de las estaciones
	//Usuarios de prueba
	struct Usuario usuario1;
	struct Usuario usuario2;
	usuario1.nombre_usuario = "gustavo";
	usuario1.password = "admin123";
	usuario2.nombre_usuario = "gonzalez";
	usuario2.password = "admin321";
	struct Usuario usuarios[NUMUSERS] = {usuario1,usuario2};
	
	//Inicializaciones
	datos = csv_parser();  //Lleno con los datos del archivo
	listar_estaciones(datos,promedio_estaciones,TAM_FILE,ESTACIONES); //Listo las estaciones sin repetir
	
	//Creo el socket y lo ligo a una direccion ip
	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		perror( " apertura de socket ");
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		exit( 1 );
	}
    printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );

	listen( sockfd, 5 );
	clilen = sizeof( cli_addr );

	//Loop infinito para que el server se quede en espera de nuevos clientes
	while( 1 ) {
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			perror( "accept" );
			exit( 1 );
		}

		pid = fork(); 
		if ( pid < 0 ) {
			perror( "fork" );
			exit( 1 );
		}

		if ( pid == 0 ) {  // Proceso hijo
			close( sockfd );
			
			//Validacion
			//~ memset( buffer, 0, TAM );
			//~ n = read(newsockfd,buffer,TAM-1);
			//~ if ( n < 0 ) {
				//~ perror( "lectura de socket" );
				//~ exit(1);
			//~ }
			//~ char *usuario = malloc(sizeof(char)*strlen(buffer));
			//~ strcpy(usuario,buffer);
			//~ n = read(newsockfd,buffer,TAM-1);
			//~ if ( n < 0 ) {
				//~ perror( "lectura de socket" );
				//~ exit(1);
			//~ }
			//~ char *password = malloc(sizeof(char)*strlen(buffer));
			//~ strcpy(password,buffer);
			//~ int cantidad_usuarios = sizeof(usuarios)/sizeof(usuarios[0]);
			//~ bool usuario_valido;
			//~ usuario_valido = autenticacion(usuario,password,usuarios,cantidad_usuarios);
			//~ if(usuario_valido){
				//~ printf("El usuario es correcto \n");
				//~ fflush(stdout);
			//~ }
			//~ //Termino el proceso hijo
			//~ else{
				//~ printf("El usuario es incorrecto \n");
				//~ fflush(stdout);
				//~ exit(1);
			//~ }
			
			//Acepto comandos
			while ( 1 ) {
				memset( buffer, 0, TAM );

				n = read( newsockfd, buffer, TAM-1 );
				if ( n < 0 ) {
					perror( "lectura de socket" );
					exit(1);
				}
				printf("%s \n",buffer);
				fflush(stdout);
				
				if (startsWith("listar",buffer)){
					memset( buffer, 0, TAM );
					listar_estaciones(datos,promedio_estaciones,TAM_FILE,ESTACIONES);
					for(int i=0;i<ESTACIONES;i++){
						strcat(buffer,promedio_estaciones[i].nombre_estacion);
						strcat(buffer,"\n");
					}
					strcat(buffer, "%3"); //Secuencia de fin
					n = write(newsockfd,buffer,strlen(buffer));
				}
				
				else if (startsWith("descargar",buffer)){
					strcpy(buffer,"Descarga no disponible");
					strcat(buffer, "%3"); //Secuencia de fin
					n = write(newsockfd,buffer,strlen(buffer));
				}
				
				else if(startsWith("diario_precipitacion",buffer)){
					printf("Diario precipitacion");
					fflush(stdout);
					char *estacion;
					int nro_estacion;
					char cadena[80];
					estacion = strstr(buffer,"diario_precipitacion")+strlen("diario_precipitacion") + 1;
					nro_estacion = atoi(estacion);
					diario_precipitacion(nro_estacion,precipitacion_diaria,datos,TAM_FILE);
					memset( buffer, 0, TAM );
					for(int i=0;i<365;i++){
						sprintf(estacion,"Dia: %f ",precipitacion_diaria[i]);
						printf("%s \n",cadena);
						fflush(stdout);
						strcat(buffer,cadena);
					}
					strcat(buffer, "%3"); //Secuencia de fin
					n = write(newsockfd,buffer,strlen(buffer));
				}
				
				else if(startsWith("mensual_precipitacion",buffer)){
					char *estacion;
					int nro_estacion;
					char cadena[80];
					estacion = strstr(buffer,"mensual_precipitacion")+strlen("mensual_precipitacion") + 1;
					nro_estacion = atoi(estacion);
					mensual_precipitacion(nro_estacion,precipitacion_mensual,datos,TAM_FILE);
					memset( buffer, 0, TAM );
					for(int i=0;i<12;i++){
						sprintf(cadena,"Mes: %f ",precipitacion_mensual[i]);
						printf("%s \n",cadena);
						fflush(stdout);
						strcat(buffer,cadena);
					}
					strcat(buffer, "%3"); //Secuencia de fin
					n = write(newsockfd,buffer,strlen(buffer));		
				}
				
				else if(startsWith("promedio",buffer)){
					char *variable = buffer +strlen("promedio") + 1;
					int ln = strlen (variable);
					if ((ln > 0) && (variable[ln-1] == '\n'))
						variable[ln-1] = '\0';
					puts(variable);
					calcular_promedio(variable,promedio_estaciones,datos,TAM_FILE,ESTACIONES);
					memset( buffer, 0, TAM );
					for(int i=0;i<ESTACIONES;i++){
						sprintf(promedio,"Estacion: %f ",promedio_estaciones[i].variable);
						strcat(buffer,promedio);
					}
					strcat(buffer, "%3"); //Secuencia de fin
					puts(buffer);
					n = write(newsockfd,buffer,strlen(buffer));
				}
				
				else if (startsWith("desconectar",buffer)){
					strcpy(buffer,"Finalizando conexion");
					strcat(buffer, "%3"); //Secuencia de fin
					n = write(newsockfd,buffer,strlen(buffer));
					exit(1);
				}
				
				else{
					strcpy(buffer,"Comando no disponible. Intente nuevamente");
					strcat(buffer, "%3"); //Secuencia de fin
					n = write(newsockfd,buffer,strlen(buffer));
				}
				

			}
		}
		
		else { //Proceso padre
			printf( "SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid );
			close( newsockfd );
		}
	}
	return 0; 
} 
