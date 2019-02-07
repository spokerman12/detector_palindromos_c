# Detector de palíndromos en el sistema de archivos

Para la realización del proyecto se requiere que un programa sea capaz de
navegar un árbol de directorios UNIX, con el objetivo de encontrar palíndromos en
el resultado de concatenar los nombres de cada carpeta. Esta
tarea
debe
realizarse con la implementación de procesos hijo, a través de la instrucción
“fork()”.
También se solicita implementar diferentes parámetros opcionales para la
ejecución de la tarea. Estos parámetros permiten al programa comenzar desde un
directorio específico al proveer el path absoluto del mismo (flag -d <carpeta>),
que el programa descienda un altura específica en el árbol (flag -m <altura>) y
finalmente, la opción de incluir nombres de archivos en el análisis (flag -f).