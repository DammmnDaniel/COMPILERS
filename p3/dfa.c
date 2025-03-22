#include "dfa.h"
#include <stdlib.h>



void initDFA(srdfa* dfa) {
    
    for (int i = 0; i < MAX_TOKENS; i++) {
        dfa->tokens[i] = tokens[i];
    }
    dfa->initial_state = initial_state;

    // Copiar el conjunto de estados
    for (int i = 0; i < MAX_STATES; i++) {
        dfa->states[i] = nodes[i];
    }
    

    // Copiar los estados de aceptación
    int num_accepting_states = sizeof(accepting_states) / sizeof(accepting_states[0]);
    for (int i = 0; i < num_accepting_states; i++) {
        dfa->accepting_states[i] = accepting_states[i];
    }

    // Copiar la matriz de transición
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_INPUTS; j++) {
            dfa->transition_matrix[i][j] = transition_table[i][j];
        }
    }
    

}

void shift(srdfa* dfa, Token* token, int* current_state, sdfaStack* stack, FILE* outFile) {
    int symbol = get_token_index(dfa, token->type);
    int previous_state = *current_state;
    int next_state_index = dfa->transition_matrix[*current_state][symbol];
    int n_state = dfa->states[next_state_index].go_state; // Usamos "states" en lugar de "nodes"

    if(n_state == -1){
        fprintf(outFile, "ERROR: No hay transición para '%s' en el estado %d\n", token->lexeme, previous_state);
        return;
    }
    push(stack, token);
    *current_state = n_state;
    token->state = n_state;
    printf("SHIFT: Token '%s' (tipo %d) procesado. Estado anterior: %d, Nuevo estado: %d. Pila: \n", 
            token->lexeme, token->type, previous_state, n_state);
    print_stack(stack, outFile);
    fprintf(outFile, "\n");
}

void reduce(srdfa* dfa, sdfaStack* stack, int* current_state, FILE* outFile) {

    int rule_state = -1;

    for (int i = 0; i < MAX_INPUTS; i++) {
        int next_state_index = dfa->transition_matrix[*current_state][i];
        if (next_state_index != -1) {
            if (next_state_index > MAX_STATES){
                rule_state = next_state_index;
            }
        }
    }

    if(rule_state == -1){
        fprintf(outFile, "ERROR: No hay regla para reducir en el estado %d\n", *current_state);
        return;
    }
    
    // Obtenemos la regla del estado actual
    rule* r = dfa->states[rule_state].rule;
    if(r == NULL || r->num_symbols <= 0){
        fprintf(outFile, "ERROR: No hay regla para reducir en el estado %d\n", *current_state);
        return;
    }
    int previous_state = *current_state;
    int num_pops = r->num_symbols;
    for (int i = 0; i < num_pops; i++) {
        pop(stack);
    }
    
    // Crear un nuevo token usando el Token almacenado en left_side de la regla
    Token* newToken = (Token*)malloc(sizeof(Token));
    if(newToken == NULL) {
        fprintf(outFile, "Error: No se pudo asignar memoria para el token de reducción\n");
        return;
    }
    strcpy(newToken->lexeme, r->left_side.lexeme);
    newToken->type = r->left_side.type;
    push(stack, newToken);
    
    int symbol = get_token_index(dfa, r->left_side.type);
    int state_after_reduce = (stack->posit_of_last_token >= 0) ? 
                                stack->tokens[stack->posit_of_last_token]->state : dfa->initial_state;
    *current_state = dfa->transition_matrix[state_after_reduce][symbol];
    
    fprintf(outFile, "REDUCE: Se aplicó la producción %s -> (", r->left_side.lexeme);
    for (int i = 0; i < r->num_symbols; i++) {
        fprintf(outFile, "%d ", r->right_side[i]);
    }
    fprintf(outFile, "). Se hicieron %d pops. Estado anterior: %d, Nuevo estado: %d. Pila: ", 
            num_pops, previous_state, *current_state);
    print_stack(stack, outFile);
    fprintf(outFile, "\n");
}

bool is_accepting(srdfa* dfa, int state) {
    for (int i = 0; i < (sizeof(accepting_states) / sizeof(accepting_states[0])); i++) {
        if(state == dfa->accepting_states[i]){
            return true;
        }
    }
    return false;
}

void reset_dfa(srdfa* dfa){
    dfa->initial_state = 0;
}

int count_symbols(const char* str) {
    int count = 0;

    while (*str) {
        if (*str != ' ') {
            count++;
        }
        str++;
    }
    return count;
}

void print_stack(sdfaStack* stack, FILE* outFile) {
    printf("[");
    for (int i = 0; i <= stack->posit_of_last_token; i++) {
        printf("%s", stack->tokens[i]->lexeme);
        if (i < stack->posit_of_last_token)
            printf(", ");
    }
    printf("]\n");
}