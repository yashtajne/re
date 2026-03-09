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
    Expr_FunctionCall,

    EXPR_LENGTH
};

enum SymbolKind {
    SK_Unknown,

    // Arithmetic
    SK_Multiply,
    SK_Divide,
    SK_Modulus,
    SK_Positive,
    SK_Negation,

    // Comparison
    SK_LessThan,
    SK_GreaterThan,
    SK_LessOrEquals,
    SK_GreaterOrEquals,
    SK_EqualsEquals,
    SK_NotEquals,

    // Logical
    SK_LogicalAnd,
    SK_LogicalOr,

    // Bitwise
    SK_BitwiseAnd,
    SK_BitwiseOr,

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
    expressionsMap[Expr_FunctionCall] = "Expr_FunctionCall";
}
void initSymbols()
{
    symbolsMap[SK_Unknown][0]     = "PK_Unknown";
    symbolsMap[SK_Unknown][1]     = "Unknown";

    // Arithmetic
    symbolsMap[SK_Multiply][0]    = "PK_Multiply";
    symbolsMap[SK_Multiply][1]    = "*";

    symbolsMap[SK_Divide][0]      = "PK_Divide";
    symbolsMap[SK_Divide][1]      = "/";

    symbolsMap[SK_Modulus][0]     = "PK_Modulus";
    symbolsMap[SK_Modulus][1]     = "%";

    symbolsMap[SK_Positive][0]    = "PK_Positive";
    symbolsMap[SK_Positive][1]    = "+";

    symbolsMap[SK_Negation][0]    = "PK_Negation";
    symbolsMap[SK_Negation][1]    = "-";

    // Comparison
    symbolsMap[SK_LessThan][0]    = "PK_LessThan";
    symbolsMap[SK_LessThan][1]    = "<";

    symbolsMap[SK_GreaterThan][0] = "PK_GreaterThan";
    symbolsMap[SK_GreaterThan][1] = ">";

    symbolsMap[SK_LessOrEquals][0] = "PK_LessOrEquals";
    symbolsMap[SK_LessOrEquals][1] = "<=";

    symbolsMap[SK_GreaterOrEquals][0] = "PK_GreaterOrEquals";
    symbolsMap[SK_GreaterOrEquals][1] = ">=";

    symbolsMap[SK_EqualsEquals][0] = "PK_EqualsEquals";
    symbolsMap[SK_EqualsEquals][1] = "==";

    symbolsMap[SK_NotEquals][0]   = "PK_NotEquals";
    symbolsMap[SK_NotEquals][1]   = "!=";

    // Logical
    symbolsMap[SK_LogicalAnd][0]  = "PK_LogicalAnd";
    symbolsMap[SK_LogicalAnd][1]  = "&&";

    symbolsMap[SK_LogicalOr][0]   = "PK_LogicalOr";
    symbolsMap[SK_LogicalOr][1]   = "||";

    // Bitwise
    symbolsMap[SK_BitwiseAnd][0]  = "PK_BitwiseAnd";
    symbolsMap[SK_BitwiseAnd][1]  = "&";

    symbolsMap[SK_BitwiseOr][0]   = "PK_BitwiseOr";
    symbolsMap[SK_BitwiseOr][1]   = "|";
}
#endif // RE_DEBUG

typedef struct _Expression Expr;
typedef struct { char* value; } LeafExpr;

typedef struct {
    enum SymbolKind symbol;
    int count;
    Expr* operands[2];
} SymbolicExpr;

typedef struct {
    char* name;
    int count;
    Expr* args[16];
} CallExpr;

struct _Expression {
    enum ExprKind kind;
    union {
        LeafExpr leaf;
        SymbolicExpr sexpr;
        CallExpr call;
    };
};

#endif // EXPR_H
