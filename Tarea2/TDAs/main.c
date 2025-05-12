#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  char *id;
  char *artist;
  char *album_name;
  char *track_name;
  char *track_genre;
  double tempo;
} Song;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Base de Datos de Canciones");
  puts("========================================");
  
  puts("1) Cargar Canciones");
  puts("2) Buscar por Genero");
  puts("3) Buscar por Artista");
  puts("4) Buscar por Tempo");
  puts("5) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

// Crea una copia en memoria de una cadena para asi evitar problemas relacionados
char* copiarCadena(const char *og)
{
  if(og == NULL) return NULL; //se verifica si esta
  size_t len = strlen(og)+1; //se considera el caracter de termino
  char* nuevo = malloc(len); //Asignacion de memoria
  strcpy(nuevo,og);
  return nuevo;
}

// Elimina espacios al inicio y final de una cadena modificando directamente la original
void cortar_espacios(char* cadena)
{
  if(cadena == NULL) return;
  //Se quitan espacios al inicio
  char *inicio = cadena;
  while (*inicio && isspace((unsigned char)*inicio) ) //se avanza mientras haya espacios
  {
    inicio++;
  }
  //Se desplaza la parte sin espacios al principio de la cadena
  memmove(cadena, inicio, strlen(inicio)+1); //se incluye el caracter '\0'

  //Se quitan espacios al final de la cadena
  char* final = cadena + strlen(cadena)-1; //antes del '\0'
  while (final >= cadena && isspace((unsigned char)*final)) //retrocede mientras haya espacios
  {
    final--;
  }
  //Se coloca el caracter '\0' despues del ultimo caracter valido, sino todo falla :p
  *(final + 1) = '\0';  
}
/**
 * Carga canciones desde CSV en mapas y listas:
 * - canciones_byartist: Mapa clave artista, valor lista de canciones
 * - canciones_bygenres: Mapa clave género, valor lista de canciones
 * - lentas/moderadas/rapidas: Listas según tempo (<80, 80-120, >120 BPM)
 */
void cargar_canciones(Map *canciones_byartist, Map *canciones_bygenres,List* lentas, List* moderadas, List* rapidas) {
  char ruta [300];
  printf("Ingrese la ruta del archivo: ");
  scanf("%s", ruta);
  printf("\n");
  FILE *archivo = fopen(ruta, "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo"); 
    return;
  }
  printf("Archivo abierto correctamente\n");

  char **campos;
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV
  printf("Encabezados leidos\n");
  int contador = 0;

  //se empieza a leer el cuerpo del archivo
  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    contador++;
    printf("Procesando linea %d\n", contador);
    Song *cancion = (Song *)malloc(sizeof(Song));
    if(cancion == NULL)
    {
      printf("Error: Asignacion de memoria fallida");
    }
  
    cancion->id = copiarCadena(campos[0]);        // Asigna ID
    cancion->album_name = copiarCadena(campos[3]);     // Asigna título
    cancion->track_name = copiarCadena(campos[4]); // Asigna el nombre de la canciom
    cancion->track_genre = copiarCadena(campos[20]); //Asigna el genero de la cancion
    cancion->tempo = atof(campos[18]); //Asigna tempo
    cancion->artist = copiarCadena(campos[2]); // Asigna artista

    //Se empieza a poblar el mapa que tiene de clave los generos
    MapPair * genrepair = map_search(canciones_bygenres, cancion->track_genre);
    //Si el genero ya existe en el mapa, los datos de la cancion se añaden al value el cual es una lista
    if(genrepair != NULL)
    {
      List* lista = genrepair->value;
      list_pushBack(lista, cancion);
    }
    // Si no existe, se crea una lista para el value y se agrega el mapa el genero
    else
    {
      size_t len = strlen(cancion->track_genre) +1;
      char *key_genero = malloc(len);
      strcpy(key_genero, cancion->track_genre);
      List* lista = list_create();
      list_pushBack(lista, cancion);
      map_insert(canciones_bygenres, key_genero, lista);
    }

    //Como en las canciones pueden participar más de un artista, se tienen que separar para guardar los datos correctamente
    //En el archivo estan separados por un ";", asi que split_string tomara ese caracter como separador
    List *artistas = split_string(cancion->artist, ";");
    char *nombre; //variable la cual tendra el nombre del artista con espacios cortados
    while ((nombre=list_popFront(artistas)) != NULL)
    {
      cortar_espacios(nombre); //Se cortan los espacios debido a que dan fallos.
      //Se empieza a poblar el mapa que tiene de clave a los artistas
      MapPair * artistpair = map_search(canciones_byartist, nombre);
      //Se usa la misma logica y codigo similar en comparacion al mapa de generos
      if(artistpair != NULL)
      {
        List* lista1 = artistpair->value;
        list_pushBack(lista1, cancion);
      }
      else
      {
        size_t len = strlen(nombre) +1;
        char *key_artista = malloc(len);
        strcpy(key_artista, nombre);
        List* lista1 = list_create();
        list_pushBack(lista1, cancion);
        map_insert(canciones_byartist, key_artista, lista1);
      }


    //Para el tempo se hace una lista para cada categoria y se añaden canciones segun los criterios establecidos
    }
    if(cancion->tempo < 80)
    {
      list_pushBack(lentas, cancion);
    }
    else if(cancion->tempo <= 120)
    {
      list_pushBack(moderadas, cancion);
    }
    else
    {
      list_pushBack(rapidas, cancion);
    }
  }
    
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  printf("\nCanciones cargadas con exito\n");
}

