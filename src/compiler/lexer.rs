use std::io::{BufRead};
use crate::compiler::token::{Token};
use crate::compiler::Compiler;

pub fn next(compiler: &mut Compiler) -> Token {
    let (character, consumed) = {
        let buffer = compiler.file.fill_buf()
            .expect("Error while fill_buf()");

        if buffer.is_empty() { return Token::Eof; }
        let mut consumed = 0;

        while consumed < buffer.len() && (buffer[consumed] as char).is_whitespace() {
            if buffer[consumed] as char == '\n' {
                compiler.row += 1;
                compiler.bol = 1;
            }

            consumed += 1;
        }

        if buffer.len() <= consumed { return Token::Eof; }
        (buffer[consumed] as char, consumed)
    };

    compiler.file.consume(consumed);

    if character == '#' {
        let buffer = compiler.file.fill_buf()
            .expect("Error while fill_buf() in character.is_alphabetic()");
        let mut length = 0;

        while length < buffer.len() &&
        (buffer[length] as char) != '\n' {
            length += 1;
        }

        compiler.file.consume(length + 1);
        return next(compiler);
    }

    else
    if character.is_alphabetic() {
        let buffer = compiler.file.fill_buf()
            .expect("Error while fill_buf() in character.is_alphabetic()");
        let mut length = 0;

        while length < buffer.len() &&
        (buffer[length] as char).is_alphanumeric() || character == '_' {
            length += 1;
        }

        let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
        compiler.file.consume(length);

        if compiler.types.contains(&lexeme) {
            return Token::TypeName(lexeme);
        }

        return Token::Identifier(lexeme);
    }

    else
    if character.is_digit(10) {
        let buffer = compiler.file.fill_buf()
            .expect("Error while fill_buf() in character.is_digit()");
        let mut length = 0;

        while length < buffer.len() &&
        ((buffer[length] as char).is_digit(10)) {
            length += 1;
        }

        let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
        compiler.file.consume(length);

        let int = lexeme.parse::<i64>()
            .expect("Error while typecasting to int in character.is_digit()");

        return Token::IntLiteral(int);
    }

    else
    if character == '"' {
        let buffer = compiler.file.fill_buf()
            .expect("Error while fill_buf() in character == '\"'");
        let mut length = 1;

        while length < buffer.len() &&
        ((buffer[length] as char) != '"') {
            length += 1;
        }

        length += 1;

        let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
        compiler.file.consume(length);

        return Token::StringLiteral(lexeme);
    }

    let token = match character {
        ';' => Token::SemiColon,

        '(' => Token::OpenRoundBracket,
        ')' => Token::CloseRoundBracket,

        '+' => Token::Plus,
        '-' => Token::Minus,

        _ => Token::Invalid
    };

    compiler.file.consume(1);
    token
}