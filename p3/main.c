#include "dfa.h"
#include "stack.h"
#include "process_tokens.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Uso: %s <archivo_de_tokens>\n", argv[0]);
        return 1;
    }

    char* input_filename = argv[1];
    
    srdfa dfa;
    initDFA(&dfa);
    sdfaStack stack;
    initStack(&stack);
    
    char output_filename[256];
    strcat(output_filename, "_p3dbg.txt");
    
    FILE* pFile = fopen(output_filename, "w");
    if(pFile == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    TokenLinkedList* tokenFile = create_tokens_from_file(input_filename);
    process_tokens(tokenFile, &dfa, &stack, pFile);
    
    // tFile es una lista enlazada; no se cierra con fclose
    fclose(pFile);

    return 0;
}