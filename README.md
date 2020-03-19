# Manual de usuario Práctica 1: Red Hipercubo
**Autor:** *David Camuñas*  

La compilación y ejecución se realiza de forma automática gracias a la herramienta Makefile.

## Creación de directorios necesarios
Con este comando se crearan los diferentes directorios entre ellos el directorio que contiene el archivo ejecutable del programa.  
**$ make dirs**  

## Limpieza de directorios
Este comando se ha utilizado para limpiar los directorios en los que se almacenan los ejecutables y las clases objetos (explicada anteriormente su creación).\
**$ make clean**

## Compilación
El proceso de compilación, se puede realizar de varias formas:  
* **Automaticamente con Makefile:** *$ make hipercubeNet*

* **Manualmente:** *$ mpicc src/hipercubeNet.c -I include/ -lm -o exec/hipercubeNet*  

Donde se puede observar la opción *-I include/* la cual define donde se encuentran las librerias propias (*definitions.h*). Y la opción *-lm* esta opción define la utilización de la libreria *math.h* de **C**.

## Ejecución
Para la ejecución del programa se puede realizar al igual que la compilación de varias formas:  
* **Automaticamente con Makefile:** *$ make solution*  
Se ejecutará el ejecutable *./exec/hipercubeNet*

* **Manualmente:** *$ mpirun -n 9 ./exec/hipercubeNet* 

Donde la opción *-n 9* en este caso indica el **número de nodos que contiene la red toroidal**.
