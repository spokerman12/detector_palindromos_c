
/*
 *	@brief: Funcion sameChar: Compara dos caracteres para saber si son iguales.
 *
 *	@param: c1: Primer caracter.
 *	@param:	c2: Segundo caracter.
 *
 *	@return: Entero que representa si son iguales los caracteres o no.
 *
 */

int sameChar(char c1, char c2)
{
	if(c1  < 'a' && c2 >= 'a')
	{
		return(c1+32==c2);
	}

	if(c2  < 'a' && c1 >= 'a')
	{
		return(c2+32==c1);
	}

	return(c1==c2);
}

/*
 *	@brief: Funcion printSubarray: Imprime un arreglo de caracteres.
 *
 *	@param: array: Arreglo de caracteres.
 *	@param:	i: Entero que indica desde donde imprimir.
 *	@param:	j: Entero que indica hasta donde imprimir.
 *
 */

void printSubarray(char array[], int i, int j)
{
	int len = strlen(array);
	//printf("i es %d, j  es %d\n", i,j);

	if (j-i > 1)
	{
		while (i <= j)
		{
			printf("%c",array[i]);
			i++;
		}
		//printf("\n");
	}

	
	
}

/*
 *	@brief: Funcion esPalindromo: Determina si un string es un palindromo. Llama a la funcion sameChar.
 *
 *	@param: palabra: Arreglo de caracteres que representa el string donde se buscara la exisencia de un palindromo.
 *	@param:	i: Entero que indica el primer caracter a comparar.
 *	@param:	j: Entero que indica el segundo caracter a comparar.
 *
 *	@return: Entero que representa si es palindromo o no.
 *
 */

int esPalindromo(char palabra[], int i, int j)
{
	while(i < j)
	{
		if(!sameChar(palabra[i],palabra[j]) || (j-i<1))
			return(false);

		i++;
		j--;
	}

	return(true);
}

/*
 *	@brief: Funcion buscaPalindromo: Busca en un arreglo de string la existencia de un palindromo. Llama a la funcion esPalindromo.
 *
 *	@param: palabra: Arreglo de caracteres que representa el string donde se buscara la exisencia de un palindromo.
 *
 *	@return: Entero por defecto para finalizar la funcion.
 *
 */

int buscaPalindromo(char palabra[])
{
	int i = 0;
	int len = strlen(palabra);
	int j = len-1;
	int k = 0;

	while (i < len-1)
	{
		while(i < j)
		{
			if (esPalindromo(palabra,i,j))
			{
				if (k == 0)
				{
					printSubarray(palabra,i,j);	
				}
				
				if (k != 0)
				{
					printf(",");
					printSubarray(palabra,i,j);	
				}

				k++;
			}

			j--;
		}

		j = len;
		i++;
	}


	if (k==0)
	{
		printf("No existen\n");
		return(0);
	}
	printf("\n");
}

/*
 *	@brief: Funcion limpiaString: Elimina los / del string a evaluar y luego lo almacena en un archivo temporal.
 *
 *	@param: string: Arreglo de caracteres que representa el string donde se buscara la exisencia de un palindromo.
 *	@param:	*file: Apuntador al archivo temporal donde sera almacenado el string.
 *
 *	@return: Apuntador a un string.
 *
 */

const char * limpiaString(char string[], FILE *file)
{
	int i = 0;
	int len = strlen(string);
	char nuevoString[len];
	int j = 0;

	while (i <= len)
	{
		if (string[i] != '/')
		{
			nuevoString[j] = string[i];
			j++;
		}

		i++; 		
	}

	fprintf(file, "%s\n", nuevoString);
}

/*
 *	@brief: Funcion verificaDirectorio: Verifica si lo que el programa lee actualmente es un archivo o un directorio.
 *
 *	@param: *path: Apuntador a un string que representa el camino del directorio que se esta leyendo. 
 *
 *	@return: Entero que indica si lo que se lee es un directorio o no.
 *
 */

int verificaDirectorio(const char *path)
{
	struct stat path_stat;
	stat(path, &path_stat);
	return S_ISDIR(path_stat.st_mode);
}

/*
 *	@brief: Funcion visitaDirectorio: Recorrido de un arbol de directorios.
 *
 *	@param: *dp: Apuntador al directorio en el que se encuentra el programa.
 *	@param:	* actual: Apuntador al string que representa el camino desde la raiz hasta el directorio actual. 
 *	@param: * cwd: Apuntador al string que representa el camino desde el directorio de inicio de recorrido, hasta el directorio actual.
 *	@param: alturaMaxima: Entero que representa la altura maxima que puede tener el arbol de directorios.
 *	@param: alturaActual: Entero que representa la altura actual que tiene el arbol de directorios.
 *	@param: *file: Apuntador al archivo donde luego se guardan los strings enlos cuales se buscaran los palindromos.
 *	@param: incluyeArchivos: Entero que indica si el programa debe tomar en cuenta los archivos como hojas del arbol.
 *
 *	@return: Entero por defecto para finalizar la funcion.
 *
 */

int visitaDirectorio(DIR *dp, char * actual, char * cwd, int alturaMaxima, int alturaActual, FILE *file, int incluyeArchivos)
{
	alturaActual++;

	if (alturaActual < alturaMaxima)
	{
		struct dirent *ep;
		int num;
		char dirActual[256];
		char nuevoCwd[256];


		if (dp != NULL)
		{

			while (ep = readdir(dp))
			{
				
				strcpy(dirActual,actual);
				strcat(dirActual,"/");
				strcat(dirActual,ep->d_name);

				strcpy(nuevoCwd,cwd);
				strcat(nuevoCwd,"/");
				strcat(nuevoCwd,ep->d_name);
				num = verificaDirectorio(dirActual);

				if (num != 0)
				{
					if (strcmp(ep->d_name,".") != 0)
					{
						if (strcmp(ep->d_name,"..") != 0)
						{
							DIR *newDir;

							newDir = opendir(dirActual);
							visitaDirectorio(newDir,dirActual,nuevoCwd,alturaMaxima,alturaActual,file,incluyeArchivos);    
						}
					}
				}

				else
				{
					if (incluyeArchivos == 1)
					{
						char archivo[256];
						strcpy(archivo,cwd);
						strcat(archivo,ep->d_name);
						limpiaString(archivo,file);
					}
				}
			}

			limpiaString(cwd,file);
			(void) closedir(dp);
		}

		else
			perror ("No se pudo abrir directorio");
	}

	else
	{
		limpiaString(cwd,file);
	}

	return 0;
}