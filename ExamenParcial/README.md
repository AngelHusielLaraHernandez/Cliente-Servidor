# Examen parcial: Arquitectura Cliente-Servidor

Para el caso de este examen se tomó como base el par de programas 04 del tema 3. En esos ejemplos el servidor envía primero una frase y el cliente la recibe. Ahora bien, lo que se modificó fue el orden de la comunicación, para que el cliente lea el nombre desde el teclado, lo envíe al servidor y después reciba su respuesta.

El trabajo realizado hasta este momento corresponde únicamente a la prueba local en la Mac. La conexión al servidor del profesor queda pendiente, por lo que todavía no se considera completa la entrega del examen.

## Código utilizado

- `code/servidor_examen_parcial.c`: recibe el puerto desde la línea de comandos, lo convierte con `htons()`, acepta una conexión, muestra la IP del cliente y el nombre recibido, envía una respuesta y termina.
- `code/cliente_examen_parcial.c`: recibe la IP y el puerto del servidor como argumentos. Después de conectarse, utiliza `fgets()` para leer los apellidos y el nombre desde el teclado, incluyendo los espacios. El nombre no está colocado como mensaje fijo dentro del programa.

Lo que vemos en el servidor es que `accept()` también llena la estructura con la dirección del cliente. Para mostrar esa dirección como una IP se utiliza `inet_ntop()`. Por otra parte, el cliente utiliza `inet_pton()` para colocar la IPv4 del servidor en la estructura de conexión.

Además de eso, se revisa el resultado de las llamadas para detectar errores de conexión. El puerto debe estar entre 1 y 65535 y se convierte a orden de red con `htons()`. Se conserva la cola de espera de una conexión que utiliza el ejemplo original. La opción `SO_REUSEADDR` permite repetir la prueba en el mismo puerto.

Ahora bien, TCP puede entregar los datos en varias lecturas, por lo que se acumula lo recibido hasta encontrar el salto de línea o el cierre del envío. En el cliente se utiliza `shutdown(fd, SHUT_WR)` después de enviar el nombre; esto termina solamente el envío y permite continuar recibiendo la respuesta del servidor. Al final ambos programas cierran sus sockets y terminan.

## Compilación en la Mac

Desde una terminal normal, ingresar a esta carpeta:

```text
cd /Users/angellara/Documents/NovenoSemestre/Cliente-Servidor/ExamenParcial
```

Después ejecutar cada comando por separado:

```text
mkdir -p bin
```

```text
clang -std=c11 -Wall -Wextra -Wpedantic -Werror code/servidor_examen_parcial.c -o bin/servidor_examen_parcial
```

```text
clang -std=c11 -Wall -Wextra -Wpedantic -Werror code/cliente_examen_parcial.c -o bin/cliente_examen_parcial
```

Los ejecutables quedan en `bin/`, separados de los archivos fuente. Esta carpeta está excluida de Git. Para el entregable del profesor se utilizan los dos archivos `.c`; los ejecutables no se adjuntan.

## Prueba local y captura pendiente

En la primera terminal, desde esta carpeta, ejecutar:

```text
./bin/servidor_examen_parcial 4898
```

En una segunda terminal, ingresar a la misma carpeta y ejecutar:

```text
./bin/cliente_examen_parcial 127.0.0.1 4898
```

Cuando el cliente solicite el nombre, escribir desde el teclado y presionar Enter:

```text
LARA HERNANDEZ ANGEL HUSIEL
```

Para el caso de esta prueba, el servidor mostró la IP `127.0.0.1` y el nombre completo que recibió. Después envió la respuesta `Registraste exitosamente tu nombre!`, que el cliente imprimió en su terminal. Ambos programas terminaron con estado de salida 0.

La salida real de esa ejecución se encuentra en `evidencia/prueba_local.txt`. Este archivo es un registro de texto; no sustituye la captura de pantalla. Falta tomar una captura de las dos terminales una al lado de la otra y guardarla en `image/`, mostrando el nombre recibido por el servidor y la respuesta que recibió el cliente. Se pueden capturar las ventanas después de que terminen los dos programas, conservando sus salidas visibles.

## Comprobaciones realizadas

Los dos programas compilaron con Clang sin advertencias. Además de eso, se probaron las siguientes situaciones:

| Prueba | Resultado |
| --- | --- |
| Ejemplos originales 04 en localhost | El servidor envió la frase inicial y el cliente la recibió. |
| Programas modificados en el puerto 4898 | El servidor recibió el nombre y el cliente recibió la respuesta; ambos terminaron correctamente. |
| Programas modificados en el puerto 4900 | La comunicación también terminó correctamente, utilizando el nuevo puerto recibido como argumento. |
| Puerto 0 y puerto 65536 | El servidor rechazó los valores antes de crear la conexión. |
| IPv4 incorrecta | El cliente mostró el error y terminó. |
| Cliente ejecutado sin argumentos | Se mostró la forma correcta de ejecutarlo. |

## Estado del entregable

Los dos códigos fuente y la prueba local están listos. La captura local todavía está pendiente. Por indicación del alumno, no se ha realizado la conexión al servidor del profesor ni se ha enviado correo. La captura de la prueba remota con la salida de `date` queda para esa etapa posterior.
