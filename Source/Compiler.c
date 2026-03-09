#define  RE_DEBUG
#include "Re/Colors.h"
#include "Re/Token.h"
#include "Re/Lexer.h"
#include "Re/Parser.h"
#include "Re/Compiler.h"

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
    return com;
}

int FreeCompiler(Compiler* com)
{
    for (int i = 0; i < com->types_len; i++)
        free(com->types[i]);
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
    Expr expr = {0};

    while (tok.kind != Tk_EOF)
    { advance(&com, &tok); print_tok(tok); }
    ResetCompiler(&com);
    printf("-END-\n\n");

    advance(&com, &tok);

    while (1)
    {
        Stmt stmt;
        if (!parse(&com, &tok, &stmt))
        {
            if (tok.kind == Tk_EOF) break;
            continue;
        }
        print_stmt(&com, &stmt);
    }

    return 0;
}
