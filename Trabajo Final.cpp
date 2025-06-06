#include <iostream>
#include <string>
using namespace std;

// ==================== ESTRUCTURAS ====================

// Esta estructura guarda la información de cada proceso
struct Proceso {
    string id;          // Guarda el ID único del proceso (como un nombre corto para identificarlo)
    string nombre;      // Guarda el nombre del proceso (una palabra para saber qué hace)
    int prioridad;      // Guarda la prioridad del proceso (número, donde uno es más importante)
    Proceso* siguiente; // Apunta al siguiente proceso en la lista (para enlazar procesos en cadena)

    // Este es el constructor que crea un proceso nuevo con los datos que damos
    Proceso(string _id, string _nombre, int _prioridad) {
        id = _id;               // Guarda el ID que recibimos en el proceso
        nombre = _nombre;       // Guarda el nombre que recibimos en el proceso
        prioridad = _prioridad; // Guarda la prioridad que recibimos
        siguiente = NULL;       // Al inicio no apunta a ningún otro proceso (fin de la lista)
    }
};

// Nodo para la cola de procesos que se ejecutan en la CPU
struct NodoCola {
    string id;           // ID del proceso en la cola
    string nombre;       // Nombre del proceso en la cola
    int prioridad;       // Prioridad del proceso en la cola
    NodoCola* siguiente; // Apunta al siguiente proceso en la cola
};

// Nodo para la pila de memoria (simula la memoria usada por procesos)
struct NodoMemoria {
    string procesoID;    // Guarda el ID del proceso que usa la memoria
    NodoMemoria* siguiente; // Apunta a la siguiente memoria (pila enlazada)
};

// ==================== VARIABLES GLOBALES ====================
// Aquí guardamos las listas principales para manejar procesos y memoria

Proceso* listaProcesos = NULL;  // Lista enlazada con todos los procesos
NodoCola* colaCPU = NULL;       // Cola de procesos que esperan para ser ejecutados
NodoMemoria* pilaMemoria = NULL; // Pila para controlar la memoria asignada

// ==================== FUNCIONES ====================

// Busca un proceso por su ID en la lista de procesos
Proceso* buscarProcesoID(string id) {
    Proceso* actual = listaProcesos; // Empezamos desde el primer proceso
    while (actual) {                 // Recorremos todos los procesos uno por uno
        if (actual->id == id)       // Si encontramos el ID que buscamos
            return actual;          // Retornamos ese proceso
        actual = actual->siguiente; // Si no, seguimos al siguiente proceso
    }
    return NULL;                    // Si no lo encontramos, devolvemos NULL (nada)
}

// Pide datos al usuario y crea un nuevo proceso, luego lo añade al final de la lista
void insertarProceso() {
    string id, nombre;
    int prioridad;

    // Pedimos un ID que no exista para que sea único
    do {
        cout << "Ingrese ID del proceso: ";
        cin >> id;
        if (buscarProcesoID(id)) { // Si ya existe un proceso con ese ID
            cout << "Error: El ID ya existe. Ingrese un ID unico.\n";
        }
    } while (buscarProcesoID(id)); // Repetimos hasta que sea único

    cout << "Ingrese nombre del proceso: ";
    cin >> nombre; // Pedimos el nombre

    // Pedimos la prioridad, que debe ser un número entero no negativo
    do {
        cout << "Ingrese prioridad (entero no negativo): ";
        cin >> prioridad;
        if (prioridad < 0) {
            cout << "Error: La prioridad no puede ser negativa. Intente nuevamente.\n";
        }
    } while (prioridad < 0);

    // Creamos el nuevo proceso con los datos ingresados
    Proceso* nuevo = new Proceso(id, nombre, prioridad);

    // Si la lista está vacía, el nuevo proceso será el primero
    if (!listaProcesos) {
        listaProcesos = nuevo;
    } else {
        // Si no está vacía, recorremos hasta el final para añadir el nuevo proceso
        Proceso* actual = listaProcesos;
        while (actual->siguiente)
            actual = actual->siguiente;
        actual->siguiente = nuevo; // Enlazamos el nuevo proceso al final
    }
    cout << "Proceso insertado correctamente.\n";
    cout << "---------------------------------------------\n";
}

