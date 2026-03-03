#define   RE_DEBUG
#include "Re/Token.h"
#include "Re/Lexer.h"
#include "Re/Compiler.h"

#define filename "/home/yash/Projects/re-remake/Examples/example1.re"
#define print(_tokenkind_) printf("[ %2d:%-2d ] %26s  %-16s \n", tok.row, tok.col, tokenMap[_tokenkind_], tok.lexeme)

int main()
{
#ifdef RE_DEBUG
    initTokens();
#endif

    Compiler com;
    com.row = 1, com.col = 1;
    com.file = fopen(filename, "r");
    if (com.file == NULL) return 1;

    Token tok;

    int i;
    while (1)
    {
        if (!next(&com, &tok))
        {
            printf("failed! at token %d\n", i);
            break;
        }
        print(tok.kind);
        if (tok.kind == Tk_EOF) break;
        i++;
    }

    return 0;
}