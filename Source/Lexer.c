#include "Re/Lexer.h"

#define red   "\033[31m"    // \033[1;31m
#define reset "\033[0m"

#define cur  com->cur
#define file com->file
#define cur_getc() { cur = fgetc(file); com->col++; }
#define cur_ungetc() { cur = (ungetc(cur, file)); com->col--; }

#define getrow() com->row
#define getcol() com->col

#define max_buffer_length 16
#define cur_newline() if (cur == '\n') { com->row++; com->col = 1; }
#define bufwrite(_string_) { strcpy(buffer, _string_); }
#define print(_preprocessor_) { printf("[ %2d:%-2d ] %26s  #%-16s \n", getrow() - 1, getcol(), "Preprocessor", _preprocessor_); }

#define peek(_dothis_) \
{ \
    int next = fgetc(file); \
    _dothis_\
    ungetc(next, file); \
}

#define token(_kind_) \
{ \
    tok->kind = _kind_; \
    tok->row = getrow(); \
    tok->col = getcol(); \
    strcpy(tok->lexeme, buffer); \
    return 1; \
}

#define error(_message_) \
{ \
    printf("[%2d:%-2d ] "red"lexical error:"reset" %s\n", getrow(), getcol(), _message_); \
    exit(1); \
}


int next(Compiler* com, Token* tok)
{
    int len;
    char buffer[max_buffer_length];
    for (;;)
    {
        len = 0;
        memset(buffer,0,sizeof(buffer));

        cur_getc();
        if (cur == EOF) token(Tk_EOF);

        while (isspace(cur))
        {
            cur_newline();
            cur_getc();
        }

        if (cur == '#')
        {
            cur_getc();
            if (isalpha(cur))
            {
                while (isalpha(cur))
                {
                    if (len > max_buffer_length) error("Preprocessor too long!?")
                    buffer[len++] = cur;
                    cur_getc();
                    cur_newline();
                }
                buffer[len] = '\0';
                if (0 == strcmp(buffer, "import")) print("import") else
                if (0 == strcmp(buffer, "if")) print("if") else
                if (0 == strcmp(buffer, "end")) print("end") else
                error("Invalid Preprocessor!? Or you may have forgot to put a space for comment.")
                continue;
            }
            else
            if (isspace(cur))
            {
                while(cur != '\n' && cur != EOF)
                    cur_getc();
                cur_newline();
                continue;
            }
            else
            if (cur == '#')
            {
                int prev = cur;
                cur_getc();
                while (!(prev == '#' && cur == '#'))
                {
                    cur_newline();
                    prev = cur;
                    cur_getc();
                }
                continue;
            }
        }

        if (isalpha(cur) || cur == '_')
        {
            do
            {
                if (len >= max_buffer_length-1) error("Identifier too long");
                buffer[len++] = cur;
                cur_getc();
            }
            while (isalnum(cur) || cur == '_');
            cur_ungetc();

            buffer[len] = '\0';

            if (0==strcmp(buffer, "return")) token(Tk_Return) else
            if (0==strcmp(buffer, "if")) token(Tk_If) else
            token(Tk_Identifier)
        }

        else
        if (isdigit(cur))
        {
            do
            {
                if (len >= max_buffer_length - 1) error("Digit too long");
                buffer[len++] = cur;
                cur_getc();
            }
            while (isdigit(cur));
            buffer[len] = '\0';
            token(Tk_IntLiteral)
        }

        else
        {
            switch (cur)
            {
                case '(': { bufwrite("(") token(Tk_OpenRoundBracket) }
                case ')': { bufwrite(")") token(Tk_CloseRoundBracket) }
                case '{': { bufwrite("{") token(Tk_OpenCurlyBracket) }
                case '}': { bufwrite("}") token(Tk_CloseCurlyBracket) }
                case '[': { bufwrite("[") token(Tk_OpenSquareBracket) }
                case ']': { bufwrite("]") token(Tk_CloseSquareBracket) }

                case ',': { bufwrite(",") token(Tk_Comma) }
                case ';': { bufwrite(";") token(Tk_SemiColon) }
                case '.': { bufwrite(".") token(Tk_Period) }

                case '%': { bufwrite("%") token(Tk_Modulus) }
                case '*': { bufwrite("*") token(Tk_Asterisk) }
                case '+':
                {
                    peek(if (next == '+') { bufwrite("++") token(Tk_PlusPlus) })
                    bufwrite("+") token(Tk_Plus)
                }
                case '-':
                {
                    peek(if (next == '-') { bufwrite("--") token(Tk_MinusMinus) })
                    bufwrite("-") token(Tk_Minus)
                }
                case '/':
                {
                    peek(if (next == '/') { bufwrite("//") token(Tk_DoubleSlash) })
                    bufwrite("/") token(Tk_NormalSlash)
                }

                default:
                    buffer[0] = cur;
                    buffer[1] = '\0';
                    cur_getc();
                    token(Tk_Unknown)
            }
        }
    }
}