# Tarea 5: sniffer de red

Para el caso de esta tarea se utilizan el sniffer y el par de programas 04 del tema 3. La intención es que el cliente reciba una frase del servidor y que, al mismo tiempo, el sniffer permita ver ese texto dentro de un segmento TCP.

- `code/sniffer.c`: copia del laboratorio con el filtro ajustado de 8080 a 4898, como pide la consigna. Conserva `AF_PACKET`, `SOCK_RAW`, el análisis Ethernet/IPv4/TCP y la impresión del payload. Se revisa la reserva de memoria y el tamaño de las cabeceras, se omiten fragmentos IPv4 y se calcula el payload con la longitud IPv4 para no incluir relleno Ethernet. Las cabeceras IP y TCP se copian a estructuras alineadas para equipos ARM. No reconstruye flujos TCP ni analiza VLAN, IPv6 o interfaces con formatos distintos de Ethernet/loopback Linux.
- `code/programa04_servidor_internet.c`: adaptación del ejemplo 04. Recibe el puerto como argumento y conserva la frase original. Usa `int main`, `socklen_t`, comprobación de errores, `SO_REUSEADDR`, envío completo e impresión de IP con `inet_ntop()` en lugar de depender de DNS inverso.
- `code/programa04_cliente_internet.c`: adaptación del ejemplo 04. Valida la IPv4 y el puerto, recibe hasta el cierre e imprime la frase. No solicita un nombre como el cliente del examen.
- `main.tex`, `Portada.tex` y `portada_img/`: documento con el formato de las tareas anteriores. Es un borrador hasta realizar la prueba e integrar la captura.
- `PRUEBAS.md`: comandos manuales, equipos compatibles, resultados esperados y solución de problemas.
- `Consigna_Tarea_5.pdf`: copia de la consigna proporcionada.

Los originales de `LaboratorioVirtual` permanecen sin modificar. No hay scripts de automatización. Los ejecutables se generan en Linux y están excluidos de Git.

La opción recomendada es **Ubuntu en UTM, tres terminales y 127.0.0.1**. El sniffer de esta práctica no compila directamente en macOS porque utiliza la interfaz de paquetes de Linux. Ver `PRUEBAS.md` antes de ejecutarlo.
