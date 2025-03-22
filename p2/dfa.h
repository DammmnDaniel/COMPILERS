#ifndef DFA_H
#define DFA_H

// ==========================
//       IMPORTS & CONFIG
// ==========================
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "err.h"
#include "countOperations.h"

// ==========================
//       CONFIGURACIONES
// ==========================

#define MAX_STATES 34
#define MAX_INPUTS 255
#define MAX_TOKEN_LENGTH 256

#define STATE_START 0
#define STATE_IDENTIFIER 1
#define STATE_NUMBER 2
#define STATE_OPERATOR 3
#define STATE_SPECIALCHAR 4
#define STATE_LITERAL 5
#define STATE_NONRECOGNIZED 6

// "if"
#define STATE_I_IFINT 7
#define STATE_E_ELSE 8
#define STATE_W_WHILE 9
#define STATE_R_RETURN 10
#define STATE_C_CHAR 11
#define STATE_V_VOID 12



// "int"
#define STATE_N_INT 13


// "else"
#define STATE_L_ELSE 14
#define STATE_S_ELSE 15


// "while"
#define STATE_H_WHILE 16
#define STATE_I_WHILE 17
#define STATE_L_WHILE 18

// "return"
#define STATE_E_RETURN 19
#define STATE_T_RETURN 20
#define STATE_U_RETURN 21
#define STATE_R_RETURN2 22


// "char"
#define STATE_H_CHAR 23
#define STATE_A_CHAR 24


// "void"
#define STATE_O_VOID 25
#define STATE_I_VOID 26

#define STATE_F_IF 27
#define STATE_T_INT 28
#define STATE_E_END_ELSE 29
#define STATE_E_END_WHILE 30
#define STATE_N_END_RETURN 31
#define STATE_R_END_CHAR 32
#define STATE_D_END_VOID 33
#define STATE_FINAL 100

// ==========================
//      ESTRUCTURA DEL DFA
// ==========================

typedef struct {
    int transition_table[MAX_STATES][MAX_INPUTS];
} DFA;

// ==========================
//      ESTRUCTURAS DE TOKENS
// ==========================

typedef struct {
    char lexeme[MAX_TOKEN_LENGTH];
    char category[50];
    int line_number;
} Token;

typedef struct {
    Token tokens[1024];  // Almacena los tokens detectados
    int size;
} TokenList;

// ==========================
//       Categories of tokens
// ==========================

#define CAT_NUMBER        "CAT_NUMBER"
#define CAT_IDENTIFIER    "CAT_IDENTIFIER"
#define CAT_KEYWORD       "CAT_KEYWORD"
#define CAT_LITERAL       "CAT_LITERAL"
#define CAT_SPECIALCHAR   "CAT_SPECIALCHAR"
#define CAT_OPERATOR      "CAT_OPERATOR"
#define CAT_NONRECOGNIZED "CAT_NONRECOGNIZED"

#define KEYWORDS_COUNT (sizeof(keywords) / sizeof(keywords[0]))

// Functions
void init_transition_table(DFA* dfa);
int is_keyword(const char* lexeme);
int dfa_get_next_state(DFA* dfa, int current_state, char current_char, char lookahead, char* buffer);
void categorize_token(const char* lexeme, int line_number, TokenList* token_list);
void lexical_analyzer(FILE* input_file, FILE* output_file, TokenList* token_list, DFA* dfa);
int my_strlen(const char* str);
int my_strcmp(const char* str1, const char* str2);
char* my_strcpy(char* dest, const char* src);
char* my_strchr(const char* str, int c);

#endif