/*
 * Examen parcial de Arquitectura Cliente-Servidor.
 * Alumno: Lara Hernandez Angel Husiel.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in socket_cliente;
    int fd;
    char *fin;
    long puerto;
    char nombre[128], respuesta[256];
    size_t longitud, enviados = 0, recibidos = 0;
    ssize_t n;

    if (argc != 3) {
        fprintf(stderr, "Uso: %s IP_DEL_SERVIDOR PUERTO\n", argv[0]);
        return EXIT_FAILURE;
    }
    errno = 0;
    puerto = strtol(argv[2], &fin, 10);
    if (errno != 0 || fin == argv[2] || *fin != '\0' ||
        puerto < 1 || puerto > 65535) {
        fprintf(stderr, "El puerto debe ser un numero entre 1 y 65535.\n");
        return EXIT_FAILURE;
    }

    memset(&socket_cliente, 0, sizeof(socket_cliente));
    socket_cliente.sin_family = AF_INET;
    socket_cliente.sin_port = htons((unsigned short) puerto);
    if (inet_pton(AF_INET, argv[1], &socket_cliente.sin_addr) != 1) {
        fprintf(stderr, "La direccion IPv4 no es valida.\n");
        return EXIT_FAILURE;
    }

    signal(SIGPIPE, SIG_IGN);
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }
    if (connect(fd, (struct sockaddr *) &socket_cliente,
                sizeof(socket_cliente)) == -1) {
        perror("connect");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Conectado al servidor %s:%ld.\n", argv[1], puerto);

    printf("Introduce tus apellidos y nombre(s): ");
    fflush(stdout);
    if (fgets(nombre, sizeof(nombre), stdin) == NULL) {
        fprintf(stderr, "No se pudo leer el nombre.\n");
        close(fd);
        return EXIT_FAILURE;
    }
    if (strchr(nombre, '\n') == NULL && !feof(stdin)) {
        fprintf(stderr, "El nombre es demasiado largo (maximo 126 bytes).\n");
        close(fd);
        return EXIT_FAILURE;
    }
    if (nombre[strspn(nombre, " \t\r\n")] == '\0') {
        fprintf(stderr, "El nombre no puede estar vacio.\n");
        close(fd);
        return EXIT_FAILURE;
    }

    longitud = strlen(nombre);
    while (enviados < longitud) {
        n = send(fd, nombre + enviados, longitud - enviados, 0);
        if (n == -1 && errno == EINTR)
            continue;
        if (n <= 0) {
            perror("send");
            close(fd);
            return EXIT_FAILURE;
        }
        enviados += (size_t) n;
    }

    if (shutdown(fd, SHUT_WR) == -1) {
        perror("shutdown");
        close(fd);
        return EXIT_FAILURE;
    }

    while (recibidos < sizeof(respuesta) - 1) {
        n = recv(fd, respuesta + recibidos,
                 sizeof(respuesta) - 1 - recibidos, 0);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("recv");
            close(fd);
            return EXIT_FAILURE;
        }
        if (n == 0)
            break;
        recibidos += (size_t) n;
        if (memchr(respuesta, '\n', recibidos) != NULL)
            break;
    }
    if (recibidos == 0) {
        fprintf(stderr, "El servidor cerro la conexion sin responder.\n");
        close(fd);
        return EXIT_FAILURE;
    }
    respuesta[recibidos] = '\0';
    printf("Respuesta del servidor: %s", respuesta);
    if (respuesta[recibidos - 1] != '\n')
        printf("\n");

    close(fd);
    printf("Cliente terminado.\n");
    return EXIT_SUCCESS;
}
