Practice 1: Preprocessor user manual

Overview

This preprocessor processes C source files by performing the following tasks:
Removing comments.
Replacing preprocessor directives starting with #.
Supporting nested #ifdef and #ifndef directives.

Usage

Command-Line Syntax:
./preprocessor <file_name> [OPTIONS]

Options
c: Eliminate all comments from the input file.
d: Replace all preprocessor directives starting with #.
all: Perform all processing tasks, including removing comments and replacing directives.
help: Display this help message (man page).

Examples
Remove comments:
./preprocessor file.c -c
Replace preprocessor directives:
./preprocessor file.c -d
Perform all tasks:
./preprocessor file.c -all


-------------------------------------------------------

Practice 2: User Manual for DFA-based Lexical Analyzer

Table of Contents
Introduction
Installation and Setup
Running the Program
COUNTOUT
Debug Mode (ddebug)
Release Mode (drelease)
Understanding the Output
Error Handling and Troubleshooting
Frequently Asked Questions (FAQs)

1. Introduction
The DFA-based Lexical Analyzer reads an input file, processes tokens using a Deterministic Finite Automaton (DFA), and categorizes them. It supports two operational modes: ddebug for detailed logs and drelease for streamlined execution.

3. Installation and Setup
Clone or download the project files.
Open a terminal or command prompt.
Compile the program using:
For Debug Mode:
gcc main.c err.c dfa.c countOperations.c -o input.c lexer -DDEBUG
For Release Mode:
gcc main.c err.c dfa.c countOperations.c -o input.c lexer -DRELEASE

4. Running the Program
The program processes a text file and generates a tokenized output.
Countout setting
At the beginning of the main, we have the following code line:

If countout is set to 1, we will write everything that we do on debug mode on the input.cscn file and on the input.cdbgcnt file and, if countout is set to 0, we only write everything into the input.cscn file.
Debug Mode (ddebug)
In this mode, the program provides detailed information about token processing, including computational operations and token tracking.
Usage:
./lexer input.c
Expected Outputs:
Tokenized Output: Stored in input.cscn
Debug Log: Stored in input.txtdbgcnt
Example Output:
1 <if, CAT_KEYWORD> <else, CAT_KEYWORD> <while, CAT_KEYWORD> <return, CAT_KEYWORD> <int, CAT_KEYWORD> <char, CAT_KEYWORD> <void, CAT_KEYWORD> <11111111111, CAT_NUMBER> <int, CAT_KEYWORD> 
2 <prueba, CAT_IDENTIFIER> 

0.Function: lexical_analyzer
Line: 2
Totals - Comp: 3128, IO: 60, Gen: 3374
Increments - Comp: 326, IO: 60, Gen: 484

1.Function: categorize_token
Line: 2
Totals - Comp: 2802, IO: 0, Gen: 2890
Increments - Comp: 6, IO: 0, Gen: 6

2.Function: my_strcpy
Line: 2
Totals - Comp: 2796, IO: 0, Gen: 2884
Increments - Comp: 14, IO: 0, Gen: 44

...
Release Mode (drelease)
This mode provides only the tokenized output, without additional logs.
Usage:
./lexer input.c
Expected Outputs:
Tokenized Output: Stored in input.txtscn
Example Output:
<if, KEYWORD> <x, IDENTIFIER> <==, OPERATOR> <10, NUMBER>
<return, KEYWORD> <x, IDENTIFIER> <;, SPECIALCHAR>


5. Understanding the Output
The output consists of tokens formatted as <lexeme, category>. Categories include:
KEYWORD (e.g., if, else, return)
IDENTIFIER (e.g., variable names)
NUMBER (e.g., 10, 42)
OPERATOR (e.g., ==, +, *)
SPECIALCHAR (e.g., (, ), ;)
LITERAL (e.g., "hello")

6. Error Handling and Troubleshooting
Common Errors:
File Not Found (NOFILEFOUND)
Ensure the input file exists in the specified directory.
Invalid Token Detected (NONRECOGNIZETOKEN)
Check for syntax errors in the input file.
Segmentation Fault
Ensure correct file paths and memory allocation.
Debug File Not Generated
Verify the program was compiled in ddebug mode.

--------------------------------------------------------------------------
Practice 3 Bottom-up Parsing automata 
Introduction: 
Bottom-up parsing is a technique in compilers and formal language processing to construct parse trees from a given input. Unlike top-down parsing, which starts from the grammar's root and attempts to derive the input, the bottom-up approach begins with the input tokens and works upwards, reducing productions until it reaches the start symbol.
Implementing this technique was more complicated than we thought.
This method relies on shift-reduce parsing, where the parser uses a stack to store symbols and perform two key operations:
Shift: Reads a new token from the input and pushes it onto the stack.
Reduce: Replaces a sequence of symbols on the stack with the left-hand side of a grammar production, depending on the states and rules.


