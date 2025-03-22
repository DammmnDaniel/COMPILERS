#include "dfa.h"

const char* keywords[] = { "if", "else", "while", "return", "void", "char", "int"};
int line_number = 1;

// ==========================
//   INITIALIZATION FUNCTIONS   
// ==========================

// Start the transition table of the DFA
void init_transition_table(DFA* dfa) {
    int i, j;

    // Category for the characters we are not interested in, non-recognized
    for (i = 127; i <= 255; i++) {
        dfa->transition_table[STATE_START][i] = STATE_NONRECOGNIZED;
        dfa->transition_table[STATE_NONRECOGNIZED][i] = STATE_NONRECOGNIZED;
    }
    for (i = 0; i < MAX_STATES; i++) {
        for (j = 0; j < MAX_INPUTS; j++) {
            dfa->transition_table[i][j] = -1;
        }
        dfa->transition_table[i][' '] = -1;
    }
    // The nodes in the middle of a keyword
    for (i = 7; i < 26; i++) {
        for (j = 0; j < MAX_INPUTS; j++) {
            dfa->transition_table[i][j] = STATE_NONRECOGNIZED;
        }
    }
    
    //Transitions for Keywords
    dfa->transition_table[STATE_START]['i'] = STATE_I_IFINT;
    dfa->transition_table[STATE_I_IFINT]['f'] = STATE_F_IF;

    dfa->transition_table[STATE_START]['e'] = STATE_E_ELSE;
    dfa->transition_table[STATE_START]['w'] = STATE_W_WHILE;
    dfa->transition_table[STATE_START]['r'] = STATE_R_RETURN;
    dfa->transition_table[STATE_START]['c'] = STATE_C_CHAR;
    dfa->transition_table[STATE_START]['v'] = STATE_V_VOID;

    dfa->transition_table[STATE_I_IFINT]['n'] = STATE_N_INT;
    dfa->transition_table[STATE_N_INT]['t'] = STATE_T_INT;

    dfa->transition_table[STATE_E_ELSE]['l'] = STATE_L_ELSE;
    dfa->transition_table[STATE_L_ELSE]['s'] = STATE_S_ELSE;
    dfa->transition_table[STATE_S_ELSE]['e'] = STATE_E_END_ELSE;

    dfa->transition_table[STATE_W_WHILE]['h'] = STATE_H_WHILE;
    dfa->transition_table[STATE_H_WHILE]['i'] = STATE_I_WHILE;
    dfa->transition_table[STATE_I_WHILE]['l'] = STATE_L_WHILE;
    dfa->transition_table[STATE_L_WHILE]['e'] = STATE_E_END_WHILE;

    dfa->transition_table[STATE_R_RETURN]['e'] = STATE_E_RETURN;
    dfa->transition_table[STATE_E_RETURN]['t'] = STATE_T_RETURN;
    dfa->transition_table[STATE_T_RETURN]['u'] = STATE_U_RETURN;
    dfa->transition_table[STATE_U_RETURN]['r'] = STATE_R_RETURN2;
    dfa->transition_table[STATE_R_RETURN2]['n'] = STATE_N_END_RETURN;

    dfa->transition_table[STATE_C_CHAR]['h'] = STATE_H_CHAR;
    dfa->transition_table[STATE_H_CHAR]['a'] = STATE_A_CHAR;
    dfa->transition_table[STATE_A_CHAR]['r'] = STATE_R_END_CHAR;

    dfa->transition_table[STATE_V_VOID]['o'] = STATE_O_VOID;
    dfa->transition_table[STATE_O_VOID]['i'] = STATE_I_VOID;
    dfa->transition_table[STATE_I_VOID]['d'] = STATE_D_END_VOID;

    // Letters and numbers can be identifiers
    for (i = 'a'; i <= 'z'; i++) {
        dfa->transition_table[STATE_IDENTIFIER][i] = STATE_IDENTIFIER;

        if (i == 'i' || i == 'e' || i == 'w' || i == 'r' || i == 'c' || i == 'v') {
            continue;
        }

        dfa->transition_table[STATE_START][i] = STATE_IDENTIFIER;
    }
    for (i = 'A'; i <= 'Z'; i++) {
        dfa->transition_table[STATE_IDENTIFIER][i] = STATE_IDENTIFIER;
    }
    for (i = '0'; i <= '9'; i++) {
        dfa->transition_table[STATE_IDENTIFIER][i] = STATE_IDENTIFIER;
    }

    for (i = '0'; i <= '9'; i++) {
        dfa->transition_table[STATE_NUMBER][i] = STATE_NUMBER;
    } 

    for (i = '0'; i <= '9'; i++) {
        dfa->transition_table[STATE_START][i] = STATE_NUMBER;
    }

    // Operators
    dfa->transition_table[STATE_START]['='] = STATE_OPERATOR;
    dfa->transition_table[STATE_START]['>'] = STATE_OPERATOR;
    dfa->transition_table[STATE_START]['+'] = STATE_OPERATOR;
    dfa->transition_table[STATE_START]['*'] = STATE_OPERATOR;

    // Special characters
    dfa->transition_table[STATE_START]['('] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START][')'] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START]['{'] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START]['}'] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START]['['] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START][']'] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START][';'] = STATE_SPECIALCHAR;
    dfa->transition_table[STATE_START][','] = STATE_SPECIALCHAR;

    // Start of literals
    dfa->transition_table[STATE_START]['"'] = STATE_LITERAL;

    // Let every character inside the literal (from ASCII 32 to 126)
    for (i = 32; i <= 255; i++) {
        dfa->transition_table[STATE_LITERAL][i] = STATE_LITERAL;
    }

    // A second " will end the literal
    dfa->transition_table[STATE_LITERAL]['"'] = STATE_FINAL;

    // If there is a newline inside a literal, it is an error
    dfa->transition_table[STATE_LITERAL]['\n'] = STATE_NONRECOGNIZED;
}

