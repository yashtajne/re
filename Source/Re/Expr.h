#ifndef EXPR_H
#define EXPR_H
#include "TokenKind.h"
// #include "ExprKind.h"

enum ExprKind {
    Expr_Invalid,
    Expr_Identifier,

    Expr_Int,
    Expr_Float,
    Expr_String,

    Expr_Prefix,
    Expr_Suffix,

    EXPR_LENGTH
};

enum PrefixKind {
    PK_Unknown,

    // Arithmetic
    PK_Positive,
    PK_Negation,
    PK_Increment,
    PK_Decrement,

    // Logical
    PK_LogicalNot,

    // Bitwise
    PK_BitwiseNot,

    PREFIX_LENGTH
};

extern char* expressionsMap[EXPR_LENGTH];
extern char* prefixesMap[PREFIX_LENGTH][2];

#ifdef RE_DEBUG
char* expressionsMap[EXPR_LENGTH];
char* prefixesMap[PREFIX_LENGTH][2];
void initExpressions()
{
    expressionsMap[Expr_Invalid] = "Expr_Invalid";
    expressionsMap[Expr_Identifier] = "Expr_Identifier";

    expressionsMap[Expr_Int] = "Expr_Int";
    expressionsMap[Expr_Float] = "Expr_Float";
    expressionsMap[Expr_String] = "Expr_String";

    expressionsMap[Expr_Prefix] = "Expr_Prefix";
    expressionsMap[Expr_Suffix] = "Expr_Suffix";
}
void initPrefixes()
{
    prefixesMap[PK_Unknown][0]     = "PK_Unknown";
    prefixesMap[PK_Unknown][1]     = "Unknown";

    // Arithmetic
    prefixesMap[PK_Positive][0]    = "PK_Positive";
    prefixesMap[PK_Positive][1]    = "+";

    prefixesMap[PK_Negation][0]    = "PK_Negation";
    prefixesMap[PK_Negation][1]    = "-";

    prefixesMap[PK_Increment][0]   = "PK_Increment";
    prefixesMap[PK_Increment][1]   = "++";

    prefixesMap[PK_Decrement][0]   = "PK_Decrement";
    prefixesMap[PK_Decrement][1]   = "--";

    // Logical
    prefixesMap[PK_LogicalNot][0]  = "PK_LogicalNot";
    prefixesMap[PK_LogicalNot][1]  = "!";

    // Bitwise
    prefixesMap[PK_BitwiseNot][0]  = "PK_BitwiseNot";
    prefixesMap[PK_BitwiseNot][1]  = "~";
}
#endif // RE_DEBUG

typedef struct _Expr Expr;
typedef struct { char* value; } LeafExpr;

typedef struct {
    enum PrefixKind kind;
    Expr* expr;
} PrefixExpr;

struct _Expr {
    enum ExprKind kind;
    union {
        LeafExpr leaf;
        PrefixExpr prefix;
    };
};

#endif // EXPR_H