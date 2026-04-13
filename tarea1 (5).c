#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

typedef struct{
char nombre[50];
}categoria;

typedef struct{
char descripcion[100];
char categorias[50];
char tiempo[50];
}tarea;

void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("Sistema de Gestión de Tareas Pendientes ");
  puts("========================================");

  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  puts("4) Registrar Pendiente");
  puts("5) Atender Siguiente");
  puts("6) Visualización del Tablero General");
  puts("7) Filtrado por Categoría");
  puts("8) Salir");
}

void registrar_categorias(List *categorias) {
  categoria *nuevo = (categoria*)malloc(sizeof(categoria));
  printf("Registrar nueva categoría\n");

  getchar();
  
  fgets(nuevo->nombre,50,stdin);
  nuevo->nombre[strcspn(nuevo->nombre,"\n")] = '\0';

  list_pushBack(categorias,nuevo); 
}

void mostrar_categorias(List *categorias) {
  categoria* aux  = list_first(categorias);
  if (aux == NULL) return;
  printf("Categorías:\n");
  while (aux != NULL){
    printf("%s\n",aux->nombre);
    aux = list_next(categorias);
  }
  
}

int main() {
  char opcion;
  List *categorias = list_create();
  List *tareas = list_create();
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); 
    switch (opcion) {
    case '1':
      registrar_categorias(categorias);
      break;
    case '2':{
      char cat[50];
      printf("Ingrese categoria para eliminar : ");
      getchar();
      fgets(cat,50,stdin);
      cat[strcspn(cat, "\n")] = '\0';
      categoria* aux = list_first(categorias);
      while (aux != NULL){
        if (strcmp(aux->nombre,cat) == 0){
          categoria* borrar = list_popCurrent(categorias);
          free(borrar);
          tarea* aux2 = list_first(tareas);
          while (aux2 != NULL){
            if (strcmp(aux2->categorias,cat) == 0){
              tarea* borrar2 = list_popCurrent(tareas);
              free(borrar2);
              aux2 = list_first(tareas);
              continue;
            }
            aux2 = list_next(tareas);
            }
          break;
          }
        aux = list_next(categorias);
        }
      break;
      }
    case '3':
      mostrar_categorias(categorias);
      break;
    case '4':{
      tarea* nuevo = (tarea*)malloc(sizeof(tarea));

      getchar();
      
      printf("Ingrese descripción : \n");
      fgets(nuevo->descripcion, 100, stdin);
      nuevo->descripcion[strcspn(nuevo->descripcion, "\n")] = '\0';

      printf("Ingrese una categoria valida : \n");
      fgets(nuevo->categorias, 50, stdin);
      nuevo->categorias[strcspn(nuevo->categorias, "\n")] = '\0';

      int esta = 0;
      categoria* aux3 = list_first(categorias);
      while(aux3 != NULL){
        if(strcmp(aux3->nombre,nuevo->categorias) == 0){
          esta = 1;
          break;
        }
        aux3 = list_next(categorias);
      }
      if (!esta){
        printf("Esa categoria no existe. Intente de nuevo\n");
        free(nuevo);
        break;
      }

      time_t t = time(NULL);
      t -= 14400;
      struct tm *tm_info = gmtime(&t);
      strftime(nuevo->tiempo, 50, "%H:%M:%S", tm_info);

      list_pushBack(tareas,nuevo);
      break;
    }
    case '5':{
      tarea* aux4 = list_first(tareas);
      if (aux4 == NULL){
        printf("¡Libre de pendientes!\n");
        break;
      }
      printf("Atendiendo : %s\n",aux4->descripcion);
      printf("Categoria : %s\n",aux4->categorias);
      printf("Registrada a las : %s\n",aux4->tiempo);

      list_popFront(tareas);
      free(aux4);
    }
      break;
    case '6':{
      printf("Tablero general\n");
      categoria* cataux = list_first(categorias);
      if (cataux == NULL){
        printf("No hay categorias\n");
        break;
      }
      while(cataux!= NULL){
        tarea* taraux = list_first(tareas);

        while(taraux != NULL){
          if (strcmp(taraux->categorias, cataux->nombre)== 0){
            printf("[%s] Descripcion : %s (%s)\n",taraux->categorias,taraux->descripcion,taraux->tiempo);
          }
          taraux = list_next(tareas);
        }
        cataux = list_next(categorias);
      }
      break;
      }
    case '7':{
      char nombreCat[50];
      printf("Ingrese categoria para filtrar: ");
      getchar();
      fgets(nombreCat,50,stdin);
      nombreCat[strcspn(nombreCat,"\n")] = '\0';

      printf("Tareas en %s \n",nombreCat);
      tarea* tAux = list_first(tareas);
      int cont = 0;

      while(tAux != NULL){
        if(strcmp(tAux->categorias,nombreCat) == 0){
          printf("Descripcion : %s (%s)\n",tAux->descripcion,tAux->tiempo);
          cont++;
        }
        tAux = list_next(tareas);
    }
      if (cont == 0)printf("No habian tareas en esa categoria\n");
      break;
    }
    case '8':
      puts("Saliendo del sistema de gestión de tareas...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Liberar recursos, si es necesario
  list_clean(categorias);

  return 0;
}
