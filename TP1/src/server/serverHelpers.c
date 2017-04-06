#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "usuarios.h"
#include "serverHelpers.h"



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
