#ifndef TOKENKIND_H
#define TOKENKIND_H

enum TokenKind
{
    // Special tokens
    Tk_Unknown,
    Tk_EOF,

    // User defined
    Tk_TypeName,
    Tk_Identifier,

    // Literals
    Tk_IntLiteral,
    Tk_FloatLiteral,
    Tk_StringLiteral,

    // Keywords
    Tk_Typedef,
    Tk_Struct,
    Tk_Return,
    Tk_Function,
    Tk_If,
    Tk_Else,
    Tk_Break,
    Tk_Continue,

    // Operators
    Tk_Equals,
    Tk_Colon,
    Tk_ColonColon,
    Tk_BackTick,
    Tk_Period,
    Tk_Comma,
    Tk_SemiColon,
    Tk_DoubleBackSlash,
    Tk_BackSlash,
    Tk_QuestionMark,
    Tk_DoubleQuestionMark,
    Tk_AtSign,
    Tk_LeftArrow,
    Tk_RightArrow,
    Tk_FatArrow,

    // Arithmetic Operators
    Tk_Asterisk,
    Tk_NormalSlash,
    Tk_DoubleSlash,
    Tk_Modulus,
    Tk_Plus,
    Tk_Minus,
    Tk_PlusPlus,
    Tk_MinusMinus,

    // Comparison Operators
    Tk_LessThan,
    Tk_GreaterThan,
    Tk_LessOrEquals,
    Tk_GreaterOrEquals,
    Tk_EqualsEquals,
    Tk_ExclaimEquals,

    // Bitwise Operator
    Tk_Ambersand,
    Tk_VerticalBar,
    Tk_Caret,
    Tk_Tilde,
    Tk_LeftShift,
    Tk_RightShift,

    // Logical Operator
    Tk_ExclamationMark,
    Tk_DoubleVerticalBar,
    Tk_DoubleAmbersand,

    // Punctuators / Brackets
    Tk_OpenRoundBracket,
    Tk_CloseRoundBracket,
    Tk_OpenCurlyBracket,
    Tk_CloseCurlyBracket,
    Tk_OpenSquareBracket,
    Tk_CloseSquareBracket,

    TOKENS_LENGTH
};

#ifdef RE_DEBUG
char* tokenMap[TOKENS_LENGTH];
void initTokens()
{
    // Special tokens
    tokenMap[Tk_Unknown] = "Tk_Unknown";
    tokenMap[Tk_EOF] = "Tk_EOF";

    // User defined
    tokenMap[Tk_TypeName] = "Tk_TypeName";
    tokenMap[Tk_Identifier] = "Tk_Identifier";

    // Literals
    tokenMap[Tk_IntLiteral] = "Tk_IntLiteral";
    tokenMap[Tk_FloatLiteral] = "Tk_FloatLiteral";
    tokenMap[Tk_StringLiteral] = "Tk_StringLiteral";

    // Keywords
    tokenMap[Tk_Typedef] = "Tk_Typedef";
    tokenMap[Tk_Struct] = "Tk_Struct";
    tokenMap[Tk_Return] = "Tk_Return";
    tokenMap[Tk_Function] = "Tk_Function";
    tokenMap[Tk_If] = "Tk_If";
    tokenMap[Tk_Else] = "Tk_Else";
    tokenMap[Tk_Break] = "Tk_Break";
    tokenMap[Tk_Continue] = "Tk_Continue";

    // Operators
    tokenMap[Tk_Equals] = "Tk_Equals";
    tokenMap[Tk_Colon] = "Tk_Colon";
    tokenMap[Tk_ColonColon] = "Tk_ColonColon";
    tokenMap[Tk_BackTick] = "Tk_BackTick";
    tokenMap[Tk_Period] = "Tk_Period";
    tokenMap[Tk_Comma] = "Tk_Comma";
    tokenMap[Tk_SemiColon] = "Tk_SemiColon";
    tokenMap[Tk_DoubleBackSlash] = "Tk_DoubleBackSlash";
    tokenMap[Tk_BackSlash] = "Tk_BackSlash";
    tokenMap[Tk_QuestionMark] = "Tk_QuestionMark";
    tokenMap[Tk_DoubleQuestionMark] = "Tk_DoubleQuestionMark";
    tokenMap[Tk_AtSign] = "Tk_AtSign";
    tokenMap[Tk_LeftArrow] = "Tk_LeftArrow";
    tokenMap[Tk_RightArrow] = "Tk_RightArrow";
    tokenMap[Tk_FatArrow] = "Tk_FatArrow";

    // Arithmetic Operators
    tokenMap[Tk_Asterisk] = "Tk_Asterisk";
    tokenMap[Tk_NormalSlash] = "Tk_NormalSlash";
    tokenMap[Tk_DoubleSlash] = "Tk_DoubleSlash";
    tokenMap[Tk_Modulus] = "Tk_Modulus";
    tokenMap[Tk_Plus] = "Tk_Plus";
    tokenMap[Tk_Minus] = "Tk_Minus";
    tokenMap[Tk_PlusPlus] = "Tk_PlusPlus";
    tokenMap[Tk_MinusMinus] = "Tk_MinusMinus";

    // Comparison Operators
    tokenMap[Tk_LessThan] = "Tk_LessThan";
    tokenMap[Tk_GreaterThan] = "Tk_GreaterThan";
    tokenMap[Tk_LessOrEquals] = "Tk_LessOrEquals";
    tokenMap[Tk_GreaterOrEquals] = "Tk_GreaterOrEquals";
    tokenMap[Tk_EqualsEquals] = "Tk_EqualsEquals";
    tokenMap[Tk_ExclaimEquals] = "Tk_ExclaimEquals";

    // Bitwise Operators
    tokenMap[Tk_Ambersand] = "Tk_Ambersand";
    tokenMap[Tk_VerticalBar] = "Tk_VerticalBar";
    tokenMap[Tk_Caret] = "Tk_Caret";
    tokenMap[Tk_Tilde] = "Tk_Tilde";
    tokenMap[Tk_LeftShift] = "Tk_LeftShift";
    tokenMap[Tk_RightShift] = "Tk_RightShift";

    // Logical Operators
    tokenMap[Tk_ExclamationMark] = "Tk_ExclamationMark";
    tokenMap[Tk_DoubleVerticalBar] = "Tk_DoubleVerticalBar";
    tokenMap[Tk_DoubleAmbersand] = "Tk_DoubleAmbersand";

    // Punctuators / Brackets
    tokenMap[Tk_OpenRoundBracket] = "Tk_OpenRoundBracket";
    tokenMap[Tk_CloseRoundBracket] = "Tk_CloseRoundBracket";
    tokenMap[Tk_OpenCurlyBracket] = "Tk_OpenCurlyBracket";
    tokenMap[Tk_CloseCurlyBracket] = "Tk_CloseCurlyBracket";
    tokenMap[Tk_OpenSquareBracket] = "Tk_OpenSquareBracket";
    tokenMap[Tk_CloseSquareBracket] = "Tk_CloseSquareBracket";
}
#endif // RE_DEBUG
#endif // TOKENKIND_H