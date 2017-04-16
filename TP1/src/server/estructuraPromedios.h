/** @file estructuraPromedios.h
 *  @brief Estructura usada para listar las estaciones(sin repetir) y calcular los promedios de la variable solicitada
 *
 */
struct Promedio{
	int numero;
	char nombre_estacion[50];
	int cantidad_lecturas; //Variable para luego calcular promedio
	float variable;
};
