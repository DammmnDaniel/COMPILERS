#include "dfa.h"
#include "countOperations.h"
#include <stdio.h>
#include <stdlib.h>

#define COUNTOUT 0 // 1 - to output file, 0 - to data file

int main(int argc, char* argv[]) {

    // Check if the correct number of arguments is provided
    if (argc != 2) {
        return call_error(NOFILEFOUND);
    }

    // Initialize DFA
    DFA *dfa = malloc(sizeof(DFA));
    if (!dfa) {
        fprintf(stderr, "Error: No se pudo asignar memoria para DFA\n");
        return 1;
    }

    // Initialize the transition table for the DFA
    init_transition_table(dfa);

    // Open the input file
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        return call_error(FILENOTWORK);
    }

    // Prepare the base and suffix for the output filename
    char *base = argv[1];
    char *suffix = "scn";

    // Calculate the lengths of the base and suffix
    int len = 0;
    while (base[len] != '\0') len++;
    int suffix_len = 0;
    while (suffix[suffix_len] != '\0') suffix_len++;

    // Create the output filename
    char output_filename[len + suffix_len + 1];

    // Copy the base to the output filename
    for (int i = 0; i < len; i++) {
        output_filename[i] = base[i];
    }

    // Append the suffix to the output filename
    for (int i = 0; i < suffix_len; i++) {
        output_filename[len + i] = suffix[i];
    }

    // Null-terminate the output filename
    output_filename[len + suffix_len] = '\0';

    // Open the output file
    FILE* output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        return call_error(FILENOTWORK);
    }

    #ifdef DEBUG
    // Prepare the debug filename
    suffix = "dbgcnt";

    while(suffix[suffix_len] != '\0') suffix_len++;

    char debug_filename[len + suffix_len + 1];

    // Copy the base to the debug filename
    for (int i = 0; i < len; i++) {
        debug_filename[i] = base[i];
    }

    // Append the suffix to the debug filename
    for (int i = 0; i < suffix_len; i++) {
        debug_filename[len + i] = suffix[i];
    }

    // Null-terminate the debug filename
    debug_filename[len + suffix_len] = '\0';

    // Open the debug file
    FILE* countFile = fopen(debug_filename, "w");
    if (output_file == NULL) {
        return call_error(FILENOTWORK);
    }
    #endif

    // Allocate memory for the token list
    TokenList *tokenlist = malloc(sizeof(TokenList));
    if (!tokenlist) {
        fprintf(stderr, "Error: No se pudo asignar memoria para TokenList\n");
        return 1;
    }

    // Perform lexical analysis
    lexical_analyzer(file, output_file, tokenlist, dfa);

    #ifdef DEBUG
    // Initialize line number for debug output
    int line = 1;

    // Print the first line number
    fprintf(output_file, "%d ", tokenlist->tokens[0].line_number);

    // Print tokens with their line numbers
    for (int i = 0; i < tokenlist->size; i++) {
        if (tokenlist->tokens[i].line_number != line) {
            fprintf(output_file, "\n");
            line = tokenlist->tokens[i].line_number;
            fprintf(output_file, "%d ", tokenlist->tokens[i].line_number);
        }
        fprintf(output_file, "<%s, %s> ", tokenlist->tokens[i].lexeme, tokenlist->tokens[i].category);
    }

    // Print a newline at the end
    fprintf(output_file, "\n");

    // Write debug information to the appropriate file
    if (COUNTOUT == 0) {
        write_dbg_file(countFile);
    } else {
        write_dbg_file(output_file);
    }
    #elif defined(RELEASE)
    // Print tokens without line numbers for release mode
    for (int i = 0; i < tokenlist->size; i++) {
        fprintf(output_file, "<%s, %s> ", tokenlist->tokens[i].lexeme, tokenlist->tokens[i].category);
    }
    #endif

    return 0;
}