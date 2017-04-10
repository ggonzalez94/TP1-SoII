#define _GNU_SOURCE

#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "usuarios.h"
#include "serverHelpers.h"
#include "estructuraDeDatos.h"



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

struct Datos *csv_parser(){
	char buffer[BUFFER_SIZE];
	FILE *fp;
	static struct Datos datos[TAM_FILE];
	int indice = 0; //Indice para ir llenando los datos
	fp = fopen("../../datos_test.csv","r");
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
		datos[indice].temperatura_suelo_1 = atof(temperatura_suelo_1);
		datos[indice].temperatura_suelo_2 = atof(temperatura_suelo_2);
		datos[indice].temperatura_suelo_3 = atof(temperatura_suelo_3);
		datos[indice].humedad_suelo_1 = atof(humedad_suelo_1);
		datos[indice].humedad_suelo_2 = atof(humedad_suelo_2);
		datos[indice].humedad_suelo_3 = atof(humedad_suelo_3);
		datos[indice].humedad_hoja = atof(humedad_hoja);
		//printf("Numero: %i Nombre: %s ID: %i\n",datos[indice].numero,datos[indice].nombre_estacion,datos[indice].ID_localidad);
		//printf("Fecha: %i \n",datos[indice].fecha.tm_min);
		indice++;

	}
	fclose(fp);
	return datos;
}

void listar_estaciones(struct Datos * const datos, char *estaciones[],int size_datos){
	//Uso un ptr auxiliar para no modificar el puntero datos
	struct Datos *datos_auxiliar = datos;
	for(int i=0;i<size_datos;i++){
		estaciones[i] = datos_auxiliar->nombre_estacion;
		datos_auxiliar++;
	}
	return;
}

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
			array_precipitacion[datos_auxiliar->fecha.tm_mon]+=datos_auxiliar->precipitacion;
		}
		datos_auxiliar++;
	}
	return;
}

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
			array_precipitacion[datos_auxiliar->fecha.tm_yday]+=datos_auxiliar->precipitacion;
		}
		datos_auxiliar++;
	}
	return;
}


