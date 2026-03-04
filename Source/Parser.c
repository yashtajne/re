#include "Re/Colors.h"
#include "Re/Parser.h"
#include "Re/Lexer.h"
#include "Re/Expr.h"
#include "Re/TokenKind.h"

#define getrow() com->row
#define getcol() com->col

#define error(_message_) \
{ \
    printf("\n[ %2d:%-2d ] "red"parse error:"reset" %s\n", getrow(), getcol(), _message_); \
    exit(1); \
}

#define token_in_range(_min_, _max_) \
    (tok->kind >= _min_) && (tok->kind <= _max_)

#define expression(_kind_) \
{ \
    expr.kind = _kind_; \
    if (expr.kind == Expr_Int) { expr.leaf.value = strdup(tok->lexeme); } else \
    if (expr.kind == Expr_Float) { expr.leaf.value = strdup(tok->lexeme); } \
    return expr; \
}

enum PrefixKind token_to_prefix_kind(Token* tok) {
    switch (tok->kind) {
        case Tk_Plus:
            return PK_Positive;
        case Tk_Minus:
            return PK_Negation;
        case Tk_PlusPlus:
            return PK_Increment;
        case Tk_MinusMinus:
            return PK_Decrement;
        default:
            return PK_Unknown;
    }
}

#define print_tabs() { for (int i = 0; i < tab_size; i++) printf("  "); }
void print_expr(Expr expr, int tab_size)
{
    print_tabs()
    if (expr.kind == Expr_Prefix)
    {
        printf("\n[ %s ]\n", expressionsMap[expr.kind]);

        print_tabs()
        printf("(%s '%s')\n", prefixesMap[expr.prefix.kind][0], prefixesMap[expr.prefix.kind][1]);

        if (expr.prefix.expr != NULL) {
            print_expr(*expr.prefix.expr, tab_size + 1);
            printf("\n");
        } else {
            print_tabs()
            printf("Error: Null expression in prefix!\n");
        }
    }
    else
    {
        print_tabs()
        printf("[ %s ] -> %s\n", expressionsMap[expr.kind], expr.kind != Expr_Invalid ? expr.leaf.value : "(invalid)");
    }
}


Expr parse(Compiler* com, Token* tok)
{
    if (tok->kind == Tk_EOF) error("End of file...")

    Expr expr;
    if ((tok->kind >= Tk_Plus) && (tok->kind <= Tk_MinusMinus))
    {
        expr.kind = Expr_Prefix;
        expr.prefix.kind = token_to_prefix_kind(tok);

        advance(com, tok);

        expr.prefix.expr = malloc(sizeof(Expr));
        if (!expr.prefix.expr) {
            error("Malloc failed! parser.c");
        }

        *expr.prefix.expr = parse(com, tok);
        return expr;
    }
    else
    {
        switch (tok->kind)
        {
            case Tk_IntLiteral: expr.kind = Expr_Int; break;
            case Tk_FloatLiteral: expr.kind = Expr_Float; break;
            case Tk_StringLiteral: expr.kind = Expr_String; break;
            case Tk_Identifier: expr.kind = Expr_Identifier; break;
            default: expr.kind = Expr_Invalid; break;
        }
        if (expr.kind != Expr_Invalid)
            expr.leaf.value = strdup(tok->lexeme);
    }
    return expr;
}