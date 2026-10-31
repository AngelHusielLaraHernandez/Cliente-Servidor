/* Basado en programa04_servidor_internet.c del tema 3.
 * Conserva el envio de la frase del profesor y atiende un solo cliente.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in servidor = {0}, cliente;
    socklen_t longitud = sizeof(cliente);
    char *fin, ip[INET_ADDRSTRLEN];
    long puerto;
    int fd_s, fd_c, opcion = 1;
    const char mensaje[] = "El mensaje del servidor es: Conexion exitosa!\n";
    size_t enviados = 0;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s PUERTO\n", argv[0]);
        return EXIT_FAILURE;
    }
    errno = 0;
    puerto = strtol(argv[1], &fin, 10);
    if (errno || fin == argv[1] || *fin || puerto < 1 || puerto > 65535) {
        fprintf(stderr, "Puerto invalido: utiliza un numero de 1 a 65535.\n");
        return EXIT_FAILURE;
    }
    signal(SIGPIPE, SIG_IGN);
    fd_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd_s == -1) { perror("socket"); return EXIT_FAILURE; }
    if (setsockopt(fd_s, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion)) == -1) {
        perror("setsockopt"); close(fd_s); return EXIT_FAILURE;
    }
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    /* El puerto viene de la terminal, como lo indica la guia. */
    servidor.sin_port = htons((unsigned short)puerto);
    if (bind(fd_s, (struct sockaddr *)&servidor, sizeof(servidor)) == -1 ||
        listen(fd_s, 1) == -1) {
        perror("bind/listen"); close(fd_s); return EXIT_FAILURE;
    }
    printf("Servidor en puerto %ld. Esperando al cliente...\n", puerto);
    fflush(stdout);
    fd_c = accept(fd_s, (struct sockaddr *)&cliente, &longitud);
    if (fd_c == -1) { perror("accept"); close(fd_s); return EXIT_FAILURE; }
    if (inet_ntop(AF_INET, &cliente.sin_addr, ip, sizeof(ip)) == NULL) {
        perror("inet_ntop"); close(fd_c); close(fd_s); return EXIT_FAILURE;
    }
    printf("Cliente conectado desde: %s\n", ip);
    /* TCP puede requerir mas de un send() para enviar la frase completa. */
    while (enviados < strlen(mensaje)) {
        ssize_t n = send(fd_c, mensaje + enviados, strlen(mensaje) - enviados, 0);
        if (n == -1 && errno == EINTR) continue;
        if (n <= 0) { perror("send"); close(fd_c); close(fd_s); return EXIT_FAILURE; }
        enviados += (size_t)n;
    }
    printf("Texto enviado: %s", mensaje);
    close(fd_c);
    close(fd_s);
    puts("Servidor terminado.");
    return EXIT_SUCCESS;
}
