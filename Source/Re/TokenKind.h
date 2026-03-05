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

extern char* tokensMap[TOKENS_LENGTH];

#ifdef RE_DEBUG
char* tokensMap[TOKENS_LENGTH];
void initTokens()
{
    // Special tokens
    tokensMap[Tk_Unknown] = "Tk_Unknown";
    tokensMap[Tk_EOF] = "Tk_EOF";

    // User defined
    tokensMap[Tk_TypeName] = "Tk_TypeName";
    tokensMap[Tk_Identifier] = "Tk_Identifier";

    // Literals
    tokensMap[Tk_IntLiteral] = "Tk_IntLiteral";
    tokensMap[Tk_FloatLiteral] = "Tk_FloatLiteral";
    tokensMap[Tk_StringLiteral] = "Tk_StringLiteral";

    // Keywords
    tokensMap[Tk_Typedef] = "Tk_Typedef";
    tokensMap[Tk_Struct] = "Tk_Struct";
    tokensMap[Tk_Return] = "Tk_Return";
    tokensMap[Tk_Function] = "Tk_Function";
    tokensMap[Tk_If] = "Tk_If";
    tokensMap[Tk_Else] = "Tk_Else";
    tokensMap[Tk_Break] = "Tk_Break";
    tokensMap[Tk_Continue] = "Tk_Continue";

    // Operators
    tokensMap[Tk_Equals] = "Tk_Equals";
    tokensMap[Tk_Colon] = "Tk_Colon";
    tokensMap[Tk_ColonColon] = "Tk_ColonColon";
    tokensMap[Tk_BackTick] = "Tk_BackTick";
    tokensMap[Tk_Period] = "Tk_Period";
    tokensMap[Tk_Comma] = "Tk_Comma";
    tokensMap[Tk_SemiColon] = "Tk_SemiColon";
    tokensMap[Tk_DoubleBackSlash] = "Tk_DoubleBackSlash";
    tokensMap[Tk_BackSlash] = "Tk_BackSlash";
    tokensMap[Tk_QuestionMark] = "Tk_QuestionMark";
    tokensMap[Tk_DoubleQuestionMark] = "Tk_DoubleQuestionMark";
    tokensMap[Tk_AtSign] = "Tk_AtSign";
    tokensMap[Tk_LeftArrow] = "Tk_LeftArrow";
    tokensMap[Tk_RightArrow] = "Tk_RightArrow";
    tokensMap[Tk_FatArrow] = "Tk_FatArrow";

    // Arithmetic Operators
    tokensMap[Tk_Asterisk] = "Tk_Asterisk";
    tokensMap[Tk_NormalSlash] = "Tk_NormalSlash";
    tokensMap[Tk_DoubleSlash] = "Tk_DoubleSlash";
    tokensMap[Tk_Modulus] = "Tk_Modulus";
    tokensMap[Tk_Plus] = "Tk_Plus";
    tokensMap[Tk_Minus] = "Tk_Minus";
    tokensMap[Tk_PlusPlus] = "Tk_PlusPlus";
    tokensMap[Tk_MinusMinus] = "Tk_MinusMinus";

    // Comparison Operators
    tokensMap[Tk_LessThan] = "Tk_LessThan";
    tokensMap[Tk_GreaterThan] = "Tk_GreaterThan";
    tokensMap[Tk_LessOrEquals] = "Tk_LessOrEquals";
    tokensMap[Tk_GreaterOrEquals] = "Tk_GreaterOrEquals";
    tokensMap[Tk_EqualsEquals] = "Tk_EqualsEquals";
    tokensMap[Tk_ExclaimEquals] = "Tk_ExclaimEquals";

    // Bitwise Operators
    tokensMap[Tk_Ambersand] = "Tk_Ambersand";
    tokensMap[Tk_VerticalBar] = "Tk_VerticalBar";
    tokensMap[Tk_Caret] = "Tk_Caret";
    tokensMap[Tk_Tilde] = "Tk_Tilde";
    tokensMap[Tk_LeftShift] = "Tk_LeftShift";
    tokensMap[Tk_RightShift] = "Tk_RightShift";

    // Logical Operators
    tokensMap[Tk_ExclamationMark] = "Tk_ExclamationMark";
    tokensMap[Tk_DoubleVerticalBar] = "Tk_DoubleVerticalBar";
    tokensMap[Tk_DoubleAmbersand] = "Tk_DoubleAmbersand";

    // Punctuators / Brackets
    tokensMap[Tk_OpenRoundBracket] = "Tk_OpenRoundBracket";
    tokensMap[Tk_CloseRoundBracket] = "Tk_CloseRoundBracket";
    tokensMap[Tk_OpenCurlyBracket] = "Tk_OpenCurlyBracket";
    tokensMap[Tk_CloseCurlyBracket] = "Tk_CloseCurlyBracket";
    tokensMap[Tk_OpenSquareBracket] = "Tk_OpenSquareBracket";
    tokensMap[Tk_CloseSquareBracket] = "Tk_CloseSquareBracket";
}
#endif // RE_DEBUG
#endif // TOKENKIND_H