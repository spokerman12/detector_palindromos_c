/* 
 * Universidad Simon Bolivar
 * Departamento de Computación y Tecnologı́a de la Información
 * Sistemas de Operacion I - CI3825
 * Trimestre Enero-Marzo 2018
 *
 * Proyecto 1
 * 
 * Detector de Palindromos en el Sistema de Archivos: 	Este programa permite recorrer directorios en forma de
 *                         								arbol y busca si existen palindromos en la union de los
 *                         								nombres de los directorios visitados y sus archivos.
 *
 * Ultima modificacion: 28.02.2018
 *
 * @Autores: Daniel Francis. 12-10863
 *          Javier Vivas. 12-11067
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "funcionesProy1.h"


int main(int argc, char *argv[])
{

	// Establece por defecto la altura del arbol de recorrido,
	// que se lean los archivos y el directorio de inicio
	int alturaMaxima = 20;
	int alturaActual = 0;
	int incluyeArchivos = 0;
	int dir = 0;
	char cwdNuevo[56];
	char cwdOriginal[56];
	char pathLog[56];
	int status;


	// Se crea temporalmente un archivo log.txt donde se almacenaran los strings 
	FILE *log = fopen("log.txt","w");
	getcwd(cwdOriginal,sizeof(cwdOriginal));
	strcpy(pathLog,cwdOriginal);
	strcat(pathLog,"/log.txt");

	while ((argc > 1) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			// Caso en el que se solicite cambiar el directorio de inicio
			case 'd':
				printf("Iniciando desde %s \nSi no existe, se usa el directorio nativo\n", argv[2]);
				dir = 1;
				char * directorio = argv[2];
				chdir(directorio);

				// Obtiene el path actual
				if (getcwd(cwdNuevo, sizeof(cwdNuevo)) != NULL)
					//printf("El dir es %s\n", cwdNuevo);
					;
				else
					perror("Error!");

				break;

			// Caso en el que se solicite cambiar la altura maxima del arbol de directorios
			case 'm':
				printf("Altura %d\n",atoi(argv[2]));
				alturaMaxima = atoi(argv[2]);
				break;

			// Caso en el que se desee que se lean los nombres de los archivos para buscar palindromos
			case 'f':
				printf("Incluye archivos = True\n");
				incluyeArchivos = 1;
		}

		++argv;
		--argc;
	}

	// Creacion del proceso hijo
	pid_t child_pid = fork();

	if (child_pid < 0)
	{
		printf("Fallo el fork!\n");
		exit(1);
	}

	if (dir == 0 && child_pid == 0)
	{
		char cwd[56];
		// Obtiene el path actual
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			//printf("El dir es %s\n", cwd);
			;
		else
			perror("Error!");

		int i = 0;
		int temp;
		int len = strlen(cwd);
		// Elimina del path todos los / encontrados
		while (i < len)
		{
			if (cwd[i] == '/')
			{
				temp = i;
			}

			i++;
		}

		i = 0;
		char nuevoCwd[len];

		while (temp <= len)
		{
			nuevoCwd[i] = cwd[temp];
			i++;
			temp++;
		}
		//printf("El dir es %s\n", nuevoCwd);

		// Crea un apuntador al directorio de inicio y abre el directorio para explorarlo
		DIR *d;
		struct dirent *dir;
		d = opendir(".");
		// Llama a la funcion visitaDirectorio sobre la direccion por defecto
		visitaDirectorio(d,cwd,nuevoCwd,alturaMaxima,alturaActual,log,incluyeArchivos);	
	}

	if (dir == 1 && child_pid == 0)
	{
		int i = 0;
		int temp;
		int len = strlen(cwdNuevo);
		// Elimina del path todos los / encontrados
		while (i <= len)
		{
			if (cwdNuevo[i] == '/')
			{
				temp = i;
			}

			i++;
		}

		i = 0;
		char nuevoCwd[len];
		while (temp <= len)
		{
			nuevoCwd[i] = cwdNuevo[temp];
			i++;
			temp++;
		}

		// Crea un apuntador al directorio de inicio y abre el directorio para explorarlo
		DIR *d;
		struct dirent *dir;
		d = opendir(cwdNuevo);
		// Llama a la funcion visitaDirectorio sobre la direccion proporcionada por el usuario
		visitaDirectorio(d,cwdNuevo,nuevoCwd,alturaMaxima,alturaActual,log,incluyeArchivos);	
	}

	wait(&status);

	rewind(log);
	log = fopen(pathLog,"r+");
	
	
	if (child_pid != 0)
	{	
		const size_t line_size = 300;
		char* line = malloc(line_size);

		// Llama a la funcion buscaPalindromo sobre todas las lineas del archivo log.txt creado temporalmente
		while (fgets(line, line_size, log) != NULL)  
		{
			printf("Palindromos en:%s", line);
    		buscaPalindromo(line);
    	}

    
    free(line);
    remove("log.txt");
	
	fclose(log);

	}


	return 0;
}