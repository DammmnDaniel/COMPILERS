#ifndef STACK_H
#define STACK_H

#include "common.h"

#define MAX_STACK_SIZE 1000

struct sdfaStack {
    Token* tokens[MAX_STACK_SIZE];
    int posit_of_last_token;
};
typedef struct sdfaStack sdfaStack;

void initStack(sdfaStack* stack);
void push(sdfaStack* stack, Token* token);
Token* pop(sdfaStack* stack);

#endif