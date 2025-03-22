#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include "preprocessor.h"

typedef struct {
    int in_backslash;
    char line[BUFFER_SIZE];
} backslash;

void process_include(const char *line,  FILE *input ,FILE *output, PreprocessorOptions *po, DefineTable **define_table);
void process_define(char *line, DefineTable **define_table, FILE *input);
void process_ifdef(const char *line, FILE *input, FILE *output, DefineTable *define_table);
void substitute_defines(char *line, FILE* input, FILE*output,DefineTable *define_table);
void substitute_parametrized_macros(char *line, DefineTable *define_table);
void process_backslash(char *line, backslash *bs);

#endif