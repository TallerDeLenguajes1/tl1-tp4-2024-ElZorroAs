#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Nodo Nodo;

typedef struct Tarea {
    int TareaID;
    char *Descripcion;
    int Duracion;
} Tarea;

struct Nodo {  
    Tarea T;
    Nodo *Siguiente;
};

// Prototipos de funciones
Nodo* crearTarea(int *id);
void agregarTarea(Nodo **lista, int *id);
void transferirTareas(Nodo **pendientes, Nodo **realizadas);
void imprimirTareas(Nodo* lista);
void eliminarTarea(Nodo **lista, Nodo *tarea);
void consultarTareas(Nodo *pendientes, Nodo *realizadas, int id, const char *palabraClave);

int main()
{
    Nodo *tareasPendientes = NULL;
    Nodo *tareasRealizadas = NULL;
    int idActual = 1000;
    char opcion;
    do
    {
        agregarTarea(&tareasPendientes, &idActual);
        printf("Desea ingresar una nueva tarea? (s/n): ");
        scanf(" %c", &opcion); // Ignora los espacios en blanco antes de leer un carácter
        getchar();
    } while (opcion == 's' || opcion == 'S');

    // Impresión de tareas pendientes
    printf("Tareas pendientes:\n");
    imprimirTareas(tareasPendientes);

    // Llamada a la función para transferir tareas
    transferirTareas(&tareasPendientes, &tareasRealizadas);

    // Consulta de tareas por ID o palabra clave
    int idConsulta;
    char palabraClave[100];
    printf("Ingrese el ID de la tarea a consultar (o 0 para buscar por palabra clave): ");
    scanf("%d", &idConsulta);
    if (idConsulta != 0) {
        consultarTareas(tareasPendientes, tareasRealizadas, idConsulta, NULL);
    } else {
        printf("Ingrese la palabra clave para buscar tareas: ");
        scanf("%s", palabraClave);
        consultarTareas(tareasPendientes, tareasRealizadas, 0, palabraClave);
    }

    return 0;
}

Nodo* crearTarea(int *id){
    Nodo *nuevo=malloc(sizeof(Nodo));
    nuevo->T.TareaID=(*id)++;
    char buff[100];
    printf("Ingrese la descripcion de la tarea. ");
    fgets(buff, 100, stdin);
    nuevo->T.Descripcion = malloc(strlen(buff) + 1);  // Allocate memory
    strcpy(nuevo->T.Descripcion,buff);
    printf("Duracion de la entrada: ");
    scanf("%d", &(nuevo->T.Duracion));
    getchar();
    nuevo->Siguiente=NULL;
    return nuevo;
}

void agregarTarea(Nodo **lista, int *id){
    Nodo *nuevaTarea=crearTarea(id);
    nuevaTarea->Siguiente= *lista;
    *lista=nuevaTarea;
}

void eliminarTarea(Nodo **lista, Nodo *tarea) {
    Nodo *actual = *lista;
    Nodo *anterior = NULL;

    while (actual != NULL && actual != tarea) {
        anterior = actual;
        actual = actual->Siguiente;
    }

    if (anterior == NULL) {
        *lista = actual->Siguiente;
    } else {
        anterior->Siguiente = actual->Siguiente;
    }
}

void imprimirTareas(Nodo* lista) {
    Nodo* temp = lista;
    while (temp != NULL) {
        printf("ID: %d, Descripcion: %s, Duracion: %d\n", temp->T.TareaID, temp->T.Descripcion, temp->T.Duracion);
        temp = temp->Siguiente;
    }
}

void transferirTareas(Nodo **pendientes, Nodo **realizadas) {
    printf("Tareas pendientes:\n");
    imprimirTareas(*pendientes);

    printf("Seleccione las tareas que desea transferir a la lista de tareas realizadas (ingrese '0' para finalizar):\n");
    int idTarea;
    while (true) {
        printf("Ingrese el ID de la tarea a transferir (o '0' para finalizar): ");
        scanf("%d", &idTarea);
        if (idTarea == 0) break;

        Nodo *temp = *pendientes;
        Nodo *anterior = NULL;
        while (temp != NULL && temp->T.TareaID != idTarea) {
            anterior = temp;
            temp = temp->Siguiente;
        }

        if (temp != NULL) {
            if (anterior == NULL) *pendientes = temp->Siguiente;
            else anterior->Siguiente = temp->Siguiente;

            temp->Siguiente = *realizadas;
            *realizadas = temp;
            printf("La tarea con ID %d ha sido transferida a la lista de tareas realizadas.\n", idTarea);
        } else {
            printf("No se encontró ninguna tarea con el ID %d en la lista de pendientes.\n", idTarea);
        }
    }
}

void consultarTareas(Nodo *pendientes, Nodo *realizadas, int id, const char *palabraClave) {
    Nodo *temp = pendientes;
    bool encontrada = false;

    while (temp != NULL) {
        if ((id != 0 && temp->T.TareaID == id) || (palabraClave != NULL && strstr(temp->T.Descripcion, palabraClave) != NULL)) {
            printf("Tarea encontrada - ID: %d, Descripcion: %s, Duracion: %d - (Pendiente)\n", temp->T.TareaID, temp->T.Descripcion, temp->T.Duracion);
            encontrada = true;
        }
        temp = temp->Siguiente;
    }

    temp = realizadas;
    while (temp != NULL) {
        if ((id != 0 && temp->T.TareaID == id) || (palabraClave != NULL && strstr(temp->T.Descripcion, palabraClave) != NULL)) {
            printf("Tarea encontrada - ID: %d, Descripcion: %s, Duracion: %d - (Realizada)\n", temp->T.TareaID, temp->T.Descripcion, temp->T.Duracion);
            encontrada = true;
        }
        temp = temp->Siguiente;
    }

    if (!encontrada) {
        printf("No se encontraron tareas con el ID o palabra clave especificada.\n");
    }
}