#include "countOperations.h"

int totalCOMP = 0;
int totalIO = 0;
int totalGEN = 0;

countStruct* head = NULL;
int number = 0;

void acumulated_count(int line, const char* function_name, int valueCOMP, int valueIO, int valueGEN) {
    // Update the total counts
    totalCOMP = totalCOMP + valueCOMP;
    totalIO = valueIO + totalIO;
    totalGEN = totalGEN + valueGEN;

    // Allocate memory for a new countStruct
    countStruct* newStruct = (countStruct*)malloc(sizeof(countStruct));
    if (!newStruct) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Duplicate the function name string
    newStruct->functionName = my_strdup(function_name);
    if (!newStruct->functionName) {
        fprintf(stderr, "Memory allocation failed for functionName\n");
        free(newStruct);
        return;
    }

    // Set the values for the new countStruct
    newStruct->numberLine = line;
    newStruct->totCountComp = totalCOMP;
    newStruct->totCountIO = totalIO;
    newStruct->totCountGen = totalGEN;
    newStruct->incCountComp = valueCOMP;
    newStruct->incCountIO = valueIO;
    newStruct->incCountGen = valueGEN;
    newStruct->next = head;
    head = newStruct;
}

void write_dbg_file (FILE* dbg_file){
    if (!dbg_file) {
        fprintf(stderr, "Invalid file pointer\n");
        return;
    }
    countStruct* current = head;
    while (current) {
        // Write the debug information to the file
        fprintf(dbg_file, "\n%d.Function: %s\nLine: %d\nTotals - Comp: %d, IO: %d, Gen: %d\nIncrements - Comp: %d, IO: %d, Gen: %d\n", number, 
                current->functionName, current->numberLine, current->totCountComp, current->totCountIO, current->totCountGen,
                current->incCountComp, current->incCountIO, current->incCountGen);
        current = current->next;
        number++;
    }
}

char* my_strdup(const char* str) {
    // Calculate the length of the input string
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    
    // Allocate memory for the new string
    char* new_str = (char*)malloc(len + 1);
    if (new_str == NULL) {
        return NULL; 
    }

    // Copy the input string to the new string
    for (int i = 0; i <= len; i++) { 
        new_str[i] = str[i];
    }

    return new_str;
}
