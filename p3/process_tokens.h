#ifndef PROCESS_TOKENS_H
#define PROCESS_TOKENS_H

#include "common.h"
#include "dfa.h"
#include "stack.h"
#include <string.h>
#include <stdio.h>

// Funciones de procesamiento de tokens:
void process_tokens(TokenLinkedList* token_list, srdfa* dfa, sdfaStack* stack, FILE* outFile);
TokenLinkedList* create_tokens_from_file(char* token_file);
TokenType get_token_type(char* type_str);
bool can_reduce(srdfa* dfa, int current_state) ;
int get_token_index(struct srdfa* dfa, TokenType type);

#endif