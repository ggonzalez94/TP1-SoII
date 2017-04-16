/** @file estructuraDeDatos.h
 *  @brief Estructura de datos con todos los campos necesarias para almacenar los datos del archivo
 *
 * Se almacena cada linea del archivo en una estructura de Datos.
 */

#include <time.h>

struct Datos{
	int numero;
	char nombre_estacion[50];
	int ID_localidad;
	struct tm fecha;
	float temperatura;
	float humedad;
	float punto_rocio;
	float precipitacion;
	float velocidad_viento;
	char direccion_viento[50];
	float rafaga_maxima;
	float presion;
	float radiacion_solar;
	float temperatura_suelo_1;
	float temperatura_suelo_2;
	float temperatura_suelo_3;
	float humedad_suelo_1;
	float humedad_suelo_2;
	float humedad_suelo_3;
	float humedad_hoja;
};
