#ifndef ERR_H
#define ERR_H

#include <stdio.h>

#define NOFILEFOUND 1
#define FILENOTWORK 2
#define NONRECOGNIZETOKEN 3

int call_error(int error);

#endif