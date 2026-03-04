#ifndef LEXER_H
#define LEXER_H
#include "Compiler.h"
#include "Token.h"

#define advance(_com_, _tok_) \
    if (!next(_com_, _tok_)) { printf("failed! at token\n"); exit(1); }

int next(Compiler* com, Token* tok);

#endif // LEXER_H