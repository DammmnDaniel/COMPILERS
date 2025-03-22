#include "remove_comments.h"

/*========================================================================================================*/
/*                                   DELETE COMENTS OF LINE                                               */
/*========================================================================================================*/

char* remove_comments(char* line) {
    int i = 0, inComment = 0, in_double_quotes = 0, in_single_quotes = 0;
    char new_line[strlen(line)];
    
    while (i < strlen(line)) {
        if ((line[i] == '"') && in_single_quotes == 0 && inComment == 0) {
            in_double_quotes = !in_double_quotes;
            inComment = 1;
        } else if ((line[i] == '\'') && in_double_quotes == 0 && inComment == 0) {
            in_single_quotes = !in_single_quotes;
            inComment = 1;
        } else if ((line[i] == '"') && in_double_quotes == 1 && inComment == 1) {
            in_double_quotes = !in_double_quotes;
            inComment = 0;
        } else if ((line[i] == '\'') && in_single_quotes == 1 && inComment == 1) {
            in_single_quotes = !in_single_quotes;
            inComment = 0;
        }
        
        if (line[i] == '/' && line[i + 1] == '/' && inComment == 0) {
            line[i] = '\0';
            break;
        }

        i++;
    }

    strcpy(new_line, line);
    return line;
}

/*========================================================================================================*/
/*                                   DELETE COMENTS OF BLOCK                                              */
/*========================================================================================================*/

Comment remove_blocks(char* line) {
    Comment new_line;
    new_line.line = malloc(strlen(line) + 1); // Allocate memory for the processed line
    if (!new_line.line) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    int in_single_quotes = 0;
    int in_double_quotes = 0;
    char* start = NULL;
    char* end = NULL;
    char* current = line;


    strcpy(new_line.line, line);

    while ((start = strstr(current, "/*")) != NULL) {
        // Check if the start is inside quotes
        for (char* p = current; p < start; p++) {
            if (*p == '"' && !in_single_quotes) {
                in_double_quotes = !in_double_quotes;
            } else if (*p == '\'' && !in_double_quotes) {
                in_single_quotes = !in_single_quotes;
            }
        }

        if (in_single_quotes || in_double_quotes) {
            current = start + 2;
            continue;
        }

        // Find the end of the block comment
        end = strstr(start + 2, "*/");
        if (!end) {
            *start = '\0';
            new_line.in_comments = 1;
            strcpy(new_line.line, line); // Update the processed line
            return new_line;
        }
        // Remove the block comment
        char* content_after = end + 2;
        memmove(start, content_after, strlen(content_after) + 1);
        current = start;
    }


    end = strstr(line, "*/");
    if(end && start == NULL){
        strcpy(new_line.line, end + 2); // Return the line after "*/"
        new_line.in_comments = 0;
        return new_line;
    }
    
    if (!end) {
        strcpy(new_line.line, ""); // Return a blank line
        new_line.in_comments = 1;
        return new_line;
    }

    new_line.in_comments = 0;
    strcpy(new_line.line, line);

    return new_line;
}