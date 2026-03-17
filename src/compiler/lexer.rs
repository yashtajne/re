use std::io::{BufRead, Seek};
use crate::compiler::token::{Token};
use crate::compiler::Compiler;


impl Compiler {
    pub fn next(&mut self) {
        let start_pos = self.file.stream_position()
            .expect("Failed to get stream_position before lexing") as usize;

        let (character, consumed, newlines, offset, reached_eof) = {
            let buffer = self.file.fill_buf()
                .expect("Error while fill_buf()");

            if buffer.is_empty() {
                self.current_token = Token::Eof{ row: 0, col: 0 };
                return
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
            self.current_token = Token::Eof{ row: self.row, col: 0 };
            return
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
            (((buffer[length] as char).is_alphanumeric()) || (buffer[length] as char) == '_') {
                length += 1;
            }

            let lexeme = String::from_utf8_lossy(&buffer[..length]).to_string();
            self.file.consume(length);

            let (row, col) = self.get_current_position();
            if self.types.contains(&lexeme) {
                self.current_token = Token::TypeName{ row, col, value: lexeme };
                return
            } else {
                self.current_token = Token::Identifier{ row, col: 0, value: lexeme };
                return
            }
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

            let (row, col) = self.get_current_position();
            self.current_token = Token::IntLiteral{ row, col, value: int };
            return
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

            let (row, col) = self.get_current_position();
            self.current_token = Token::StringLiteral{ row, col, value: lexeme };
            return
        }

        let (row, col) = self.get_current_position();
        let token = match character {
            ';' => Token::SemiColon{ row, col },

            '(' => Token::OpenRoundBracket{ row, col },
            ')' => Token::CloseRoundBracket{ row, col },

            '+' => Token::Plus{ row, col },
            '-' => Token::Minus{ row, col },

            _ => Token::Invalid{ row, col }
        };

        self.file.consume(1);
        self.current_token = token;
        return
    }
}