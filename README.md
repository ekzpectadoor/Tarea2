# Base de Datos de Canciones

## Descripción  
Aplicación en C para gestionar una colección de canciones desde archivos CSV. Permite buscar por género, artista y tempo (BPM), organizando los datos en mapas y listas para un acceso eficiente.

---

## Cómo compilar y ejecutar

### Requisitos previos:
- Compilador **gcc**.
- Sistema operativo Linux/Windows con terminal.
- Visual studio code
### Pasos:  
1. **Descargar archivos**:  
   Asegúrate de tener todos los archivos fuente:  
   main.c, map.c, list.c, extra.c, y sus headers correspondientes.
   Asegurate que el dataset se haya descargado correctamente
   

2. **Compilar**:
   Abrir Visual Studio code
   Abrir el archivo
   Ejecutar en la terminal:
   gcc main.c map.c list.c extra.c -o music
   Si falla, entregale al terminal la direccion en donde estan estos archivos.

4. **Ejecutar**:
   Ejecutar en la terminal:
   ./music o .\music.exe

**Funcionalidades**
  Funcionando correctamente:
  1. Cargar canciones desde CSV: Lee archivos CSV con campos como artista, género, tempo, etc.
   
  2.Búsqueda por género: Muestra todas las canciones de un género específico.
  
  3.Búsqueda por artista: Lista canciones agrupadas por artista (soporta múltiples artistas por canción).

  4.Filtrar por tempo: Clasifica canciones en lentas (<80 BPM), moderadas (80-120 BPM), y rápidas (>120 BPM).

**Problemas conocidos**:
  1.Limpieza de pantalla:
    -Comportamiento: No limpia completamente la pantalla en algunas terminales.
    -Causa probable: Uso de system("clear") no compatible con todos los sistemas.

  2.Acentos:
    -Comportamiento: El programa debe mostrar un caracter con acento, muestra otra cosa
    -Causa probable: No se hizo un soporte para estos casos

**A mejorar**:
  -Buscar una alternativa a system("clear")
  -Implementar una interfaz más bonita
  -Tal vez sea bueno al usuario indicarle cuantos caracteres es el maximo de caracteres cuando ingresa datos 

**Ejemplo de uso**
  Paso 1: Cargar canciones
    Al iniciar, selecciona la opción 1 e ingresa la ruta del archivo CSV:
    Ingrese la ruta del archivo: music_dataset.csv
    Archivo abierto correctamente.
    Canciones cargadas con éxito.
    
  Paso 2: Buscar por genero
    Selecciona opcion 2 e ingresa un genero:
    Ingrese el genero de canción a buscar: Pop
    ========================================
    ID: 789 | Album: Chromatica | Canción: Rain On Me
    Artista: Lady Gaga | Tempo: 110.5
    ========================================

  Paso 3: Filtrar por tempo
    Selecciona opcion 4 y elige una categoría:
    1. Lentas (<80 BPM)
    2. Moderadas (80-120 BPM)
    3. Rapidas (>120 BPM)
    Ingrese su opcion: 2
    ========================================
    ID: 456 | Album: ÷ | Canción: Shape of You
    Artista: Ed Sheeran | Tempo: 96.0
    ========================================

  Paso 4: Salir
    Selecciona opcion 5 para salir:
    Saliendo del programa...
    
Nota: Las canciones se almacenan en memoria durante la sesión. Al salir, se liberan los recursos.
Nota2: Trabajo individual
