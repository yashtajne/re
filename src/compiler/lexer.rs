use std::io::{BufRead};
use crate::compiler::token::{Token};
use crate::compiler::Compiler;


impl<R: BufRead> Compiler<R> {
    fn read_character(&mut self) -> Option<char> {
        let mut buffer = [0u8; 1];

        match self.file.read_exact(&mut buffer) {
            Ok(()) => {
                let character = buffer[0] as char;

                self.pos += 1;

                if character == '\n' {
                    self.row += 1;
                    self.bol = self.pos;
                }

                Some(character)
            },
            Err(_) => None,
        }
    }

    fn next_character(&mut self) -> Option<char> {
        if let Some(character) = self.peeked.take() {
            return Some(character);
        }

        self.read_character()
    }

    fn peek_character(&mut self) -> Option<char> {
        if self.peeked.is_none() {
            self.peeked = self.read_character();
        }

        self.peeked
    }

    pub fn next(&mut self) {
        while let Some(ch) = self.peek_character() {
            if !ch.is_whitespace() {
                break;
            }
            self.next_character();
        }

        let Some(character) = self.peek_character() else {
            let position = self.get_current_position();
            self.current_token = Token::Eof { row: position.0, col: position.1 };
            return;
        };

        if character == '#' {
            while let Some(ch) = self.next_character() {
                if ch == '\n' {
                    break;
                }
            }
            self.next();
            return;
        }

        if character.is_alphabetic() {
            let position = self.get_current_position();
            let mut lexeme = String::new();

            while let Some(ch) = self.peek_character() {
                if !(ch.is_alphanumeric() || ch == '_') {
                    break;
                }
                lexeme.push(self.next_character().unwrap());
            }

            if self.types.contains(&lexeme) {
                self.current_token = Token::TypeName{ row: position.0, col: position.1, value: lexeme };
                return;
            }

            self.current_token = Token::Identifier{ row: position.0, col: position.1, value: lexeme };
            return;
        }

        if character.is_ascii_digit() {
            let position = self.get_current_position();
            let mut lexeme = String::new();

            while let Some(ch) = self.peek_character() {
                if !ch.is_ascii_digit() {
                    break;
                }
                lexeme.push(self.next_character().unwrap());
            }

            let int = lexeme.parse::<i64>()
                .expect("Failed to parse IntLiteral!");

            self.current_token = Token::IntLiteral{ row: position.0, col: position.1, value: int };
            return;
        }

        if character == '"' {
            let position = self.get_current_position();
            let mut lexeme = String::new();

            self.next_character();

            while let Some(ch) = self.peek_character() {
                if ch == '"' {
                    break;
                }
                lexeme.push(self.next_character().unwrap());
            }

            self.next_character();

            self.current_token = Token::StringLiteral{ row: position.0, col: position.1, value: lexeme };
            return;
        }

        let position = self.get_current_position();
        self.current_token = match self.next_character().unwrap() {
            '(' => Token::OpenRoundBracket  { row: position.0, col: position.1 },
            ')' => Token::CloseRoundBracket { row: position.0, col: position.1 },

            '+' => Token::Plus  { row: position.0, col: position.1 },
            '-' => Token::Minus { row: position.0, col: position.1 },

            ';' => Token::SemiColon { row: position.0, col: position.1 },

            _ => Token::Invalid { row: position.0, col: position.1 }
        };
    }
}
