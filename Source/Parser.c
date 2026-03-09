#include "Re/Colors.h"
#include "Re/Parser.h"
#include "Re/Lexer.h"
#include "Re/Expr.h"
#include "Re/TokenKind.h"
#include <stdlib.h>

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
        case Tk_Asterisk: return SK_Multiply;
        case Tk_NormalSlash: return SK_Divide;
        case Tk_Modulus: return SK_Modulus;
        case Tk_Plus: return SK_Positive;
        case Tk_Minus: return SK_Negation;
        case Tk_LessThan: return SK_LessThan;
        case Tk_GreaterThan: return SK_GreaterThan;
        case Tk_LessOrEquals: return SK_LessOrEquals;
        case Tk_GreaterOrEquals: return SK_GreaterOrEquals;
        case Tk_EqualsEquals: return SK_EqualsEquals;
        case Tk_ExclaimEquals: return SK_NotEquals;
        case Tk_DoubleAmbersand: return SK_LogicalAnd;
        case Tk_DoubleVerticalBar: return SK_LogicalOr;
        case Tk_Ambersand: return SK_BitwiseAnd;
        case Tk_VerticalBar: return SK_BitwiseOr;
        default: return SK_Unknown;
    }
}

static int is_binary_operator(Token* tok)
{
    switch (tok->kind)
    {
        case Tk_Asterisk:
        case Tk_NormalSlash:
        case Tk_Modulus:
        case Tk_Plus:
        case Tk_Minus:
        case Tk_LessThan:
        case Tk_GreaterThan:
        case Tk_LessOrEquals:
        case Tk_GreaterOrEquals:
        case Tk_EqualsEquals:
        case Tk_ExclaimEquals:
        case Tk_DoubleAmbersand:
        case Tk_DoubleVerticalBar:
        case Tk_Ambersand:
        case Tk_VerticalBar:
            return 1;
        default:
            return 0;
    }
}

