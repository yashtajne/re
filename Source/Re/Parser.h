#ifndef PARSER_H
#define PARSER_H
#include "Compiler.h"
#include "Expr.h"
#include "Token.h"
#include "Compiler.h"

enum StatementKind {
    Stmt_Invalid,
    Stmt_UnitDeclaration,
    Stmt_Expression,
    Stmt_CompoundExpression,
    Stmt_FunctionDeclaration,
    Stmt_Return,
    Stmt_IfElse,
};
typedef struct _Statement Stmt;

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

struct _TupleItem {
    int type;
    char* name;
};
typedef struct _TupleItem TupleItem;

struct _CompoundStatement {
    char* locals;
    Stmt* statements;
    int locals_len, statements_len;
};
typedef struct _CompoundStatement CompoundStmt;

struct _FunctionStmt {
    int return_type;
    char* name;
    TupleItem params[16];
    int params_len;
    CompoundStmt body;
};
typedef struct _FunctionStmt FuncStmt;

struct _ReturnStatement {
    Expr* expr;
};
typedef struct _ReturnStatement ReturnStmt;

struct _IfElseStatement {
    Expr* condition;
    CompoundStmt then_block;
    bool has_else;
    CompoundStmt else_block;
};
typedef struct _IfElseStatement IfElseStmt;

struct _Statement {
    enum StatementKind kind;
    union {
        UnitDeclStmt unit_decl;
        ExprStmt expr_stmt;
        CompoundStmt block_stmt;
        FuncStmt func_stmt;
        ReturnStmt return_stmt;
        IfElseStmt if_stmt;
    };
};

void print_expr(Expr* expr, int tab_size);
void print_stmt(Compiler* com, Stmt* stmt);

int parse(Compiler* com, Token* tok, Stmt* stmt);
int parse_expr(Compiler* com, Token* tok, Expr* expr);
int parse_sexpr(Compiler* com, Token* tok, Expr* expr);
int parse_tschema(Compiler* com, Token* tok, TupleItem* tuple, int* tuple_len);
void emit_c_program(FILE* out, Compiler* com, Stmt* statements, int statements_len);

#endif // PARSER_H
