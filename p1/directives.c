#include "directives.h"
#include "preprocessor.h"  // Asegurar la declaración de process_line
#include <stdlib.h>  // Para malloc, free
#include <string.h>  // Para strcpy, strlen, strcat, strstr



void process_define(char *line, DefineTable **define_table, FILE *input) {

    DefineTable *new_entry = malloc(sizeof(DefineTable));
    if (!new_entry) {
        fprintf(stderr, "Error allocating memory for define.\n");
        return;
    }

    char identifier[256], params[1024] = "", value[1024] = "";  // Default value for identifier is "1"
    int param_count = 0;

    // Process lines continued with '\'
    while (line[strlen(line) - 1] == '\\') {
        line[strlen(line) - 1] = '\0';  // Eliminate the backslash
        char next_line[1024];

        if (fgets(next_line, sizeof(next_line), input)) {
            next_line[strcspn(next_line, "\r\n")] = '\0';  // Eliminar saltos de línea
            strcat(line, "\n");  // Mantener el salto de línea original
            strcat(line, next_line);
        } else {
            fprintf(stderr, "Error: Unexpected end of file after line continuation.\n");
            free(new_entry);
            return;
        }
    }

    // Detectar si es una macro parametrizada
    char *open_paren = strchr(line, '(');
    if (open_paren) {
        char *close_paren = strchr(open_paren + 1, ')');
        if (!close_paren) {
            fprintf(stderr, "Invalid macro syntax: %s\n", line);
            free(new_entry);
            return;
        }

        // Extraer identificador
        *open_paren = '\0';  // Terminar el identificador antes del paréntesis
        sscanf(line + 8, "%s", identifier);  // Saltar "#define "

        // Extraer parámetros
        *close_paren = '\0';
        strcpy(params, open_paren + 1);

        // Dividir los parámetros por comas
        char *param = strtok(params, ",");
        while (param && param_count < 10) {
            while (*param == ' ') param++;  // Eliminar espacios iniciales
            strcpy(new_entry->parameters[param_count++], param);
            param = strtok(NULL, ",");
        }

        // Extraer el cuerpo de la macro, si existe
        char *value_start = close_paren + 1;
        while (*value_start == ' ') value_start++;  // Ignorar espacios iniciales
        if (*value_start != '\0') {
            strcpy(value, value_start);
        }
    } else {
        // Macro simple
        if (sscanf(line, "#define %255s %1023[^\n]", identifier, value) < 2) {
            // If no value is provided, leave value empty
            sscanf(line, "#define %255s", identifier);
        }
    }

    // Verificar si la macro ya está definida
    DefineTable *current = *define_table;
    while (current) {
        if (strcmp(current->identifier, identifier) == 0) {
            fprintf(stderr, "Warning: Redefinition of macro %s\n", identifier);
            free(new_entry);
            return;
        }
        current = current->next;
    }

    // Guardar la macro en la tabla
    strcpy(new_entry->identifier, identifier);
    strcpy(new_entry->value, value);
    new_entry->param_count = param_count;
    new_entry->next = *define_table;
    *define_table = new_entry;

    // Debug message
    if (param_count > 0) {
        printf("Defined parametrized macro: %s(", identifier);
        for (int i = 0; i < param_count; i++) {
            printf("%s%s", new_entry->parameters[i], (i < param_count - 1) ? ", " : "");
        }
        printf(") -> %s\n", value);
    } else {
        printf("Defined simple macro: %s -> %s\n", identifier, value);
    }
}

void process_ifdef(const char *line, FILE *input, FILE *output, DefineTable *define_table) {
    char identifier[256] = {0};  // Store the extracted identifier
    const char *start = strstr(line, "#ifdef");
    if (!start) {
        fprintf(stderr, "Error: Line does not contain #ifdef.\n");
        return;
    }

    // Move past "#ifdef" and skip whitespace
    start += strlen("#ifdef");
    while (*start == ' ' || *start == '\t') {
        start++;
    }

    // Extract the identifier
    const char *end = start;
    while (*end != '\0' && *end != ' ' && *end != '\t' && *end != '\n') {
        end++;
    }

    if (end == start) {
        fprintf(stderr, "Error: No identifier found after #ifdef.\n");
        return;
    }

    strncpy(identifier, start, end - start);
    identifier[end - start] = '\0';  // Null-terminate the identifier

    printf("Processing #ifdef for identifier: '%s'\n", identifier);

    // Check if the identifier is defined
    DefineTable *current = define_table;
    int is_defined = 0;
    while (current) {
        if (strcmp(current->identifier, identifier) == 0) {
            is_defined = 1; // Found the identifier
            break;
        }
        current = current->next;
    }

    char block[BUFFER_SIZE];
    int nesting_level = 1;

    while (fgets(block, BUFFER_SIZE, input)) {
        // Handle nested #ifdef
        if (strstr(block, "#ifdef")) {
            nesting_level++;
        } else if (strstr(block, "#endif")) {
            nesting_level--;
            if (nesting_level == 0) {
                break; // Reached the matching #endif
            }
        }

        // If the identifier is not defined, skip the block
        if (!is_defined) {
            continue; // Do not process or output these lines
        }

        // If the identifier is defined and this is the first level, output the block
        if (is_defined && nesting_level == 1) {
            fputs(block, output);
        }
    }

    // Error if #ifdef was not properly closed
    if (nesting_level != 0) {
        fprintf(stderr, "Error: Unmatched #ifdef directive for '%s'.\n", identifier);
    }
}






