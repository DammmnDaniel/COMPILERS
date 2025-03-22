#include <stdio.h>

// Definición de macros simples
#define PI 3.14159
#define MAX 100

// Definición de macros parametrizadas
#define SQUARE(x) ((x) * (x))
#define PRODUCT(x, y) ((x) * (y))
#define x 5 y 3
#define DEBUG
// Bloques condicionales

int main() {    
    // Imprimir el valor de PI
    printf("Value of PI: %f\n", PI);

    // Imprimir el cuadrado de un número
    printf("Square of 5: %d\n", SQUARE(5)) ;

    // Imprimir el producto de dos números
    printf("Product of 3 and 4: %d\n", PRODUCT(3, 4)) ;

    // Bloque condicional
    #ifdef DEBUG
    printf("Debug mode is enabled.\n");
    #endif

    // Bloque de comentarios
    /*
    Esto es un comentario de bloque
    que no debería aparecer en el archivo de salida.
    */
    

    // Imprimir el valor máximo
    printf("Max value is: %d\n", MAX);

    return 0; // Comentario de una línea
}