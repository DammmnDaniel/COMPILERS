#ifndef COUNTOPERATIONS_H
#define COUNTOPERATIONS_H

#include <stdio.h>
#include <stdlib.h>

typedef struct countStruct {
    int totCountComp;
    int totCountIO;
    int totCountGen;
    int incCountComp;
    int incCountIO;
    int incCountGen;
    int numberLine;
    char* functionName;
    struct countStruct* next;
} countStruct;

typedef enum {
    COMP = 0,
    IO = 1,
    GEN = 2
} typecomp;

void acumulated_count(int line, const char* function_name, int valueCOMP, int valueIO, int valueGEN);
void write_dbg_file (FILE* dbg_file);
char* my_strdup(const char* str);

#endif