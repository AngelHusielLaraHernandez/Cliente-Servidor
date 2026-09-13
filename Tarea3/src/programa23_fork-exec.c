
#include <stdio.h>      
#include <stdlib.h>    
#include <sys/types.h>  
#include <unistd.h>     

/*

 * El proposito de esta funcion es que es duplicar el proceso llamador mediante fork() y ejecuta un nuevo
 * y lo que hara el proceso hijo es usar execvp().
 * lo que se le pasa a la funcion es:
 *   - program: Nombre o ruta del programa a ejecutar (ej. "ls").
 *   - arg_list: Lista de argumentos terminada en NULL.
 * y al final lo que nos dara es el PID del proceso hijo al proceso padre.:
 */
int spawn (char* program, char** arg_list)
{
  pid_t child_pid;

  // Clona el proceso actual creando un proceso hijo
  child_pid = fork ();

  if (child_pid != 0)
  {
    // Bloque ejecutado por el proceso PADRE (child_pid contiene el PID del hijo)
    return child_pid;
  }
  else
  {
    // Bloque ejecutado por el proceso HIJO esto es cuando el proceso hijo es creado osea un child_pid == 0

    /*
     * Si execvp() tiene éxito, la imagen del hijo es reemplazada por el programa
     * y jamás llega a la línea siguiente.
     * Si falla, retorna -1, se procesa el error y finaliza con exit(EXIT_FAILURE).
     */
    if ( execvp (program, arg_list) == -1 )
    {
      perror ("Ocurrió un error al ejecutar execvp");
      exit( EXIT_FAILURE ); // Termina el proceso hijo con estado de error
    }

    // este return se pone, por que si execvp falla, el proceso hijo no termina y continua ejecutando el código, por lo que se pone un return 1 para indicar que hubo un error en la ejecución del programa.
    return 1;
  }
}

int main ()
{
  // Lista de argumentos para el comando "ls -l /"
  // arg_list[0] es el nombre del comando
  // El último elemento debe ser NULL como final de la lista
  char* arg_list[] = {
    "ls",
    "-l",
    "/",
    NULL
  };

  // El padre crea al hijo y le indica que ejecute "ls" con sus argumentos
  spawn ("ls", arg_list);

  // El proceso padre continúa inmediatamente de forma concurrente
  printf ("Termina el proceso padre\n");

  return 0;
}