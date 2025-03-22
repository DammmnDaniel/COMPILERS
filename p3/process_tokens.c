#include "process_tokens.h"

#include <stdio.h>
#include <string.h>

#include "process_tokens.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> // Para isdigit()

void process_tokens(TokenLinkedList* token_list,srdfa* dfa, sdfaStack* stack, FILE* outFile) {
    printf("Procesando tokens...\n");
    int current_state = dfa->initial_state;
    TokenLinkedList* current_token = token_list;
    while(current_token != NULL){
        int symbol_index = current_token->token.type;
        if (symbol_index == -1) {
            printf("Error: Token no reconocido\n");
            continue;
        }
        int actual_state = dfa->transition_matrix[current_state][symbol_index];
        printf("DEBUG: Estado actual %d, regla de reducción: %s\n", current_state, 
            dfa->states[current_state].rule ? "Sí" : "No");
        
        // Solo hacemos shift si todavía hay tokens
        if (current_token != NULL) {
            shift(dfa, &current_token->token, &current_state, stack, outFile);
        }
        
        printf("can_reduce: %s\n", can_reduce(dfa, current_state) ? "Sí" : "No");

        if(can_reduce(dfa, current_state)){
            printf("REDUCE: Estado %d contiene una regla. Aplicando reducción...\n", current_state);
            reduce(dfa, stack, &current_state, outFile);
        }

        if (is_accepting(dfa, current_state)) {
            printf("Entrada aceptada en el estado %d\n", current_state);
        } else {
            printf("Entrada rechazada. Estado final: %d\n", current_state);
        }
        current_token = current_token->next;
    }
}


TokenLinkedList* create_tokens_from_file(char* token_file){
    FILE* tFile = fopen(token_file, "r");
    if (tFile == NULL) {
        printf("Error opening file\n");
        return NULL;
    }
    char ch;
    char lexeme[256];
    char type[256];
    int lexeme_index = 0;
    int type_index = 0;
    int in_lexeme = 0;
    int in_type = 0;
    int in_quotes = 0;

    TokenLinkedList* head = NULL;
    TokenLinkedList* current = NULL;
    while ((ch = fgetc(tFile)) != EOF) {
        if (ch == '<') {
            in_lexeme = 1;
            lexeme_index = 0;
        } else if (ch == ',' && !in_quotes) {
            in_lexeme = 0;
            in_type = 1;
            type_index = 0;
        } else if (ch == '>') {
            in_type = 0;
            // Create a new token and add it to the linked list
            TokenLinkedList* new_token = (TokenLinkedList*)malloc(sizeof(TokenLinkedList));
            if (new_token == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            lexeme[lexeme_index] = '\0';
            type[type_index] = '\0';
            strcpy(new_token->token.lexeme, lexeme);
            new_token->token.type = get_token_type(type);
            new_token->next = NULL;
            if (head == NULL) {
                head = new_token;
                current = new_token;
            } else {
                current->next = new_token;
                current = new_token;
            }
        } else if (in_lexeme) {
            lexeme[lexeme_index] = ch;
            lexeme_index++;
        } else if (in_type) {
            type[type_index] = ch;
            type_index++;

        }
    }
    fclose(tFile);
    return head;
}

bool can_reduce(srdfa* dfa, int current_state) {
    // Recorrer todas las posibles entradas (símbolos) en la tabla de transiciones
    for (int symbol = 0; symbol < MAX_INPUTS; symbol++) {
        int next_state_index = dfa->transition_matrix[current_state][symbol];

        // Si la transición es válida (no es -1)
        if (next_state_index > MAX_STATES){
            return true;
            }
        }

    // Si no se encontró ninguna reducción posible
    return false;
}

TokenType get_token_type(char* type_str) {
    if (strcmp(type_str, "CAT_NUMBER") == 0) return TOKEN_NUM;
    if (strcmp(type_str, "CAT_PLUS") == 0) return TOKEN_PLUS;
    if (strcmp(type_str, "CAT_TIMES") == 0) return TOKEN_TIMES;
    if (strcmp(type_str, "CAT_LPAREN") == 0) return TOKEN_LPAREN;
    if (strcmp(type_str, "CAT_RPAREN") == 0) return TOKEN_RPAREN;
    if (strcmp(type_str, "CAT_DOLLAR") == 0) return TOKEN_DOLLAR;
    if (strcmp(type_str, "CAT_T") == 0) return TOKEN_T;
    if (strcmp(type_str, "CAT_F") == 0) return TOKEN_F;
    if (strcmp(type_str, "CAT_E") == 0) return TOKEN_E;
    if (strcmp(type_str, "CAT_S") == 0) return TOKEN_S;
    return -1;
}

int get_token_index(srdfa* dfa, TokenType type) {
    for (int i = 0; i < MAX_TOKENS; i++) {
        if (dfa->tokens[i] == type) {
            return i;
        }
    }
    return -1;
}