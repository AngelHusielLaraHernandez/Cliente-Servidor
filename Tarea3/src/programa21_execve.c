#include <stdio.h>   
#include <stdlib.h> 
#include <unistd.h> 

int main(int argc, char *argv[])
{
  //Al iniciar se decalra un arreglo de cadenas de caracteres (punteros a char) llamado newargv, que contiene cinco elementos: NULL, "hola", "mundo", "buen", "dia" y NULL, donde al ser el primer elemento NULL, se indica que no hay un nombre de programa especificado para ejecutar. Los elementos "hola", "mundo", "buen" y "dia" son los argumentos que se pasarán al programa que se ejecutará.
  char *newargv[] = { NULL, "hola", "mundo", "buen", "dia",  NULL };

  //la sentencia de la condicional es por que se espera que el programa se ejecute con un argumento adicional, que es el nombre del archivo a ejecutar. Si no se proporciona exactamente un argumento (es decir, si argc no es igual a 2), se imprime un mensaje de error en la salida estándar de error (stderr) y sale del programa automaticamente.
  if( argc != 2 )
  {
    //Aqui es donde se imprime el mensaje de error en la salida estándar de error (stderr) indicando la sintaxis correcta para ejecutar el programa. Se utiliza fprintf para formatear el mensaje, donde argv[0] representa el nombre del programa que se está ejecutando. Luego, se llama a exit con EXIT_FAILURE para indicar que el programa terminó con un error.
    fprintf( stderr, "La sintaxis es: %s <archivo-a-ejecutar>\n", argv[0] );
    //Finalmente, se llama a exit con EXIT_FAILURE para indicar que el programa terminó con un error. Esto asegura que el programa no continúe ejecutándose si no se proporciona el argumento esperado.
    exit( EXIT_FAILURE );
  }

  /*
   * Algo que no mencione anteriormente, fue que, 
   * argv[0] almacena el nombre del ejecutable.
   * La sentencia newargv[0] = argv[1]; asigna el valor del segundo argumento pasado al programa (argv[1]) al primer elemento del arreglo newargv. Esto significa que el nombre del archivo a ejecutar se establece como el primer argumento para la función execve, que se utilizará para ejecutar el programa especificado por argv[1].
   */
  newargv[0] = argv[1];

  /*
   * LLAMADA A execve DENTRO DE UN BLOQUE IF:
   * Si execve tiene éxito, la imagen de memoria del proceso se sustituye
   * y esta función NUNCA retorna.
   * Si execve falla, retorna -1, por lo que entra al if, imprime el error
   * del sistema y concluye con exit(EXIT_FAILURE).
   */
  if ( execve( argv[1], newargv, NULL ) == -1 )
  {
    perror("Error en llamada al sistema execve");
    exit( EXIT_FAILURE );
  }

  /*
   * Gracias al bloque if con exit(EXIT_FAILURE), este punto es
   * completamente inalcanzable, tanto si execve tiene éxito como si falla.
   */
  printf("Esta sentencia no se ejecuta\n");
  return 0;
}