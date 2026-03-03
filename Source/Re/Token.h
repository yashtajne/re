#ifndef TOKEN_H
#define TOKEN_H
#include "TokenKind.h"

struct _Token {
    enum TokenKind kind;
    int row, col;
    char lexeme[32];
};
typedef struct _Token Token;

#endif // TOKEN_H