// Muestra todos los procesos que existen en la lista
void mostrarProcesos() {
    if (!listaProcesos) { // Si la lista está vacía
        cout << "No hay procesos registrados.\n";
    } else {
        Proceso* actual = listaProcesos; // Empezamos desde el primero
        cout << "Lista de procesos:\n";
        // Recorremos todos los procesos y mostramos su info
        while (actual) {
            cout << "ID: " << actual->id
                 << " | Nombre: " << actual->nombre
                 << " | Prioridad: " << actual->prioridad << "\n";
            actual = actual->siguiente; // Pasamos al siguiente
        }
    }
    cout << "---------------------------------------------\n";
}

// Borra un proceso de la lista usando su ID
void eliminarProceso() {
    string id;
    cout << "Ingrese el ID del proceso a eliminar: ";
    cin >> id;
    Proceso* actual = listaProcesos;
    Proceso* anterior = NULL; // Para guardar el proceso anterior y poder enlazar

    // Buscamos el proceso con el ID dado
    while (actual && actual->id != id) {
        anterior = actual;          // Guardamos el proceso actual como anterior
        actual = actual->siguiente; // Seguimos al siguiente proceso
    }

    if (!actual) { // Si no encontramos el proceso
        cout << "Proceso no encontrado.\n";
    } else {
        // Si el proceso está al inicio de la lista
        if (!anterior)
            listaProcesos = actual->siguiente; // La lista empieza desde el siguiente
        else
            anterior->siguiente = actual->siguiente; // Enlazamos anterior con siguiente

        delete actual; // Liberamos la memoria del proceso eliminado
        cout << "Proceso eliminado correctamente.\n";
    }
    cout << "---------------------------------------------\n";
}

// Busca un proceso por su nombre y muestra su información si lo encuentra
void buscarProcesoPorNombre() {
    string nombre;
    cout << "Ingrese el nombre del proceso a buscar: ";
    cin >> nombre;
    Proceso* actual = listaProcesos;
    bool encontrado = false;

    // Recorremos la lista buscando el nombre que ingresamos
    while (actual) {
        if (actual->nombre == nombre) {
            cout << "Proceso encontrado:\n";
            cout << "ID: " << actual->id << " | Nombre: " << actual->nombre << " | Prioridad: " << actual->prioridad << "\n";
            encontrado = true;
            break; // Salimos si lo encontramos
        }
        actual = actual->siguiente; // Pasamos al siguiente proceso
    }

    if (!encontrado) {
        cout << "Proceso no encontrado.\n";
    }
    cout << "---------------------------------------------\n";
}

// Cambia la prioridad de un proceso encontrado por su ID
void modificarPrioridad() {
    string id;
    cout << "Ingrese el ID del proceso para modificar prioridad: ";
    cin >> id;

    // Buscamos el proceso con ese ID
    Proceso* p = buscarProcesoID(id);
    if (!p) {
        cout << "Proceso no encontrado.\n";
    } else {
        int nuevaPrioridad;
        cout << "Prioridad actual: " << p->prioridad << "\n";

        // Pedimos una nueva prioridad que sea número positivo o cero
        do {
            cout << "Ingrese nueva prioridad (entero no negativo): ";
            cin >> nuevaPrioridad;
            if (nuevaPrioridad < 0) {
                cout << "Error: La prioridad no puede ser negativa. Intente nuevamente.\n";
            }
        } while (nuevaPrioridad < 0);

        // Actualizamos la prioridad del proceso
        p->prioridad = nuevaPrioridad;
        cout << "Prioridad modificada correctamente.\n";
    }
    cout << "---------------------------------------------\n";
}

