#ifndef PARSER_H
#define PARSER_H
#include "Expr.h"
#include "Token.h"
#include "Compiler.h"

void print_expr(Expr* expr, int tab_size);
Expr parse(Compiler* com, Token* tok);

#endif // PARSER_H