// Busca y muestra canciones por genero usando el mapa de generos
void buscar_por_genero(Map *canciones_bygenres) {
  char genero[100];

  // Solicita al usuario el genero de cancion
  printf("Ingrese el género de cancion a buscar: ");
  scanf("%s", genero);
  printf("\n");
  //Se empieza a buscar
  MapPair *pair = map_search(canciones_bygenres, genero);
  if (pair != NULL)
  {
    List* canciones = pair->value;
    Song *cancion = list_first(canciones);
    while (cancion != NULL) // Se recupero la lista y se imprimen las canciones  
    {
      printf("========================================\n");
      printf("ID: %s\nAlbum: %s\nCancion: %s\nArtista: %s\nGenero: %s\nTempo: %lf\n", cancion->id, cancion->album_name, cancion->track_name,
      cancion->artist, cancion->track_genre, cancion->tempo);
      printf("========================================\n");
      cancion = list_next(canciones);
    }
  }
  else
  {
    printf("Genero de cancion no encontrado");
  }
}

// Busca y muestra canciones por artista usando el mapa de artistas
// Misma logica y casi mismo codigo que esta en la funcion anterior, solo que con un detalle más
void buscar_por_artista(Map* canciones_byartists)
{
  char artista[100];
  printf("Ingrese el nombre del artista: ");  
  scanf(" %99[^\n]", artista); //Se lee hasta el maximo de caracteres posible o hasta salto de linea
  cortar_espacios(artista); //Se cortan espacios debido a que generan problemas
  MapPair * pair = map_search(canciones_byartists, artista);
  if (pair != NULL)
  {
    List* canciones = pair->value;
    Song *cancion = list_first(canciones);
    while (cancion != NULL) 
    {
      printf("========================================\n");
      printf("ID: %s\nAlbum: %s\nCancion: %s\nArtista: %s\nGenero: %s\nTempo: %lf\n", cancion->id, cancion->album_name, cancion->track_name,
      cancion->artist, cancion->track_genre, cancion->tempo);
      printf("========================================\n");
      cancion = list_next(canciones);
    }
  }
  else
  {
    printf("Artista no encontrado");
  }


}
// Se filtran las canciones segun el tempo, con ayuda de listas ya hechas
void buscar_por_tempo(List* lentas, List* moderadas, List* rapidas)
{
  //El usuario debe elegir entre tres opciones 
  printf("Opciones para filtrar las canciones\n");
  printf("1. Lentas (<80 BPM)\n");
  printf("2. Moderadas (80-120 BPM)\n");
  printf("3. Rapidas (>120 BPM)\n");
  printf("Ingrese su opcion\n");
  int opcion;
  scanf("%i", &opcion);
  List * lista = NULL; //lista que representara una de las 3 ya hechas
  switch (opcion) // La lista recien creada toma datos segun la opcion elegida
  {
  case 1:
    lista = lentas;
    break;
  case 2:
    lista = moderadas;
    break;
  case 3:
    lista = rapidas;
    break;
  default:
    printf("Opcion no valida\n");
    return;
  }
  //Se empieza a recorrer la lista imprimiendo los datos de cada cancion.
  Song* cancion = list_first(lista);
  while (cancion != NULL)
  {
    printf("========================================\n");
    printf("ID: %s\nAlbum: %s\nCancion: %s\nArtista: %s\nGenero: %s\nTempo: %lf\n", cancion->id, cancion->album_name, cancion->track_name,
    cancion->artist, cancion->track_genre, cancion->tempo);
    printf("========================================\n");
    cancion = list_next(lista);  
  }
  

}

int main() {
  int opcion;; // Variable para almacenar una opción ingresada por el usuario
  Map *canciones_byartist = map_create(is_equal_str); //mapa de artistas
  Map *canciones_bygenres = map_create(is_equal_str); // mapa de genero
  //listas las cuales almacenaran las canciones que esten dentro de un rango de tempo
  List *lentas =list_create();
  List *moderadas = list_create();
  List *rapidas = list_create();

  do {
    mostrarMenuPrincipal(); //menu principal
    printf("Ingrese su opcion: ");
    scanf("%i", &opcion);
    while (getchar() != '\n'); //para evitar errores...

    switch (opcion) {
    case 1:
      cargar_canciones(canciones_byartist, canciones_bygenres, lentas, moderadas, rapidas);
      break;
    case 2:
      buscar_por_genero(canciones_bygenres);
      break;
    case 3:
      buscar_por_artista(canciones_byartist);
      break;
    case 4:
      buscar_por_tempo(lentas, moderadas, rapidas);
      break;
    case 5:
      printf("Saliendo del programa...");
      exit(EXIT_SUCCESS);
    }
    presioneTeclaParaContinuar();

  } while (opcion != 5);

  //Se libera la memoria de las distintas estructuras usadas al salir
  map_clean(canciones_bygenres);
  map_clean(canciones_byartist);
  list_clean(lentas);
  list_clean(moderadas);
  list_clean(rapidas);


  return 0;
}