// ==========================
//    DFA FUNCTIONS
// ==========================

// Verify if a lexeme is a keyword
int is_keyword(const char* lexeme) {

    #ifdef DEBUG
    int comp = 0;
    int io = 0;
    int gen = 1;
    #endif


    for (int i = 0; i < KEYWORDS_COUNT; i++) {

        #ifdef DEBUG
        comp = comp + 2;
        gen = gen + 2;
        #endif
        
        if (my_strcmp(lexeme, keywords[i]) == 0) {
            #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
            #endif

            return 1;
        }
    }

    #ifdef DEBUG
    acumulated_count(line_number, __func__, comp, io, gen);
    #endif

    return 0;
}

// Determines the next state based on the DFA
int dfa_get_next_state(DFA* dfa, int current_state, char current_char, char lookahead, char* buffer) {
    
    #ifdef DEBUG
    int comp = 0;
    int io = 0;
    int gen = 1;
    #endif

    int next_state = dfa->transition_table[current_state][(int)current_char];

    #ifdef DEBUG
    comp++;
    #endif

    if (next_state == -1) {

        #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
        #endif

        return STATE_FINAL;
    }


    #ifdef DEBUG
    comp = comp + 2;
    #endif

    // Spaces
    if (current_char == ' ' || current_char == '\n') {

        #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
        #endif
        
        return current_state; 
    }

    #ifdef DEBUG
    comp++;
    #endif

    // Special handle of literals
    if (current_state == STATE_LITERAL) {

        #ifdef DEBUG
        comp = comp + 2;
        #endif

        if (current_char == '"' && buffer[0] == '"') {

            #ifdef DEBUG
                acumulated_count(line_number, __func__, comp, io, gen);
            #endif

            return STATE_FINAL; // El literal se cierra correctamente
        }

        #ifdef DEBUG
        comp++;
        #endif

        if (current_char == '\n') {

            #ifdef DEBUG
                acumulated_count(line_number, __func__, comp, io, gen);
            #endif

            return STATE_NONRECOGNIZED; // Error: the literal is not closed before the end of the line
        }
    }

    #ifdef DEBUG
    gen = gen + 4;
    #endif

    // Add the current character to the buffer and check if the token should end
    int len = my_strlen(buffer);
    buffer[len] = current_char;
    buffer[len + 1] = '\0';

    #ifdef DEBUG
    comp = comp + 2;
    gen = gen + 2;
    #endif

    // If the current character is a letter and the lookahead is a letter, the token should end
    if (is_keyword(buffer) && isalnum(lookahead)) {
        
        #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
        #endif
        
        return STATE_FINAL;
    }

    #ifdef DEBUG
    comp = comp + 2;
    gen++;
    #endif

    // If the current character is a letter and the lookahead is a special character, the token should end
    int lookahead_state = dfa->transition_table[next_state][(int)lookahead];
    if (lookahead_state == -1 || lookahead == EOF) {
                
        #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
        #endif
        
        return STATE_FINAL;
    }
  
    #ifdef DEBUG
        acumulated_count(line_number, __func__, comp, io, gen);
    #endif

    return next_state;
}

// ==========================
//    TOKEN FUNCTIONS
// ==========================

