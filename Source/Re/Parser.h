#ifndef PARSER_H
#define PARSER_H
#include "Compiler.h"
#include "Expr.h"
#include "Token.h"
#include "Compiler.h"

enum StatementKind {
    Stmt_UnitDeclaration,
    Stmt_Expression,
};

struct _UnitDeclaration {
    bool isconst;
    int type;
    char* name;
    Expr* expr;
};
typedef struct _UnitDeclaration UnitDeclStmt;

struct _ExprStatement {
    Expr* expr;
};
typedef struct _ExprStatement ExprStmt;

struct _Statement {
    enum StatementKind kind;
    union {
        UnitDeclStmt unit_decl;
        ExprStmt expr_stmt;
    };
};
typedef struct _Statement Stmt;

void print_expr(Expr* expr, int tab_size);
void print_stmt(Compiler* com, Stmt* stmt);

int parse(Compiler* com, Token* tok, Stmt* stmt);
int parse_expr(Compiler* com, Token* tok, Expr* expr);


#endif // PARSER_H