#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include "estructuraDeDatos.h"

#define BUFFER_SIZE 10000
#define CANT_DAT 8

int main(){
	char buffer[BUFFER_SIZE];
	FILE *fp;
	struct Datos datos[8];
	int indice = 0; //Indice para ir llenando los datos
	fp = fopen("../datos_test.csv","r");
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
		printf("Numero: %i Nombre: %s ID: %i\n",datos[indice].numero,datos[indice].nombre_estacion,datos[indice].ID_localidad);
		indice++;

	}
	
	return 0;
}

