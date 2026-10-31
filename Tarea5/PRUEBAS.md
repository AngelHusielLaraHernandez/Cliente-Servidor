# Tarea 5: prueba del sniffer

La opción recomendada es Ubuntu dentro de UTM en tu Mac, con tres terminales normales de la misma VM. No necesitas otra computadora ni conectarte al servidor del profesor. Todos los comandos se ejecutan uno por uno, sin scripts.

## Equipos donde se puede realizar

| Equipo o distribución | ¿Sirve para toda la práctica? | Consideración |
| --- | --- | --- |
| Tu Mac con Ubuntu en UTM | Sí, recomendado | Ejecutar los tres programas dentro de Ubuntu. |
| PC con Linux nativo | Sí | Usar tres terminales y privilegios sudo para el sniffer. |
| PC Windows con Ubuntu en VirtualBox o VMware | Sí | Ejecutar los tres programas dentro de la misma VM Linux. |
| macOS directamente | No con este sniffer | Cliente y servidor sí funcionan; `AF_PACKET` y `linux/if_packet.h` son de Linux. |
| Windows directamente | No con estos códigos | Utilizar una VM Linux. |
| WSL2 o contenedor Linux | Depende del entorno | Requieren acceso a raw sockets y permisos para cambiar la interfaz; no es la primera opción. |

## Preparación

Copiar la carpeta `Tarea5` a la VM. La ruta usada abajo es la de tu repositorio anterior en Ubuntu; si cambió, ajustarla en las tres terminales. No copiar los ejecutables de macOS: compilar los `.c` dentro de Linux.

Si Ubuntu no tiene compilador, `ip` o `ifconfig`, instalar las herramientas (solo una vez):

```bash
sudo apt update
sudo apt install build-essential iproute2 net-tools
```

Estos son comandos independientes; ejecutar cada uno por separado.

## Terminal 1: sniffer

```bash
cd ~/Documentos/Cliente-Servidor/Tarea5/code
```

Registrar primero el estado de `lo`. Si ya muestra `PROMISC`, conservar ese estado al finalizar.

```bash
ip link show lo
```

```bash
sudo ip link set lo promisc on
```

```bash
ifconfig lo
```

Debe aparecer `PROMISC`. Guardar esta vista como `Tarea5/image/configuracion.png` si se desea incluir la evidencia de configuración en el reporte.

```bash
gcc -Wall -Wextra sniffer.c -o sniffer
```

```bash
sudo ./sniffer
```

Dejarlo abierto. Debe indicar que espera tráfico TCP del puerto **4898**. El programa no activa el modo promiscuo por sí solo.

## Terminal 2: servidor

```bash
cd ~/Documentos/Cliente-Servidor/Tarea5/code
```

```bash
gcc -Wall -Wextra programa04_servidor_internet.c -o programa04_servidor_internet
```

```bash
./programa04_servidor_internet 4898
```

Dejarlo esperando al cliente. Solo atiende una conexión y termina; para repetir la prueba hay que volver a levantarlo.

## Terminal 3: cliente

```bash
cd ~/Documentos/Cliente-Servidor/Tarea5/code
```

```bash
gcc -Wall -Wextra programa04_cliente_internet.c -o programa04_cliente_internet
```

```bash
./programa04_cliente_internet 127.0.0.1 4898
```

Al presionar Enter se conecta. No se escribe un nombre ni un mensaje adicional: este par 04 conserva el envío automático de la frase del servidor, distinto del examen parcial.

## Qué debe verse y qué capturar

- Servidor: cliente conectado desde `127.0.0.1`, frase enviada y finalización.
- Cliente: `El mensaje del servidor es: Conexion exitosa!` y bytes recibidos.
- Sniffer: MAC, IP de origen, puertos y el mismo texto en el payload; en la dirección servidor a cliente, el puerto de origen es 4898.

El cliente y servidor ya terminaron cuando se toma la captura; conservar abiertas las ventanas con sus salidas. Acomodar **las tres terminales juntas** y guardar una captura legible como `Tarea5/image/prueba_tres_terminales.png`. Esa es la evidencia solicitada por el profesor. El reporte integra automáticamente esos dos nombres de imagen cuando existan.

En loopback las MAC pueden aparecer como ceros y puede repetirse el mismo mensaje en la captura. No representa otra computadora ni demuestra que la aplicación lo haya enviado dos veces. El texto también podría dividirse entre segmentos TCP; revisar los payloads de esa conexión.

## Finalización

En la terminal 1 presionar Ctrl+C para detener el sniffer. Si al principio `lo` no estaba en modo promiscuo, restaurarlo:

```bash
sudo ip link set lo promisc off
```

```bash
ip link show lo
```

Si originalmente estaba en modo promiscuo, conservarlo así.

## Si no aparece el mensaje

1. Iniciar primero el sniffer, después el servidor y al final el cliente.
2. Revisar que los tres procesos estén en el mismo Ubuntu y que el puerto sea 4898.
3. Usar `127.0.0.1` solo cuando servidor y cliente están en la misma máquina.
4. Si aparece `Operation not permitted`, ejecutar el sniffer con sudo. En contenedores, sudo puede no ser suficiente por las capacidades del contenedor.
5. Si el cliente indica `Connection refused`, confirmar que el servidor siga esperando. Si el servidor muestra `Address already in use`, revisar `ss -ltnp 'sport = :4898'` e identificar el proceso antes de cerrarlo.

## Si se utilizan varios equipos

No es necesario para entregar esta tarea. La opción sencilla es ejecutar **servidor y sniffer en el mismo Linux**, y el cliente en otro equipo con comunicación por la misma red. Activar el modo promiscuo en la interfaz Linux utilizada, identificándola con `ip -br addr`; el cliente apunta a la IPv4 del servidor. Si el cliente utiliza macOS, compilar su `.c` con `clang`.

Para un sniffer en una **tercera máquina**, el modo promiscuo por sí solo no garantiza recibir el tráfico entre cliente y servidor: el switch o hipervisor debe entregarle una copia, mediante un puerto espejo o una red virtual que permita ese tráfico. Estar en el mismo Wi-Fi o en VMs con NAT separadas no basta. Por eso se recomienda la prueba con los tres programas dentro de una sola VM y loopback.

## Alcance de las comprobaciones realizadas

Cliente y servidor se compilaron en la Mac con `-Wall -Wextra -Wpedantic -Werror` y se ejecutaron en localhost: se recibieron los 46 bytes de la frase y ambos terminaron con estado 0. Esto comprueba el intercambio TCP, **no** la captura del sniffer.

Quedan pendientes la compilación del sniffer en Linux, la activación del modo promiscuo y la captura real de las tres terminales. No se incluyeron resultados ni capturas inventadas.
