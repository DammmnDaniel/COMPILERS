Group: GB
Preprocessor user manual

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
