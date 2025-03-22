#ifndef REMOVE_COMMENTS_H
#define REMOVE_COMMENTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 1024

typedef struct {
    int in_comments;
    char* line;
} Comment;

char* remove_comments(char* line);
Comment remove_blocks(char* line);

#endif
