#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

void convertir_mayusculas(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
}

int main() {
    int pipe1[2], pipe2[2]; // Dos pipes: uno para enviar y otro para recibir
    pid_t pid;
    char buffer[100];

    // Crear los dos pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Error al crear los pipes");
        return 1;
    }

    // Crear el proceso hijo
    pid = fork();

    if (pid < 0) {
        perror("Error al crear el proceso hijo");
        return 1;
    }

    if (pid == 0) {  // Proceso hijo
        close(pipe1[1]); // Cerrar el extremo de escritura del primer pipe
        close(pipe2[0]); // Cerrar el extremo de lectura del segundo pipe

        // Leer el mensaje del proceso padre
        read(pipe1[0], buffer, sizeof(buffer));
        close(pipe1[0]); // Cerrar el extremo de lectura después de leer

        printf("Hijo: Recibido del padre: %s\n", buffer);

        // Convertir el mensaje a mayúsculas
        convertir_mayusculas(buffer);

        // Enviar el mensaje convertido de vuelta al padre
        write(pipe2[1], buffer, strlen(buffer) + 1);
        close(pipe2[1]); // Cerrar el extremo de escritura después de enviar

    } else {  // Proceso padre
        close(pipe1[0]); // Cerrar el extremo de lectura del primer pipe
        close(pipe2[1]); // Cerrar el extremo de escritura del segundo pipe

        // Mensaje para enviar al hijo
        char mensaje[] = "Hola desde el proceso padre";

        // Enviar el mensaje al proceso hijo
        write(pipe1[1], mensaje, strlen(mensaje) + 1);
        close(pipe1[1]); // Cerrar el extremo de escritura después de enviar

        // Esperar a que el hijo termine
        wait(NULL);

        // Leer el mensaje modificado del hijo
        read(pipe2[0], buffer, sizeof(buffer));
        close(pipe2[0]); // Cerrar el extremo de lectura después de leer

        printf("Padre: Recibido del hijo en mayúsculas: %s\n", buffer);
    }

    return 0;
}
