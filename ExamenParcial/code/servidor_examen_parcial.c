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
    struct sockaddr_in servidor, cliente;
    socklen_t long_cliente = sizeof(cliente);
    int fd_s, fd_c;
    int reutilizar = 1;
    char *fin;
    long puerto;
    char ip_cliente[INET_ADDRSTRLEN];
    char nombre[128];
    const char respuesta[] = "Registraste exitosamente tu nombre!\n";
    size_t recibidos = 0, enviados = 0;
    ssize_t n;

    if (argc != 2) {
        fprintf(stderr, "Uso: %s PUERTO\n", argv[0]);
        return EXIT_FAILURE;
    }
    errno = 0;
    puerto = strtol(argv[1], &fin, 10);
    if (errno != 0 || fin == argv[1] || *fin != '\0' ||
        puerto < 1 || puerto > 65535) {
        fprintf(stderr, "El puerto debe ser un numero entre 1 y 65535.\n");
        return EXIT_FAILURE;
    }

    signal(SIGPIPE, SIG_IGN);
    fd_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd_s == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    if (setsockopt(fd_s, SOL_SOCKET, SO_REUSEADDR,
                   &reutilizar, sizeof(reutilizar)) == -1) {
        perror("setsockopt");
        close(fd_s);
        return EXIT_FAILURE;
    }

    memset(&servidor, 0, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);
    servidor.sin_port = htons((unsigned short) puerto);

    if (bind(fd_s, (struct sockaddr *) &servidor, sizeof(servidor)) == -1) {
        perror("bind");
        close(fd_s);
        return EXIT_FAILURE;
    }
    if (listen(fd_s, 1) == -1) {
        perror("listen");
        close(fd_s);
        return EXIT_FAILURE;
    }

    printf("Servidor iniciado en el puerto %ld.\n", puerto);
    printf("Esperando la conexion del cliente...\n");
    fflush(stdout);
    fd_c = accept(fd_s, (struct sockaddr *) &cliente, &long_cliente);
    if (fd_c == -1) {
        perror("accept");
        close(fd_s);
        return EXIT_FAILURE;
    }

    if (inet_ntop(AF_INET, &cliente.sin_addr,
                  ip_cliente, sizeof(ip_cliente)) == NULL) {
        perror("inet_ntop");
        close(fd_c);
        close(fd_s);
        return EXIT_FAILURE;
    }
    printf("IP del cliente: %s\n", ip_cliente);

    while (recibidos < sizeof(nombre) - 1) {
        n = recv(fd_c, nombre + recibidos,
                 sizeof(nombre) - 1 - recibidos, 0);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("recv");
            close(fd_c);
            close(fd_s);
            return EXIT_FAILURE;
        }
        if (n == 0)
            break;
        recibidos += (size_t) n;
        if (memchr(nombre, '\n', recibidos) != NULL)
            break;
    }
    nombre[recibidos] = '\0';
    nombre[strcspn(nombre, "\r\n")] = '\0';
    if (nombre[0] == '\0') {
        fprintf(stderr, "El cliente no envio un nombre.\n");
        close(fd_c);
        close(fd_s);
        return EXIT_FAILURE;
    }
    printf("Nombre recibido: %s\n", nombre);

    while (enviados < strlen(respuesta)) {
        n = send(fd_c, respuesta + enviados, strlen(respuesta) - enviados, 0);
        if (n == -1 && errno == EINTR)
            continue;
        if (n <= 0) {
            perror("send");
            close(fd_c);
            close(fd_s);
            return EXIT_FAILURE;
        }
        enviados += (size_t) n;
    }
    printf("Respuesta enviada: %s", respuesta);

    close(fd_c);
    close(fd_s);
    printf("Servidor terminado.\n");
    return EXIT_SUCCESS;
}
