# Tecnología de Videojuegos y Machine Learning desde cero
Este repositorio es en el que almaceno el resultado de mi **Trabajo de Fin de Grado (TFG)** para el grado de **Ingeniería Informática** en **la Universidad de Alicante (UA)**. En él encontrarás tanto la memoria como el código fuente del proyecto desarrollado por mí desde cero.

La memoria de este proyecto ha sido desarrollada en LaTeX, y para ser compilado puede ser usado cualquier compilador, como por ejemplo xelatex.

## Instalación
En caso de querer compilar el proyecto, este está preparado con un Makefile para ello. Las dependencias del proyecto son X11, GLFW, OpenGL y GLEW, asegúrate de tenerlo instalado, de no ser así búscalo en los repositorios del gestor de paquetes de tu distribución Linux. El proyecto está hecho para ser compilado en Linux, pero se puede compilar para Windows con la herramienta MinGW, el Makefile está preparado para ello.
Una vez instaladas las dependencias, por supuesto también hará falta tener instalado git y make. Los comandos que has de ejecutar son los siguientes:
```
git clone https://github.com/arturo-source/tfg-motor-juego-y-ia.git
cd tfg-motor-juego-y-ia
make libs
make
```
En caso de tener ya el proyecto descargado, no es necesario ejecutar el primer comandos.

## Ejecución
Una vez compilado, ejecutarás `./game` desde la misma carpeta y se abrirá una ventana con el juego. Y aparecerá el siguiente menú en la ventana:

![Menú del juego](https://github.com/arturo-source/tfg-motor-juego-y-ia/blob/master/Memoria/archivos/imagenes/menu-inicia-definitivo.png)

En él podrás seleccionar las opciones de jugar, hacer arena, o entrenar. También editar un fichero de pesos de una red neuronal, y salir del programa. 
- La opción de **Play** es solo para jugar entre dos inteligencias artificiales, por lo que tendrás que escoger dos ficheros de pesos. En caso de no escoger un fichero, se asigna una IA diseñada por defecto.
- La opción de **Arena** sirve para obtener dataset con los que entrenarás a la IA, has de mover tú la pala. Elige un lado y se generará un dataset a partir de los movimientos que hagas durante la partida. Presiona la tecla `esc` para salir del modo Arena.
- La opción de **Train** te permite coger un dataset de los extraídos del modo arena, y entrenar a una red neuronal. En este menú puedes escoger diferentes parámetros para ajustar el entrenamiento de la red.
- La opción de **Edit neural network** sirve para editar los pesos uno a uno individualmente, y te permite ver cuánto error comete la red conforme lo vas editando.

Los dataset se guardan en una carpeta llamada `dataset_CSVs`, y los pesos de las redes neuronales en una capeta llamada `weights_CSVs`. Por defecto vienen unos archivos en cada carpeta, que han sido escogidos por mí, en caso de querer probar el juego sin necesidad de entrenar previamente. 

## Controles
- `esc`: sirve para salir de cualquier modo de juego en el que te hayas metido
- `w`: la pala de la izquierda va hacia arriba
- `s`: la pala de la izquierda va hacia abajo
- `o`: la pala de la derecha va hacia arriba
- `l`: la pala de la derecha va hacia abajo