// Ordena y mete los procesos en una cola para que se ejecuten según su prioridad (menor número = más importante)
void encolarCPU() {
    // Primero borramos todos los procesos que estén en la cola actual (para actualizar)
    while (colaCPU) {
        NodoCola* temp = colaCPU;
        colaCPU = colaCPU->siguiente;
        delete temp; // Liberamos memoria de cada nodo
    }

    Proceso* actual = listaProcesos;

    // Recorremos todos los procesos para agregarlos a la cola
    while (actual) {
        NodoCola* nuevo = new NodoCola; // Creamos un nodo nuevo para la cola
        nuevo->id = actual->id;         // Guardamos el ID
        nuevo->nombre = actual->nombre; // Guardamos el nombre
        nuevo->prioridad = actual->prioridad; // Guardamos la prioridad
        nuevo->siguiente = NULL;        // Por ahora no apunta a nadie

        // Si la cola está vacía o el nuevo proceso tiene prioridad más alta (menor número)
        if (!colaCPU || nuevo->prioridad < colaCPU->prioridad) {
            nuevo->siguiente = colaCPU; // Apuntamos al inicio actual
            colaCPU = nuevo;            // Nuevo proceso queda al frente
        } else {
            // Si no, buscamos dónde insertar para mantener orden por prioridad
            NodoCola* temp = colaCPU;
            while (temp->siguiente && temp->siguiente->prioridad <= nuevo->prioridad)
                temp = temp->siguiente;
            nuevo->siguiente = temp->siguiente; // Insertamos después del temp
            temp->siguiente = nuevo;
        }
        actual = actual->siguiente; // Pasamos al siguiente proceso
    }
    cout << "Procesos encolados para ejecucion segun prioridad.\n";
    cout << "---------------------------------------------\n";
}

// Ejecuta (saca) el proceso que está al frente de la cola
void ejecutarCPU() {
    if (!colaCPU) {
        cout << "No hay procesos en la cola de ejecucion.\n";
    } else {
        NodoCola* temp = colaCPU;      // Guardamos el primer proceso de la cola
        colaCPU = colaCPU->siguiente; // La cola ahora empieza desde el siguiente
        cout << "Ejecutando proceso:\n";
        cout << "ID: " << temp->id << " | Nombre: " << temp->nombre << " | Prioridad: " << temp->prioridad << "\n";
        delete temp; // Liberamos memoria del proceso ejecutado
    }
    cout << "---------------------------------------------\n";
}

// Muestra todos los procesos que están en la cola de ejecución
void mostrarColaCPU() {
    if (!colaCPU) {
        cout << "La cola de ejecucion esta vacia.\n";
    } else {
        cout << "Cola de ejecucion (ordenada por prioridad):\n";
        NodoCola* temp = colaCPU;
        while (temp) {
            cout << "ID: " << temp->id << " | Nombre: " << temp->nombre << " | Prioridad: " << temp->prioridad << "\n";
            temp = temp->siguiente;
        }
    }
    cout << "---------------------------------------------\n";
}

// Asigna memoria a un proceso, agregando su ID en la pila de memoria (push)
void pushMemoria() {
    string id;
    cout << "Ingrese el ID del proceso para asignar memoria: ";
    cin >> id;
    NodoMemoria* nuevo = new NodoMemoria; // Creamos un nodo para memoria
    nuevo->procesoID = id;                // Guardamos el ID del proceso
    nuevo->siguiente = pilaMemoria;       // El nuevo nodo apunta al tope actual
    pilaMemoria = nuevo;                   // El nuevo nodo ahora es el tope
    cout << "Memoria asignada correctamente al proceso.\n";
    cout << "---------------------------------------------\n";
}

// Libera la memoria del proceso que está en el tope de la pila (pop)
void popMemoria() {
    if (!pilaMemoria) {
        cout << "La memoria ya esta vacia.\n";
    } else {
        cout << "Liberando memoria del proceso ID: " << pilaMemoria->procesoID << "\n";
        NodoMemoria* temp = pilaMemoria;     // Guardamos el nodo actual
        pilaMemoria = pilaMemoria->siguiente; // El tope baja al siguiente nodo
        delete temp;                         // Liberamos memoria del nodo liberado
    }
    cout << "---------------------------------------------\n";
}

