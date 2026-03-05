#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Re/Parser.h"
#include "Re/Lexer.h"
#include "Re/Expr.h"
#include "Re/TokenKind.h"

enum SymbolKind token_to_symbol_kind(Token* tok)
{
    switch (tok->kind)
    {
        case Tk_Plus: return SK_Positive;
        case Tk_Minus: return SK_Negation;
        case Tk_PlusPlus: return SK_Increment;
        case Tk_MinusMinus: return SK_Decrement;
        default: return SK_Unknown;
    }
}

void print_expr(Expr* expr, int tabs)
{
    for (int i = 0; i < tabs; i++) printf("  ");

    if (expr->kind == Expr_Symbolic)
    {
        printf("[ Expr_Symbolic ] %s\n",
               symbolsMap[expr->sexpr.symbol][1]);

        for (int i = 0; i < expr->sexpr.count; i++)
            print_expr(expr->sexpr.operands[i], tabs + 1);
    }
    else
    {
        printf("[ %s ] -> %s\n",
               expressionsMap[expr->kind],
               expr->leaf.value);
    }
}

Expr parse(Compiler* com, Token* tok)
{
    Expr expr = {0};

    if (tok->kind >= Tk_Plus && tok->kind <= Tk_MinusMinus)
    {
        expr.kind = Expr_Symbolic;
        expr.sexpr.symbol = token_to_symbol_kind(tok);
        expr.sexpr.count = 0;

        advance(com, tok);

        for (int i = 0; i < 2; i++)
        {
            Expr child = parse(com, tok);

            if (child.kind == Expr_Invalid)
                break;

            Expr* node = malloc(sizeof(Expr));
            *node = child;

            expr.sexpr.operands[expr.sexpr.count++] = node;
        }

        return expr;
    }

    switch (tok->kind)
    {
        case Tk_IntLiteral:
            expr.kind = Expr_IntLeaf;
            expr.leaf.value = strdup(tok->lexeme);
            break;

        case Tk_FloatLiteral:
            expr.kind = Expr_FloatLeaf;
            expr.leaf.value = strdup(tok->lexeme);
            break;

        case Tk_StringLiteral:
            expr.kind = Expr_StringLeaf;
            expr.leaf.value = strdup(tok->lexeme);
            break;

        case Tk_Identifier:
            expr.kind = Expr_Identifier;
            expr.leaf.value = strdup(tok->lexeme);
            break;

        default:
            expr.kind = Expr_Invalid;
            break;
    }

    advance(com, tok);
    return expr;
}