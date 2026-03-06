#ifndef COMPILER_H
#define COMPILER_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct _Compiler {
    int cur;
    int row, col;
    FILE* file;
    char* types[16],
          global, scoped;
    int types_len;
};
typedef struct _Compiler Compiler;

int is_type(Compiler* com, char* buffer, int* typeindex);

int AddType(Compiler* com, const char* name);
int ResetCompiler(Compiler* com);

#endif // COMPILER_H