static int parse_block(Compiler* com, Token* tok, CompoundStmt* block)
{
    if (tok->kind != Tk_OpenCurlyBracket)
        error("Expected { but got %s", tok->lexeme);

    block->statements = NULL;
    block->statements_len = 0;
    block->locals = NULL;
    block->locals_len = 0;

    advance(com, tok);

    while (tok->kind != Tk_CloseCurlyBracket)
    {
        if (tok->kind == Tk_EOF)
            error("Expected } but got %s", tok->lexeme);

        block->statements = realloc(block->statements, sizeof(Stmt) * (block->statements_len + 1));
        if (!parse(com, tok, &block->statements[block->statements_len]))
            error("Failed to parse statement in block near %s", tok->lexeme);
        block->statements_len++;
    }

    advance(com, tok);
    return 1;
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
    if (expr->kind == Expr_FunctionCall)
    {
        printf("[ Expr_FunctionCall ] %s\n", expr->call.name);
        for (int i = 0; i < expr->call.count; i++)
            print_expr(expr->call.args[i], tabs + 1);
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
    if (stmt->kind == Stmt_CompoundExpression)
    {
        printf("\n; Stmt_CompoundExpression \n");
        printf("  - Statements %d\n", stmt->block_stmt.statements_len);
        for (int i = 0; i < stmt->block_stmt.statements_len; i++)
            print_stmt(com, &stmt->block_stmt.statements[i]);
    }
    else
    if (stmt->kind == Stmt_FunctionDeclaration)
    {
        printf("\n; Stmt_FunctionDeclaration \n");
        printf("  - Return Type %s\n", com->types[stmt->func_stmt.return_type]);
        printf("  - Name %s\n", stmt->func_stmt.name);
        printf("  - Params %d\n", stmt->func_stmt.params_len);
        for (int i = 0; i < stmt->func_stmt.params_len; i++)
            printf("    * %s %s\n",
                   com->types[stmt->func_stmt.params[i].type],
                   stmt->func_stmt.params[i].name);
        printf("  - Body Statements %d\n", stmt->func_stmt.body.statements_len);
        for (int i = 0; i < stmt->func_stmt.body.statements_len; i++)
            print_stmt(com, &stmt->func_stmt.body.statements[i]);
    }
    else
    if (stmt->kind == Stmt_Return)
    {
        printf("\n; Stmt_Return \n");
        print_expr(stmt->return_stmt.expr, 0);
    }
    else
    if (stmt->kind == Stmt_IfElse)
    {
        printf("\n; Stmt_IfElse \n");
        printf("  - Condition\n");
        print_expr(stmt->if_stmt.condition, 1);
        printf("  - Then (%d)\n", stmt->if_stmt.then_block.statements_len);
        for (int i = 0; i < stmt->if_stmt.then_block.statements_len; i++)
            print_stmt(com, &stmt->if_stmt.then_block.statements[i]);
        if (stmt->if_stmt.has_else)
        {
            printf("  - Else (%d)\n", stmt->if_stmt.else_block.statements_len);
            for (int i = 0; i < stmt->if_stmt.else_block.statements_len; i++)
                print_stmt(com, &stmt->if_stmt.else_block.statements[i]);
        }
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
    if (tok->kind == Tk_CloseCurlyBracket)
        return 0;

    switch (tok->kind)
    {
        case Tk_TypeName:
        {
            int typename = -1;
            is_type(com, tok->lexeme, &typename);
            advance(com, tok);
            if (tok->kind == Tk_Identifier)
            {
                char* name = strdup(tok->lexeme);
                advance(com, tok);

                if (tok->kind == Tk_OpenRoundBracket)
                {
                    stmt->kind = Stmt_FunctionDeclaration;
                    stmt->func_stmt.return_type = typename;
                    stmt->func_stmt.name = name;

                    if (!parse_tschema(com, tok, stmt->func_stmt.params, &stmt->func_stmt.params_len))
                        error("Invalid tuple schema near %s", tok->lexeme);

                    AddFunction(com, stmt->func_stmt.name, stmt->func_stmt.params_len);

                    if (tok->kind != Tk_OpenCurlyBracket)
                        error("Expected { after function declaration but got %s", tok->lexeme);
                    parse_block(com, tok, &stmt->func_stmt.body);

                    while (tok->kind == Tk_SemiColon)
                        advance(com, tok);
                    return 1;
                }

                stmt->kind = Stmt_UnitDeclaration;
                stmt->unit_decl.type = typename;
                stmt->unit_decl.name = name;
                alloc_expr(expr);
                if (!parse_expr(com, tok, expr))
                    error("Expected an expression after declaration but got %s", tok->lexeme);
                stmt->unit_decl.expr = expr;

                if (is_unit(com, stmt->unit_decl.name))
                {
                    error("Redefinition of unit %s", stmt->unit_decl.name);
                }
                else
                    AddUnit(com, stmt->unit_decl.name);

                while (tok->kind == Tk_SemiColon)
                    advance(com, tok);
                return 1;
            } else error("Expected an Identifier but got %s", tok->lexeme);
            break;
        }
        case Tk_OpenCurlyBracket:
            stmt->kind = Stmt_CompoundExpression;
            parse_block(com, tok, &stmt->block_stmt);

            while (tok->kind == Tk_SemiColon)
                advance(com, tok);
            return 1;
        case Tk_Return:
            stmt->kind = Stmt_Return;
            advance(com, tok);
            alloc_expr(rexpr);
            if (!parse_expr(com, tok, rexpr))
                error("Expected an expression after return but got %s", tok->lexeme);
            stmt->return_stmt.expr = rexpr;

            while (tok->kind == Tk_SemiColon)
                advance(com, tok);
            return 1;
        case Tk_If:
            stmt->kind = Stmt_IfElse;
            advance(com, tok);
            alloc_expr(cond);
            if (!parse_expr(com, tok, cond))
                error("Expected condition expression after if but got %s", tok->lexeme);
            stmt->if_stmt.condition = cond;

            if (tok->kind != Tk_OpenCurlyBracket)
                error("Expected { after if condition but got %s", tok->lexeme);
            parse_block(com, tok, &stmt->if_stmt.then_block);

            stmt->if_stmt.has_else = false;
            if (tok->kind == Tk_Else)
            {
                stmt->if_stmt.has_else = true;
                advance(com, tok);
                if (tok->kind != Tk_OpenCurlyBracket)
                    error("Expected { after else but got %s", tok->lexeme);
                parse_block(com, tok, &stmt->if_stmt.else_block);
            }

            while (tok->kind == Tk_SemiColon)
                advance(com, tok);
            return 1;
        default: {
            stmt->kind = Stmt_Expression;
            alloc_expr(expr);
            if (!parse_expr(com, tok, expr)) {
                free(expr);
                break;
            }
            if (expr->kind == Expr_Invalid)
                error("Invalid expression near %s", tok->lexeme);
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
    switch (tok->kind) {
        case Tk_EOF:
        case Tk_CloseCurlyBracket:
        case Tk_SemiColon:
            return 0;
        default: break;
    }
    return parse_sexpr(com, tok, expr);
}

int parse_tschema(Compiler* com, Token* tok, TupleItem* tuple, int* tuple_len)
{
    int i = 0;
    if (tok->kind != Tk_OpenRoundBracket)
        error("Expected ( but got %s", tok->lexeme);

    advance(com, tok);

    if (tok->kind == Tk_CloseRoundBracket)
    {
        *tuple_len = 0;
        advance(com, tok);
        return 1;
    }

    for (;;)
    {
        if (tok->kind != Tk_TypeName)
            error("Expected a typename but got %s", tok->lexeme);
        if (!is_type(com, tok->lexeme, &tuple[i].type))
            error("Expected a typename, %s is not a type", tok->lexeme);

        advance(com, tok);
        if (tok->kind != Tk_Identifier)
            error("Expected an Identifier but got %s", tok->lexeme);
        tuple[i].name = strdup(tok->lexeme);
        i++;

        if (i >= 16)
            error("Too many function parameters near %s", tok->lexeme);

        advance(com, tok);
        if (tok->kind == Tk_CloseRoundBracket) {
            *tuple_len = i;
            advance(com, tok);
            break;
        }
        if (tok->kind != Tk_Comma)
            error("Expected a , but got %s", tok->lexeme);
        advance(com, tok);
    }
    return 1;
}

int parse_sexpr(Compiler* com, Token* tok, Expr* expr)
{
    if (is_binary_operator(tok))
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
        {
            int params_len = 0;
            if (is_function(com, tok->lexeme, &params_len))
            {
                expr->kind = Expr_FunctionCall;
                expr->call.name = strdup(tok->lexeme);
                expr->call.count = 0;
                advance(com, tok);

                for (int i = 0; i < params_len; i++)
                {
                    Expr arg;
                    if (!parse_expr(com, tok, &arg))
                    {
                        printf("[%2d:%-2d ] "red"error: "reset"Expected argument %d for function %s near %s\n",
                               getrow(), getcol(), i + 1, expr->call.name, tok->lexeme);
                        exit(1);
                    }
                    Expr* node = malloc(sizeof(Expr));
                    *node = arg;
                    expr->call.args[expr->call.count++] = node;
                }
            }
            else
            {
                expr->kind = Expr_Identifier;
                expr->leaf.value = strdup(tok->lexeme);
                advance(com, tok);
            }
            return 1;
        }

        default:
            return 0;
    }

    advance(com, tok);
    return 1;
}

static void emit_indent(FILE* out, int depth)
{
    for (int i = 0; i < depth; i++) fputs("    ", out);
}

static void emit_c_expr(FILE* out, Expr* expr)
{
    if (expr->kind == Expr_Identifier ||
        expr->kind == Expr_IntLeaf ||
        expr->kind == Expr_FloatLeaf)
    {
        fprintf(out, "%s", expr->leaf.value);
        return;
    }

    if (expr->kind == Expr_StringLeaf)
    {
        fprintf(out, "\"%s\"", expr->leaf.value);
        return;
    }

    if (expr->kind == Expr_Symbolic && expr->sexpr.count == 2)
    {
        fprintf(out, "(");
        emit_c_expr(out, expr->sexpr.operands[0]);
        fprintf(out, " %s ", symbolsMap[expr->sexpr.symbol][1]);
        emit_c_expr(out, expr->sexpr.operands[1]);
        fprintf(out, ")");
        return;
    }

    if (expr->kind == Expr_FunctionCall)
    {
        fprintf(out, "%s(", expr->call.name);
        for (int i = 0; i < expr->call.count; i++)
        {
            if (i > 0) fputs(", ", out);
            emit_c_expr(out, expr->call.args[i]);
        }
        fputs(")", out);
        return;
    }

    fprintf(out, "0");
}

static void emit_c_stmt(FILE* out, Compiler* com, Stmt* stmt, int depth);

static void emit_c_block(FILE* out, Compiler* com, CompoundStmt* block, int depth)
{
    emit_indent(out, depth);
    fputs("{\n", out);
    for (int i = 0; i < block->statements_len; i++)
        emit_c_stmt(out, com, &block->statements[i], depth + 1);
    emit_indent(out, depth);
    fputs("}\n", out);
}

static void emit_c_stmt(FILE* out, Compiler* com, Stmt* stmt, int depth)
{
    switch (stmt->kind)
    {
        case Stmt_UnitDeclaration:
            emit_indent(out, depth);
            fprintf(out, "%s %s = ", com->types[stmt->unit_decl.type], stmt->unit_decl.name);
            emit_c_expr(out, stmt->unit_decl.expr);
            fputs(";\n", out);
            return;
        case Stmt_Expression:
            emit_indent(out, depth);
            emit_c_expr(out, stmt->expr_stmt.expr);
            fputs(";\n", out);
            return;
        case Stmt_Return:
            emit_indent(out, depth);
            fputs("return ", out);
            emit_c_expr(out, stmt->return_stmt.expr);
            fputs(";\n", out);
            return;
        case Stmt_CompoundExpression:
            emit_c_block(out, com, &stmt->block_stmt, depth);
            return;
        case Stmt_IfElse:
            emit_indent(out, depth);
            fputs("if (", out);
            emit_c_expr(out, stmt->if_stmt.condition);
            fputs(")\n", out);
            emit_c_block(out, com, &stmt->if_stmt.then_block, depth);
            if (stmt->if_stmt.has_else)
            {
                emit_indent(out, depth);
                fputs("else\n", out);
                emit_c_block(out, com, &stmt->if_stmt.else_block, depth);
            }
            return;
        case Stmt_FunctionDeclaration:
            emit_indent(out, depth);
            fprintf(out, "%s %s(", com->types[stmt->func_stmt.return_type], stmt->func_stmt.name);
            for (int i = 0; i < stmt->func_stmt.params_len; i++)
            {
                if (i > 0) fputs(", ", out);
                fprintf(out, "%s %s",
                        com->types[stmt->func_stmt.params[i].type],
                        stmt->func_stmt.params[i].name);
            }
            fputs(")\n", out);
            emit_c_block(out, com, &stmt->func_stmt.body, depth);
            fputs("\n", out);
            return;
        default:
            return;
    }
}

static void emit_c_global_decl(FILE* out, Compiler* com, Stmt* stmt)
{
    fprintf(out, "%s %s;\n", com->types[stmt->unit_decl.type], stmt->unit_decl.name);
}

static void emit_c_global_init_assignment(FILE* out, Stmt* stmt, int depth)
{
    emit_indent(out, depth);
    fprintf(out, "%s = ", stmt->unit_decl.name);
    emit_c_expr(out, stmt->unit_decl.expr);
    fputs(";\n", out);
}

void emit_c_program(FILE* out, Compiler* com, Stmt* statements, int statements_len)
{
    int has_main = 0;
    int has_printable_global = 0;
    int has_global_units = 0;
    int has_top_level_exec = 0;

    fputs("#include <stdio.h>\n\n", out);
    for (int i = 0; i < statements_len; i++)
    {
        if (statements[i].kind == Stmt_FunctionDeclaration &&
            0 == strcmp(statements[i].func_stmt.name, "main"))
        {
            has_main = 1;
        }

        if (statements[i].kind == Stmt_UnitDeclaration)
        {
            char* tname = com->types[statements[i].unit_decl.type];
            if ((0 == strcmp(tname, "int")) || (0 == strcmp(tname, "float")))
                has_printable_global = 1;
            has_global_units = 1;
            emit_c_global_decl(out, com, &statements[i]);
            continue;
        }

        if (statements[i].kind == Stmt_FunctionDeclaration)
        {
            emit_c_stmt(out, com, &statements[i], 0);
            continue;
        }

        has_top_level_exec = 1;
    }

    if (has_global_units)
    {
        fputs("\nstatic void __re_init_globals(void)\n{\n", out);
        for (int i = 0; i < statements_len; i++)
        {
            if (statements[i].kind != Stmt_UnitDeclaration)
                continue;
            emit_c_global_init_assignment(out, &statements[i], 1);
        }
        fputs("}\n\n", out);
    }

    if (has_top_level_exec)
    {
        fputs("static void __re_run_top_level(void)\n{\n", out);
        for (int i = 0; i < statements_len; i++)
        {
            if (statements[i].kind == Stmt_FunctionDeclaration ||
                statements[i].kind == Stmt_UnitDeclaration)
                continue;
            if (statements[i].kind == Stmt_Return)
                continue;
            emit_c_stmt(out, com, &statements[i], 1);
        }
        fputs("}\n\n", out);
    }

    if (has_global_units || has_top_level_exec)
    {
        fputs("__attribute__((constructor)) static void __re_ctor(void)\n{\n", out);
        if (has_global_units) fputs("    __re_init_globals();\n", out);
        if (has_top_level_exec) fputs("    __re_run_top_level();\n", out);
        fputs("}\n\n", out);
    }

    if (!has_main)
    {
        fputs("int main(void)\n{\n", out);
        if (has_printable_global)
        {
            for (int i = 0; i < statements_len; i++)
            {
                if (statements[i].kind != Stmt_UnitDeclaration)
                    continue;

                char* tname = com->types[statements[i].unit_decl.type];
                if (0 == strcmp(tname, "int"))
                {
                    fprintf(out, "    printf(\"%s = %%d\\n\", %s);\n",
                            statements[i].unit_decl.name,
                            statements[i].unit_decl.name);
                }
                else if (0 == strcmp(tname, "float"))
                {
                    fprintf(out, "    printf(\"%s = %%f\\n\", %s);\n",
                            statements[i].unit_decl.name,
                            statements[i].unit_decl.name);
                }
            }
        }
        else
        {
            fputs("    printf(\"Generated program ran.\\n\");\n", out);
        }
        fputs("    return 0;\n}\n", out);
    }
}
