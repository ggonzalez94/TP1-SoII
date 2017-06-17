/** @file cliente.c
 *  @brief Codigo fuente del cliente. Cada cliente se conecta a un servidor del que pide los datos de las estaciones.
 *
 * 	Para la conexion se debe tener la IP y puerto del servidor.
 * 	El comando utilizado es connect usuario@numero_ip:port
 * 	Si el usuario ingresado es invalido la conexion muere.
 * 	En caso de ser valido el usuario podra ejecutar los siguientes comandos:
 * 	-listar
 * 	-descargar nro estacion
 * 	-diario_precipitacion no_estación
 * 	-mensual_precipitacion no_estación
 * 	-promedio variable
 * 	-desconectar
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h> //inet_addr

#include "cliente.h"
#include "usuarios.h"
#include "colores.h"



int main( int argc, char *argv[] ) {
	int sockfd, n;
	int socket_udp;
	char buffer_server[50];
	char *ip,*puerto,*usuario;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int terminar = 0;

	char buffer[TAM];

	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sockfd < 0 ) {
		perror( "ERROR apertura de socket" );
		exit( 1 );
	}
	


	
	//Sistema de autenticacion
	printf("Porfavor ingrese su nombre de usuario,ip del servidor y numero de puerto \n");
	printf("connect usuario@numero_ip:port \n");
	fgets(buffer_server,sizeof(buffer_server),stdin);
	//Verifico si comienza con la palabra connect,caso contrario termino el programa
	char *conexion = "connect";
	if(!startsWith(conexion,buffer_server)){
		perror("Comando de conexion invalido");
		exit(1);
	}
	ip = strstr(buffer_server,"@")+1;
	puerto = strtok(strstr(ip,":")+1,"\n");
	ip = strtok(ip,":");
	usuario = strtok(buffer_server,"@");
	usuario = strstr(usuario,"connect")+strlen("connect")+1;
	
	//Armo la estructura serv_addr para ligarla con el socket y conectarme al server
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(puerto));
	if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr ) ) < 0 ) {
		perror( "conexion" );
		exit( 1 );
	}
	
	//~ //Envio los datos del usuario
	strcat(usuario, "%3"); //Secuencia de fin
	n = write (sockfd,usuario,strlen(usuario));
	
	//Prompt para pedir password
    printf(BOLDGREEN"Ingerese su contrasena: ");
    printf("> " RESET);
    fgets(buffer,TAM,stdin);
    strtok(buffer, "\n");
    strcat(buffer, "%3"); //Secuencia de fin
    n = write (sockfd,buffer,strlen(buffer));
    if ( n < 0 ) {
		perror( "escritura de socket" );
		exit(1);
	}
	printf("Pass: %s \n",buffer);
		
	

	 while(1) {
		printf( "Ingrese el mensaje a transmitir: " );
		memset( buffer, '\0', TAM );
		fgets( buffer, TAM-1, stdin );

		strcat(buffer, "%3"); //Secuencia de fin
		n = write( sockfd, buffer, strlen(buffer) );
		if ( n < 0 ) {
			perror( "escritura de socket" );
			exit( 1 );
		}

		if (startsWith("descargar",buffer)){
			printf("Iniciando socket udp\n");
			socket_udp = start_udp_socket(socket_udp);
			prompt_socket(socket_udp,serv_addr);
		}

		memset( buffer, '\0', TAM );
		n = read_all( sockfd, buffer, TAM );
		
		if ( n < 0 ) {
			perror( "socket tcp" );
			exit( 1 );
		}
		printf("%s",buffer);
		fflush(stdout);
		
		if (startsWith("Finalizando conexion",buffer)){
			exit(1);
		}
	 }
	return 0;
} 