// Muestra cómo está la pila de memoria, desde el tope hasta el fondo
void mostrarMemoria() {
    if (!pilaMemoria) {
        cout << "La memoria esta vacia.\n";
    } else {
        NodoMemoria* temp = pilaMemoria; // Empezamos desde el tope
        int pos = 1;                    // Posición para mostrar la pila
        cout << "Estado actual de la memoria (tope -> fondo):\n";
        while (temp) {
            cout << "Posicion " << pos++ << ": " << temp->procesoID << "\n";
            temp = temp->siguiente; // Bajamos a la siguiente posición
        }
    }
    cout << "---------------------------------------------\n";
}

// ==================== MENU ====================
// Muestra las opciones para que el usuario elija qué hacer
void mostrarMenu() {
    cout << "+---------------------------------------------------------+\n";
    cout << "|           SISTEMA DE GESTION DE PROCESOS                |\n";
    cout << "+---------------------------------------------------------+\n";
    cout << "|  1. Insertar proceso                                    |\n";
    cout << "|  2. Eliminar proceso por ID                             |\n";
    cout << "|  3. Buscar proceso por ID                               |\n";
    cout << "|  4. Buscar proceso por nombre                           |\n";
    cout << "|  5. Modificar prioridad de un proceso                   |\n";
    cout << "|  6. Mostrar todos los procesos                          |\n";
    cout << "|  7. Encolar procesos para ejecucion                     |\n";
    cout << "|  8. Ejecutar proceso (desencolar)                       |\n";
    cout << "|  9. Mostrar cola de ejecucion                           |\n";
    cout << "| 10. Asignar memoria (push)                              |\n";
    cout << "| 11. Liberar memoria (pop)                               |\n";
    cout << "| 12. Ver estado actual de memoria                        |\n";
    cout << "| 13. Salir                                               |\n";
    cout << "+---------------------------------------------------------+\n";
    cout << "Seleccione una opcion (1-13): ";
}

// ==================== MAIN ====================
int main() {
    int opcion;
    do {
        mostrarMenu(); // Mostrar opciones
        cin >> opcion; // Leer opción del usuario
        cout << "\n";
        switch (opcion) {
            case 1: insertarProceso(); break;        // Insertar proceso nuevo
            case 2: eliminarProceso(); break;        // Eliminar proceso por ID
            case 3: {                                // Buscar proceso por ID y mostrarlo
                string id;
                cout << "Ingrese el ID del proceso a buscar: ";
                cin >> id;
                Proceso* p = buscarProcesoID(id);
                if (p) {
                    cout << "Proceso encontrado:\n";
                    cout << "ID: " << p->id << " | Nombre: " << p->nombre << " | Prioridad: " << p->prioridad << "\n";
                } else {
                    cout << "Proceso no encontrado.\n";
                }
                cout << "---------------------------------------------\n";
                break;
            }
            case 4: buscarProcesoPorNombre(); break;  // Buscar por nombre
            case 5: modificarPrioridad(); break;       // Cambiar prioridad
            case 6: mostrarProcesos(); break;           // Mostrar todos los procesos
            case 7: encolarCPU(); break;                  // Poner en cola por prioridad
            case 8: ejecutarCPU(); break;                 // Ejecutar proceso (sacar cola)
            case 9: mostrarColaCPU(); break;              // Mostrar cola actual
            case 10: pushMemoria(); break;                // Asignar memoria
            case 11: popMemoria(); break;                  // Liberar memoria
            case 12: mostrarMemoria(); break;              // Mostrar memoria
            case 13: cout << "Saliendo del programa...\n"; break; // Salir
            default: cout << "Opcion invalida. Intente de nuevo.\n";
        }
    } while (opcion != 13);

    return 0;
}

