/* Basado en programa04_cliente_internet.c del tema 3.
 * Recibe e imprime la frase del servidor sin cifrado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in servidor = {0};
    char *fin, buffer[128];
    long puerto;
    int fd;
    size_t total = 0;
    ssize_t n;
    if (argc != 3) {
        fprintf(stderr, "Uso: %s IP_DEL_SERVIDOR PUERTO\n", argv[0]);
        return EXIT_FAILURE;
    }
    errno = 0;
    puerto = strtol(argv[2], &fin, 10);
    if (errno || fin == argv[2] || *fin || puerto < 1 || puerto > 65535) {
        fprintf(stderr, "Puerto invalido.\n"); return EXIT_FAILURE;
    }
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons((unsigned short)puerto);
    if (inet_pton(AF_INET, argv[1], &servidor.sin_addr) != 1) {
        fprintf(stderr, "IPv4 invalida.\n"); return EXIT_FAILURE;
    }
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) { perror("socket"); return EXIT_FAILURE; }
    if (connect(fd, (struct sockaddr *)&servidor, sizeof(servidor)) == -1) {
        perror("connect"); close(fd); return EXIT_FAILURE;
    }
    printf("Conectado a %s:%ld\n", argv[1], puerto);
    /* Se recibe hasta que el servidor cierre, sin suponer un solo recv(). */
    for (;;) {
        n = recv(fd, buffer, sizeof(buffer), 0);
        if (n == -1 && errno == EINTR) continue;
        if (n == -1) { perror("recv"); close(fd); return EXIT_FAILURE; }
        if (n == 0) break;
        if (fwrite(buffer, 1, (size_t)n, stdout) != (size_t)n) {
            perror("fwrite"); close(fd); return EXIT_FAILURE;
        }
        total += (size_t)n;
    }
    printf("Bytes recibidos: %zu\nCliente terminado.\n", total);
    close(fd);
    return total ? EXIT_SUCCESS : EXIT_FAILURE;
}
