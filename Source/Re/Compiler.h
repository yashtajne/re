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
    char* types[16];
    char* units[16];
    char* functions[32];
    int function_params[32];
    int types_len;
    int units_len;
    int functions_len;
};
typedef struct _Compiler Compiler;

int is_type(Compiler* com, char* buffer, int* typeindex);
int is_unit(Compiler* com, char* buffer);

int AddType(Compiler* com, const char* name);
int AddUnit(Compiler* com, const char* name);
int AddFunction(Compiler* com, const char* name, int params_len);
int is_function(Compiler* com, char* buffer, int* params_len);
int ResetCompiler(Compiler* com);

#endif // COMPILER_H
