
 - Para instalar el programa, descargue el repositorio en su computadora.
 - Luego, mediante la terminal, ejecute los siguientes comandos para descomprimir el archivo .zip y situarse en la carpeta donde se halla el Makefile:# TP1-SO-2016 

### TP1-So2
___
|Autores                               |
|------------------------------------|
|Gonzalez Somerstein Gustavo -7721064  |

## 1. Descripción del programa
---
El programa permite conectarse a un servidor que posee los datos de varias estaciones meteorologicas.

## 2. Instrucciones de instalación
---
```
% cd Downloads
% unzip TP1-SO2.zip
% cd TP1-SO2/TP1/src/cliente  (para el cliente)
% cd TP1-SO2/TP1/src/servidor  (para el servidor)
```

```
 - Para comenzar el proceso de compilación y linkeo de los archivos, ejecute el comando:
```
% make
```
 
 - Una vez terminado, para visualizar en consola la información, ejecute el comando: 
```
% ./server (dentro de la carpeta server)
% ./cliente (dentro de la carpeta cliente)
```

```
Si desea más información, remítase a la documentación proporcionada, que se encuentra en la ruta ```\TP1\doc\html\index.html``
