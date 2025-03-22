#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

#define BUFFER_SIZE 1024  // Tamaño del buffer para leer líneas
#define IDENTIFIER_SIZE 256  // Tamaño máximo de un identificador
#define PARAMETER_SIZE 32  // Tamaño máximo de un parámetro
typedef struct {
    int comments; 
    int directives;
} PreprocessorOptions;

typedef struct DefineTable{
    char identifier[IDENTIFIER_SIZE];         // Nombre del identificador
    char value[BUFFER_SIZE];             // Valor asociado
    char parameters[IDENTIFIER_SIZE][PARAMETER_SIZE];     // Parámetros de macros parametrizadas
    int param_count;              // Número de parámetros
    struct DefineTable *next;     // Siguiente nodo en la lista
} DefineTable;

typedef struct {
    char filename[256];           // Nombre del archivo incluido
    struct IncludedFile *next;    // Siguiente nodo en la lista
} IncludedFile;

void process_line(char *line,  FILE *input , FILE *output, PreprocessorOptions *po, DefineTable **define_table);


#endif