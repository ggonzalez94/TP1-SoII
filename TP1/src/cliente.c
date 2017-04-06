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
#include<arpa/inet.h> //inet_addr

#include "cliente.h"
#include "usuarios.h"
#include "colores.h"
#define TAM 256



int main( int argc, char *argv[] ) {
	int sockfd, n;
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
	//Inicializo mi usuario con el nombre y contrasena
	struct Usuario usuario_actual;
	usuario_actual.nombre_usuario = usuario;
	//usuario_actual.password = password
	//Envio los datos del usuario
	n = write (sockfd,usuario,strlen(usuario));
	//~ //Prompt para pedir password
    printf(BOLDGREEN"Ingerese su contrasena: ");
    printf("> " RESET);
    fgets(buffer,TAM,stdin);
    strtok(buffer, "\n");
    n = write (sockfd,buffer,strlen(buffer));
    if ( n < 0 ) {
		perror( "escritura de socket" );
		exit(1);
	}
	printf("Pass: %s \n",buffer);
		
	
	//~ memset( (char *) &serv_addr, '0', sizeof(serv_addr) );
	//~ serv_addr.sin_family = AF_INET;
	//~ bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	//~ serv_addr.sin_port = htons( puerto );

	//~ while(1) {
		//~ printf( "Ingrese el mensaje a transmitir: " );
		//~ memset( buffer, '\0', TAM );
		//~ fgets( buffer, TAM-1, stdin );

		//~ n = write( sockfd, buffer, strlen(buffer) );
		//~ if ( n < 0 ) {
			//~ perror( "escritura de socket" );
			//~ exit( 1 );
		//~ }

		//~ // Verificando si se escribió: fin
		//~ buffer[strlen(buffer)-1] = '\0';
		//~ if( !strcmp( "fin", buffer ) ) {
			//~ terminar = 1;
		//~ }

		//~ memset( buffer, '\0', TAM );
		//~ n = read( sockfd, buffer, TAM );
		//~ if ( n < 0 ) {
			//~ perror( "lectura de socket" );
			//~ exit( 1 );
		//~ }
		//~ printf( "Respuesta: %s\n", buffer );
		//~ if( terminar ) {
			//~ printf( "Finalizando ejecución\n" );
			//~ exit(0);
		//~ }
	//~ }
	return 0;
} 
