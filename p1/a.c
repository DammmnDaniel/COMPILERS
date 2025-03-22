#include <stdio.h>

// This is a single-line comment

/*
This is a
multi-line comment
*/

#define DEBUG
#define VERSION 3
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#ifdef DEBUG
    printf("Debug mode is active.\n");
#endif

#ifndef RELEASE
    printf("Release mode is disabled.\n");
#endif

#ifdef VERSION
    printf("Version: %d\n", VERSION);
#endif

#ifndef UNUSED
    printf("This block will always be included.\n");
#endif

// Parameterized macro usage
int main() {
    int x = 5, y = 10;
    printf("Max value: %d\n", MAX(x, y));
    return 0;
}