void process_include(const char *line,  FILE *input ,  FILE *output, PreprocessorOptions *po, DefineTable **define_table) {
    char filename[256];
    if (sscanf(line, "#include \"%255[^\"]\"", filename) == 1) {
        FILE *included_file = fopen(filename, "r");
        if (!included_file) {
            fprintf(stderr, "Error opening include file: %s\n", filename);
            return;
        }

        char included_line[BUFFER_SIZE];
        while (fgets(included_line, BUFFER_SIZE, included_file)) {
            process_line(included_line, included_file,  output, po, define_table);
        }

        fclose(included_file);
    } else {
        fputs(line, output);
    }
}
void substitute_defines(char *line,FILE*input,FILE*output, DefineTable *define_table) {
    if (strstr(line, "#ifdef")) {
    // Skip substitution and simply write the #ifdef line as is
        fputs(line, output);
        return;
    }



    DefineTable *current;
    char temp_line[BUFFER_SIZE];
    char *src_ptr = line;
    char *dest_ptr = temp_line;
    int in_quotes = 0;

    while (*src_ptr != '\0') {
        if (*src_ptr == '"' && (src_ptr == line || *(src_ptr - 1) != '\\')) {
            in_quotes = !in_quotes;  // Alternar dentro/fuera de cadena de texto
        }

        current = define_table;
        int replaced = 0;

        if (!in_quotes) {
            while (current != NULL) {
                size_t id_len = strlen(current->identifier);

                if (strncmp(src_ptr, current->identifier, id_len) == 0) {
                    strcpy(dest_ptr, current->value);
                    dest_ptr += strlen(current->value);
                    src_ptr += id_len;
                    replaced = 1;
                    break;
                }
                current = current->next;
            }
        }

        if (!replaced) {
            *dest_ptr = *src_ptr;
            dest_ptr++;
            src_ptr++;
        }
    }

    *dest_ptr = '\0';  // Asegurar terminación de cadena

    strncpy(line, temp_line, BUFFER_SIZE - 1);
    line[BUFFER_SIZE - 1] = '\0';  // Evitar desbordamiento
}
void substitute_parametrized_macros(char *line, DefineTable *define_table) {
    DefineTable *current = define_table;

    while (current) {
        if (current->param_count > 0) { // Solo procesar macros parametrizadas
            char *macro_start = strstr(line, current->identifier);
            while (macro_start) {
                // Detectar y extraer argumentos
                char *open_paren = strchr(macro_start + strlen(current->identifier), '(');
                char *close_paren = strchr(open_paren + 1, ')');
                if (!open_paren || !close_paren) {
                    fprintf(stderr, "Invalid macro usage: %s\n", macro_start);
                    break;
                }

                // Extraer argumentos entre paréntesis
                char args[10][256]; // Hasta 10 parámetros de macro
                char arg_buffer[1024]; 
                strncpy(arg_buffer, open_paren + 1, close_paren - open_paren - 1);
                arg_buffer[close_paren - open_paren - 1] = '\0';

                char *arg = strtok(arg_buffer, ",");
                int arg_count = 0;
                while (arg && arg_count < current->param_count) {
                    while (*arg == ' ') arg++; // Eliminar espacios iniciales
                    strcpy(args[arg_count++], arg);
                    arg = strtok(NULL, ",");
                }

                if (arg_count != current->param_count) {
                    fprintf(stderr, "Incorrect number of arguments for macro %s\n", current->identifier);
                    break;
                }

                // Expandir el cuerpo de la macro
                char expanded_value[1024];
                strcpy(expanded_value, current->value);
                for (int i = 0; i < current->param_count; i++) {
                    char *param_pos = strstr(expanded_value, current->parameters[i]);
                    while (param_pos) {
                        char temp[1024];
                        strncpy(temp, expanded_value, param_pos - expanded_value);
                        temp[param_pos - expanded_value] = '\0';
                        strcat(temp, args[i]);
                        strcat(temp, param_pos + strlen(current->parameters[i]));
                        strcpy(expanded_value, temp);
                        param_pos = strstr(expanded_value, current->parameters[i]);
                    }
                }

                // Construir la línea procesada correctamente
                char temp_line[1024];
                strncpy(temp_line, line, macro_start - line);
                temp_line[macro_start - line] = '\0';
                strcat(temp_line, expanded_value);
                strcat(temp_line, close_paren + 1); // Resto de la línea después de la macro
                strcpy(line, temp_line);

                // Buscar la siguiente ocurrencia de la macro
                macro_start = strstr(line, current->identifier);
            }
        }
        current = current->next;
    }
}

void process_backslash(char *line, backslash *bs){

}