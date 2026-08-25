#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int puerto = 80;               /* Segmento Data: Variable global inicializada */
int conexiones_pendientes;     /* Segmento BSS: Variable global no inicializada (inicia en 0) */

int main(int argc, char *argv[]) {
    int socket_fd = 5;         /* Pila (Stack): Variable local */
    char *buffer = malloc(1024); /* Montículo (Heap): Asignación dinámica de memoria */
    
    /* 5. Segmento de Código (Text): Las instrucciones compiladas */
    printf("Proceso en ejecución con PID: %d\n", getpid());
    printf("Ve a tu segunda terminal y ejecuta pmap. Luego presiona Enter aquí para terminar...\n");
    
    /* Pausa la ejecución en espera de entrada estándar */
    getchar(); 
    
    free(buffer);
    return 0;
}
