#include "Re/Colors.h"
#include "Re/Parser.h"
#include "Re/Lexer.h"
#include "Re/Expr.h"
#include "Re/TokenKind.h"

#define alloc_expr(_expr_) Expr* _expr_ = malloc(sizeof(Expr));

#define getrow() com->row
#define getcol() com->col
#define error(_message_, _args_) \
{ \
    printf("[%2d:%-2d ] "red"error: "reset _message_"\n", getrow(), getcol(), _args_); \
    exit(1); \
}

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
    for (int i = 1; i <= tabs; i++) printf("  ");

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

void print_stmt(Compiler* com, Stmt* stmt)
{
    if (stmt->kind == Stmt_UnitDeclaration)
    {
        printf("\n; Stmt_UnitDeclaration \n");
        printf("  - Type %s\n", com->types[stmt->unit_decl.type]);
        printf("  - Name %s\n", stmt->unit_decl.name);
        print_expr(stmt->unit_decl.expr, 0);
    }
    else
    if (stmt->kind == Stmt_Expression)
    {
        printf("\n; Stmt_Expression \n");
        print_expr(stmt->expr_stmt.expr, 0);
    }
    else
        printf("; Invalid Statement\n");
}

int is_type(Compiler* com, char* buffer, int* typeindex)
{
    for (int i = 0; i < com->types_len; i++)
        if (0 == strcmp(com->types[i], buffer))
        {
            if (typeindex != NULL)
                *typeindex = i;
            return 1;
        }
    return 0;
}

int is_unit(Compiler* com, char* buffer)
{
    for (int i = 0; i < com->units_len; i++)
        if (0 == strcmp(com->units[i], buffer))
            return 1;
    return 0;
}

int parse(Compiler* com, Token* tok, Stmt* stmt)
{
    stmt->kind = Stmt_Invalid;

    while (tok->kind == Tk_SemiColon)
        advance(com, tok);

    if (tok->kind == Tk_EOF)
        return 0;

    switch (tok->kind)
    {
        case Tk_TypeName:
            stmt->kind = Stmt_UnitDeclaration;
            advance(com, tok);
            is_type(com, tok->lexeme, &stmt->unit_decl.type);
            if (tok->kind == Tk_Identifier)
            {
                stmt->unit_decl.name = strdup(tok->lexeme);
                advance(com, tok);
                alloc_expr(expr);
                if (!parse_expr(com, tok, expr))
                    error("Expected an expression after declaration but got %s", tok->lexeme);
                stmt->unit_decl.expr = expr;

                if (is_unit(com, stmt->unit_decl.name))
                { error("Redefinition of unit %s", stmt->unit_decl.name); }
                else
                    AddUnit(com, stmt->unit_decl.name);

                while (tok->kind == Tk_SemiColon)
                    advance(com, tok);
                return 1;
            } else error("Expected an Identifier but got %s", tok->lexeme);
            break;
        default: {
            stmt->kind = Stmt_Expression;
            alloc_expr(expr);
            if (!parse_expr(com, tok, expr)) {
                free(expr);
                break;
            }
            stmt->expr_stmt.expr = expr;

            while (tok->kind == Tk_SemiColon)
                advance(com, tok);
            return 1;
        }
    }
    return 0;
}

int parse_expr(Compiler* com, Token* tok, Expr* expr)
{
    if ((tok->kind == Tk_SemiColon) ||
        (tok->kind == Tk_EOF))
    {
        return 0;
    }
    else
    if (tok->kind >= Tk_Plus && tok->kind <= Tk_MinusMinus)
    {
        expr->kind = Expr_Symbolic;
        expr->sexpr.symbol = token_to_symbol_kind(tok);
        expr->sexpr.count = 0;

        advance(com, tok);

        for (int i = 0; i < 2; i++)
        {
            Expr child;

            if (!parse_expr(com, tok, &child))
                break;

            if (child.kind == Expr_Invalid)
                error("Expected a literal but got %s", tok->lexeme)

            Expr* node = malloc(sizeof(Expr));
            *node = child;

            expr->sexpr.operands[expr->sexpr.count++] = node;
        }

        return 1;
    }
    else
    switch (tok->kind)
    {
        case Tk_IntLiteral:
            expr->kind = Expr_IntLeaf;
            expr->leaf.value = strdup(tok->lexeme);
            break;

        case Tk_FloatLiteral:
            expr->kind = Expr_FloatLeaf;
            expr->leaf.value = strdup(tok->lexeme);
            break;

        case Tk_StringLiteral:
            expr->kind = Expr_StringLeaf;
            expr->leaf.value = strdup(tok->lexeme);
            break;

        case Tk_Identifier:
            expr->kind = Expr_Identifier;
            expr->leaf.value = strdup(tok->lexeme);
            break;

        default:
            expr->kind = Expr_Invalid;
            return 1;
    }

    advance(com, tok);
    return 1;
}
