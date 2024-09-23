#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

// Estructura para pasar datos a los hilos
typedef struct {
    int id;
    int inicio;
    int fin;
    int total_primos;
} ThreadArgs;

// Función para verificar si un número es primo
bool es_primo(int num) {
    if (num <= 1) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

// Función que cuenta los números primos en un rango
void* contar_primos(void* arg) {
    ThreadArgs* args = (ThreadArgs*)arg;
    args->total_primos = 0;

    for (int i = args->inicio; i <= args->fin; i++) {
        if (es_primo(i)) {
            args->total_primos++;
        }
    }

    printf("Hilo pesado %d - Primos en el rango [%d, %d]: %d\n", args->id, args->inicio, args->fin, args->total_primos);
    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[3];
    ThreadArgs args[3];
    
    // Definir los intervalos
    int intervalos[][2] = {{1, 100}, {101, 200}, {201, 300}};

    // Crear hilos pesados para contar números primos en los intervalos
    for (int i = 0; i < 3; ++i) {
        args[i].id = i + 1;
        args[i].inicio = intervalos[i][0];
        args[i].fin = intervalos[i][1];
        pthread_create(&hilos[i], NULL, contar_primos, (void*)&args[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < 3; ++i) {
        pthread_join(hilos[i], NULL);
    }

    return 0;
}
