/*
 * Muestra cómo en la terminal se pasan los argumentos 
 * hacia la función main del proceso.
 */

#include <stdio.h>   
#include <stdlib.h>  

// main recibe el número de argumentos (argc) y el vector de cadenas (argv)
int main(int argc, char *argv[])
{
  int j; // Variable iteradora para recorrer el arreglo de argumentos

  // Itera desde el índice 0 hasta argc - 1
  for( j = 0; j < argc; j++ )
  {
    // Imprime el índice actual y la cadena correspondiente al argumento
    printf("argv[%d]: %s\n", j, argv[j]);
  }

  // Termina el proceso exitosamente devolviendo un código 0 al SO
  exit( EXIT_SUCCESS );
}