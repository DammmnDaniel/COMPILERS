#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
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
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_LENGTH];
    TokenType type;
    int state;
} Token;

typedef struct TokenLinkedList {
    Token token;
    struct TokenLinkedList* next;
} TokenLinkedList;

#endif
