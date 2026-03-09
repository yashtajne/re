#define  RE_DEBUG
#include "Re/Colors.h"
#include "Re/Token.h"
#include "Re/Lexer.h"
#include "Re/Parser.h"
#include "Re/Compiler.h"
#include <errno.h>
#include <sys/stat.h>

#define FILENAME "/home/yash/Projects/re-remake/Examples/example1.re"

#define print_tok(_tok_) \
    printf("[ %2d:%-2d ] %26s  %-16s \n", _tok_.row, _tok_.col, tokensMap[_tok_.kind], _tok_.lexeme)

#define error(_message_) \
{ \
    printf(red"error:"reset" %s\n", _message_); \
    exit(1); \
}

Compiler NewCompiler(const char* filename)
{
    Compiler com = {0};
    com.row = 1, com.col = 1;
    com.file = fopen(filename, "r");
    if (com.file == NULL) error("Could not open file: " FILENAME);
    AddType(&com, "int");
    AddType(&com, "float");
    return com;
}

int FreeCompiler(Compiler* com)
{
    for (int i = 0; i < com->types_len; i++)
        free(com->types[i]);
    for (int i = 0; i < com->units_len; i++)
        free(com->units[i]);
    for (int i = 0; i < com->functions_len; i++)
        free(com->functions[i]);
    return 1;
}

int AddType(Compiler* com, const char* name)
{
    char* typename = strdup(name);
    com->types[com->types_len] = typename;
    com->types_len++;
    return 1;
}

int AddUnit(Compiler* com, const char* name)
{
    char* unitname = strdup(name);
    com->units[com->units_len] = unitname;
    com->units_len++;
    return 1;
}

int AddFunction(Compiler* com, const char* name, int params_len)
{
    char* fname = strdup(name);
    com->functions[com->functions_len] = fname;
    com->function_params[com->functions_len] = params_len;
    com->functions_len++;
    return 1;
}

int is_function(Compiler* com, char* buffer, int* params_len)
{
    for (int i = 0; i < com->functions_len; i++)
    {
        if (0 == strcmp(com->functions[i], buffer))
        {
            if (params_len != NULL)
                *params_len = com->function_params[i];
            return 1;
        }
    }
    return 0;
}

int ResetCompiler(Compiler* com)
{
    com->row = 1, com->col = 1;
    rewind(com->file);
    return 1;
}

int main()
{
#ifdef RE_DEBUG
    initTokens();
    initExpressions();
    initSymbols();
#endif

    Compiler com = NewCompiler(FILENAME);
    Token tok;
    Stmt* statements = NULL;
    int statements_len = 0;

    advance(&com, &tok);

    while (1)
    {
        Stmt stmt;
        if (!parse(&com, &tok, &stmt))
        {
            if (tok.kind == Tk_EOF) break;
            continue;
        }

        statements = realloc(statements, sizeof(Stmt) * (statements_len + 1));
        statements[statements_len++] = stmt;
        print_stmt(&com, &stmt);
    }

    if (mkdir("Generated", 0755) == -1 && errno != EEXIST)
        error("Failed to create Generated directory");

    FILE* out = fopen("Generated/out.c", "w");
    if (out == NULL)
        error("Failed to open Generated/out.c for writing");
    emit_c_program(out, &com, statements, statements_len);
    fclose(out);

    printf("\nGenerated C code at Generated/out.c\n");

    fflush(stdout);
    if (0 != system("gcc -std=c11 Generated/out.c -o Generated/out -lc"))
        error("Failed to compile Generated/out.c");

    printf("Built executable at Generated/out\n");
    printf("Program output:\n");
    fflush(stdout);
    if (0 != system("./Generated/out"))
        error("Generated executable returned an error");

    return 0;
}
