#ifndef COMPILER_H
#define COMPILER_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _Compiler {
    int cur;
    int row, col;
    FILE* file;
};
typedef struct _Compiler Compiler;

#endif // COMPILER_H