#include "stack.h"



// Initialize the stack
void initStack(sdfaStack* stack) {
    stack->posit_of_last_token = -1;
}

// Push operation
void push(sdfaStack* stack, Token* token) {
    if (stack->posit_of_last_token >= MAX_STACK_SIZE - 1) {
        //printf("Stack overflow!\n");
        return;
    }
    stack->tokens[++stack->posit_of_last_token] = token;
}

// Pop operation
Token* pop(sdfaStack* stack) {
    if (stack->posit_of_last_token < 0) {
        printf("Stack underflow! Couldn't POP.\n");
        return NULL;
    }
    return stack->tokens[stack->posit_of_last_token--];
}