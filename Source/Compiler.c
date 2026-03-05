#define  RE_DEBUG
#include "Re/Token.h"
#include "Re/Lexer.h"
#include "Re/Parser.h"
#include "Re/Compiler.h"

#define filename "/home/yash/Projects/re-remake/Examples/example1.re"

#define print_tok(_tok_) \
    printf("[ %2d:%-2d ] %26s  %-16s \n", _tok_.row, _tok_.col, tokensMap[_tok_.kind], _tok_.lexeme)


int main()
{
#ifdef RE_DEBUG
    initTokens();
    initExpressions();
    initSymbols();
#endif

    Compiler com;
    com.row = 1, com.col = 1;
    com.file = fopen(filename, "r");
    if (com.file == NULL) return 1;

    Token tok;
    Expr expr;

    while (tok.kind != Tk_EOF)
    { advance(&com, &tok); print_tok(tok); }
    com.row = 1, com.col = 1;
    rewind(com.file);
    printf("-END-\n\n");

    advance(&com, &tok);

    int i;
    while (1)
    {
        expr = parse(&com, &tok);
        print_expr(&expr, 0);
        // free_expr(expr);

        if (tok.kind == Tk_EOF) break;
        i++;
    }

    return 0;
}