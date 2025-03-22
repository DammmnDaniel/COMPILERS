#ifndef DFA_H
#define DFA_H

#include "common.h"
#include "stack.h"
#include <stdbool.h>


#define MAX_TOKEN_LENGTH 100
#define MAX_STATES 19
#define MAX_TOKENS 10
#define MAX_RULES 7
#define MAX_REDUCE_RULES 7
#define MAX_INPUTS 9

typedef struct {
    Token left_side;              // Token completo para el no terminal resultante
    TokenType right_side[8];      // Símbolos del lado derecho
    int num_symbols;              // Número de símbolos en right_side
    TokenType on[5];              // Lookahead (opcional)
    int num_on;                   // Número de símbolos en on
} rule;


typedef struct {
    rule* rule; //rule to reduce, can be null if there is no rule to reduce in that node
	int go_state; //state to go
} dfa_state;

struct srdfa{
    TokenType tokens[MAX_TOKENS]; // Lista de tokens (columnas)
    int *initial_state;             // Estado inicial
    int *accepting_states[MAX_STATES]; // Estados de aceptación
    int *transition_matrix[MAX_STATES][MAX_INPUTS]; // Tabla de transiciones
    dfa_state states[MAX_STATES];  // Arreglo de estados del DFA
};
typedef struct srdfa srdfa;

static rule rules[MAX_RULES] = {
    // Producción 1: S -> E
    { { "S", TOKEN_S, 0 }, { TOKEN_E }, 1, {0}, 0 },
    // Producción 2: E -> E + T
    { { "E", TOKEN_E, 0 }, { TOKEN_E, TOKEN_PLUS, TOKEN_T }, 3, {0}, 0 },
    // Producción 3: E -> T
    { { "E", TOKEN_E, 0 }, { TOKEN_T }, 1, {0}, 0 },
    // Producción 4: T -> T * F
    { { "T", TOKEN_T, 0 }, { TOKEN_T, TOKEN_TIMES, TOKEN_F }, 3, {0}, 0 },
    // Producción 5: T -> F
    { { "T", TOKEN_T, 0 }, { TOKEN_F }, 1, {0}, 0 },
    // Producción 6: F -> (E)
    { { "F", TOKEN_F, 0 }, { TOKEN_LPAREN, TOKEN_E, TOKEN_RPAREN }, 3, {0}, 0 },
    // Producción 7: F -> NUM
    { { "F", TOKEN_F, 0 }, { TOKEN_NUM }, 1, {0}, 0 }
};

static dfa_state nodes[] = {
    {NULL, 0},   // Estado 0
    {NULL, 1},   // Estado 1
    {NULL, 2},   // Estado 2
    {NULL, 3},   // Estado 3
    {NULL, 4},   // Estado 4
    {NULL, 5},   // Estado 5
    {NULL, 6},   // Estado 6
    {NULL, 7},   // Estado 7
    {NULL, 8},   // Estado 8
    {NULL, 9},   // Estado 9
    {NULL, 10},  // Estado 10
    {NULL, 11},  // Estado 11
    {NULL, 12},  // Estado 12
    {NULL, 13},  // Estado 13
    {NULL, 14},  // Estado 14
    {NULL, 15},  // Estado 15
    {NULL, 16},  // Estado 16
    {NULL, 17},  // Estado 17
    {NULL, 18},  // Estado 18
    {&rules[0], MAX_STATES},  // Estado 19
    {&rules[1], 0},           // Estado 20
    {&rules[1], 9},           // Estado 21
    {&rules[2], 0},           // Estado 22
    {&rules[2], 9},           // Estado 23
    {&rules[3], 0},           // Estado 24
    {&rules[3], 9},           // Estado 25
    {&rules[4], 0},           // Estado 26
    {&rules[4], 9},           // Estado 27
    {&rules[5], 0},           // Estado 28
    {&rules[6], 0},           // Estado 29
    {&rules[6], 9}            // Estado 30
};

//Definition of automata
static TokenType tokens[MAX_TOKENS] = {
    TOKEN_NUM,
    TOKEN_PLUS,
    TOKEN_TIMES,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_DOLLAR,
    TOKEN_T,
    TOKEN_F,
    TOKEN_E,
    TOKEN_S
};

static int transition_table[MAX_STATES][MAX_INPUTS]= { //each number makes refrerence to the index of the next state in the nodes array in the automata
    {1,-1,-1,9,-1,-1,3,2,6}, 
    {-1,29,29,-1,29,29,-1,-1,-1},
    {-1,26,26,-1,26,26,-1,-1,-1},
    {-1,22,4,-1,22,22,-1,-1,-1},
    {1,-1,-1,9,-1,-1,-1,5,-1},
    {-1,24,24,-1,24,24,-1,-1,-1},
    {-1,7,-1,-1,-1,19,-1,-1,-1},
    {1,-1,-1,9,-1,-1,8,2,-1},
    {-1,20,4,-1,20,20,-1,-1,-1},
    {14,-1,-1,9,-1,-1,15,13,10},
    {-1,12,-1,-1,11,-1,-1,-1,-1},
    {-1,28,28,-1,28,28,-1,-1,-1},
    {14,-1,-1,9,-1,-1,17,13,-1},
    {-1,27,27,-1,27,27,-1,-1,-1},
    {-1,30,30,-1,30,30,-1,-1,-1},
    {-1,23,16,-1,23,23,-1,-1,-1},
    {14,-1,-1,9,-1,-1,-1,18,-1},
    {-1,21,16,-1,21,21,-1,-1,-1},
    {-1,25,25,-1,25,25,-1,-1,-1}
      
 //-1 means that there is no transition for that input
};

static int set_of_states[MAX_STATES] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};

static int initial_state = 0;

static int accepting_states[] = {2};


void initDFA(srdfa* dfa);
void shift(srdfa* dfa, Token* token, int* current_state, sdfaStack* stack, FILE* outFile);
void reduce(srdfa* dfa, sdfaStack* stack, int* current_state, FILE* outFile);
void reset_dfa(srdfa* dfa);
bool is_accepting(srdfa* dfa, int state);
int count_symbols(const char* str);


#endif