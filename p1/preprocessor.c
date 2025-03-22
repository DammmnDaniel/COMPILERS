#include "preprocessor.h"
//#include "remove_comments.h"
#include "directives.h"

/*int process_comments(const char *input_file, const char *output_file) {
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "w");
    if (!input || !output) return 0;

    int result = remove_comments(input, output);

    fclose(input);
    fclose(output);
    return result;
}

int process_directives(const char *input_file, const char *output_file) {
    FILE *input = fopen(input_file, "r");
    FILE *output = fopen(output_file, "w");
    if (!input || !output) return 0;

    DefineTable *define_table = NULL;
    int result = handle_directives(input, output, &define_table);

    fclose(input);
    fclose(output);
    return result;
}

void log_error(const char *message, const char *filename, int line_number) {
    fprintf(stderr, "Error in file %s at line %d: %s\n", filename, line_number, message);
}*/