// Save the token in the token list
void categorize_token(const char* lexeme, int line_number, TokenList* token_list) {

    #ifdef DEBUG
    int comp = 3;
    int io = 0;
    int gen = 0;
    #endif

    // Ignore empty tokens or tokens that are just spaces or newlines
    if (lexeme[0] == '\0' || lexeme[0] == ' ' || lexeme[0] == '\n') { 
          
        #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
        #endif

        return;
    }

    #ifdef DEBUG
    comp++;
    gen = gen + 3;
    #endif

    // Initialize a new token
    Token token;
    my_strcpy(token.lexeme, lexeme); // Copy the lexeme to the token
    token.line_number = line_number; // Set the line number of the token

    // Determine the category of the token
    if (is_keyword(lexeme)) {

        #ifdef DEBUG
        gen++;
        #endif

        my_strcpy(token.category, CAT_KEYWORD); // Keyword category
    }
    else if (isalpha(lexeme[0]) || lexeme[0] == '_') {
        
        #ifdef DEBUG
        comp = comp + 2;
        gen = gen + 2;
        #endif
        
        my_strcpy(token.category, CAT_IDENTIFIER); // Identifier category
    }
    else if (isdigit(lexeme[0])) {
        
        #ifdef DEBUG
        comp = comp + 3;
        gen = gen + 3;
        #endif
        
        my_strcpy(token.category, CAT_NUMBER); // Number category
    }
    else if (my_strchr("=+*>", lexeme[0])) {

        #ifdef DEBUG
        comp = comp + 4;
        gen = gen + 4;
        #endif
        
        my_strcpy(token.category, CAT_OPERATOR); // Operator category
    }
    else if (my_strchr("(){}[];,", lexeme[0])) {
                
        #ifdef DEBUG
        comp = comp + 5;
        gen = gen + 5;
        #endif
        
        my_strcpy(token.category, CAT_SPECIALCHAR); // Special character category
    }
    else if (lexeme[0] == '"' && lexeme[my_strlen(lexeme) - 1] == '"') {
        
        #ifdef DEBUG
        comp = comp + 7;
        gen = gen + 6;
        #endif
        
        my_strcpy(token.category, CAT_LITERAL); // Literal category
    }
    else {

        #ifdef DEBUG
        comp = comp + 7;
        gen = gen + 8;
        #endif

        // Unrecognized token
        printf("Error: Unrecognized token: %s\n", lexeme);
        my_strcpy(token.category, CAT_NONRECOGNIZED);
        call_error(NONRECOGNIZETOKEN);
    }

    #ifdef DEBUG
    gen++;
    #endif

    // Add the token to the token list
    token_list->tokens[token_list->size++] = token;

    #ifdef DEBUG
        acumulated_count(line_number, __func__, comp, io, gen);
    #endif

}

// ==========================
//    LEXICAL ANALYZER
// ==========================

