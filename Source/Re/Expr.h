#ifndef EXPR_H
#define EXPR_H
#include "TokenKind.h"
// #include "ExprKind.h"

enum ExprKind {
    Expr_Invalid,
    Expr_Identifier,

    Expr_IntLeaf,
    Expr_FloatLeaf,
    Expr_StringLeaf,

    Expr_Symbolic,

    EXPR_LENGTH
};

enum SymbolKind {
    SK_Unknown,

    // Arithmetic
    SK_Positive,
    SK_Negation,
    SK_Increment,
    SK_Decrement,

    // Logical
    SK_LogicalNot,

    // Bitwise
    SK_BitwiseNot,

    SYMBOL_LENGTH
};

extern char* expressionsMap[EXPR_LENGTH];
extern char* symbolsMap[SYMBOL_LENGTH][2];

#ifdef RE_DEBUG
char* expressionsMap[EXPR_LENGTH];
char* symbolsMap[SYMBOL_LENGTH][2];
void initExpressions()
{
    expressionsMap[Expr_Invalid] = "Expr_Invalid";
    expressionsMap[Expr_Identifier] = "Expr_Identifier";

    expressionsMap[Expr_IntLeaf] = "Expr_IntLeaf";
    expressionsMap[Expr_FloatLeaf] = "Expr_FloatLeaf";
    expressionsMap[Expr_StringLeaf] = "Expr_StringLeaf";

    expressionsMap[Expr_Symbolic] = "Expr_Symbolic";
}
void initSymbols()
{
    symbolsMap[SK_Unknown][0]     = "PK_Unknown";
    symbolsMap[SK_Unknown][1]     = "Unknown";

    // Arithmetic
    symbolsMap[SK_Positive][0]    = "PK_Positive";
    symbolsMap[SK_Positive][1]    = "+";

    symbolsMap[SK_Negation][0]    = "PK_Negation";
    symbolsMap[SK_Negation][1]    = "-";

    symbolsMap[SK_Increment][0]   = "PK_Increment";
    symbolsMap[SK_Increment][1]   = "++";

    symbolsMap[SK_Decrement][0]   = "PK_Decrement";
    symbolsMap[SK_Decrement][1]   = "--";

    // Logical
    symbolsMap[SK_LogicalNot][0]  = "PK_LogicalNot";
    symbolsMap[SK_LogicalNot][1]  = "!";

    // Bitwise
    symbolsMap[SK_BitwiseNot][0]  = "PK_BitwiseNot";
    symbolsMap[SK_BitwiseNot][1]  = "~";
}
#endif // RE_DEBUG

typedef struct _Expression Expr;
typedef struct { char* value; } LeafExpr;

typedef struct {
    enum SymbolKind symbol;
    int count;
    Expr* operands[2];
} SymbolicExpr;

struct _Expression {
    enum ExprKind kind;
    union {
        LeafExpr leaf;
        SymbolicExpr sexpr;
    };
};

#endif // EXPR_H