# Tarea 5: sniffer de red en Linux

Para el caso de esta tarea se realizaron dos pruebas en Linux, utilizando el sniffer y los pares cliente-servidor 04 y 05 del tema 3. Los tres procesos se ejecutaron en el mismo equipo, con la dirección 127.0.0.1 y el puerto 4898.

El documento principal es `main.tex`, con su portada en `Portada.tex`. Incluye la preparación de la interfaz, las dos pruebas, sus resultados y las tres capturas originales:

- `img/snifferpreparacion.png`: interfaz `lo` con la bandera `PROMISC`.
- `img/programa04.png`: frase automática del servidor, 46 bytes recibidos y el mismo texto en el payload del sniffer.
- `img/programa05.png`: conversación entre cliente y servidor, cierre con `adios` y captura de `Hola` en el sniffer.

En `code/` se conservan el sniffer y las fuentes del par 05. Los ejemplos base del par 04 están en `LaboratorioVirtual/tema3/`. Los ejecutables de ambas pruebas se nombraron `servidor` y `cliente`; la compilación del par 05 sustituyó los ejecutables utilizados para la primera prueba.

El filtro del sniffer utiliza el puerto 4898. El reporte describe lo visible en las capturas, sin presentar resultados pendientes como si ya hubieran ocurrido. `PRUEBAS.md` conserva el registro de los pasos en Linux y la forma de repetir la prueba con el par 05.
