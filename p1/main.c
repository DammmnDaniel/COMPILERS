#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "remove_comments.h"
#include "directives.h"
#include "preprocessor.h"

int seen_comment = 0;
Comment block_struct;

/*========================================================================================================*/
/*                                            HELP FUNCTIONS                                              */
/*========================================================================================================*/

void print_help() {
    printf("Usage: ./preprocessor <file_name> [OPTIONS]\n");
    printf("\nOptions:\n");
    printf("  -c       Eliminate all comments from the input file.\n");
    printf("  -d       Replace all preprocessor directives starting with '#'.\n");
    printf("  -all     Perform all processing tasks, including removing comments and replacing directives.\n");
    printf("  -help    Display this help message (man page).\n");
    printf("\nDescription:\n");
    printf("  This program processes a C source file by applying various preprocessor-related transformations.\n");
    printf("  You must specify the file to process as the first argument, followed by one or more options.\n");
    printf("\nExamples:\n");
    printf("  1. Remove comments from a file:\n");
    printf("     ./preprocessor file.c -c\n");
    printf("\n");
    printf("  2. Replace preprocessor directives in a file:\n");
    printf("     ./preprocessor file.c -d\n");
    printf("\n");
    printf("  3. Apply all transformations:\n");
    printf("     ./preprocessor file.c -all\n");
    printf("\n");
    printf("Note:\n");
    printf("  Ensure the input file exists and is readable before using this program.\n");
}

/*========================================================================================================*/
/*                                          VALIDATION FUNCTION                                                 */
/*========================================================================================================*/int MAX = 0;

#include <string.h>

int validate_line(char* line, char* token) {
    int in_double_quotes = 0;  
    int in_single_quotes = 0; 
    char* token_position = strstr(line, token); 

    while (token_position != NULL) {
        in_double_quotes = 0;
        in_single_quotes = 0;

        for (size_t i = 0; &line[i] <= token_position; i++) {
            if (line[i] == '"' && in_single_quotes == 0) {
                in_double_quotes = !in_double_quotes;
            }
            if (line[i] == '\'' && in_double_quotes == 0) {
                in_single_quotes = !in_single_quotes;
            }
        }

        if (!in_double_quotes && !in_single_quotes) {
            return 1; 
        }

        token_position = strstr(token_position + 1, token);
    }
    return 0;
}

/*========================================================================================================*/
/*                                          PROCESS LINE                                                  */
/*========================================================================================================*/
void process_line(char *line, FILE *input, FILE *output, PreprocessorOptions *po, DefineTable **define_table) {

    // Eliminar saltos de línea innecesarios
    line[strcspn(line, "\r\n")] = '\0';

    if(po->comments) {
        strcpy(line, remove_comments(line));
        if (strstr(line, "/*") || seen_comment) {
            if (validate_line(line, "/*") || seen_comment) {
                block_struct = remove_blocks(line);
                seen_comment = block_struct.in_comments;
                strcpy(line, block_struct.line);
            }
        }
    }

    if (po->directives) {
        if (strncmp(line, "#define", 7) == 0) {
            process_define(line, define_table, input);
            return;
        }
        else if (strncmp(line, "#include", 8) == 0) {
            process_include(line, input, output, po, define_table);
            return;
        }
        else if (strstr(line, "#ifdef")) {
            process_ifdef(line, input, output, *define_table);
            return;
        }
    }
    

    if (po->directives) {
        substitute_parametrized_macros(line, *define_table);
        substitute_defines(line,input, output, *define_table);
    }
    
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
        line[len - 1] = '\0';  // Eliminar el salto de línea existente
    }
    strcat(line, "\n");  // Agregar un único salto de línea

    fputs(line, output);
}



/*========================================================================================================*/
/*                                                MAIN                                                    */
/*========================================================================================================*/

int main(int argc, char* argv[]) {

    
    if (argc < 2) {
        print_help();
        return 1;
    }
    
    PreprocessorOptions po = {0, 0};
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            po.comments = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            po.directives = 1;
        } else if (strcmp(argv[i], "-all") == 0) {
            po.comments = 1;
            po.directives = 1;
        } else if (strcmp(argv[i], "-help") == 0) {
            print_help();
            return 0;
        }
    }

    FILE *input = fopen(argv[1], "r");
    if (!input) {
        printf("Unable to open input file\n");
        return 1;
    }

    FILE *output = fopen("output_pp.c", "w");
    if (!output) {
        printf("Unable to create output file\n");
        fclose(input);
        return 1;
    }
    DefineTable *define_table = NULL;
    char line[BUFFER_SIZE];
    while (fgets(line, BUFFER_SIZE, input)) {
        process_line(line, input ,output, &po, &define_table);
    }

    fclose(input);
    fclose(output);

    return 0;
}