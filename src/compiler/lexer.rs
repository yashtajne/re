use std::io::{BufRead, Seek};
use crate::compiler::token::{Token};
use crate::compiler::Compiler;


impl Compiler {
    pub fn next(&mut self) -> Token {
        let start_pos = self.file.stream_position()
            .expect("Failed to get stream_position before lexing") as usize;

        let (character, consumed, newlines, offset, reached_eof) = {
            let buffer = self.file.fill_buf()
                .expect("Error while fill_buf()");

            if buffer.is_empty() {
                return Token::Eof;
            }

            let mut consumed = 0;
            let mut newlines = 0;
            let mut offset = Option::None;

            while consumed < buffer.len() && (buffer[consumed] as char).is_whitespace() {
                if buffer[consumed] as char == '\n' {
                    newlines += 1;
                    offset = Some(consumed);
                }

                consumed += 1;
            }

            if buffer.len() <= consumed {
                ('\0', consumed, newlines, offset, true)
            } else {
                (buffer[consumed] as char, consumed, newlines, offset, false)
            }
        };

        self.file.consume(consumed);
        self.row += newlines;

        if let Some(offset) = offset {
            self.bol = start_pos + offset + 1;
        }

        if reached_eof {
            return Token::Eof;
        }

        if character == '#' {
            let comment_start = self.file.stream_position()
                .expect("Failed to get stream_position at comment start") as usize;

            let (length, hit_newline) = {
                let buffer = self.file.fill_buf()
                    .expect("Error while fill_buf() in character == '#'");
                let mut length = 0;

                while length < buffer.len() &&
                (buffer[length] as char) != '\n' {
                    length += 1;
                }

                (length, length < buffer.len())
            };

            self.file.consume(length);
            if hit_newline {
                self.file.consume(1);
                self.row += 1;
                self.bol = comment_start + length + 1;
            }
            return self.next();
        }

        else
        if character.is_alphabetic() {
            let buffer = self.file.fill_buf()
                .expect("Error while fill_buf() in character.is_alphabetic()");
            let mut length = 0;

            while length < buffer.len() &&
            (buffer[length] as char).is_alphanumeric() || character == '_' {
                length += 1;
            }

            let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
            self.file.consume(length);

            if self.types.contains(&lexeme) {
                return Token::TypeName(lexeme);
            }

            return Token::Identifier(lexeme);
        }

        else
        if character.is_digit(10) {
            let buffer = self.file.fill_buf()
                .expect("Error while fill_buf() in character.is_digit()");
            let mut length = 0;

            while length < buffer.len() &&
            ((buffer[length] as char).is_digit(10)) {
                length += 1;
            }

            let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
            self.file.consume(length);

            let int = lexeme.parse::<i64>()
                .expect("Error while typecasting to int in character.is_digit()");

            return Token::IntLiteral(int);
        }

        else
        if character == '"' {
            let buffer = self.file.fill_buf()
                .expect("Error while fill_buf() in character == '\"'");
            let mut length = 1;

            while length < buffer.len() &&
            ((buffer[length] as char) != '"') {
                length += 1;
            }

            length += 1;

            let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
            self.file.consume(length);

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

        self.file.consume(1);
        token
    }
}