// Function to perform lexical analysis
void lexical_analyzer(FILE* input_file, FILE* output_file, TokenList* token_list, DFA* dfa) {

    #ifdef DEBUG
    int comp = 1;
    int io = 1;
    int gen = 5;
    #endif

    char current_char, lookahead;
    char buffer[MAX_TOKEN_LENGTH] = "";
    int index = 0;
    int current_state = STATE_START;
    // int line_number = 1;

    // Read the first character from the input file
    current_char = fgetc(input_file);
    if (ferror(input_file)) {

        #ifdef DEBUG
        gen++;
        #endif

        perror("Error reading input file");

        #ifdef DEBUG
        acumulated_count(line_number, __func__, comp, io, gen);
        #endif

        return;
    }

    #ifdef DEBUG
    comp++;
    io++;
    gen++;
    #endif

    // Read the lookahead character from the input file
    lookahead = fgetc(input_file);
    if (ferror(input_file)) {

        #ifdef DEBUG
        gen++;
        #endif

        perror("Error reading input file");

        #ifdef DEBUG
        acumulated_count(line_number, __func__, comp, io, gen);
        #endif

        return;
    }

    // Process characters until the end of the file
    while (current_char != EOF) {
        #ifdef DEBUG
        comp = comp + 3;
        #endif

        // Handle newlines and carriage returns
        if (current_char == '\n' || current_char == '\r') {

            #ifdef DEBUG
            gen = gen + 3;
            io++;
            #endif

            line_number++;
            current_char = lookahead;
            lookahead = fgetc(input_file);
        }

        #ifdef DEBUG
        comp++;
        gen = gen + 2;
        #endif

        // Get the next state based on the current state and character
        int next_state = dfa_get_next_state(dfa, current_state, current_char, lookahead, buffer);

        // If the next state is not final, continue processing the token
        if (next_state != STATE_FINAL) {
            #ifdef DEBUG
            comp++;
            #endif

            // Check for token buffer overflow
            if (index >= MAX_TOKEN_LENGTH - 1) {

                #ifdef DEBUG
                gen++;
                #endif

                fprintf(stderr, "Error: Token buffer overflow at line %d\n", line_number);

                #ifdef DEBUG
                acumulated_count(line_number, __func__, comp, io, gen);
                #endif

                return;
            }

            #ifdef DEBUG
            gen = gen + 2;
            #endif

            // Add the current character to the buffer and update the state
            buffer[index++] = current_char;
            current_state = next_state;
        } else {

            #ifdef DEBUG
            gen = gen + 6;
            #endif

            // Finalize the token and categorize it
            buffer[index++] = current_char;
            buffer[index] = '\0';
            categorize_token(buffer, line_number, token_list);
            index = 0;
            buffer[0] = '\0';
            current_state = STATE_START;
        }

        #ifdef DEBUG
        comp++;
        gen = gen + 3;
        io++;
        #endif

        // Read the next character and lookahead character from the input file
        current_char = lookahead;
        lookahead = fgetc(input_file);
        if (ferror(input_file)) {

            #ifdef DEBUG
            gen++;
            #endif

            perror("Error reading input file");

            #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
            #endif

            return;
        }
    }

    #ifdef DEBUG
    comp++;
    #endif

    // If there is any remaining token in the buffer, categorize it
    if (index > 0) {
        
        #ifdef DEBUG
        gen = gen + 2;
        #endif
        
        buffer[index] = '\0';
        categorize_token(buffer, line_number, token_list);
    }

    #ifdef DEBUG
    acumulated_count(line_number, __func__, comp, io, gen);
    #endif
}

// ==========================
//    STRING FUNCTIONS
// ==========================

// Function to calculate the length of a string
int my_strlen(const char* str) {

    #ifdef DEBUG
    int comp = 0;
    int io = 0;
    int gen = 1;
    #endif

    int length = 0;
    while (str[length] != '\0') {

        #ifdef DEBUG
        comp++;
        gen++;
        #endif

        length++;
    }

    #ifdef DEBUG
    acumulated_count(line_number, __func__, comp, io, gen);
    #endif

    return length;
}

// Function to compare two strings
int my_strcmp(const char* str1, const char* str2) {
    
    #ifdef DEBUG
    int comp = 0;
    int io = 0;
    int gen = 1;
    #endif

    while (*str1 != '\0' && *str2 != '\0') {
        
        #ifdef DEBUG
        comp = comp + 3;
        #endif

        if (*str1 != *str2) {

            #ifdef DEBUG
            gen++;

            acumulated_count(line_number, __func__, comp, io, gen);
            #endif

            return (unsigned char)*str1 - (unsigned char)*str2;
        }

        #ifdef DEBUG
        gen = gen + 2;
        #endif

        str1++;
        str2++;
    }

    #ifdef DEBUG
    gen++;

    acumulated_count(line_number, __func__, comp, io, gen);
    #endif

    return (unsigned char)*str1 - (unsigned char)*str2;
}

// Function to copy a string
char* my_strcpy(char* dest, const char* src) {

    #ifdef DEBUG
    int comp = 0;
    int io = 0;
    int gen = 1;
    #endif

    char* original_dest = dest; 

    while (*src != '\0') {

        #ifdef DEBUG
        comp++;
        gen = gen + 3;
        #endif

        *dest = *src;
        dest++;
        src++;
    }

    #ifdef DEBUG
    gen++;
    #endif
    
    *dest = '\0';

    #ifdef DEBUG
    acumulated_count(line_number, __func__, comp, io, gen);
    #endif

    return original_dest; 
}

// Function to find the first occurrence of a character in a string
char* my_strchr(const char* str, int c) {
    
    #ifdef DEBUG
    int comp = 0;
    int io = 0;
    int gen = 0;
    #endif

    while (*str != '\0') {

        #ifdef DEBUG
        comp = comp + 2;
        #endif

        if (*str == c) {

            #ifdef DEBUG
            acumulated_count(line_number, __func__, comp, io, gen);
            #endif

            return (char*)str;  // Return a pointer to the character
        }

        #ifdef DEBUG
        gen++;
        #endif

        str++;
    }

    #ifdef DEBUG
    acumulated_count(line_number, __func__, comp, io, gen);
    #endif

    return NULL;
}
